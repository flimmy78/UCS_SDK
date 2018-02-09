#include "historycallitemdelegate.h"
#include <QPixmap>


HistoryCallItemDelegate::HistoryCallItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void HistoryCallItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    if (viewOption.state & QStyle::State_HasFocus)
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }

//    QStyledItemDelegate::paint(painter, viewOption, index);

    // prepare
    painter->save();

    // get the data and the rectangles
    QString headerPath = index.data(Qt::UserRole).toString();
    QString nickname = index.data(Qt::UserRole + 1).toString();
    QString callTime = index.data(Qt::UserRole + 2).toString();
    QString callType = index.data(Qt::UserRole + 3).toString();
    QString callIcon = index.data(Qt::UserRole + 4).toString();

//    qDebug() << "headerPath " << headerPath;
//    qDebug() << "callIcon " << callIcon;

    QPixmap headerPix = QPixmap(headerPath);
    QPixmap callPix = QPixmap(callIcon);

    QRect headerRect = QRect(viewOption.rect.topLeft() + QPoint(8, 5), QSize(50, 50));
    QRect nameRect = QRect(headerRect.topRight() + QPoint(8, 5), QSize(80, 30));
    QRect timeRect = QRect(nameRect.topRight() + QPoint(8, 5), QSize(80, 30));
    QRect callRect = QRect(headerRect.topRight() + QPoint(8, 40), QSize(30, 20));
    QRect callIconRect = QRect(headerRect.topRight() + QPoint(38, 36), QSize(18, 18));

    painter->setPen(QColor(Qt::gray));
    painter->drawPixmap(headerRect, headerPix);
    painter->setFont(QFont("微软雅黑", 11, QFont::Normal, false));
    painter->drawText(nameRect, nickname);
    painter->setFont(QFont("微软雅黑", 10, QFont::Normal, false));
    painter->drawText(timeRect, callTime);
    painter->drawText(callRect, callType);
    painter->drawPixmap(callIconRect, callPix);

    // done
    painter->restore();
}

QSize HistoryCallItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(250, 60);
}
