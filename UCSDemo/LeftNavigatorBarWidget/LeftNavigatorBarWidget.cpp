#include "LeftNavigatorBarWidget.h"
#include <QVBoxLayout>
#include <QString>
#include <QMessageBox>
#include <QPainter>
#include <QMenu>
#include "Interface/UCSTcpClient.h"
#include "Interface/UCSIMClient.h"
#include "Interface/UCSLogger.h"
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
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);

//    QString userId = CommonHelper::readSetting(kSettingLoginUserId).toString();
//    m_pBtnInfo->setToolTip(userId);

    m_pBtn[0] = new StackButton(0,
                                ":/Resources/call_history_normal.png",
                                ":/Resources/call_history_hover.png",
                                ":/Resources/call_history_hover.png",
                                this);
    m_pBtn[1] = new StackButton(1,
                                ":/Resources/contact_normal.png",
                                ":/Resources/contact_hover.png",
                                ":/Resources/contact_hover.png",
                                this);
    m_pBtn[2] = new StackButton(2,
                                ":/Resources/message_normal.png",
                                ":/Resources/message_hover.png",
                                ":/Resources/message_hover.png",
                                this);
    m_pBtn[1]->setToolTip(QStringLiteral("通讯录"));
    m_pBtn[0]->setToolTip(QStringLiteral("通话记录"));
    m_pBtn[2]->setToolTip(QStringLiteral("消息"));
    for (int i = 0; i < 3; i++)
    {
        QString objName = QString("leftStackBtn");
        m_pBtn[i]->setObjectName(objName);
        m_pBtn[i]->setFixedHeight(60);
        m_pBtn[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
    m_pBtn[0]->setSelected(true);

    m_pBtnSetting = new QPushButton(this);
    m_pBtnSetting->setFlat(true);
    m_pBtnSetting->setCursor(Qt::ArrowCursor);
    m_pBtnSetting->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pBtnSetting->setFixedHeight(60);
//    m_pBtnSetting->setStyleSheet("QPushButton{background-image:url(:/Resources/app_setting.png);background-repeat:no-repeat;background-position:center;border:none;}"
//                                 "QPushButton::hover {}"
//                                 "QPushButton::pressed{}");
    m_pBtnSetting->setToolTip(QStringLiteral("设置"));
    m_pBtnSetting->setObjectName("btnAppSetting");

    pMainLayout->addWidget(m_pBtn[0]);
    pMainLayout->addWidget(m_pBtn[1]);
    pMainLayout->addWidget(m_pBtn[2]);
    pMainLayout->addStretch();
    pMainLayout->addWidget(m_pBtnSetting);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 1, 0, 0);
}

void LeftNavigatorBarWidget::initConnection()
{
    connect(m_pBtnSetting, SIGNAL(pressed()), this, SLOT(onBtnSettingClicked()));
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

void LeftNavigatorBarWidget::onBtnSettingClicked()
{
    QMenu *pSettingMenu = new QMenu(this);
    pSettingMenu->setObjectName("leftSettingMenu");

    QAction *pActFeedBack = new QAction(QStringLiteral("意见反馈"));
    QAction *pActVerUpdate = new QAction(QStringLiteral("版本更新"));
    QAction *pActCommSetting = new QAction(QStringLiteral("通用设置"));

    connect(pActFeedBack, SIGNAL(triggered(bool)), this, SLOT(onActionFeedBack()));
    connect(pActVerUpdate, SIGNAL(triggered(bool)), this, SLOT(onActionVerUpdate()));
    connect(pActCommSetting, SIGNAL(triggered(bool)), this, SLOT(onActionCommSetting()));

    pSettingMenu->addAction(pActFeedBack);
    pSettingMenu->addAction(pActVerUpdate);
    pSettingMenu->addAction(pActCommSetting);

    QPoint settingPos = mapToGlobal(m_pBtnSetting->pos());
    int x = settingPos.x() + m_pBtnSetting->width();
    int y = settingPos.y() + (m_pBtnSetting->height() / 2) - 120; ///< 120 is menu height >
    QPoint pos = QPoint(x, y);
    pSettingMenu->exec(pos);
}

void LeftNavigatorBarWidget::onActionFeedBack()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onActionFeedBack"));
}

void LeftNavigatorBarWidget::onActionVerUpdate()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onActionVerUpdate"));
}

void LeftNavigatorBarWidget::onActionCommSetting()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onActionCommSetting"));
}
