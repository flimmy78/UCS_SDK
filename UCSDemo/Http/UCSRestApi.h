#ifndef UCSRESTAPI_H
#define UCSRESTAPI_H

#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

enum UCSRestError
{
    NoError = 0,
    AuthCodeExpired = 2001,
    AuthCodeInCorrect = 2002,
    PhoneNumberEmpty = 2003,
    AuthCodeEmpty = 2004,
    NickNameEmpty = 2005,
    NoDataError = 2006,
    CodeRequestFailed = 2007,
    RegisterUserFailed = 2008,
    InvalidParameters = 2009,
    PhoneNumberInExistent = 2011,
    PhoneNumberInCorrect = 2012,
    JoinGroupFailed = 2013,

    DataFormatInvalid = 2101,
    DoRequestFailed = 2102,
    UnknownError = 2099
};

class UCSRestApi : public QObject
{
    Q_OBJECT

public:
    explicit UCSRestApi();

    /* 根据手机号码获取验证码 */
    void getAuthCode(QString phone);

    /* 登录认证 */
    void login(QString phone, QString authCode);

    /* 查询昵称 */
    void getNickName(QString phone);

    QString errorString(UCSRestError error) const;

signals:

    void sig_onFailed(UCSRestError error);

    void sig_onAuthSuccessed(int expired);
    void sig_onLoginSuccessed(QString token, QString nickname);
    void sig_onGetNickName(QString nickname);

private slots:
    void slot_onAuthReply(QNetworkReply *reply);
    void slot_onLoginReply(QNetworkReply *reply);
    void slot_onGetNickName(QNetworkReply *reply);

private:
    QString m_baseUrl;
};

#endif // UCSRESTAPI_H
