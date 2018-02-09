#ifndef QSTRINGUTIL_H
#define QSTRINGUTIL_H

#include <QObject>
#include <qglobal.h>

class QStringUtil
{
public:
    explicit QStringUtil();

    static QString fromBool(bool b);
    static bool toBool(QString s);

    static QString fromStringList(QStringList list);
    static QStringList toStringList(QString s);


};

#endif // QSTRINGUTIL_H
