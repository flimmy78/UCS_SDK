#include "CallListView.h"
#include "CallListItemDelegate.h"

CallListView::CallListView(QWidget *parent)
    : QListView(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setCursor(Qt::ArrowCursor);
    setResizeMode(QListView::Adjust);
    setUniformItemSizes(true);

    setObjectName("CallListView");

    m_pCallModel = new CallListItemModel(this);
    this->setModel(m_pCallModel);
    m_pCallModel->setModelData(&m_callList);

    CallListItemDelegate *delegate = new CallListItemDelegate(this);
    setItemDelegate(delegate);

    initConnections();

    // test
    updateListData();
}

void CallListView::updateListData()
{
    for (int i = 0; i < 15; i++)
    {
        CallListItem item;
        item.callId = i;

        if (i % 2)
        {
            item.headerPath = ":/images/u57.png";
            item.nickName = QStringLiteral("小美");
            item.callTime = "2018/01/06";
            item.callType = QStringLiteral("电话");
            item.typeIconPath = ":/images/u73.png";
        }
        else
        {
            item.headerPath = ":/Resources/Headers/header_conf.png";
            item.nickName = QStringLiteral("全球云通讯能力提供商");
            item.callTime = "17:50";
            item.callType = QStringLiteral("电话会议");
            item.typeIconPath = ":/images/u73.png";
        }
        m_callList.append(item);
    }

    m_pCallModel->refreshModel();
}

void CallListView::contextMenuEvent(QContextMenuEvent *event)
{
    int row = this->indexAt(event->pos()).row();
    if (row > -1)
    {
        m_eventPos = event->pos();

        QMenu * pMenu = new QMenu(this);

        QAction *pDelAct = new QAction(QStringLiteral("删除"), pMenu);
        QAction *pAudioCallAct = new QAction(QStringLiteral("网络电话"), pMenu);
        QAction *pVideoCallAct = new QAction(QStringLiteral("视频"), pMenu);
        QAction *pDirectCallAct = new QAction(QStringLiteral("高清电话"), pMenu);
        QAction *pIMAct = new QAction(QStringLiteral("消息"), pMenu);
        QAction *pMeetingAct = new QAction(QStringLiteral("电话会议"), pMenu);

        connect(pDelAct, SIGNAL(triggered()), this, SLOT(onDeleteItemAction()));
        connect(pAudioCallAct, SIGNAL(triggered()), this, SLOT(onAudioCallItemAction()));
        connect(pVideoCallAct, SIGNAL(triggered()), this, SLOT(onVideoCallItemAction()));
        connect(pDirectCallAct, SIGNAL(triggered()), this, SLOT(onDirectCallItemAction()));
        connect(pIMAct, SIGNAL(triggered()), this, SLOT(onIMMsgItemAction()));
        connect(pMeetingAct, SIGNAL(triggered()), SLOT(onMeetingItemAction()));

        pMenu->addAction(pAudioCallAct);
        pMenu->addAction(pVideoCallAct);
        pMenu->addAction(pDirectCallAct);
        pMenu->addAction(pIMAct);
        pMenu->addAction(pMeetingAct);
        pMenu->addAction(pDelAct);
        pMenu->exec(QCursor::pos());
    }
}

void CallListView::initConnections()
{
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(onItemClicked(QModelIndex)));
}

void CallListView::onDeleteItemAction()
{
    QModelIndex index = indexAt(m_eventPos);
    if (index.isValid())
    {
        m_callList.removeAt(index.row());
        m_pCallModel->refreshModel();
    }
}

void CallListView::onAudioCallItemAction()
{
    QModelIndex index = indexAt(m_eventPos);
    if (index.isValid())
    {
        qDebug() << "onDialItemAction: " << index.data(Qt::UserRole + 1).toString();
    }
}

void CallListView::onVideoCallItemAction()
{

}

void CallListView::onDirectCallItemAction()
{

}

void CallListView::onIMMsgItemAction()
{

}

void CallListView::onMeetingItemAction()
{

}

void CallListView::onItemClicked(QModelIndex index)
{
    qDebug() << "onItemClicked: " << index.data(Qt::UserRole + 1).toString();
}
