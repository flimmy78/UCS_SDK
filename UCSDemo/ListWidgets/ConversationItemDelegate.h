#ifndef CONVERSATIONITEMDELEGATE_H
#define CONVERSATIONITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <qglobal.h>

class ConversationItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ConversationItemDelegate(QObject *parent = 0);

protected:
    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

#endif // CONVERSATIONITEMDELEGATE_H
