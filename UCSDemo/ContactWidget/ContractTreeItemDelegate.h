#ifndef CONTRACT_TREE_ITEM_DELEGATE_H
#define CONTRACT_TREE_ITEM_DELEGATE_H

#include "common/qtheaders.h"

class ContractsTreeView;

class ContractTreeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ContractTreeItemDelegate(QObject *parent = 0);

    void setView(ContractsTreeView *treeView);

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
    ContractsTreeView *m_pTreeView;

    int m_pixmapWidth;
    int m_pixmapHeight;
};

#endif // CONTRACT_TREE_ITEM_DELEGATE_H
