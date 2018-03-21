#ifndef CONTACTSEARCHLISTMODEL_H
#define CONTACTSEARCHLISTMODEL_H

#include <QAbstractListModel>
#include <qglobal.h>
#include "ContactDefine.h"

class ContactSearchListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ContactSearchListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void setModelData(ContactList *pContactList);
    void refreshModel();

private:
    ContactList *m_pContactList;
};

#endif // CONTACTSEARCHLISTMODEL_H
