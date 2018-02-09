#ifndef TRAYICONMENU_H
#define TRAYICONMENU_H

#include <QMenu>
#include <QObject>
#include <QWidget>

class TrayIconMenu : public QMenu
{
    Q_OBJECT

public:
    TrayIconMenu(QWidget *parent = 0);

signals:
    void sig_close();
};

#endif // TRAYICONMENU_H
