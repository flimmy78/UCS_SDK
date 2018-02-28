#ifndef UCSDISCUSSION_H
#define UCSDISCUSSION_H

#include <QObject>
#include <qglobal.h>
#include "UCSIMDefine.h"
#include "UCSUserInfo.h"

class  UCSIMLIBSHARED_EXPORT UCSDiscussion
{

public:
    explicit UCSDiscussion();

    QString discussionId() const;
    void setDiscussionId(const QString &discussionId);

    QString discussionName() const;
    void setDiscussionName(const QString &discussionName);

    QString creatorId() const;
    void setCreatorId(const QString &creatorId);

    UCS_IM_ConversationType conversationType() const;
    void setConversationType(const UCS_IM_ConversationType &conversationType);

    QList<UCSUserInfo> memberList() const;
    void setMemberList(const QList<UCSUserInfo> &memberList);

    qint32 inviteStatus() const;
    void setInviteStatus(const qint32 &inviteStatus);

    qint32 pushMessageNotificationstatus() const;
    void setPushMessageNotificationstatus(const qint32 &pushMessageNotificationstatus);

    QString extra() const;
    void setExtra(const QString &extra);

private:
    /*!
     * \brief m_discussionId 讨论组Id
     */
    QString m_discussionId;

    /*!
     * \brief m_discussionName 讨论组名称
     */
    QString m_discussionName;

    /*!
     * \brief m_creatorId 创建讨论组用户Id
     */
    QString m_creatorId;

    /*!
     * \brief m_conversationType 会话类型
     */
    UCS_IM_ConversationType m_conversationType;

    /*!
     * \brief m_memberList 讨论组成员列表，每个元素是一个UCSUserInfo对象
     */
    QList<UCSUserInfo> m_memberList;

    /*!
     * \brief m_inviteStatus 保留字段
     */
    qint32 m_inviteStatus;

    /*!
     * \brief m_pushMessageNotificationstatus 保留字段
     */
    qint32 m_pushMessageNotificationstatus;

    /*!
     * \brief m_extra 保留字段
     */
    QString m_extra;
};

#endif // UCSDISCUSSION_H
