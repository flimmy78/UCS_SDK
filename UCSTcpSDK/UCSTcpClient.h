#ifndef UCSTCPCLIENT_H
#define UCSTCPCLIENT_H

#include "ucstcpsdk_global.h"
#include <QObject>
#include <QSharedPointer>
#include <QMutex>

class UCSTcpManager;

class UCSTCPSDKSHARED_EXPORT UCSTcpClient : public QObject
{
    Q_OBJECT

public:
    static QSharedPointer<UCSTcpClient> &Instance();
    ~UCSTcpClient();

    /*!
     * 初始化
    */
    void init();

    /*!
     * \brief 用户使用token方式登录
    */
    void doLogin(QString imToken);

    /*!
     * \brief 用户退出登录
    */
    void doLogout();

    /*!
     * \brief 设置IM client对象
    */
    void setIMClient(QObject *obj);

    /*!
     * \brief 设置VoIP client对象
    */
    void setVoIPClient(QObject *obj);

    /*!
     * \brief 获取TCP SDK版本号
    */
    QString getVersion();

signals:
    void sig_onLoginSuccessed();
    void sig_onLoginFailed(int error);
    void sig_onLogoutSuccessed();
    void sig_ConnectionChanged(int state);

private:
    explicit UCSTcpClient(QObject* parent = 0);
    UCSTcpClient(const UCSTcpClient&);
    UCSTcpClient& operator=(const UCSTcpClient&);

    void initConnections();

private:
    static QSharedPointer<UCSTcpClient> m_pInstance;
    static QMutex m_Mutex;
    UCSTcpManager *m_pConnManager;
};

#endif // UCSTCPCLIENT_H
