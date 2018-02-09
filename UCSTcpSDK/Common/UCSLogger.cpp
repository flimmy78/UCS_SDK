#include "UCSLogger.h"
#include <QMutexLocker>
#include <QThread>
#include <QDateTime>
#include <QDebug>

QMutex UCSLogger::m_Mutex;
QSharedPointer<UCSLogger> UCSLogger::m_pInstance = Q_NULLPTR;

UCSLogger::UCSLogger(QObject *parent) : QObject(parent)
{
#ifdef QT_NO_DEBUG
    m_levelFilter = kTraceDefault;
#else
    m_levelFilter = kTraceDefault | kTraceDebug;
#endif
}

QString UCSLogger::addLevel(UCSLogger::UcsLogLevel level)
{
    switch (level) {
    case kTraceInfo:
        return QString("Info    ");

    case kTraceWarning:
        return QString("Warnning");

    case kTraceError:
        return QString("Error   ");

    case kTraceCritical:
        return QString("Critical");

    case kTraceApiCall:
        return QString("ApiCall ");

    case kTraceTimer:
        return QString("Timer   ");

    case kTraceDebug:
        return QString("Debug   ");

    case kTraceFatal:
        return QString("Fatal   ");

    default:
        return QString("Unknown ");
    }
}

QString UCSLogger::addModule(const UCSLogger::UcsLogModule module)
{
    switch (module) {
    case kTcpClient:
        return QString("TcpClient   ");

    case kIMClient:
        return QString("IMClient    ");

    case kVoIPClient:
        return QString("VoipClient  ");

    case kProxyUpdate:
        return QString("ProxyUpdate ");

    case kLoginManager:
        return QString("LoginManager");

    case kTcpManager:
        return QString("TcpManager  ");

    case kMsgDispatch:
        return QString("MsgDispatch ");

    case kTcpSocket:
        return QString("TcpSocket   ");

    case kIMManager:
        return QString("IMManager   ");

    case kIMHelper:
        return QString("IMHelper    ");

    case kIMDataBase:
        return QString("IMDataBase  ");

    default:
        return QString("UnDefine    ");
    }
}

QSharedPointer<UCSLogger> &UCSLogger::Instance()
{
    if (m_pInstance.isNull())
    {
        QMutexLocker locker(&m_Mutex);
        if (m_pInstance.isNull())
        {
            m_pInstance = QSharedPointer<UCSLogger>(new UCSLogger);
        }
    }

    return m_pInstance;
}

void UCSLogger::add(const UcsLogLevel level,
                    const UcsLogModule module,
                    const QString &msg)
{
    if (!(m_levelFilter & level))
    {
        return;
    }

    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString strMessage = QString("%1 [%2] - [%3] %4")
            .arg(strDateTime)
            .arg(addModule(module))
            .arg(addLevel(level))
            .arg(msg);

    qInfo() << strMessage;
}

quint32 UCSLogger::levelFilter() const
{
    return m_levelFilter;
}

void UCSLogger::setLevelFilter(const quint32 &levelFilter)
{
    m_levelFilter = levelFilter;
}
