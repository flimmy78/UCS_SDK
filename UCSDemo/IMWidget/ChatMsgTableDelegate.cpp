#include "ChatMsgTableDelegate.h"
#include "ChatMsgTableModel.h"

ChatMsgTableDelegate::ChatMsgTableDelegate(QObject *parent)
{

}

ChatMsgTableDelegate::~ChatMsgTableDelegate()
{

}

void ChatMsgTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QPixmap header = qvariant_cast<QPixmap>(index.data(HeaderRole));
    bool isSender = index.data(DirectionRole).toBool();
    QString nickName = index.data(NickNameRole).toString();
    QString userName = index.data(UserNameRole).toString();
    MessageBodyType type = (MessageBodyType)index.data(MsgTypeRole).toInt();

    if (type == MessageBodyType_Text)
    {
        QString content = index.data(ContentRole).toString();

    }

    painter->restore();
    QStyledItemDelegate::paint(painter, option, index);
}

QSize ChatMsgTableDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}
