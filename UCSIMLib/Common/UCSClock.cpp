#include "UCSClock.h"

UCSClock::UCSClock()
{

}

qint64 UCSClock::TimeInMicroseconds()
{
    return QDateTime::currentSecsSinceEpoch();
}
