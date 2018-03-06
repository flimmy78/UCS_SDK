#include "midRightChatStackWidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include "AbsFiles/myIconStyle.h"
#include "Interface/UCSIMClient.h"
#include "Interface/UCSLogger.h"

#define TAG "ChatWidget"

MidRightChatStackWidget::MidRightChatStackWidget(QWidget *parent)
    : BaseWidget(parent)
    , m_txtSending(this)
    , m_btnSend(this)
{
    setObjectName("ChatStackWidget");

    initLayout();
    initTBtnMenu();
    initConnections();
    readSetting();
}

void MidRightChatStackWidget::initLayout()
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

    QListWidget *pListWid = new QListWidget(this);
    pListWid->setFrameShadow(QFrame::Plain);
    pListWid->setFrameShape(QFrame::NoFrame);

    pMainLayout->addWidget(pListWid);
    pMainLayout->addLayout(pToolsLayout);
    pMainLayout->addWidget(&m_txtSending);
    pMainLayout->addWidget(&m_btnSend, 0, Qt::AlignRight);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 2, 8, 8);

    setLayout(pMainLayout);
}

void MidRightChatStackWidget::initConnections()
{
    connect(&m_txtSending, SIGNAL(sig_msgSending()), this, SLOT(onSendingMsg()));
    connect(&m_btnSend, SIGNAL(clicked()), this, SLOT(onSendingMsg()));
}

void MidRightChatStackWidget::initTBtnMenu()
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

//    MyIconStyle *style = new MyIconStyle();
//    pMenu->setStyle(style);
//    pMenu->setStyleSheet(
//                "QMenu::indicator{width:25px; height: 25px;}"
//                "QMenu::indicator:exclusive:unchecked{image: url(:/images/midright/radiobutton_unchecked.png);}"
//                "QMenu::indicator:exclusive:checked{image: url(:/images/midright/radiobutton_checked.png);}");

    connect(pActionGrp, SIGNAL(triggered(QAction*)), this, SLOT(onUpdateSendAction(QAction*)));

    m_btnSend.setMenu(pMenu);
    m_btnSend.setPopupMode(QToolButton::MenuButtonPopup);
}

void MidRightChatStackWidget::saveSetting()
{
    QSettings setting("config.ini", QSettings::IniFormat, 0);
    setting.beginGroup("chatWid");
    setting.setValue("sendMode", m_txtSending.sendingMode());
    setting.endGroup();
}

void MidRightChatStackWidget::readSetting()
{
    QSettings setting("config.ini", QSettings::IniFormat, 0);
    setting.beginGroup("chatWid");
    int sendingMode = setting.value("sendMode", CTRL_ENTER_SEND).toInt();
    setting.endGroup();

    m_pAct[sendingMode]->setChecked(true);
    m_txtSending.setSendingMode((CustomSendingMode)sendingMode);
}

void MidRightChatStackWidget::onChangeConversation(QString targetId, quint32 type)
{
    UCS_LOG(UCSLogger::kTraceApiCall, TAG,
            QString("onChangeConversation targetId: %1 type: %2")
            .arg(targetId).arg(type));
}

void MidRightChatStackWidget::onSendingMsg()
{
    if (!m_txtSending.toPlainText().isEmpty())
    {
        qDebug() << "sending msg: " << m_txtSending.toPlainText();
        UCSMessage message;
        UCSTextMsg *txtMsg = new UCSTextMsg(m_txtSending.toPlainText());
        message.receivedId = "18018780872";
        message.conversationType = UCS_IM_SOLOCHAT;
        message.messageType = UCS_IM_TEXT;
        message.content = txtMsg;

        UCSIMClient::Instance()->sendMessage(&message);
        m_txtSending.clear();
    }
}

void MidRightChatStackWidget::onUpdateSendAction(QAction *pAction)
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
