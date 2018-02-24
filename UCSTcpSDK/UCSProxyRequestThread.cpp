#include "UCSProxyRequestThread.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QMutexLocker>
#include <QUrlQuery>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "Common/UCSLogger.h"

UCSProxyRequestThread::UCSProxyRequestThread(QObject *parent)
    : QThread(parent)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kProxyUpdate,
            "UCSProxyRequestThread ctor");

    m_baseUrlList.clear();
    m_proxyList.clear();

    m_baseUrlList.append("http://cps.ucpaas.com:9997/v2/getproxylist");
    m_baseUrlList.append("http://47.94.42.238:9997/v2/getproxylist");
}

UCSProxyRequestThread::~UCSProxyRequestThread()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kProxyUpdate,
            "UCSProxyRequestThread ~dtor");

    requestInterruption();
    quit();
    wait();
}

QString UCSProxyRequestThread::imToken() const
{
    return m_imToken;
}

int UCSProxyRequestThread::setImToken(const QString &imToken)
{
    int res = parseToken(imToken);
    if (res == NoError)
    {
        m_imToken = imToken;
    }

    return res;
}

void UCSProxyRequestThread::run()
{
    if (m_imToken.isEmpty())
    {
        sig_onFail(TokenEmpty);
        return;
    }
    m_requestTimes = 0;
    while(!isInterruptionRequested() || m_requestTimes > 3)
    {
        /* would try 3 times to get proxy list */
        UcsTcpError res = NoError;
        if (m_requestTimes < 2)
        {
            res = doRequest(m_baseUrlList.at(0));
            if (res == NoError)
            {
                emit sig_onSuccess();
                break;
            }
            ++m_requestTimes;

            sleep(2);
        }

        if (m_requestTimes == 2)
        {
            if (m_baseUrlList.size() < 2)
            {
                emit sig_onFail(res);
                break;
            }

            res = doRequest(m_baseUrlList.at(1));
            if (res == 0)
            {
                emit sig_onSuccess();
            }
            else
            {
                emit sig_onFail(res);
            }
            break;
        }
    }

    UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kProxyUpdate,
            "UCSProxyRequestThread Thread finish");

    quit();    
}

/*
 * token 是json内容以base64编码的数据, 点'.'后面的为无效数据，json解析前需去除. eg:
 * "eyJBbGciOiJIUzI1NiIsIkFjY2lkIjoiY2U4OGQ4OWM0ZmU1ODhkODE1MTc3NDA1OGE0ZTA3ZGMiLCJBcHBpZCI6ImJhMjAyYzhlMWM5MzQ1MmI4YzAyNzY3MzJjYmM3ZDE1IiwiVXNlcmlkIjoiNjg0OTEwMDAwNTE5MjIifQ==.XJl/AePNwQg0jzeDamX2oMiMntD7KQGbOF2FXlBd0Yk="
 * 经过base64解码后的json内容:
 * {"Alg":"HS256","Accid":"ce88d89c4fe588d8151774058a4e07dc","Appid":"ba202c8e1c93452b8c0276732cbc7d15","Userid":"68491000051922"}
*/
int UCSProxyRequestThread::parseToken(const QString &imToken)
{
    QString validToken = imToken.section('.', 0, 0).trimmed();

    QByteArray token = QByteArray::fromBase64(validToken.toUtf8());

    UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kProxyUpdate, imToken);

    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(token, &jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        if (doc.isObject())
        {
            QJsonObject jsonObj = doc.object();
            QString userid;

            if (jsonObj.contains("Userid"))
            {
                userid = jsonObj["Userid"].toString();
            }

            if (userid.isEmpty())
            {
                return InvalidToken;
            }

            QMutexLocker locker(&m_Mutex);
            m_tokenData.userId = userid;
            if (jsonObj.contains("Accid"))
            {
                m_tokenData.accId = jsonObj["Accid"].toString();
            }

            if (jsonObj.contains("Appid"))
            {
                m_tokenData.appId = jsonObj["Appid"].toString();
            }

            {
                UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kProxyUpdate,
                        QString("Accid: ").append(m_tokenData.accId));
                UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kProxyUpdate,
                        QString("Appid: ").append(m_tokenData.appId));
                UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kProxyUpdate,
                        QString("Userid: ").append(m_tokenData.userId));
            }

            return NoError;
        }
    }

    return InvalidToken;
}

UcsTcpError UCSProxyRequestThread::doRequest(QString baseUrl)
{
    QUrl url(baseUrl);

    QUrlQuery query;
    {
        QMutexLocker locker(&m_Mutex);
        query.addQueryItem("appid", m_tokenData.appId);
        query.addQueryItem("userid", m_tokenData.userId);
    }
    query.addQueryItem("ver", "PC");
    url.setQuery(query);

    UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kProxyUpdate,
            QString("request url: ").append(url.url()));

    QNetworkRequest request;
    request.setUrl(url);

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(request);

    // wait for finish
    QEventLoop eventloop;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventloop, SLOT(quit()));
    eventloop.exec();

    QVariant variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (variant.isValid() && variant.toInt() != 200)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kProxyUpdate,
                QString(QStringLiteral("proxy列表请求错误")).append(" status code(%1)").arg(variant.toInt()));
        return ResponseError;
    }

    if (reply->bytesAvailable() <= 0 ||
        reply->error() != QNetworkReply::NoError)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kProxyUpdate,
                QString(QStringLiteral("Proxy URL请求错误 ")).append(reply->errorString()));

        return RequestUrlError;
    }
    QByteArray dataArray = reply->readAll();
//    qDebug() << "doRequest: " << dataArray;

    return parseReply(dataArray);
}

UcsTcpError UCSProxyRequestThread::parseReply(QByteArray dataArray)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(dataArray, &jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        if (document.isObject())
        {
            int ret = -1;

            QJsonObject jsonObj = document.object();
            if (jsonObj.contains("ret"))
            {
                ret = jsonObj["ret"].toInt();
            }

            if (jsonObj.contains("reason"))
            {
//                qDebug() << "parseReply: reason " << jsonObj["reason"];
            }

            if (ret != 0)
            {
                return ProxyQueryError;
            }

            if (jsonObj.contains("proxy"))
            {
                if (jsonObj["proxy"].isArray())
                {
                    QJsonArray jsonArray = jsonObj["proxy"].toArray();
                    for (int i = 0; i < jsonArray.size(); i++)
                    {
                        m_proxyList.append(jsonArray.at(i).toString());
                    }
                }
            }
            if (m_proxyList.size() <= 0)
            {
                return ProxyListEmpty;
            }

//            qDebug() << " proxys: " << m_proxyList;

            return NoError;
        }
    }

    return JsonFormatError;
}

TokenData UCSProxyRequestThread::tokenData() const
{
    return m_tokenData;
}

QStringList UCSProxyRequestThread::proxyList() const
{
    return m_proxyList;
}

