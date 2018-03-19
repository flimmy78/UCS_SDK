#ifndef UCSTCPMANAGER_H
#define UCSTCPMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QMultiMap>
#include <QThread>
#include <QNetworkReply>
#include "UCSTcpDefine.h"
#include "UCSTcpSocket.h"
#include "UCSProxyRequestThread.h"
#include "UCSLoginManager.h"
#include "UCSTcpMsgDispatch.h"
#include "UCSEvent.h"
#include <UCSCommonTypes.h>

class UCSTcpManager : public QObject
{
    Q_OBJECT
public:
    explicit UCSTcpManager(QObject* parent = 0);
    ~UCSTcpManager();

    /*!
     * \brief toggleEnvironment 切换生产环境和测试环境
     * \param isOnLine true for 生产环境
     */
    void DoUseOnLineEnv(bool isOnLine);

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

    /*!
     * \brief registerEventListener 注册eventType的指定接收对象，上层需重载 customEvent()函数
     * \param eventType 自定义事件类型
     * \param receiver 事件接收对象
     */
    void registerEventListener(UCSCustomEventType eventType,
                               QObject *receiver);

    /*!
     * \brief unRegisterEventListener 注销并删除eventType的指定接收对象
     * \param eventType 自定义事件类型
     * \param receiver 需注销的事件接收对象
     */
    void unRegisterEventListener(UCSCustomEventType eventType,
                                 QObject *receiver);

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

    /*!
     * \brief onUpdateProxyFinished 更新proxy列表完成
     * \param error
     */
    void onUpdateProxyFinished(int error);

    /*!
     * \brief TCP状态变换通知
    */
    void onTcpStateChanged(UcsTcpState state);

    /*!
    * \brief socket 接收到TCP 消息
    */
    void onReadReady(QByteArray dataArray);

    /*!
     * \brief 登录状态变换通知
    */
    void onLoginStateChanged(UcsLoginState state);

    /*!
     * \brief onPostMessage 分发消息到各对应模块
     * \param cmd
     * \param dataArray
     */
    void onPostMessage(quint32 cmd, QByteArray dataArray);

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
    ///< SDK 内部模块事件接收对象存储 >
    QMap<UCSModule, QObject*> m_listenMap;

    ///< SDK 外部即上层事件接收对象存储 >
    QMultiMap<UCSCustomEventType, QObject*> m_cusListenMap;
};

#endif // UCSTCPMANAGER_H
