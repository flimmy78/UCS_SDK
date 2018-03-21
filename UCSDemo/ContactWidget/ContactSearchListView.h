#ifndef CONTACTSEARCHLISTVIEW_H
#define CONTACTSEARCHLISTVIEW_H

#include <QListView>
#include <QObject>
#include <qglobal.h>
#include "ContactSearchListModel.h"
#include "ContactDefine.h"

class ContactSearchListView : public QListView
{
    Q_OBJECT

public:
    explicit ContactSearchListView(QWidget *parent = nullptr);
    virtual ~ContactSearchListView();

    void setSearchResult(const ContactList &contactList);
    void clearSearchResult();

private:
    void initConnections();
    void initData();

signals:
    void sigItemClicked(ContactItem);

public slots:

private slots:
    void onItemClicked(QModelIndex index);

private:
    ContactSearchListModel *m_pListModel;
    ContactList m_contactList;
};

#endif // CONTACTSEARCHLISTVIEW_H
