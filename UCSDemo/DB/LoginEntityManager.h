#ifndef LOGINENTITYMANAGER_H
#define LOGINENTITYMANAGER_H

#include <QObject>
#include <qglobal.h>
#include "DBEntity.h"

class LoginEntityManager
{
public:
    explicit LoginEntityManager();

    /*!
     * \brief addLoginInfo 添加登录信息，新用户登录
     * \param loginEntity
     * \return true for success
     */
    bool addLoginInfo(const LoginEntity &loginEntity);

    /*!
     * \brief delLoginInfo 删除用户登录信息
     * \param userId
     * \return
     */
    bool delLoginInfo(const QString &userId);

    /*!
     * \brief getLoginInfo 查询用户登录信息
     * \param userId
     * \param loginEntity
     * \return
     */
    bool getLoginInfo(const QString &userId, LoginEntity &loginEntity);

    /*!
     * \brief getAllLoginInfo 查询客户端所有用户登录信息
     * \param loginList
     * \return
     */
    bool getAllLoginInfo(LoginEntityList &loginList);


    /*!
     * \brief updateLoginInfo 更新用户登录信息
     * \param conditions
     * \param values
     * \return
     */
    bool updateLoginInfo(MapConditions conditions, MapValues values);

private:
    QString m_createSql;
};

#endif // LOGINENTITYMANAGER_H
