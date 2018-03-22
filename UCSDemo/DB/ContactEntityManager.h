#ifndef CONTACTENTITYMANAGER_H
#define CONTACTENTITYMANAGER_H

#include <QObject>
#include <qglobal.h>
#include "DBEntity.h"

class ContactEntityManager
{
public:
    explicit ContactEntityManager();

    /*!
     * \brief addContact 插入单条通讯录
     * \param contact
     * \return
     */
    bool addContact(const ContactEntity &contact);

    /*!
     * \brief addContacts 批量插入多条通讯录
     * \param contactList
     * \return
     */
    bool addContacts(const ContactEntityList &contactList);

    /*!
     * \brief getContact 根据contactId 查询通讯录
     * \param contactId
     * \param contact
     * \return
     */
    bool getContact(const int &contactId,
                    ContactEntity &contact);

    /*!
     * \brief getContact 根据userId 查询通讯录
     * \param userId
     * \param contact
     * \return
     */
    bool getContact(const QString userId, ContactEntity &contact);

    /*!
     * \brief getAllContacts 查询所有通讯录
     * \param contactList
     * \return
     */
    bool getAllContacts(ContactEntityList &contactList);

    /*!
     * \brief clearContacts 清空通讯录数据
     * \return
     */
    bool clearContacts();

    /*!
     * \brief updateContact 更新单条通讯录
     * \param condition
     * \param values
     * \return
     */
    bool updateContact(MapConditions condition, MapValues values);

private:
    QString m_createSql;
    QString m_insertSql;
};

#endif // CONTACTENTITYMANAGER_H
