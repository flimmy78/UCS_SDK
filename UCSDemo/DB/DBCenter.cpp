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

void DBCenter::release()
{
    QMutexLocker locker(&m_Mutex);
    if (m_pInstance != Q_NULLPTR)
    {
        delete m_pInstance;
        m_pInstance = Q_NULLPTR;
    }
}


DBCenter::DBCenter(QObject *parent)
    : QObject(parent)
{
    m_pLoginMgr = new LoginEntityManager;
    m_pContactMgr = new ContactEntityManager;
}

DBCenter::~DBCenter()
{
    if (m_pLoginMgr != Q_NULLPTR)
    {
        delete m_pLoginMgr;
        m_pLoginMgr = Q_NULLPTR;
    }

    if (m_pContactMgr != Q_NULLPTR)
    {
        delete m_pContactMgr;
        m_pContactMgr = Q_NULLPTR;
    }

    DBConnectionPool::release();
}

ContactEntityManager *DBCenter::contactMgr()
{
    DBCenter *center = getInstance();
    return center->m_pContactMgr;
}

LoginEntityManager *DBCenter::loginMgr()
{
    DBCenter *center = getInstance();
    return center->m_pLoginMgr;
}



