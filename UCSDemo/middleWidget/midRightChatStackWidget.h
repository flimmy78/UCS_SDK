#ifndef MESSAGECHATSTACKWIDGET_H
#define MESSAGECHATSTACKWIDGET_H

#include "common/qtheaders.h"
#include "absFiles/myPushButton.h"
#include "absFiles/myChatTextEdit.h"
#include "absFiles/myToolButton.h"
#include "BaseWidget.h"

class MidRightChatStackWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit MidRightChatStackWidget(QWidget *parent = 0);

private:
    void initLayout();
    void initConnections();
    void initTBtnMenu();

    void saveSetting();
    void readSetting();

signals:

public slots:
    void onChangeConversation(QString targetId, quint32 type);

private slots:
    void onSendingMsg();
    void onUpdateSendAction(QAction* pAction);


private:
    MyChatTextEdit m_txtSending;
    MyToolButton m_btnSend;
    QAction *m_pAct[2];
};

#endif // MESSAGECHATSTACKWIDGET_H
