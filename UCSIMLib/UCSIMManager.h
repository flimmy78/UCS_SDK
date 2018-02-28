#ifndef UCSIMMANAGER_H
#define UCSIMMANAGER_H

#include <QObject>
#include <qglobal.h>
#include <QMap>
#include "UCSTcp/UCSTcpDefine.h"
#include "UCSTcp/UCSPackage.h"
#include "UCSMessage.h"
#include "UCSIMDefine.h"
#include "UCSUserInfo.h"
#include "UCSDiscussion.h"
#include "UCSConversation.h"
#include <UCSIMTimer.h>
#include <UCSDBEntity.h>

class UCSIMManager : public QObject
{
    Q_OBJECT
public:
    explicit UCSIMManager(QObject *parent = 0);
    ~UCSIMManager();

    void init();

    /*!
     * \brief setIMReceiver 注册IM相关事件接收者
     * \param receiver 事件接收者，需重载customEvent()函数
     */
    void setIMReceiver(QObject *receiver);

    /*!
     * \brief updateTcpState 更新tcp连接状态
     * \param state 更新的Tcp状态
     */
    void updateTcpState(UcsTcpState state);

    /*!
     * \brief updateLoginState 更新登录状态
     * \param state 更新的登录状态
     * \param userid 当前登录的userid
     */
    void updateLoginState(UcsLoginState state, QString userid);

    /*!
     * \brief processRecvData 处理IM响应或请求的消息
     * \param cmd 请求或响应的命令字
     * \param recvData socket收到的数据
     */
    void processRecvData(quint32 cmd, QByteArray recvData);

    /*!
     * \brief doSendMessage 发送一条聊天消息
     * \param pMessage  发送的消息
     * \return 0 for success, else failed。
     */
    bool doSendMessage(UCSMessage *pMessage);

    /*!
     * \brief doDownloadVoice 手动下载消息的语音数据
     */
    bool doDownloadVoice(UCS_IM_ConversationType conversationType,
                       QString targetId,
                       quint64 messageId);

    /*!
     * \brief doCreateDiscussion 主动创建一个讨论组
     */
    bool doCreateDiscussion(QString topic, QList<UCSUserInfo> memberList);

    /*!
     * \brief doAddMemberToDiscussion 给已经存在的讨论组添加新的用户
     */
    bool doAddMemberToDiscussion(QString discussionId, QList<UCSUserInfo> memberList);

    /*!
     * \brief doRemoveMemberFromDiscussion 在讨论组中移除已经存在的用户
     */
    bool doRemoveMemberFromDiscussion(QString discussionId, QList<UCSUserInfo> memberList);

    /*!
     * \brief doQuitDiscussion 主动退出讨论组
     */
    bool doQuitDiscussion(QString discussionId);

    /*!
     * \brief doSetDiscussionTopic 修改讨论组的主题(讨论组名称)
     */
    bool doSetDiscussionTopic(QString discussionId, QString newTopic);

    /*!
     * \brief doGetDiscussionWithId 查询指定讨论组Id的讨论组信息
     * \param discussionId
     * \param discussion
     * \return
     */
    bool doGetDiscussionWithId(QString discussionId, UCSDiscussion &discussion);

    /*!
     * \brief doGetAllDiscussions
     * \return 所有该用户的讨论组列表
     */
    bool doGetAllDiscussions(QList<UCSDiscussion> &discussionList);

    /*!
     * \brief doGetAllConversation 查询指定会话类型的所有会话
     * \param typeList
     * \param conversationList
     * \return true for success
     */
    bool doGetConversationList(UCS_IM_ConversationListType typeList,
                              QList<UCSConversation*> &conversationList);

    /*!
     * \brief doRemoveConversation 删除指定会话
     * \param type
     * \param targetId
     * \return
     */
    bool doRemoveConversation(UCS_IM_ConversationType type, QString targetId);

    /*!
     * \brief doClearMessages 清空指定会话所有聊天记录
     * \param type
     * \param targetId
     * \return
     */
    bool doClearMessages(UCS_IM_ConversationType type, QString targetId);

    /*!
     * \brief doClearConversations 删除所有会话列表
     * \return
     */
    bool doClearConversations();

    /*!
     * \brief doGetLastestMessages 拉取最新消息
     * \param type
     * \param targetId
     * \param count
     * \param messageList
     * \return
     */
    bool doGetLastestMessages(UCS_IM_ConversationType type,
                              QString targetId,
                              quint32 count,
                              QList<UCSMessage*> &messageList);

    /*!
     * \brief doGetHistoryMessages 拉取更多历史消息
     * \param type
     * \param targetId
     * \param oldestMessageId
     * \param count
     * \param messageList
     * \return
     */
    bool doGetHistoryMessages(UCS_IM_ConversationType type,
                              QString targetId,
                              quint64 oldestMessageId,
                              quint32 count,
                              QList<UCSMessage*> &messageList);

    /*!
     * \brief doDeleteMessage 删除一条指定消息
     * \param type
     * \param targetId
     * \param messageId
     * \return
     */
    bool doDeleteMessage(UCS_IM_ConversationType type,
                         QString targetId,
                         quint64 messageId);

    /*!
     * \brief doGetUnReadCount 查询指定会话的未读消息数
     * \param type
     * \param targetId
     * \return
     */
    qint32 doGetUnReadCount(UCS_IM_ConversationType type,
                          QString targetId);

    /*!
     * \brief doGetAllUnReadCount 查询所有会话的总未读消息数
     * \return
     */
    qint32 doGetAllUnReadCount();

    /*!
     * \brief doClearUnReadCount 清空指定会话的未读消息数
     * \param type
     * \param targetId
     * \return
     */
    bool doClearUnReadCount(UCS_IM_ConversationType type,
                            QString targetId);

    /*!
     * \brief doSearchMessage 查找指定会话内带有关键字的聊天记录
     * \param type
     * \param targetId
     * \param keyWord
     * \param messageList
     * \return
     */
    bool doSearchMessage(UCS_IM_ConversationType type,
                         QString targetId,
                         QString keyWord,
                         QList<UCSMessage*> &messageList);

    /*!
     * \brief doSetTop 聊天会话是否置顶
     * \param type
     * \param targetId
     * \param isTop
     * \return
     */
    bool doSetTop(UCS_IM_ConversationType type,
                  QString targetId,
                  bool isTop);

    /*!
     * \brief doUpdateReceivedStatus 更新指定聊天记录的接收状态
     * \param type
     * \param targetId
     * \param messageId
     * \param status
     * \return
     */
    bool doUpdateReceivedStatus(UCS_IM_ConversationType type,
                                QString targetId,
                                quint64 messageId,
                                UCSReceivedStatus status);

    /*!
     * \brief doReset 清空登录用户所有记录
     * \return
     */
    bool doReset();

    /*!
     * \brief doGetDraft 读取指定会话的文本草稿内容
     * \param type
     * \param targetId
     * \return
     */
    QString doGetDraft(UCS_IM_ConversationType type,
                       QString targetId);

    /*!
     * \brief doUpdateDraft 保存指定会话的文本草稿
     * \param type
     * \param targetId
     * \param draft
     * \return
     */
    bool doUpdateDraft(UCS_IM_ConversationType type,
                       QString targetId,
                       QString draft);

private:
    void initConnections();

    /*!
     * \brief doInitSyncRequest 请求拉取初始化消息
     */
    void doInitSyncRequest();

    /*!
     * \brief doNewSyncRequest 同步消息请求
     * \param selector 选择同步的内容
     */
    void doNewSyncRequest(quint32 selector);

    /*!
     * \brief doNewSyncCheckRequest
     * \details 客户端从休眠到激活状态(从Home状态到进入App状态)时，
     *          不再使用NewInit（仅在Auth后进行一次）来进行同步数据，
     *          而是使用此协议进行Check，根据返回的Selector，使用
     *          NewSync协议进行同步数据
     */
    void doNewSyncCheckRequest();

    /*!
     * \brief 处理发送文本消息的响应数据
     * \param recvData 响应数据
    */
    void handleSendMessageResponse(QByteArray recvData);

    /*!
     * \brief 处理账号首次登陆init拉取消息的响应数据
     * \param recvData 响应数据
    */
    void handleInitSyncResponse(QByteArray recvData);

    /*!
     * \brief 处理消息同步响应
    */
    void handleNewSyncResponse(QByteArray recvData);

    /*!
     * \brief 处理服务器发送的消息通知
    */
    void handleNewNotifyRequest(QByteArray recvData);

    /*!
     * \brief 处理同步key有效检查响应
    */
    void handleNewSyncCheckResponse(QByteArray recvData);

    /*!
     * \brief 处理上传短语音响应
    */
    void handleUploadVoiceResponse(QByteArray recvData);

    /*!
     * \brief 处理下载短语音响应
    */
    void handleDownloadVoiceResponse(QByteArray recvData);

    /*!
     * \brief 处理上传图片响应
    */
    void handleUploadImgResponse(QByteArray recvData);

    /*!
     * \brief 处理下载图片响应
    */
    void handleDownloadImgResponse(QByteArray recvData);

    /*!
     * \brief handleSendCustomMsgResponse
     * \param recvData
     */
    void handleSendCustomMsgResponse(QByteArray recvData);

    /*!
     * \brief 处理创建群响应
    */
    void handleCreateGroupResponse(QByteArray recvData);

    /*!
     * \brief 处理群组加人响应
    */
    void handleAddGroupMemberResponse(QByteArray recvData);

    /*!
     * \brief 处理群组删人响应
    */
    void handleDelGroupMemberResponse(QByteArray recvData);

    /*!
     * \brief 处理退出群组响应
    */
    void handleQuitGroupResponse(QByteArray recvData);

    //////////////////////////////////////////////////
    /*!
     * \brief sendData 发送socket数据
     * \details 该函数实际是发送一个send event到UCSTcpSocket对象，由该对象发送数据
     * \param cmd IM命令字
     * \param dataArray 数据
     */
    bool sendData(quint32 cmd, QByteArray dataArray);

    QString userId() const;

    ///////////////////////////////////////////////////
    void sendMsgTimeout();
    void removeMsgFromMap(QString msgId);

    //////////////////////////////////////////////////
    /// \brief updateMsgSendStatus 更新图片、消息的发送状态
    /// \param msgId
    /// \param status
    /// \param time
    /// \param toUserName
    ///
    void updateMsgSendStatus(QString msgId,
                             UCSSendStatus status,
                             QString time,
                             QString toUserName);

    /*!
     * \brief handleRecivedMsgList 对init sync或new sync响应的消息队列进行处理
     * \param msgList 消息数组
     */
    void handleRecivedMsgList(const QList<UCSIMAddMsg_t> msgList);

    void saveConversationAndChat(QMap<QString, QList<ChatEntity>> chatListMap,
                                 UCS_IM_ConversationType type);

signals:

public slots:
    void slot_timeOut(UCSIMTimer::ImTimerId id);

private:
    QString m_userId;
    UcsTcpState m_tcpState;

    ///< 图片上传 >
    typedef  QMap<QString, UCSUploadMsgImgRequest_t> ImgUploadReq;
    ImgUploadReq m_mapImgUpReq;

    ///< 语音下载 >
    typedef QMap<QString, VoiceInfoEntity> VoiceEntityMap;
    typedef QMap<QString, VoiceDownloadEntity> VoiceDownMap;
    VoiceEntityMap m_voiceEntity;
    VoiceDownMap m_voiceDown;

    ///< 消息发送缓存 >
    typedef QMap<QString, ChatEntity> MsgSendMap;
    MsgSendMap m_msgSendQueue;

    QObject *m_pReceiver;
};

#endif // UCSIMMANAGER_H
