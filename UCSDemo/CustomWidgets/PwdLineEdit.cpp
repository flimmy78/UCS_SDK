#include "PwdLineEdit.h"

#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>

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

PwdLineEdit::PwdLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_bPwdShowing(false)
{
    setEchoMode(QLineEdit::Password);
    initLayout();
    initConnections();
}

//void PwdLineEdit::paintEvent(QPaintEvent *event)
//{
////    Q_UNUSED(event);

//    /// <If you subclass from QWidget, you need to provide a
//    ///  paintEvent for your custom QWidget as below for style sheet used>
//    QStyleOption opt;
//    opt.init(this);
//    QPainter painter(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

//    QWidget::paintEvent(event);
//}

void PwdLineEdit::initLayout()
{
    QHBoxLayout *pLineLayout = new QHBoxLayout(this);
    m_pBtnShow = new PwdShowButton(this);

    QMargins margins = this->textMargins();
    setTextMargins(margins.left() + 5,
                   margins.top(),
                   m_pBtnShow->width() + 5,
                   margins.bottom());

    pLineLayout->addStretch();
    pLineLayout->addWidget(m_pBtnShow, 0, Qt::AlignVCenter);
    pLineLayout->setSpacing(0);
    pLineLayout->setContentsMargins(0, 0, 5, 0);
}

void PwdLineEdit::initConnections()
{
    connect(m_pBtnShow, SIGNAL(clicked(bool)), this, SLOT(onBtnPwdShowClicked(bool)));
}

void PwdLineEdit::onBtnPwdShowClicked(bool)
{
    m_bPwdShowing = !m_bPwdShowing;
    m_pBtnShow->setShow(m_bPwdShowing);
    if (m_bPwdShowing)
    {
        this->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        this->setEchoMode(QLineEdit::Password);
    }
}
