#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>

class MyScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    MyScrollArea(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *) { setFocus(); }
};

#endif // MYSCROLLAREA_H
