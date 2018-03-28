#include "ToggleButton.h"

#include <QPainter>

ToggleButton::ToggleButton(QWidget *parent)
    : QWidget(parent)
    , m_bChecked(false)
    , m_background(Qt::black)
    , m_checkedColor(0, 150, 136)
    , m_disabledColor(190, 190, 190)
    , m_thumbColor(Qt::white)
    , m_radius(8.0)
    , m_nHeight(16)
    , m_nMargin(3)
{
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void ToggleButton::setBackGroundColor(QColor color)
{
    m_background = color;
}

void ToggleButton::setCheckedColor(QColor color)
{
    m_checkedColor = color;
}

void ToggleButton::setDisabledColor(QColor color)
{
    m_disabledColor = color;
}

bool ToggleButton::bChecked() const
{
    return m_bChecked;
}

void ToggleButton::setBChecked(bool bChecked)
{
    m_bChecked = bChecked;
    m_timer.start(10);
}

void ToggleButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    QColor background;
    QColor thumbColor;
    qreal dOpacity;
    if (isEnabled()) ///< 可用状态 >
    {
        if (m_bChecked)
        {
            background = m_checkedColor;
            thumbColor = m_checkedColor;
            dOpacity = 0.600;
        }
        else
        {
            background = m_background;
            thumbColor = m_thumbColor;
            dOpacity = 0.800;
        }
    }
    else
    {
        background = m_background;
        dOpacity = 0.260;
        thumbColor = m_disabledColor;
    }

    ///< 绘制大椭圆 >
    painter.setBrush(background);
    painter.setOpacity(dOpacity);
    path.addRoundedRect(QRectF(m_nMargin, m_nMargin,
                               this->width() - 2 * m_nMargin,
                               this->height() - 2 * m_nMargin),
                        m_radius, m_radius);
    painter.drawPath(path.simplified());

    /// < 绘制小椭圆 >
    painter.setBrush(thumbColor);
    painter.setOpacity(1.0);
    painter.drawEllipse(QRectF(m_nX - (m_nHeight / 2),
                               m_nY - (m_nHeight / 2),
                               this->height(),
                               this->height()));
}

void ToggleButton::mousePressEvent(QMouseEvent *event)
{
    if (isEnabled())
    {
        if (event->buttons() & Qt::LeftButton)
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
}

void ToggleButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (isEnabled())
    {
        if ((event->type() == QMouseEvent::MouseButtonRelease)
            && (event->button() == Qt::LeftButton))
        {
            event->accept();
            m_bChecked = !m_bChecked;
            emit sigCheckedChanged(m_bChecked);
            m_timer.start(10);
        }
        else
        {
            event->ignore();
        }
    }
}

void ToggleButton::resizeEvent(QResizeEvent *event)
{
    m_nX = m_nHeight / 2;
    m_nY = m_nHeight / 2;
    QWidget::resizeEvent(event);
}

QSize ToggleButton::sizeHint() const
{
    return minimumSizeHint();
}

QSize ToggleButton::minimumSizeHint() const
{
    return QSize(2 * (m_nHeight + m_nMargin), m_nHeight + 2 * m_nMargin);
}

void ToggleButton::onTimeout()
{
    if (m_bChecked)
    {
        m_nX += 1;
        if (m_nX >= this->width() - m_nHeight)
        {
            m_timer.stop();
        }
    }
    else
    {
        m_nX -= 1;
        if (m_nX <= m_nHeight / 2)
        {
            m_timer.stop();
        }
    }
    update();
}
