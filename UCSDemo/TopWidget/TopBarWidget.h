#ifndef TOPWIDGETS_H
#define TOPWIDGETS_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QToolButton>
#include "myPushButton.h"
#include "myLabel.h"
#include "BaseWidget.h"

class TopBarWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit TopBarWidget(QWidget *parent = 0);

    void setTitle(QString title);
    void updateTopBar();

private:
    void initLayout();
    void initConnection();

protected:


signals:

public slots:


private slots:
    void onBtnMinClicked();
    void onBtnRestoreClicked();
    void onBtnMaxClicked();
    void onBtnCloseClicked();
    void onBtnGroupClicked();
    void onBtnConfClicked();
    void onBtnHeaderClicked();
    void onBtnDropDownClicked();
    void onActionModPhone();
    void onActionModPwd();

private:
    MyPushButton *m_pBtnClose;
    MyPushButton *m_pBtnMin;
    MyPushButton *m_pBtnMax;
    MyPushButton *m_pBtnRestore;
    MyPushButton *m_pBtnGroup;
    MyPushButton *m_pBtnConf;
    QPushButton *m_pBtnHeader;
    MyPushButton *m_pBtnDropDown;

    MyLabel m_lblTitle;
};

#endif // TOPWIDGETS_H
