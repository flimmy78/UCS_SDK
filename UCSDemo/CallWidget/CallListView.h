#ifndef HISTORYCALLLISTVIEW_H
#define HISTORYCALLLISTVIEW_H

#include "common/qtheaders.h"
#include "CallListItemModel.h"

class CallListView : public QListView
{
    Q_OBJECT

public:
    explicit CallListView(QWidget* parent = 0);

    void updateListData();

protected:
    void contextMenuEvent(QContextMenuEvent* event);

private:
    void initConnections();

signals:

public slots:
    void onDeleteItemAction();
    void onDialItemAction();
    void onItemClicked(QModelIndex index);

private:
    CallListItemModel* m_pCallModel;
    QPoint m_eventPos;
    QList<CallListItem> m_callList;
};

#endif // HISTORYCALLLISTVIEW_H
