#ifndef CHATDBMANAGER_H
#define CHATDBMANAGER_H

#include <QObject>
#include <qglobal.h>
#include <UCSDBEntity.h>
#include <UCSIMDefine.h>
#include <QSqlQuery>

class ChatDBManager
{
public:
    explicit ChatDBManager();

    /*!
     * \brief addChat 保存指定会话Id的单条聊天消息
     * \param targetId
     * \param type
     * \param chatEntity 指定会话的聊天消息
     * \return true for success, else false
     */
    bool addChat(const QString targetId,
                 const UCS_IM_ConversationType type,
                 const ChatEntity chatEntity);

    /*!
     * \brief addChat 保存指定会话Id的多条聊天消息
     * \param targetId 会话Id
     * \param type 会话类型
     * \param chatList 同一会话类型的消息列表
     * \return true for success, else false
     */
    bool addChat(const QString targetId,
                 const UCS_IM_ConversationType type,
                 const QList<ChatEntity> chatList);

    /*!
     * \brief delChat 删除指定的一条聊天消息
     * \param targetId
     * \param type
     * \param msgId 指定的聊天消息Id
     * \return true for success, else false
     */
    bool delChat(const QString targetId,
                 const UCS_IM_ConversationType type,
                 const QString msgId);

    /*!
     * \brief clearChats 删除指定会话的所有聊天消息
     * \param targetId
     * \param type
     * \return true for success, else false
     */
    bool clearChats(const QString targetId,
                    const UCS_IM_ConversationType type);

    /*!
     * \brief updateChat 更新指定聊天消息字段内容
     * \param targetId
     * \param type
     * \param values 待更新值的key-value对
     * \param conditions 指定的更新条件，And 关系
     * \return
     */
    bool updateChat(const QString targetId,
                    const UCS_IM_ConversationType type,
                    MapValues values,
                    MapConditions conditions);

    /*!
     * \brief updateChatReceivedStatus 更新指定聊天消息的接收状态
     * \param targetId
     * \param type
     * \param msgId
     * \param status
     * \return true for success, else false
     */
    bool updateChatReceivedStatus(const QString targetId,
                                  const UCS_IM_ConversationType type,
                                  const QString msgId,
                                  const UCSReceivedStatus status);

    /*!
     * \brief getChat 查询指定的一条消息
     * \param targetId
     * \param type
     * \param msgId
     * \param chatEntity 查询到的聊天消息实体
     * \return true for success, else false
     */
    bool getChat(const QString targetId,
                 const UCS_IM_ConversationType type,
                 const QString msgId,
                 ChatEntity &chatEntity);

    /*!
     * \brief getChats 获取指定会话的@count条聊天消息
     * \param targetId
     * \param type
     * \param count
     * \return <= @count条聊天消息列表
     */
    QList<ChatEntity> getChats(const QString targetId,
                               const UCS_IM_ConversationType type,
                               const int count);

    /*!
     * \brief getChats 拉取@oldestMessageId之前的@count条旧聊天消息
     * \param targetId
     * \param type
     * \param count
     * \param oldestMessageId
     * \return <= @count条聊天消息列表
     */
    QList<ChatEntity> getHistoryChats(const QString targetId,
                               const UCS_IM_ConversationType type,
                               const int count,
                               const QString oldestMessageId);

    /*!
     * \brief getUnReadCount 获取指定会话未读消息数，用于更新会话
     * \param targetId
     * \param type
     * \return
     */
    quint32 getUnReadCount(const QString targetId,
                           const UCS_IM_ConversationType type);

    /*!
     * \brief searchChats 返回聊天会话中含有@keyWord的聊天消息
     * \param targetId
     * \param type
     * \param keyWord 关键字
     * \return 返回含有@keyWord的所有聊天消息
     */
    QList<ChatEntity> searchChats(const QString targetId,
                                  const UCS_IM_ConversationType type,
                                  const QString keyWord);

private:
    QString tableName(const QString targetId,
                      const UCS_IM_ConversationType type);

    bool checkAndCreateTable(const QString targetId,
                             const UCS_IM_ConversationType type);

    void query2Entity(const QSqlQuery *sqlQuery, ChatEntity *chatEntity);

    QString getChatId(const QString targetId,
                      const UCS_IM_ConversationType type,
                      const QString msgId);
private:
    QString m_createSql;
};

#endif // CHATDBMANAGER_H
