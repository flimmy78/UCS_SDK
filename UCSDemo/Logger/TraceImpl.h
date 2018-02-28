#ifndef TRACEIMPL_H
#define TRACEIMPL_H

#include <QObject>
#include <QSharedPointer>
#include <qglobal.h>

class TraceImpl
{
public:
    enum TraceLevel
    {
        kLevelNone  = 0x0000,
        kLevelInfo               = 0x0001,
        kLevelWarning            = 0x0002,
        kLevelError              = 0x0004,
        kLevelCritical           = 0x0008,
        kLevelApiCall            = 0x0010,
        kLevelTimer              = 0x0020,   // timing info
        kLevelDefault            = 0x00ff,

        kLevelDebug              = 0x0100,  // debug
        kLevelFatal              = 0x0200,
        kLevelAll                = 0xffff
    };

    static QSharedPointer<TraceImpl> &Instance();

    void setLevelFilter(const quint32 &levelFilter);

private:
    explicit TraceImpl();
    TraceImpl(const TraceImpl&);
    TraceImpl &operator =(const TraceImpl&);

    QString addLevel(TraceLevel level);

private:
    static QMutex m_Mutex;
    static QSharedPointer<TraceImpl> m_pInstance;
    quint32 m_levelFilter;
};

#endif // TRACEIMPL_H
