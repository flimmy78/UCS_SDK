#include "ConversationItemModel.h"
#include <QDateTime>
#include <QPixmap>
#include "Interface/UCSIMSDKPublic.h"
#include "Interface/UCSLogger.h"

ConversationItemModel::ConversationItemModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_pDataList(Q_NULLPTR)
{
//    m_conversationList.clear();
}

//QModelIndex ConversationItemModel::index(int row, int column, const QModelIndex &parent) const
//{

//}

//QModelIndex ConversationItemModel::parent(const QModelIndex &index) const
//{

//}

int ConversationItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    if (m_pDataList == Q_NULLPTR)
    {
        return 0;
    }

    return m_pDataList->count();
}

//int ConversationItemModel::columnCount(const QModelIndex &parent) const
//{

//}

QVariant ConversationItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= m_pDataList->size()))
        return QVariant();

    // FIXME: Implement me!
    ConversationItem item = m_pDataList->at(index.row());
//    if (role == Qt::DisplayRole)
//    {
//        return item.conversationTitle;
//    }
    if (role == Qt::UserRole)
    {
        return item.conversationTitle;
    }
    else if (role == Qt::UserRole + 1)
    {
        QPixmap pixmap;
        pixmap.load(item.headerPath);
        return pixmap.scaled(44, 44, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }
    else if (role == Qt::UserRole + 2)
    {
        return item.lastMsgTime;
    }
    else if (role == Qt::UserRole + 3)
    {
        return item.lastMsgContent;
    }

    return QVariant();
}

bool ConversationItemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIX ME
    endInsertRows();

    return true;
}

bool ConversationItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    endRemoveRows();

    return true;
}

void ConversationItemModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}

void ConversationItemModel::setModelData(QList<ConversationItem> *dataList)
{
    m_pDataList = dataList;
}
