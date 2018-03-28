#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton(QWidget* parent = 0);
    MyPushButton(const QString& str, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) override;
};

/*
 * stack button, add index for stackwidget
*/
class StackButton : public QPushButton
{
    Q_OBJECT
public:
    explicit StackButton(const int index,
                         const QString& pixnormal,
                         const QString& pixhover,
                         const QString& pixsel,
                         QWidget* parent);

    void setSelected(bool selected = true);
    int index() { return m_index; }

protected:
    void mousePressEvent(QMouseEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void paintEvent(QPaintEvent* event);

private:
    QPixmap m_pixnormal;
    QPixmap m_pixhover;
    QPixmap m_pixselected;

    int m_index;
    bool m_enter;
    bool m_pressed;
};

class PwdShowButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PwdShowButton(QWidget* parent = Q_NULLPTR);
    void setShow(bool isShow);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_enter;
    bool m_isShow;
};
#endif // MYPUSHBUTTON_H
