#ifndef UCSSYSTEMWRAPPER_H
#define UCSSYSTEMWRAPPER_H
#include <QObject>
#include <QSysInfo>

class UCSSystemWrapper
{
public:
    explicit UCSSystemWrapper() {}
    static QString prettyProductName() {
        return QSysInfo::prettyProductName();
    }

    static QString productType() {
        return QSysInfo::productType();
    }

#ifdef Q_OS_WIN32
    static QString machineHostName() {
        return QSysInfo::machineHostName();
    }

    static QString windowVersion() {
        return QSysInfo::windowsVersion();
    }
#endif
};

#endif // UCSSYSTEMWRAPPER_H
