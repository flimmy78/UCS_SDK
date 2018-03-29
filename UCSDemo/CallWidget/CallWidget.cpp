#include "CallWidget.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QVBoxLayout>

CallWidget::CallWidget(QWidget *parent)
    : BaseWidget(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setObjectName("CallWidget");

    initLayout();
    initConnections();
}

void CallWidget::updateLogin()
{
    m_pTitleBar->updateTopBar();
}

void CallWidget::initLayout()
{
    m_pTitleBar = new TopBarWidget(this);
    m_pSearchEdit = new SearchLineEdit(this);
    m_pDialWidget = new DialWidget(this);
    m_pCallListView = new CallListView(this);

    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    QVBoxLayout *pLeftLayout = new QVBoxLayout;
    QVBoxLayout *pRightLayout = new QVBoxLayout;
    QHBoxLayout *pSearchLayout = new QHBoxLayout;

    pSearchLayout->addWidget(m_pSearchEdit);
    pSearchLayout->setContentsMargins(15, 20, 15, 10);
    pLeftLayout->addLayout(pSearchLayout);

    pLeftLayout->addWidget(m_pCallListView);
    pLeftLayout->setSpacing(0);
    pLeftLayout->setContentsMargins(0, 0, 0, 0);

    pRightLayout->addWidget(m_pTitleBar);
    pRightLayout->addWidget(m_pDialWidget);
    pRightLayout->setSpacing(0);
    pRightLayout->setContentsMargins(0, 0, 0, 0);

    pMainLayout->addLayout(pLeftLayout);
    pMainLayout->addLayout(pRightLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
}

void CallWidget::initConnections()
{

}

