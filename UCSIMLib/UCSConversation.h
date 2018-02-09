#ifndef UCSCONVERSATION_H
#define UCSCONVERSATION_H

#include <QObject>
#include <qglobal.h>
#include "UCSIMDefine.h"
#include "UCSMessage.h"

/*!
 * \brief The UCSConversation class 会话类，每一个会话对应一个聊天
 */
class UCSConversation
{

public:
    explicit UCSConversation();

    UCS_IM_ConversationType conversationType() const;
    void setConversationType(const UCS_IM_ConversationType &conversationType);

    QString targetId() const;
    void setTargetId(const QString &targetId);

    QString conversationTitle() const;
    void setConversationTitle(const QString &conversationTitle);

    quint32 unreadMessageCount() const;
    void setUnreadMessageCount(const quint32 &unreadMessageCount);

    bool isTop() const;
    void setIsTop(bool isTop);

    UCSReceivedStatus receivedStatus() const;
    void setReceivedStatus(const UCSReceivedStatus &receivedStatus);

    UCSSendStatus sendStatus() const;
    void setSendStatus(const UCSSendStatus &sendStatus);

    quint64 time() const;
    void setTime(const quint64 &time);

    QString draft() const;
    void setDraft(const QString &draft);

    QString objectName() const;
    void setObjectName(const QString &objectName);

    UCSMessage lastestMessage() const;
    void setLastestMessage(const UCSMessage &lastestMessage);

private:
    /*!
     * \brief m_conversationType 会话类型。单聊会话、讨论组会话、群会话
     */
    UCS_IM_ConversationType m_conversationType;

    /*!
     * \brief m_targetId 会话Id。(单聊就是对方id，讨论组是讨论组id，群聊就是群组id)
     */
    QString m_targetId;

    /*!
     * \brief m_conversationTitle 会话名称
     */
    QString m_conversationTitle;

    /*!
     * \brief m_unreadMessageCount 会话中未读消息数
     */
    quint32 m_unreadMessageCount;

    /*!
     * \brief m_isTop 当前会话是否置顶
     */
    bool m_isTop;

    /*!
     * \brief m_receivedStatus 消息接收状态。(仅仅用于接收消息，判断接收状态)
     */
    UCSReceivedStatus m_receivedStatus;

    /*!
     * \brief m_sendStatus 消息发送状态.(仅仅用于自己发送消息时，判断发送状态)
     */
    UCSSendStatus m_sendStatus;

    /*!
     * \brief m_time 时间.(如果这条消息是自己发送的，为自己发送该条消息的时间；如果这条消息是接收的，为对方发送这条消息的时间)
     */
    quint64 m_time;

    /*!
     * \brief m_draft 消息草稿，尚未发送的消息内容。如果开发者需要使用这个字段，应该调用UCSIMClient.h中相关的接口操作消息草稿
     */
    QString m_draft;

    /*!
     * \brief m_objectName 保留字段
     */
    QString m_objectName;

    UCSMessage m_lastestMessage;
};

#endif // UCSCONVERSATION_H
