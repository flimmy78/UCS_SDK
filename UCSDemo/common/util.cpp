#include "util.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QSettings>

QString Util::appDir()
{
    return QCoreApplication::applicationDirPath();
}

QString Util::appName()
{
    return QCoreApplication::applicationName();
}

QList<ContactUtil> Util::parseJson(QString fileName)
{
    QList<ContactUtil> contactList;
    QList<ContactUtil> personList;

    if (fileName.isEmpty())
    {
        return contactList;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return contactList;
    }

    QByteArray byteArray = file.readAll();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(byteArray, &jsonError);
    if (!document.isNull() &&
        jsonError.error == QJsonParseError::NoError)
    {
        if (!document.isArray())
        {
            return contactList;
        }

        int id = 0;
        QJsonArray jsonArray = document.array();
        int arraySize = jsonArray.size();
        for (int idx = 0; idx < arraySize; idx++)
        {
            QString secName;
            QString secId;
            int secGrade = 0;

            QJsonValue value = jsonArray.at(idx);

            if (!value.isObject())
            {
                continue;
            }

            ContactUtil section;

            QJsonObject object = value.toObject();
            if (object.contains("name"))
            {
                QJsonValue name = object["name"];
                if (name.isString())
                {
                    secName = name.toString();
                }
            }
            if (object.contains("sectionid"))
            {
                secId = object["sectionid"].toString();
            }
            if (object.contains("grade"))
            {
                secGrade = object["grade"].toInt();
            }

            /* add top section */
            if (secGrade == 1)
            {
                section.name = secName;
                section.sectionId = secId;
                section.grade = secGrade;
                section.parentSecId = "top";
                section.sex = -1;
                contactList.append(section);
            }

            if (object.contains("section"))
            {
                QJsonArray childSectionArray = object["section"].toArray();
                for (int i = 0; i < childSectionArray.size(); i++)
                {
                    QJsonObject childSecObj = childSectionArray.at(i).toObject();
                    ContactUtil childSec;

                    childSec.deptName = secName;
                    childSec.parentSecId = secId;
                    childSec.sex = -1;
                    if (childSecObj.contains("name"))
                    {
                        childSec.name = childSecObj["name"].toString();
                    }
                    if (childSecObj.contains("sectionid"))
                    {
                        childSec.sectionId = childSecObj["sectionid"].toString();
                    }
                    if (childSecObj.contains("grade"))
                    {
                        childSec.grade = object["grade"].toInt();
                    }

                    contactList.append(childSec);
                }
            }
            if (object.contains("person"))
            {
                QJsonArray personArray = object["person"].toArray();
                for (int i = 0; i < personArray.size(); i++)
                {
                    QJsonObject personObj = personArray.at(i).toObject();
                    ContactUtil person;

                    person.deptName = secName;
                    person.parentSecId = secId;
                    person.grade = secGrade;
                    if (personObj.contains("userid"))
                    {
                        /* person的userid当做是自己的sectionId，方便形成树的唯一标识 */
                        person.sectionId = personObj["userid"].toString();
                    }
                    if (personObj.contains("name"))
                    {
                        person.name = personObj["name"].toString();
                    }
                    if (personObj.contains("sex"))
                    {
                        if (!QString::compare(personObj["sex"].toString(), QStringLiteral("男"), Qt::CaseSensitive))
                        {
                            person.sex = 0;
                        }
                        else if (!QString::compare(personObj["sex"].toString(), QStringLiteral("女"), Qt::CaseSensitive))
                        {
                            person.sex = 1;
                        }
                    }
                    personList.append(person);
                }
            }
        }
    }

    qSort(contactList.begin(), contactList.end(), sortContactData);
//    for (int i = 0; i < personList.count(); i++)
//    {
//        contactList.append(personList.at(i));
//    }
    contactList.append(personList);
    return contactList;
}

bool Util::sortContactData(const ContactUtil &c1, const ContactUtil &c2)
{
    int res = QString::compare(c1.sectionId, c2.sectionId, Qt::CaseSensitive);
    if (res < 0)
    {
        return true;
    }
    else if (res == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QVariant Util::readSetting(QString &key, const QVariant &defaultVal)
{
    QSettings settings(appDir() + "/ucsdemo.ini", QSettings::IniFormat);
    return settings.value(key, defaultVal);
}

void Util::writeSetting(const QString &key, const QVariant &value)
{
    QSettings settings(appDir() + "/ucsdemo.ini", QSettings::IniFormat);
    settings.setValue(key, value);
}
