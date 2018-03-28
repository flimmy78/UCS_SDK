#include "ContactEntityManager.h"
#include "UCSLogger.h"
#include "DBConnectionPool.h"
#include "DBHelper.h"

#define TAG "ContactTable"

ContactEntityManager::ContactEntityManager()
{
    m_createSql = "CREATE TABLE IF NOT EXISTS CONTACT_TABLE ("
                        "contactId   INTEGER PRIMARY KEY UNIQUE,"
                        "sectionName TEXT,"
                        "sectionId   TEXT,"
                        "parentId    TEXT,"
                        "parentName  TEXT,"
                        "grade       INTEGER,"
                        "sortNum     INTEGER,"
                        "userNum     INTEGER,"
                        "userId      TEXT,"
                        "userName    TEXT,"
                        "userPinyin  TEXT,"
                        "userSex     INTEGER,"
                        "headUrl     TEXT,"
                        "headPath    TEXT,"
                        "extra1      INTEGER,"
                        "extra2      INTEGER,"
                        "extra3      INTEGER,"
                        "extra4      TEXT,"
                        "extra5      TEXT,"
                        "extra6      TEXT);";

    m_insertSql = "INSERT OR REPLACE INTO CONTACT_TABLE ("
                          "contactId,"
                          "sectionName,"
                          "sectionId,"
                          "parentId,"
                          "parentName,"
                          "grade,"
                          "sortNum,"
                          "userNum,"
                          "userId,"
                          "userName,"
                          "userPinyin,"
                          "userSex,"
                          "headUrl,"
                          "headPath,"
                          "extra1,"
                          "extra2,"
                          "extra3,"
                          "extra4,"
                          "extra5,"
                          "extra6"
                        " ) VALUES ("
                          ":contactId,"
                          ":sectionName,"
                          ":sectionId,"
                          ":parentId,"
                          ":parentName,"
                          ":grade,"
                          ":sortNum,"
                          ":userNum,"
                          ":userId,"
                          ":userName,"
                          ":userPinyin,"
                          ":userSex,"
                          ":headUrl,"
                          ":headPath,"
                          ":extra1,"
                          ":extra2,"
                          ":extra3,"
                          ":extra4,"
                          ":extra5,"
                          ":extra6);";
}

bool ContactEntityManager::addContact(const ContactEntity &contact)
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "CONTACT_TABLE"))
    {
        return false;
    }

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    sqlQuery.prepare(m_insertSql);
    sqlQuery.bindValue(":contactId", contact.contactId);
    sqlQuery.bindValue(":sectionName", contact.sectionName);
    sqlQuery.bindValue(":sectionId",contact.sectionId);
    sqlQuery.bindValue(":parentId", contact.parentId);
    sqlQuery.bindValue(":parentName", contact.parentName);
    sqlQuery.bindValue(":grade", contact.grade);
    sqlQuery.bindValue(":sortNum", contact.sortNum);
    sqlQuery.bindValue(":userNum", contact.userNum);
    sqlQuery.bindValue(":userId", contact.userId);
    sqlQuery.bindValue(":userName", contact.userName);
    sqlQuery.bindValue(":userPinyin", contact.userPinyin);
    sqlQuery.bindValue(":userSex", contact.userSex);
    sqlQuery.bindValue(":headUrl", contact.headUrl);
    sqlQuery.bindValue(":headPath", contact.headPath);
    sqlQuery.bindValue(":extra1", contact.extra1);
    sqlQuery.bindValue(":extra2", contact.extra2);
    sqlQuery.bindValue(":extra3", contact.extra3);
    sqlQuery.bindValue(":extra4", contact.extra4);
    sqlQuery.bindValue(":extra5", contact.extra5);
    sqlQuery.bindValue(":extra6", contact.extra6);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, TAG,
                QString("failed insert CONTACT_TABLE table with error(%1)")
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    return true;
}

bool ContactEntityManager::addContacts(const ContactEntityList &contactList)
{
    if (contactList.isEmpty())
    {
        return false;
    }

    if (!DBHelper::checkAndCreateTable(m_createSql, "CONTACT_TABLE"))
    {
        return false;
    }

    if (DBHelper::transactionSupported())
    {
        DBScoped scoped;
        QSqlDatabase db = scoped.db();
        bool result = true;
        QSqlQuery sqlQuery(db);

        db.transaction();

        QTime time;
        time.start();

        sqlQuery.prepare(m_insertSql);

        foreach (ContactEntity contact, contactList)
        {
            sqlQuery.bindValue(":contactId", contact.contactId);
            sqlQuery.bindValue(":sectionName", contact.sectionName);
            sqlQuery.bindValue(":sectionId",contact.sectionId);
            sqlQuery.bindValue(":parentId", contact.parentId);
            sqlQuery.bindValue(":parentName", contact.parentName);
            sqlQuery.bindValue(":grade", contact.grade);
            sqlQuery.bindValue(":sortNum", contact.sortNum);
            sqlQuery.bindValue(":userNum", contact.userNum);
            sqlQuery.bindValue(":userId", contact.userId);
            sqlQuery.bindValue(":userName", contact.userName);
            sqlQuery.bindValue(":userPinyin", contact.userPinyin);
            sqlQuery.bindValue(":userSex", contact.userSex);
            sqlQuery.bindValue(":headUrl", contact.headUrl);
            sqlQuery.bindValue(":headPath", contact.headPath);
            sqlQuery.bindValue(":extra1", contact.extra1);
            sqlQuery.bindValue(":extra2", contact.extra2);
            sqlQuery.bindValue(":extra3", contact.extra3);
            sqlQuery.bindValue(":extra4", contact.extra4);
            sqlQuery.bindValue(":extra5", contact.extra5);
            sqlQuery.bindValue(":extra6", contact.extra6);

            if (!sqlQuery.exec())
            {
                UCS_LOG(UCSLogger::kTraceError, TAG,
                        QString("failed insert CONTACT_TABLE table with error(%1)")
                                .arg(sqlQuery.lastError().text()));
                result = false;
                break;
            }
        }
        if (result)
        {
            db.commit();

            UCS_LOG(UCSLogger::kTraceDebug, TAG, QString("insert CONTACT_TABLE elapsed time: %1 ms").arg(time.elapsed()));
            return true;
        }

        db.rollback();
        return false;
    }
    else
    {
        foreach (ContactEntity contact, contactList)
        {
            bool result = addContact(contact);
            if (!result)
            {
                return false;
            }
        }

        return true;
    }
}

bool ContactEntityManager::getContact(const int &contactId, ContactEntity &contact)
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "CONTACT_TABLE"))
    {
        return false;
    }

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM CONTACT_TABLE WHERE contactId = ?;";
    sqlQuery.prepare(sql);
    sqlQuery.addBindValue(contactId);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query CONTACT_TABLE with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    while (sqlQuery.next())
    {
        contact.contactId = sqlQuery.value("contactId").toInt();
        contact.sectionName = sqlQuery.value("sectionName").toString();
        contact.sectionId = sqlQuery.value("sectionId").toString();
        contact.parentId = sqlQuery.value("parentId").toString();
        contact.parentName = sqlQuery.value("parentName").toString();
        contact.grade = sqlQuery.value("grade").toInt();
        contact.sortNum = sqlQuery.value("sortNum").toInt();
        contact.userNum = sqlQuery.value("userNum").toInt();
        contact.userId = sqlQuery.value("userId").toString();
        contact.userName = sqlQuery.value("userName").toString();
        contact.userPinyin = sqlQuery.value("userPinyin").toString();
        contact.userSex = sqlQuery.value("userSex").toInt();
        contact.headUrl = sqlQuery.value("headUrl").toString();
        contact.headPath = sqlQuery.value("headPath").toString();
        contact.extra1 = sqlQuery.value("extra1").toString();
        contact.extra2 = sqlQuery.value("extra2").toString();
        contact.extra3 = sqlQuery.value("extra3").toString();
        contact.extra4 = sqlQuery.value("extra4").toString();
        contact.extra5 = sqlQuery.value("extra5").toString();
        contact.extra6 = sqlQuery.value("extra6").toString();
    }

    return true;
}

bool ContactEntityManager::getContact(const QString userId, ContactEntity &contact)
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "CONTACT_TABLE"))
    {
        return false;
    }

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM CONTACT_TABLE WHERE userId = :userId LIMIT 1;";
    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":userId", userId);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query CONTACT_TABLE with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    while (sqlQuery.next())
    {
        contact.contactId = sqlQuery.value("contactId").toInt();
        contact.sectionName = sqlQuery.value("sectionName").toString();
        contact.sectionId = sqlQuery.value("sectionId").toString();
        contact.parentId = sqlQuery.value("parentId").toString();
        contact.parentName = sqlQuery.value("parentName").toString();
        contact.grade = sqlQuery.value("grade").toInt();
        contact.sortNum = sqlQuery.value("sortNum").toInt();
        contact.userNum = sqlQuery.value("userNum").toInt();
        contact.userId = sqlQuery.value("userId").toString();
        contact.userName = sqlQuery.value("userName").toString();
        contact.userPinyin = sqlQuery.value("userPinyin").toString();
        contact.userSex = sqlQuery.value("userSex").toInt();
        contact.headUrl = sqlQuery.value("headUrl").toString();
        contact.headPath = sqlQuery.value("headPath").toString();
        contact.extra1 = sqlQuery.value("extra1").toString();
        contact.extra2 = sqlQuery.value("extra2").toString();
        contact.extra3 = sqlQuery.value("extra3").toString();
        contact.extra4 = sqlQuery.value("extra4").toString();
        contact.extra5 = sqlQuery.value("extra5").toString();
        contact.extra6 = sqlQuery.value("extra6").toString();
    }

    return true;
}

bool ContactEntityManager::getAllContacts(ContactEntityList &contactList)
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "CONTACT_TABLE"))
    {
        return false;
    }

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM CONTACT_TABLE;";
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query CONTACT_TABLE with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    while (sqlQuery.next())
    {
        ContactEntity contact;
        contact.contactId = sqlQuery.value("contactId").toInt();
        contact.sectionName = sqlQuery.value("sectionName").toString();
        contact.sectionId = sqlQuery.value("sectionId").toString();
        contact.parentId = sqlQuery.value("parentId").toString();
        contact.parentName = sqlQuery.value("parentName").toString();
        contact.grade = sqlQuery.value("grade").toInt();
        contact.sortNum = sqlQuery.value("sortNum").toInt();
        contact.userNum = sqlQuery.value("userNum").toInt();
        contact.userId = sqlQuery.value("userId").toString();
        contact.userName = sqlQuery.value("userName").toString();
        contact.userPinyin = sqlQuery.value("userPinyin").toString();
        contact.userSex = sqlQuery.value("userSex").toInt();
        contact.headUrl = sqlQuery.value("headUrl").toString();
        contact.headPath = sqlQuery.value("headPath").toString();
        contact.extra1 = sqlQuery.value("extra1").toString();
        contact.extra2 = sqlQuery.value("extra2").toString();
        contact.extra3 = sqlQuery.value("extra3").toString();
        contact.extra4 = sqlQuery.value("extra4").toString();
        contact.extra5 = sqlQuery.value("extra5").toString();
        contact.extra6 = sqlQuery.value("extra6").toString();

        contactList.append(contact);
    }

    return true;
}

bool ContactEntityManager::clearContacts()
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "CONTACT_TABLE"))
    {
        return false;
    }

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "DELETE FROM CONTACT_TABLE;";
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed clear CONTACT_TABLE with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool ContactEntityManager::updateContact(MapConditions condition, MapValues values)
{
    if (!DBHelper::checkAndCreateTable(m_createSql, "CONTACT_TABLE"))
    {
        return false;
    }

    DBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("UPDATE CONTACT_TABLE SET ").append(DBHelper::formatValueCondition(values, condition));
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed update CONTACT_TABLE with error(%1)")
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    return true;
}
