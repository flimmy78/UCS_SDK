#include "MidLeftConversationListWidget.h"

MidLeftIMRowWidget::MidLeftIMRowWidget(IMConversationItem *pItem, QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(250, 60);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setCursor(Qt::ArrowCursor);

    m_LblHeaderIcon.setFixedSize(50, 50);
    m_LblHeaderIcon.setPixmap(QPixmap(pItem->headerPath));

    QString strName = m_LblName.fontMetrics().elidedText(pItem->conversationTitle,
                                                         Qt::ElideRight,
                                                         100,
                                                         Qt::TextShowMnemonic);

    QString strData = m_LblContent.fontMetrics().elidedText(pItem->lastMsgContent,
                                                         Qt::ElideRight,
                                                         150,
                                                         Qt::TextShowMnemonic);

    m_LblName.setText(strName);
    m_LblTime.setText(pItem->lastMsgTime);
    m_LblContent.setText(strData);
    m_LblUnReadCount.setText(QString::number(pItem->unReadCount));

    QString style1 = "QLabel{font: 11 微软雅黑; color: black;}";
    QString style2 = "QLabel{font: 10 微软雅黑; color: rgb(158, 158, 158);}";

    m_LblName.setStyleSheet(style1);
    m_LblTime.setStyleSheet(style2);
    m_LblContent.setStyleSheet(style2);

    initLayout();
}

void MidLeftIMRowWidget::initLayout()
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

    pBottomLayout->addWidget(&m_LblContent);
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

MidLeftConversationListWidget::MidLeftConversationListWidget(QWidget *parent)
    :QListWidget(parent)
{
    setMouseTracking(true);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    setFocusPolicy(Qt::NoFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setSelectionMode(QAbstractItemView::SingleSelection);

    verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;margin: 0px 2px 0px 0px;}"
                                        "QScrollBar::handle{background:rgb(217,217,217);border-radius:4px;}"
                                        "QScrollBar::handle:hover{background: rgb(191,191,191);}"
                                        "QScrollBar::add-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                                        "QScrollBar::sub-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:transparent;}");

    /* 去除底部多出的空白行 */
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    initConnection();
}

void MidLeftConversationListWidget::addListItem(IMConversationItem *pItem)
{
    QListWidgetItem *item = new QListWidgetItem(this);
    MidLeftIMRowWidget *pMsg = new MidLeftIMRowWidget(pItem, this);
    item->setSizeHint(QSize(this->width(), 60));
    setItemWidget(item, pMsg);
    addItem(item);
}

void MidLeftConversationListWidget::contextMenuEvent(QContextMenuEvent * event)
{
    int row = this->indexAt(event->pos()).row();
    if (row > -1)
    {
        QMenu * pMenu = new QMenu(this);
        pMenu->setFixedWidth(120);
//        pMenu->setStyleSheet("QMenu{background-color:white;}");
        pMenu->setContentsMargins(0, 0, 0, 0);

        QAction *pDelAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("删除聊天"), pMenu);
        QAction *pTopAct = new QAction(QIcon(":/images/btn_del.png"), QStringLiteral("置顶"), pMenu);
        connect(pDelAct, SIGNAL(triggered()), this, SLOT(slot_removeRow()));
        connect(pTopAct, SIGNAL(triggered()), this, SLOT(slot_setTopRow()));

        pMenu->addAction(pTopAct);
        pMenu->addSeparator();
        pMenu->addAction(pDelAct);

        pMenu->exec(QCursor::pos());
    }
}

void MidLeftConversationListWidget::initConnection()
{
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_itemCliecked(QListWidgetItem*)));
}

void MidLeftConversationListWidget::slot_itemCliecked(QListWidgetItem *pItem)
{
    qDebug() << "slot_itemCliecked " << currentIndex().row();
    emit sig_itemClicked(this->row(pItem));
}

void MidLeftConversationListWidget::slot_addItem(IMConversationItem *pItem)
{

}

void MidLeftConversationListWidget::slot_removeRow()
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

void MidLeftConversationListWidget::slot_setTopRow()
{
    int row = currentIndex().row();
    qDebug() << "slot_setTopRow " << row;
}
