#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include <QWidget>
#include <qglobal.h>
#include <QStackedLayout>
#include "BaseWidget.h"
#include "ContactsTreeView.h"
#include "ContactInfoWidget.h"
#include "topWidget.h"
#include "searchLineEdit.h"

class ContactWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit ContactWidget(QWidget *parent = 0);

    ContactsTreeView *contactListView() const;

private:
    void initLayout();
    void initConnections();

signals:

public slots:
    void onContactViewClicked(ContactItem contact);

private:
    TopWidget *m_pTitleBar;
    SearchLineEdit *m_pSearchEdit;
    ContactsTreeView *m_pContactListView;
    ContactInfoWidget *m_pContactInfoWidget;
    QStackedLayout *m_pStackedLayout;
};

#endif // CONTACTWIDGET_H
