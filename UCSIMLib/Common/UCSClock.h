#ifndef UCSCLOCK_H
#define UCSCLOCK_H

#include <QObject>
#include <qglobal.h>
#include <QDateTime>

class UCSClock
{
public:
    UCSClock();

    /*!
     * \brief TimeInMicroseconds
     * \return Returns the number of seconds since 1970-01-01T00:00:00 Universal Coordinated Time
     */
    static qint64 TimeInMicroseconds();

};

#endif // UCSCLOCK_H
