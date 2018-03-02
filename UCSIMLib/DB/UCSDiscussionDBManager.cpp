#include "UCSDiscussionDBManager.h"
#include <QVariant>
#include <UCSConnectionPool.h>
#include <UCSDBHelper.h>
#include <UCSTcp/UCSLogger.h>

DiscussionDBManager::DiscussionDBManager()
{
    m_createSql = "CREATE TABLE IF NOT EXISTS UCS_IM_DISCUSSION ("
                          "discussionId      TEXT    PRIMARY KEY"
                          "                          NOT NULL,"
                          "discussionName    TEXT,"
                          "categoryId        INTEGER,"
                          "memberCount       INTEGER,"
                          "ownerId           TEXT,"
                          "members           TEXT,"
                          "settings          TEXT"
                          "createTime        TEXT,"
                          "state             INTEGER,"
                          "extra1            INTEGER,"
                          "extra2            INTEGER,"
                          "extra3            INTEGER,"
                          "extra4            TEXT,"
                          "extra5            TEXT,"
                          "extra6            TEXT)";
}

bool DiscussionDBManager::addDiscussioin(const DiscussionEntity *discussion)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_DISCUSSION"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "INSERT OR REPLACE INTO UCS_IM_DISCUSSION ("
                                    "extra6,"
                                    "extra5,"
                                    "extra4,"
                                    "extra3,"
                                    "extra2,"
                                    "extra1,"
                                    "state,"
                                    "createTime,"
                                    "settings,"
                                    "members,"
                                    "ownerId,"
                                    "memberCount,"
                                    "categoryId,"
                                    "discussionName,"
                                    "discussionId"
                                ") VALUES ("
                                    ":extra6,"
                                    ":extra5,"
                                    ":extra4,"
                                    ":extra3,"
                                    ":extra2,"
                                    ":extra1,"
                                    ":state,"
                                    ":createTime,"
                                    ":settings,"
                                    ":members,"
                                    ":ownerId,"
                                    ":memberCount,"
                                    ":categoryId,"
                                    ":discussionName,"
                                    ":discussionId"
                                ");";

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":discussionId", discussion->discussionId);
    sqlQuery.bindValue(":discussionName", discussion->discussionName);
    sqlQuery.bindValue(":categoryId", discussion->categoryId);
    sqlQuery.bindValue(":memberCount", discussion->memberCount);
    sqlQuery.bindValue(":ownerId", discussion->ownerId);
    sqlQuery.bindValue(":members", discussion->members);
    sqlQuery.bindValue(":settings", discussion->sSettings);
    sqlQuery.bindValue(":createTime", discussion->createTime);
    sqlQuery.bindValue(":state", discussion->state);
    sqlQuery.bindValue(":extra1", discussion->extra1);
    sqlQuery.bindValue(":extra2", discussion->extra2);
    sqlQuery.bindValue(":extra3", discussion->extra3);
    sqlQuery.bindValue(":extra4", discussion->extra4);
    sqlQuery.bindValue(":extra5", discussion->extra5);
    sqlQuery.bindValue(":extra6", discussion->extra6);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed insert discussion info with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool DiscussionDBManager::addDiscussioin(const QList<DiscussionEntity> discussionList)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_DISCUSSION"))
    {
        return false;
    }

    if (UCSDBHelper::transactionSupported())
    {
        UCSDBScoped scoped;
        QSqlDatabase db = scoped.db();
        db.transaction();
        bool result = false;
        for (qint32 i = 0; i < discussionList.size(); ++i)
        {
            result = addDiscussioin((DiscussionEntity*)&discussionList.at(i));
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
        for (qint32 i = 0; i < discussionList.size(); ++i)
        {
            bool result = addDiscussioin((DiscussionEntity*)&discussionList.at(i));
            if (!result)
            {
                return false;
            }
        }

        return true;
    }
}

bool DiscussionDBManager::delDiscussion(const QString discussionId)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_DISCUSSION"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "DELETE FROM UCS_IM_DISCUSSION WHERE discussionId = ?;";
    sqlQuery.prepare(sql);
    sqlQuery.addBindValue(discussionId);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed delete discussion with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool DiscussionDBManager::getDiscussion(const QString discussionId, DiscussionEntity &discussion)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_DISCUSSION"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM UCS_IM_DISCUSSION WHERE discussionId = ?;";
    sqlQuery.prepare(sql);
    sqlQuery.addBindValue(discussionId);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query target discussion with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    if (sqlQuery.next())
    {
        discussion.discussionId = sqlQuery.value("discussionId").toString();
        discussion.discussionName = sqlQuery.value("discussionName").toString();
        discussion.categoryId = sqlQuery.value("categoryId").toString();
        discussion.ownerId = sqlQuery.value("ownerId").toString();
        discussion.memberCount = sqlQuery.value("memberCount").toString();
        discussion.members = sqlQuery.value("members").toString();
        discussion.sSettings = sqlQuery.value("settings").toString();
        discussion.createTime = sqlQuery.value("createTime").toString();
        discussion.state = sqlQuery.value("state").toString();
        discussion.extra1 = sqlQuery.value("extra1").toString();
        discussion.extra2 = sqlQuery.value("extra2").toString();
        discussion.extra3 = sqlQuery.value("extra3").toString();
        discussion.extra4 = sqlQuery.value("extra4").toString();
        discussion.extra5 = sqlQuery.value("extra5").toString();
        discussion.extra6 = sqlQuery.value("extra6").toString();
    }

    return true;
}

QList<DiscussionEntity> DiscussionDBManager::getAllDiscussions()
{
    QList<DiscussionEntity> discussionList;

    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_DISCUSSION"))
    {
        return discussionList;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM UCS_IM_DISCUSSION;";
    sqlQuery.prepare(sql);
    if (sqlQuery.exec())
    {
        while (sqlQuery.next())
        {
            DiscussionEntity discussion;
            discussion.discussionId = sqlQuery.value("discussionId").toString();
            discussion.discussionName = sqlQuery.value("discussionName").toString();
            discussion.categoryId = sqlQuery.value("categoryId").toString();
            discussion.ownerId = sqlQuery.value("ownerId").toString();
            discussion.memberCount = sqlQuery.value("memberCount").toString();
            discussion.members = sqlQuery.value("members").toString();
            discussion.sSettings = sqlQuery.value("settings").toString();
            discussion.createTime = sqlQuery.value("createTime").toString();
            discussion.state = sqlQuery.value("state").toString();
            discussion.extra1 = sqlQuery.value("extra1").toString();
            discussion.extra2 = sqlQuery.value("extra2").toString();
            discussion.extra3 = sqlQuery.value("extra3").toString();
            discussion.extra4 = sqlQuery.value("extra4").toString();
            discussion.extra5 = sqlQuery.value("extra5").toString();
            discussion.extra6 = sqlQuery.value("extra6").toString();

            discussionList.append(discussion);
        }
    }

    return discussionList;
}

bool DiscussionDBManager::updateDiscussion(MapValues values, MapConditions conditions)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_DISCUSSION"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("UPDATE UCS_IM_DISCUSSION SET ").append(UCSDBHelper::formatValueCondition(values, conditions));
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed update target discussion info with error(%1)")
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    return true;
}

