#include "UCSIMEvent.h"


UCSMsgSendEvent::UCSMsgSendEvent(UCSErrorCode error, qint64 messageId)
    : QEvent((QEvent::Type)kUCSMsgSendEvent)
    , m_error(error)
    , m_messageId(messageId)
{

}

UCSErrorCode UCSMsgSendEvent::error() const
{
    return m_error;
}

qint64 UCSMsgSendEvent::messageId() const
{
    return m_messageId;
}

UCSDiscussionEvent::UCSDiscussionEvent(UCSErrorCode error, UCSDiscussion discussion)
    : QEvent((QEvent::Type)kUCSDiscussionEvent)
    , m_error(error)
    , m_discussion(discussion)
{

}

UCSErrorCode UCSDiscussionEvent::error() const
{
    return m_error;
}

UCSDiscussion UCSDiscussionEvent::discussion() const
{
    return m_discussion;
}

UCSVoiceDownloadEvent::UCSVoiceDownloadEvent(UCSErrorCode error, UCSMessage message)
    : QEvent((QEvent::Type)kUCSVoiceDownloadEvent)
    , m_error(error)
    , m_message(message)
{

}

UCSErrorCode UCSVoiceDownloadEvent::error() const
{
    return m_error;
}

UCSMessage UCSVoiceDownloadEvent::message() const
{
    return m_message;
}


//UCSMsgSyncEvent::UCSMsgSyncEvent(UCSErrorCode error, QList<UCSMessage *> messages)
//    : QEvent((QEvent::Type)kUCSMsgSyncEvent)
//    , m_error(error)
//    , m_messages(messages)
//{

//}

UCSMsgSyncEvent::UCSMsgSyncEvent(UCSErrorCode error, QMap<QString, qint32> msgCount)
    : QEvent((QEvent::Type)kUCSMsgSyncEvent)
    , m_error(error)
    , m_msgCount(msgCount)
{

}

UCSErrorCode UCSMsgSyncEvent::error() const
{
    return m_error;
}

QMap<QString, qint32> UCSMsgSyncEvent::msgCount() const
{
    return m_msgCount;
}

//QList<UCSMessage*> UCSMsgSyncEvent::messages() const
//{
//    return m_messages;
//}
