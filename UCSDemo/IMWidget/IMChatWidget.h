#ifndef MESSAGECHATSTACKWIDGET_H
#define MESSAGECHATSTACKWIDGET_H

#include "common/qtheaders.h"
#include "absFiles/myPushButton.h"
#include "absFiles/myChatTextEdit.h"
#include "absFiles/myToolButton.h"
#include "BaseWidget.h"
#include "Interface/UCSIMSDKPublic.h"


class IMChatWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit IMChatWidget(QWidget *parent = 0);
    void updateMessages(QList<UCSMessage*> pMessageList);

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
    MyChatTextEdit m_txtSending;
    MyToolButton m_btnSend;
    QAction *m_pAct[2];
    QString m_targetId;
    quint32 m_type;
};

#endif // MESSAGECHATSTACKWIDGET_H
