﻿#ifndef UCSLOGIN_H
#define UCSLOGIN_H

#include <QObject>
#include <QWidget>
#include <qglobal.h>
#include <myPushButton.h>
#include <QLineEdit>
#include <QLabel>
#include <QDialog>
#include <rest/UCSRestManager.h>


class UCSLogin : public QDialog
{
    Q_OBJECT
public:
    explicit UCSLogin(QWidget *parent = 0);
    ~UCSLogin();

private:
    void initLayout();
    void initConnections();

protected:
    void timerEvent(QTimerEvent *event) override;

signals:

public slots:
    void slot_onClosed();
    void slot_onCodeReqClicked();
    void slot_onLoginClicked();

    void slot_onAuthSuccess(int expired);

    void slot_onLoginSuccess(QString token, QString nickname);

private:
    MyPushButton *m_pBtnClosed;
    MyPushButton *m_pBtnLoginOn;
    MyPushButton *m_pBtnCodeReq;
    QLineEdit    *m_pLineUserName;
    QLineEdit    *m_pLineVerifyCode;
    QLabel       *m_pLblHeaderImg;

    UCSRestManager m_restManager;
    qint32 m_timerId;
    qint32 m_count;
};

#endif // UCSLOGIN_H
