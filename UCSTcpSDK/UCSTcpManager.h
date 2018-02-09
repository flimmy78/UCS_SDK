#ifndef UCSTCPMANAGER_H
#define UCSTCPMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QThread>
#include <QNetworkReply>
#include "UCSTcpDefine.h"
#include "UCSTcpSocket.h"
#include "UCSProxyRequestThread.h"
#include "UCSLoginManager.h"
#include "UCSTcpMsgDispatch.h"
#include "UCSEvent.h"

class UCSTcpManager : public QObject
{
    Q_OBJECT
public:
    explicit UCSTcpManager(QObject* parent = 0);
    ~UCSTcpManager();

    /*!
     * \brief 用户登录
    */
    void doLogin(QString imToken);

    /*!
     * \brief 用户退出登录
    */
    void doLogout();

    void setIMClient(QObject *obj);

    void setVoIPClient(QObject *obj);

private:
    /*!
     * \brief 初始化
    */
    void init();

    /*!
     * \brief 信号与槽连接
    */
    void initConnections();

signals:
    void sig_onError(UcsTcpError error);

public slots:
    void slot_onError(UcsTcpError error);
    /*!
     * \brief 更新proxy列表成功
    */
    void slot_onProxySuccess();

    /*!
     * \brief TCP状态变换通知
    */
    void slot_onTcpStateChanged(UcsTcpState state);

    /*!
    * \brief socket 接收到TCP 消息
    */
    void slot_readReady(QByteArray dataArray);

    /*!
     * \brief 登录状态变换通知
    */
    void slot_onLoginStateChanged(UcsLoginState state);

    void slot_onPostMessage(quint32 cmd, QByteArray dataArray);

private:
    QThread *m_pDispatchThread;
    UCSTcpSocket *m_pTcpSocket;
    UCSProxyRequestThread *m_pProxyThread;
    UCSLoginManager *m_pLoginManager;
    UCSTcpMsgDispatch *m_pMsgDispatch;

    QString m_imToken;
    QStringList m_ProxyList;
    TokenData m_tokenData;

    QMutex m_Mutex;
    QMap<UCSModule, QObject*> m_listenMap;
};

#endif // UCSTCPMANAGER_H
