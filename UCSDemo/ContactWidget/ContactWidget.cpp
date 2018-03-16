#include "ContactWidget.h"

ContactWidget::ContactWidget(QWidget *parent)
    : BaseWidget(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setObjectName("ContactWidget");

    initLayout();
    initConnections();
}

void ContactWidget::initLayout()
{
    m_pTitleBar = new TopWidget(this);
    m_pSearchEdit = new SearchLineEdit(this);
    m_pContactListView = new ContactsTreeView(this);
    m_pContactInfoWidget = new ContactInfoWidget(this);

    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    QVBoxLayout *pLeftLayout = new QVBoxLayout;
    QVBoxLayout *pRightLayout = new QVBoxLayout;
    QHBoxLayout *pSearchLayout = new QHBoxLayout;

    pSearchLayout->addWidget(m_pSearchEdit);
    pSearchLayout->setContentsMargins(15, 20, 15, 10);

    pLeftLayout->addLayout(pSearchLayout);
    pLeftLayout->addWidget(m_pContactListView);
    pLeftLayout->setSpacing(0);
    pLeftLayout->setContentsMargins(0, 0, 0, 0);

    pRightLayout->addWidget(m_pTitleBar);
    pRightLayout->addWidget(m_pContactInfoWidget);
    pRightLayout->setSpacing(0);
    pRightLayout->setContentsMargins(0, 0, 0, 0);

    pMainLayout->addLayout(pLeftLayout);
    pMainLayout->addLayout(pRightLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
}

void ContactWidget::initConnections()
{

}
