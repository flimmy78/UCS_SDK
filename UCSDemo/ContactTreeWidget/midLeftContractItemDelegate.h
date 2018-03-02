#ifndef MIDLEFTCONTRACTITEMDELEGATE_H
#define MIDLEFTCONTRACTITEMDELEGATE_H

#include "common/qtheaders.h"

class MidLeftContractsTreeView;

class MidLeftContractItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit MidLeftContractItemDelegate(QObject *parent = 0);

    void setView(MidLeftContractsTreeView *treeView);

protected:
    // painting
    void paint(QPainter * painter,
               const QStyleOptionViewItem & option,
               const QModelIndex & index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    // editing
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

private:
    MidLeftContractsTreeView *m_pTreeView;

    int m_pixmapWidth;
    int m_pixmapHeight;
};

#endif // MIDLEFTCONTRACTITEMDELEGATE_H
