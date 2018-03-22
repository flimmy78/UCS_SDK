#ifndef CONTACTDEFINE_H
#define CONTACTDEFINE_H
#include <QObject>

enum ContactRole
{
    ContactIdRole = Qt::UserRole,
    SectionNameRole,
    SectionIdRole,
    parentIdRole,
    parentNameRole,
    gradeRole,
    userNumRole,
    userIdRole,
    userNameRole,
    userSexRole,
    headUrlRole,
    headPathRole,
};

typedef struct ContactItem
{
    ContactItem() {
        contactId = 0;
        sectionName = "";
        sectionId = "";
        parentId = "";
        grade = 0;
        sortNum = 0;
        userNum = 0;
        userId = "";
        userName = "";
        userSex = 0;
        headUrl = "";
        headPath = ":/images/midright/u3901.png";
    }

    QString toString() {
        return QString("%1,%2,%3,%4")
                .arg(sectionName)
                .arg(parentName)
                .arg(userId)
                .arg(userName);
    }

    int     contactId;      ///< 唯一标识 >
    QString sectionName;    ///< 部门名称 >
    QString sectionId;      ///< 部门Id >
    QString parentId;       ///< 直属部门Id >
    QString parentName;     ///< 直属部门名称 >
    int     grade;          ///< 部门层级 >
    int     sortNum;        ///< 顶级部门排序 >
    int     userNum;        ///< 本部门人员数 >
    QString userId;         ///< 人员Id >
    QString userName;       ///< 人员名称 >
    int     userSex;        ///< 人员性别 >
    QString headUrl;        ///< 头像地址 >
    QString headPath;       ///< 头像本地路径 >
} ContactItem;
typedef QList<ContactItem> ContactList;

#endif // CONTACTDEFINE_H
