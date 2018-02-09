#include "historycallmodel.h"

HistoryCallModel::HistoryCallModel(QObject *parent)
    : QAbstractListModel(parent)
{
    updateData();
}

QVariant HistoryCallModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    return QVariant();
}

int HistoryCallModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_callList.count();
}

QVariant HistoryCallModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    CallHistory* callhistory = m_callList.at(index.row());

    if (role == Qt::UserRole)
    {
        return callhistory->headerPath;
    }
    else if (role == Qt::UserRole + 1)
    {
        return callhistory->nickName;
    }
    else if (role == Qt::UserRole + 2)
    {
        return callhistory->callTime;
    }
    else if (role == Qt::UserRole + 3)
    {
        return callhistory->callType;
    }
    else if (role == Qt::UserRole + 4)
    {
        return callhistory->typeIconPath;
    }

    return QVariant();
}

bool HistoryCallModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool HistoryCallModel::insertRow(int row, const QModelIndex &parent)
{
    return true;
}

bool HistoryCallModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();

    return true;
}

bool HistoryCallModel::removeRow(int row, const QModelIndex &parent)
{
    m_callList.removeAt(row);
    return true;
}

void HistoryCallModel::updateData()
{
    for (int i = 0; i < 10; i++)
    {
//        QString str = QString("XiaoMing_%1").arg(i);
//        QString str = QString().arg(i);
        m_callList.append(new CallHistory(":/images/mainleft/u6.png",
                                          QStringLiteral("川流不息"),
                                          "2018/01/05",
                                          QStringLiteral("电话"),
                                          ":/images/u73.png"));
    }
}

CallHistory *HistoryCallModel::getRowData(int row)
{
    if (row < m_callList.count())
    {
        return m_callList.at(row);
    }

    return NULL;
}

void HistoryCallModel::deleteRow(int row)
{
    if (row < m_callList.count())
    {
        m_callList.removeAt(row);
    }
}
