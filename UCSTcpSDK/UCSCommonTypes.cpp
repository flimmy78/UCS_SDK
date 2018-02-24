#include "UCSCommonTypes.h"

UCSConnectStatusEvent::UCSConnectStatusEvent(UCSConnectionStatus status)
    : QEvent((QEvent::Type)kUCSConnectStatusEvent)
    , m_status(status)
{

}

UCSConnectionStatus UCSConnectStatusEvent::status() const
{
    return m_status;
}

UCSLoginEvent::UCSLoginEvent(UCSErrorCode error,
                                         QString userId)
    : QEvent((QEvent::Type)kUCSLoginEvent)
    , m_error(error)
    , m_userId(userId)
{

}

QString UCSLoginEvent::userId() const
{
    return m_userId;
}

UCSErrorCode UCSLoginEvent::error() const
{
    return m_error;
}
