#include "UCSMessage.h"
#include <UCSClock.h>

UCSMessage::UCSMessage()
{
    time = UCSClock::TimeInMicroseconds();
    content = Q_NULLPTR;
    messageType = UCS_IM_UnknownType;
}

UCSMessage::~UCSMessage()
{
    if (content != Q_NULLPTR)
    {
        delete content;
        content = Q_NULLPTR;
    }
}

//UCSMessage &UCSMessage::operator=(const UCSMessage &message)
//{
//    this->conversationType = message.conversationType;
//    this->messageType = message.messageType;
//    this->receivedId = message.receivedId;
//    this->messageId = message.messageId;
//    this->messageDirection = message.messageDirection;
//    this->senderUserId = message.senderUserId;
//    this->senderNickName = message.senderNickName;
//    this->receivedStatus = message.receivedStatus;
//    this->sendStatus = message.sendStatus;
//    this->time = message.time;
//    this->objectName = message.objectName;

//    if (message.content != Q_NULLPTR)
//    {
//        this->content = new UCSMsgContent;
//    }
//}
