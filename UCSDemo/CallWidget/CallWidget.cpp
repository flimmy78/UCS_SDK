#include "CallWidget.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QVBoxLayout>

CallWidget::CallWidget(QWidget *parent)
    : BaseWidget(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setObjectName("CallWidget");

    initLayout();
    initConnections();

    // test
    updateListData();
}

void CallWidget::initLayout()
{
    m_pTitleBar = new TopWidget(this);
    m_pSearchEdit = new SearchLineEdit(this);
    m_pDialWidget = new MidRightDialStackWidget(this);
    m_pCallListWidget = new MidLeftCallListWidget(this);

    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    QVBoxLayout *pLeftLayout = new QVBoxLayout;
    QVBoxLayout *pRightLayout = new QVBoxLayout;
    QHBoxLayout *pLayout = new QHBoxLayout;

    pLayout->addWidget(m_pSearchEdit);
    pLayout->setContentsMargins(15, 20, 15, 10);
    pLeftLayout->addLayout(pLayout);

    pLeftLayout->addWidget(m_pCallListWidget);
    pLeftLayout->setSpacing(0);
    pLeftLayout->setContentsMargins(0, 0, 0, 0);

    pRightLayout->addWidget(m_pTitleBar);
    pRightLayout->addWidget(m_pDialWidget);
    pRightLayout->setSpacing(0);
    pRightLayout->setContentsMargins(0, 0, 0, 0);

    pMainLayout->addLayout(pLeftLayout);
    pMainLayout->addLayout(pRightLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
}

void CallWidget::initConnections()
{

}

void CallWidget::updateListData()
{
    for (int i = 0; i < 15; i++)
    {
        CallHistoryItem item;
        if (i % 2)
        {
            item.headerPath = ":/images/u57.png";
            item.nickName = QStringLiteral("小美");
            item.callTime = "2018/01/06";
            item.callType = kAudioCall;
        }
        else
        {
            item.headerPath = ":/images/u29.png";
            item.nickName = QStringLiteral("全球唯一云通讯能力提供商");
            item.callTime = "17:50";
            item.callType = kAudioConf;
        }

        m_pCallListWidget->addCallListItem(&item);
    }
}

