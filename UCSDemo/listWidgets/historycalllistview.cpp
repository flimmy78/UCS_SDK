#include "historycalllistview.h"
#include "historycallitemdelegate.h"

HistoryCallListView::HistoryCallListView(QWidget *parent)
    : QListView(parent)
{
    setMouseTracking(true);
    setFixedWidth(250);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setFrameShadow(QFrame::Plain);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//    setContentsMargins(8, 8, 8, 8);

    m_pCallModel = new HistoryCallModel(this);
    this->setModel(m_pCallModel);

    HistoryCallItemDelegate *delegate = new HistoryCallItemDelegate(this);
    setItemDelegate(delegate);

    verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;margin: 0px 2px 0px 0px;}"
                                       "QScrollBar::handle{background:rgb(217,217,217);border-radius:4px;}"
                                       "QScrollBar::handle:hover{background: rgb(191,191,191);}"
                                       "QScrollBar::add-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                                       "QScrollBar::sub-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                                       "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:transparent;}");
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_clickItem(QModelIndex)));
}

void HistoryCallListView::updateData(QList<CallHistory *> pList)
{
    m_pCallModel->updateData();
}

void HistoryCallListView::contextMenuEvent(QContextMenuEvent *event)
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
        connect(pDelAct, SIGNAL(triggered()), this, SLOT(slot_deleteItem()));
        connect(pDialAct, SIGNAL(triggered()), this, SLOT(slot_dialItem()));

        pMenu->addAction(pDialAct);
        pMenu->addAction(pDelAct);
        pMenu->exec(QCursor::pos());
    }
}

void HistoryCallListView::slot_deleteItem()
{
    int row = this->currentIndex().row();
    if (row > -1)
    {
        m_pCallModel->deleteRow(row);
    }
}

void HistoryCallListView::slot_dialItem()
{
    int row = this->currentIndex().row();
    if (row > -1)
    {
        CallHistory* callItem = m_pCallModel->getRowData(row);
        qDebug() << "slot_dialItem item " << callItem->nickName;
    }
}

void HistoryCallListView::slot_clickItem(QModelIndex index)
{
    qDebug() << "row data " << index.data(Qt::UserRole + 1).toString();
}
