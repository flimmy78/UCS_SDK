#include "EmptyWidget.h"
#include "CommonHelper.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>

EmptyWidget::EmptyWidget(QWidget *parent)
    : BaseWidget(parent)
{
    setObjectName("EmptyWidget");

    QLabel *icon = new QLabel(this);
    icon->setObjectName("emptyBG");

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->addWidget(icon, Qt::AlignCenter);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
}
