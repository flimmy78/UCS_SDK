#include "conferenceStackWidget.h"
#include "myPushButton.h"

ConferenceStackWidget::ConferenceStackWidget(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    initLayout();
}

void ConferenceStackWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void ConferenceStackWidget::initLayout()
{
    QBoxLayout *pMainLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    QHBoxLayout *pTopLayout = new QHBoxLayout;
    QHBoxLayout *pBottomLayout = new QHBoxLayout;

    m_pLblIcon = new QLabel(this);
    QPixmap pix = QPixmap(":/images/midright/u426.png");
    m_pLblIcon->setPixmap(pix.scaled(pix.width() * 0.6, pix.height() * 0.6, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    m_pStartConfBtn = new MyPushButton(QStringLiteral("发起会议"), this);
    m_pBookConfBtn = new MyPushButton(QStringLiteral("预约会议"), this);
    m_pStartConfBtn->setStyleSheet("QPushButton{background-image: url(:/images/midright/u398.png); color: white; border:none; font: 12pt;}");
    m_pBookConfBtn->setStyleSheet("QPushButton{background-image: url(:/images/midright/u398.png); color: white; border:none; font: 12pt;}");

    m_pStartConfBtn->setFixedSize(143, 43);
    m_pBookConfBtn->setFixedSize(143, 43);

    pBottomLayout->addStretch(60);
    pBottomLayout->addWidget(m_pStartConfBtn, 0, Qt::AlignHCenter);
    pBottomLayout->addStretch(10);
    pBottomLayout->addWidget(m_pBookConfBtn, 0, Qt::AlignHCenter);
    pBottomLayout->addStretch(60);

    pTopLayout->addWidget(m_pLblIcon, 0, Qt::AlignCenter);

    pMainLayout->setDirection(QBoxLayout::TopToBottom);
    pMainLayout->addLayout(pTopLayout);
    pMainLayout->addLayout(pBottomLayout);

    setLayout(pMainLayout);
}


