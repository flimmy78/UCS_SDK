#include "ContactSearchListView.h"
#include "ContactSearchListItemDelegate.h"
#include "UCSLogger.h"

ContactSearchListView::ContactSearchListView(QWidget *parent)
    : QListView(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setCursor(Qt::ArrowCursor);
    setResizeMode(QListView::Adjust);
    setUniformItemSizes(true);

    setObjectName("SearchListView");

    m_pListModel = new ContactSearchListModel(this);
    this->setModel(m_pListModel);
    m_pListModel->setModelData(&m_contactList);

    ContactSearchListItemDelegate *delegate = new ContactSearchListItemDelegate(this);
    this->setItemDelegate(delegate);

    initConnections();
    initData();
}

ContactSearchListView::~ContactSearchListView()
{
    if (m_pListModel)
    {
        delete m_pListModel;
        m_pListModel = Q_NULLPTR;
    }
}

void ContactSearchListView::setSearchResult(const ContactList &contactList)
{
    m_contactList = contactList;
    m_pListModel->refreshModel();
}

void ContactSearchListView::clearSearchResult()
{
    initData();
}

void ContactSearchListView::initConnections()
{
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
}

void ContactSearchListView::initData()
{
    ContactItem contact;
    contact.userName = QStringLiteral("搜索通讯录");
    contact.parentName = QStringLiteral("查找本地通讯录");
    contact.headPath = ":/Resources/app_icon.png";

    m_contactList.clear();
    m_contactList.append(contact);
    m_pListModel->refreshModel();
}

void ContactSearchListView::onItemClicked(QModelIndex index)
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onItemClicked: userName(%1)").arg(index.data(userNameRole).toString()));

    ContactItem contact = m_contactList.at(index.row());
    if (contact.userId.size())
    {
        emit sigItemClicked(contact);
    }
}
