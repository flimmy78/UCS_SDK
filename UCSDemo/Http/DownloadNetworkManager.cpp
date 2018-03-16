#include "DownloadNetworkManager.h"
#include "UCSLogger.h"
#include "CommonHelper.h"

#include <QDir>
#include <QFileInfo>
#include <QtCore/qmath.h>

DownloadNetworkManager::DownloadNetworkManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
    this->setObjectName("DownloadNetworkManager");

    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("yyyy-MM-dd-hh-mm-ss-zzz");
    m_tmpFileName = CommonHelper::userTempPath() + QString("/%1.tmp").arg(date);

    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReplyFinished(QNetworkReply*)));
    connect(this, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this, SLOT(onAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
}

DownloadNetworkManager::~DownloadNetworkManager()
{
    if (m_pReply != Q_NULLPTR)
    {
        ///< abort download >
        m_pReply->abort();
        m_pReply->deleteLater();
        m_pReply = Q_NULLPTR;
    }
}

void DownloadNetworkManager::setAuthenticator(QString username, QString password)
{
    m_userName = username;
    m_password = password;
}

void DownloadNetworkManager::execute(QString url, QString type)
{
    if (url.isEmpty())
    {
        emit sigReplyFinished(404);
    }
    else
    {
        m_url = QUrl(url);

        QNetworkRequest request;
        request.setUrl(m_url);

        if (type.startsWith("zip"))
        {
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/zip");
        }

        m_pReply = this->get(request);

        if (m_downloadTime.elapsed())
        {
            m_downloadTime.restart();
        }
        else
        {
            m_downloadTime.start();
        }
        m_nTime = 0;

        connect(m_pReply, SIGNAL(downloadProgress(qint64,qint64)),
                this, SIGNAL(sigDownloadProgress(qint64,qint64)));
        connect(m_pReply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
//        connect(m_pReply, SIGNAL(downloadProgress(qint64,qint64)),
//                this, SLOT(onDownloadProgress(qint64,qint64)));
    }
}

void DownloadNetworkManager::downloadTest()
{
    QString url = "https://nchc.dl.sourceforge.net/project/libpng/zlib/1.2.11/zlib1211.zip";
    execute(url, "zip");
}

QString DownloadNetworkManager::size(qint64 bytes)
{
    QString strUnit;
    double dSize = bytes * 1.0;

    if (dSize <= 0)
    {
        dSize = 0.0;
    }
    else if (dSize < 1024)
    {
        strUnit = "Bytes";
    }
    else if (dSize < 1024 * 1024)
    {
        dSize /= 1024;
        strUnit = "KB";
    }
    else if (dSize < 1024 * 1024 * 1024)
    {
        dSize /= (1024 * 1024);
        strUnit = "MB";
    }
    else
    {
        dSize /= (1024 * 1024 * 1024);
        strUnit = "GB";
    }

    return QString("%1 %2").arg(QString::number(dSize, 'f', 2)).arg(strUnit);
}

QString DownloadNetworkManager::speed(double speed)
{
    QString strUnit;
    if (speed <= 0)
    {
        speed = 0;
        strUnit = "Bytes/S";
    }
    else if (speed < 1024)
    {
        strUnit = "Bytes/S";
    }
    else if (speed < 1024 * 1024)
    {
        speed /= 1024;
        strUnit = "KB/S";
    }
    else if (speed < 1024 * 1024 * 1024)
    {
        speed /= (1024 * 1024);
        strUnit = "MB/S";
    }
    else
    {
        speed /= (1024 * 1024 * 1024);
        strUnit = "GB/S";
    }

    QString strSpeed = QString::number(speed, 'f', 2);
    return QString("%1 %2").arg(strSpeed).arg(strUnit);
}

QString DownloadNetworkManager::timeFormat(int seconds)
{
    QString strValue;
    QString strSpacing(" ");
    if (seconds <= 0)
    {
        strValue = QString("%1s").arg(0);
    }
    else if (seconds < 60)
    {
        strValue = QString("%1s").arg(seconds);
    }
    else if (seconds < 60 * 60)
    {
        int nMinute = seconds / 60;
        int nSecond = seconds - nMinute * 60;

        strValue = QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else if (seconds < 60 * 60 * 24)
    {
        int nHour = seconds / (60 * 60);
        int nMinute = (seconds - nHour * 60 * 60) / 60;
        int nSecond = seconds - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else
    {
        int nDay = seconds / (60 * 60 * 24);
        int nHour = (seconds - nDay * 60 * 60 * 24) / (60 * 60);
        int nMinute = (seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60) / 60;
        int nSecond = seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1d").arg(nDay);

        if (nHour > 0)
            strValue += strSpacing + QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }

    return strValue;
}

void DownloadNetworkManager::onReplyFinished(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (reply->error() == QNetworkReply::NoError)
    {
        ///< 重命名临时文件 >
        QFileInfo fileInfo(m_tmpFileName);
        QFileInfo newFileInfo = fileInfo.absolutePath() + "/" + m_url.fileName();
        QDir dir(fileInfo.absolutePath());
        if (dir.exists())
        {
            if (newFileInfo.exists())
            {
                newFileInfo.dir().remove(newFileInfo.fileName());
            }
            QFile::rename(m_tmpFileName, newFileInfo.absoluteFilePath());

            UCS_LOG(UCSLogger::kTraceInfo, this->objectName(),
                    QString("download finished. save in(%1)")
                            .arg(newFileInfo.absoluteFilePath()));
        }
    }
    else
    {
        UCS_LOG(UCSLogger::kTraceError, this->objectName(),
                reply->errorString());
    }

    emit sigReplyFinished(statusCode.toInt());
}

void DownloadNetworkManager::onReadyRead()
{
    QFileInfo fileInfo(m_tmpFileName);
    QFileInfo newFileInfo = fileInfo.absolutePath() + m_url.fileName();
    QString strFileName = newFileInfo.absoluteFilePath();

    emit sigFileName(strFileName);

    QFile file(m_tmpFileName);
    if (file.open(QIODevice::Append))
    {
        file.write(m_pReply->readAll());
    }

    file.close();
}

void DownloadNetworkManager::onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    Q_UNUSED(reply);

    authenticator->setUser(m_userName);
    authenticator->setPassword(m_password);
}

///< 文件剩余大小 = 文件总大小 - 文件已下载大小  >
/// 平均速度 = 文件已下载大小 / 文件已下载大小所用的时间
/// 瞬时速度 = 每秒下载的文件大小
/// 剩余时间 = 文件剩余大小 / 瞬时速度
void DownloadNetworkManager::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ///< 总时间, 单位: 毫秒 >
    int nTime = m_downloadTime.elapsed();

    ///< 平均速度 >
    double dAvgBytesSpeed = (bytesReceived * 1000.0) / nTime;

    ///< 本次下载进度更新所用时间 >
    nTime -= m_nTime;

    ///< 瞬时速度 >
    double dBytesSpeed = (bytesReceived * 1000.0) / nTime;

    ///< 剩余时间 >
    qint64 leftBytes = bytesTotal - bytesReceived;
    double dLeftTime = (leftBytes * 1.0) / dBytesSpeed;

    QString strAvgSpeed = speed(dAvgBytesSpeed);
    QString strSpeed = speed(dBytesSpeed);
    QString strLeftTime = timeFormat(qCeil(dLeftTime));
    QString strTotalSize = size(bytesTotal);
    QString strReceivedSize = size(bytesReceived);

    UCS_LOG(UCSLogger::kTraceInfo, this->objectName(),
            QString("avgSpeed: %1 speed: %2 leftTime: %3 totalSize: %4 receivedSize: %5 nTime: %6")
                .arg(strAvgSpeed).arg(strSpeed).arg(strLeftTime)
                .arg(strTotalSize).arg(strReceivedSize).arg(nTime));

    m_nTime = nTime;
}
