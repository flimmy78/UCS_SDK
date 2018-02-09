#include "UCSMessage.h"
#include <UCSClock.h>

UCSMessage::UCSMessage()
{
    time = UCSClock::TimeInMicroseconds();
    content = Q_NULLPTR;
}

UCSMessage::~UCSMessage()
{
    if (content != Q_NULLPTR)
    {
        delete content;
        content = Q_NULLPTR;
    }
}
