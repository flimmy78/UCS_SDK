#include "midLeftSearchStackWidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>

MidLeftSearchStackWidget::MidLeftSearchStackWidget(QWidget *parent)
    :MyScrollArea(parent)
{
    initLayout();
    initConnection();
}

void MidLeftSearchStackWidget::initLayout()
{
    qDebug() << "MiddleSearchStackWidget";
    QHBoxLayout *pMainLayout = new QHBoxLayout;

    QLabel *lbl = new QLabel(this);
    lbl->setText("MiddleSearchStackWidget");

    pMainLayout->addWidget(lbl);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void MidLeftSearchStackWidget::initConnection()
{

}
