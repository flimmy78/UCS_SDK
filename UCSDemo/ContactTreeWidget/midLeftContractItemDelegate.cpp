#include "midLeftContractItemDelegate.h"
#include "midLeftContactTreeModel.h"

#include <QPainter>

MidLeftContractItemDelegate::MidLeftContractItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , m_pTreeView(NULL)
    , m_pixmapWidth(9)
    , m_pixmapHeight(9)
{

}

void MidLeftContractItemDelegate::setView(MidLeftContractsTreeView *treeView)
{
    m_pTreeView = treeView;
}

void MidLeftContractItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    if (viewOption.state & QStyle::State_HasFocus)
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, viewOption, index);
}

QSize MidLeftContractItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

QWidget *MidLeftContractItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return new QLineEdit(parent);
}

void MidLeftContractItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *pLineEdit = qobject_cast<QLineEdit*>(editor);
    const QString text = index.data().toString();
    pLineEdit->setText(text);
}

void MidLeftContractItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *pLineEdit = qobject_cast<QLineEdit*>(editor);
    model->setData(index, qVariantFromValue(pLineEdit->text()));
}

void MidLeftContractItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect displayRect = QRect(option.rect);
    editor->setGeometry(displayRect);
}
