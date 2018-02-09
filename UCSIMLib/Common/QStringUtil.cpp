#include "QStringUtil.h"

#include <QVariant>

QStringUtil::QStringUtil()
{

}

QString QStringUtil::fromBool(bool b)
{
    return QString::number(b);
}

bool QStringUtil::toBool(QString s)
{
    QVariant v = s;
    return v.toBool();
}

QString QStringUtil::fromStringList(QStringList list)
{
    return list.join(",");
}

QStringList QStringUtil::toStringList(QString s)
{
    if (s.size() > 0)
    {
        return s.split(",", QString::SkipEmptyParts);
    }

    return QStringList();
}
