#ifndef CONTRACTS_TREE_VIEW_H
#define CONTRACTS_TREE_VIEW_H

#include "common/qtheaders.h"
#include "ContactTreeItemModel.h"
#include "UPlusRestApi.h"

class ContactsTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit ContactsTreeView(QWidget *parent = 0);
    virtual ~ContactsTreeView();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    void init();
    void initConnection();
    void initMenu();
    void loadStyleSheet();

signals:

public slots:
    ///< item slot >
    void onItemClicked(QModelIndex index);
    void onItemDoubleClicked(QModelIndex index);
    void onItemPressed(QModelIndex index);

    ///< menu action >
    void onAddGroupAction(bool checked);
    void onDeleteGroupAction(bool checked);
    void onAddContactAction(bool checked);
    void onRemoveContactAction(bool checked);
    void onSendMessageAction(bool checked);
    void onAudioCallAction(bool checked);
    void onVideoCallAction(bool checked);

    ///< contact update action >
    void onUpdateContactsReply(QByteArray data, int code);

private:
    ContactTreeItemModel *m_pContactModel;
    QMenu *m_pGroupMenu;
    QMenu *m_pPersonMenu;

    UPlusRestApi *m_pRestApi;
};

#endif // MIDLEFTCONTRACTSTREEVIEW_H
