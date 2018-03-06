#ifndef ADDRESSBOOKWIDGET_H
#define ADDRESSBOOKWIDGET_H

#include <QObject>
#include <QWidget>
#include "absFiles/myScrollArea.h"
#include "ContractsTreeView.h"

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
    ContractsTreeView m_contactView;
};

#endif // ADDRESSBOOKWIDGET_H
