#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSystemTrayIcon>
#include "baseWindow.h"
#include "trayiconmenu/trayIconMenu.h"
#include "leftWidget/leftWidget.h"
#include "topWidget/topWidget.h"
#include "middleWidget/middleLeftWidget.h"
#include "middleWidget/middleRightWidget.h"

class MainWindow : public BaseWindow
{
    Q_OBJECT

public:
    static MainWindow* InitInstance();
    static MainWindow *GetInstance() { return s_pMainWnd; }

    ~MainWindow();

protected:
    static MainWindow* s_pMainWnd;
    explicit MainWindow(QWidget *parent = 0);

    void mouseDoubleClickEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void customEvent(QEvent *event) override;

private:
    void initLayout();
    void initTrayMenu();
    void initConnection();
    void initMisc();
    void initNetwork();
    void initDatabase();

    void readSetting();
    void saveSetting();

public slots:    
    void slot_iconIsActived(QSystemTrayIcon::ActivationReason);
    void slot_quitApp();

private:
    QSystemTrayIcon m_system_tray;
    TrayIconMenu m_traymenu;
    LeftWidget m_leftWid;
    MiddleLeftWidget m_midLeft;
    MiddleRightWidget m_midRight;

};

#endif // MAINWINDOW_H
