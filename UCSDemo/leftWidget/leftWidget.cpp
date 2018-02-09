#include "leftWidget.h"
#include <QVBoxLayout>
#include <QString>
#include <QMessageBox>
#include <QPainter>
#include "inc/UCSTcpClient.h"
#include "inc/UCSIMClient.h"

LeftWidget::LeftWidget(QWidget *parent, int width)
    : QWidget(parent)
{
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(32, 155, 152));
    setPalette(palette);
    setAutoFillBackground(true);

    setFixedWidth(width);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    initLayout();
    initConnection();
}

LeftWidget::~LeftWidget()
{

}

void LeftWidget::initLayout()
{
    QVBoxLayout *vlayout = new QVBoxLayout;

    m_pBtnInfo = new MyPushButton(this);
    m_pBtnInfo->setFixedHeight(60);
    m_pBtnInfo->setStyleSheet("QPushButton{background-image:url(:/images/mainleft/u6.png);background-repeat:no-repeat;background-position:center;border:none;}"
                               "QPushButton::hover{background-image:url(:/images/mainleft/u6.png);background-repeat:no-repeat;background-position:center;border:none;}"
                               "QPushButton::pressed{background-image:url(:/images/mainleft/u6.png);background-repeat:no-repeat;background-position:center;border:none;}");
//    m_pBtnInfo->setToolTip(QStringLiteral("个人信息"));

    m_pBtn[0] = new StackButton(0,
                                ":/images/mainleft/u80.png",
                                ":/images/mainleft/u80.png",
                                ":/images/mainleft/u80.png",
                                this);
    m_pBtn[1] = new StackButton(1,
                                ":/images/mainleft/u8.png",
                                ":/images/mainleft/u8.png",
                                ":/images/mainleft/u8.png",
                                this);
    m_pBtn[2] = new StackButton(2,
                                ":/images/mainleft/u78.png",
                                ":/images/mainleft/u78.png",
                                ":/images/mainleft/u78.png",
                                this);
    m_pBtn[1]->setToolTip(QStringLiteral("通讯录"));
    m_pBtn[0]->setToolTip(QStringLiteral("通话记录"));
    m_pBtn[2]->setToolTip(QStringLiteral("消息"));
    for (int i = 0; i < 3; i++)
    {
        m_pBtn[i]->setObjectName(QString::number(i));
        m_pBtn[i]->setFixedHeight(60);
    }
    m_pBtn[0]->setSelected(true);

    m_pBtnSetting = new MyPushButton(this);
    m_pBtnSetting->setFixedHeight(60);
    m_pBtnSetting->setStyleSheet("QPushButton{background-image:url(:/images/mainleft/u68.png);background-repeat:no-repeat;background-position:center;border:none;}"
                                 "QPushButton::hover{background-image:url(:/images/mainleft/u68.png);background-repeat:no-repeat;background-position:center;border:none;}"
                                 "QPushButton::pressed{background-image:url(:/images/mainleft/u68.png);background-repeat:no-repeat;background-position:center;border:none;}");
    m_pBtnSetting->setToolTip(QStringLiteral("设置"));

    vlayout->addSpacerItem(new QSpacerItem(this->width(), 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    vlayout->addWidget(m_pBtnInfo);
    vlayout->addSpacerItem(new QSpacerItem(this->width(), 10, QSizePolicy::Fixed, QSizePolicy::Fixed));
    vlayout->addWidget(m_pBtn[0]);
    vlayout->addWidget(m_pBtn[1]);
    vlayout->addWidget(m_pBtn[2]);
    vlayout->addSpacerItem(new QSpacerItem(this->width(), 60, QSizePolicy::Fixed, QSizePolicy::Expanding));
    vlayout->addWidget(m_pBtnSetting);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0, 0, 0, 0);
    setLayout(vlayout);
}

void LeftWidget::initConnection()
{
    connect(m_pBtnInfo, SIGNAL(pressed()), this, SLOT(slot_personInfoButtonClick()));
    connect(m_pBtnSetting, SIGNAL(pressed()), this, SLOT(slot_settingButtonClick()));
}

void LeftWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

}

void LeftWidget::slot_changeButtonSelected(int index)
{
    if (index >= 3)
    {
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        if (i == index)
        {
            m_pBtn[index]->setSelected(true);
        }
        else
        {
            m_pBtn[i]->setSelected(false);
        }
    }
}

void LeftWidget::slot_personInfoButtonClick()
{
//    QMessageBox::information(NULL, QStringLiteral("信息提示"), "user info", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//    QString token = "eyJBbGciOiJIUzI1NiIsIkFjY2lkIjoiY2U4OGQ4OWM0ZmU1ODhkODE1MTc3NDA1OGE0ZTA3ZGMiLCJBcHBpZCI6ImJhMjAyYzhlMWM5MzQ1MmI4YzAyNzY3MzJjYmM3ZDE1IiwiVXNlcmlkIjoiNjg0OTEwMDAwNTE5MjIifQ==.XJl/AePNwQg0jzeDamX2oMiMntD7KQGbOF2FXlBd0Yk=";

    UCSTcpClient::Instance()->init();
    UCSIMClient::Instance()->init();

    QString token = "eyJBbGciOiJIUzI1NiIsIkFjY2lkIjoiZDU1ODA5MDJjYWI0ZDg4NTNiMDY2NDRhNTVlZjcyMGEiLCJBcHBpZCI6ImFhMmUyYmM2ZTIyZTRmYmZhZjkyNGZhNzVjMDJkYzA5IiwiVXNlcmlkIjoiMTM2MzI3ODA4NzIifQ==.OvXyq1GWSRykxlkWuF27RgllaPufxb99srh07LcsJEk=";
    UCSTcpClient::Instance()->doLogin(token);
}

void LeftWidget::slot_settingButtonClick()
{
//    QMessageBox::information(this, QStringLiteral("设置"), "user setting", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    m_restManager.getAuthCode("13632780872");
}
