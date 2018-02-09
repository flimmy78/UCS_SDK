#ifndef MESSAGECHATSTACKWIDGET_H
#define MESSAGECHATSTACKWIDGET_H

#include "common/qtheaders.h"
#include "absFiles/myPushButton.h"
#include "absFiles/myChatTextEdit.h"
#include "absFiles/myToolButton.h"

class MidRightChatStackWidget : public QWidget
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
    void slot_sendingMsg();
    void slot_updateAction(QAction* pAction);

private:
    MyChatTextEdit m_txtSending;
    MyToolButton m_btnSend;
    QAction *m_pAct[2];
};

#endif // MESSAGECHATSTACKWIDGET_H
