#include "UCSTcpSocket.h"
#include <QMutexLocker>
#include "Common/UCSLogger.h"
#include "UCSEvent.h"
#include "UCSPackage.h"

#define TRY_CONN_TIMES_PER_PROXY    (3)


UCSTcpSocket::UCSTcpSocket(QObject *parent)
    : QObject(parent)
    , m_proxyIndex(0)
    , m_curProxyIndex(0)
    , m_proxyTryTimes(0)
    , m_preState(TcpDisconnected)
    , m_state(TcpDisconnected)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kTcpSocket,
            "UCSTcpClient ctor");

    m_proxyList.clear();

    m_pSocket = new QTcpSocket();
    m_pSocket->setSocketOption(QAbstractSocket::LowDelayOption, 0);
    m_pSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 0);
//    m_pSocket->setReadBufferSize(0x800);

    initConnections();
    initTimer();
}

UCSTcpSocket::~UCSTcpSocket()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kTcpSocket,
            "UCSTcpClient ~dtor");

    timerStop(kAllTimer);

    if (m_pSocket != NULL)
    {
        m_pSocket->disconnectFromHost();
        m_pSocket->deleteLater();
    }
}

void UCSTcpSocket::requestNewFortune()
{
    QMutexLocker locker(&m_Mutex);
    QString proxy = m_proxyList.at(m_proxyIndex);
    QString strIP = proxy.section(':', 0, 0);
    QString strPort = proxy.section(':', 1, 1);

    m_pSocket->abort();
    m_pSocket->connectToHost(strIP, strPort.toInt());
    m_state = TcpConnecting;
    if (m_preState == TcpConnected ||
        m_preState == TcpReConnected)
    {
        m_state = TcpReConnecting;

        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kTcpSocket,
                QStringLiteral("TCP正在重连..."));
    }
    else
    {
        UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kTcpSocket,
                QStringLiteral("TCP正在连接..."));
    }

    UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kTcpSocket,
            QString("connecting to ").append(strIP));
}

void UCSTcpSocket::destoryFortune()
{
    if (m_pSocket != Q_NULLPTR)
    {
        m_pSocket->disconnectFromHost();
        if (m_pSocket->isValid())
        {
            m_pSocket->waitForDisconnected();
        }

        UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kTcpSocket,
                QStringLiteral("TCP连接已经断开..."));
    }
}

void UCSTcpSocket::initTimer()
{
    m_timers[kProxyTimerT1].timerId = 0;
    m_timers[kProxyTimerT1].timeout = 3 * 1000; // 3s
    m_timers[kProxyTimerT1].name = "ProxyTimerT1";

    m_timers[kProxyTimerT2].timerId = 0;
    m_timers[kProxyTimerT2].timeout = 10 * 1000; // 10s
    m_timers[kProxyTimerT2].name = "ProxyTimerT2";

    m_timers[kNetLinkTimer].timerId = 0;
    m_timers[kNetLinkTimer].timeout = 10 * 1000; // 10s
    m_timers[kNetLinkTimer].name = "NetLinkTimer";
}

void UCSTcpSocket::timerStart(UCSTcpSocket::TcpTimerId id)
{
    if (id >= kTimerNum)
    {
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kTcpSocket,
                QString("failed start invalid timer id(%1)").arg(id));
        return;
    }
    if (m_timers[id].timerId > 0)
    {
        killTimer(m_timers[id].timerId);
        m_timers[id].timerId = 0;
    }

    m_timers[id].timerId = startTimer(m_timers[id].timeout);

    UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kTcpSocket,
            QString("start timer ").append(m_timers[id].name));
}

void UCSTcpSocket::timerStop(UCSTcpSocket::TcpTimerId id)
{
    if (id != kAllTimer)
    {
        if (m_timers[id].timerId > 0)
        {
            UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kTcpSocket,
                    QString("kill timer ").append(m_timers[id].name));

            killTimer(m_timers[id].timerId);
            m_timers[id].timerId = 0;
        }
    }
    else
    {
        for (int i = 0; i < kTimerNum; i++)
        {
            if (m_timers[i].timerId > 0)
            {
                UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kTcpSocket,
                        QString("kill timer ").append(m_timers[i].name));

                killTimer(m_timers[i].timerId);
                m_timers[i].timerId = 0;
            }
        }
    }
}

UCSTcpSocket::TcpTimerId UCSTcpSocket::timerIndex(qint32 timerId)
{
    for (int idx = 0; idx < kTimerNum; idx++)
    {
        if (m_timers[idx].timerId == timerId)
        {
            return (TcpTimerId)idx;
        }
    }

    return kProxyTimerT1;
}

void UCSTcpSocket::doConnect()
{
    if (m_state != TcpWaitProxyUpdate)
    {
        QMutexLocker locker(&m_Mutex);
        m_proxyTryTimes = 0;
        m_preState = m_state = TcpDisconnected;

        timerStop(kProxyTimerT1);
    }
    else
    {
        /* start 10s timer to connect */
        timerStart(kProxyTimerT2);
    }
    requestNewFortune();
}

void UCSTcpSocket::doDisConnect()
{
    QMutexLocker locker(&m_Mutex);

    timerStop(kProxyTimerT1);
    destoryFortune();
    m_state = TcpDisconnected;
}

void UCSTcpSocket::doReConnect()
{
    UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kTcpSocket,
            QStringLiteral("TCP开始重连..."));
    doDisConnect();
    doConnect();
}

void UCSTcpSocket::onSendData(QByteArray dataArray)
{
    if (m_pSocket != Q_NULLPTR)
    {
        if (0 && dataArray.size() > 1400)
        {
            int idx = 0;
            while(idx < dataArray.size())
            {
                QByteArray sendData = dataArray.mid(idx, 1400);
                m_pSocket->write(sendData);
                m_pSocket->waitForBytesWritten(-1);
                idx += 1400;
            }
        }
        else
        {
            m_pSocket->write(dataArray);
            m_pSocket->waitForBytesWritten(-1);
        }
    }
}

void UCSTcpSocket::setState(const UcsTcpState &state)
{
    m_state = state;
    emit sigStateChanged(state);
}

void UCSTcpSocket::forceChangeProxy()
{
    m_proxyIndex = (++m_proxyIndex) % m_proxyList.size();
}

UcsTcpState UCSTcpSocket::state() const
{
    QMutexLocker locker((QMutex*)&m_Mutex);
    return m_state;
}

void UCSTcpSocket::timerEvent(QTimerEvent *event)
{
    if ((event->timerId() == m_timers[kProxyTimerT1].timerId) ||
        (event->timerId() == m_timers[kProxyTimerT2].timerId))
    {
        TcpTimerId id = timerIndex(event->timerId());
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kTcpSocket,
                QString("Fired ").append(m_timers[id].name));

        timerStop(id);

        requestNewFortune();
    }
    else if (event->timerId() == m_timers[kNetLinkTimer].timerId)
    {
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kTcpSocket,
                m_timers[kNetLinkTimer].name.append(" timeout."));
    }
}

void UCSTcpSocket::customEvent(QEvent *event)
{
    if (kSendDataEvent == event->type())
    {
        UCSSendDataEvent *sendEvent = (UCSSendDataEvent*)event;
        UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kTcpSocket,
                QString("send data, cmd(%1)").arg(UCSPackage::cmdName(sendEvent->cmd())));

        if (m_pSocket != Q_NULLPTR)
        {
//            qint64 bytes = m_pSocket->write(sendEvent->data());
            onSendData(sendEvent->data());

            UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kTcpSocket,
                    QString("send data bytes(%1) ").arg(sendEvent->data().size()));
        }
    }
}

void UCSTcpSocket::setProxyList(const QStringList &proxyList)
{
    QMutexLocker locker(&m_Mutex);
    m_proxyList = proxyList;
}

void UCSTcpSocket::initConnections()
{
    connect(m_pSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(onReadFortune()));
    connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

void UCSTcpSocket::onConnected()
{
    if ((m_preState == TcpConnected && m_state != TcpConnected) ||
        (m_preState == TcpReConnected && m_state != TcpReConnected))
    {
        setState(TcpReConnected);
        m_preState = TcpReConnected;

        UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kTcpSocket,
                QStringLiteral("TCP 重连成功..."));
    }
    else
    {
        setState(TcpConnected);
        m_preState = TcpConnected;

        UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kTcpSocket,
                QStringLiteral("TCP 连接成功..."));
    }

    m_proxyTryTimes = 0;
    m_curProxyIndex = m_proxyIndex;
}

void UCSTcpSocket::onDisconnected()
{
    UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kTcpSocket,
            QStringLiteral("TCP 连接断开..."));

    setState(TcpDisconnected);
}

void UCSTcpSocket::onReadFortune()
{
    if (m_pSocket->bytesAvailable() > 0)
    {
        QByteArray dataArray = m_pSocket->readAll();

        emit sigReadReady(dataArray);
    }
}

void UCSTcpSocket::onSocketError(QAbstractSocket::SocketError socketError)
{
    UCS_LOG(UCSLogger::kTraceError, UCSLogger::kTcpSocket,
            m_pSocket->errorString());

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kTcpSocket,
                QStringLiteral("远端主机关闭连接..."));
        break;

    case QAbstractSocket::HostNotFoundError:
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kTcpSocket,
                QStringLiteral("连接主机未找到..."));
        break;

    case QAbstractSocket::ConnectionRefusedError:
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kTcpSocket,
                QStringLiteral("远端主机拒绝连接..."));
        break;

    case QAbstractSocket::SocketTimeoutError:
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kTcpSocket,
                QStringLiteral("Socket超时..."));
        break;

    case QAbstractSocket::NetworkError:
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kTcpSocket,
                QStringLiteral("网络异常错误..."));
        break;

    default:
        break;
    }

    if (m_state == TcpConnected || m_state == TcpReConnected)
    {
        /* 如果之前状态是连接成功，则继续连接之前连接成功的proxy */
        m_proxyIndex = m_curProxyIndex;
    }
    else
    {
        if ((++m_proxyTryTimes) > TRY_CONN_TIMES_PER_PROXY)
        {
            m_proxyIndex = (++m_proxyIndex) % m_proxyList.size();
            m_proxyTryTimes = 0;

            if (m_proxyIndex == m_curProxyIndex)
            {
                setState(TcpWaitProxyUpdate);
            }
        }
    }

    if (state() != TcpWaitProxyUpdate)
    {
        timerStart(kProxyTimerT1);
    }
}
