#ifndef MIDDLESEARCHSTACKWIDGET_H
#define MIDDLESEARCHSTACKWIDGET_H

#include <QObject>
#include <QWidget>
#include "absFiles/myScrollArea.h"

class MidLeftSearchStackWidget : public MyScrollArea
{
    Q_OBJECT

public:
    explicit MidLeftSearchStackWidget(QWidget *parent = 0);

private:
    void initLayout();
    void initConnection();

signals:

public slots:

private:

};

#endif // MIDDLESEARCHSTACKWIDGET_H
