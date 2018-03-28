#include "myPushButton.h"
#include <QDebug>
#include <QPainter>
#include <QStyleOption>

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

void MyPushButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    /// <If you subclass from QWidget, you need to provide a
    ///  paintEvent for your custom QWidget as below for style sheet used>
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
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

        setStyleSheet("QPushButton{background-color: rgb(45, 177, 255); border:none;}");
    }
    else
    {
        // normal state
        if (!m_enter)
        {
            painter.drawPixmap((width() - m_pixnormal.width()) / 2,
                               (height() - m_pixnormal.height()) / 2,
                               m_pixnormal);

            setStyleSheet("QPushButton{background-color: rgb(0, 160, 255); border:none;}");
        }
        else
        {
            // enter state
            painter.drawPixmap((width() - m_pixhover.width()) / 2,
                               (height() - m_pixhover.height()) / 2,
                               m_pixhover);

            setStyleSheet("QPushButton{background-color: rgb(45, 177, 255); border:none;}");
        }
    }
}

PwdShowButton::PwdShowButton(QWidget *parent)
    : QPushButton(parent)
    , m_enter(false)
    , m_isShow(false)
{
    setCursor(Qt::PointingHandCursor);
    setFlat(true);
    setFixedSize(20, 20);
}

void PwdShowButton::setShow(bool isShow)
{
    m_isShow = isShow;
    update();
}

void PwdShowButton::enterEvent(QEvent *event)
{
    QPushButton::enterEvent(event);

    m_enter = true;
    update();
}

void PwdShowButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);

    m_enter = false;
    update();
}

void PwdShowButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

//    QPainter painter(this);

    // normal state
    if (!m_enter)
    {
//        painter.drawPixmap(0, 0, m_pixnormal);

        if (m_isShow)
        {
            setStyleSheet("QPushButton{border-image: url(:/Resources/pwd_show_normal.png); border:none;}");
        }
        else
        {
            setStyleSheet("QPushButton{border-image: url(:/Resources/pwd_hide_normal.png); border:none;}");
        }
    }
    else
    {
        // enter state
        if (m_isShow)
        {
            setStyleSheet("QPushButton{border-image: url(:/Resources/pwd_show_hover.png); border:none;}");
        }
        else
        {
            setStyleSheet("QPushButton{border-image: url(:/Resources/pwd_hide_hover.png); border:none;}");
        }
    }
}
