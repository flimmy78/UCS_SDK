#ifndef UCSIMEVENT_H
#define UCSIMEVENT_H

#include <QObject>
#include <qglobal.h>
#include "UCSDiscussion.h"
#include "UCSMessage.h"
#include "ucsimlib_global.h"
#include <UCSTcp/UCSCommonTypes.h>

class UCSIMLIBSHARED_EXPORT UCSMsgSendEvent : public QEvent {
public:
    explicit UCSMsgSendEvent(UCSErrorCode error, qint64 messageId);

    UCSErrorCode error() const;

    qint64 messageId() const;

private:
    UCSErrorCode m_error;
    qint64 m_messageId;
};

class UCSIMLIBSHARED_EXPORT UCSDiscussionEvent : public QEvent {
public:
    explicit UCSDiscussionEvent(UCSErrorCode error, UCSDiscussion discussion);

    UCSErrorCode error() const;

    UCSDiscussion discussion() const;

private:
    UCSErrorCode m_error;
    UCSDiscussion m_discussion;
};

class UCSIMLIBSHARED_EXPORT UCSVoiceDownloadEvent : public QEvent
{
public:
    explicit UCSVoiceDownloadEvent(UCSErrorCode error, UCSMessage message);

    UCSErrorCode error() const;

    UCSMessage message() const;

private:
    UCSErrorCode m_error;
    UCSMessage m_message;
};

class UCSIMLIBSHARED_EXPORT UCSMsgSyncEvent : public QEvent
{
public:
    explicit UCSMsgSyncEvent(UCSErrorCode error, QList<UCSMessage*> messages);

    UCSErrorCode error() const;

    QList<UCSMessage*> messages() const;

private:
    UCSErrorCode m_error;
    QList<UCSMessage*> m_messages;
};

#endif // UCSIMEVENT_H
