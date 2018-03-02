#include "UCSUserInfoDBManager.h"
#include <QVariant>
#include <UCSConnectionPool.h>
#include <UCSDBHelper.h>
#include <UCSTcp/UCSLogger.h>

//const static QString tableName = "UCS_IM_USERINFO";

UserInfoDBManager::UserInfoDBManager()
{
    m_createSql = "CREATE TABLE IF NOT EXISTS UCS_IM_USERINFO ("
                          "userId       TEXT    NOT NULL"
                          "                     PRIMARY KEY,"
                          "userName     TEXT,"
                          "categoryId   TEXT,"
                          "portraitUrl  TEXT,"
                          "updateTime   TEXT,"
                          "userSettings TEXT,"
                          "extra1       INTEGER,"
                          "extra2       INTEGER,"
                          "extra3       INTEGER,"
                          "extra4       TEXT,"
                          "extra5       TEXT,"
                          "extra6       TEXT)";
}

bool UserInfoDBManager::addUserInfo(UserInfoEntity *userInfo)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_USERINFO"))
    {
        return false;
    }

    QString sql = "INSERT OR REPLACE INTO UCS_IM_USERINFO "
                        "(userId, userName, categoryId, portraitUrl, "
                        "updateTime, userSettings, extra1, extra2, extra3,"
                        " extra4, extra5, extra6) "
                        "VALUES("
                        ":userId, :userName, :categoryId, :portraitUrl, "
                        ":updateTime, :userSettings, :extra1, :extra2, :extra3, "
                        ":extra4, :extra5, :extra6);";

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":userId", userInfo->userId);
    sqlQuery.bindValue(":userName", userInfo->userName);
    sqlQuery.bindValue(":categoryId", userInfo->categoryId);
    sqlQuery.bindValue(":portraitUrl", userInfo->portraitUrl);
    sqlQuery.bindValue(":udpateTime", userInfo->updateTime);
    sqlQuery.bindValue(":userSettings", userInfo->userSettings);
    sqlQuery.bindValue(":extra1", userInfo->extra1);
    sqlQuery.bindValue(":extra2", userInfo->extra2);
    sqlQuery.bindValue(":extra3", userInfo->extra3);
    sqlQuery.bindValue(":extra4", userInfo->extra4);
    sqlQuery.bindValue(":extra5", userInfo->extra5);
    sqlQuery.bindValue(":extra6", userInfo->extra6);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed insert user info with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool UserInfoDBManager::addUserInfo(QList<UserInfoEntity> userList)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_USERINFO"))
    {
        return false;
    }

    if (UCSDBHelper::transactionSupported())
    {
        UCSDBScoped scoped;
        QSqlDatabase db = scoped.db();
        db.transaction();
        bool result = false;
        for (qint32 i = 0; i < userList.size(); i++)
        {
            result = addUserInfo((UserInfoEntity*)&userList.at(i));
            if (result == false)
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
        for (qint32 i = 0; i < userList.size(); i++)
        {
            bool result = addUserInfo((UserInfoEntity*)&userList.at(i));
            if (result == false)
            {
                return false;
            }
        }

        return true;
    }
}

bool UserInfoDBManager::delUserInfo(QString userId)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_USERINFO"))
    {
        return false;
    }

    QString sql = "DELETE FROM UCS_IM_USERINFO WHERE userId = :userId;";
    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":userId", userId);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed delete user info with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

QList<UserInfoEntity> UserInfoDBManager::getAllUserInfo()
{
    QList<UserInfoEntity> userList;

    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_USERINFO"))
    {
        return userList;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM UCS_IM_USERINFO;";
    sqlQuery.prepare(sql);
    if (sqlQuery.exec())
    {
        while (sqlQuery.next())
        {
            UserInfoEntity userInfo;
            userInfo.userId = sqlQuery.value("userId").toString();
            userInfo.userName = sqlQuery.value("userName").toString();
            userInfo.categoryId = sqlQuery.value("categoryId").toString();
            userInfo.portraitUrl = sqlQuery.value("portraitUrl").toString();
            userInfo.updateTime = sqlQuery.value("updateTime").toString();
            userInfo.userSettings = sqlQuery.value("userSettings").toString();
            userInfo.extra1 = sqlQuery.value("extra1").toString();
            userInfo.extra2 = sqlQuery.value("extra2").toString();
            userInfo.extra3 = sqlQuery.value("extra3").toString();
            userInfo.extra4 = sqlQuery.value("extra4").toString();
            userInfo.extra5 = sqlQuery.value("extra5").toString();
            userInfo.extra6 = sqlQuery.value("extra6").toString();

            userList.append(userInfo);
        }
    }

    return userList;
}

bool UserInfoDBManager::getUserInfo(QString userId, UserInfoEntity &userInfo)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_USERINFO"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM UCS_IM_USERINFO WHERE userId = ?;";
    sqlQuery.prepare(sql);
    sqlQuery.addBindValue(userId);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query target user info with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    if (sqlQuery.next())
    {
        userInfo.userId = sqlQuery.value("userId").toString();
        userInfo.userName = sqlQuery.value("userName").toString();
        userInfo.categoryId = sqlQuery.value("categoryId").toString();
        userInfo.portraitUrl = sqlQuery.value("portraitUrl").toString();
        userInfo.updateTime = sqlQuery.value("updateTime").toString();
        userInfo.userSettings = sqlQuery.value("userSettings").toString();
        userInfo.extra1 = sqlQuery.value("extra1").toString();
        userInfo.extra2 = sqlQuery.value("extra2").toString();
        userInfo.extra3 = sqlQuery.value("extra3").toString();
        userInfo.extra4 = sqlQuery.value("extra4").toString();
        userInfo.extra5 = sqlQuery.value("extra5").toString();
        userInfo.extra6 = sqlQuery.value("extra6").toString();
    }
    return true;
}

bool UserInfoDBManager::updateUserInfo(MapValues values, MapConditions conditions)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_USERINFO"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("UPDATE UCS_IM_USERINFO SET ").append(UCSDBHelper::formatValueCondition(values, conditions));
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed update target user info with error(%1)")
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    return true;
}
