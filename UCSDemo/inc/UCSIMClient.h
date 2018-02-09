#ifndef UCSIMCLIENT_H
#define UCSIMCLIENT_H

#include "ucsimlib_global.h"
#include <QObject>
#include <QSharedPointer>
#include <QMutex>
#include <QString>
#include <QEvent>

class UCSIMManager;

class UCSIMLIBSHARED_EXPORT UCSIMClient : public QObject
{
    Q_OBJECT
public:
    static QSharedPointer<UCSIMClient> &Instance();
    virtual ~UCSIMClient();

    /*!
     * 初始化
    */
    void init();

    QString version();

    /* 发送即时消息 */
    void sendMessage();

    /* 主动下载短语音消息 */
    void downloadShortVoice();

    /* 创建讨论组 */
    void createDiscussion();

    /* 添加新成员到讨论组 */
    void addMemberToDiscussion();

    /* 从讨论组移除指定成员 */
    void removeMemberFromDiscussion();

    /* 成员主动退出讨论组 */
    void quitDiscussion();

    /* 修改讨论组名称 */
    void setDiscussionTopic();

    /* 获取讨论组列表 */
    QStringList getDiscussionList();

    /* 获取会话列表 */
    QStringList getConversationList();

    /* 在会话列表中移除一个会话，但是不会删除该会话在数据库中的消息。 */
    bool removeConversation();

    /* 清空某一个会话所有的聊天记录 */
    bool clearMessages();

    /* 清空会话列表 */
    bool clearConversations();

    /* 获取最新的消息.用于进入聊天界面后第一次加载消息 */
    QStringList getLatestMessages();

    /* 获取更多的消息。用于聊天界面中加载更多消息。 */
    QStringList getHistoryMessages();

    /* 删除一条指定的消息 */
    bool deleteMessages();

    /* 获取某个会话的未读消息数 */
    quint32 getUnreadCount();

    /* 获取所有会话的未读消息总数 */
    quint32 getTotalUnreadCount();

    /* 清空某个会话的未读消息总数 */
    bool clearConversationsUnreadCount();

    /* 根据关键字查询某个会话的聊天记录 */
    QStringList checkChatMessages();

    /* 会话列表中,设置某条会话的置顶状态 */
    bool setConversationToTop();

protected:
    void customEvent(QEvent *event) override;

private:
    explicit UCSIMClient(QObject *parent = Q_NULLPTR);

    /* do nothing, ensure single instance */
    UCSIMClient(const UCSIMClient &);
    UCSIMClient& operator =(const UCSIMClient&);

    void initConnections();

signals:
    void sig_msgSendSuccess(quint32 messageId);
    void sig_msgSendFailed(quint32 messageId);

public slots:

private:
    static QSharedPointer<UCSIMClient> m_pInstance;
    static QMutex m_Mutex;

    UCSIMManager *m_pIManager;
};

#endif // UCSIMCLIENT_H
