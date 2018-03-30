#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include <QWidget>
#include <qglobal.h>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QFutureWatcher>
#include <QtConcurrent>
#include "BaseWidget.h"
#include "ContactsTreeView.h"
#include "ContactInfoWidget.h"
#include "TopBarWidget.h"
#include "searchLineEdit.h"
#include "ContactSearchListView.h"


class ContactWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit ContactWidget(QWidget *parent = 0);
    virtual ~ContactWidget();

    ContactsTreeView *contactListView() const;
    ContactInfoWidget *contactInfoWidget() const;
    void updateLogin();

    static ContactList searchContact(ContactWidget *pObj, QString strSearch);

private:
    void init();
    void initLayout();
    void initConnections();

signals:

public slots:
    void onContactViewClicked(ContactItem contact);
    void onSearchContact();
    void onSearchFocusChanged(bool isFocusIn);    

    void onSearchFinished();

private:
    TopBarWidget *m_pTitleBar;
    SearchLineEdit *m_pSearchEdit;
    ContactsTreeView *m_pContactTreeView;
    ContactInfoWidget *m_pContactInfoWidget;
    QStackedLayout *m_pStackedLayout;
    ContactSearchListView *m_pSearchListView;

    QFutureWatcher<ContactList> *m_pSearchWatcher;
};

#endif // CONTACTWIDGET_H
