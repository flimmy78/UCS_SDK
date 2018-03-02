#include "ConversationListView.h"
#include <QMenu>
#include "Interface/UCSLogger.h"
#include "ConversationItemDelegate.h"

ConversationListView::ConversationListView(QWidget *parent)
    : QListView(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    setFocusPolicy(Qt::NoFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setCursor(Qt::ArrowCursor);
    setResizeMode(QListView::Adjust);
    setUniformItemSizes(true);

    m_pModel = new ConversationItemModel(this);
    this->setModel(m_pModel);

    ConversationItemDelegate *delegate = new ConversationItemDelegate(this);
    this->setItemDelegate(delegate);

    initConnections();
}

ConversationListView::~ConversationListView()
{

}

void ConversationListView::updateConversationList()
{
    m_pModel->refreshModelData();
}

void ConversationListView::initConnections()
{
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(onClickedItem(QModelIndex)));
}

void ConversationListView::contextMenuEvent(QContextMenuEvent *event)
{
    int row = this->indexAt(event->pos()).row();
    if (row >= 0)
    {
        m_eventPos = event->pos();
        ConversationItemType itemType = m_pModel->itemType(row);

        QMenu * pMenu = new QMenu(this);
        pMenu->setContentsMargins(0, 0, 0, 0);

        QAction *pDelAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("删除聊天"), pMenu);
        QAction *pTopAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("置顶"), pMenu);
        QAction *pModGroup = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("修改群名称"), pMenu);

        connect(pDelAct, SIGNAL(triggered()), this, SLOT(onDeleteItem()));
        connect(pTopAct, SIGNAL(triggered()), this, SLOT(onSetTopItem()));
        connect(pModGroup, SIGNAL(triggered()), this, SLOT(onModifyItem()));

        if (itemType != OtherTypeConversation)
        {
            pMenu->addAction(pTopAct);
        }
        if (itemType == GroupTypeConversation)
        {
            pMenu->addAction(pModGroup);
        }

        pMenu->addAction(pDelAct);

        pMenu->exec(QCursor::pos());
    }
}

void ConversationListView::resizeEvent(QResizeEvent *event)
{
//    int height = this->sizeHint().height();

//    int height = m_pModel->rowCount() * 60;
//    this->resize(this->width(), height);

    QWidget::resizeEvent(event);
}

void ConversationListView::onClickedItem(QModelIndex index)
{
    if (index.row())
    {
        UCS_LOG(UCSLogger::kTraceApiCall, "Conversation",
                QString("onClickedItem: %1").arg(index.data(Qt::UserRole).toString()));

        QString conversationId = m_pModel->conversationId(index.row());
        quint32 conversationType = m_pModel->conversationType(index.row());

        emit itemClicked(conversationId, conversationType);
    }
}

void ConversationListView::onDeleteItem()
{
    QModelIndex index = indexAt(m_eventPos);
    if (index.row() >= 0)
    {
        UCS_LOG(UCSLogger::kTraceInfo, "Conversation",
                QString("onDeleteItem: %1")
                .arg(index.data(Qt::UserRole).toString()));
        m_pModel->removeItem(index.row());
    }
}

void ConversationListView::onSetTopItem()
{
    QModelIndex index = indexAt(m_eventPos);
    if (index.row() >= 0)
    {
        UCS_LOG(UCSLogger::kTraceInfo, "Conversation",
                QString("onSetTopItem: %1")
                .arg(index.data(Qt::UserRole).toString()));
        m_pModel->topItem(index.row());
    }
}

void ConversationListView::onModifyItem()
{
    QModelIndex index = indexAt(m_eventPos);
    if (index.row() >= 0)
    {
        UCS_LOG(UCSLogger::kTraceInfo, "Conversation",
                QString("onModifyItem: %1")
                .arg(index.data(Qt::UserRole).toString()));
        m_pModel->topItem(index.row());
    }
}
