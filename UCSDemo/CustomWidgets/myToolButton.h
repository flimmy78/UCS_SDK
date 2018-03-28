#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H
#include <QToolButton>
#include <QWidget>

class MyToolButton : public QToolButton
{
    Q_OBJECT

public:
    MyToolButton(QWidget *parent = 0);
    MyToolButton(const QString &text, QWidget *parent = 0);

protected:
    QSize sizeHint() const override;
};

#endif // MYTOOLBUTTON_H
