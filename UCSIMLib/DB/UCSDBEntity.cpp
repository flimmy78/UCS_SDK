#include <UCSDBEntity.h>
#include <UCSMessage.h>
#include <QStringUtil.h>
#include <UCSClock.h>
#include <UCSIMHelper.h>
#include <UCSIMTimer.h>
#include <UCSDiscussion.h>
#include <UCSDBCenter.h>

void UCSDBEntity::convertMessageFromChat(const ChatEntity &chatEntity, UCSMessage *message)
{
    message->messageId = chatEntity.msgId.toLongLong();
    message->conversationType = (UCS_IM_ConversationType)chatEntity.categoryId.toInt();
    message->messageType = (UCS_IM_MsgType)chatEntity.msgType.toInt();
    message->receivedId = chatEntity.targetId;
    message->senderUserId = chatEntity.senderId;
    message->senderNickName = chatEntity.senderNickName;
    message->sendStatus = (UCSSendStatus)chatEntity.sendStatus.toInt();
    message->receivedStatus = (UCSReceivedStatus)chatEntity.readStatus.toInt();

    qint64 nowTime = UCSClock::TimeInMicroseconds();
    qint64 loginTime = UCSIMHelper::readSettings(kUcsSettingsKeyLoginTime).toLongLong();

    bool isFromMySelf = QStringUtil::toBool(chatEntity.isFromMySelf);
    if (isFromMySelf)
    {
        message->messageDirection = MessageDirection_SEND;

        ///< receiveTime = 0, 说明尚未收到响应 >
        if (chatEntity.receivedTime.isEmpty() ||
            chatEntity.receivedTime.toInt() == 0)
        {
            message->time = chatEntity.sendTime.toLongLong();

            ///< 30s发送超时，或发送中退出或被踢线，发送状态认为失败 >
            quint32 timeout = UCSIMTimer::timeout(UCSIMTimer::kMsgSendTimer) / 1000;
            if (((nowTime - message->time > timeout) || (message->time < loginTime))
                && (message->sendStatus == SendStatus_sending))
            {
                message->sendStatus = SendStatus_fail;
            }
        }
        else
        {
            message->time = chatEntity.receivedTime.toLongLong();
        }
    }
    else
    {
        message->messageDirection = MessageDirection_RECEIVE;

        message->time = chatEntity.sendTime.toLongLong();

        ///< 单聊情况下，接收的消息，receivedId应该是自己的Id >
        QString userId = UCSIMHelper::readSettings(kUcsSettingsKeyLoginId).toString();
        message->receivedId = userId;

        if (message->receivedStatus == ReceivedStatus_DOWNLOADING)
        {
            ///< 语音或图片消息在下载中用户退出或超时无响应，视为下载失败 >
            quint32 timeout = UCSIMTimer::timeout(UCSIMTimer::kMsgSendTimer) / 1000;
            qint64 receiveTime = chatEntity.receivedTime.toLongLong();
            if ((receiveTime < loginTime) || (nowTime - receiveTime > timeout))
            {
                message->receivedStatus = ReceivedStatus_DOWNLOADFail;
            }
        }
    }

    switch (message->messageType) {
    case UCS_IM_TEXT:
    {
        UCSTextMsg *textMsg = new UCSTextMsg(chatEntity.content);
        message->content = textMsg;
    }
        break;

    case UCS_IM_IMAGE:
    {
        UCSImageMsg *imgMsg = new UCSImageMsg();
        imgMsg->thumbnailLocalPath = chatEntity.thumbnaiUrl;
        if (message->messageDirection == MessageDirection_SEND)
        {
            imgMsg->imageLocalPath = chatEntity.content;
        }
        else
        {
            imgMsg->imageRemoteUrl = chatEntity.content;
        }

        message->content = imgMsg;
    }
        break;

    case UCS_IM_VOICE:
    {
        UCSVoiceMsg *voiceMsg = new UCSVoiceMsg();
        voiceMsg->voicePath = chatEntity.content;
        voiceMsg->duration = chatEntity.msgLength.toInt();
        message->content = voiceMsg;
    }
        break;

    case UCS_IM_System:
    {
        UCSDiscussionNotification *notify = new UCSDiscussionNotification();
        notify->m_operatorId = message->receivedId;
        notify->m_extension = chatEntity.content;
        message->content = notify;
    }
        break;

    case UCS_IM_Custom:
    {
        UCSCustomMsg *customMsg = new UCSCustomMsg(chatEntity.content.toLocal8Bit());
        message->content = customMsg;
    }
        break;

    case UCS_IM_Location:
    {

    }
        break;

    default:
        break;
    }
}

void UCSDBEntity::convertConversationFromEntity(const ConversationEntity &entity,
                                                UCSConversation *conversation)
{
    conversation->setConversationType((UCS_IM_ConversationType)entity.categoryId.toInt());
    conversation->setConversationTitle(entity.conversationTitle);
    conversation->setTargetId(entity.targetId);
    conversation->setDraft(entity.draft);
    conversation->setUnreadMessageCount(entity.unReadMsgCount.toInt());
    conversation->setTime(entity.lastTime.toLongLong());
    conversation->setIsTop(QStringUtil::toBool(entity.isTop));
}

void UCSDBEntity::convertEntityFromContact(const UCSIMModContact_t contact, DiscussionEntity &entity)
{
    if (contact.tUserName.contains("@group"))
    {
        entity.discussionId = contact.tUserName.section('@', 0, 0);
    }
    else
    {
        entity.discussionId = contact.tUserName;
    }

    entity.discussionName = contact.tNickName;

    QStringList members;
    QList<UserInfoEntity> userList;
    for (qint32 i = 0; i < contact.ptRoomMemberList.size(); ++i)
    {
        UCSIMChatRoomMember_t member = contact.ptRoomMemberList.at(i);

        ///< 保存每个成员信息 >
        UserInfoEntity user;
        user.userId = member.tMemberName;
        user.userName = member.tNickName;
//        UCSDBCenter::userInfoMgr()->addUserInfo(user);
        userList.append(user);

        ///< 排在最前面的是管理员 >
        if ( i == 0)
        {
            entity.ownerId = member.tMemberName;
        }

        members.append(user.userId);
    }

    entity.members = QStringUtil::fromStringList(members);
    entity.memberCount = members.size();

    UCSDBCenter::userInfoMgr()->addUserInfo(userList);
}

void UCSDBEntity::convertDiscussionFromEntity(DiscussionEntity entity, UCSDiscussion &discussion)
{
    discussion.setDiscussionId(entity.discussionId);
    discussion.setDiscussionName(entity.discussionName);
    discussion.setCreatorId(entity.ownerId);
    discussion.setConversationType((UCS_IM_ConversationType)entity.categoryId.toInt());

    QStringList memberList = QStringUtil::toStringList(entity.members);
    QList<UCSUserInfo> userList;
    foreach (QString member, memberList) {
        UCSUserInfo userInfo;
        UserInfoEntity userEntity;
        UCSDBCenter::userInfoMgr()->getUserInfo(member, userEntity);
        userInfo.setUserId(userEntity.userId);
        userInfo.setNickName(userEntity.userName);
        userInfo.setPortraitUri(userEntity.portraitUrl);

        userList.append(userInfo);
    }

    discussion.setMemberList(userList);
}

