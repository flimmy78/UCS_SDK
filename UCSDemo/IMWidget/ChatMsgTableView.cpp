#include "ChatMsgTableView.h"
#include <QHeaderView>
#include "ChatMsgTableDelegate.h"

ChatMsgTableView::ChatMsgTableView(QWidget *parent)
    : QTableView(parent)
{
    setMouseTracking(true);
    setSelectionMode(QAbstractItemView::NoSelection);
    setShowGrid(false);
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setFocusPolicy(Qt::NoFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    horizontalHeader()->hide();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    resizeColumnsToContents();
    resizeRowsToContents();

    setObjectName("ChatMsgTableView");

    m_pTableModel = new ChatMsgTableModel(this);
    this->setModel(m_pTableModel);

    ChatMsgTableDelegate *delegate = new ChatMsgTableDelegate(this);
    setItemDelegate(delegate);
}

ChatMsgTableView::~ChatMsgTableView()
{

}
