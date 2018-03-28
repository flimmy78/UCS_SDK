#ifndef MESSAGECHATSTACKWIDGET_H
#define MESSAGECHATSTACKWIDGET_H

#include "common/qtheaders.h"
#include <QStackedLayout>
#include "myPushButton.h"
#include "ChatTextEdit.h"
#include "myToolButton.h"
#include "BaseWidget.h"
#include "ChatWebView.h"
#include "IMCommonDefine.h"

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

    void createChat();
    void changeChat();
    void showMessages(int count = 20);

signals:
    void sigSendingMsg();

public slots:
    void onChangeConversation(ConversationItem conversation);
    void onConversationDeleted(ConversationItem conversation);


private slots:
    void onSendingMsg();
    void urlChanged(const QUrl &url);
    void onLoadFinished();
    void onOpenEmojiPanel();
    void onSendImage();
    void onClearMessage();
    void onStartConference();
    void onStartDirectCall();
    void onStartAudioCall();
    void onStartVideoCall();

private:
    ConversationItem m_conversation;
    QMap<QString, ChatWebView*> m_chatsMap;
    QWebChannel *m_pWebChannel;
    QStackedLayout *m_pStackedLayout;
    QList<qint64> m_msgIdList;

    ChatTextEdit *m_pTxtSending;
    MyPushButton *m_pBtnSend;
    MyPushButton *m_pBtnEmoji;
    MyPushButton *m_pBtnClear;
    MyPushButton *m_pBtnImage;
    MyPushButton *m_pBtnConf;
    MyPushButton *m_pBtnDirect;
    MyPushButton *m_pBtnAudio;
    MyPushButton *m_pBtnVideo;
};

#endif // MESSAGECHATSTACKWIDGET_H
