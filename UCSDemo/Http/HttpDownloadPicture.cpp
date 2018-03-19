#include "HttpDownloadPicture.h"
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

HttpDownloadPicture::HttpDownloadPicture(QObject *parent)
    : QObject(parent)
{

}

void HttpDownloadPicture::execute(QString url, QString saveDir)
{
    if (url.isEmpty() || saveDir.isEmpty())
    {
        emit sigDownloadFinished(QString(""));
    }

    m_saveDir = saveDir;
    m_url = QUrl(url);
    QString fileName = m_saveDir + "/" + m_url.fileName();
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
        emit sigDownloadFinished(fileName);
        return;
    }

    QNetworkRequest request;
    request.setUrl(m_url);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadReply(QNetworkReply*)));
    manager->get(request);
}

QString HttpDownloadPicture::downloadBlock(QString url, QString saveDir)
{
    if (url.isEmpty() || saveDir.isEmpty())
    {
        return QString("");
    }

    QUrl reqUrl = QUrl(url);
    QString fileName = saveDir + "/" + reqUrl.fileName();
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
        return fileName;
    }

    QNetworkRequest request;
    request.setUrl(reqUrl);

    QNetworkAccessManager *manager = new QNetworkAccessManager;
    QNetworkReply *reply = manager->get(request);

    // wait for finish
    QEventLoop eventloop;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventloop, SLOT(quit()));
    eventloop.exec();

    if (reply->error() != QNetworkReply::NoError)
    {
        reply->deleteLater();
        return QString("");
    }

    QByteArray data = reply->readAll();

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(data);
    }
    file.close();

    reply->deleteLater();

    return fileName;
}

void HttpDownloadPicture::onDownloadReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        QString fileName = m_saveDir + "/" + m_url.fileName();

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(data);
        }
        file.close();

        emit sigDownloadFinished(fileName);
    }
    else
    {
        emit sigDownloadFinished(QString(""));
    }

    reply->deleteLater();
}
