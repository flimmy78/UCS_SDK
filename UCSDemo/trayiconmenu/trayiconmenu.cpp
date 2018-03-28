#include "trayIconMenu.h"

TrayIconMenu::TrayIconMenu(QWidget *parent)
    :QMenu(parent)
{
    setObjectName("TrayIconMenu");

    QAction* pActSetting = new QAction(QStringLiteral("设置"), this);
    addAction(pActSetting);

    QAction* pActFeedBack = new QAction(QStringLiteral("意见反馈"), this);
    addAction(pActFeedBack);

    QAction* pActHelp = new QAction(QStringLiteral("帮助"), this);
    addAction(pActHelp);

    QAction* pActAbout = new QAction(QStringLiteral("关于U+"), this);
    addAction(pActAbout);

    QAction* pActClose = new QAction(QStringLiteral("退出"), this);
    addAction(pActClose);

    connect(pActSetting, SIGNAL(triggered(bool)), this, SIGNAL(sigSetting()));
    connect(pActFeedBack, SIGNAL(triggered(bool)), this, SIGNAL(sigFeedBack()));
    connect(pActHelp, SIGNAL(triggered(bool)), this, SIGNAL(sigHelp()));
    connect(pActAbout, SIGNAL(triggered(bool)), this, SIGNAL(sigAbout()));
    connect(pActClose, SIGNAL(triggered()), this, SIGNAL(sigClose()));
}
