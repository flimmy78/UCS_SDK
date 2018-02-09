#include "UCSDiscussion.h"

UCSDiscussion::UCSDiscussion()
{

}

QString UCSDiscussion::discussionId() const
{
    return m_discussionId;
}

void UCSDiscussion::setDiscussionId(const QString &discussionId)
{
    m_discussionId = discussionId;
}

QString UCSDiscussion::discussionName() const
{
    return m_discussionName;
}

void UCSDiscussion::setDiscussionName(const QString &discussionName)
{
    m_discussionName = discussionName;
}

QString UCSDiscussion::creatorId() const
{
    return m_creatorId;
}

void UCSDiscussion::setCreatorId(const QString &creatorId)
{
    m_creatorId = creatorId;
}

UCS_IM_ConversationType UCSDiscussion::conversationType() const
{
    return m_conversationType;
}

void UCSDiscussion::setConversationType(const UCS_IM_ConversationType &conversationType)
{
    m_conversationType = conversationType;
}

QList<UCSUserInfo> UCSDiscussion::memberList() const
{
    return m_memberList;
}

void UCSDiscussion::setMemberList(const QList<UCSUserInfo> &memberList)
{
    m_memberList = memberList;
}

qint32 UCSDiscussion::inviteStatus() const
{
    return m_inviteStatus;
}

void UCSDiscussion::setInviteStatus(const qint32 &inviteStatus)
{
    m_inviteStatus = inviteStatus;
}

qint32 UCSDiscussion::pushMessageNotificationstatus() const
{
    return m_pushMessageNotificationstatus;
}

void UCSDiscussion::setPushMessageNotificationstatus(const qint32 &pushMessageNotificationstatus)
{
    m_pushMessageNotificationstatus = pushMessageNotificationstatus;
}

QString UCSDiscussion::extra() const
{
    return m_extra;
}

void UCSDiscussion::setExtra(const QString &extra)
{
    m_extra = extra;
}
