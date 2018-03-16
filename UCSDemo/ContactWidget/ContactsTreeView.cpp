#include "ContactsTreeView.h"
#include "common/util.h"
#include "ContractTreeItemDelegate.h"
#include "treeItem.h"

ContactsTreeView::ContactsTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setMouseTracking(true);
    setExpandsOnDoubleClick(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setHeaderHidden(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    /* 去除item选中时的虚线边框 */
    setFocusPolicy(Qt::NoFocus);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setCursor(Qt::ArrowCursor);
    setObjectName("ContractsTreeView");

    /* set icon size for items */
    setIconSize(QSize(25, 25));

    init();
    initConnection();
    initMenu();
    loadStyleSheet();
}

ContactsTreeView::~ContactsTreeView()
{
    if (m_pRestApi)
    {
        delete m_pRestApi;
        m_pRestApi = Q_NULLPTR;
    }
}

void ContactsTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

    QModelIndex index = currentIndex();

    bool bGroup = index.data(GroupRole).toBool();
    if (bGroup)
    {
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if (item->data(0) == "pri")
        {
            m_pGroupMenu->exec(QCursor::pos());
        }
    }
    else
    {
        m_pPersonMenu->exec(QCursor::pos());
    }
}

void ContactsTreeView::init()
{
    m_pRestApi = new UPlusRestApi;

    m_pContactModel = new ContactTreeItemModel(this);
    this->setModel(m_pContactModel);
    m_pContactModel->importJSON(Util::appDir() + "/contacts.json");

    ContractTreeItemDelegate *delegate = new ContractTreeItemDelegate(this);
    this->setItemDelegate(delegate);
}

void ContactsTreeView::initConnection()
{
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onItemDoubleClicked(QModelIndex)));
    //    connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(slot_itemPressed(QModelIndex)));
}

void ContactsTreeView::initMenu()
{
    m_pGroupMenu = new QMenu(this);
    m_pPersonMenu = new QMenu(this);
    m_pGroupMenu->setFixedWidth(130);
    m_pPersonMenu->setFixedWidth(130);

    /* 分组右键菜单 */
    QAction *pAddGroup = new QAction(QStringLiteral("添加分组"));
    QAction *pDelGroup = new QAction(QStringLiteral("删除该组"));
    QAction *pAddPerson = new QAction(QStringLiteral("添加联系人"));

    connect(pAddGroup, SIGNAL(triggered(bool)), this, SLOT(onAddGroupAction(bool)));
    connect(pAddPerson, SIGNAL(triggered(bool)), this, SLOT(onAddContactAction(bool)));
    connect(pDelGroup, SIGNAL(triggered(bool)), this, SLOT(onDeleteGroupAction(bool)));

    m_pGroupMenu->addAction(pAddGroup);
    m_pGroupMenu->addAction(pAddPerson);
    m_pGroupMenu->addSeparator();
    m_pGroupMenu->addAction(pDelGroup);

    /* 联系人右键菜单 */
    QAction *pDelAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("删除该联系人"));
    QAction *pMsgAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("发消息"));
    QAction *pAudioAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("音频通话"));
    QAction *pVideoAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("视频通话"));

    connect(pDelAct, SIGNAL(triggered(bool)), this, SLOT(onRemoveContactAction(bool)));
    connect(pMsgAct, SIGNAL(triggered(bool)), this, SLOT(onSendMessageAction(bool)));
    connect(pAudioAct, SIGNAL(triggered(bool)), this, SLOT(onAudioCallAction(bool)));
    connect(pVideoAct, SIGNAL(triggered(bool)), this, SLOT(onVideoCallAction(bool)));

    m_pPersonMenu->addAction(pMsgAct);
    m_pPersonMenu->addAction(pAudioAct);
    m_pPersonMenu->addAction(pVideoAct);
    m_pPersonMenu->addSeparator();
    m_pPersonMenu->addAction(pDelAct);
}

void ContactsTreeView::loadStyleSheet()
{
    QFile file(":/Resources/TreeView/TreeView.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QString(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
}

void ContactsTreeView::onItemClicked(QModelIndex index)
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    qDebug() << "sectionid " << item->data(0) << " data " << index.data();
}

void ContactsTreeView::onItemDoubleClicked(QModelIndex index)
{
    qDebug() << index.data().toString();
//    qDebug() << index.parent().data().toString();
}

void ContactsTreeView::onItemPressed(QModelIndex index)
{
//    qDebug() << "row " << index.row() << "column " << index.column() << " data " << index.data();
    qDebug() << index.data(Qt::DisplayRole).toString();
}

void ContactsTreeView::onAddGroupAction(bool checked)
{
    Q_UNUSED(checked);
}

void ContactsTreeView::onDeleteGroupAction(bool checked)
{
    Q_UNUSED(checked);
}

void ContactsTreeView::onAddContactAction(bool checked)
{
    Q_UNUSED(checked);
}

void ContactsTreeView::onRemoveContactAction(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "remove " << currentIndex().data().toString();
}

void ContactsTreeView::onSendMessageAction(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "msg " << currentIndex().data().toString();
}

void ContactsTreeView::onAudioCallAction(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "audio " << currentIndex().data().toString();
}

void ContactsTreeView::onVideoCallAction(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "video " << currentIndex().data().toString();
}

void ContactsTreeView::onUpdateContactsReply(QByteArray data, int code)
{

}
