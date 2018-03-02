#include "ConversationItemModel.h"
#include "Interface/UCSIMSDKPublic.h"

#include <QDateTime>
#include <QPixmap>

ConversationItemModel::ConversationItemModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_conversationList.clear();
}

//QModelIndex ConversationItemModel::index(int row, int column, const QModelIndex &parent) const
//{

//}

//QModelIndex ConversationItemModel::parent(const QModelIndex &index) const
//{

//}

int ConversationItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_conversationList.count();
}

//int ConversationItemModel::columnCount(const QModelIndex &parent) const
//{

//}

QVariant ConversationItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= m_conversationList.size()))
        return QVariant();

    // FIXME: Implement me!
    ConversationItem item = m_conversationList.at(index.row());
//    if (role == Qt::DisplayRole)
//    {
//        return item.conversationTitle;
//    }
    if (role == Qt::UserRole)
    {
        return item.conversationTitle;
    }
    else if (role == Qt::UserRole + 1)
    {
        QPixmap pixmap;
        pixmap.load(item.headerPath);
        return pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::FastTransformation);
    }
    else if (role == Qt::UserRole + 2)
    {
        return item.lastMsgTime;
    }
    else if (role == Qt::UserRole + 3)
    {
        return item.lastMsgContent;
    }

    return QVariant();
}

void ConversationItemModel::refreshModelData()
{
    m_conversationList.clear();

    /// test data
    for (int i = 0; i < 12; i++)
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
        item.unReadCount = conversation->unreadMessageCount();
        item.isTop = conversation->isTop();
        item.headerPath = ":/images/u29.png";

        if (conversation->conversationType() == UCS_IM_SOLOCHAT)
        {
            item.itemType = SoloTypeConversation;
        }
        else if (conversation->conversationType() == UCS_IM_GROUPCHAT ||
                 conversation->conversationType() == UCS_IM_DISCUSSIONCHAT)
        {
            item.itemType = GroupTypeConversation;
        }
        else
        {
            item.itemType = OtherTypeConversation;
        }

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

        m_conversationList.append(item);
    }

    qDeleteAll(conversationList);
    conversationList.clear();
}

void ConversationItemModel::removeItem(int row)
{
    if (m_conversationList.isEmpty())
    {
        return;
    }

    m_conversationList.removeAt(row);

//    UCSIMClient::Instance()->removeConversation(m_conversationList.at(row).conversationType,
//                                                m_conversationList.at(row).conversationId);

//    refreshModelData();
}

void ConversationItemModel::topItem(int row)
{
    if (m_conversationList.isEmpty())
    {
        return;
    }

    bool isTop = !m_conversationList.at(row).isTop;
    UCSIMClient::Instance()->setConversationToTop(m_conversationList.at(row).conversationType,
                                                  m_conversationList.at(row).conversationId,
                                                  isTop);

    refreshModelData();
}

void ConversationItemModel::modifyItem(int row, QString &title)
{
    if (m_conversationList.isEmpty())
    {
        return;
    }

    if (m_conversationList.at(row).itemType == GroupTypeConversation)
    {

    }
}

void ConversationItemModel::clearAll()
{
    m_conversationList.clear();
}

ConversationItemType ConversationItemModel::itemType(int row)
{
    if (m_conversationList.isEmpty())
    {
        return OtherTypeConversation;
    }
    return m_conversationList.at(row).itemType;
}

QString ConversationItemModel::conversationId(int row)
{
    return m_conversationList.at(row).conversationId;
}

quint32 ConversationItemModel::conversationType(int row)
{
    return (quint32)m_conversationList.at(row).conversationType;
}
