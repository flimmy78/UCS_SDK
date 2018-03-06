#include "CallListItemDelegate.h"
#include <QPixmap>


CallListItemDelegate::CallListItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void CallListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    if (viewOption.state & QStyle::State_HasFocus)
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

    painter->save();
    QSize itemSize = viewOption.rect.size();
    const QPixmap header = qvariant_cast<QPixmap>(index.data(Qt::UserRole));
    const QString nickname = index.data(Qt::UserRole + 1).toString();
    const QString callTime = index.data(Qt::UserRole + 2).toString();
    const QString callType = index.data(Qt::UserRole + 3).toString();
    const QPixmap typeIcon = qvariant_cast<QPixmap>(index.data(Qt::UserRole + 4));

    int padding = (itemSize.height() - header.height()) / 2;
    int scroll_width = 0;

    // header
    QRect headerRect = QRect(viewOption.rect.topLeft() + QPoint(padding, padding), header.size());
    painter->drawPixmap(headerRect, header);

    painter->restore();
    painter->save();

    QFont font = painter->font();
    font.setPointSize(9);
    painter->setFont(font);
    painter->setPen(QColor(Qt::lightGray));
    QFontMetrics fm(painter->font());

    // time
    QSize timeSize = QSize(fm.width(callTime), fm.height());
    QRect timeRect = QRect(viewOption.rect.topRight() + QPoint(-(timeSize.width() + scroll_width), padding), timeSize);
    painter->drawText(timeRect, callTime);

    // type
    QSize typeTextSize = QSize(fm.width(callType), fm.height());
    QRect typeTextRect = QRect(headerRect.bottomRight() + QPoint(padding, (-typeTextSize.height())), typeTextSize);
    painter->drawText(typeTextRect, callType);

    QRect typeIconRect = QRect(headerRect.bottomRight() + QPoint(80, (-typeIcon.size().height())), typeIcon.size());
    painter->drawPixmap(typeIconRect, typeIcon);

    painter->restore();
    painter->save();

    // name
    QFont font1 = painter->font();
    font1.setPointSize(10);
    painter->setFont(font1);
    painter->setPen(QColor(Qt::black));

    QFontMetrics fm1(painter->font());
    QSize nameSize = QSize(itemSize.width() - itemSize.height() - timeSize.width() - scroll_width - padding, fm1.height());
    QString nameElided = fm1.elidedText(nickname, Qt::ElideRight,
                                        nameSize.width(), Qt::TextShowMnemonic);

    QRect nameRect = QRect(headerRect.topRight() + QPoint(padding, 0), nameSize);
    painter->drawText(nameRect, nameElided);

    // done
    painter->restore();

    QStyledItemDelegate::paint(painter, option, index);
}

QSize CallListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qint32 width = QStyledItemDelegate::sizeHint(option, index).width();
    return QSize(width, 60);
}
