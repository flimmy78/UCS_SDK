#include "DBConnectionPool.h"
#include <QMutexLocker>
#include <QCoreApplication>
#include "UCSLogger.h"
#include "CommonHelper.h"

QMutex DBConnectionPool::m_mutex;
QWaitCondition DBConnectionPool::m_waitConnection;
DBConnectionPool* DBConnectionPool::m_instance = Q_NULLPTR;

DBConnectionPool::DBConnectionPool()
{
    QString exeName = QCoreApplication::applicationName();
    m_databaseName = exeName.append(".sqlite3");
    m_databaseType = "QSQLITE";

    m_isTestOnBorrow = true;
    m_testOnBorrowSql = "SELECT 1";
    m_maxConnectionCount = 5;

    m_maxWaitTime = 1000;   // 1s
    m_waitInterval = 200;   // 200ms
}

DBConnectionPool::~DBConnectionPool()
{
    foreach (QString connectionName, m_usedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    foreach (QString connectionName, m_unusedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
}

DBConnectionPool &DBConnectionPool::getInstance()
{
    if (m_instance == Q_NULLPTR)
    {
        QMutexLocker locker(&m_mutex);
        if (m_instance == Q_NULLPTR)
        {
            m_instance = new DBConnectionPool();
        }
    }

    return *m_instance;
}

void DBConnectionPool::release()
{
    QMutexLocker locker(&m_mutex);
    if (m_instance == Q_NULLPTR)
    {
        delete m_instance;
        m_instance = Q_NULLPTR;
    }
}

QSqlDatabase DBConnectionPool::openConnection()
{
    DBConnectionPool &pool = DBConnectionPool::getInstance();
    QString connectionName;

    QMutexLocker locker(&m_mutex);

    ///< 已创建连接数 >
    quint32 connectionCount = pool.m_usedConnectionNames.size()
                            + pool.m_unusedConnectionNames.size();

    for (quint32 i = 0;
        (i < pool.m_maxWaitTime) &&
            (pool.m_unusedConnectionNames.size() == 0) &&
            (connectionCount == pool.m_maxConnectionCount);
        i += pool.m_waitInterval)
    {
        m_waitConnection.wait(&m_mutex, pool.m_waitInterval);

        ///< 重新计算已创建连接数 >
        connectionCount = pool.m_usedConnectionNames.size()
                + pool.m_unusedConnectionNames.size();
    }

    if (pool.m_unusedConnectionNames.size() > 0)
    {
        ///< 有已经回收的连接，复用它们 >
        connectionName = pool.m_unusedConnectionNames.dequeue();
    }
    else if (connectionCount < pool.m_maxConnectionCount)
    {
        ///< 没有已经回收的连接，但是没有达到最大连接数，则创建新的连接 >
        connectionName = QString("DBConnection-%1").arg(connectionCount + 1);
    }
    else
    {
        ///< 已经达到最大连接数 >
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kIMDataBase,
                QStringLiteral("暂无可用数据库连接"));
        return QSqlDatabase();
    }

    ///< 创建连接 >
    QSqlDatabase db = pool.createConnection(connectionName);

    ///< 有效的数据库连接才放入 usedConnectionNames >
    if (db.isOpen())
    {
        pool.m_usedConnectionNames.enqueue(connectionName);
    }

    return db;
}

void DBConnectionPool::closeConnection(QSqlDatabase connection)
{
    DBConnectionPool &pool = DBConnectionPool::getInstance();
    QString connectionName = connection.connectionName();

    ///< 如果是我们创建的连接，从used里面删除，放入unused里 >
    if (pool.m_usedConnectionNames.contains(connectionName))
    {
        QMutexLocker locker(&m_mutex);
        pool.m_usedConnectionNames.removeOne(connectionName);
        pool.m_unusedConnectionNames.enqueue(connectionName);
        m_waitConnection.wakeOne();
    }
}

QSqlDatabase DBConnectionPool::createConnection(const QString &connectionName)
{
    ///< 如果连接已经创建过，则复用 >
    if (QSqlDatabase::contains(connectionName))
    {
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        if (m_isTestOnBorrow)
        {
            ///< 返回连接前访问数据库，如果连接断开，重新建立连接 >
            QSqlQuery query(m_testOnBorrowSql, db);
            if ((query.lastError().type() != QSqlError::NoError) &&
                !(db.open()))
            {
                UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                        QStringLiteral("错误: 打开数据库失败"));
                return QSqlDatabase();
            }
        }
        return db;
    }

//    QString userId = CommonHelper::readSetting(kSettingLoginUserId).toString();
//    if (userId.isEmpty())
//    {
//        return QSqlDatabase();
//    }
    QSqlDatabase db = QSqlDatabase::addDatabase(m_databaseType, connectionName);
    QString dataBaseName = CommonHelper::dataDir() + "/" + m_databaseName;
    db.setDatabaseName(dataBaseName);
    db.setUserName("ucpaas");
    db.setPassword("123456");

    if (!db.open())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString(QStringLiteral("错误: 打开数据库失败(%1)"))
                .arg(db.lastError().text()));
        return QSqlDatabase();
    }

    return db;
}

DBScoped::DBScoped()
{
    m_db = DBConnectionPool::openConnection();
}

DBScoped::~DBScoped()
{
    DBConnectionPool::closeConnection(m_db);
}

QSqlDatabase DBScoped::db() const
{
    return m_db;
}
