#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <QObject>
#include <qglobal.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

class DBConnectionPool
{
public:
    /*!
     * \brief release 释放所有数据库连接
     */
    static void release();

    /*!
     * \brief openConnection 获取数据库连接
     * \return
     */
    static QSqlDatabase openConnection();

    /*!
     * \brief closeConnection 释放数据库连接回连接池
     * \param connection 需释放的已打开数据库连接
     */
    static void closeConnection(QSqlDatabase connection);

private:
    static DBConnectionPool& getInstance();

    DBConnectionPool();
    DBConnectionPool(const DBConnectionPool &other);
    DBConnectionPool &operator =(const DBConnectionPool &other);
    ~DBConnectionPool();

    /*!
     * \brief createConnection 创建数据库连接
     * \param connectionName 指定要创建的连接名称
     * \return 创建的数据库连接
     */
    QSqlDatabase createConnection(const QString &connectionName);

private:
    static QMutex m_mutex;
    static QWaitCondition m_waitConnection;
    static DBConnectionPool *m_instance;

    QString m_databaseName;
    QString m_databaseType;

    /*!
     * \brief m_usedConnectionNames 已使用的数据库连接名
     */
    QQueue<QString> m_usedConnectionNames;

    /*!
     * \brief m_unusedConnectionNames 未使用的数据库连接名
     */
    QQueue<QString> m_unusedConnectionNames;

    /*!
     * \brief testOnBorrow 取得连接的时候验证连接是否有效
     */
    bool m_isTestOnBorrow;
    /*!
     * \brief testOnBorrowSql 测试访问数据库的 sql 语句
     */
    QString m_testOnBorrowSql;

    /*!
     * \brief maxWaitTime 获取连接最大等待时间
     */
    quint32 m_maxWaitTime;
    /*!
     * \brief waitInterval 尝试获取连接时等待时间间隔
     */
    quint32 m_waitInterval;
    /*!
     * \brief maxConnectionCount 最大连接数
     */
    quint32 m_maxConnectionCount;
};

class DBScoped
{
public:
    explicit DBScoped();

    ~DBScoped();

    QSqlDatabase db() const;

private:
    QSqlDatabase m_db;
};

#endif // CONNECTIONPOOL_H
