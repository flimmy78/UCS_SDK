#include "UCSVoiceInfoDBManager.h"
#include <UCSDBHelper.h>
#include <UCSConnectionPool.h>
#include <UCSTcp/UCSLogger.h>

VoiceInfoDBManager::VoiceInfoDBManager()
{
    m_createSql = "CREATE TABLE IF NOT EXISTS UCS_IM_VOICEINFO ("
                          "messageId     INTEGER PRIMARY KEY"
                          "                      NOT NULL,"
                          "iMsgId        INTEGER UNIQUE,"
                          "iLength       INTEGER,"
                          "pcClientMsgId TEXT)";
}

bool VoiceInfoDBManager::addVoiceInfo(const VoiceInfoEntity *pVoice)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_VOICEINFO"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "INSERT OR REPLACE INTO UCS_IM_VOICEINFO ("
                    "messageId, iMsgId, iLength, pcClientMsgId"
                    ") VALUES ("
                    ":messageId, :iMsgId, :iLength, :pcClientMsgId)";
    sqlQuery.prepare(sql);
    sqlQuery.bindValue(":messageId", pVoice->messageId);
    sqlQuery.bindValue(":iMsgId", pVoice->iMsgId);
    sqlQuery.bindValue(":iLength", pVoice->iLength);
    sqlQuery.bindValue(":pcClientMsgId", pVoice->pcClientMsgId);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed insert voice info with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool VoiceInfoDBManager::delVoiceInfo(const QString messageId)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_VOICEINFO"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "DELETE FROM UCS_IM_VOICEINFO WHERE messageId = ?;";
    sqlQuery.prepare(sql);
    sqlQuery.addBindValue(messageId);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed delete target voice info with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    return true;
}

bool VoiceInfoDBManager::getVoiceInfo(const QString messageId, VoiceInfoEntity &voiceInfo)
{
    if (!UCSDBHelper::checkAndCreateTable(m_createSql, "UCS_IM_VOICEINFO"))
    {
        return false;
    }

    UCSDBScoped scoped;
    QSqlDatabase db = scoped.db();
    QSqlQuery sqlQuery(db);

    QString sql = "SELECT * FROM UCS_IM_VOICEINFO WHERE messageId = ?;";
    sqlQuery.prepare(sql);
    sqlQuery.addBindValue(messageId);

    if (!sqlQuery.exec())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMDataBase,
                QString("failed query target voice info with error(%1)")
                        .arg(sqlQuery.lastError().text()));

        return false;
    }

    if (sqlQuery.next())
    {
        voiceInfo.messageId = sqlQuery.value("messageId").toString();
        voiceInfo.iMsgId = sqlQuery.value("iMsgId").toString();
        voiceInfo.iLength = sqlQuery.value("iLength").toString();
        voiceInfo.pcClientMsgId = sqlQuery.value("pcClientMsgId").toString();
    }

    return true;
}

