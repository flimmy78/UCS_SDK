#ifndef MIDLEFTCALLLISTWIDGET_H
#define MIDLEFTCALLLISTWIDGET_H

#include "common/qtheaders.h"
#include "absFiles/myPushButton.h"

enum CallType
{
    kAudioCall = 0,
    kVideoCall = 1,
    kAudioConf = 2,
    kVideoConf = 3
};

typedef struct
{
    QString headerPath;
    QString nickName;
    QString callTime;
    CallType callType;
} CallHistoryItem;

class CallListRowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CallListRowWidget(QString headerPath,
                         QString nameTxt,
                         QString timeTxt,
                         int callType = 0,
                         QWidget *parent = 0);

private:
    void initLayout();

private:
    QLabel m_LblHeaderIcon;
    QLabel m_LblName;
    QLabel m_LblTime;
    QLabel m_LblType;
    QLabel m_LblTypeIcon;
};

class MidLeftCallListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit MidLeftCallListWidget(QWidget *parent = 0);
    void init();
    void addCallListItem(CallHistoryItem* pItem);

protected:
    void contextMenuEvent(QContextMenuEvent *);

private:
    void initConnection();

signals:
    void sig_itemClicked(int row);

public slots:
    void slot_addItem(CallHistoryItem *pItem);
    void slot_itemClicked(QListWidgetItem *pItem);
    void slot_removeRow();
    void slot_dialItem();

public:

private:

};

#endif // MIDLEFTCALLLISTWIDGET_H
