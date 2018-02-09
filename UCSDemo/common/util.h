#ifndef UTIL_H
#define UTIL_H
#include <QString>

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
    static QString exePath();
    static QList<ContactUtil> parseJson(QString fileName);
    static bool sortContactData(const ContactUtil &s1, const ContactUtil &s2);
};

#endif // UTIL_H
