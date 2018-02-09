#include "middleRightWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

MiddleRightWidget::MiddleRightWidget(QWidget *parent, int width)
    : QWidget(parent)
    , m_stackWid(this)
    , m_dialSWid(this)
    , m_contactSWid(this)
    , m_chatSWid(this)
{
//    QPalette palette;
//    palette.setColor(QPalette::Background, QColor(242, 242, 242));
//    setPalette(palette);
//    setAutoFillBackground(true);

    setMinimumWidth(width);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    initLayout();
    initConnection();
}

void MiddleRightWidget::initLayout()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    QHBoxLayout *pTopLayout = new QHBoxLayout;
    QHBoxLayout *pMidLayout = new QHBoxLayout;

    pTopLayout->addWidget(&m_topWid);

    m_stackWid.setFrameShadow(QFrame::Plain);
    m_stackWid.setFrameShape(QFrame::NoFrame);
    m_stackWid.addWidget(&m_dialSWid);
    m_stackWid.addWidget(&m_contactSWid);
    m_stackWid.addWidget(&m_chatSWid);
    pMidLayout->addWidget(&m_stackWid);
    pMidLayout->setSpacing(0);

    pMainLayout->addLayout(pTopLayout);
    pMainLayout->addLayout(pMidLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void MiddleRightWidget::initConnection()
{
    m_stackWid.setCurrentIndex(0);
}

void MiddleRightWidget::slot_switchStack()
{
    StackButton *stackBtn = (StackButton*)sender();
    int index = stackBtn->index();
    if (index != 2)
    {
        m_topWid.setTitle("");
    }
    m_stackWid.setCurrentIndex(index);
}

void MiddleRightWidget::slot_setTitle(QString title)
{
    m_topWid.setTitle(title);
}

