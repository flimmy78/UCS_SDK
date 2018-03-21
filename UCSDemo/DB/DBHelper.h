#ifndef DBHELPER_H
#define DBHELPER_H

#include <QObject>
#include <qglobal.h>
#include <QMap>
#include <QVariant>
#include "DBEntity.h"

class DBHelper
{
public:
    explicit DBHelper();

    /*!
     * \brief transactionSupported 检测是否驱动支持事务transaction
     * \return
     */
    static bool transactionSupported();

    /*!
     * \brief checkTableExist 检查数据库中是否存在对应表
     * \param tableName 表名
     * \return true for exist, else false
     */
    static bool checkTableExist(QString tableName);

    /*!
     * \brief checkAndCreateTable 检查数据库中是否存在对应表,若不存在，则尝试创建
     * \param sql 创建对应表的SQL语句
     * \param tableName 表名
     * \return true for exist or create success, false for create failed.
     */
    static bool checkAndCreateTable(QString sql, QString tableName);

    /*!
     * \brief formatValueCondition 格式化键值对，生成对应SQL语句
     * \param values
     * \param conditions
     * \return 生成的SQL语句
     */
    static QString formatValueCondition(MapValues values, MapConditions conditions);
};

#endif // DBHELPER_H
