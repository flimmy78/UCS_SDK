#include "IMChatWidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QtWebEngineWidgets>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEnginePage>
#include "Interface/UCSLogger.h"

IMChatWidget::IMChatWidget(QWidget *parent)
    : BaseWidget(parent)
    , m_txtSending(this)
    , m_btnSend(this)
{
    setObjectName("IMChatWidget");

    initLayout();
    initTBtnMenu();
    initConnections();
    readSetting();
}

IMChatWidget::~IMChatWidget()
{

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

    m_pWebView = new QWebEngineView(this);
    QWebEnginePage *page = new QWebEnginePage(this);
    m_pWebView->setPage(page);
//    page->setUrl(QUrl("http://www.hao123.com"));
    page->setUrl(QUrl("qrc:/Resources/Chat/messageBox.html"));
//    m_pWebView->resize(this->width(), m_pWebView->height());

    pMainLayout->addWidget(m_pWebView);

    pMainLayout->addLayout(pToolsLayout);
    pMainLayout->addWidget(&m_txtSending);
    pMainLayout->addWidget(&m_btnSend, 0, Qt::AlignRight);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 2, 8, 8);

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

void IMChatWidget::onChangeConversation(QString targetId, quint32 type)
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onChangeConversation targetId: %1 type: %2")
            .arg(targetId).arg(type));
    m_targetId = targetId;
    m_type = type;
}

void IMChatWidget::onConversationDeleted()
{

}

void IMChatWidget::onSendingMsg()
{
    if (!m_txtSending.toPlainText().isEmpty())
    {
        qDebug() << "sending msg: " << m_txtSending.toPlainText();
        UCSMessage message;
        UCSTextMsg *txtMsg = new UCSTextMsg(m_txtSending.toPlainText().trimmed());
        message.receivedId = m_targetId;
        message.conversationType = (UCS_IM_ConversationType)(m_type);
        message.messageType = UCS_IM_TEXT;
        message.content = txtMsg;

        UCSIMClient::Instance()->sendMessage(&message);


//        QString html = QString("appendMyMessage(%1, %2);scrollBottom();")
//                            .arg("'haha'")
//                            .arg("'fafsdfasdfa'");
//        m_pWebView->page()->runJavaScript(html);

        QString MyHead = QString("<img src=qrc:/images/u1183.png width='30px' heigth='30px'/>");
        QString html = QString("document.getElementById(\"content\").insertAdjacentHTML(\"beforeEnd\",\"<div style='overflow:hidden;'><p class='divotherHead'>%1 </p><p class='triangle-left left'>%2</p></div>\")")
                                .arg(MyHead)
                                .arg(m_txtSending.toPlainText().replace("\n", "</br>"));
        m_pWebView->page()->runJavaScript(html);

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
