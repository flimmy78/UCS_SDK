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

IMChatWidget::IMChatWidget(QWidget *parent)
    : BaseWidget(parent)
    , m_txtSending(this)
    , m_btnSend(this)
    , m_pWebChannel(Q_NULLPTR)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setObjectName("IMChatWidget");

    m_chatsMap.clear();
    m_msgIdList.clear();

    initLayout();
    initTBtnMenu();
    initConnections();
    readSetting();

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

    if (m_conversationId.isEmpty())
    {
        return;
    }

    int count = messageCount[m_conversationId];
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
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    QHBoxLayout *pToolsLayout = new QHBoxLayout;

    MyPushButton *emoji = new MyPushButton(this);
    MyPushButton *shot = new MyPushButton(this);
    MyPushButton *floder = new MyPushButton(this);
    emoji->setFixedSize(25, 25);
    shot->setFixedSize(25, 25);
    floder->setFixedSize(25, 25);
    emoji->setStyleSheet("QPushButton{border-image:url(:/images/midright/u4383.png)}");
    shot->setStyleSheet("QPushButton{border-image:url(:/images/midright/u4379.png)}");
    floder->setStyleSheet("QPushButton{border-image:url(:/images/midright/u4381.png)}");
    pToolsLayout->addWidget(emoji);
    pToolsLayout->addWidget(shot);
    pToolsLayout->addWidget(floder);
    pToolsLayout->addStretch();
    pToolsLayout->setSpacing(8);
    pToolsLayout->setContentsMargins(8, 4, 0, 4);

    m_txtSending.setFixedHeight(100);
    m_txtSending.setStyleSheet("QTextEdit{color: green; font-size:11pt; font-family: 微软雅黑;}");

    m_btnSend.setText(QStringLiteral("发送"));
    m_btnSend.setFixedSize(65, 30);

    m_pStackedLayout = new QStackedLayout;
    pMainLayout->addLayout(m_pStackedLayout);

    pMainLayout->addLayout(pToolsLayout);
    pMainLayout->addWidget(&m_txtSending);
    pMainLayout->addWidget(&m_btnSend, 0, Qt::AlignRight);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(2, 2, 2, 2);

    setLayout(pMainLayout);
}

void IMChatWidget::initConnections()
{
    connect(&m_txtSending, SIGNAL(sig_msgSending()), this, SLOT(onSendingMsg()));
    connect(&m_btnSend, SIGNAL(clicked()), this, SLOT(onSendingMsg()));
}

void IMChatWidget::initTBtnMenu()
{
    QMenu *pMenu = new QMenu(this);
    QActionGroup *pActionGrp = new QActionGroup(this);
    pActionGrp->setExclusive(true);

    m_pAct[0] = new QAction(QStringLiteral("按Enter键发送消息"), pActionGrp);
    m_pAct[0]->setCheckable(true);

    m_pAct[1] = new QAction(QStringLiteral("按Ctrl+Enter键发送消息"), pActionGrp);
    m_pAct[1]->setCheckable(true);
    m_pAct[1]->setChecked(true);

    pMenu->addAction(m_pAct[0]);
    pMenu->addAction(m_pAct[1]);

    connect(pActionGrp, SIGNAL(triggered(QAction*)), this, SLOT(onUpdateSendAction(QAction*)));

    m_btnSend.setMenu(pMenu);
    m_btnSend.setPopupMode(QToolButton::MenuButtonPopup);
}

void IMChatWidget::saveSetting()
{
    QSettings setting("config.ini", QSettings::IniFormat, 0);
    setting.beginGroup("chatWid");
    setting.setValue("sendMode", m_txtSending.sendingMode());
    setting.endGroup();
}

void IMChatWidget::readSetting()
{
    QSettings setting("config.ini", QSettings::IniFormat, 0);
    setting.beginGroup("chatWid");
    int sendingMode = setting.value("sendMode", CTRL_ENTER_SEND).toInt();
    setting.endGroup();

    m_pAct[sendingMode]->setChecked(true);
    m_txtSending.setSendingMode((CustomSendingMode)sendingMode);
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
    if (m_chatsMap.contains(m_conversationId))
    {
        ChatWebView *webView = m_chatsMap[m_conversationId];
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

    m_chatsMap.insert(m_conversationId, webView);
#endif
}

void IMChatWidget::showMessages(int count)
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(), "showMessages");
    if (m_conversationId.isEmpty())
    {
        return;
    }

    ChatWebView *webView = m_chatsMap[m_conversationId];
    if (webView == Q_NULLPTR || !webView->isLoadFinished())
    {
        return;
    }

    UCS_LOG(UCSLogger::kTraceInfo, this->objectName(), "showMessages");
    QList<UCSMessage*> messageList;
    messageList = UCSIMClient::Instance()->getLatestMessages((UCS_IM_ConversationType)m_conversationType, m_conversationId, count);
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

void IMChatWidget::onChangeConversation(QString targetId, quint32 type)
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onChangeConversation targetId: %1 type: %2")
            .arg(targetId).arg(type));

    m_conversationId = targetId;
    m_conversationType = type;

    if (m_conversationId.isEmpty())
    {
        return;
    }

    changeChat();
    showMessages();
}

void IMChatWidget::onConversationDeleted(QString targetId, quint32 type)
{
    if (m_chatsMap.contains(targetId))
    {
        ChatWebView *webView = m_chatsMap.take(targetId);
        m_pStackedLayout->removeWidget(webView);
        webView->deleteLater();
    }
}

void IMChatWidget::onSendingMsg()
{
    if (!m_txtSending.toPlainText().isEmpty())
    {
        qDebug() << "sending msg: " << m_txtSending.toPlainText();
        UCSMessage message;
        UCSTextMsg *txtMsg = new UCSTextMsg(m_txtSending.toPlainText().trimmed());
        message.receivedId = m_conversationId;
        message.conversationType = (UCS_IM_ConversationType)(m_conversationType);
        message.messageType = UCS_IM_TEXT;
        message.content = txtMsg;

        UCSIMClient::Instance()->sendMessage(&message);

        ChatMsgModel msg = MsgConvert::convert2Model(&message);
//        m_pChatWebView->sendMsgShow(msg);
        ChatWebView *webView = m_chatsMap[m_conversationId];
        webView->msgShow(msg);

        m_txtSending.clear();
        emit sendingNewMsg();
    }
}

void IMChatWidget::onUpdateSendAction(QAction *pAction)
{

    if (pAction == m_pAct[0])
    {
        m_txtSending.setSendingMode(ENTER_SEND);
    }
    else
    {
        m_txtSending.setSendingMode(CTRL_ENTER_SEND);
    }

    saveSetting();
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
