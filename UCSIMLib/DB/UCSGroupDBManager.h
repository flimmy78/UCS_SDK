#ifndef GROUPDBMANAGER_H
#define GROUPDBMANAGER_H

#include <QObject>
#include <qglobal.h>
#include <UCSDBEntity.h>

class GroupDBManager
{
public:
    explicit GroupDBManager();

    /*!
     * \brief addGroup 添加单条群组信息
     * \param group
     * \return true for success, else false
     */
    bool addGroup(GroupEntity *group);

    /*!
     * \brief addGroup 添加多条群组信息
     * \param groupList
     * \return true for success, else false
     */
    bool addGroup(QList<GroupEntity> groupList);

    /*!
     * \brief delGroup 删除指定groupId的群组
     * \param groupId
     * \return true for success, else false
     */
    bool delGroup(QString groupId);

    /*!
     * \brief getGroup 查询指定groupId的群组信息
     * \param groupId
     * \param pGroup 群组查询结果
     * \return true for success, else false
     */
    bool getGroup(QString groupId, GroupEntity &group);

    /*!
     * \brief getAllGroups 查询所有群组信息
     * \return 所有群组信息列表
     */
    QList<GroupEntity> getAllGroups();

    /*!
     * \brief updateGroup 更新指定条件的group信息
     * \param values 待更新值的key-value对
     * \param conditions 指定的更新条件，And 关系
     * \return true for success
     */
    bool updateGroup(MapValues values, MapConditions conditions);

private:
    QString m_createSql;
};

#endif // GROUPDBMANAGER_H
