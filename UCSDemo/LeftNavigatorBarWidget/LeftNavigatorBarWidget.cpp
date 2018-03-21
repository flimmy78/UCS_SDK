#include "LeftNavigatorBarWidget.h"
#include <QVBoxLayout>
#include <QString>
#include <QMessageBox>
#include <QPainter>
#include "Interface/UCSTcpClient.h"
#include "Interface/UCSIMClient.h"
#include "CommonHelper.h"

LeftNavigatorBarWidget::LeftNavigatorBarWidget(QWidget *parent)
    : BaseWidget(parent)
{
//    QPalette palette;
//    palette.setColor(QPalette::Background, QColor(32, 155, 152));
//    setPalette(palette);
//    setAutoFillBackground(true);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setObjectName("NavigatorBar");

    initLayout();
    initConnection();
}

LeftNavigatorBarWidget::~LeftNavigatorBarWidget()
{

}

void LeftNavigatorBarWidget::initLayout()
{
    QVBoxLayout *vlayout = new QVBoxLayout;

    m_pBtnInfo = new MyPushButton(this);
    m_pBtnInfo->setFixedHeight(60);
    m_pBtnInfo->setStyleSheet("QPushButton{background-image:url(:/images/mainleft/u6.png);background-repeat:no-repeat;background-position:center;border:none;}"
                               "QPushButton::hover{background-image:url(:/images/mainleft/u6.png);background-repeat:no-repeat;background-position:center;border:none;}"
                               "QPushButton::pressed{background-image:url(:/images/mainleft/u6.png);background-repeat:no-repeat;background-position:center;border:none;}");


    QString userId = CommonHelper::readSetting(kSettingLoginUserId).toString();
    m_pBtnInfo->setToolTip(userId);

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

void LeftNavigatorBarWidget::initConnection()
{
    connect(m_pBtnInfo, SIGNAL(pressed()), this, SLOT(slot_personInfoButtonClick()));
    connect(m_pBtnSetting, SIGNAL(pressed()), this, SLOT(slot_settingButtonClick()));
}

void LeftNavigatorBarWidget::onChangeButtonSelected(int index)
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

void LeftNavigatorBarWidget::slot_personInfoButtonClick()
{

}

void LeftNavigatorBarWidget::slot_settingButtonClick()
{
//    QMessageBox::information(this, QStringLiteral("设置"), "user setting", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);    
}
