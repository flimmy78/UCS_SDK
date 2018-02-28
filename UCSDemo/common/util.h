#ifndef UTIL_H
#define UTIL_H
#include <QString>
#include <QVariant>

typedef struct
{
    QString name;
    QString userId;
    int sex;
    QString sectionId;
    QString parentSecId;
    QString deptName;
    int grade;
} ContactUtil;

class Util
{
public:
    Util();
    static QString appDir();
    static QString appName();

    static QList<ContactUtil> parseJson(QString fileName);
    static bool sortContactData(const ContactUtil &s1, const ContactUtil &s2);

    static QVariant readSetting(QString &key, const  QVariant &defaultVal = QVariant());
    static void writeSetting(const QString &key, const QVariant &value);
};

#endif // UTIL_H
