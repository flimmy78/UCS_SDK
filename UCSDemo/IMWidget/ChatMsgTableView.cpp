#include "ChatMsgTableView.h"
#include <QHeaderView>
#include <QScrollBar>
#include "ChatMsgTableDelegate.h"

ChatMsgTableView::ChatMsgTableView(QWidget *parent)
    : QTableView(parent)
{
//    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//    setSelectionMode(QAbstractItemView::NoSelection);
    setShowGrid(false);
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setFocusPolicy(Qt::NoFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);

    resizeColumnsToContents();
    resizeRowsToContents();

    setObjectName("ChatMsgTableView");

    ChatMsgTableDelegate *delegate = new ChatMsgTableDelegate(this);
    setItemDelegate(delegate);

    m_pTableModel = new ChatMsgTableModel(this);
    this->setModel(m_pTableModel);
    m_pTableModel->setModelData(&m_msgList);

    initList();
}

ChatMsgTableView::~ChatMsgTableView()
{

}

void ChatMsgTableView::initList()
{
    for (int i = 0; i < 15; i++)
    {
        ChatMsgItem item;
        item.content = "1111";
        m_msgList.append(item);
    }

    m_pTableModel->refreshModel();

//    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}
