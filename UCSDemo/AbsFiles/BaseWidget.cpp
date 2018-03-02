#include "BaseWidget.h"

#include <QFile>

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
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
