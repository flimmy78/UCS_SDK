#ifndef USERINFODBMANAGER_H
#define USERINFODBMANAGER_H

#include <QObject>
#include <qglobal.h>
#include <QMap>
#include <UCSDBEntity.h>

class UserInfoDBManager
{
public:
    explicit UserInfoDBManager();

    /*!
     * \brief addUserInfo 添加一条用户信息
     * \param userInfo 用户信息
     * \return true for success
     */
    bool addUserInfo(UserInfoEntity *userInfo);

    /*!
     * \brief addUserInfo 一次性添加多条用户信息
     * \detail 使用数据库事务操作
     * \param userList 用户信息列表
     * \return true for success
     */
    bool addUserInfo(QList<UserInfoEntity> userList);

    /*!
     * \brief delUserInfo 删除指定userid的用户信息
     * \param userId
     * \return true for success
     */
    bool delUserInfo(QString userId);

    /*!
     * \brief getAllUserInfo 查询所有用户信息
     * \return 所有用户信息列表
     */
    QList<UserInfoEntity> getAllUserInfo();

    /*!
     * \brief getUserInfo 返回指定userId的用户信息
     * \param userId
     * \param userInfo 返回的用户信息
     * \return true for success
     */
    bool getUserInfo(QString userId, UserInfoEntity &userInfo);

    /*!
     * \brief updateUserInfo 更新用户信息
     * \param values 待更新值的key-value对
     * \param conditions 指定更新条件
     * \return true for success
     */
    bool updateUserInfo(MapValues values, MapConditions conditions);

private:
    QString m_createSql;
    QString m_insertSql;
};

#endif // USERINFODBMANAGER_H
