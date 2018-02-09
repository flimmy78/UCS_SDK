#ifndef CALLHISTORYWIDGET_H
#define CALLHISTORYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QListView>
#include "absFiles/myScrollArea.h"
#include "listWidgets/historycalllistview.h"
#include "listWidgets/midLeftCallListWidget.h"

class MidLeftCallListStackWidget : public MyScrollArea
{
    Q_OBJECT

public:
    explicit MidLeftCallListStackWidget(QWidget *parent);
    ~MidLeftCallListStackWidget() {}

    void updateData();
private:
    void initLayout();
    void initConnection();

signals:

public slots:

public:
    MidLeftCallListWidget m_callListWid;
};

#endif // CALLHISTORYWIDGET_H
