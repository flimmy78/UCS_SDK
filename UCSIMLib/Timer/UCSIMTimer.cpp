#include "UCSIMTimer.h"
#include "UCSTcp/UCSLogger.h"

QMutex UCSIMTimer::m_mutex;
UCSIMTimer *UCSIMTimer::m_instance = Q_NULLPTR;

UCSIMTimer::UCSIMTimer(QObject *parent)
    : QObject(parent)
{
    initTimers();
}

UCSIMTimer::~UCSIMTimer()
{
    timerStop(kAllTimers);
}

UCSIMTimer *UCSIMTimer::getInstance()
{
    if (m_instance == Q_NULLPTR)
    {
        QMutexLocker locker(&m_mutex);
        if (m_instance == Q_NULLPTR)
        {
            m_instance = new UCSIMTimer;
        }
    }

    return m_instance;
}

void UCSIMTimer::release()
{
    QMutexLocker locker(&m_mutex);
    if (m_instance != Q_NULLPTR)
    {
        delete m_instance;
        m_instance = Q_NULLPTR;
    }
}

void UCSIMTimer::start(UCSIMTimer::ImTimerId id)
{
    getInstance()->timerStart(id);
}

void UCSIMTimer::kill(UCSIMTimer::ImTimerId id)
{
    getInstance()->timerStop(id);
}

void UCSIMTimer::setTime(UCSIMTimer::ImTimerId id, qint32 timeout)
{
    getInstance()->setTimeout(id, timeout);
}

qint32 UCSIMTimer::timeout(UCSIMTimer::ImTimerId id)
{
    return getInstance()->timeoutValue(id);
}

void UCSIMTimer::initTimers()
{
    for (qint32 i = 0; i < kTimerNum; ++i)
    {
        m_timers[i].timerId = 0;
        m_timers[i].timeout = 30 * 1000; // 30s
    }

    m_timers[kMsgSendTimer].name = "MsgSendTimer";
    m_timers[kCreateGroupTimer].name = "CreateGroupTimer";
    m_timers[kAddGroupMemberTimer].name = "AddGroupMemberTimer";
    m_timers[kDelGroupMemberTimer].name = "DelGroupMemberTimer";
    m_timers[kQuitGroupTimer].name = "QuitGroupTimer";
    m_timers[kSetGroupTopicTimer].name = "SetGroupTopicTimer";
}

void UCSIMTimer::timerStart(UCSIMTimer::ImTimerId id)
{
    if (m_timers[id].timerId > 0)
    {
        killTimer(m_timers[id].timerId);
        m_timers[id].timerId = 0;
    }

    if (m_timers[id].timeout <= 0)
    {
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kIMManager,
                QString("failed start timer ").append(m_timers[id].name));
        return;
    }

    m_timers[id].timerId = startTimer(m_timers[id].timeout);

    UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kIMManager,
            QString("start timer ").append(m_timers[id].name));
}

void UCSIMTimer::timerStop(UCSIMTimer::ImTimerId id)
{
    if (id != kAllTimers)
    {
        if (m_timers[id].timerId > 0)
        {
            UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kIMManager,
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
                UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kIMManager,
                        QString("kill timer ").append(m_timers[i].name));

                killTimer(m_timers[i].timerId);
                m_timers[i].timerId = 0;
            }
        }
    }
}

void UCSIMTimer::setTimeout(UCSIMTimer::ImTimerId id, qint32 timeout)
{
    if (id < kTimerNum)
    {
        m_timers[id].timeout = timeout;
    }
}

UCSIMTimer::ImTimerId UCSIMTimer::timerIndex(qint32 timerId)
{
    for (int idx = 0; idx < kTimerNum; idx++)
    {
        if (m_timers[idx].timerId == timerId)
        {
            return (ImTimerId)idx;
        }
    }

    return kTimerNum;
}

qint32 UCSIMTimer::timeoutValue(UCSIMTimer::ImTimerId id)
{
    if (id >= kTimerNum)
    {
        return -1;
    }

    return m_timers[id].timeout;
}

void UCSIMTimer::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timers[kCreateGroupTimer].timerId)
    {
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kIMManager,
                QString("Fired ").append(m_timers[kCreateGroupTimer].name));

        timerStop(kCreateGroupTimer);
    }
    else if (event->timerId() == m_timers[kAddGroupMemberTimer].timerId)
    {
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kIMManager,
                QString("Fired ").append(m_timers[kAddGroupMemberTimer].name));

        timerStop(kAddGroupMemberTimer);
    }
    else if (event->timerId() == m_timers[kDelGroupMemberTimer].timerId)
    {
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kIMManager,
                QString("Fired ").append(m_timers[kDelGroupMemberTimer].name));

        timerStop(kDelGroupMemberTimer);
    }
    else if (event->timerId() == m_timers[kQuitGroupTimer].timerId)
    {
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kIMManager,
                QString("Fired ").append(m_timers[kQuitGroupTimer].name));

        timerStop(kQuitGroupTimer);
    }
    else if (event->timerId() == m_timers[kSetGroupTopicTimer].timerId)
    {
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kIMManager,
                QString("Fired ").append(m_timers[kSetGroupTopicTimer].name));

        timerStop(kSetGroupTopicTimer);
    }

    ImTimerId id = timerIndex(event->timerId());
    if ( id < kTimerNum)
    {
        emit sig_timeout(id);
    }
}
