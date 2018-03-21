#include "ContactSearchListItemDelegate.h"
#include "ContactDefine.h"

#include <QPainter>

ContactSearchListItemDelegate::ContactSearchListItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void ContactSearchListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    if (viewOption.state & QStyle::State_HasFocus)
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    painter->save();
    QSize itemSize = viewOption.rect.size();

    ///< get data >
    const QPixmap header = qvariant_cast<QPixmap>(index.data(headPathRole));
    const QString userName = index.data(userNameRole).toString();
    const QString sectionName = index.data(parentNameRole).toString();

    /// < draw >
    int padding = (itemSize.height() - header.height()) / 2;
    int scrollWidth = 0;

    ///< header >
    QRect headerRect = QRect(viewOption.rect.topLeft() + QPoint(padding, padding), header.size());
    painter->drawPixmap(headerRect, header);

    painter->restore();
    painter->save();

    ///< name >
    QFont font = painter->font();
    font.setPointSize(10);
    painter->setFont(font);
    painter->setPen(QColor(Qt::black));

    QFontMetrics fm(painter->font());
    QSize nameSize = QSize(itemSize.width() - itemSize.height() - scrollWidth - padding, fm.height());
    QString nameElided = fm.elidedText(userName, Qt::ElideRight,
                                        nameSize.width(), Qt::TextShowMnemonic);

    QRect nameRect = QRect(viewOption.rect.topLeft() + QPoint(itemSize.height(), padding), nameSize);
    painter->drawText(nameRect, nameElided);

    painter->restore();
    painter->save();

    ///< section name >
    QFont font1 = painter->font();
    font1.setPointSize(9);
    painter->setFont(font1);
    painter->setPen(QColor(Qt::lightGray));
    QFontMetrics fm1(painter->font());

    QSize sectionSize = QSize(itemSize.width() - itemSize.height() - scrollWidth - padding, fm1.height());
    QRect sectionRect = QRect(viewOption.rect.bottomLeft() + QPoint(itemSize.height(), (-(sectionSize.height() + padding))), sectionSize);
    QString sectionElided = fm.elidedText(sectionName, Qt::ElideRight,
                                        sectionSize.width(), Qt::TextShowMnemonic);
    painter->drawText(sectionRect, sectionElided);

    /// < done >
    painter->restore();

    QStyledItemDelegate::paint(painter, option, index);
}

QSize ContactSearchListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qint32 width = QStyledItemDelegate::sizeHint(option, index).width();
    return QSize(width, 60);
}
