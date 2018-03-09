#include "IMWidget.h"
#include "Interface/UCSLogger.h"
#include "EmptyWidget.h"
#define TAG "IMWidget"

IMWidget::IMWidget(QWidget *parent)
    : BaseWidget(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setObjectName("IMWidget");

    initLayout();
    initConnections();
}

void IMWidget::initLayout()
{
    m_pTitleBar = new TopWidget(this);
    m_pSearchEdit = new SearchLineEdit(this);
    m_pConversationListView = new ConversationListView(this);
    m_pMsgChatWidget = new IMChatWidget(this);

    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    QVBoxLayout *pLeftLayout = new QVBoxLayout;
    QVBoxLayout *pRightLayout = new QVBoxLayout;
    QHBoxLayout *pSearchLayout = new QHBoxLayout;
    m_pStackedLayout = new QStackedLayout;

    pSearchLayout->addWidget(m_pSearchEdit);
    pSearchLayout->setContentsMargins(15, 20, 15, 10);

    pLeftLayout->addLayout(pSearchLayout);
    pLeftLayout->addWidget(m_pConversationListView);
    pLeftLayout->setSpacing(0);
    pLeftLayout->setContentsMargins(0, 0, 0, 0);

    EmptyWidget *emptyWidget = new EmptyWidget(this);
    m_pStackedLayout->addWidget(emptyWidget);
    m_pStackedLayout->addWidget(m_pMsgChatWidget);
    m_pStackedLayout->setSpacing(0);
    m_pStackedLayout->setContentsMargins(0, 0, 0, 0);

    pRightLayout->addWidget(m_pTitleBar);
    pRightLayout->addLayout(m_pStackedLayout);
    pRightLayout->setSpacing(0);
    pRightLayout->setContentsMargins(0, 0, 0, 0);

    pMainLayout->addLayout(pLeftLayout);
    pMainLayout->addLayout(pRightLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
}

void IMWidget::initConnections()
{
    connect(m_pConversationListView, SIGNAL(itemClicked(QString,quint32)), this, SLOT(onConversationItemClicked(QString,quint32)));
    connect(m_pConversationListView, SIGNAL(itemClicked(QString,quint32)), m_pMsgChatWidget, SLOT(onChangeConversation(QString,quint32)));
//    connect(m_pConversationListView, SIGNAL(itemDeleted()), m_pMsgChatWidget, SLOT(onConversationDeleted()));
    connect(m_pConversationListView, SIGNAL(itemDeleted()), this, SLOT(onPageSwitch()));
    connect(m_pMsgChatWidget, SIGNAL(sendingNewMsg()), m_pConversationListView, SLOT(onUpdateData()));
}

void IMWidget::onPageSwitch()
{
    m_pStackedLayout->setCurrentIndex(0);
    m_pTitleBar->setTitle("");
}

void IMWidget::onConversationItemClicked(QString targetId, quint32 type)
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onConversationItemClicked targetId: %1 type: %2")
            .arg(targetId).arg(type));

    m_pTitleBar->setTitle(targetId);
    m_pStackedLayout->setCurrentIndex(1);
}

IMChatWidget *IMWidget::msgChatWidget() const
{
    return m_pMsgChatWidget;
}

void IMWidget::receivedMessages(QMap<QString, qint32> messageCount)
{
    m_pConversationListView->updateConversationList();
    m_pMsgChatWidget->doSyncMessages(messageCount);
}

ConversationListView *IMWidget::conversationListView() const
{
    return m_pConversationListView;
}
