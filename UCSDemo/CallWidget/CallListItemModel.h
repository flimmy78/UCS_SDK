#ifndef HISTORYCALLITEM_H
#define HISTORYCALLITEM_H

#include <QAbstractListModel>
#include "common/qtheaders.h"

typedef struct
{
    quint32 callId;

    /* 头像路径 */
    QString headerPath;
    /* 昵称 */
    QString nickName;
    /* 呼叫时间 */
    QString callTime;
    /* 呼叫类型 */
    QString callType;
    /* 呼叫类型图片路径 */
    QString typeIconPath;
} CallListItem;

class CallListItemModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CallListItemModel(QObject *parent = 0);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // remove data
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // refresh model
    void refreshModel();
    void setModelData(QList<CallListItem> *pDataList);

private:
    QList<CallListItem> *m_pCallList;
};

#endif // HISTORYCALLITEM_H
