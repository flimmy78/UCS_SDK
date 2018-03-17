#include "CommonHelper.h"

#include <QFile>
#include <QApplication>
#include <QDateTime>
#include <QStandardPaths>
#include <QSettings>
#include <QDir>

CommonHelper::CommonHelper()
{

}

void CommonHelper::loadAppStyle(const QString &style)
{
    QFile file(style);
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = QString(file.readAll());
        qApp->setStyleSheet(styleSheet);
        file.close();
    }
}

QImage CommonHelper::toGray(QImage image)
{
    int height = image.height();
    int width = image.width();
    QImage ret = QImage(width, height, QImage::Format_Indexed8);

    ret.setColorCount(256);

    for (int i = 0; i < 256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }

    switch (image.format()) {
    case QImage::Format_Indexed8:
    {
        for (int i = 0; i < height; i++)
        {
            const uchar *pSrc = (uchar*)image.constScanLine(i);
            uchar *pDst = (uchar*)ret.scanLine(i);
            memcpy(pDst, pSrc, width);
        }
    }
        break;

    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        for (int i = 0; i < height; i++)
        {
            const QRgb *pSrc = (QRgb*)image.constScanLine(i);
            uchar *pDst = (uchar*)ret.scanLine(i);
            for (int j = 0; j < width; j++)
            {
                pDst[j] = qGray(pSrc[j]);
            }
        }
    }
        break;

    default:
        break;
    }

    return ret;
}

QString CommonHelper::timeToString(qint64 time, QString format)
{
    QDateTime dateTime;
    return dateTime.fromTime_t(time).toString(format);
}

QString CommonHelper::tempDir()
{
    QString appName =  QCoreApplication::applicationName();
    QString sysTempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString myTempPath = sysTempPath + "/" + appName;

    QDir dir(myTempPath);
    if (!dir.exists())
    {
        dir.mkpath(myTempPath);
    }

    return myTempPath;
}

QString CommonHelper::userTempPath()
{
    QString userId = readSetting("Login", "userId", "").toString();
    if (userId.isEmpty())
    {
        return QString("");
    }

    QString tempPath = tempDir() + "/" + userId;
    QDir dir(tempPath);
    if (!dir.exists())
    {
        dir.mkpath(tempPath);
    }

    return tempPath;
}

QString CommonHelper::dataDir()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    QDir dir(dataPath);
    if (!dir.exists())
    {
        dir.mkpath(dataPath);
    }

    return dataPath;
}

QString CommonHelper::userDataPath()
{
    QString userId = readSetting("Login", "userId", "").toString();
    if (userId.isEmpty())
    {
        return QString();
    }
    QString dataPath = dataDir() + "/" + userId;
    QDir dir(dataPath);
    if (!dir.exists())
    {
        dir.mkpath(dataPath);
    }

    return dataPath;
}

void CommonHelper::saveSetting(const QString &group, const QString &key, const QVariant &value)
{
    QString iniName = QCoreApplication::applicationName() + ".ini";
    QSettings settings(dataDir() + "/" + iniName, QSettings::IniFormat);
    if (!group.isEmpty())
    {
        settings.beginGroup(group);
    }

    settings.setValue(key, value);

    if (!group.isEmpty())
    {
        settings.endGroup();
    }
}

QVariant CommonHelper::readSetting(const QString &group, const QString &key, const QVariant &defaultVal)
{
    QString iniName = QCoreApplication::applicationName() + ".ini";
    QSettings settings(dataDir() + "/" + iniName, QSettings::IniFormat);
    if (!group.isEmpty())
    {
        settings.beginGroup(group);
    }

    QVariant value = settings.value(key, defaultVal);

    if (!group.isEmpty())
    {
        settings.endGroup();
    }

    return value;
}

void CommonHelper::saveUserPwd(QString key, QString pwd)
{
    CommonHelper::saveSetting("Login", key, QString(pwd.toLocal8Bit().toBase64()));
}

QString CommonHelper::readUserPwd(QString key)
{
    QString pwdBase64 = CommonHelper::readSetting("Login", key, "").toString();
    QString pwd = QString(QByteArray::fromBase64(pwdBase64.toLocal8Bit()));

    return pwd;
}
