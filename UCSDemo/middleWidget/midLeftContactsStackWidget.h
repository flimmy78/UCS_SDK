#ifndef ADDRESSBOOKWIDGET_H
#define ADDRESSBOOKWIDGET_H

#include <QObject>
#include <QWidget>
#include "absFiles/myScrollArea.h"
#include "midleftcontractstreeview.h"

class MidLeftContactsStackWidget : public MyScrollArea
{
    Q_OBJECT

public:
    explicit MidLeftContactsStackWidget(QWidget *parent = 0, int width = 250);
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
