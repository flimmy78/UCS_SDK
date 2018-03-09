﻿#ifndef IMWIDGET_H
#define IMWIDGET_H

#include <QStackedLayout>
#include <QWidget>
#include <qglobal.h>
#include "BaseWidget.h"
#include "ConversationListView.h"
#include "IMChatWidget.h"
#include "topWidget.h"
#include "searchLineEdit.h"

class IMWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit IMWidget(QWidget *parent = 0);

    ConversationListView *conversationListView() const;

    IMChatWidget *msgChatWidget() const;

    void receivedMessages(QMap<QString, qint32> messageCount);

private:
    void initLayout();
    void initConnections();

signals:

public slots:
    void onPageSwitch();

private slots:
    void onConversationItemClicked(QString targetId, quint32 type);

private:
    TopWidget *m_pTitleBar;
    SearchLineEdit *m_pSearchEdit;
    ConversationListView *m_pConversationListView;
    IMChatWidget *m_pMsgChatWidget;
    QStackedLayout *m_pStackedLayout;
};

#endif // IMWIDGET_H
