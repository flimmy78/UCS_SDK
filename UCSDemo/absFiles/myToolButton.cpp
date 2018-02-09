#include "myToolButton.h"

MyToolButton::MyToolButton(QWidget *parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setStyleSheet("QToolButton{background:transparent;}");
    setCursor(Qt::ArrowCursor);    
}

MyToolButton::MyToolButton(const QString &text, QWidget *parent)
    : QToolButton(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setStyleSheet("QToolButton{background:transparent;}");
    setCursor(Qt::ArrowCursor);
    setFont(QFont(QStringLiteral("微软雅黑"), 17, QFont::Bold, false));
    setText(text);
}

QSize MyToolButton::sizeHint() const
{
    QSize size = QToolButton::sizeHint();
//    size.rheight() += 20;
//    size.rwidth() = qMax(size.width(), size.height());
    return size;
}
