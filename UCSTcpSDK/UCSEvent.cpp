#include "UCSEvent.h"
#include <QCoreApplication>
#include <QMutexLocker>

QMutex UCSEvent::m_Mutex;
QObject *UCSEvent::m_pSockObj = Q_NULLPTR;

void UCSEvent::postEvent(QObject *receiver, QEvent *event)
{
    if (receiver != Q_NULLPTR)
    {
        QCoreApplication::postEvent(receiver, event);
    }
}

void UCSEvent::postEvent(QObject *receiver, QEvent *event, int priority)
{
    if (receiver != Q_NULLPTR)
    {
        QCoreApplication::postEvent(receiver, event, priority);
    }
}

void UCSEvent::sendData(QEvent *event)
{
    QMutexLocker locker(&m_Mutex);
    if (m_pSockObj != Q_NULLPTR)
    {
        QCoreApplication::postEvent(m_pSockObj, event);
    }
}

void UCSEvent::setSockObj(QObject *pSockObj)
{
    QMutexLocker locker(&m_Mutex);
    m_pSockObj = pSockObj;
}

UCSRecvDataEvent::UCSRecvDataEvent(quint32 cmd, QByteArray dataArray, UCSEventType type)
    : QEvent((QEvent::Type)type)
    , m_cmd(cmd)
    , m_data(dataArray)
{

}

UCSRecvDataEvent::~UCSRecvDataEvent()
{

}

QByteArray UCSRecvDataEvent::data() const
{
    return m_data;
}

quint32 UCSRecvDataEvent::cmd() const
{
    return m_cmd;
}

UCSSendDataEvent::UCSSendDataEvent(quint32 cmd, QByteArray dataArray)
    : QEvent((QEvent::Type)kSendDataEvent)
    , m_cmd(cmd)
    , m_data(dataArray)
{

}

UCSSendDataEvent::~UCSSendDataEvent()
{

}

QByteArray UCSSendDataEvent::data() const
{
    return m_data;
}

quint32 UCSSendDataEvent::cmd() const
{
    return m_cmd;
}

UCSTcpStateEvent::UCSTcpStateEvent(UcsTcpState state)
    : QEvent((QEvent::Type)kTcpStateEvent)
    , m_state(state)
{

}

UcsTcpState UCSTcpStateEvent::state() const
{
    return m_state;
}

UCSLoginStateEvent::UCSLoginStateEvent(UcsLoginState state, QString userid)
    : QEvent((QEvent::Type)kLoginStateEvent)
    , m_state(state)
    , m_userId(userid)
{

}

UcsLoginState UCSLoginStateEvent::state() const
{
    return m_state;
}

QString UCSLoginStateEvent::userId() const
{
    return m_userId;
}
