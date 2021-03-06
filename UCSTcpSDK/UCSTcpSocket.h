﻿#ifndef UCSTCPSOCKET_H
#define UCSTCPSOCKET_H

#include <QObject>
#include <QMutex>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QEvent>
#include "ucsTcpDefine.h"

class UCSTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit UCSTcpSocket(QObject *parent = 0);
    ~UCSTcpSocket();

    void doConnect();
    void doDisConnect();
    void doReConnect();

    void setProxyList(const QStringList &proxyList);

    /*!
     * \brief forceChangeProxy 连接成功，但登录失败，强制切换proxy
     */
    void forceChangeProxy();

    UcsTcpState state() const;

protected:
    void timerEvent(QTimerEvent *event) override;
    void customEvent(QEvent *event) override;

private:
    enum TcpTimerId
    {
        kProxyTimerT1,   /* proxy 连接定时器T1 */
        kProxyTimerT2,   /* proxy 连接定时器T2 */
        kNetLinkTimer,   /* 网络设备状态监测定时器 */
        kTimerNum,
        kAllTimer
    };

    typedef struct
    {
        qint32      timerId;
        quint32     timeout;
        QString     name;
    } TcpTimer;

    void initConnections();
    void requestNewFortune();
    void destoryFortune();
    void initTimer();
    void timerStart(TcpTimerId id);
    void timerStop(TcpTimerId id);
    TcpTimerId timerIndex(qint32 timerId);

    void setState(const UcsTcpState &state);

signals:
    void sigStateChanged(UcsTcpState state);
    void sigReadReady(QByteArray dataArray);

public slots:
    void onConnected();
    void onDisconnected();
    void onReadFortune();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onSendData(QByteArray dataArray);

private:
    QMutex m_Mutex;
    QStringList m_proxyList;
    quint32 m_proxyIndex;
    quint32 m_curProxyIndex;
    quint32 m_proxyTryTimes;

    QTcpSocket *m_pSocket;
    UcsTcpState m_preState;
    UcsTcpState m_state;

    TcpTimer m_timers[kTimerNum];
};

#endif // UCSTCPSOCKET_H
