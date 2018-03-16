#include "HttpDownloadPicture.h"
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
    }

    QNetworkRequest request;
    request.setUrl(m_url);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onDownloadReply(QNetworkReply*)));
    manager->get(request);
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
