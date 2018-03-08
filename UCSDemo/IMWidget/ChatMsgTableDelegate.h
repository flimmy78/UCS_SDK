#ifndef CHATMSGTABLEDELEGATE_H
#define CHATMSGTABLEDELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include <QPainter>
#include <QStyledItemDelegate>
#include <qglobal.h>

class ChatMsgTableDelegate : public QItemDelegate
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

    // editing
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // CHATMSGTABLEDELEGATE_H
