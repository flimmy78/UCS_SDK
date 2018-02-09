#include "UCSGroupDBManager.h"
#include <UCSDBHelper.h>
#include <UCSConnectionPool.h>
#include <UCSTcp/UCSLogger.h>

GroupDBManager::GroupDBManager()
{
    m_createSql = "CREATE TABLE IF NOT EXISTS UCS_IM_GROUP ("
                        "groupId    TEXT    PRIMARY KEY"
                        "                   NOT NULL,"
                        "groupName  TEXT,"
                        "categoryId TEXT,"
                        "updateTime TEXT,"
                        "extra1     INTEGER,"
                        "extra2     INTEGER,"
                        "extra3     INTEGER,"
                        "extra4     TEXT,"
                        "extra5     TEXT,"
                        "extra6     TEXT)";
}

bool GroupDBManager::addGroup(GroupEntity *group)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_GROUP"))
    {
        return false;
    }

    QString sql = "INSERT OR REPLACE INTO UCS_IM_GROUP "
                        "(groupId, groupName, categoryId, updateTime, "
                        "extra1, extra2, extra3, extra4, extra5, extra6) "
                        "VALUES("
                        ":groupId, :groupName, :categoryId, :updateTime, "
                        ":extra1, :extra2, :extra3, :extra4, :extra5, :extra6)";

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":groupId", group->groupId);
    sqlQuery.bindValue(":groupName", group->groupName);
    sqlQuery.bindValue(":categoryId", group->categoryId);
    sqlQuery.bindValue(":updateTime", group->updateTime);
    sqlQuery.bindValue(":extra1", group->extra1);
    sqlQuery.bindValue(":extra2", group->extra2);
    sqlQuery.bindValue(":extra3", group->extra3);
    sqlQuery.bindValue(":extra4", group->extra4);
    sqlQuery.bindValue(":extra5", group->extra5);
    sqlQuery.bindValue(":extra6", group->extra6);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed insert group info with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool GroupDBManager::addGroup(QList<GroupEntity> groupList)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_GROUP"))
    {
        return false;
    }

    if (UCSDBHelper::transactionSupported())
    {
        UCSDBScoped scoped;
        QSqlDatabase db = scoped.db();
        db.transaction();
        bool result = false;
        for (qint32 i = 0; i < groupList.size(); ++i)
        {
            result = addGroup((GroupEntity*)&groupList.at(i));
            if (!result)
            {
                break;
            }
        }

        if (result)
        {
            db.commit();
            return true;
        }

        db.rollback();
        return false;
    }
    else
    {
        for (qint32 i = 0; i < groupList.size(); ++i)
        {
            bool result = addGroup((GroupEntity*)&groupList.at(i));
            if (!result)
            {
                return false;
            }
        }

        return true;
    }
}

bool GroupDBManager::delGroup(QString groupId)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_GROUP"))
    {
        return false;
    }

    QString sql = "DELETE FROM UCS_IM_GROUP WHERE groupId = :groupId;";
    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":groupId", groupId);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed delete target group with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool GroupDBManager::getGroup(QString groupId, GroupEntity &group)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_GROUP"))
    {
        return false;
    }

    QString sql = "SELECT * FROM UCS_IM_GROUP WHERE groupId = ?;";
    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    sqlQuery.prepare(sql);
    sqlQuery.addBindValue(groupId);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query target group with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    if (sqlQuery.next())
    {
        group.groupId = sqlQuery.value("groupId").toString();
        group.groupName = sqlQuery.value("groupName").toString();
        group.categoryId = sqlQuery.value("categoryId").toString();
        group.updateTime = sqlQuery.value("updateTime").toString();
        group.extra1 = sqlQuery.value("extra1").toString();
        group.extra2 = sqlQuery.value("extra2").toString();
        group.extra3 = sqlQuery.value("extra3").toString();
        group.extra4 = sqlQuery.value("extra4").toString();
        group.extra5 = sqlQuery.value("extra5").toString();
        group.extra6 = sqlQuery.value("extra6").toString();
    }

    return true;
}

QList<GroupEntity> GroupDBManager::getAllGroups()
{
    QList<GroupEntity> groupList;
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_GROUP"))
    {
        return groupList;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM UCS_IM_GROUP;";
    sqlQuery.prepare(sql);
    if (sqlQuery.exec())
    {
        while (sqlQuery.next())
        {
            GroupEntity group;
            group.groupId = sqlQuery.value("groupId").toString();
            group.groupName = sqlQuery.value("groupName").toString();
            group.categoryId = sqlQuery.value("categoryId").toString();
            group.updateTime = sqlQuery.value("updateTime").toString();
            group.extra1 = sqlQuery.value("extra1").toString();
            group.extra2 = sqlQuery.value("extra2").toString();
            group.extra3 = sqlQuery.value("extra3").toString();
            group.extra4 = sqlQuery.value("extra4").toString();
            group.extra5 = sqlQuery.value("extra5").toString();
            group.extra6 = sqlQuery.value("extra6").toString();

            groupList.append(group);
        }
    }

    return groupList;
}

bool GroupDBManager::updateGroup(MapValues values, MapConditions conditions)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_GROUP"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("UPDATE UCS_IM_GROUP SET").append(UCSDBHelper::formatValueCondition(values, conditions));
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed update target group info with error(%1)")
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    return true;
}
