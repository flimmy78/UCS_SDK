#include "ChatMsgTableDelegate.h"
#include "ChatMsgTableModel.h"
#include "BaseMsgWidget.h"

ChatMsgTableDelegate::ChatMsgTableDelegate(QObject *parent)
    : QItemDelegate(parent)
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
    QItemDelegate::paint(painter, option, index);
}

QSize ChatMsgTableDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QItemDelegate::sizeHint(option, index);
}

QWidget *ChatMsgTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    BaseMsgWidget *msgWidget = new BaseMsgWidget(parent);
    return msgWidget;
}

void ChatMsgTableDelegate::setEditorData(QWidget *editor,
                                         const QModelIndex &index) const
{

}

void ChatMsgTableDelegate::setModelData(QWidget *editor,
                                        QAbstractItemModel *model,
                                        const QModelIndex &index) const
{

}

void ChatMsgTableDelegate::updateEditorGeometry(QWidget *editor,
                                                const QStyleOptionViewItem &option,
                                                const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
