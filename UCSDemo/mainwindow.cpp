#include "mainwindow.h"

#include <QHBoxLayout>
#include <myPushButton.h>
#include <QDebug>
#include <QLineEdit>
#include <QString>
#include <QRegExpValidator>
#include <QTextEdit>

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

}

MainWindow::MainWindow(QWidget *parent)
    : BaseWindow(parent)
    , m_leftWid(this, 94)
    , m_midLeft(this, 250)
    , m_midRight(this, 612)
{
    s_pMainWnd = this;
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setMinimumSize(964, 648);
    installEventFilter(this);

    m_midLeft.setMidRightWidget(&m_midRight);

    /* 加载布局 */
    initLayout();

    initNetwork();
    initDatabase();

    /* 加载系统托盘菜单 */
    initTrayMenu();

    /* 信号与槽连接 */
    initConnection();

    initMisc();

    readSetting();
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
//    qDebug() << "closed event " << event->type();
//    event->accept();
}

void MainWindow::initLayout()
{
    QHBoxLayout *pMainLayout = new QHBoxLayout;

    pMainLayout->addWidget(&m_leftWid);
    pMainLayout->addWidget(&m_midLeft);
    pMainLayout->addWidget(&m_midRight);

    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(4, 4, 4, 4);
    setLayout(pMainLayout);
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

void MainWindow::initConnection()
{
    for (int i = 0; i < 3; i++)
    {
        connect(m_leftWid.m_pBtn[i], SIGNAL(pressed()), &m_midLeft, SLOT(slot_switchStack()));
        connect(m_leftWid.m_pBtn[i], SIGNAL(pressed()), &m_midRight, SLOT(slot_switchStack()));
    }
    connect(&m_midLeft.m_stackWid, SIGNAL(currentChanged(int)), &m_leftWid, SLOT(slot_changeButtonSelected(int)));

    connect(&m_midLeft.m_imMsgSWid, SIGNAL(sig_itemClicked(QString)), &m_midRight, SLOT(slot_setTitle(QString)));
}

void MainWindow::initMisc()
{

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


