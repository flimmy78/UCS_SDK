#include "IMChatWidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QtWebEngineWidgets>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEnginePage>
#include "Interface/UCSIMSDKPublic.h"
#include "Interface/UCSLogger.h"
#include "WebBridge.h"
#include "CommonHelper.h"

IMChatWidget::IMChatWidget(QWidget *parent)
    : BaseWidget(parent)
    , m_pWebChannel(Q_NULLPTR)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setObjectName("IMChatWidget");

    m_chatsMap.clear();
    m_msgIdList.clear();

    initLayout();
    initConnections();

    initMisc();
}

IMChatWidget::~IMChatWidget()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            "~dtor()");

//    m_chatsMap.clear();

    m_msgIdList.clear();
}

void IMChatWidget::doSyncMessages(QMap<QString, qint32> messageCount)
{
    createChat();

    if (m_conversation.conversationId.isEmpty())
    {
        return;
    }

    int count = messageCount[m_conversation.conversationId];
    if (count <= 0)
    {
        return;
    }

    showMessages(count);
}

void IMChatWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

}

void IMChatWidget::initMisc()
{
    if (m_pWebChannel == Q_NULLPTR)
    {
        m_pWebChannel = new QWebChannel(this);
        WebBridge *bridge = new WebBridge(this, this);
        m_pWebChannel->registerObject(QStringLiteral("bridge"), (QObject*)bridge);
    }
}

void IMChatWidget::initLayout()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    QHBoxLayout *pToolBox = new QHBoxLayout;
    QHBoxLayout *pSendBox = new QHBoxLayout;

    m_pStackedLayout = new QStackedLayout;
    m_pBtnEmoji = new MyPushButton(this);
    m_pBtnClear = new MyPushButton(this);
    m_pBtnImage = new MyPushButton(this);
    m_pBtnConf = new MyPushButton(this);
    m_pBtnDirect = new MyPushButton(this);
    m_pBtnAudio = new MyPushButton(this);
    m_pBtnVideo = new MyPushButton(this);
    m_pBtnSend = new MyPushButton(this);
    m_pTxtSending = new MyChatTextEdit(this);

    m_pBtnEmoji->setObjectName("btnChatEmoji");
    m_pBtnImage->setObjectName("btnChatImage");
    m_pBtnClear->setObjectName("btnChatClear");
    m_pBtnConf->setObjectName("btnChatConf");
    m_pBtnDirect->setObjectName("btnChatDirect");
    m_pBtnAudio->setObjectName("btnChatAudio");
    m_pBtnVideo->setObjectName("btnChatVideo");
    m_pBtnSend->setObjectName("btnChatSend");
    m_pTxtSending->setObjectName("txtChatSend");

    m_pBtnEmoji->setToolTip(QStringLiteral("选择表情"));
    m_pBtnImage->setToolTip(QStringLiteral("发送图片"));
    m_pBtnClear->setToolTip(QStringLiteral("清空消息"));
    m_pBtnConf->setToolTip(QStringLiteral("发起会议"));
    m_pBtnDirect->setToolTip(QStringLiteral("高清电话"));
    m_pBtnAudio->setToolTip(QStringLiteral("语音聊天"));
    m_pBtnVideo->setToolTip(QStringLiteral("视频聊天"));

    pToolBox->addWidget(m_pBtnEmoji);
    pToolBox->addWidget(m_pBtnImage);
    pToolBox->addWidget(m_pBtnClear);
    pToolBox->addStretch();
    pToolBox->addWidget(m_pBtnConf);
    pToolBox->addWidget(m_pBtnDirect);
    pToolBox->addWidget(m_pBtnAudio);
    pToolBox->addWidget(m_pBtnVideo);
    pToolBox->setSpacing(8);
    pToolBox->setContentsMargins(8, 4, 8, 4);

    m_pBtnSend->setText(QStringLiteral("发送"));
    pSendBox->addWidget(m_pBtnSend, 0, Qt::AlignRight);
    pSendBox->setContentsMargins(0, 0, 8, 0);

    pMainLayout->addLayout(m_pStackedLayout);
    pMainLayout->addLayout(pToolBox);
    pMainLayout->addWidget(m_pTxtSending);
    pMainLayout->addLayout(pSendBox);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(2, 2, 2, 8);
}

void IMChatWidget::initConnections()
{
    connect(m_pBtnEmoji, SIGNAL(clicked(bool)), this, SLOT(onOpenEmojiPanel()));
    connect(m_pBtnImage, SIGNAL(clicked(bool)), this, SLOT(onSendImage()));
    connect(m_pBtnClear, SIGNAL(clicked(bool)), this, SLOT(onClearMessage()));
    connect(m_pBtnConf, SIGNAL(clicked(bool)), this, SLOT(onStartConference()));
    connect(m_pBtnDirect, SIGNAL(clicked(bool)), this, SLOT(onStartDirectCall()));
    connect(m_pBtnAudio, SIGNAL(clicked(bool)), this, SLOT(onStartAudioCall()));
    connect(m_pBtnVideo, SIGNAL(clicked(bool)), this, SLOT(onStartVideoCall()));
    connect(m_pTxtSending, SIGNAL(sig_msgSending()), this, SLOT(onSendingMsg()));
    connect(m_pBtnSend, SIGNAL(clicked()), this, SLOT(onSendingMsg()));
}

void IMChatWidget::createChat()
{
    QList<UCSConversation*> conversationList =  UCSIMClient::Instance()->getConversationList(allChat);
    foreach (UCSConversation *conversation, conversationList)
    {
        QString conversationId = conversation->targetId();
        if (m_chatsMap.contains(conversationId))
        {
            continue;
        }
        else
        {
            ChatWebView *webView = new ChatWebView(this);
            webView->page()->setWebChannel(m_pWebChannel);
            m_pStackedLayout->addWidget(webView);
            m_chatsMap.insert(conversationId, webView);
        }
    }
}

void IMChatWidget::changeChat()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(), "changeChat");
    if (m_chatsMap.contains(m_conversation.conversationId))
    {
        ChatWebView *webView = m_chatsMap[m_conversation.conversationId];
        int index = m_pStackedLayout->indexOf(webView);
        m_pStackedLayout->setCurrentIndex(index);

        showMessages();
        return;
    }

#if 1
    UCS_LOG(UCSLogger::kTraceInfo, this->objectName(), "changeChat before");
    ChatWebView *webView = new ChatWebView(this);
    UCS_LOG(UCSLogger::kTraceInfo, this->objectName(), "changeChat after");
    webView->page()->setWebChannel(m_pWebChannel);

    connect(webView, SIGNAL(sigLoadFinished()), this, SLOT(onLoadFinished()));

    m_pStackedLayout->addWidget(webView);

    // Assume that the last added widget index equal to count() - 1
    m_pStackedLayout->setCurrentIndex(m_pStackedLayout->count() - 1);

    m_chatsMap.insert(m_conversation.conversationId, webView);
#endif
}

void IMChatWidget::showMessages(int count)
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(), "showMessages");
    if (m_conversation.conversationId.isEmpty())
    {
        return;
    }

    ChatWebView *webView = m_chatsMap[m_conversation.conversationId];
    if (webView == Q_NULLPTR || !webView->isLoadFinished())
    {
        return;
    }

    UCS_LOG(UCSLogger::kTraceInfo, this->objectName(), "showMessages");
    QList<UCSMessage*> messageList;
    messageList = UCSIMClient::Instance()->getLatestMessages((UCS_IM_ConversationType)m_conversation.conversationType,
                                                             m_conversation.conversationId,
                                                             count);
    int msgCount = messageList.size();
    for (int i = msgCount - 1; i >= 0; i--)
    {
        UCSMessage *message = messageList.at(i);
        // avoid repeat
        if (m_msgIdList.contains(message->messageId))
        {
            continue;
        }
        m_msgIdList.append(message->messageId);

        ChatMsgModel model = MsgConvert::convert2Model(message);

        webView->msgShow(model);
    }

    qDeleteAll(messageList);
    messageList.clear();
}

void IMChatWidget::onChangeConversation(ConversationItem conversation)
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onChangeConversation conversationId: %1 type: %2")
            .arg(conversation.conversationId).arg(conversation.conversationType));

    m_conversation = conversation;

    if (m_conversation.conversationId.isEmpty())
    {
        return;
    }

    changeChat();
    showMessages();
}

void IMChatWidget::onConversationDeleted(ConversationItem conversation)
{
    if (m_chatsMap.contains(conversation.conversationId))
    {
        ChatWebView *webView = m_chatsMap.take(conversation.conversationId);
        m_pStackedLayout->removeWidget(webView);
        webView->deleteLater();
    }
}

void IMChatWidget::onSendingMsg()
{
    if (!m_pTxtSending->toPlainText().isEmpty())
    {
        UCSMessage message;
        UCSTextMsg *txtMsg = new UCSTextMsg(m_pTxtSending->toPlainText().trimmed());
        message.receivedId = m_conversation.conversationId;
        message.conversationType = (UCS_IM_ConversationType)(m_conversation.conversationType);
        message.messageType = UCS_IM_TEXT;
        message.content = txtMsg;
        message.senderUserId = CommonHelper::readSetting(kSettingLoginUserId).toString();
        message.senderNickName = CommonHelper::readSetting(kSettingLoginUserName).toString();

        UCSIMClient::Instance()->sendMessage(&message);
        m_msgIdList.append(message.messageId);

        ChatMsgModel msg = MsgConvert::convert2Model(&message);
        ChatWebView *webView = m_chatsMap[m_conversation.conversationId];
        webView->msgShow(msg);

        m_pTxtSending->clear();
        emit sigSendingMsg();
    }
}

void IMChatWidget::urlChanged(const QUrl &url)
{
    UCS_LOG(UCSLogger::kTraceInfo, this->objectName(),
            "urlChanged: url = " + url.url());
}

void IMChatWidget::onLoadFinished()
{
    UCS_LOG(UCSLogger::kTraceInfo, this->objectName(),
            "onLoadFinished");
    showMessages();
}

void IMChatWidget::onOpenEmojiPanel()
{

}

void IMChatWidget::onSendImage()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    QStringLiteral("选择图片"),
                                                    ".",
                                                    "*.png *.jpg *.jpeg");

    UCS_LOG(UCSLogger::kTraceInfo, this->objectName(),
            QString("onSendImage: path = %1").arg(filePath));
}

void IMChatWidget::onClearMessage()
{

}

void IMChatWidget::onStartConference()
{

}

void IMChatWidget::onStartDirectCall()
{

}

void IMChatWidget::onStartAudioCall()
{

}

void IMChatWidget::onStartVideoCall()
{

}
