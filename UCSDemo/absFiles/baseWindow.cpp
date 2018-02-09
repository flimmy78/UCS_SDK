#include "baseWindow.h"

#include <QPainter>
#include<qmath.h>

BaseWindow::BaseWindow(QWidget *parent)
    :AbsFrameLessAutoSize(parent)
{
    m_border = 8;
    setMinimumSize(550, 440);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void BaseWindow::paintEvent(QPaintEvent *event)
{
    AbsFrameLessAutoSize::paintEvent(event);

    QPainter painter(this);

    // draw background
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(4, 4, this->width() - 8, this->height() - 8);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    /* draw shadow margin */
    QColor color(0, 0, 0, 50);
    for (int i = 0; i < 4; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(4 - i, 4 - i, this->width() - (4 - i) * 2, this->height() - (4 - i) * 2);
        color.setAlpha(100 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}
