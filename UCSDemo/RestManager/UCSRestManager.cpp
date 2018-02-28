#include "UCSRestManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

UCSRestManager::UCSRestManager()
{
    m_baseUrl = "http://imactivity.ucpaas.com/ottdemoapi/";
}

void UCSRestManager::getAuthCode(QString phone)
{
    /* 构造Url */
//    QUrl url("http://imactivity.ucpaas.com/ottdemoapi/getAuthcode.do");
    QUrl url(m_baseUrl + "getAuthcode.do");

    /* Json 数据 */
    QJsonObject json;
    json.insert("phone", phone);

    QJsonDocument doc;
    doc.setObject(json);
    QByteArray dataArray = doc.toJson(QJsonDocument::Compact).toBase64();
    qDebug() << "getAuthcode: " << doc.toJson(QJsonDocument::Compact);

    /* 构造请求 */
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/text");
    request.setHeader(QNetworkRequest::ContentLengthHeader, dataArray.length());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    /* 连接信号槽 */
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_onAuthReply(QNetworkReply*)));

    /* 发送请求 */
    manager->post(request, dataArray);
}

void UCSRestManager::login(QString phone, QString code)
{
    QUrl url(m_baseUrl + "login.do");

    /* Json 数据 */
    QJsonObject json;
    json.insert("phone", phone);
    json.insert("auth_code", code);

    QJsonDocument doc;
    doc.setObject(json);
    QByteArray dataArray = doc.toJson(QJsonDocument::Compact).toBase64();
    qDebug() << "do login: " << dataArray;

    /* 构造请求 */
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/text");
    request.setHeader(QNetworkRequest::ContentLengthHeader, dataArray.length());

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    /* 连接信号槽 */
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_onLoginReply(QNetworkReply*)));

    /* 发送请求 */
    manager->post(request, dataArray);
}

void UCSRestManager::getNickName(QString phone)
{
    QUrl url(m_baseUrl + "getNickname.do");

    /* Json 数据 */
    QJsonObject json;
    json.insert("phone", phone);

    QJsonDocument doc;
    doc.setObject(json);
    QByteArray dataArray = doc.toJson(QJsonDocument::Compact).toBase64();
    qDebug() << "do getNickname: " << dataArray;

    /* 构造请求 */
    QNetworkRequest request;
    request.setUrl(url);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    /* 连接信号槽 */
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_onGetNickName(QNetworkReply*)));

    /* 发送请求 */
    manager->post(request, dataArray);
}

QString UCSRestManager::errorString(UCSRestError error) const
{
    switch (error) {
    case NoError:
        return QStringLiteral("操作成功");

    case AuthCodeExpired:
        return QStringLiteral("验证码过期");

    case AuthCodeInCorrect:
        return QStringLiteral("验证码不正确");

    case PhoneNumberEmpty:
        return QStringLiteral("手机号码为空");

    case AuthCodeEmpty:
        return QStringLiteral("验证码为空");

    case NickNameEmpty:
        return QStringLiteral("昵称为空");

    case NoDataError:
        return QStringLiteral("查无数据");

    case CodeRequestFailed:
        return QStringLiteral("发送短信验证码失败");

    case RegisterUserFailed:
        return QStringLiteral("注册client用户失败");

    case InvalidParameters:
        return QStringLiteral("参数不正确");

    case PhoneNumberInExistent:
        return QStringLiteral("手机号码不存在");

    case PhoneNumberInCorrect:
        return QStringLiteral("手机号码不正确");

    case JoinGroupFailed:
        return QStringLiteral("加入群组失败");

    case DataFormatInvalid:
        return QStringLiteral("数据格式错误");

    case DoRequestFailed:
        return QStringLiteral("请求响应错误");

    case UnknownError:
    default:
        return QStringLiteral("其他错误");
    }
}

void UCSRestManager::slot_onAuthReply(QNetworkReply *reply)
{
    QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (variant.isValid() && variant.toInt() != 200)
    {
        qDebug() << QStringLiteral("Rest 请求错误, 获取验证码失败.") << reply->request().url().toString();
        emit sig_onFailed(DoRequestFailed);
        return;
    }

    QByteArray dataArray = reply->readAll();

    qDebug() << "onAuthReply: " << dataArray;

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(dataArray, &jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        int result;
        QString expired;
        QString phone;

        if (document.isObject())
        {
            QJsonObject jsonObj = document.object();
            if (jsonObj.contains("result"))
            {
                result = jsonObj["result"].toInt();
            }

            if (jsonObj.contains("phone"))
            {
                phone = jsonObj["phone"].toString();
            }

            if (jsonObj.contains("expired"))
            {
                expired = jsonObj["expired"].toString();
//                qDebug() << "expired: " << jsonObj["expired"].toString();
            }

            if (result == 0)
            {
                emit sig_onAuthSuccessed(expired.toInt());
            }
            else
            {
                emit sig_onFailed((UCSRestError)result);
            }
        }
    }
    else
    {
        qDebug("json format invalid.");
        emit sig_onFailed(DataFormatInvalid);
    }
}

void UCSRestManager::slot_onLoginReply(QNetworkReply *reply)
{
    QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (variant.isValid() && variant.toInt() != 200)
    {
        qDebug() << QStringLiteral("Rest 请求错误, 登录失败.") << reply->request().url().toString();
        emit sig_onFailed(DoRequestFailed);
    }

    QByteArray dataArray = reply->readAll();
    qDebug() << "onLoginReply: " << dataArray;

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(dataArray, &jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        int result;
        QString token;
        QString nickname;

        if (document.isObject())
        {
            QJsonObject jsonObj = document.object();
            if (jsonObj.contains("result"))
            {
                result = jsonObj["result"].toString().toInt();
            }

            if (jsonObj.contains("token"))
            {
                token = jsonObj["token"].toString();
            }

            if (jsonObj.contains("nickname"))
            {
                nickname = jsonObj["nickname"].toString();
            }

            if (result == 0)
            {
                emit sig_onLoginSuccessed(token, nickname);
            }
            else
            {
                emit sig_onFailed((UCSRestError)(result));
            }
        }
    }
    else
    {
        qDebug("json format invalid.");
        emit sig_onFailed(DataFormatInvalid);
    }
}

void UCSRestManager::slot_onGetNickName(QNetworkReply *reply)
{
    QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (variant.isValid() && variant.toInt() != 200)
    {
        qDebug() << QStringLiteral("Rest 请求错误, 查询昵称失败.") << reply->request().url().toString();
        emit sig_onFailed(DoRequestFailed);
    }

    QByteArray dataArray = reply->readAll();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(dataArray, &jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        int result;
        QString token;
        QString nickname;

        if (document.isObject())
        {
            QJsonObject jsonObj = document.object();
            if (jsonObj.contains("result"))
            {
                result = jsonObj["result"].toInt();
            }

            if (jsonObj.contains("nickname"))
            {
                nickname = jsonObj["nickname"].toString();
            }

            if (result == 0)
            {
                emit sig_onGetNickName(nickname);
            }
            else
            {
                emit sig_onFailed((UCSRestError)(result));
            }
        }
    }
    else
    {
        qDebug("json format invalid.");
        emit sig_onFailed(DataFormatInvalid);
    }
}
