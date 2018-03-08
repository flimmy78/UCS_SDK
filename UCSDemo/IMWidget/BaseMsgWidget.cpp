#include "BaseMsgWidget.h"
#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>


BaseMsgWidget::BaseMsgWidget(QWidget *parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    initLayout();
}

bool BaseMsgWidget::isSender() const
{
    return m_isSender;
}

void BaseMsgWidget::setIsSender(bool isSender)
{
    m_isSender = isSender;
}

QString BaseMsgWidget::nickName() const
{
    return m_nickName;
}

void BaseMsgWidget::initLayout()
{
    QBoxLayout *pMainLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
    QLabel *label1 = new QLabel;
    QLabel *label2 = new QLabel;

    label1->setText("test1");
    label2->setText("text2222222222222222222222222222222222222222222222222222222222222222222222222");

    pMainLayout->setDirection(QBoxLayout::LeftToRight);
    pMainLayout->addWidget(label1);
    pMainLayout->addWidget(label2);
}

void BaseMsgWidget::initConnections()
{

}

QString BaseMsgWidget::time() const
{
    return m_time;
}

void BaseMsgWidget::setTime(const QString &time)
{
    m_time = time;
}

QString BaseMsgWidget::userName() const
{
    return m_userName;
}

void BaseMsgWidget::setUserName(const QString &userName)
{
    m_userName = userName;
}

void BaseMsgWidget::setNickName(const QString &nickName)
{
    m_nickName = nickName;
}
