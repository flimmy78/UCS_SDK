#ifndef UCSLOGIN_H
#define UCSLOGIN_H

#include <QObject>
#include <QWidget>
#include <qglobal.h>
#include <myPushButton.h>
#include <QLineEdit>
#include <QLabel>
//#include <QDialog>
#include "BaseDialog.h"
#include "RestManager/UCSRestManager.h"
#include "absFiles/MyTitleBar.h"

class UCSLogin : public BaseDialog
{
    Q_OBJECT
public:
    explicit UCSLogin(QWidget *parent = 0);
    ~UCSLogin();

private:
    void initLayout();
    void initConnections();

    void onLoginFailed();

protected:
    void timerEvent(QTimerEvent *event) override;
    void customEvent(QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:
    void slot_onBtnClose();
    void slot_onBtnMin();
    void slot_onCodeReqClicked();
    void slot_onLoginClicked();

    void slot_onAuthSuccess(int expired);
    void slot_onLoginSuccess(QString token, QString nickname);
    void slot_onRestFailed(UCSRestError error);

private:
    MyTitleBar   *m_titleBar;
    MyPushButton *m_pBtnLoginOn;
    MyPushButton *m_pBtnCodeReq;
    QLineEdit    *m_pLineUserName;
    QLineEdit    *m_pLineVerifyCode;
    QLabel       *m_pLblHeaderImg;

    UCSRestManager m_restManager;
    qint32 m_timerId;
    qint32 m_countDown;
    bool m_isLoggin;
};

#endif // UCSLOGIN_H
