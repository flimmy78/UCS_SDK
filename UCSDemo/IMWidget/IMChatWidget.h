#ifndef MESSAGECHATSTACKWIDGET_H
#define MESSAGECHATSTACKWIDGET_H

#include "common/qtheaders.h"
#include "absFiles/myPushButton.h"
#include "absFiles/myChatTextEdit.h"
#include "absFiles/myToolButton.h"
#include "BaseWidget.h"
#include "ChatWebView.h"

class QWebEngineView;
class IMChatWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit IMChatWidget(QWidget *parent = 0);
    ~IMChatWidget();

    void doSyncMessages(QMap<QString, qint32> messageCount);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void initLayout();
    void initConnections();
    void initTBtnMenu();

    void saveSetting();
    void readSetting();

signals:
    void sendingNewMsg();

public slots:
    void onChangeConversation(QString targetId, quint32 type);
    void onConversationDeleted();


private slots:
    void onSendingMsg();
    void onUpdateSendAction(QAction* pAction);


private:
    ChatWebView *m_pChatWebView;
    MyChatTextEdit m_txtSending;
    MyToolButton m_btnSend;
    QAction *m_pAct[2];
    QString m_conversationId;
    quint32 m_conversationType;

    QMap<QString, QString> m_conversationContent;
};

#endif // MESSAGECHATSTACKWIDGET_H
