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
    void enterConversation(QString targetId, QString name);

private:
    void initConnections();

    void contextMenuEvent(QContextMenuEvent *event) override;

signals:
    void itemClicked(ConversationItem);
    void itemDeleted(ConversationItem);

public slots:
    void onUpdateData();

private slots:
    void onClickedItem(QModelIndex index);
    void onDeleteItemAction();
    void onSetTopItemAction();
    void onModifyItemAction();

private:
    ConversationItemModel *m_pModel;
    QPoint m_eventPos;
    QList<ConversationItem> m_conversationList;
};

#endif // CONVERSATIONLISTWIDGET_H
