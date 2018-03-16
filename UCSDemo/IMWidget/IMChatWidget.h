#ifndef MESSAGECHATSTACKWIDGET_H
#define MESSAGECHATSTACKWIDGET_H

#include "common/qtheaders.h"
#include <QStackedLayout>
#include "absFiles/myPushButton.h"
#include "absFiles/myChatTextEdit.h"
#include "absFiles/myToolButton.h"
#include "BaseWidget.h"
#include "ChatWebView.h"

class QWebEngineView;
class QWebChannel;
class QWebEnginePage;

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
    void initMisc();
    void initLayout();
    void initConnections();
    void initTBtnMenu();

    void saveSetting();
    void readSetting();

    void createChat();
    void changeChat();
    void showMessages(int count = 20);

signals:
    void sendingNewMsg();

public slots:
    void onChangeConversation(QString targetId, quint32 type);
    void onConversationDeleted(QString targetId, quint32 type);


private slots:
    void onSendingMsg();
    void onUpdateSendAction(QAction* pAction);
    void urlChanged(const QUrl &url);
    void onLoadFinished();

private:
    MyChatTextEdit m_txtSending;
    MyToolButton m_btnSend;
    QAction *m_pAct[2];
    QString m_conversationId;
    quint32 m_conversationType;

    QMap<QString, ChatWebView*> m_chatsMap;
    QWebChannel *m_pWebChannel;
    QStackedLayout *m_pStackedLayout;
    QList<qint64> m_msgIdList;
};

#endif // MESSAGECHATSTACKWIDGET_H
