#ifndef UCSIMCLIENT_H
#define UCSIMCLIENT_H

#include "ucsimlib_global.h"
#include <QObject>
#include <QSharedPointer>
#include <QMutex>
#include <QString>
#include <QEvent>
#include "UCSMessage.h"
#include "UCSIMDefine.h"
#include "UCSUserInfo.h"
#include "UCSDiscussion.h"
#include "UCSConversation.h"

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

class UCSIMManager;

/*!
 * \brief UCSIMClient 是IMSDK的核心能力类，提供所有的IM能力。使用im能力必须导入tcp sdk。
 */
class UCSIMLIBSHARED_EXPORT UCSIMClient : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Instance 实例化UCSIMClient单例对象
     * \return UCSIMClient的单例对象
     */
    static QSharedPointer<UCSIMClient> &Instance();
    virtual ~UCSIMClient();

    /*!
     * \brief init 初始化
     */
    void init();

    /*!
     * \brief 版本号
     * \return 版本号
     */
    QString version();

    /*!
     * \brief setIMReceiver 注册IM相关事件接收者
     * \param receiver 事件接收者，需重载customEvent()函数
     */
    void setIMReceiver(QObject *receiver);

    /*!
     * \brief sendMessage 发送一条聊天消息
     * \details SDK所有的消息发送都使用这个方法，根据不同的参数来区分不同的消息类型。
     * \param pMessage 待发送的message, 需设置conversationType, receiveId, msgType和content，
     *                  其余字段发送成功后会自动进行补充。
     * \return true for success, else failed。
     */
     bool sendMessage(UCSMessage *pMessage);

    /*!
     * \brief downloadVoiceWithConversationType 手动下载消息的语音数据
     * \details 云之讯IM收到语音消息时首先会自动下载语音数据。在绝大部分的情况下,
     *          语音数据能够下载成功,但不排除因网络等因素带来的失败情况,这个时候开
     *          发者可以调用这个接口主动下载语音数据。一般在自动下载失败的情况下调用。
     * \param conversationType 会话类型
     * \param targetId 聊天对象的id。群组聊天时为群组id,讨论组聊天时为讨论组id。
     * \param messageId 消息id
     */
    bool downloadVoiceWithConversationType(IN UCS_IM_ConversationType conversationType,
                                           IN QString targetId,
                                           IN quint64 messageId);

    /*!
     * \brief createDiscussionWithTopic 主动创建一个讨论组
     * \details 这个方法用来主动创建一个讨论组。创建成功后，创建者会自动变成讨论组的所有者，拥有对讨论组操作的最高权限。
     * \param topic 创建后的讨论组主题(讨论组名),不能为空。
     * \param memberList 创建时需要邀请到讨论组的成员数组(不需要把自己加进去),数组中每个元素为UCSUserInfo类的对象。
     *                  加入到这个成员数组的每个成员必须保证是已经在云平台注册的成员，否知会失败。
     */
    bool createDiscussionWithTopic(IN QString topic, IN QList<UCSUserInfo> memberList);

    /*!
     * \brief addMemberToDiscussionWithDiscussionId 给已经存在的讨论组添加新的用户
     * \details 这个方法用来给一个已经存在的讨论组添加新的成员，如果用户无权限进行操作会失败。
     * \param discussionId 讨论组id
     * \param memberList 要新加的成员的数组，数组中的每个元素为UCSUserInfo类的对象。
     */
    bool addMemberToDiscussionWithDiscussionId(IN QString discussionId, IN QList<UCSUserInfo> memberList);

    /*!
     * \brief removeMemberFromDiscussionWithDiscussionId 在讨论组中移除已经存在的用户
     * \details 这个方法用来删除一个存在的讨论组中存在的成员,如果用户无权限进行操作会失败。
     * \param discussionId 讨论组id
     * \param memberList 要移除的用户的数组，数组中的每个元素为UCSUserInfo类的对象。
     */
    bool removeMemberFromDiscussionWithDiscussionId(IN QString discussionId, IN QList<UCSUserInfo> memberList);

    /*!
     * \brief quitDiscussionWithDiscussionId 主动退出讨论组
     * \param discussionId 讨论组id
     */
    bool quitDiscussionWithDiscussionId(IN QString discussionId);

    /*!
     * \brief setDiscussionTopicWithDiscussionId 修改讨论组的主题(讨论组名称)
     * \param discussionId 讨论组id
     * \param newTopic 新的主题
     */
    bool setDiscussionTopicWithDiscussionId(IN QString discussionId,
                                            IN QString newTopic);

    /*!
     * \brief getDiscussionInfoWithDiscussionId 获取一个存在的讨论组。如果本地不存在这个讨论组的信息，就会返回空。
     * \param targetId 讨论组id
     * \return 讨论组 UCSDiscussion 对象
     */
    UCSDiscussion getDiscussionInfoWithDiscussionId(IN QString targetId);

    /*!
     * \brief getDiscussions 获取当前账号加入的所有讨论组，返回一个列表，其中的每个元素是一个UCSDiscussion类型的对象
     * \return 返回加入的所有讨论组.如果当前账号没有加入任何讨论组，则列表为空。
     */
    QList<UCSDiscussion> getDiscussions();

    /*!
     * \brief getConversationList 获取会话列表
     * \param conversationListType 获取的会话列表的类型。具体参考UCS_IM_ConversationListType
     * \return 会话数组，每个元素是一个UCSConversation对象
     */
    QList<UCSConversation*> getConversationList(IN UCS_IM_ConversationListType conversationListType);

    /*!
     * \brief removeConversation 在会话列表中移除一个会话，但是不会删除该会话在数据库中的消息。
     * \param conversationListType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \return 操作结果的bool值
     */
    bool removeConversation(IN UCS_IM_ConversationType conversationListType,
                            IN QString targetId);

    /*!
     * \brief clearMessages 清空某一个会话所有的聊天记录
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \return 操作结果的bool值
     */
    bool clearMessages(IN UCS_IM_ConversationType conversationType,
                       IN QString targetId);

    /*!
     * \brief clearConversations 清空会话列表
     * \details 这个方法是用来清空会话列表，注意和clearMessages:targetId:这个方法区分开来。
     * \return 操作结果的bool值
     */
    bool clearConversations();

    /*!
     * \brief getLatestMessages 获取最新的消息.用于进入聊天界面后第一次加载消息
     * \details 这个方法用于进入聊天界面第一次加载消息时候调用
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \param count 需要获取的消息数量。默认获取20条。也可以自己设置,如果该会话的消息总数小于设置的count数，则返回该会话的消息总数。
     * \return 消息数组。数组中元素的类型是UCSMessage对象，需主动释放
     */
    QList<UCSMessage*> getLatestMessages(IN UCS_IM_ConversationType conversationType,
                                        IN QString targetId,
                                        IN quint32 count);

    /*!
     * \brief getHistoryMessages 获取更多的消息。用于聊天界面中加载更多消息。
     * \details 这个方法用于聊天界面中下拉加载更多消息时候调用。
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \param oldestMessageId  当前已经请求好的消息中最早的那条消息的消息id。
     * \param count 需要获取的消息数量
     * \return 消息数组
     */
    QList<UCSMessage*> getHistoryMessages(IN UCS_IM_ConversationType conversationType,
                                         IN QString targetId,
                                         IN quint64 oldestMessageId,
                                         IN quint32 count);

    /*!
     * \brief deleteMessage 删除一条指定的消息
     * \brief 这个方法一般用于聊天界面删除一条消息，删除后这条消息在数据库中也会被删除。
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \param messageId 消息id
     * \return 操作结果的bool值
     */
    bool deleteMessage(IN UCS_IM_ConversationType conversationType,
                        IN QString targetId,
                        IN quint64 messageId);

    /*!
     * \brief getUnreadCount 获取某个会话的未读消息数
     * \details 这个方法用来获取某个会话的未读消息数，提醒用户有未读的消息.(如果传入的参数错误或者操作获取失败,会返回-1)
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \return 未读消息数量.(注意:如果传入的参数错误，则直接返回-1)
     */
    qint32 getUnreadCount(IN UCS_IM_ConversationType conversationType,
                          IN QString targetId);

    /*!
     * \brief getTotalUnreadCount 获取所有会话的未读消息总数
     * \return 未读消息总数
     */
    qint32 getTotalUnreadCount();

    /*!
     * \brief clearConversationsUnreadCount 清空某个会话的未读消息总数
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \return  操作结果的bool值
     */
    bool clearConversationsUnreadCount(IN UCS_IM_ConversationType conversationType,
                                       IN QString targetId);

    /*!
     * \brief checkChatMessages 根据关键字查询某个会话的聊天记录
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \param keyWord 关键字
     * \return 查询结果。数组中的每个元素是一个UCSMessage对象
     */
    QList<UCSMessage*> checkChatMessages(IN UCS_IM_ConversationType conversationType,
                                        IN QString targetId,
                                        IN QString keyWord);

    /*!
     * \brief setConversationToTop 会话列表中,设置某条会话的置顶状态
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \param isTop true 置顶 , false 取消置顶
     * \return 操作结果的bool值
     */
    bool setConversationToTop(IN UCS_IM_ConversationType conversationType,
                              IN QString targetId,
                              IN bool isTop);

    /*!
     * \brief setMessageReceivedStatus 设置接收到的某条消息在本地的接收状态。具体参考UCSReceivedStatus
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \param messageId 消息id。
     * \param receivedStatus 要设置的状态。类型值参考UCSReceivedStatus
     * \return 操作结果的bool值
     */
    bool setMessageReceivedStatus(IN UCS_IM_ConversationType conversationType,
                                  IN QString targetId,
                                  IN quint64 messageId,
                                  IN UCSReceivedStatus receivedStatus);

    /*!
     * \brief reset 清除本地保存的所有聊天数据，包括语音、图片。
     * \warning 1、调用此接口可清除本地存储的所有IM数据,且不可恢复；
     *          2、调用此接口时，如有账号处于登录状态，会主动退出。
     *          3、清除的数据包括：聊天记录 ，账号信息等；
     */
    bool reset();

    /*!
     * \brief getTextMessageDraft 获取某个会话的文本草稿
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \return 文本草稿。如果没有，为空。
     */
    QString getTextMessageDraft(IN UCS_IM_ConversationType conversationType,
                                IN QString targetId);

    /*!
     * \brief saveTextMessageDraft 保存某个会话的文本草稿
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \param content 文本草稿
     * \return 操作结果的bool值
     */
    bool saveTextMessageDraft(IN UCS_IM_ConversationType conversationType,
                              IN QString targetId,
                              IN QString content);

    /*!
     * \brief clearTextMessageDraft 清除某个会话的文本草稿
     * \param conversationType 会话类型。类型值参考UCS_IM_ConversationType
     * \param targetId 会话id。单聊为对方的id，讨论组聊天为讨论组id，群组聊天为群组id
     * \return 操作结果的bool值
     */
    bool clearTextMessageDraft(IN UCS_IM_ConversationType conversationType,
                               IN QString targetId);

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
