#include "midLeftIMStackWidget.h"
#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include "middleWidget/middleLeftWidget.h"
#include "Interface/UCSIMSDKPublic.h"

MidLeftIMStackWidget::MidLeftIMStackWidget(QWidget *parent, int width)
    : MyScrollArea(parent)
{
    setMouseTracking(true);
    setFixedWidth(width);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    initLayout();
    initConnection();
}

void MidLeftIMStackWidget::updateData()
{
#if 0
    m_imItems.clear();
    m_pConversationList->clearAllItems();

    QList<UCSConversation*> conversationList =  UCSIMClient::Instance()->getConversationList(allChat);
    foreach (UCSConversation *conversation, conversationList)
    {
        IMConversationItem item;

        item.conversationId = conversation->targetId();
        item.conversationTitle = conversation->conversationTitle();
        item.unReadCount = conversation->unreadMessageCount();
        item.headerPath = ":/images/u29.png";

        UCSMessage *message = conversation->lastestMessage();
        if (message != Q_NULLPTR)
        {
            QDateTime dateTime;
            item.lastMsgTime = dateTime.fromTime_t(message->time).toString("MM-dd hh:mm");

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

            case UCS_IM_DiscussionNotification:
            {
                item.lastMsgContent = QStringLiteral("[通知]");
            }
                break;

            default:
                item.lastMsgContent = QStringLiteral("[未知类型消息]");
                break;
            }
        }
        m_imItems.append(item);
        m_pConversationList->addListItem(&item);
    }

    qDeleteAll(conversationList);
    conversationList.clear();
#else
    m_pConversationListView->updateConversationList();
#endif
}

void MidLeftIMStackWidget::initLayout()
{
    qDebug() << "MidLeftIMStackWidget";
//    m_pConversationList = new MidLeftConversationListWidget(this);

    m_pConversationListView = new ConversationListView(this);
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);

//    pMainLayout->addWidget(m_pConversationList);
    pMainLayout->addWidget(m_pConversationListView);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pMainLayout);
}

void MidLeftIMStackWidget::initConnection()
{
//    connect(m_pConversationList, SIGNAL(sig_itemClicked(int)), this, SLOT(slot_itemClicked(int)));
    connect(m_pConversationListView, SIGNAL(itemClicked(QString,quint32)), this, SLOT(onItemClicked(QString,quint32)));
}

QString MidLeftIMStackWidget::currentName()
{
    return m_imItems.at(m_pConversationList->currentRow()).conversationTitle;
}

void MidLeftIMStackWidget::slot_itemClicked(int row)
{
    QString name = m_imItems.at(row).conversationTitle;
    qDebug() << "MidLeftIMStackWidget slot_itemClicked " << name;

    emit sig_itemClicked(name);
}

void MidLeftIMStackWidget::onItemClicked(QString targetId, quint32 type)
{
    qDebug() << "targetId: " << targetId;
    qDebug() << "type: " << type;
}

ConversationListView *MidLeftIMStackWidget::getListView() const
{
    return m_pConversationListView;
}
