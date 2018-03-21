#include "UPlusRestApi.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>

UPlusRestApi::UPlusRestApi()
{
    m_uplusUrl = "https://uplus.uc.ucpaas.com/";
}

void UPlusRestApi::doSwitchEnv(bool isOnLine)
{
    if (isOnLine)
    {
        m_uplusUrl = "https://uplus.uc.ucpaas.com/";
    }
    else
    {
        m_uplusUrl = "http://106.15.37.221:8666/";
    }
}

void UPlusRestApi::doLogin(QString userId, QString pwd)
{
    QUrl url(m_uplusUrl + "user/login");

    QByteArray pwdHash = QCryptographicHash::hash(pwd.toLocal8Bit(), QCryptographicHash::Md5);

    QJsonObject jsonObj;
    QJsonDocument document;
    jsonObj.insert("userid", userId);
    jsonObj.insert("pwd", QString(pwdHash.toHex()).toUpper());
    document.setObject(jsonObj);

    QByteArray data = document.toJson(QJsonDocument::Compact);

    HttpRequest httpRequest(url);
    httpRequest.setContentLength(data.length());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onLoginReply(QNetworkReply*)));
    manager->post(httpRequest.request(), data);
}

void UPlusRestApi::doReLogin(QString userId, QString pwd)
{
    QUrl url(m_uplusUrl + "user/relogin");

    QByteArray pwdHash = QCryptographicHash::hash(pwd.toLocal8Bit(), QCryptographicHash::Md5);

    QJsonObject jsonObj;
    QJsonDocument document;
    jsonObj.insert("userid", userId);
    jsonObj.insert("pwd", QString(pwdHash.toHex()).toUpper());
    document.setObject(jsonObj);
    QByteArray data = document.toJson(QJsonDocument::Compact);

    HttpRequest httpRequest(url);
    httpRequest.setContentLength(data.length());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReLoginReply(QNetworkReply*)));
    manager->post(httpRequest.request(), data);
}

void UPlusRestApi::doResetPwd(QString userId, QString pwd, QString newPwd)
{
    QUrl url(m_uplusUrl + "user/rstpwd");

    QByteArray pwdHash = QCryptographicHash::hash(pwd.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray newPwdHash = QCryptographicHash::hash(newPwd.toLocal8Bit(), QCryptographicHash::Md5);

    QJsonObject jsonObj;
    QJsonDocument document;
    jsonObj.insert("userid", userId);
    jsonObj.insert("pwd", QString(pwdHash.toHex()).toUpper());
    jsonObj.insert("newpwd", QString(newPwdHash.toHex()).toUpper());
    document.setObject(jsonObj);
    QByteArray data = document.toJson(QJsonDocument::Compact);

    HttpRequest httpRequest(url);
    httpRequest.setContentLength(data.length());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onResetPwdReply(QNetworkReply*)));
    manager->post(httpRequest.request(), data);
}

void UPlusRestApi::doUploadHeaderImg(QString userId, QString pwd,
                                     QByteArray imgData, QString imgType)
{
    QUrl url(m_uplusUrl + "user/uploadheadimg");

    QByteArray pwdHash = QCryptographicHash::hash(pwd.toLocal8Bit(), QCryptographicHash::Md5);

    QJsonObject jsonObj;
    QJsonDocument document;
    jsonObj.insert("userid", userId);
    jsonObj.insert("pwd", QString(pwdHash.toHex()).toUpper());
    jsonObj.insert("headimg", QString(imgData.toBase64()));
    jsonObj.insert("imgType", imgType);
    document.setObject(jsonObj);
    QByteArray data = document.toJson(QJsonDocument::Compact);

    HttpRequest httpRequest(url);
    httpRequest.setContentLength(data.length());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onUploadHeaderImgReply(QNetworkReply*)));
    manager->post(httpRequest.request(), data);
}

void UPlusRestApi::doGetContacts(QString userId, QString pwd)
{
    if (userId.isEmpty() || pwd.isEmpty())
    {
        return;
    }
    QUrl url(m_uplusUrl + "service/getcontacts");

    QByteArray pwdHash = QCryptographicHash::hash(pwd.toLocal8Bit(), QCryptographicHash::Md5);

    QJsonObject jsonObj;
    QJsonDocument document;
    jsonObj.insert("userid", userId);
    jsonObj.insert("pwd", QString(pwdHash.toHex()).toUpper());
    document.setObject(jsonObj);
    QByteArray data = document.toJson(QJsonDocument::Compact);

    HttpRequest httpRequest(url);
    httpRequest.setContentLength(data.length());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onGetContactsReply(QNetworkReply*)));
    manager->post(httpRequest.request(), data);
}

void UPlusRestApi::doCheckVersion(QString userId, QString pwd, qint32 platform, QString version)
{
    QUrl url(m_uplusUrl + "service/checkver");

    QByteArray pwdHash = QCryptographicHash::hash(pwd.toLocal8Bit(), QCryptographicHash::Md5);

    QJsonObject jsonObj;
    QJsonDocument document;
    jsonObj.insert("userid", userId);
    jsonObj.insert("pwd", QString(pwdHash.toHex()).toUpper());
    jsonObj.insert("type", platform);
    jsonObj.insert("ver", version);
    document.setObject(jsonObj);
    QByteArray data = document.toJson(QJsonDocument::Compact);

    HttpRequest httpRequest(url);
    httpRequest.setContentLength(data.length());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onCheckVersionReply(QNetworkReply*)));
    manager->post(httpRequest.request(), data);
}

void UPlusRestApi::doQueryFriendInfo(QString userId, QString pwd, QString friendId)
{
    QUrl url(m_uplusUrl + "service/queryfriendInfo");

    QByteArray pwdHash = QCryptographicHash::hash(pwd.toLocal8Bit(), QCryptographicHash::Md5);

    QJsonObject jsonObj;
    QJsonDocument document;
    jsonObj.insert("userid", userId);
    jsonObj.insert("pwd", QString(pwdHash.toHex()).toUpper());
    jsonObj.insert("friendId", friendId);
    document.setObject(jsonObj);

    QByteArray data = document.toJson(QJsonDocument::Compact);

    HttpRequest httpRequest(url);
    httpRequest.setContentLength(data.length());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onQueryFriendInfoReply(QNetworkReply*)));
    manager->post(httpRequest.request(), data);
}

void UPlusRestApi::doUploadFeedBack(QString userId, QString pwd,
                                    QString content, QString contact,
                                    qint32 platform)
{
    QUrl url(m_uplusUrl + "user/feedback");

    QByteArray pwdHash = QCryptographicHash::hash(pwd.toLocal8Bit(), QCryptographicHash::Md5);

    QJsonObject jsonObj;
    QJsonDocument document;
    jsonObj.insert("userid", userId);
    jsonObj.insert("pwd", QString(pwdHash.toHex()).toUpper());
    jsonObj.insert("content", content);
    jsonObj.insert("number", contact);
    jsonObj.insert("type", platform);
    document.setObject(jsonObj);

    QByteArray data = document.toJson(QJsonDocument::Compact);

    HttpRequest httpRequest(url);
    httpRequest.setContentLength(data.length());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onUploadFeedBackReply(QNetworkReply*)));
    manager->post(httpRequest.request(), data);
}

void UPlusRestApi::onLoginReply(QNetworkReply *reply)
{
    HttpReply httpReply(reply);
    emit sigOnLoginReply(httpReply.data(), httpReply.code());

    reply->deleteLater();
}

void UPlusRestApi::onReLoginReply(QNetworkReply *reply)
{
    HttpReply httpReply(reply);
    emit sigOnReLoginReply(httpReply.data(), httpReply.code());

    reply->deleteLater();
}

void UPlusRestApi::onResetPwdReply(QNetworkReply *reply)
{
    HttpReply httpReply(reply);
    emit sigOnResetPwdReply(httpReply.data(), httpReply.code());

    reply->deleteLater();
}

void UPlusRestApi::onUploadHeaderImgReply(QNetworkReply *reply)
{
    HttpReply httpReply(reply);
    emit sigOnUploadHeaderImgReply(httpReply.data(), httpReply.code());

    reply->deleteLater();
}

void UPlusRestApi::onGetContactsReply(QNetworkReply *reply)
{
    HttpReply httpReply(reply);
    emit sigOnGetContactsReply(httpReply.data(), httpReply.code());

    reply->deleteLater();
}

void UPlusRestApi::onCheckVersionReply(QNetworkReply *reply)
{
    HttpReply httpReply(reply);
    emit sigOnCheckVersionReply(httpReply.data(), httpReply.code());

    reply->deleteLater();
}

void UPlusRestApi::onQueryFriendInfoReply(QNetworkReply *reply)
{
    HttpReply httpReply(reply);
    emit sigOnQueryFriendInfoReply(httpReply.data(), httpReply.code());

    reply->deleteLater();
}

void UPlusRestApi::onUploadFeedBackReply(QNetworkReply *reply)
{
    HttpReply httpReply(reply);
    emit sigOnUploadFeedBackReply(httpReply.data(), httpReply.code());

    reply->deleteLater();
}

HttpReply::HttpReply(QNetworkReply *reply)
    : m_code(0)
{
    m_data.clear();

    QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (variant.isValid())
    {
        m_code = variant.toInt();
        if (m_code == HTTP_OK)
        {
            m_data = reply->readAll();
        }
    }
}

qint32 HttpReply::code() const
{
    return m_code;
}

QByteArray HttpReply::data() const
{
    return m_data;
}

HttpRequest::HttpRequest(QUrl url)
{
    m_request.setUrl(url);
    m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_request.setRawHeader("Accept", "application/json");

    if (url.url().startsWith("https"))
    {
        QSslConfiguration config;
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1_0);
        m_request.setSslConfiguration(config);
    }
}

void HttpRequest::setContentLength(qint32 length)
{
    m_request.setHeader(QNetworkRequest::ContentLengthHeader, length);
}

QNetworkRequest HttpRequest::request() const
{
    return m_request;
}
