#ifndef HISTORYCALLLISTVIEW_H
#define HISTORYCALLLISTVIEW_H

#include "common/qtheaders.h"
#include "historycallmodel.h"

class HistoryCallListView : public QListView
{
    Q_OBJECT

public:
    explicit HistoryCallListView(QWidget* parent = 0);
    void updateData(QList<CallHistory*> pList);

protected:
    void contextMenuEvent(QContextMenuEvent* event);

signals:

public slots:
    void slot_deleteItem();
    void slot_dialItem();
    void slot_clickItem(QModelIndex index);

private:
    HistoryCallModel* m_pCallModel;
};

#endif // HISTORYCALLLISTVIEW_H
