#include "UCSConversationDBManager.h"
#include <UCSDBHelper.h>
#include <UCSConnectionPool.h>
#include <UCSTcp/UCSLogger.h>
#include <UCSClock.h>
#include <QStringUtil.h>
#include <UCSDBCenter.h>

ConversationDBManager::ConversationDBManager(DiscussionDBManager *pDiscussionMgr)
    : m_disscussionMgr(pDiscussionMgr)
{
    m_createSql = "CREATE TABLE IF NOT EXISTS UCS_IM_CONVERSATION ("
                          "indexId           INTEGER,"
                          "targetId          TEXT    PRIMARY KEY NOT NULL,"
                          "conversationTitle TEXT,"
                          "categoryId        INTEGER,"
                          "draft             TEXT,"
                          "isTop             BOOLEAN,"
                          "lastTime          TEXT,"
                          "topTime           TEXT,"
                          "unReadMsgCount    INTEGER DEFAULT (0),"
                          "lastestMessageId  TEXT,"
                          "localTime         TEXT,"
                          "extra1            INTEGER,"
                          "extra2            INTEGER,"
                          "extra3            INTEGER,"
                          "extra4            TEXT,"
                          "extra5            TEXT,"
                          "extra6            TEXT)";
}

bool ConversationDBManager::addConversation(const ConversationEntity *pConversation)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_CONVERSATION"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "INSERT OR REPLACE INTO UCS_IM_CONVERSATION ("
                                "indexId,"
                                "targetId,"
                                "conversationTitle,"
                                "categoryId,"
                                "draft,"
                                "isTop,"
                                "lastTime,"
                                "topTime,"
                                "unReadMsgCount,"
                                "lastestMessageId,"
                                "localTime,"
                                "extra1,"
                                "extra2,"
                                "extra3,"
                                "extra4,"
                                "extra5,"
                                "extra6"
                            " ) VALUES ("
                                ":indexId,"
                                ":targetId,"
                                ":conversationTitle,"
                                ":categoryId,"
                                ":draft,"
                                ":isTop,"
                                ":lastTime,"
                                ":topTime,"
                                ":unReadMsgCount,"
                                ":lastestMessageId,"
                                ":localTime,"
                                ":extra1,"
                                ":extra2,"
                                ":extra3,"
                                ":extra4,"
                                ":extra5,"
                                ":extra6);";

    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":indexId", maxIndexId() + 1);
    sqlQuery.bindValue(":targetId", pConversation->targetId);
    sqlQuery.bindValue(":conversationTitle", pConversation->conversationTitle);
    sqlQuery.bindValue(":categoryId", pConversation->categoryId);
    sqlQuery.bindValue(":draft", pConversation->draft);
    sqlQuery.bindValue(":isTop", pConversation->isTop);
    sqlQuery.bindValue(":lastTime", pConversation->lastTime);
    sqlQuery.bindValue(":topTime", pConversation->topTime);
    sqlQuery.bindValue(":unReadMsgCount", pConversation->unReadMsgCount);
    sqlQuery.bindValue(":lastestMessageId", pConversation->lastestMessageId);
    sqlQuery.bindValue(":localTime", pConversation->localTime);
    sqlQuery.bindValue(":extra1", pConversation->extra1);
    sqlQuery.bindValue(":extra2", pConversation->extra2);
    sqlQuery.bindValue(":extra3", pConversation->extra3);
    sqlQuery.bindValue(":extra4", pConversation->extra4);
    sqlQuery.bindValue(":extra5", pConversation->extra5);
    sqlQuery.bindValue(":extra6", pConversation->extra6);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed insert conversation with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool ConversationDBManager::addConversation(const DiscussionEntity discussion)
{
    QString timeNow = QString::number(UCSClock::TimeInMicroseconds());

    ///< 本地数据库尚不存在该讨论组会话，创建之 >
    ConversationEntity conversation;
    conversation.targetId = discussion.discussionId;
    conversation.conversationTitle = discussion.discussionName;
    conversation.categoryId = QString::number(UCS_IM_DISCUSSIONCHAT);
    conversation.lastTime = timeNow;
    conversation.localTime = timeNow;

    UCSDBCenter::conversationMgr()->addConversation(&conversation);

    return true;
}

bool ConversationDBManager::addConversation(const ChatEntity *chatEntity, const quint32 count)
{
    UCS_IM_ConversationType conversationType = (UCS_IM_ConversationType)chatEntity->categoryId.toInt();
    QString targetId = chatEntity->targetId;
    bool isSender = QStringUtil::toBool(chatEntity->isFromMySelf);
    ConversationEntity oldConversation;
    bool result = getConversation(conversationType, targetId, oldConversation);

    ConversationEntity newConversation;
    newConversation.targetId = targetId;
    newConversation.categoryId = chatEntity->categoryId;
    ///< 本地发送时清除草稿 >
    newConversation.draft = isSender ? "" : oldConversation.draft;

    newConversation.isTop = oldConversation.isTop;
    newConversation.lastTime = chatEntity->sendTime;
    newConversation.topTime = oldConversation.topTime;

    newConversation.localTime = isSender ? chatEntity->sendTime : chatEntity->receivedTime;
    quint32 unReadCount = oldConversation.unReadMsgCount.toInt() + count;
    newConversation.unReadMsgCount = QString::number(unReadCount);
    newConversation.lastestMessageId = chatEntity->msgId;

    if (conversationType == UCS_IM_DISCUSSIONCHAT)
    {
        if (!result)
        {
            DiscussionEntity discussionEntity;
            result = m_disscussionMgr->getDiscussion(targetId, discussionEntity);
            if (result)
            {
                newConversation.conversationTitle = discussionEntity.discussionName;
            }
        }
        else
        {
            newConversation.conversationTitle = oldConversation.conversationTitle;
        }
    }
    else if (conversationType == UCS_IM_SOLOCHAT)
    {
        ///< 收到的消息有昵称,则会话title用昵称表示，否则用targetId表示 >
        /// < 发送的消息，如果会话已经存在，则用旧的会话名称，否则用targetId表示 >
        QString title;
        if (!isSender)
        {
            title = (chatEntity->senderNickName.size() > 0) ? chatEntity->senderNickName : targetId;
        }
        else
        {
            title = (oldConversation.conversationTitle.size() > 0) ? oldConversation.conversationTitle : targetId;
        }
        newConversation.conversationTitle = title;
    }
    else if (conversationType == UCS_IM_GROUPCHAT)
    {
        newConversation.conversationTitle = targetId;
    }

    return addConversation(&newConversation);
}

bool ConversationDBManager::delConversation(const UCS_IM_ConversationType type, const QString targetId)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_CONVERSATION"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "DELETE FROM UCS_IM_CONVERSATION WHERE targetId = :targetId AND categoryId = :categoryId;";
    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":targetId", targetId);
    sqlQuery.bindValue(":categoryId", QString::number(type));

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed delete target conversation with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool ConversationDBManager::clearConversations()
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_CONVERSATION"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "DELETE FROM UCS_IM_CONVERSATION;";
    sqlQuery.prepare(sql);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed clear conversation table with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool ConversationDBManager::getConversation(const UCS_IM_ConversationType type,
                                            const QString targetId,
                                            ConversationEntity &conversation)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_CONVERSATION"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM UCS_IM_CONVERSATION WHERE targetId = :targetId AND categoryId = :categoryId;";
    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":targetId", targetId);
    sqlQuery.bindValue(":categoryId", type);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query target conversation with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    if (sqlQuery.next())
    {
        conversation.indexId = sqlQuery.value("indexId").toString();
        conversation.targetId = sqlQuery.value("targetId").toString();
        conversation.conversationTitle = sqlQuery.value("conversationTitle").toString();
        conversation.categoryId = sqlQuery.value("categoryId").toString();
        conversation.draft = sqlQuery.value("draft").toString();
        conversation.isTop = sqlQuery.value("isTop").toString();
        conversation.lastTime = sqlQuery.value("lastTime").toString();
        conversation.unReadMsgCount = sqlQuery.value("unReadMsgCount").toString();
        conversation.lastestMessageId = sqlQuery.value("lastestMessageId").toString();
        conversation.localTime = sqlQuery.value("localTime").toString();
        conversation.extra1 = sqlQuery.value("extra1").toString();
        conversation.extra2 = sqlQuery.value("extra2").toString();
        conversation.extra3 = sqlQuery.value("extra3").toString();
        conversation.extra4 = sqlQuery.value("extra4").toString();
        conversation.extra5 = sqlQuery.value("extra5").toString();
        conversation.extra6 = sqlQuery.value("extra6").toString();
    }

    return true;
}

QList<ConversationEntity> ConversationDBManager::getConversationList(const QList<qint32> typeList)
{
    QList<ConversationEntity> conversationList;
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_CONVERSATION"))
    {
        return conversationList;
    }

    QString types = QString("%1").arg(typeList.first());
    for (qint32 i = 1; i < typeList.size(); ++i)
    {
        types += QString(", %1").arg(typeList.at(i));
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM UCS_IM_CONVERSATION WHERE categoryId in (:categoryIds) ORDER BY indexId DESC;";
    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":categoryIds", types);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query target conversations with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return conversationList;
    }

    while (sqlQuery.next())
    {
        ConversationEntity conversation;
        conversation.indexId = sqlQuery.value("indexId").toString();
        conversation.targetId = sqlQuery.value("targetId").toString();
        conversation.conversationTitle = sqlQuery.value("conversationTitle").toString();
        conversation.categoryId = sqlQuery.value("categoryId").toString();
        conversation.draft = sqlQuery.value("draft").toString();
        conversation.isTop = sqlQuery.value("isTop").toString();
        conversation.lastTime = sqlQuery.value("lastTime").toString();
        conversation.unReadMsgCount = sqlQuery.value("unReadMsgCount").toString();
        conversation.lastestMessageId = sqlQuery.value("lastestMessageId").toString();
        conversation.localTime = sqlQuery.value("localTime").toString();
        conversation.extra1 = sqlQuery.value("extra1").toString();
        conversation.extra2 = sqlQuery.value("extra2").toString();
        conversation.extra3 = sqlQuery.value("extra3").toString();
        conversation.extra4 = sqlQuery.value("extra4").toString();
        conversation.extra5 = sqlQuery.value("extra5").toString();
        conversation.extra6 = sqlQuery.value("extra6").toString();

        conversationList.append(conversation);
    }

    return conversationList;
}

quint32 ConversationDBManager::getUnReadCount(const UCS_IM_ConversationType type, const QString targetId)
{
    ConversationEntity conversation;
    getConversation(type, targetId, conversation);

    return (quint32)conversation.unReadMsgCount.toInt();
}

quint32 ConversationDBManager::getTotalUnReadCount()
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_CONVERSATION"))
    {
        return 0;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM UCS_IM_CONVERSATION;";
    sqlQuery.prepare(sql);
    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query conversation table with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return 0;
    }

    quint32 totalUnReadCount = 0;
    while (sqlQuery.next())
    {
        ConversationEntity conversation;
        conversation.unReadMsgCount = sqlQuery.value("unReadMsgCount").toString();
        totalUnReadCount += conversation.unReadMsgCount.toInt();
    }

    return totalUnReadCount;
}

bool ConversationDBManager::clearUnReadCount(const UCS_IM_ConversationType type, const QString targetId)
{
    MapValues values;
    MapConditions conditions;

    values["unReadMsgCount"] = "0";
    conditions["targetId"] = targetId;
    conditions["categoryId"] = QString::number(type);

    return updateConversation(values, conditions);
}

bool ConversationDBManager::updateConversation(MapValues values, MapConditions conditions)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_CONVERSATION"))
    {
        return 0;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = QString("UPDATE UCS_IM_CONVERSATION SET").append(UCSDBHelper::formatValueCondition(values, conditions));
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

bool ConversationDBManager::updateConversation(const DiscussionEntity discussion)
{
    ConversationEntity conversation;
    QString timeNow = QString::number(UCSClock::TimeInMicroseconds());

    bool result = getConversation(UCS_IM_DISCUSSIONCHAT, discussion.discussionId, conversation);
    if (result && conversation.targetId.size() > 0)
    {
        ///< 存在该讨论组会话，进行更新 >
        MapConditions conditions;
        MapValues values;

        conditions.insert("targetId", discussion.discussionId);
        values.insert("conversationTitle", discussion.discussionName);
        values.insert("lastTime", timeNow);
        values.insert("localTime", timeNow);

        updateConversation(values, conditions);
        return true;
    }

    return false;
}

bool ConversationDBManager::setTop(const UCS_IM_ConversationType type,
                                   const QString targetId,
                                   const bool isTop)
{
    MapValues values;
    MapConditions conditions;

    values["isTop"] = QStringUtil::fromBool(isTop);
    conditions["targetId"] = targetId;
    conditions["categoryId"] = QString::number(type);

    return updateConversation(values, conditions);
}

QString ConversationDBManager::getDraft(const UCS_IM_ConversationType type, const QString targetId)
{
    ConversationEntity conversation;
    getConversation(type, targetId, conversation);
    return conversation.draft;
}

bool ConversationDBManager::addDraft(const UCS_IM_ConversationType type,
                                     const QString targetId,
                                     const QString content)
{
    ConversationEntity conversation;
    getConversation(type, targetId, conversation);
    if (conversation.indexId > 0)
    {
        ///< 更新会话 >
        MapValues values;
        MapConditions conditions;

        values["draft"] = content;
        conditions["targetId"] = targetId;
        conditions["categoryId"] = QString::number(type);

        return updateConversation(values, conditions);
    }
    else
    {
        ///< 新建会话 >
        conversation.targetId = targetId;
        conversation.conversationTitle = targetId;
        conversation.categoryId = QString::number(type);
        conversation.draft = content;
        conversation.isTop = "0";
        conversation.lastTime = QString::number(UCSClock::TimeInMicroseconds());
        conversation.topTime = "";
        conversation.unReadMsgCount = "0";
        conversation.localTime = QString::number(UCSClock::TimeInMicroseconds());

        return addConversation(&conversation);
    }
}

bool ConversationDBManager::clearDraft(const UCS_IM_ConversationType type, const QString targetId)
{
    MapValues values;
    MapConditions conditions;

    values["draft"] = "";
    conditions["targetId"] = targetId;
    conditions["categoryId"] = QString::number(type);

    return updateConversation(values, conditions);
}

qint32 ConversationDBManager::maxIndexId()
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_GROUP"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT MAX(indexId) FROM UCS_IM_CONVERSATION;";
    sqlQuery.prepare(sql);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query max indexId with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    if (sqlQuery.next())
    {
        return sqlQuery.value(0).toInt();
    }
    else
    {
        return 0;
    }
}
