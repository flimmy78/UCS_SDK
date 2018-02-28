#include "TraceImpl.h"

QMutex TraceImpl::m_Mutex;
QSharedPointer<TraceImpl> TraceImpl::m_pInstance = Q_NULLPTR;

QSharedPointer<TraceImpl> &TraceImpl::Instance()
{

}

TraceImpl::TraceImpl()
{
    m_levelFilter = kLevelDefault;
}

QString TraceImpl::addLevel(TraceImpl::TraceLevel level)
{
    switch (level) {
    case kLevelInfo:
        return QString("Info    ");

    case kLevelWarning:
        return QString("Warnning");

    case kLevelError:
        return QString("Error   ");

    case kLevelCritical:
        return QString("Critical");

    case kLevelApiCall:
        return QString("ApiCall ");

    case kLevelTimer:
        return QString("Timer   ");

    case kLevelDebug:
        return QString("Debug   ");

    case kLevelFatal:
        return QString("Fatal   ");

    default:
        return QString("Unknown ");
    }
}

void TraceImpl::setLevelFilter(const quint32 &levelFilter)
{
    m_levelFilter = levelFilter;
}
