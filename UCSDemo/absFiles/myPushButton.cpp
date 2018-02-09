#include "myPushButton.h"
#include <QDebug>
#include <QPainter>

MyPushButton::MyPushButton(QWidget *parent)
    :QPushButton(parent)
{
    setCursor(Qt::PointingHandCursor);
    setFlat(true);
//    setStyleSheet("QPushButton{background:transparent;border:none;}");
}

MyPushButton::MyPushButton(const QString &str, QWidget *parent)
    :QPushButton(str, parent)
{
    setCursor(Qt::PointingHandCursor);
    setFlat(true);
//    setStyleSheet("QPushButton{background:transparent;border:none;}");
}

StackButton::StackButton(const int index,
                         const QString &pixnormal,
                         const QString &pixhover,
                         const QString &pixsel,
                         QWidget *parent)
    :QPushButton(parent)
    , m_pixnormal(pixnormal)
    , m_pixhover(pixhover)
    , m_pixselected(pixsel)
    , m_index(index)
    , m_enter(false)
    , m_pressed(false)
{
    setCursor(Qt::PointingHandCursor);    
    setFlat(true);
}

void StackButton::setSelected(bool selected)
{
    m_pressed = selected;
    update();
}

void StackButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);

    if (event->button() == Qt::LeftButton)
    {
        m_pressed = true;
        update();
    }
}

void StackButton::enterEvent(QEvent *event)
{
    QPushButton::enterEvent(event);

    m_enter = true;
    update();
}

void StackButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);

    m_enter = false;
    update();
}

void StackButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);

    // pressed state
    if (m_pressed)
    {
        painter.drawPixmap((width() - m_pixselected.width()) / 2,
                           (height() - m_pixselected.height()) / 2,
                           m_pixselected);

        setStyleSheet("QPushButton{background-color: rgb(37, 174, 154); border:none;}");
    }
    else
    {
        // normal state
        if (!m_enter)
        {
            painter.drawPixmap((width() - m_pixnormal.width()) / 2,
                               (height() - m_pixnormal.height()) / 2,
                               m_pixnormal);

            setStyleSheet("QPushButton{background-color: rgb(32, 155, 152); border:none;}");
        }
        else
        {
            // enter state
            painter.drawPixmap((width() - m_pixhover.width()) / 2,
                               (height() - m_pixhover.height()) / 2,
                               m_pixhover);

            setStyleSheet("QPushButton{background-color: rgb(37, 174, 154); border:none;}");
        }
    }
}
