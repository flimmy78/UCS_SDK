#ifndef UPLUSRESTAPI_H
#define UPLUSRESTAPI_H

#include <QNetworkRequest>
#include <QObject>
#include <QUrl>
#include <qglobal.h>

#define HTTP_OK (200)

class QNetworkReply;

class HttpRequest
{
public:
    explicit HttpRequest(QUrl url);

    void setContentLength(qint32 length);

    QNetworkRequest request() const;

private:
    QNetworkRequest m_request;
};

class HttpReply
{
public:
    explicit HttpReply(QNetworkReply *reply);

    qint32 code() const;

    QByteArray data() const;

private:
    qint32 m_code;
    QByteArray m_data;
};

class UPlusRestApi : public QObject
{
    Q_OBJECT

public:
    explicit UPlusRestApi();

    /*!
     * \brief doLogin 登录UPlus server
     * \param userId 用户名
     * \param pwd 用户密码
     */
    void doLogin(QString userId, QString pwd);

    /*!
     * \brief doReLogin 重登陆
     * \param userId
     * \param pwd
     */
    void doReLogin(QString userId, QString pwd);

    /*!
     * \brief doResetPwd 修改密码
     * \param userId
     * \param pwd
     * \param newPwd 新密码
     */
    void doResetPwd(QString userId, QString pwd, QString newPwd);

    /*!
     * \brief doUploadHeaderImg 用户头像上传
     * \param userId
     * \param pwd
     * \param imgData 头像数据
     * \param imgType 头像类型(png, jpg)
     */
    void doUploadHeaderImg(QString userId, QString pwd,
                           QByteArray imgData, QString imgType);

    /*!
     * \brief doGetContracts 拉取通讯录
     * \param userId
     * \param pwd
     */
    void doGetContacts(QString userId, QString pwd);

    /*!
     * \brief doCheckVersion 检查版本更新
     * \param userId
     * \param pwd
     * \param platform 客户端类型(1: android, 2: ios)
     * \param version 当前客户端版本
     */
    void doCheckVersion(QString userId, QString pwd,
                        qint32 platform, QString version);

    /*!
     * \brief doQueryFriendInfo 查询好友信息
     * \param userId
     * \param pwd
     * \param friendId 需查询的好友Id
     */
    void doQueryFriendInfo(QString userId, QString pwd, QString friendId);

    /*!
     * \brief doUploadFeedBack 上传用户反馈
     * \param userId
     * \param pwd
     * \param content 反馈内容
     * \param contact 联系方式
     * \param platform 所属客户端类型
     */
    void doUploadFeedBack(QString userId, QString pwd,
                          QString content, QString contact,
                          qint32 platform);

signals:
    /*!
     * \brief sigOnLoginReply 登录响应，第一个参数为响应的内容， 第二个参数为http响应code
     */
    void sigOnLoginReply(QByteArray, int);
    void sigOnReLoginReply(QByteArray, int);
    void sigOnResetPwdReply(QByteArray, int);
    void sigOnUploadHeaderImgReply(QByteArray, int);
    void sigOnGetContactsReply(QByteArray, int);
    void sigOnCheckVersionReply(QByteArray, int);
    void sigOnQueryFriendInfoReply(QByteArray, int);
    void sigOnUploadFeedBackReply(QByteArray, int);

private slots:
    void onLoginReply(QNetworkReply *reply);
    void onReLoginReply(QNetworkReply *reply);
    void onResetPwdReply(QNetworkReply *reply);
    void onUploadHeaderImgReply(QNetworkReply *reply);
    void onGetContactsReply(QNetworkReply *reply);
    void onCheckVersionReply(QNetworkReply *reply);
    void onQueryFriendInfoReply(QNetworkReply *reply);
    void onUploadFeedBackReply(QNetworkReply *reply);

private:
    QString m_uplusUrl;
};

#endif // UPLUSRESTAPI_H
