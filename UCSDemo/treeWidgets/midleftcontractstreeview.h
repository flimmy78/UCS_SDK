#ifndef MIDLEFTCONTRACTSTREEVIEW_H
#define MIDLEFTCONTRACTSTREEVIEW_H

#include "common/qtheaders.h"
#include "treeWidgets/midLeftContactTreeModel.h"

class MidLeftContractsTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit MidLeftContractsTreeView(QWidget *parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    void init();
    void initConnection();
    void initMenu();
    void loadStyleSheet();

signals:

public slots:
    void slot_itemClicked(QModelIndex index);
    void slot_doubleClicked(QModelIndex index);
    void slot_itemPressed(QModelIndex index);

    /* menu action */
    void slot_addGroup(bool checked);
    void slot_delGroup(bool checked);
    void slot_addPerson(bool checked);
    void slot_removePerson(bool checked);
    void slot_sendMessage(bool checked);
    void slot_audioCall(bool checked);
    void slot_videoCall(bool checked);

private:
    MidLeftContactTreeModel *m_pContactModel;
    QMenu *m_pGroupMenu;
    QMenu *m_pPersonMenu;
};

#endif // MIDLEFTCONTRACTSTREEVIEW_H
