#include "ConversationItemDelegate.h"

#include <QPainter>

ConversationItemDelegate::ConversationItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void ConversationItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if 1
    // prepare
    painter->save();
    QSize itemSize = option.rect.size();

    // get the data and the rectangles
    const QString title = index.data(Qt::UserRole).toString();
    const QString time = index.data(Qt::UserRole + 2).toString();
    const QString content = index.data(Qt::UserRole + 3).toString();
    const QPixmap header = qvariant_cast<QPixmap>(index.data(Qt::UserRole + 1));

    // header
    int padding = (itemSize.height() - header.height()) / 2;
    int scroll_width = 0;

    QRect headerRect = QRect(option.rect.topLeft() + QPoint(padding, padding), header.size());
    painter->drawPixmap(headerRect, header);

    painter->restore();
    painter->save();

    QFont font = painter->font();
    font.setPointSize(9);
    painter->setFont(font);
    painter->setPen(QColor(Qt::lightGray));
    QFontMetrics fm(painter->font());

    ///< time >
    QSize timeSize = QSize(fm.width(time), fm.height());
    QRect timeRect = QRect(option.rect.topRight() + QPoint(-(timeSize.width() + scroll_width), padding), timeSize);
    painter->drawText(timeRect, time);

    ///< content_width = item_width - header_width - scroll_width(10) >
    /// header_width contains real header img width and padding, so eaual to item height.
    QSize contentSize = QSize(itemSize.width() - itemSize.height() - padding - scroll_width, fm.height());
    QString contentElided = fm.elidedText(content, Qt::ElideRight,
                                          contentSize.width(),
                                          Qt::TextShowMnemonic);

    QRect contentRect = QRect(option.rect.bottomLeft() + QPoint(itemSize.height(), -(fm.height() + padding)), contentSize);
    painter->drawText(contentRect, contentElided);

    painter->restore();
    painter->save();

    QFont titleFont = painter->font();
    titleFont.setPointSize(10);
    painter->setFont(titleFont);
    painter->setPen(QColor(Qt::black));

    QFontMetrics fm1(painter->font());

    ///< title_width = item_width - header_width -  time_width - scroll_width(10) >
    QSize titleSize = QSize(itemSize.width() - itemSize.height() - timeSize.width() - padding - scroll_width, fm1.height());
    QString titleElided = fm1.elidedText(title, Qt::ElideRight,
                                       titleSize.width(),
                                       Qt::TextShowMnemonic);
    QRect titleRect = QRect(option.rect.topLeft() + QPoint(itemSize.height(), padding), titleSize);
    painter->drawText(titleRect, titleElided);

    // done
    painter->restore();
#endif

    QStyledItemDelegate::paint(painter, option, index);
}

QSize ConversationItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    qint32 width = QStyledItemDelegate::sizeHint(option, index).width();
    return QSize(width, 60);
}
