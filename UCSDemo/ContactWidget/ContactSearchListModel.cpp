#include "ContactSearchListModel.h"

#include <QPixmap>

ContactSearchListModel::ContactSearchListModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_pContactList(Q_NULLPTR)
{
}

int ContactSearchListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    if (m_pContactList == Q_NULLPTR)
    {
        return 0;
    }

    return m_pContactList->count();
}

QVariant ContactSearchListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || m_pContactList->isEmpty())
        return QVariant();

    // FIXME: Implement me!
    ContactItem contact = m_pContactList->at(index.row());

    if (role == ContactIdRole)
    {
        return contact.contactId;
    }
    else if (role == SectionNameRole)
    {
        return contact.sectionName;
    }
    else if (role == parentIdRole)
    {
        return contact.parentId;
    }
    else if (role == parentNameRole)
    {
        return contact.parentName;
    }
    else if (role == userNumRole)
    {
        return contact.userNum;
    }
    else if (role == userIdRole)
    {
        return contact.userId;
    }
    else if (role == userNameRole)
    {
        return contact.userName;
    }
    else if (role == userSexRole)
    {
        return contact.userSex;
    }
//    else if (role == headUrlRole)
//    {
//        return contact.headUrl;
//    }
    else if (role == headPathRole)
    {
        QPixmap pixmap;
        pixmap.load(contact.headPath);
        return pixmap.scaled(44, 44, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }

    return QVariant();
}

bool ContactSearchListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();

    return true;
}

bool ContactSearchListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();

    return true;
}

void ContactSearchListModel::setModelData(ContactList *pContactList)
{
    m_pContactList = pContactList;
    refreshModel();
}

void ContactSearchListModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}
