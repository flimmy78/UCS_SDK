#ifndef IMWIDGET_H
#define IMWIDGET_H

#include <QWidget>
#include <qglobal.h>
#include "BaseWidget.h"

class IMWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit IMWidget(QWidget *parent = 0);

signals:

public slots:
};

#endif // IMWIDGET_H
