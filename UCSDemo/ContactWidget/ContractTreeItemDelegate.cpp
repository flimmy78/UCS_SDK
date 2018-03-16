#include "ContractTreeItemDelegate.h"
#include "ContactTreeItemModel.h"

#include <QPainter>

ContractTreeItemDelegate::ContractTreeItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , m_pTreeView(NULL)
    , m_pixmapWidth(9)
    , m_pixmapHeight(9)
{

}

void ContractTreeItemDelegate::setView(ContactsTreeView *treeView)
{
    m_pTreeView = treeView;
}

void ContractTreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    if (viewOption.state & QStyle::State_HasFocus)
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, viewOption, index);
}

QSize ContractTreeItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

QWidget *ContractTreeItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);

    return new QLineEdit(parent);
}

void ContractTreeItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *pLineEdit = qobject_cast<QLineEdit*>(editor);
    const QString text = index.data().toString();
    pLineEdit->setText(text);
}

void ContractTreeItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *pLineEdit = qobject_cast<QLineEdit*>(editor);
    model->setData(index, qVariantFromValue(pLineEdit->text()));
}

void ContractTreeItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);

    QRect displayRect = QRect(option.rect);
    editor->setGeometry(displayRect);
}
