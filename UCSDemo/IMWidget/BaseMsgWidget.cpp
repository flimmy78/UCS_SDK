#include "BaseMsgWidget.h"

BaseMsgWidget::BaseMsgWidget(QWidget *parent)
    : QWidget(parent)
{

}

bool BaseMsgWidget::isSender() const
{
    return m_isSender;
}

void BaseMsgWidget::setIsSender(bool isSender)
{
    m_isSender = isSender;
}

void BaseMsgWidget::paintEvent(QPaintEvent *event)
{

}

QString BaseMsgWidget::nickName() const
{
    return m_nickName;
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
