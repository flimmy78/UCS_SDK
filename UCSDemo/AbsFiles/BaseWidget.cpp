#include "BaseWidget.h"

#include <QFile>
#include <QPainter>
#include <QStyleOption>

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
}

void BaseWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    /// <If you subclass from QWidget, you need to provide a
    ///  paintEvent for your custom QWidget as below for style sheet used>
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void BaseWidget::loadStyleSheet(const QString &stylePath)
{
    QFile file(stylePath);
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QString(file.readAll());
        this->setStyleSheet(styleSheet);
        file.close();
    }
}
