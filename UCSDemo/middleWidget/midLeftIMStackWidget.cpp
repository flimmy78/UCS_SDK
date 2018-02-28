#include "midLeftIMStackWidget.h"
#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include "middleWidget/middleLeftWidget.h"
#include "Interface/UCSIMSDKPublic.h"

MidLeftIMStackWidget::MidLeftIMStackWidget(QWidget *parent, int width)
    : MyScrollArea(parent)
    , m_imListWid(this)
{
    setMouseTracking(true);
    setFixedWidth(width);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    initLayout();
    initConnection();
//    updateData();
}

void MidLeftIMStackWidget::updateData()
{

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
        m_imListWid.addListItem(&item);
    }

    qDeleteAll(conversationList);
    conversationList.clear();
}

void MidLeftIMStackWidget::initLayout()
{
    qDebug() << "MidLeftIMStackWidget";
    QHBoxLayout *pMainLayout = new QHBoxLayout;

    pMainLayout->addWidget(&m_imListWid);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void MidLeftIMStackWidget::initConnection()
{
    connect(&m_imListWid, SIGNAL(sig_itemClicked(int)), this, SLOT(slot_itemClicked(int)));
}

QString MidLeftIMStackWidget::currentName()
{
    return m_imItems.at(m_imListWid.currentRow()).conversationTitle;
}

void MidLeftIMStackWidget::slot_itemClicked(int row)
{
    QString name = m_imItems.at(row).conversationTitle;
    qDebug() << "MidLeftIMStackWidget slot_itemClicked " << name;

    emit sig_itemClicked(name);
}
