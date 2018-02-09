#include "midLeftContractsTreeView.h"
#include "common/util.h"
#include "midLeftContractItemDelegate.h"
#include "treeItem.h"

MidLeftContractsTreeView::MidLeftContractsTreeView(QWidget *parent)
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

    QString viewStyle = "QTreeView {\
        border: none; \
        background: rgb(242, 242, 242);\
    }\
    QTreeView::item {\
        height: 30px;\
        border: none;\
        background: transparent;\
        color: black;\
    }\
    QTreeView::item:hover {\
        background: rgb(216, 244, 240);\
    }\
    QTreeView::item:selected {\
        background: rgb(193, 243, 230);\
    }\
    QTreeView::branch:open:has-children {\
        image: url(:/images/arrowBottom.png);\
    }\
    QTreeView::branch:closed:has-children {\
        image: url(:images/arrowRight.png);\
    }";

    setStyleSheet(viewStyle);

    QString scrollStyle = "QScrollBar{background:transparent; width: 10px;margin: 0px 2px 0px 0px;}"
                          "QScrollBar::handle{background:rgb(217,217,217);border-radius:4px;}"
                          "QScrollBar::handle:hover{background: rgb(191,191,191);}"
                          "QScrollBar::add-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                          "QScrollBar::sub-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                          "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:transparent;}";
    verticalScrollBar()->setStyleSheet(scrollStyle);

    /* set icon size for items */
    setIconSize(QSize(20, 20));

    init();
    initConnection();
    initMenu();
}

void MidLeftContractsTreeView::contextMenuEvent(QContextMenuEvent *event)
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

void MidLeftContractsTreeView::init()
{
    m_pContactModel = new MidLeftContactTreeModel(this);
    this->setModel(m_pContactModel);
    m_pContactModel->importJSON(Util::exePath() + "/contacts.json");

    MidLeftContractItemDelegate *delegate = new MidLeftContractItemDelegate(this);
    this->setItemDelegate(delegate);
}

void MidLeftContractsTreeView::initConnection()
{
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_itemClicked(QModelIndex)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slot_doubleClicked(QModelIndex)));
    //    connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(slot_itemPressed(QModelIndex)));
}

void MidLeftContractsTreeView::initMenu()
{
    m_pGroupMenu = new QMenu(this);
    m_pPersonMenu = new QMenu(this);
    m_pGroupMenu->setFixedWidth(130);
    m_pPersonMenu->setFixedWidth(130);

    /* 分组右键菜单 */
    QAction *pAddGroup = new QAction(QStringLiteral("添加分组"));
    QAction *pDelGroup = new QAction(QStringLiteral("删除该组"));
    QAction *pAddPerson = new QAction(QStringLiteral("添加联系人"));

    connect(pAddGroup, SIGNAL(triggered(bool)), this, SLOT(slot_addGroup(bool)));
    connect(pAddPerson, SIGNAL(triggered(bool)), this, SLOT(slot_addPerson(bool)));
    connect(pDelGroup, SIGNAL(triggered(bool)), this, SLOT(slot_delGroup(bool)));

    m_pGroupMenu->addAction(pAddGroup);
    m_pGroupMenu->addAction(pAddPerson);
    m_pGroupMenu->addSeparator();
    m_pGroupMenu->addAction(pDelGroup);

    /* 联系人右键菜单 */
    QAction *pDelAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("删除该联系人"));
    QAction *pMsgAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("发消息"));
    QAction *pAudioAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("音频通话"));
    QAction *pVideoAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("视频通话"));

    connect(pDelAct, SIGNAL(triggered(bool)), this, SLOT(slot_removePerson(bool)));
    connect(pMsgAct, SIGNAL(triggered(bool)), this, SLOT(slot_sendMessage(bool)));
    connect(pAudioAct, SIGNAL(triggered(bool)), this, SLOT(slot_audioCall(bool)));
    connect(pVideoAct, SIGNAL(triggered(bool)), this, SLOT(slot_videoCall(bool)));

    m_pPersonMenu->addAction(pMsgAct);
    m_pPersonMenu->addAction(pAudioAct);
    m_pPersonMenu->addAction(pVideoAct);
    m_pPersonMenu->addSeparator();
    m_pPersonMenu->addAction(pDelAct);
}

void MidLeftContractsTreeView::slot_itemClicked(QModelIndex index)
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    qDebug() << "sectionid " << item->data(0) << " data " << index.data();
}

void MidLeftContractsTreeView::slot_doubleClicked(QModelIndex index)
{
    qDebug() << index.data().toString();
//    qDebug() << index.parent().data().toString();
}

void MidLeftContractsTreeView::slot_itemPressed(QModelIndex index)
{
//    qDebug() << "row " << index.row() << "column " << index.column() << " data " << index.data();
    qDebug() << index.data(Qt::DisplayRole).toString();
}

void MidLeftContractsTreeView::slot_addGroup(bool checked)
{
    Q_UNUSED(checked);
}

void MidLeftContractsTreeView::slot_delGroup(bool checked)
{
    Q_UNUSED(checked);
}

void MidLeftContractsTreeView::slot_addPerson(bool checked)
{
    Q_UNUSED(checked);
}

void MidLeftContractsTreeView::slot_removePerson(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "remove " << currentIndex().data().toString();
}

void MidLeftContractsTreeView::slot_sendMessage(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "msg " << currentIndex().data().toString();
}

void MidLeftContractsTreeView::slot_audioCall(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "audio " << currentIndex().data().toString();
}

void MidLeftContractsTreeView::slot_videoCall(bool checked)
{
    Q_UNUSED(checked);
    qDebug() << "video " << currentIndex().data().toString();
}
