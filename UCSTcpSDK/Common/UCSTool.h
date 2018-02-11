#ifndef UCSTOOL_H
#define UCSTOOL_H
#include <QString>

QString charToQString(char *ptr)
{
    if (ptr != NULL)
    {
        return QString(ptr);
    }

    return QString("");
}

void QStringToChar(char *dest, QString src, size_t size)
{
    std::string strdata = src.toStdString();
    memcpy(dest, strdata.c_str(), size);
    dest[size-1] = '\0';
}

#endif // UCSTOOL_H
