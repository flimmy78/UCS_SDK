#ifndef HISTORYCALLITEM_H
#define HISTORYCALLITEM_H

#include <QAbstractListModel>
#include "common/qtheaders.h"

class CallHistory
{
public:
    CallHistory(QString hPath,
                QString name,
                QString time,
                QString type,
                QString tPath)
    {
        headerPath = hPath;
        nickName = name;
        callTime = time;
        callType = type;
        typeIconPath = tPath;
    }

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
};

class HistoryCallModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit HistoryCallModel(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool insertRow(int row, const QModelIndex &parent);

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRow(int row, const QModelIndex &parent);

    // update data
    void updateData();

    CallHistory *getRowData(int row);
    void deleteRow(int row);

private:
    QList<CallHistory *> m_callList;
};

#endif // HISTORYCALLITEM_H
