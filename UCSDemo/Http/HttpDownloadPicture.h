#ifndef HTTPDOWNLOADPICTURE_H
#define HTTPDOWNLOADPICTURE_H

#include <QObject>
#include <qglobal.h>
#include <QNetworkReply>

/**
 * @brief 下载头像或小文件，不适用与大文件
 * @author Vinton.Liu
 * @date
*/
class HttpDownloadPicture : public QObject
{
    Q_OBJECT
public:
    explicit HttpDownloadPicture(QObject *parent = nullptr);

    /*!
     * \brief execute 开始请求下载
     * \param url 图片 http 地址
     * \param saveDir 图片保存绝对目录路径，文件名与 url 文件名相同
     */
    void execute(QString url, QString saveDir);

    /*!
     * \brief downloadBlock 阻塞式下载，下载成功返回保存绝对路径
     * \param url
     * \param saveDir
     * \return 成功返回绝对路径，失败返回空
     */
    static QString downloadBlock(QString url, QString saveDir);

signals:
    void sigDownloadFinished(QString filepath);

private slots:
    void onDownloadReply(QNetworkReply* reply);

private:
    QString m_saveDir;
    QUrl m_url;
};

#endif // HTTPDOWNLOADPICTURE_H
