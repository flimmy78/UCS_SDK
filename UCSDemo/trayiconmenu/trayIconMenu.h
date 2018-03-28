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
    void sigSetting();
    void sigFeedBack();
    void sigAbout();
    void sigHelp();
    void sigClose();
};

#endif // TRAYICONMENU_H
