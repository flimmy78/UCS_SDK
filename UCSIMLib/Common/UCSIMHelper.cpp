#include "UCSIMHelper.h"
#include "UCSClock.h"
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QStandardPaths>
#include <QXmlStreamReader>
#include "UCSTcp/UCSLogger.h"
#include <QtGui/qimage.h>

UCSIMHelper::UCSIMHelper()
{

}

QString UCSIMHelper::tempDir()
{
    QString appName =  QCoreApplication::applicationName();
    QString sysTempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString myTempPath = sysTempPath + "/" + appName;

    QDir dir(myTempPath);
    if (!dir.exists())
    {
        dir.mkpath(myTempPath);
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMHelper,
                QString("create temp path: ").append(myTempPath));
    }

    return myTempPath;
}

QString UCSIMHelper::userTempPath()
{
    QString userId = readSettings(kUcsSettingsKeyLoginId).toString();
    if (userId.isEmpty())
    {
        return QString();
    }

    QString tempPath = tempDir() + "/" + userId;
    QDir dir(tempPath);
    if (!dir.exists())
    {
        dir.mkpath(tempPath);
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMHelper,
                QString("create private temp path: ").append(tempPath));
    }

    return tempPath;
}

QString UCSIMHelper::dataDir()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    QDir dir(dataPath);
    if (!dir.exists())
    {
        dir.mkpath(dataPath);
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMHelper,
                QString("create data path: ").append(dataPath));
    }

    return dataPath;
}

QString UCSIMHelper::userDataPath()
{
    QString userId = readSettings(kUcsSettingsKeyLoginId).toString();
    if (userId.isEmpty())
    {
        return QString();
    }
    QString dataPath = dataDir() + "/" + userId;
    QDir dir(dataPath);
    if (!dir.exists())
    {
        dir.mkpath(dataPath);
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMHelper,
                QString("create private data path: ").append(dataPath));
    }

    return dataPath;
}

void UCSIMHelper::saveSetting(const UcsSettingsKey &key, const QVariant &value)
{
    QSettings settings(UCSIMHelper::dataDir() + "/UCSCfg.ini", QSettings::IniFormat);

    switch (key)
    {
    case kUcsSettingsKeyLoginId:
        settings.beginGroup("Login");
        settings.setValue("loginId", value);
        settings.endGroup();
        break;

    case kUcsSettingsKeyLoginTime:
    {
        settings.beginGroup("Login");
        settings.setValue("loginTime", value);
        settings.endGroup();
    }
        break;

    case kUcsSettingsKeyMsgSyncKey:
    {
        settings.beginGroup("Login");
        QString userId = settings.value("loginId", "").toString();
        settings.endGroup();

        if (userId.size() > 0)
        {
            settings.beginGroup(userId);
            settings.setValue("msgKey", value);
            settings.endGroup();
        }
    }
        break;

    case kUcsSettingsKeyContactSyncKey:
    {
        settings.beginGroup("Login");
        QString userId = settings.value("loginId", "").toString();
        settings.endGroup();

        if (userId.size() > 0)
        {
            settings.beginGroup(userId);
            settings.setValue("contactKey", value);
            settings.endGroup();
        }
    }
        break;

    default:
        break;
    }
}

QVariant UCSIMHelper::readSettings(const UcsSettingsKey &key)
{
    QSettings settings(UCSIMHelper::dataDir() + "/ucscfg.ini", QSettings::IniFormat);
    QVariant value;

    switch (key)
    {
    case kUcsSettingsKeyLoginId:
    {
        settings.beginGroup("Login");
        value = settings.value("loginId", "");
        settings.endGroup();
    }
        break;

    case kUcsSettingsKeyLoginTime:
    {
        settings.beginGroup("Login");
        value = settings.value("loginTime", "");
        settings.endGroup();
    }
        break;

    case kUcsSettingsKeyMsgSyncKey:
    {
        settings.beginGroup("Login");
        QString userId = settings.value("loginId", "").toString();
        settings.endGroup();

        if (!userId.isEmpty())
        {
            settings.beginGroup(userId);
            value = settings.value("msgKey", 0);
            settings.endGroup();
        }
    }
        break;

    case kUcsSettingsKeyContactSyncKey:
    {
        settings.beginGroup("Login");
        QString userId = settings.value("loginId", "").toString();
        settings.endGroup();

        if (!userId.isEmpty())
        {
            settings.beginGroup(userId);
            value = settings.value("contactKey", 0);
            settings.endGroup();
        }
    }
        break;

    default:
        break;
    }

    return value;
}

bool UCSIMHelper::checkConversationType(UCS_IM_ConversationType conversationType)
{
    if ((conversationType != UCS_IM_SOLOCHAT) &&
        (conversationType != UCS_IM_GROUPCHAT) &&
        (conversationType != UCS_IM_DISCUSSIONCHAT) &&
        (conversationType != UCS_IM_Broadcast))
    {
        return false;
    }

    return true;
}

bool UCSIMHelper::checkMsgType(UCS_IM_MsgType msgType)
{
    /* PC端暂不支持定位和语音消息发送 */
    if ((msgType != UCS_IM_TEXT) &&
        (msgType != UCS_IM_IMAGE) &&
//        (msgType != UCS_IM_Location) &&
//        (msgType != UCS_IM_VOICE) &&
        (msgType != UCS_IM_Custom))
    {
        return false;
    }

    return true;
}

QString UCSIMHelper::messageId()
{
    qint64 time = UCSClock::TimeInMicroseconds();
    qint32 msgKey = readSettings(kUcsSettingsKeyMsgSyncKey).toInt();
    qint64 msgId = (time + msgKey);

    return QString::number(msgId);
}

QString UCSIMHelper::recvMsgId(qint32 index)
{
    qint64 time = UCSClock::TimeInMicroseconds();
    qint32 msgKey = readSettings(kUcsSettingsKeyMsgSyncKey).toInt();
    qint64 msgId = (time + msgKey + index);

    return QString::number(msgId);
}

QString UCSIMHelper::formatToUserName(QString targetId, UCS_IM_ConversationType conversationType)
{
    if (conversationType == UCS_IM_DISCUSSIONCHAT)
    {
        return targetId.append("@group");
    }
    else if (conversationType == UCS_IM_GROUPCHAT)
    {
        return targetId.append("@chatroom");
    }
    else
    {
        return targetId;
    }
}

QString UCSIMHelper::getFromName(QString fromUserName)
{
    if (fromUserName.contains("@"))
    {
        return fromUserName.section('@', 0, 0);
    }

    return fromUserName;
}

UCS_IM_ConversationType UCSIMHelper::getConvTypeByFromUserName(const QString fromUserName)
{
    if (fromUserName.contains("@"))
    {
        if (fromUserName.contains("@chatroom"))
        {
            return UCS_IM_GROUPCHAT;
        }
        else if (fromUserName.contains("@group"))
        {
            return UCS_IM_DISCUSSIONCHAT;
        }
        else if (fromUserName.contains("@broadcast"))
        {
            return UCS_IM_Broadcast;
        }
        else
            return UCS_IM_UnknownConversationType;
    }
    else
    {
        return UCS_IM_SOLOCHAT;
    }
}

QString UCSIMHelper::saveImage(const QString targetId, const QString msgId, const QByteArray data)
{
    QString path = userTempPath().append("/%1").arg(targetId);
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(path);
    }

    QString fullPath = path.append("/%2.jpg").arg(msgId);
    QImage image;
    if (image.loadFromData(data))
    {
        if (image.save(fullPath, "JPEG", 100))
        {
            return fullPath;
        }
    }

    return QString();
}

QString UCSIMHelper::saveVoice(const QString targetId, const QString msgId, const QByteArray data)
{
    QString path = userTempPath().append("/%1").arg(targetId);
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(path);
    }

    QString fullPath = path.append("/%1.amr").arg(targetId).arg(msgId);
    QFile amrFile(fullPath);
    if (amrFile.open(QIODevice::WriteOnly))
    {
        amrFile.write(data);

        return fullPath;
    }

    return QString();
}

///< content format like: "<msg><imgmsg cdnbigimgurl=\"url\" length=\"100020\"></imgmsg></msg>" >
/// the url may content '&' that not support in xml attribute
QString UCSIMHelper::getBigImgUrl(QString content)
{
    ///< xml属性中不支持'&', 需提前转换 >
    QString strXml = content.replace('&', ',');
    QXmlStreamReader reader(strXml);
    while(!reader.atEnd())
    {
        QXmlStreamReader::TokenType type = reader.readNext();
        if (type == QXmlStreamReader::StartElement)
        {
            QString tagName = reader.name().toString();
            if (QString::compare(tagName, "msg") == 0)
            {
                while (!reader.atEnd())
                {
                    reader.readNext();
                    QString name = reader.name().toString();
                    if (QString::compare(name, "imgmsg") == 0)
                    {
                        QXmlStreamAttributes attributes = reader.attributes();
                        if (attributes.hasAttribute("cdnbigimgurl"))
                        {
                            QString url = attributes.value("cdnbigimgurl").toString();
                            ///< 解析完后恢复替换字符 >
                            url = url.replace(',', '&');
                            return url;
                        }
                    }
                }
            }
        }
    }

    return QString();
}

///< content format: "<msg><voicemsg endflag=\"1\" length=\"6950\" voicelength=\"4\" clientmsgid=\"1518163940384\" fromusername=\"15919776461\" downcount=\"0\" cancelflag=\"0\" forwardflag=\"0\" /></msg>" >
void UCSIMHelper::getVoiceDownloadInfo(const QString content,
                                       qint32 &lenInBytes,
                                       qint32 &duration,
                                       QString &msgId)
{
    QXmlStreamReader reader(content);
    while(!reader.atEnd())
    {
        QXmlStreamReader::TokenType type = reader.readNext();
        if (type == QXmlStreamReader::StartElement)
        {
            QString tagName = reader.name().toString();
            if (QString::compare(tagName, "msg") == 0)
            {
                while (!reader.atEnd())
                {
                    reader.readNext();
                    QString name = reader.name().toString();
                    if (QString::compare(name, "voicemsg") == 0)
                    {
                        QXmlStreamAttributes attributes = reader.attributes();
                        if (attributes.hasAttribute("length"))
                        {
                            lenInBytes = attributes.value("length").toInt();
                        }
                        if (attributes.hasAttribute("voicelength"))
                        {
                            duration = attributes.value("voicelength").toInt();
                        }
                        if (attributes.hasAttribute("clientmsgid"))
                        {
                            msgId = attributes.value("clientmsgid").toString();
                        }
                    }
                }
            }
        }
    }
}
