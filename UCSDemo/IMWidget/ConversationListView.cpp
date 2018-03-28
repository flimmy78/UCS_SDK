#include "ConversationListView.h"
#include <QDateTime>
#include <QMenu>
#include "Interface/UCSLogger.h"
#include "ConversationItemDelegate.h"
#include "Interface/UCSIMSDKPublic.h"
#include "CommonHelper.h"
#include "DBCenter.h"

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

    setObjectName("ConversationListView");

    m_pModel = new ConversationItemModel(this);
    this->setModel(m_pModel);
    m_pModel->setModelData(&m_conversationList);

    ConversationItemDelegate *delegate = new ConversationItemDelegate(this);
    this->setItemDelegate(delegate);

    initConnections();
}

ConversationListView::~ConversationListView()
{
    if (m_pModel)
    {
        delete m_pModel;
        m_pModel = Q_NULLPTR;
    }
}

void ConversationListView::updateConversationList()
{
    m_conversationList.clear();

    /// test data
    for (int i = 0; i < 0; i++)
    {
        ConversationItem item;

        item.conversationId = QString::number(i);
        item.conversationTitle = QString("conversation%1ersationTitle()").arg(i + 100);
        item.unReadCount = 3;
        item.headerPath = ":/images/u29.png";
        item.lastMsgContent = QStringLiteral("test message aaaaaaaaaaaaaaa");
        item.lastMsgTime = "18/11/11";

        if (i % 3 == 0)
        {
            item.itemType = SoloTypeConversation;
        }
        else if (i % 3 == 1)
        {
            item.itemType = GroupTypeConversation;
        }
        else
        {
            item.itemType = OtherTypeConversation;
        }

        m_conversationList.append(item);
    }
    /// test data end

    QList<UCSConversation*> conversationList =  UCSIMClient::Instance()->getConversationList(allChat);
    foreach (UCSConversation *conversation, conversationList)
    {
        ConversationItem item;

        item.conversationId = conversation->targetId();
        item.conversationType = conversation->conversationType();
        item.conversationTitle = conversation->conversationTitle();
        if (item.conversationTitle.isEmpty())
        {
            item.conversationTitle = item.conversationId;
        }
        item.unReadCount = conversation->unreadMessageCount();
        item.isTop = conversation->isTop();

        if (conversation->conversationType() == UCS_IM_SOLOCHAT)
        {
            ContactEntity contact;
            DBCenter::contactMgr()->getContact(conversation->targetId(), contact);

            item.headerPath = contact.headPath;
            item.itemType = SoloTypeConversation;
        }
        else if (conversation->conversationType() == UCS_IM_GROUPCHAT ||
                 conversation->conversationType() == UCS_IM_DISCUSSIONCHAT)
        {
            item.itemType = GroupTypeConversation;
            item.headerPath = ":/Resources/Headers/header_group.png";
        }
        else
        {
            item.itemType = OtherTypeConversation;
            item.headerPath = ":/Resources/Headers/header_other.png";
        }

        UCSMessage *message = conversation->lastestMessage();
        if (message != Q_NULLPTR)
        {
            item.lastMsgTime = CommonHelper::timeToString(message->time, "MM-dd hh:mm");
            UCS_IM_MsgType msgType = message->messageType;
            switch (msgType) {
            case UCS_IM_TEXT:
            {
                UCSTextMsg *textMsg = static_cast<UCSTextMsg*>(message->content);
                item.lastMsgContent = textMsg->content();
            }
                break;

            case UCS_IM_IMAGE:
            {
                item.lastMsgContent = QStringLiteral("[图片]");
            }
                break;

            case UCS_IM_VOICE:
            {
                item.lastMsgContent = QStringLiteral("[语音]");
            }
                break;

            case UCS_IM_Location:
            {
                item.lastMsgContent = QStringLiteral("[位置]");
            }
                break;

            case UCS_IM_Custom:
            {
                item.lastMsgContent = QStringLiteral("[自定义消息]");
            }
                break;

            case UCS_IM_System:
            {
                item.lastMsgContent = QStringLiteral("[通知]");
            }
                break;

            default:
                item.lastMsgContent = QStringLiteral("[未知类型消息]");
                break;
            }
        }
        else
        {
            item.lastMsgContent = QString("");
        }

        m_conversationList.append(item);
    }

    qDeleteAll(conversationList);
    conversationList.clear();

    m_pModel->refreshModel();
}

void ConversationListView::enterConversation(QString targetId, QString name)
{
    for (int row = 0; row < m_conversationList.size(); ++row)
    {
        ConversationItem conversation = m_conversationList.at(row);
        if (!QString::compare(conversation.conversationId, targetId, Qt::CaseSensitive))
        {
            QModelIndex index = m_pModel->index(row);
            this->setCurrentIndex(index);
            emit itemClicked(conversation);
        }
    }
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
        ConversationItemType itemType = m_conversationList.at(row).itemType;

        QMenu * pMenu = new QMenu(this);

        QAction *pDelAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("删除聊天"), pMenu);
        QAction *pTopAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("置顶"), pMenu);
        QAction *pModGroup = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("修改群名称"), pMenu);

        connect(pDelAct, SIGNAL(triggered()), this, SLOT(onDeleteItemAction()));
        connect(pTopAct, SIGNAL(triggered()), this, SLOT(onSetTopItemAction()));
        connect(pModGroup, SIGNAL(triggered()), this, SLOT(onModifyItemAction()));

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

void ConversationListView::onUpdateData()
{
    updateConversationList();
}

void ConversationListView::onClickedItem(QModelIndex index)
{
    if (index.isValid())
    {
        ConversationItem conversation = m_conversationList.at(index.row());

//        UCS_LOG(UCSLogger::kTraceApiCall, "Conversation",
//                QString("onClickedItem: %1").arg(index.data(Qt::UserRole).toString()));

        emit itemClicked(conversation);
    }
}

void ConversationListView::onDeleteItemAction()
{
    QModelIndex index = indexAt(m_eventPos);
    if (index.isValid())
    {
        UCS_LOG(UCSLogger::kTraceInfo, "Conversation",
                QString("onDeleteItem: %1")
                .arg(index.data(Qt::UserRole).toString()));

        if (!m_conversationList.isEmpty())
        {
            ConversationItem conversation = m_conversationList.at(index.row());
            QString conversationId = conversation.conversationId;
            quint32 conversationType = conversation.conversationType;

            m_conversationList.removeAt(index.row());

            UCSIMClient::Instance()->removeConversation((UCS_IM_ConversationType)conversationType,
                                                        conversationId);

            m_pModel->refreshModel();

            emit itemDeleted(conversation);
        }
    }
}

void ConversationListView::onSetTopItemAction()
{
    QModelIndex index = indexAt(m_eventPos);
    if (index.row() >= 0)
    {
        UCS_LOG(UCSLogger::kTraceInfo, "Conversation",
                QString("onSetTopItem: %1")
                .arg(index.data(Qt::UserRole).toString()));

        if (m_conversationList.isEmpty())
        {
            return;
        }

        bool isTop = !m_conversationList.at(index.row()).isTop;
        UCSIMClient::Instance()->setConversationToTop(m_conversationList.at(index.row()).conversationType,
                                                      m_conversationList.at(index.row()).conversationId,
                                                      isTop);

        updateConversationList();
        m_pModel->refreshModel();
    }
}

void ConversationListView::onModifyItemAction()
{
    QModelIndex index = indexAt(m_eventPos);
    if (index.row() >= 0)
    {
        UCS_LOG(UCSLogger::kTraceInfo, "Conversation",
                QString("onModifyItem: %1")
                .arg(index.data(Qt::UserRole).toString()));

        if (m_conversationList.isEmpty())
        {
            return;
        }

        if (m_conversationList.at(index.row()).itemType == GroupTypeConversation)
        {
    //        UCSIMClient::Instance()->setDiscussionTopicWithDiscussionId(m_conversationList.at(row).conversationId, title);
        }

        updateConversationList();
        m_pModel->refreshModel();
    }
}
