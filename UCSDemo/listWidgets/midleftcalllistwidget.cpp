#include "midLeftCallListWidget.h"

CallListRowWidget::CallListRowWidget(QString headerPath,
                                           QString nameTxt,
                                           QString timeTxt,
                                           int callType,
                                           QWidget *parent)
    : QWidget(parent)
    , m_LblHeaderIcon(this)
    , m_LblName(this)
    , m_LblTime(this)
    , m_LblType(this)
    , m_LblTypeIcon(this)
{
    setFixedSize(250, 60);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setCursor(Qt::ArrowCursor);

    m_LblHeaderIcon.setPixmap(QPixmap(headerPath));
    m_LblHeaderIcon.setFixedSize(50, 50);
    m_LblType.setFixedWidth(60);

    QString strName = m_LblName.fontMetrics().elidedText(nameTxt,
                                                         Qt::ElideRight,
                                                         100,
                                                         Qt::TextShowMnemonic);
    m_LblName.setText(strName);
    m_LblTime.setText(timeTxt);

    QString style1 = "QLabel{font: 11 微软雅黑; color: black;}";
    QString style2 = "QLabel{font: 10 微软雅黑; color: rgb(158, 158, 158);}";
    m_LblName.setStyleSheet(style1);
    m_LblTime.setStyleSheet(style2);
    m_LblType.setStyleSheet(style2);

    switch (callType) {
    case kAudioCall:
    case kVideoCall:
    {
        m_LblType.setText(QStringLiteral("电话"));
        m_LblTypeIcon.setPixmap(QPixmap(":/images/u73.png"));
    }
        break;

    case kAudioConf:
    case kVideoConf:
    {
        m_LblType.setText(QStringLiteral("电话会议"));
        m_LblTypeIcon.setPixmap(QPixmap(":/images/u73.png"));
    }
        break;

    default:
        break;
    }

    initLayout();
}

void CallListRowWidget::initLayout()
{
    QHBoxLayout *pMainLayout = new QHBoxLayout;
    QHBoxLayout *pLeftLayout = new QHBoxLayout;
    QVBoxLayout *pRightLayout = new QVBoxLayout;
    QHBoxLayout *pTopLayout = new QHBoxLayout;
    QHBoxLayout *pBottomLayout = new QHBoxLayout;

    pLeftLayout->addWidget(&m_LblHeaderIcon);
    pLeftLayout->setSpacing(0);
    int margin = (this->height() - m_LblHeaderIcon.height()) / 2;
    pLeftLayout->setContentsMargins(margin, margin, margin, margin);

    pTopLayout->addWidget(&m_LblName);
    pTopLayout->addSpacerItem(new QSpacerItem(20, this->height() / 2, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    pTopLayout->addWidget(&m_LblTime);
    pTopLayout->setSpacing(0);
    pTopLayout->setContentsMargins(10, 0, 10, 0);

    pBottomLayout->addWidget(&m_LblType);
//    pBottomLayout->setSpacing(10);
//    pBottomLayout->addSpacerItem(new QSpacerItem(20, 30, QSizePolicy::Fixed, QSizePolicy::Fixed));
    pBottomLayout->addWidget(&m_LblTypeIcon);
    pBottomLayout->addSpacerItem(new QSpacerItem(20, this->height() / 2, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    pBottomLayout->setContentsMargins(10, 0, 0, 0);

    pRightLayout->addLayout(pTopLayout);
    pRightLayout->addLayout(pBottomLayout);
    pRightLayout->setSpacing(0);

    pMainLayout->addLayout(pLeftLayout);
    pMainLayout->addLayout(pRightLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

MidLeftCallListWidget::MidLeftCallListWidget(QWidget *parent)
    :QListWidget(parent)
{
    setMouseTracking(true);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    setFocusPolicy(Qt::NoFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    /* 去除底部多出的空白行 */
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    setObjectName("CallListWidget");

#if 0
    setStyleSheet("QListWidget{background:white; border:none;}"
                  "QListWidget::item{border:none; height:60px;}"
                  "QListWidget::item::hover{background:rgb(252,240,193);}"
                  "QListWidget::item::selected{background:rgb(252,233,161);}");
#endif

//    init();
    initConnection();
}

void MidLeftCallListWidget::init()
{
    for (int i = 0; i < 10; i++)
    {
        QListWidgetItem *item = new QListWidgetItem(this);
        CallListRowWidget* pCall = new CallListRowWidget(":/images/mainleft/u6.png", QStringLiteral("川流不息"), "2018/01/06", kAudioCall);

        item->setSizeHint(QSize(this->width(), 60));
        setItemWidget(item, pCall);
        addItem(item);
    }
}

void MidLeftCallListWidget::addCallListItem(CallHistoryItem *pItem)
{
    QListWidgetItem *item = new QListWidgetItem(this);
    CallListRowWidget* pCall = new CallListRowWidget(pItem->headerPath,
                                                           pItem->nickName,
                                                           pItem->callTime,
                                                           pItem->callType,
                                                           this);

    item->setSizeHint(QSize(this->width(), pCall->height()));
    setItemWidget(item, pCall);
    addItem(item);
}

void MidLeftCallListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    int row = this->indexAt(event->pos()).row();
    if (row > -1)
    {
        QMenu * pMenu = new QMenu(this);
        pMenu->setFixedWidth(120);
//        pMenu->setStyleSheet("QMenu{background-color:white;}");
        pMenu->setContentsMargins(0, 0, 0, 0);

        QAction *pDelAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("删除"), pMenu);
        QAction *pDialAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("拨打"), pMenu);
        connect(pDelAct, SIGNAL(triggered()), this, SLOT(slot_removeRow()));
        connect(pDialAct, SIGNAL(triggered()), this, SLOT(slot_dialItem()));

        pMenu->addAction(pDialAct);
        pMenu->addAction(pDelAct);

        pMenu->exec(QCursor::pos());
    }
}

void MidLeftCallListWidget::initConnection()
{
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_itemClicked(QListWidgetItem*)));
}

void MidLeftCallListWidget::slot_addItem(CallHistoryItem *pItem)
{
    QListWidgetItem *item = new QListWidgetItem(this);
    CallListRowWidget* pCall = new CallListRowWidget(pItem->headerPath,
                                                           pItem->nickName,
                                                           pItem->callTime,
                                                           pItem->callType);

    item->setSizeHint(QSize(this->width(), 60));
    setItemWidget(item, pCall);
    addItem(item);
}

void MidLeftCallListWidget::slot_itemClicked(QListWidgetItem *item)
{
    qDebug() << "slot_ItemClicked " << this->row(item);
    qDebug() << "currentindex " << currentIndex().row();
    emit sig_itemClicked(this->row(item));
}

void MidLeftCallListWidget::slot_removeRow()
{
    int row = currentIndex().row();
    qDebug() << "slot_removeRow " << row;

    if (row > -1)
    {
        QListWidgetItem* pItem =  QListWidget::takeItem(row);
        this->removeItemWidget(pItem);
        delete pItem;
    }
}

void MidLeftCallListWidget::slot_dialItem()
{
    int row = currentIndex().row();

    qDebug() << "slot_dialItem " << row;

}


