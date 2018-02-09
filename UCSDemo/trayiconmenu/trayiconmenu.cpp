#include "trayIconMenu.h"

TrayIconMenu::TrayIconMenu(QWidget *parent)
    :QMenu(parent)
{
    setFixedWidth(200);
    setContentsMargins(0, 0, 0, 0);

    QAction* act_level4 = new QAction(QIcon(":/images/yzx_logo.ico"), tr("Exit"), this);
    addAction(act_level4);

    QAction* act_level3 = new QAction(QIcon(":/images/yzx_logo.ico"), tr("Exit"), this);
    addAction(act_level3);

    addSeparator();

    QAction* act_level2 = new QAction(QIcon(":/images/yzx_logo.ico"), QStringLiteral("选项设置"), this);
    addAction(act_level2);

    QAction* act_level1 = new QAction(QIcon(":/images/yzx_logo.ico"), tr("Settings"), this);
    addAction(act_level1);

    addSeparator();

    QAction* act_close = new QAction(QIcon(":/images/yzx_logo.ico"), QStringLiteral("退出"), this);
    addAction(act_close);
    connect(act_close, SIGNAL(triggered()), this, SIGNAL(sig_close()));
}
