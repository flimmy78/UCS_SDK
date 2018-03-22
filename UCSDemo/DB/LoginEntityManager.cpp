#include "LoginEntityManager.h"
#include "DBHelper.h"
#include "DBConnectionPool.h"
#include "UCSLogger.h"

#define TAG "LoginTable"

LoginEntityManager::LoginEntityManager()
{
    m_createSql = "CREATE TABLE IF NOT EXISTS LOGIN_INFO ("
                        "id        INTEGER UNIQUE"
                        "                  PRIMARY KEY AUTOINCREMENT,"
                        "userId    TEXT    UNIQUE"
                        "                  NOT NULL,"
                        "userName  TEXT,"
                        "userPwd   TEXT    NOT NULL,"
                        "summitPwd TEXT,"
                        "headUrl   TEXT,"
                        "headPath  TEXT,"
                        "isKeepPwd BOOLEAN,"
                        "userSex   INTEGER,"
                        "token     TEXT,"
                        "time      TEXT,"
                        "extra1    INTEGER,"
                        "extra2    INTEGER,"
                        "extra3    INTEGER,"
                        "extra4    TEXT,"
                        "extra5    TEXT,"
                        "extra6    TEXT);";
}

bool LoginEntityManager::addLoginInfo(const LoginEntity &loginEntity)
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "LOGIN_INFO"))
    {
        return false;
    }

    QString sql = "INSERT OR REPLACE INTO LOGIN_INFO ("
                            "userId,"
                            "userName,"
                            "userPwd,"
                            "summitPwd,"
                            "headUrl,"
                            "headPath,"
                            "isKeepPwd,"
                            "userSex,"
                            "token,"
                            "time,"
                            "extra1,"
                            "extra2,"
                            "extra3,"
                            "extra4,"
                            "extra5,"
                            "extra6"
                        ")VALUES ("
                              ":userId,"
                              ":userName,"
                              ":userPwd,"
                              ":summitPwd,"
                              ":headUrl,"
                              ":headPath,"
                              ":isKeepPwd,"
                              ":userSex,"
                              ":token,"
                              ":time,"
                              ":extra1,"
                              ":extra2,"
                              ":extra3,"
                              ":extra4,"
                              ":extra5,"
                              ":extra6);";

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":userId",   loginEntity.userId);
    sqlQuery.bindValue(":userName", loginEntity.userName);
    sqlQuery.bindValue(":userPwd",  loginEntity.userPwd);
    sqlQuery.bindValue(":summitPwd", loginEntity.summitPwd);
    sqlQuery.bindValue(":headUrl",  loginEntity.headUrl);
    sqlQuery.bindValue(":headPath", loginEntity.headPath);
    sqlQuery.bindValue(":isKeepPwd", loginEntity.isKeepPwd);
    sqlQuery.bindValue(":userSex",  loginEntity.userSex);
    sqlQuery.bindValue(":token",    loginEntity.token);
    sqlQuery.bindValue(":time",     loginEntity.time);
    sqlQuery.bindValue(":extra1",   loginEntity.extra1);
    sqlQuery.bindValue(":extra2",   loginEntity.extra2);
    sqlQuery.bindValue(":extra3",   loginEntity.extra3);
    sqlQuery.bindValue(":extra4",   loginEntity.extra4);
    sqlQuery.bindValue(":extra5",   loginEntity.extra5);
    sqlQuery.bindValue(":extra6",   loginEntity.extra6);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, TAG,
                QString("failed insert LOGIN_INFO table with error(%1)")
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    return true;
}

bool LoginEntityManager::delLoginInfo(const QString &userId)
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "LOGIN_INFO"))
    {
        return false;
    }

    QString sql = "DELETE FROM LOGIN_INFO WHERE userId = :userId;";
    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":userId", userId);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed delete LOGIN_INFO with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool LoginEntityManager::getLoginInfo(const QString &userId, LoginEntity &loginEntity)
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "LOGIN_INFO"))
    {
        return false;
    }

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM LOGIN_INFO WHERE userId = ?;";
    sqlQuery.prepare(sql);
    sqlQuery.addBindValue(userId);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query LOGIN_INFO with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    if (sqlQuery.next())
    {
        loginEntity.userId = sqlQuery.value("userId").toString();
        loginEntity.userName = sqlQuery.value("userName").toString();
        loginEntity.userPwd = sqlQuery.value("userPwd").toString();
        loginEntity.summitPwd = sqlQuery.value("summitPwd").toString();
        loginEntity.headUrl = sqlQuery.value("headUrl").toString();
        loginEntity.headPath = sqlQuery.value("headPath").toString();
        loginEntity.isKeepPwd = sqlQuery.value("isKeepPwd").toBool();
        loginEntity.userSex = sqlQuery.value("userSex").toInt();
        loginEntity.token = sqlQuery.value("token").toString();
        loginEntity.time = sqlQuery.value("time").toString();
        loginEntity.extra1 = sqlQuery.value("extra1").toString();
        loginEntity.extra2 = sqlQuery.value("extra2").toString();
        loginEntity.extra3 = sqlQuery.value("extra3").toString();
        loginEntity.extra4 = sqlQuery.value("extra4").toString();
        loginEntity.extra5 = sqlQuery.value("extra5").toString();
        loginEntity.extra6 = sqlQuery.value("extra6").toString();
    }

    return true;
}

bool LoginEntityManager::getAllLoginInfo(LoginEntityList &loginList)
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "LOGIN_INFO"))
    {
        return false;
    }

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM LOGIN_INFO ORDER BY time DESC;";
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query LOGIN_INFO with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    while (sqlQuery.next())
    {
        LoginEntity loginEntity;
        loginEntity.userId = sqlQuery.value("userId").toString();
        loginEntity.userName = sqlQuery.value("userName").toString();
        loginEntity.userPwd = sqlQuery.value("userPwd").toString();
        loginEntity.summitPwd = sqlQuery.value("summitPwd").toString();
        loginEntity.headUrl = sqlQuery.value("headUrl").toString();
        loginEntity.headPath = sqlQuery.value("headPath").toString();
        loginEntity.isKeepPwd = sqlQuery.value("isKeepPwd").toBool();
        loginEntity.userSex = sqlQuery.value("userSex").toInt();
        loginEntity.token = sqlQuery.value("token").toString();
        loginEntity.time = sqlQuery.value("time").toString();
        loginEntity.extra1 = sqlQuery.value("extra1").toString();
        loginEntity.extra2 = sqlQuery.value("extra2").toString();
        loginEntity.extra3 = sqlQuery.value("extra3").toString();
        loginEntity.extra4 = sqlQuery.value("extra4").toString();
        loginEntity.extra5 = sqlQuery.value("extra5").toString();
        loginEntity.extra6 = sqlQuery.value("extra6").toString();

        loginList.append(loginEntity);
    }

    return true;
}

bool LoginEntityManager::updateLoginInfo(MapConditions conditions, MapValues values)
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "LOGIN_INFO"))
    {
        return false;
    }

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("UPDATE LOGIN_INFO SET ").append(DBHelper::formatValueCondition(values, conditions));
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed update LOGIN_INFO with error(%1)")
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    return true;
}
