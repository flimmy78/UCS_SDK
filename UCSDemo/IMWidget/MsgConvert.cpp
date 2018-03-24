#include "MsgConvert.h"
#include "DBCenter.h"

MsgConvert::MsgConvert()
{

}

ChatMsgModel MsgConvert::convert2Model(UCSMessage *message)
{
    ChatMsgModel model;

    ContactEntity contact;
    DBCenter::contactMgr()->getContact(message->senderUserId, contact);

    model.msgId = message->messageId;
    model.isSender = (message->messageDirection == MessageDirection_SEND ? true : false);
    model.timestamp = message->time;
    model.userName = message->senderUserId;
    model.nickName = message->senderNickName;
    model.headerUrl = contact.headPath;
    model.isRead = true;

    if (message->messageType == UCS_IM_TEXT)
    {
        UCSTextMsg *txtMsg = static_cast<UCSTextMsg*>(message->content);
        model.content = txtMsg->content().replace("\n", "</br>");

        model.type = MessageBodyType_Text;
    }
    else if (message->messageType == UCS_IM_IMAGE)
    {
        model.type = MessageBodyType_Image;
        model.content = QStringLiteral("[图片]");

        UCSImageMsg *imgMsg = static_cast<UCSImageMsg*>(message->content);
        model.thumbnailLocalPath = imgMsg->thumbnailLocalPath;
        model.imageLocalPath = imgMsg->imageLocalPath;
    }
    else if (message->messageType == UCS_IM_VOICE)
    {
        model.type = MessageBodyType_Voice;
        model.content = QStringLiteral("[语音]");
    }
    else if (message->messageType == UCS_IM_Location)
    {
        model.type = MessageBodyType_Location;
        model.content = QStringLiteral("[位置]");
    }
    else if (message->messageType == UCS_IM_Custom)
    {
        model.type = MessageBodyType_Custom;
        model.content = QStringLiteral("[自定义消息]");
    }
    else if (message->messageType == UCS_IM_System)
    {
        model.type = MessageBodyType_Notify;
        UCSDiscussionNotification *notify = static_cast<UCSDiscussionNotification*>(message->content);

        model.content = notify->m_extension;
    }
    else
    {
        model.content = QStringLiteral("[不支持的消息类型]");
    }


    return model;
}
