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

//    initMenu();
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

        if (i % 7 == 1)
        {
            item.headerPath = ":/images/u57.png";
            item.nickName = QStringLiteral("小美");
            item.callTime = "2018/01/06";
            item.callType = QStringLiteral("网络电话");
            item.typeIconPath = ":/Resources/Call/call_audio_in.png";
        }
        else if (i % 7 == 2)
        {
            item.headerPath = ":/images/u57.png";
            item.nickName = QStringLiteral("小美");
            item.callTime = "2018/01/06";
            item.callType = QStringLiteral("网络电话");
            item.typeIconPath = ":/Resources/Call/call_audio_fail.png";
        }
        else if (i % 7 == 3)
        {
            item.headerPath = ":/images/u57.png";
            item.nickName = QStringLiteral("小美");
            item.callTime = "2018/01/06";
            item.callType = QStringLiteral("高清电话");
            item.typeIconPath = ":/Resources/Call/call_audio_in.png";
        }
        else if (i % 7 == 4)
        {
            item.headerPath = ":/images/u57.png";
            item.nickName = QStringLiteral("小美");
            item.callTime = "2018/01/06";
            item.callType = QStringLiteral("高清电话");
            item.typeIconPath = ":/Resources/Call/call_audio_fail.png";
        }
        else if (i % 7 == 5)
        {
            item.headerPath = ":/images/u57.png";
            item.nickName = QStringLiteral("小美");
            item.callTime = "2018/01/06";
            item.callType = QStringLiteral("视频");
            item.typeIconPath = ":/Resources/Call/call_video_in.png";
        }
        else if (i % 7 == 6)
        {
            item.headerPath = ":/images/u57.png";
            item.nickName = QStringLiteral("小美");
            item.callTime = "2018/01/06";
            item.callType = QStringLiteral("视频");
            item.typeIconPath = ":/Resources/Call/call_video_fail.png";
        }
        else
        {
            item.headerPath = ":/Resources/Headers/header_conf.png";
            item.nickName = QStringLiteral("张修路,冯海波...");
            item.callTime = "17:50";
            item.callType = QStringLiteral("电话会议");
            item.typeIconPath = ":/Resources/Call/call_conf.png";
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
        pMenu->setObjectName("CallListMenu");

        QAction *pActDel = new QAction(QStringLiteral("删除"));
        QAction *pActAudio = new QAction(QStringLiteral("网络电话"));
        QAction *pActVideo = new QAction(QStringLiteral("视频"));
        QAction *pActDirect = new QAction(QStringLiteral("高清电话"));
        QAction *pActIM = new QAction(QStringLiteral("消息"));
        QAction *pActConf = new QAction(QStringLiteral("电话会议"));

        connect(pActDel, SIGNAL(triggered()), this, SLOT(onDeleteItemAction()));
        connect(pActAudio, SIGNAL(triggered()), this, SLOT(onAudioCallItemAction()));
        connect(pActVideo, SIGNAL(triggered()), this, SLOT(onVideoCallItemAction()));
        connect(pActDirect, SIGNAL(triggered()), this, SLOT(onDirectCallItemAction()));
        connect(pActIM, SIGNAL(triggered()), this, SLOT(onIMMsgItemAction()));
        connect(pActConf, SIGNAL(triggered()), SLOT(onMeetingItemAction()));

        pMenu->addAction(pActAudio);
        pMenu->addAction(pActVideo);
        pMenu->addAction(pActDirect);
        pMenu->addAction(pActIM);
        pMenu->addAction(pActConf);
        pMenu->addAction(pActDel);

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
