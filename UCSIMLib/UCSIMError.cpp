#include "UCSIMError.h"


QString UCSIMError::errorString() const
{
    switch (error) {
    case NoError:
        return QString("NoError");

    case IllegalArgument:
        return QString("IllegalArgument");

    case IllegalConversationType:
        return QString("IllegalConversationType");

    case IllegalMessageType:
        return QString("IllegalMessageType");

    case IllegalTargetId:
        return QString("IllegalTargetId");

    case IllegalContent:
        return QString("IllegalContent");

    default:
        return QString("Undefine");
    }
}
