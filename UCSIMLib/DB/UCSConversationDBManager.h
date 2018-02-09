#ifndef CONVERSATIONDBMANAGER_H
#define CONVERSATIONDBMANAGER_H

#include <QObject>
#include <qglobal.h>
#include <UCSDBEntity.h>
#include <UCSDiscussionDBManager.h>

class ConversationDBManager
{
public:
    explicit ConversationDBManager(DiscussionDBManager *pDiscussionMgr);

    /*!
     * \brief addConversation 增加一个会话
     * \param pConversation
     * \return true for success, else false
     */
    bool addConversation(const ConversationEntity *pConversation);

    /*!
     * \brief addConversation 同步讨论组时，用讨论组实体添加会话
     * \param discussion
     * \return
     */
    bool addConversation(const DiscussionEntity discussion);

    /*!
     * \brief addConversation 更新一条聊天记录到会话内
     * \param chatEntity 聊天记录
     * \param 增加的消息数
     * \return true for success, else false
     */
    bool addConversation(const ChatEntity *chatEntity, const quint32 count);

    /*!
     * \brief delConversation 删除单个会话
     * \param type
     * \param targetId
     * \return true for success, else false
     */
    bool delConversation(const UCS_IM_ConversationType type,
                         const QString targetId);

    /*!
     * \brief clearConversations 清空所有会话
     * \return true for success, else false
     */
    bool clearConversations();

    /*!
     * \brief getConversation 查询指定targetId和type的会话
     * \param type
     * \param targetId
     * \param conversation 查询到的会话信息
     * \return true for success, else false
     */
    bool getConversation(const UCS_IM_ConversationType type,
                         const QString targetId,
                         ConversationEntity &conversation);

    /*!
     * \brief getConversation 获取指定会话类型的所有会话
     * \param type
     * \return 返回所有同一类型的会话
     */
    QList<ConversationEntity> getConversationList(const QList<qint32> typeList);

    /*!
     * \brief getUnReadCount 获取指定会话的未读消息数
     * \param type
     * \param targetId
     * \return 返回未读消息数, 查询失败或无未读均返回0
     */
    quint32 getUnReadCount(const UCS_IM_ConversationType type,
                           const QString targetId);

    /*!
     * \brief getTotalUnReadCount
     * \return 返回所有会话的未读消息总数
     */
    quint32 getTotalUnReadCount();

    /*!
     * \brief clearUnReadCount 清除指定会话的未读消息数
     * \param type
     * \param targetId
     * \return true for success, else false
     */
    bool clearUnReadCount(const UCS_IM_ConversationType type,
                          const QString targetId);

    /*!
     * \brief updateConversation 更新指定条件的会话信息
     * \param values 待更新值的key-value对
     * \param conditions 指定的更新条件，And 关系
     * \return true for success
     */
    bool updateConversation(MapValues values, MapConditions conditions);

    /*!
     * \brief updateConversation 同步讨论组时，用讨论组实体更新会话
     * \param discussion
     * \return
     */
    bool updateConversation(const DiscussionEntity discussion);

    /*!
     * \brief setTop 会话置顶或取消
     * \param type
     * \param targetId
     * \param isTop
     * \return true for success, else false
     */
    bool setTop(const UCS_IM_ConversationType type,
                const QString targetId,
                const bool isTop);

    /*!
     * \brief getDraft 获取指定会话的草稿内容
     * \param type
     * \param targetId
     * \return 草稿内容，如果有，否则为空
     */
    QString getDraft(const UCS_IM_ConversationType type,
                     const QString targetId);

    /*!
     * \brief addDraft 保存指定会话的文本草稿
     * \param type
     * \param targetId
     * \param content
     * \return true for success, else false
     */
    bool addDraft(const UCS_IM_ConversationType type,
                  const QString targetId,
                  const QString content);

    /*!
     * \brief clearDraft 清除指定会话的草稿内容
     * \param type
     * \param targetId
     * \return true for success, else false
     */
    bool clearDraft(const UCS_IM_ConversationType type,
                    const QString targetId);

private:
    qint32 maxIndexId();

private:
    QString m_createSql;
    DiscussionDBManager *m_disscussionMgr;
};

#endif // CONVERSATIONDBMANAGER_H
