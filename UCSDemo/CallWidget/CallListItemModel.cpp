#include "CallListItemModel.h"

CallListItemModel::CallListItemModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_pCallList(Q_NULLPTR)
{

}

int CallListItemModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    if (m_pCallList == Q_NULLPTR)
    {
        return 0;
    }

    return m_pCallList->count();
}

QVariant CallListItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    CallListItem item = m_pCallList->at(index.row());

    if (role == Qt::UserRole)
    {
        QPixmap pixmap;
        pixmap.load(item.headerPath);
        return pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::FastTransformation);
    }
    else if (role == Qt::UserRole + 1)
    {
        return item.nickName;
    }
    else if (role == Qt::UserRole + 2)
    {
        return item.callTime;
    }
    else if (role == Qt::UserRole + 3)
    {
        return item.callType;
    }
    else if (role == Qt::UserRole + 4)
    {
        QPixmap pixmap;
        pixmap.load(item.typeIconPath);
        return pixmap.scaled(20, 20, Qt::KeepAspectRatio, Qt::FastTransformation);
    }

    return QVariant();
}

bool CallListItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    endRemoveRows();

    return true;
}

void CallListItemModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}

void CallListItemModel::setModelData(QList<CallListItem> *pDataList)
{
    m_pCallList = pDataList;
}
