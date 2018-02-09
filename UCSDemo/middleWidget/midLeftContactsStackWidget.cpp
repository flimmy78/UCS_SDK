#include "midLeftContactsStackWidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include "absFiles/myLabel.h"

MidLeftContactsStackWidget::MidLeftContactsStackWidget(QWidget *parent)
    : MyScrollArea(parent)
    , m_contactView(this)
{
    setMouseTracking(true);
    setFixedWidth(250);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    initLayout();
    initConnections();
}

void MidLeftContactsStackWidget::initLayout()
{
    qDebug() << "MidLeftContactsStackWidget";
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    QHBoxLayout *pTopLayout = new QHBoxLayout;
    QHBoxLayout *pBottomLayout = new QHBoxLayout;

    QLabel *pComIcon = new QLabel(this);
    QPixmap comPix = QPixmap(":/images/u3743.png");
    pComIcon->setFixedSize(comPix.size());
    pComIcon->setPixmap(comPix);

    MyLabel *pComTitle = new MyLabel(QStringLiteral("河北省电力有限公司"), this);
    pComTitle->setFont(QFont("微软雅黑", 11, QFont::Normal, false));

    QFrame* pHLine = new QFrame();
    pHLine->setFrameShape(QFrame::HLine);
    pHLine->setFrameShadow(QFrame::Plain);
    pHLine->setFixedHeight(1);
    pHLine->setMinimumWidth(this->width());
    pHLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pHLine->setStyleSheet("QFrame{background-color: gray; border:none}");

    pTopLayout->addWidget(pComIcon);
    pTopLayout->setSpacing(16);
    pTopLayout->addWidget(pComTitle);
    pTopLayout->setContentsMargins(16, 8, 8, 8);

    pBottomLayout->addWidget(&m_contactView);
    pBottomLayout->setSpacing(0);
    pBottomLayout->setContentsMargins(0, 0, 0, 0);

    pMainLayout->addLayout(pTopLayout);
    pMainLayout->addWidget(pHLine);
    pMainLayout->addLayout(pBottomLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void MidLeftContactsStackWidget::initConnections()
{

}
