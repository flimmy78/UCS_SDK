#ifndef DISCUSSIONDBMANAGER_H
#define DISCUSSIONDBMANAGER_H

#include <QObject>
#include <qglobal.h>
#include <UCSDBEntity.h>

class DiscussionDBManager
{
public:
    explicit DiscussionDBManager();

    /*!
     * \brief addDiscussioin 添加一条讨论组信息
     * \param discussion
     * \return true for success, else false
     */
    bool addDiscussioin(const DiscussionEntity *discussion);

    /*!
     * \brief addDiscussioin 批量增加多条讨论组信息
     * \param discussionList
     * \return true for success, else false
     */
    bool addDiscussioin(const QList<DiscussionEntity> discussionList);

    /*!
     * \brief delDiscussion 删除指定discussionId的讨论组信息
     * \param discussionId
     * \return true for success, else false
     */
    bool delDiscussion(const QString discussionId);

    /*!
     * \brief getDiscussion 查询指定discussionId的讨论组信息
     * \param discussionId
     * \param discussion 返回的讨论组信息
     * \return true for success, else false
     */
    bool getDiscussion(const QString discussionId, DiscussionEntity &discussion);

    /*!
     * \brief getAllDiscussions 查询所有讨论组信息
     * \return 讨论组信息列表
     */
    QList<DiscussionEntity> getAllDiscussions();

    /*!
     * \brief updateDiscussion 更新指定条件对应的讨论组信息
     * \param values 待更新的值的key-value对
     * \param conditions 指定的更新条件
     * \return true for success, else false
     */
    bool updateDiscussion(MapValues values, MapConditions conditions);

private:
    QString m_createSql;
    QString m_insertSql;
};

#endif // DISCUSSIONDBMANAGER_H
