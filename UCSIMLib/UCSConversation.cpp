#include "UCSConversation.h"

UCSConversation::UCSConversation()
{
    m_lastestMessage = Q_NULLPTR;
}

UCSConversation::~UCSConversation()
{
    if (m_lastestMessage != Q_NULLPTR)
    {
        delete m_lastestMessage;
        m_lastestMessage = Q_NULLPTR;
    }
}

UCS_IM_ConversationType UCSConversation::conversationType() const
{
    return m_conversationType;
}

void UCSConversation::setConversationType(const UCS_IM_ConversationType &conversationType)
{
    m_conversationType = conversationType;
}

QString UCSConversation::targetId() const
{
    return m_targetId;
}

void UCSConversation::setTargetId(const QString &targetId)
{
    m_targetId = targetId;
}

QString UCSConversation::conversationTitle() const
{
    return m_conversationTitle;
}

void UCSConversation::setConversationTitle(const QString &conversationTitle)
{
    m_conversationTitle = conversationTitle;
}

quint32 UCSConversation::unreadMessageCount() const
{
    return m_unreadMessageCount;
}

void UCSConversation::setUnreadMessageCount(const quint32 &unreadMessageCount)
{
    m_unreadMessageCount = unreadMessageCount;
}

bool UCSConversation::isTop() const
{
    return m_isTop;
}

void UCSConversation::setIsTop(bool isTop)
{
    m_isTop = isTop;
}

UCSReceivedStatus UCSConversation::receivedStatus() const
{
    return m_receivedStatus;
}

void UCSConversation::setReceivedStatus(const UCSReceivedStatus &receivedStatus)
{
    m_receivedStatus = receivedStatus;
}

UCSSendStatus UCSConversation::sendStatus() const
{
    return m_sendStatus;
}

void UCSConversation::setSendStatus(const UCSSendStatus &sendStatus)
{
    m_sendStatus = sendStatus;
}

quint64 UCSConversation::time() const
{
    return m_time;
}

void UCSConversation::setTime(const quint64 &time)
{
    m_time = time;
}

QString UCSConversation::draft() const
{
    return m_draft;
}

void UCSConversation::setDraft(const QString &draft)
{
    m_draft = draft;
}

QString UCSConversation::objectName() const
{
    return m_objectName;
}

void UCSConversation::setObjectName(const QString &objectName)
{
    m_objectName = objectName;
}

UCSMessage *UCSConversation::lastestMessage() const
{
    return m_lastestMessage;
}

void UCSConversation::setLastestMessage(UCSMessage *lastestMessage)
{
    m_lastestMessage = lastestMessage;
}

