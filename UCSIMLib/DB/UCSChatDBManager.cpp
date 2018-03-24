#include "UCSChatDBManager.h"
#include <UCSDBHelper.h>
#include <UCSConnectionPool.h>
#include <UCSTcp/UCSLogger.h>
#include <UCSClock.h>
#include <QStringUtil.h>
#include <UCSIMHelper.h>

ChatDBManager::ChatDBManager()
{
    m_createSql = "CREATE TABLE IF NOT EXISTS %1 ("
                            "chatId         INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "msgId          TEXT,"
                            "imsgId         TEXT UNIQUE,"
                            "targetId       TEXT,"
                            "senderId       TEXT,"
                            "senderNickName TEXT,"
                            "categoryId     TEXT,"
                            "isFromMySelf   TEXT,"
                            "sendTime       TEXT,"
                            "receiveTime    TEXT,"
                            "msgType        TEXT,"
                            "content        TEXT,"
                            "readStatus     TEXT,"
                            "sendStatus     TEXT,"
                            "msgLength      TEXT,"
                            "thumbnaiUrl    TEXT,"
                            "address        TEXT,"
                            "coordType      TEXT,"
                            "latitude       REAL,"
                            "longitude      REAL,"
                            "extra1         INTEGER,"
                            "extra2         INTEGER,"
                            "extra3         INTEGER,"
                            "extra4         TEXT,"
                            "extra5         TEXT,"
                            "extra6         TEXT);";

    m_insertSql = "INSERT OR REPLACE INTO %1 ("
                  "msgId,"
                  "imsgId,"
                  "targetId,"
                  "senderId,"
                  "senderNickName,"
                  "categoryId,"
                  "isFromMySelf,"
                  "sendTime,"
                  "receiveTime,"
                  "msgType,"
                  "content,"
                  "readStatus,"
                  "sendStatus,"
                  "msgLength,"
                  "thumbnaiUrl,"
                  "address,"
                  "coordType,"
                  "latitude,"
                  "longitude,"
                  "extra1,"
                  "extra2,"
                  "extra3,"
                  "extra4,"
                  "extra5,"
                  "extra6"
              ") VALUES ("
                  ":msgId,"
                  ":imsgId,"
                  ":targetId,"
                  ":senderId,"
                  ":senderNickName,"
                  ":categoryId,"
                  ":isFromMySelf,"
                  ":sendTime,"
                  ":receiveTime,"
                  ":msgType,"
                  ":content,"
                  ":readStatus,"
                  ":sendStatus,"
                  ":msgLength,"
                  ":thumbnaiUrl,"
                  ":address,"
                  ":coordType,"
                  ":latitude,"
                  ":longitude,"
                  ":extra1,"
                  ":extra2,"
                  ":extra3,"
                  ":extra4,"
                  ":extra5,"
                  ":extra6);";
}

bool ChatDBManager::addChat(const QString targetId, const UCS_IM_ConversationType type, const ChatEntity chatEntity)
{
    if (!checkAndCreateTable(targetId, type))
    {
        return false;
    }

    if (checkChatExist(targetId, type, chatEntity.imsgId))
    {
        // do nothing if exist
        return true;
    }
    else
    {
        UCSDBScoped scoped;
        QSqlDatabase db = scoped.db();
        QSqlQuery sqlQuery(db);

        QString sql = m_insertSql.arg(tableName(targetId, type));

        sqlQuery.prepare(sql);
        sqlQuery.bindValue(":msgId", chatEntity.msgId);
        sqlQuery.bindValue(":imsgId", chatEntity.imsgId);
        sqlQuery.bindValue(":targetId", chatEntity.targetId);
        sqlQuery.bindValue(":senderId", chatEntity.senderId);
        sqlQuery.bindValue(":senderNickName", chatEntity.senderNickName);
        sqlQuery.bindValue(":categoryId", chatEntity.categoryId);
        sqlQuery.bindValue(":isFromMySelf", chatEntity.isFromMySelf);
        sqlQuery.bindValue(":sendTime", chatEntity.sendTime);
        sqlQuery.bindValue(":receiveTime", chatEntity.receivedTime);
        sqlQuery.bindValue(":msgType", chatEntity.msgType);
        sqlQuery.bindValue(":content", chatEntity.content);
        sqlQuery.bindValue(":readStatus", chatEntity.readStatus);
        sqlQuery.bindValue(":sendStatus", chatEntity.sendStatus);
        sqlQuery.bindValue(":msgLength", chatEntity.msgLength);
        sqlQuery.bindValue(":thumbnaiUrl", chatEntity.thumbnaiUrl);
        sqlQuery.bindValue(":address", chatEntity.address);
        sqlQuery.bindValue(":coordType", chatEntity.coordType);
        sqlQuery.bindValue(":latitude", chatEntity.latitude);
        sqlQuery.bindValue(":longitude", chatEntity.longitude);
        sqlQuery.bindValue(":extra1", chatEntity.extra1);
        sqlQuery.bindValue(":extra2", chatEntity.extra2);
        sqlQuery.bindValue(":extra3", chatEntity.extra3);
        sqlQuery.bindValue(":extra4", chatEntity.extra4);
        sqlQuery.bindValue(":extra5", chatEntity.extra5);
        sqlQuery.bindValue(":extra6", chatEntity.extra6);

        if (!sqlQuery.exec())
        {
            UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                    QString("failed insert %1 with error(%2)")
                            .arg(tableName(targetId, type))
                            .arg(sqlQuery.lastError().text()));

            return false;
        }
    }
    return true;
}

bool ChatDBManager::addChat(const QString targetId,
                            const UCS_IM_ConversationType type,
                            const QList<ChatEntity> chatList)
{
    if (chatList.isEmpty())
    {
        return false;
    }

    if (!checkAndCreateTable(targetId, type))
    {
        return false;
    }

    if (UCSDBHelper::transactionSupported())
    {
        bool result = true;
        UCSDBScoped scoped;
        QSqlDatabase db = scoped.db();
        QSqlQuery sqlQuery(db);

        db.transaction();

        QString sql = m_insertSql.arg(tableName(targetId, type));
        sqlQuery.prepare(sql);

        foreach (ChatEntity chatEntity, chatList)
        {
            sqlQuery.bindValue(":msgId", chatEntity.msgId);
            sqlQuery.bindValue(":imsgId", chatEntity.imsgId);
            sqlQuery.bindValue(":targetId", chatEntity.targetId);
            sqlQuery.bindValue(":senderId", chatEntity.senderId);
            sqlQuery.bindValue(":senderNickName", chatEntity.senderNickName);
            sqlQuery.bindValue(":categoryId", chatEntity.categoryId);
            sqlQuery.bindValue(":isFromMySelf", chatEntity.isFromMySelf);
            sqlQuery.bindValue(":sendTime", chatEntity.sendTime);
            sqlQuery.bindValue(":receiveTime", chatEntity.receivedTime);
            sqlQuery.bindValue(":msgType", chatEntity.msgType);
            sqlQuery.bindValue(":content", chatEntity.content);
            sqlQuery.bindValue(":readStatus", chatEntity.readStatus);
            sqlQuery.bindValue(":sendStatus", chatEntity.sendStatus);
            sqlQuery.bindValue(":msgLength", chatEntity.msgLength);
            sqlQuery.bindValue(":thumbnaiUrl", chatEntity.thumbnaiUrl);
            sqlQuery.bindValue(":address", chatEntity.address);
            sqlQuery.bindValue(":coordType", chatEntity.coordType);
            sqlQuery.bindValue(":latitude", chatEntity.latitude);
            sqlQuery.bindValue(":longitude", chatEntity.longitude);
            sqlQuery.bindValue(":extra1", chatEntity.extra1);
            sqlQuery.bindValue(":extra2", chatEntity.extra2);
            sqlQuery.bindValue(":extra3", chatEntity.extra3);
            sqlQuery.bindValue(":extra4", chatEntity.extra4);
            sqlQuery.bindValue(":extra5", chatEntity.extra5);
            sqlQuery.bindValue(":extra6", chatEntity.extra6);

            if (!sqlQuery.exec())
            {
                UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                        QString("failed insert %1 with error(%2)")
                                .arg(tableName(targetId, type))
                                .arg(sqlQuery.lastError().text()));

                result = false;
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
        for (qint32 i = 0; i < chatList.size(); i++)
        {
            bool result = addChat(targetId, type, chatList.at(i));
            if (result == false)
            {
                return false;
            }
        }

        return true;
    }
}

bool ChatDBManager::delChat(const QString targetId,
                            const UCS_IM_ConversationType type,
                            const QString msgId)
{
    if (!checkAndCreateTable(targetId, type))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("DELETE FROM %1 WHERE msgId = :msgId;").arg(tableName(targetId, type));
    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":msgId", msgId);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed delete %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool ChatDBManager::clearChats(const QString targetId,
                               const UCS_IM_ConversationType type)
{
    if (!checkAndCreateTable(targetId, type))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);
    QString sql = QString("DELETE FROM %1;").arg(tableName(targetId, type));
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed clear %1 table with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool ChatDBManager::updateChat(const QString targetId,
                               const UCS_IM_ConversationType type,
                               MapValues values,
                               MapConditions conditions)
{
    if (!checkAndCreateTable(targetId, type))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("UPDATE %1 SET ")
            .arg(tableName(targetId, type))
            .append(UCSDBHelper::formatValueCondition(values, conditions));

    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed update target %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    return true;
}

bool ChatDBManager::updateChatReceivedStatus(const QString targetId,
                                             const UCS_IM_ConversationType type,
                                             const QString msgId,
                                             const UCSReceivedStatus status)
{
    MapValues values;
    MapConditions conditions;

    values["readstatus"] = QString::number(status);
    conditions["msgId"] = msgId;

    return updateChat(targetId, type, values, conditions);
}

bool ChatDBManager::getChat(const QString targetId,
                            const UCS_IM_ConversationType type,
                            const QString msgId,
                            ChatEntity &chatEntity)
{
    if (!checkAndCreateTable(targetId, type))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("SELECT * FROM %1 WHERE msgId = :msgId")
                        .arg(tableName(targetId, type));

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":msgId", msgId);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed get target chat %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    if (sqlQuery.next())
    {
        query2Entity(&sqlQuery, &chatEntity);

        QString loginTime = UCSIMHelper::readSettings(kUcsSettingsKeyLoginTime).toString();
        if (((UCSClock::TimeInMicroseconds() - chatEntity.sendTime.toLongLong() > 30) ||
            (chatEntity.sendTime.toLongLong() < loginTime.toLongLong()) ) &&
            (chatEntity.sendStatus.toInt() == SendStatus_sending))
        {
            chatEntity.sendStatus = QString::number(SendStatus_fail);
        }
    }

    return true;
}

bool ChatDBManager::checkChatExist(const QString targetId,
                                   const UCS_IM_ConversationType type,
                                   const QString iMsgId)
{
    if (!checkAndCreateTable(targetId, type))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("SELECT * FROM %1 WHERE imsgId = :imsgId")
                        .arg(tableName(targetId, type));

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":imsgId", iMsgId);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed get target chat %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    if (sqlQuery.next())
    {
        return true;
    }

    return false;
}

bool ChatDBManager::getNewestChat(const QString targetId, const UCS_IM_ConversationType type, ChatEntity &chatEntity)
{
    if (!checkAndCreateTable(targetId, type))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("SELECT * FROM %1 ORDER BY sendTime DESC LIMIT 1;")
                        .arg(tableName(targetId, type));

    sqlQuery.prepare(sql);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed get target chat %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));
        return false;
    }

    if (sqlQuery.next())
    {
        query2Entity(&sqlQuery, &chatEntity);

        QString loginTime = UCSIMHelper::readSettings(kUcsSettingsKeyLoginTime).toString();
        if (((UCSClock::TimeInMicroseconds() - chatEntity.sendTime.toLongLong() > 30) ||
            (chatEntity.sendTime.toLongLong() < loginTime.toLongLong()) ) &&
            (chatEntity.sendStatus.toInt() == SendStatus_sending))
        {
            chatEntity.sendStatus = QString::number(SendStatus_fail);
        }
    }

    return true;
}

QList<ChatEntity> ChatDBManager::getChats(const QString targetId,
                                          const UCS_IM_ConversationType type,
                                          const int count)
{
    QList<ChatEntity> chatList;
    if (!checkAndCreateTable(targetId, type))
    {
        return chatList;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("SELECT * FROM %1 ORDER BY sendTime DESC LIMIT :count")
                        .arg(tableName(targetId, type));

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":count", count);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed get target chat %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));
        return chatList;
    }

    while (sqlQuery.next())
    {
        ChatEntity chatEntity;
        query2Entity(&sqlQuery, &chatEntity);
        chatList.append(chatEntity);
    }

    return chatList;
}

QList<ChatEntity> ChatDBManager::getHistoryChats(const QString targetId,
                                          const UCS_IM_ConversationType type,
                                          const int count,
                                          const QString oldestMessageId)
{
    QList<ChatEntity> chatList;
    if (!checkAndCreateTable(targetId, type))
    {
        return chatList;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);
    QString chatId = getChatId(targetId, type, oldestMessageId);
    if (chatId.isEmpty())
    {
        return chatList;
    }

    QString sql = QString("SELECT * FROM %1 WHERE chatId < :chatId ORDER BY chatId DESC LIMIT :count")
                        .arg(tableName(targetId, type));

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":chatId", chatId);
    sqlQuery.bindValue(":count", count);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed get target chat %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));
        return chatList;
    }

    while (sqlQuery.next())
    {
        ChatEntity chatEntity;
        query2Entity(&sqlQuery, &chatEntity);
        chatList.append(chatEntity);
    }

    return chatList;
}

quint32 ChatDBManager::getUnReadCount(const QString targetId, const UCS_IM_ConversationType type)
{
    if (!checkAndCreateTable(targetId, type))
    {
        return 0;
    }

    QString strIn = QString("(%1, %2, %3, %4)")
                        .arg(ReceivedStatus_UNREAD)
                        .arg(ReceivedStatus_DOWNLOADED)
                        .arg(ReceivedStatus_DOWNLOADING)
                        .arg(ReceivedStatus_DOWNLOADFail);

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("SELECT COUNT(*) FROM %1 WHERE readStatus IN ")
                        .arg(tableName(targetId, type))
                        .append(strIn);

    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed get target chat %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));
        return 0;
    }

    if (sqlQuery.next())
    {
        return sqlQuery.value(0).toInt();
    }

    return 0;
}

QList<ChatEntity> ChatDBManager::searchChats(const QString targetId,
                                             const UCS_IM_ConversationType type,
                                             const QString keyWord)
{
    QList<ChatEntity> chatList;
    if (!checkAndCreateTable(targetId, type))
    {
        return chatList;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("SELECT * from %1 WHERE content like '%%2%'")
                        .arg(tableName(targetId, type)
                        .arg(keyWord));

    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed search chat %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));
        return chatList;
    }

    while (sqlQuery.next())
    {
        ChatEntity chatEntity;
        query2Entity(&sqlQuery, &chatEntity);
        chatList.append(chatEntity);
    }

    return chatList;
}

bool ChatDBManager::dropTable(const QString &targetId, const UCS_IM_ConversationType &type)
{
    if (!UCSDBHelper::checkTableExist(tableName(targetId, type)))
    {
        return true;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("DROP TABLE %1;").arg(tableName(targetId, type));
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed drop table %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

QString ChatDBManager::tableName(const QString targetId,
                                 const UCS_IM_ConversationType type)
{
    QString name;
    switch (type) {
    case UCS_IM_SOLOCHAT:
        name = "UCS_IM_SOLOCHAT_";
        break;

    case UCS_IM_GROUPCHAT:
        name = "UCS_IM_GROUPCHAT_";
        break;

    case UCS_IM_DISCUSSIONCHAT:
        name = "UCS_IM_DISCUSSIONCHAT_";
        break;

    case UCS_IM_Broadcast:
        name = "UCS_IM_Broadcast_";
        break;

    case UCS_IM_UnknownConversationType:
    default:
        name = "UCS_IM_UnknownConversationType_";
        break;
    }

    name.append(targetId);

    return name;
}

bool ChatDBManager::checkAndCreateTable(const QString targetId, const UCS_IM_ConversationType type)
{
    QString tblName = tableName(targetId, type);
    QString createSql = m_createSql.arg(tblName);
    if (!UCSDBHelper::checkAndCreateTable(createSql, tblName))
    {
        return false;
    }

    return true;
}

void ChatDBManager::query2Entity(const QSqlQuery *sqlQuery, ChatEntity *chatEntity)
{
    chatEntity->msgId = sqlQuery->value("msgId").toString();
    chatEntity->imsgId = sqlQuery->value("imsgId").toString();
    chatEntity->targetId = sqlQuery->value("targetId").toString();
    chatEntity->senderId = sqlQuery->value("senderId").toString();
    chatEntity->senderNickName = sqlQuery->value("senderNickName").toString();
    chatEntity->categoryId = sqlQuery->value("categoryId").toString();
    chatEntity->isFromMySelf = sqlQuery->value("isFromMySelf").toString();
    chatEntity->sendTime = sqlQuery->value("sendTime").toString();
    chatEntity->receivedTime = sqlQuery->value("receiveTime").toString();
    chatEntity->msgType = sqlQuery->value("msgType").toString();
    chatEntity->content = sqlQuery->value("content").toString();
    chatEntity->readStatus = sqlQuery->value("readStatus").toString();
    chatEntity->sendStatus = sqlQuery->value("sendStatus").toString();
    chatEntity->msgLength = sqlQuery->value("msgLength").toString();
    chatEntity->thumbnaiUrl = sqlQuery->value("thumbnaiUrl").toString();
    chatEntity->address = sqlQuery->value("address").toString();
    chatEntity->coordType = sqlQuery->value("coordType").toString();
    chatEntity->latitude = sqlQuery->value("latitude").toString();
    chatEntity->longitude = sqlQuery->value("longitude").toString();
    chatEntity->extra1 = sqlQuery->value("extra1").toString();
    chatEntity->extra2 = sqlQuery->value("extra2").toString();
    chatEntity->extra3 = sqlQuery->value("extra3").toString();
    chatEntity->extra4 = sqlQuery->value("extra4").toString();
    chatEntity->extra5 = sqlQuery->value("extra5").toString();
    chatEntity->extra6 = sqlQuery->value("extra6").toString();
}

QString ChatDBManager::getChatId(const QString targetId, const UCS_IM_ConversationType type, const QString msgId)
{
    if (!checkAndCreateTable(targetId, type))
    {
        return QString("");
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("SELECT chatId FROM %1 WHERE msgId = :msgId")
                        .arg(tableName(targetId, type));

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":msgId", msgId);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed get target chat %1 with error(%2)")
                        .arg(tableName(targetId, type))
                        .arg(sqlQuery.lastError().text()));
        return QString("");
    }

    if (sqlQuery.next())
    {
        return sqlQuery.value("chatId").toString();
    }

    return QString("");
}
