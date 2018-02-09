#ifndef UCSIMERROR_H
#define UCSIMERROR_H

#include <QObject>
#include <qglobal.h>

struct UCSIMError
{
    enum IMError
    {
        NoError = 0,
        IllegalArgument,
        IllegalConversationType,
        IllegalMessageType,
        IllegalTargetId,
        IllegalContent,
    };

    QString errorString() const;
    IMError error;
};

#endif // UCSIMERROR_H
