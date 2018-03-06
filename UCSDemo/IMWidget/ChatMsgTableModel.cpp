#include "ChatMsgTableModel.h"
#include <QPixmap>
#include "CommonHelper.h"

ChatMsgTableModel::ChatMsgTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_pMsgList(Q_NULLPTR)
{
}

int ChatMsgTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    if (m_pMsgList == Q_NULLPTR)
    {
        return 0;
    }

    return m_pMsgList->count();
}

int ChatMsgTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 1;
}

QVariant ChatMsgTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    if (m_pMsgList == Q_NULLPTR || m_pMsgList->isEmpty())
    {
        return QVariant();
    }

    ChatMsgItem item = m_pMsgList->at(index.row());
    switch (role) {
    case HeaderRole:
    {
        QPixmap pixmap;
        pixmap.load(item.headerUrl);
        return pixmap.scaled(35, 35, Qt::KeepAspectRatio, Qt::FastTransformation);
    }

    case NickNameRole:
    {
        return item.nickName;
    }

    case UserNameRole:
    {
        return item.userName;
    }

    case SendTimeRole:
    {
        return CommonHelper::timeToString(item.timestamp, "MM-dd hh:mm:ss");
    }

    case DirectionRole:
    {
        return item.isSender;
    }

    case MsgTypeRole:
    {
        return (qint32)item.type;
    }

    case ContentRole:
    {
        return item.content;
    }

    default:
        break;
    }
    return QVariant();
}

bool ChatMsgTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
}

bool ChatMsgTableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}

void ChatMsgTableModel::setModelData(QList<ChatMsgItem> *pChatMsgList)
{
    m_pMsgList = pChatMsgList;
}

void ChatMsgTableModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}
