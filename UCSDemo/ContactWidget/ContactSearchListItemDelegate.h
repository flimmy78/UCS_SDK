#ifndef CONTACTSEARCHLISTITEMDELEGATE_H
#define CONTACTSEARCHLISTITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QWidget>
#include <qglobal.h>

class ContactSearchListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ContactSearchListItemDelegate(QObject *parent = 0);

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
};

#endif // CONTACTSEARCHLISTITEMDELEGATE_H
