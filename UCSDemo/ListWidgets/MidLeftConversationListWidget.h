#ifndef MIDLEFTIMLISTWIDGET_H
#define MIDLEFTIMLISTWIDGET_H

#include "common/qtheaders.h"

/*!
 * \brief IM聊天会话项
*/
typedef struct
{
    QString conversationId;     ///< 会话Id >
    QString headerPath;         ///< 会话图像 >
    QString conversationTitle;   ///< 会话名称 >
    QString lastMsgTime;        ///< 会话最后一条消息时间 >
    QString lastMsgContent;     ///< 会话最后一条消息内容 >
    quint32 unReadCount;        ///< 会话未读消息数 >
} IMConversationItem;

class MidLeftIMRowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MidLeftIMRowWidget(IMConversationItem *pItem, QWidget *parent = 0);

private:
    void initLayout();

private:
    QLabel m_LblHeaderIcon;
    QLabel m_LblName;
    QLabel m_LblTime;
    QLabel m_LblContent;
    QLabel m_LblUnReadCount;
};

class MidLeftConversationListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit MidLeftConversationListWidget(QWidget *parent = 0);
    void addListItem(IMConversationItem *pItem);
    void clearAllItems();

protected:
    void contextMenuEvent(QContextMenuEvent *);

private:
    void initConnection();

signals:
    void sig_itemClicked(int row);

public slots:
    void slot_itemCliecked(QListWidgetItem *pItem);
    void slot_addItem(IMConversationItem *pItem);
    void slot_removeRow();
    void slot_setTopRow();

private:

};

#endif // MIDLEFTIMLISTWIDGET_H
