#ifndef HISTORYCALLITEMDELEGATE_H
#define HISTORYCALLITEMDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QStyledItemDelegate>
#include "common/qtheaders.h"

class CallListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit CallListItemDelegate(QObject* parent = 0);
    virtual ~CallListItemDelegate() {}

protected:
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
};

#endif // HISTORYCALLITEMDELEGATE_H
