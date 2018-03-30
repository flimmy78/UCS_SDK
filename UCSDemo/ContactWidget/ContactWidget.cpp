#include "ContactWidget.h"
#include "EmptyWidget.h"
#include "UCSLogger.h"

ContactWidget::ContactWidget(QWidget *parent)
    : BaseWidget(parent)
    , m_pSearchWatcher(Q_NULLPTR)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setObjectName("ContactWidget");

    init();

    initLayout();
    initConnections();
}

ContactWidget::~ContactWidget()
{
    if (m_pSearchWatcher)
    {
        m_pSearchWatcher->cancel();
        m_pSearchWatcher->waitForFinished();
    }
}

void ContactWidget::initLayout()
{
    m_pTitleBar = new TopBarWidget(this);
    m_pSearchEdit = new SearchLineEdit(this);
    m_pContactTreeView = new ContactsTreeView(this);
    m_pContactInfoWidget = new ContactInfoWidget(this);
    m_pSearchListView = new ContactSearchListView(this);

    m_pSearchListView->setVisible(false);

    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    QVBoxLayout *pLeftLayout = new QVBoxLayout;
    QVBoxLayout *pRightLayout = new QVBoxLayout;
    QHBoxLayout *pSearchLayout = new QHBoxLayout;
    m_pStackedLayout = new QStackedLayout;

    pSearchLayout->addWidget(m_pSearchEdit);
    pSearchLayout->setContentsMargins(15, 20, 15, 10);

    pLeftLayout->addLayout(pSearchLayout, 0);
    pLeftLayout->addWidget(m_pContactTreeView);
    pLeftLayout->addWidget(m_pSearchListView);
    pLeftLayout->setSpacing(0);
    pLeftLayout->setContentsMargins(0, 0, 0, 0);

    EmptyWidget *emptyWidget = new EmptyWidget(this);
    m_pStackedLayout->addWidget(emptyWidget);
    m_pStackedLayout->addWidget(m_pContactInfoWidget);
    m_pStackedLayout->setSpacing(0);
    m_pStackedLayout->setContentsMargins(0, 0, 0, 0);

    pRightLayout->addWidget(m_pTitleBar);
    pRightLayout->addLayout(m_pStackedLayout);
    pRightLayout->setSpacing(0);
    pRightLayout->setContentsMargins(0, 0, 0, 0);

    pMainLayout->addLayout(pLeftLayout);
    pMainLayout->addLayout(pRightLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
}

void ContactWidget::initConnections()
{
//    connect(m_pContactTreeView, SIGNAL(sigItemClicked(ContactItem)),
//            m_pContactInfoWidget, SLOT(onContactItemClicked(ContactItem)));
    connect(m_pContactTreeView, SIGNAL(sigItemClicked(ContactItem)),
            this, SLOT(onContactViewClicked(ContactItem)));
    connect(m_pContactTreeView, SIGNAL(sigItemClicked(ContactItem)),
            m_pContactInfoWidget->contactCard(), SLOT(onShowContact(ContactItem)));

    connect(m_pSearchListView, SIGNAL(sigItemClicked(ContactItem)),
            m_pContactInfoWidget->contactCard(), SLOT(onShowContact(ContactItem)));

//    connect(m_pSearchListView, SIGNAL(sigItemClicked(ContactItem)),
//            m_pContactInfoWidget, SLOT(onContactItemClicked(ContactItem)));
    connect(m_pSearchListView, SIGNAL(sigItemClicked(ContactItem)),
            this, SLOT(onContactViewClicked(ContactItem)));

    connect(m_pSearchEdit, SIGNAL(returnPressed()), this, SLOT(onSearchContact()));
    connect(m_pSearchEdit, SIGNAL(sigFocusChanged(bool)), this, SLOT(onSearchFocusChanged(bool)));

    connect(m_pSearchWatcher, SIGNAL(finished()), this, SLOT(onSearchFinished()));
}

void ContactWidget::onContactViewClicked(ContactItem contact)
{
    Q_UNUSED(contact);

    m_pStackedLayout->setCurrentIndex(1);
}

void ContactWidget::onSearchContact()
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(sender());
    QString strSearch = lineEdit->text().trimmed();

    m_pSearchWatcher->setFuture(QtConcurrent::run(ContactWidget::searchContact,
                                                  this,
                                                  strSearch));
}

void ContactWidget::onSearchFocusChanged(bool isFocusIn)
{
    if (isFocusIn)
    {
        m_pContactTreeView->setVisible(false);
        m_pSearchListView->setVisible(true);

        ///< clear history search result >
        m_pSearchListView->clearSearchResult();
    }
    else
    {
        m_pSearchListView->setVisible(false);
        m_pContactTreeView->setVisible(true);
    }
}

void ContactWidget::onSearchFinished()
{
    m_pSearchListView->setSearchResult(m_pSearchWatcher->result());
}

ContactInfoWidget *ContactWidget::contactInfoWidget() const
{
    return m_pContactInfoWidget;
}

void ContactWidget::updateLogin()
{
    m_pTitleBar->updateTopBar();
}

ContactList ContactWidget::searchContact(ContactWidget *pObj, QString strSearch)
{
    UCS_LOG(UCSLogger::kTraceApiCall, pObj->objectName(),
            QString("searchContact: strSearch = %1").arg(strSearch));

    ContactWidget *pThis = pObj;
    ContactList contactList = pThis->contactListView()->contactList();
    ContactList searchList;
    if (strSearch.isEmpty())
    {
        return searchList;
    }

    foreach (ContactItem contact, contactList)
    {
        if (contact.userId.isEmpty())
        {
            continue;
        }
        if (contact.toString().contains(strSearch))
        {
            searchList.append(contact);
        }
    }

    return searchList;
}

void ContactWidget::init()
{
    m_pSearchWatcher = new QFutureWatcher<ContactList>(this);
}

ContactsTreeView *ContactWidget::contactListView() const
{
    return m_pContactTreeView;
}
