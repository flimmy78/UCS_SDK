#ifndef CHATMSGTABLEDELEGATE_H
#define CHATMSGTABLEDELEGATE_H

#include <QObject>
#include <QPainter>
#include <QStyledItemDelegate>
#include <qglobal.h>

class ChatMsgTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ChatMsgTableDelegate(QObject *parent = 0);
    ~ChatMsgTableDelegate();

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
};

#endif // CHATMSGTABLEDELEGATE_H
