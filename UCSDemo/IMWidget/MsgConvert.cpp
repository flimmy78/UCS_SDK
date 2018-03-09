#include "MsgConvert.h"

MsgConvert::MsgConvert()
{

}

ChatMsgModel MsgConvert::convert2Model(UCSMessage *message)
{
    ChatMsgModel model;
    if (message->messageType == UCS_IM_TEXT)
    {
        UCSTextMsg *txtMsg = static_cast<UCSTextMsg*>(message->content);
        model.content = txtMsg->content();
    }
    else
    {
        model.content = QStringLiteral("[不支持的消息类型]");
    }
    model.isSender = (message->messageDirection == MessageDirection_SEND ? true : false);
    model.timestamp = message->time;
    model.userName = message->senderUserId;
    model.nickName = message->senderNickName;

    return model;
}
