#include "DBCenter.h"
#include <QMutexLocker>
#include <DBConnectionPool.h>

DBCenter *DBCenter::m_pInstance = Q_NULLPTR;
QMutex DBCenter::m_Mutex;

DBCenter *DBCenter::getInstance()
{
    if (m_pInstance == Q_NULLPTR)
    {
        QMutexLocker locker(&m_Mutex);
        if (m_pInstance == Q_NULLPTR)
        {
            m_pInstance = new DBCenter();
        }
    }

    return m_pInstance;
}

DBCenter::DBCenter(QObject *parent)
    : QObject(parent)
{

}

DBCenter::~DBCenter()
{
    DBConnectionPool::release();
}

void DBCenter::release()
{
    QMutexLocker locker(&m_Mutex);
    if (m_pInstance != Q_NULLPTR)
    {
        delete m_pInstance;
        m_pInstance = Q_NULLPTR;
    }
}

void DBCenter::customEvent(QEvent *event)
{

}


