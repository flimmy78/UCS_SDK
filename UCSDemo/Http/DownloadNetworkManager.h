#ifndef DOWNLOADNETWORKMANAGER_H
#define DOWNLOADNETWORKMANAGER_H

#include <QObject>
#include <qglobal.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>

/**
 * @brief 创建文件下载任务，保存到个人临时文件夹目录
 * @author Vinton.Liu
 * @date 2018-3-16
*/
class DownloadNetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit DownloadNetworkManager(QObject *parent = nullptr);
    virtual ~DownloadNetworkManager();

    void setAuthenticator(QString username, QString password);
    void execute(QString url, QString type);

    void downloadTest();

private:
    ///< 字节转KB、MB、GB >
    QString size(qint64 bytes);

    ///< 速度转KB/S、MB/S、GB/S >
    QString speed(double speed);

    ///< 秒转*d *h *m *s >
    QString timeFormat(int seconds);

signals:
    void sigReplyFinished(qint32);
    void sigFileName(QString);
    ///< qint64 bytesReceived, qint64 bytesTotal >
    void sigDownloadProgress(qint64, qint64);

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void onReadyRead();
    void onAuthenticationRequired(QNetworkReply *reply,
                                  QAuthenticator *authenticator);
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkReply *m_pReply;
    QUrl m_url;
    QString m_tmpFileName;
    QString m_userName;
    QString m_password;
    QTime m_downloadTime;
    int m_nTime;
};

#endif // DOWNLOADNETWORKMANAGER_H
