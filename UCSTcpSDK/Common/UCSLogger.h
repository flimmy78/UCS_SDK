#ifndef UCSLOGGER_H
#define UCSLOGGER_H

#include <QObject>
#include <QSharedPointer>
#include "ucstcpsdk_global.h"

#define UCS_LOG UCSLogger::Instance()->add

class UCSTCPSDKSHARED_EXPORT UCSLogger : public QObject
{
    Q_OBJECT
public:
    enum UcsLogLevel
    {
        kTraceNone               = 0x0000,    // no trace
        kTraceInfo               = 0x0001,
        kTraceWarning            = 0x0002,
        kTraceError              = 0x0004,
        kTraceCritical           = 0x0008,
        kTraceApiCall            = 0x0010,
        kTraceTimer              = 0x0020,   // timing info
        kTraceDefault            = 0x00ff,

        kTraceDebug              = 0x0100,  // debug
        kTraceFatal              = 0x0200,
        kTraceAll                = 0xffff
    };

    enum UcsLogModule
    {
        kTcpClient,
        kProxyUpdate,
        kLoginManager,
        kTcpManager,
        kMsgDispatch,
        kTcpSocket,
        kIMClient,
        kIMManager,
        kIMHelper,
        kIMDataBase,
        kVoIPClient,
    };

    static QSharedPointer<UCSLogger> &Instance();

    void add(const UcsLogLevel level,
             const UcsLogModule module,
             const QString &msg);

    void add(const UcsLogLevel level,
             const QString &module,
             const QString &msg);

    quint32 levelFilter() const;
    void setLevelFilter(const quint32 &levelFilter);

private:
    explicit UCSLogger(QObject *parent = 0);
    UCSLogger(const UCSLogger&);
    UCSLogger& operator=(const UCSLogger&);

    QString addLevel(UcsLogLevel level);
    QString addModule(const UcsLogModule module);

signals:

public slots:

private:
    static QSharedPointer<UCSLogger> m_pInstance;
    static QMutex m_Mutex;
    quint32  m_levelFilter;
};

#endif // UCSLOGGER_H
