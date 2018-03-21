#include "DBHelper.h"
#include <DBConnectionPool.h>
#include <QSqlDriver>
#include "UCSLogger.h"

DBHelper::DBHelper()
{

}

bool DBHelper::transactionSupported()
{
    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    if (db.driver()->hasFeature(QSqlDriver::Transactions))
    {
        return true;
    }

    return false;
}

bool DBHelper::checkTableExist(QString tableName)
{
    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);
    sqlQuery.exec(QString("SELECT COUNT(*) FROM sqlite_master WHERE type='table' and name='%1';").arg(tableName));
    if (sqlQuery.next())
    {
        if (sqlQuery.value(0).toInt() == 0)
        {
            return false;     ///< table unexist >
        }
        else
        {
            return true;     ///< table exist >
        }
    }

    return false;
}

bool DBHelper::checkAndCreateTable(QString sql, QString tableName)
{
    if (checkTableExist(tableName))
    {
        return true;
    }

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceFatal, UCSLogger::kIMDataBase,
                QString("failed create %1 table. error(%2)")
                .arg(tableName)
                .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

QString DBHelper::formatValueCondition(MapValues values, MapConditions conditions)
{
    QString strSql;
    QString strValue;
    QString strCondition;

    MapValues::iterator iter = values.begin();
    strValue = QString("%1='%2'").arg(iter.key()).arg(iter.value());
    ++iter;
    for (; iter != values.end(); ++iter)
    {
        QString strTemp = QString(", %1='%2'").arg(iter.key()).arg(iter.value());
        strValue.append(strTemp);
    }

    MapConditions::iterator iter1 = conditions.begin();
    strCondition = QString(" WHERE %1 = %2").arg(iter1.key()).arg(iter1.value());
    ++iter1;
    for (; iter1 != conditions.end(); ++iter1)
    {
        QString strTemp = QString(" AND %1 = %2").arg(iter1.key()).arg(iter1.value());
        strCondition.append(strTemp);
    }

    strSql = strValue + strCondition;

    return strSql;
}
