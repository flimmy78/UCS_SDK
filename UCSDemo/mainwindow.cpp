#include "mainwindow.h"

#include <QHBoxLayout>
#include <myPushButton.h>
#include <QDebug>
#include <QLineEdit>
#include <QString>
#include <QRegExpValidator>
#include <QTextEdit>
#include "Interface/UCSTcpSDK.h"
#include "Interface/UCSIMSDKPublic.h"
#include "CommonHelper.h"

MainWindow *MainWindow::s_pMainWnd = NULL;

MainWindow *MainWindow::InitInstance()
{
    if (!s_pMainWnd)
    {
        s_pMainWnd = new MainWindow;
    }

    return s_pMainWnd;
}

MainWindow::~MainWindow()
{
    qDebug() << "mainWindow ~dtor";

    UCSTcpClient::Instance()->unRegisterEventListener(kUCSConnectStatusEvent, this);
    UCSTcpClient::Instance()->unRegisterEventListener(kUCSLoginEvent, this);
}

MainWindow::MainWindow(QWidget *parent)
    : BaseWindow(parent)
{
    s_pMainWnd = this;
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setMinimumSize(964, 648);
    installEventFilter(this);

    /* 加载布局 */
    initLayout();

    initNetwork();
    initDatabase();

    /* 加载系统托盘菜单 */
    initTrayMenu();

    /* 信号与槽连接 */
    initConnections();

    initMisc();

    readSetting();

    CommonHelper::loadAppStyle(":/Resources/app.qss");
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (!this->isMaximized())
    {
        showMaximized();
        setGeometry(-4, -4, this->width() + 8, this->height() + 8);
    }
    else
    {
        showNormal();
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    return QObject::eventFilter(watched, event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
//    qDebug() << "closed event " << event->type();
    //    event->accept();
}

void MainWindow::customEvent(QEvent *event)
{
    switch (event->type()) {
    case kUCSLoginEvent:
    {
        UCSLoginEvent *loginEvt = (UCSLoginEvent*)event;
        if (loginEvt->error() == ErrorCode_NoError)
        {
            UCS_LOG(UCSLogger::kTraceInfo, "MainWindow",
                    QString("login success. userId = ").append(loginEvt->userId()));

            m_pImWidget->conversationListView()->updateConversationList();
        }
        else
        {
            UCS_LOG(UCSLogger::kTraceError, "MainWindow",
                    QString("login failed. error = %1").arg(loginEvt->error()));
        }
    }
        break;

    case kUCSConnectStatusEvent:
    {
        UCSConnectStatusEvent *connEvt = (UCSConnectStatusEvent*)event;
        if (connEvt->status() == UCSConnectionStatus_ConnectFail)
        {
            qDebug() << "connect status changed, status = " << connEvt->status();
        }
    }
        break;

    case kUCSMsgSendEvent:
    {
        UCSMsgSendEvent *sendEvent = (UCSMsgSendEvent*)event;
        if (sendEvent->error() == ErrorCode_NoError)
        {
            UCS_LOG(UCSLogger::kTraceInfo, "MainWindow",
                    QString("msgId: %1 success send.").arg(sendEvent->messageId()));
        }
        else
        {
            UCS_LOG(UCSLogger::kTraceError, "MainWindow",
                    QString("msgId: %1 failed send.").arg(sendEvent->messageId()));
        }
    }
        break;

    case kUCSDiscussionEvent:
    {

    }
        break;

    case kUCSVoiceDownloadEvent:
    {

    }
        break;

    case kUCSMsgSyncEvent:
    {

    }
        break;

    default:
        break;
    }
}

void MainWindow::initLayout()
{
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);

    m_pLeftNavBar = new LeftNavigatorBarWidget(this);
    pMainLayout->addWidget(m_pLeftNavBar);

    m_pStackedLayout = new QStackedLayout;

    m_pCallWidget = new CallWidget(this);
    m_pContactWidget = new ContactWidget(this);
    m_pImWidget = new IMWidget(this);

    m_pStackedLayout->addWidget(m_pCallWidget);
    m_pStackedLayout->addWidget(m_pContactWidget);
    m_pStackedLayout->addWidget(m_pImWidget);

    pMainLayout->addLayout(m_pStackedLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(4, 4, 4, 4);
}

void MainWindow::initTrayMenu()
{
    m_system_tray.setContextMenu(&m_traymenu);
    m_system_tray.setToolTip(QString("test"));
    m_system_tray.setIcon(QIcon(":/images/yzx_logo.ico"));
    m_system_tray.show();

    connect(&m_system_tray,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(slot_iconIsActived(QSystemTrayIcon::ActivationReason)));

    connect(&m_traymenu,
            SIGNAL(sig_close()),
            this,
            SLOT(slot_quitApp()));
}

void MainWindow::initConnections()
{
    for (int i = 0; i < 3; i++)
    {
        connect(m_pLeftNavBar->m_pBtn[i], SIGNAL(pressed()), this, SLOT(onSwitchPage()));
    }
}

void MainWindow::initMisc()
{
    UCSIMClient::Instance()->init();

    UCSTcpClient::Instance()->registerEventListener(kUCSConnectStatusEvent, this);
    UCSTcpClient::Instance()->registerEventListener(kUCSLoginEvent, this);

    UCSIMClient::Instance()->setIMReceiver(this);
}

void MainWindow::initNetwork()
{

}

void MainWindow::initDatabase()
{

}

void MainWindow::readSetting()
{

}

void MainWindow::saveSetting()
{

}

void MainWindow::slot_iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    {
        showNormal();
        activateWindow();
    }
        break;
    default:
        break;
    }
}

void MainWindow::slot_quitApp()
{
    m_system_tray.hide();
    close();
}

void MainWindow::onSwitchPage()
{
    StackButton *stackBtn = (StackButton*)sender();
    m_pStackedLayout->setCurrentIndex(stackBtn->index());
    m_pLeftNavBar->onChangeButtonSelected(stackBtn->index());
}


