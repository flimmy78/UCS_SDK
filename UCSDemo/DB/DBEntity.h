#ifndef DBENTITY_H
#define DBENTITY_H
#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>
#include <QDateTime>
#include "ContactDefine.h"

///< 更新时用到的键值对，为此，所有实体的字段都用QString表示 >
typedef QMap<QString, QString> MapValues, MapConditions;

typedef struct LoginEntity
{
    LoginEntity()
    {
        userId.clear();
        userName.clear();
        userPwd.clear();
        summitPwd.clear();
        headUrl.clear();
        headPath.clear();
        isKeepPwd = false;
        userSex = 0;
        token.clear();
        time = QString::number(QDateTime::currentSecsSinceEpoch());
    }

    QString userId;     ///< 用户Id >
    QString userName;   ///< 用户名 >
    QString userPwd;    ///< 用户密码 >
    QString summitPwd;  ///< summit会议系统密码 >
    QString headUrl;    ///< 用户头像地址 >
    QString headPath;   ///< 用户头像本地存储路径 >
    bool    isKeepPwd;  ///< 是否记住密码, 0 不保存, 1 保存>
    int     userSex;    ///< 用户性别, 1 man, 2 women >
    QString token;      ///< PaaS登录token >
    QString time;       ///< 登录时间 >
    QString extra1;
    QString extra2;
    QString extra3;
    QString extra4;
    QString extra5;
    QString extra6;
} LoginEntity;
typedef QList<LoginEntity> LoginEntityList;

typedef struct ContactEntity
{
    ContactEntity()
    {
        contactId = 0;
        sectionName.clear();
        sectionId.clear();
        parentId.clear();
        parentName.clear();
        grade = 0;
        sortNum = 0;
        userNum = 0;
        userId.clear();
        userName.clear();
        userSex = 0;
        headUrl.clear();
        headPath.clear();
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
    QString extra1;
    QString extra2;
    QString extra3;
    QString extra4;
    QString extra5;
    QString extra6;
} ContactEntity;
typedef QList<ContactEntity> ContactEntityList;

class DBEntity
{
public:
    explicit DBEntity() {}

    static void convertContact2Entity(const ContactItem &contact, ContactEntity &entity);
    static void convertEntity2Contact(const ContactEntity &entity, ContactItem &contact);

};

#endif // DBENTITY_H
