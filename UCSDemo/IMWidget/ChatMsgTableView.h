#ifndef CHATMSGTABLEVIEW_H
#define CHATMSGTABLEVIEW_H

#include <QTableView>
#include <QWidget>
#include <qglobal.h>
#include "ChatMsgTableModel.h"

class ChatMsgTableView : public QTableView
{
    Q_OBJECT

public:
    explicit ChatMsgTableView(QWidget *parent = 0);
    ~ChatMsgTableView();


protected:

private:
    void initList();

private:
    ChatMsgTableModel *m_pTableModel;
    QList<ChatMsgItem> m_msgList;
};

#endif // CHATMSGTABLEVIEW_H
