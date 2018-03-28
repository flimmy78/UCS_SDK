#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include <absFramelessAutoSize.h>

class BaseWindow : public AbsFrameLessAutoSize
{
    Q_OBJECT

public:
    explicit BaseWindow(QWidget* parent = 0);

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
};

#endif // BASEWINDOW_H
