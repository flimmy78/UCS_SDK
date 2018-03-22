#ifndef UPLUSLOGIN_H
#define UPLUSLOGIN_H

#include <qglobal.h>
#include <QObject>
#include <QFutureWatcher>
#include <QLineEdit>
#include <QCheckBox>
#include "BaseDialog.h"
#include "MyTitleBar.h"
#include "UPlusRestApi.h"
#include "HttpDownloadPicture.h"

class UPlusLogin : public QDialog
{
    Q_OBJECT

public:
    explicit UPlusLogin(QWidget *parent = 0);
    ~UPlusLogin();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void customEvent(QEvent *event) override;

private:
    void initLayout();
    void initConnections();
    void readSettings();

    void onLoginSuccess();

    // test begin
    void uploadHeaderImg();
    // test end
    static QString downloadHeadImg(const QString &userId);

private slots:
    void onBtnClosed();
    void onBtnMin();
    void onBtnLogin();
    void onCheckedChanged();

    void onLoginReply(QByteArray replyData, int code);
    void onReLoginReply(QByteArray replyData, int code);

    // test
    void onUploadHeaderImgReply(QByteArray replyData, int code);

private:
    MyTitleBar  *m_titleBar;
    QPushButton *m_pBtnLoginOn;
    QLineEdit   *m_pLineUserId;
    QLineEdit   *m_pLinePassword;
    QLabel      *m_pLoginTip;
    QCheckBox   *m_pChkKeepPwd;
    QCheckBox   *m_pChkOnLine;

    UPlusRestApi *m_pRestApi;
    bool m_doReLogin;
};

#endif // UPLUSLOGIN_H
