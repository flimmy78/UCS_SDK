#include "absFramelessAutoSize.h"
#include <QDebug>

AbsFrameLessAutoSize::AbsFrameLessAutoSize(QWidget *parent)
    : QWidget(parent)
    , m_border(4)
{
    m_state.MousePressed = false;
    m_state.IsPressBorder = false;

    setMinimumSize(550, 440);
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void AbsFrameLessAutoSize::setBorder(int border)
{
    m_border = border;
}

void AbsFrameLessAutoSize::mousePressEvent(QMouseEvent *event)
{
    m_state.PressedSize = this->size();
    m_state.IsPressBorder = false;

    if (event->button() == Qt::LeftButton)
    {
        // save the pressed position
        m_state.WindowPos = this->pos();

        if (QRect(m_border + 1,
                  m_border + 1,
                  width() - (m_border + 1) * 2,
                  height() - (m_border + 1) * 2)
                .contains(QPoint(event->globalX()- x(),
                                 event->globalY()- y())))
        {
            m_state.MousePos = event->globalPos();
            m_state.MousePressed = true;
        }
        else
        {
            m_state.IsPressBorder = true;
        }
    }
}

void AbsFrameLessAutoSize::mouseMoveEvent(QMouseEvent *event)
{
    mouseMoveRect(mapFromGlobal(event->globalPos()));
    if (m_state.MousePressed)
    {
        this->move(m_state.WindowPos + (event->globalPos() - m_state.MousePos));
    }
}

void AbsFrameLessAutoSize::mouseReleaseEvent(QMouseEvent *event)
{
    m_state.IsPressBorder = false;

    if (event->button() == Qt::LeftButton)
    {
        m_state.MousePressed = false;
    }
    setCursor(Qt::ArrowCursor);
}

void AbsFrameLessAutoSize::mouseMoveRect(const QPoint &p)
{
    /* just move cursor on the border, not pressed */
    if (!m_state.IsPressBorder)
    {
        if (p.x() < m_border &&
            p.y() < (this->height() - m_border) &&
            p.y() > m_border)
        {
            /* at left side */
            setCursor(Qt::SizeHorCursor);
            m_curPos = CursorPos::Left;
        }
        else if (p.y() < m_border &&
                 p.x() > m_border &&
                 p.x() < (this->width() - m_border))
        {
            /* at top side */
            setCursor(Qt::SizeVerCursor);
            m_curPos = CursorPos::Top;
        }
        else if (p.x() > (this->width() - m_border) &&
                 p.y() > m_border &&
                 p.y() < (this->height() - m_border))
        {
            /* at right side */
            setCursor(Qt::SizeHorCursor);
            m_curPos = CursorPos::Right;
        }
        else if (p.x() > m_border &&
                 p.x() < (this->width() - m_border) &&
                 p.y() > (this->height() - m_border))
        {
            /* at bottom side */
            setCursor(Qt::SizeVerCursor);
            m_curPos = CursorPos::Bottom;
        }
        else if (p.x() < m_border &&
                 p.y() < m_border)
        {
            /* at top left corner */
            setCursor(Qt::SizeFDiagCursor);
            m_curPos = CursorPos::TopLeft;
        }
        else if (p.x() > (this->width() - m_border) &&
                 p.y() < m_border)
        {
            /* at top right corner */
            setCursor(Qt::SizeBDiagCursor);
            m_curPos = CursorPos::TopRight;
        }
        else if(p.x() > (this->width() - m_border) &&
                p.y() > (this->height() - m_border))
        {
            /* at bottom right corner */
            setCursor(Qt::SizeFDiagCursor);
            m_curPos = CursorPos::BottomRight;
        }
        else if (p.x() < m_border &&
                 p.y() > (this->height() - m_border))
        {
            /* at bottom left corner */
            setCursor(Qt::SizeBDiagCursor);
            m_curPos = CursorPos::BottomLeft;
        }
        else
        {
            setCursor(Qt::ArrowCursor);
            m_curPos = CursorPos::Default;
        }
    }
    else
    {
        /* put the cursor on the border and
         *  pressed left button to resize window */
        switch (m_curPos) {
        case CursorPos::Left:
        {
            int setW = this->x() + this->width() - QCursor::pos().x();
            int setX = QCursor::pos().x();

            if (this->minimumWidth() <= setW &&
                setW <= this->maximumWidth())
            {
                setGeometry(setX, this->y(), setW, this->height());
            }
            break;
        }

        case CursorPos::Top:
        {
            int setH = this->y() + this->height() - QCursor::pos().y();

            if (this->minimumHeight() <= setH &&
                setH <= this->maximumHeight())
            {
                setGeometry(this->x(), QCursor::pos().y(), this->width(), setH);
            }
            break;
        }

        case CursorPos::Right:
        {
            int setW = QCursor::pos().x() - this->x();

            if (this->minimumWidth() <= setW &&
                setW <= this->maximumWidth())
            {
                setGeometry(this->x(), this->y(), setW, this->height());
            }
            break;
        }

        case CursorPos::Bottom:
        {
            int setH = QCursor::pos().y() - this->y();

            if (this->minimumHeight() <= setH &&
                setH <= this->maximumHeight())
            {
                setGeometry(this->x(), this->y(), this->width(), setH);
            }
            break;
        }

        case CursorPos::TopLeft:
        {
            int setX = QCursor::pos().x();
            int setY = QCursor::pos().y();
            int setW = this->pos().x() + this->width() - setX;
            int setH = this->pos().y() + this->height() - setY;
            int totalW = m_state.WindowPos.x() + m_state.PressedSize.width();
            int totalH = m_state.WindowPos.y() + m_state.PressedSize.height();

            if (totalW - setX >= this->maximumWidth())
            {
                setX = totalW - this->maximumWidth();
                setW = this->maximumWidth();
            }
            if (totalW - setX <= this->minimumWidth())
            {
                setX = totalW - this->minimumWidth();
                setW = this->minimumWidth();
            }
            if (totalH - setY >= this->maximumHeight())
            {
                setY = totalH - this->maximumHeight();
                setH = this->maximumHeight();
            }
            if (totalH - setY <= this->minimumHeight())
            {
                setY = totalH - this->minimumHeight();
                setH = this->minimumHeight();
            }
            setGeometry(setX, setY, setW, setH);
            break;
        }

        case CursorPos::TopRight:
        {
            int setY = QCursor::pos().y();
            int setW = QCursor::pos().x() - this->x();
            int setH = this->y() + height() - QCursor::pos().y();

            if (setH >= this->maximumHeight())
            {
                setY = m_state.WindowPos.y() + m_state.PressedSize.height() - this->height();
                setH = this->maximumHeight();
            }
            if (setH <= this->minimumHeight())
            {
                setY = m_state.WindowPos.y() + m_state.PressedSize.height() - this->height();
                setH = this->minimumHeight();
            }
            setGeometry(m_state.WindowPos.x(), setY, setW, setH);
            break;
        }

        case CursorPos::BottomLeft:
        {
            int setX = QCursor::pos().x();
            int setW = this->x() + this->width() - QCursor::pos().x();
            int setH = QCursor::pos().y() - m_state.WindowPos.y();
            int totalW = m_state.WindowPos.x() + m_state.PressedSize.width();

            if (totalW - setX >= this->maximumWidth())
            {
                setX = totalW - this->maximumWidth();
                setW = this->maximumWidth();
            }
            if (totalW - setX <= this->minimumWidth())
            {
                setX = totalW - this->minimumWidth();
                setW = this->minimumWidth();
            }
            setGeometry(setX, m_state.WindowPos.y(), setW, setH);
            break;
        }

        case CursorPos::BottomRight:
        {
            int setW = QCursor::pos().x() - this->x();
            int setH = QCursor::pos().y() - this->y();

            setGeometry(m_state.WindowPos.x(), m_state.WindowPos.y(), setW,  setH);
            break;
        }
        default:
            break;
        }
    }
}
