#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include <QWidget>
#include <qglobal.h>
#include "BaseWidget.h"
#include "ContractsTreeView.h"
#include "ContactInfoWidget.h"
#include "topWidget.h"
#include "searchLineEdit.h"

class ContactWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit ContactWidget(QWidget *parent = 0);

private:
    void initLayout();
    void initConnections();

signals:

public slots:

private:
    TopWidget *m_pTitleBar;
    SearchLineEdit *m_pSearchEdit;
    ContractsTreeView *m_pContactListView;
    ContactInfoWidget *m_pContactInfoWidget;
};

#endif // CONTACTWIDGET_H
