#ifndef CONVERSATIONLISTWIDGET_H
#define CONVERSATIONLISTWIDGET_H

#include <QObject>
#include <QContextMenuEvent>
#include <QListView>
#include <qglobal.h>
#include "ConversationItemModel.h"

class ConversationListView : public QListView
{
    Q_OBJECT

public:
    explicit ConversationListView(QWidget *parent = 0);
    ~ConversationListView();

    void updateConversationList();

private:
    void initConnections();

    void contextMenuEvent(QContextMenuEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void itemClicked(QString, quint32);

private slots:
    void onClickedItem(QModelIndex index);
    void onDeleteItem();
    void onSetTopItem();
    void onModifyItem();

private:
    ConversationItemModel *m_pModel;
    QPoint m_eventPos;
};

#endif // CONVERSATIONLISTWIDGET_H
