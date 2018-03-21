#ifndef CONVERSATIONITEMMODEL_H
#define CONVERSATIONITEMMODEL_H

#include <QAbstractItemModel>
#include <qglobal.h>
#include "IMCommonDefine.h"

class ConversationItemModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ConversationItemModel(QObject *parent = 0);

    // Basic functionality
//    QModelIndex index(int row, int column,
//                      const QModelIndex &parent = QModelIndex()) const override;
//    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // remove data
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // refresh model
    void refreshModel();

    void setModelData(QList<ConversationItem> *dataList);

private:
    QList<ConversationItem> *m_pDataList;
};

#endif // CONVERSATIONITEMMODEL_H
