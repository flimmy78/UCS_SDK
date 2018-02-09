#ifndef ADDRESSBOOKWIDGET_H
#define ADDRESSBOOKWIDGET_H

#include <QObject>
#include <QWidget>
#include "absFiles/myScrollArea.h"
#include "treeWidgets/midleftcontractstreeview.h"

class MidLeftContactsStackWidget : public MyScrollArea
{
    Q_OBJECT

public:
    explicit MidLeftContactsStackWidget(QWidget *parent = 0);
    ~MidLeftContactsStackWidget() {}

private:
    void initLayout();
    void initConnections();

signals:

public slots:

private:
    MidLeftContractsTreeView m_contactView;
};

#endif // ADDRESSBOOKWIDGET_H
