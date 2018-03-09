#include "UCSIMManager.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QSettings>
#include <QStringUtil.h>
#include "UCSTcp/UCSLogger.h"
#include "UCSTcp/UCSEvent.h"
#include "Common/UCSIMHelper.h"
#include "Common/UCSClock.h"
#include "UCSIMPublicDef.h"
#include "UCSMessage.h"
#include "UCSClock.h"
#include "UCSDBCenter.h"
#include "UCSIMEvent.h"
#include "UCSIMError.h"

UCSIMManager::UCSIMManager(QObject *parent)
    : QObject(parent)
    , m_tcpState(TcpDisconnected)
    , m_pReceiver(Q_NULLPTR)
{
    m_mapImgUpReq.clear();

    initConnections();
}

UCSIMManager::~UCSIMManager()
{
    while(!m_mapImgUpReq.isEmpty())
    {
        ImgUploadReq::iterator it = m_mapImgUpReq.begin();
        m_mapImgUpReq.erase(it);
    }

    UCSIMTimer::release();
    UCSDBCenter::release();
}

void UCSIMManager::init()
{
    UCSIMTimer::start(UCSIMTimer::kMsgSendTimer);
}

void UCSIMManager::setIMReceiver(QObject *receiver)
{
    m_pReceiver = receiver;
}

void UCSIMManager::updateTcpState(UcsTcpState state)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMManager,
            QString("updateTcpState state(%1)").arg(state));

    m_tcpState = state;
}

void UCSIMManager::updateLoginState(UcsLoginState state, QString userid)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMManager,
            QString("updateLoginState state(%1) userid(%2)")
            .arg(state)
            .arg(userid));

    m_userId = userid;
    UCSIMHelper::writeSettings(UCS_LOGIN_USERID_KEY, userid);

    UCSDBCenter::updateLoginState(state);

    if (state == LoginSuccessed || state == ReLoginSuccessed)
    {
        QString loginTime = QString::number(UCSClock::TimeInMicroseconds());
        UCSIMHelper::writeSettings(UCS_LOGIN_TIME_KEY, loginTime);

        doInitSyncRequest();
    }
}

void UCSIMManager::processRecvData(quint32 cmd, QByteArray recvData)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMManager,
            QString("processRecvData cmd(%1)").arg(cmd));

    switch (cmd) {
    case RESP_SEND_MSG: ///<发送文本消息响应>
    {
        handleSendMessageResponse(recvData);
    }
        break;

    case RESP_NEW_SYNC: ///<同步消息请求响应(接收到的消息)>
    {
        handleNewSyncResponse(recvData);
    }
        break;

    case RESP_INIT: ///<初始化消息请求响应>
    {
        handleInitSyncResponse(recvData);
    }
        break;

    case RESP_UPLOAD_VOICE: ///<上传语音请求响应>
    {
        handleUploadVoiceResponse(recvData);
    }
        break;

    case RESP_DOWNLOAD_VOICE: ///<下载语音请求响应>
    {
        handleDownloadVoiceResponse(recvData);
    }
        break;

    case RESP_UPLOAD_MSGIMG:    ///<上传图片请求响应>
    {
        handleUploadImgResponse(recvData);
    }
        break;

    case RESP_DOWNLOAD_MSGIMG:  ///<下载图片请求响应>
    {
        handleDownloadImgResponse(recvData);
    }
        break;

    case RESP_SEND_CUSTOMMSG:
    {

    }
        break;

    case REQ_NEW_NOTIFY:    ///<服务器发送的通知：新消息等>
    {
        handleNewNotifyRequest(recvData);
    }
        break;

    case RESP_CREATE_GROUP:  ///<创建群响应>
    {
        handleCreateGroupResponse(recvData);
    }
        break;

    case RESP_ADD_GROUP_MEMBER: ///<群组加入响应>
    {
        handleAddGroupMemberResponse(recvData);
    }
        break;

    case RESP_DEL_GROUP_MEMBER: ///<群组删人响应>
    {
        handleDelGroupMemberResponse(recvData);
    }
        break;

    case RESP_QUIT_GROUP:   ///<退出群组响应>
    {
        handleQuitGroupResponse(recvData);
    }
        break;

    default:
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kIMManager,
                QString("waitting supported message(%1)").arg(cmd));
        break;
    }
}

bool UCSIMManager::doSendMessage(UCSMessage *pMessage)
{
    bool sendRes = false;
    QString msgId = UCSIMHelper::messageId();
    qint64 nowTime = UCSClock::TimeInMicroseconds();
    QString toUserName = UCSIMHelper::formatToUserName(pMessage->receivedId,
                                                       pMessage->conversationType);

    pMessage->messageId = msgId.toLongLong();
    pMessage->messageDirection = MessageDirection_SEND;
    pMessage->senderUserId = userId();
    pMessage->senderNickName = userId();
    pMessage->sendStatus = SendStatus_sending;
    pMessage->receivedStatus = ReceivedStatus_UNREAD;
    pMessage->time = nowTime;

    ChatEntity chatEntity;
    chatEntity.categoryId = QString::number(pMessage->conversationType);
    chatEntity.msgId = msgId;
    chatEntity.targetId = pMessage->receivedId;
    chatEntity.senderId = userId();
    chatEntity.senderNickName = userId();
    chatEntity.isFromMySelf = QStringUtil::fromBool(true);
    chatEntity.sendTime = QString::number(nowTime);
    chatEntity.receivedTime = "0";
    chatEntity.msgType = QString::number(pMessage->messageType);
    chatEntity.readStatus = QString::number(ReceivedStatus_UNREAD);
    chatEntity.sendStatus = QString::number(SendStatus_sending);

    if (pMessage->messageType == UCS_IM_TEXT)
    {
        UCSTextMsg *txtMsg = static_cast<UCSTextMsg*>(pMessage->content);
        if (txtMsg->content().length() > UCS_MAX_TXT_MSG_LENGTH)
        {
            UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMManager,
                    QStringLiteral("文本消息内容超过最大长度1500字节"));

            return false;
        }

        UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kIMManager,
                QStringLiteral("发送一条文本消息"));

        chatEntity.content = txtMsg->content();

        UCSSendMsgRequest_t request;
        UCSMicroMsgRequest_t msg;
        msg.tFromUserName = userId();
        msg.tToUserName = toUserName;
        msg.iType = pMessage->messageType;
        msg.iCreateTime = pMessage->time;
        msg.pcClientMsgId = msgId;
        msg.iEmojiFlag = 0;
        msg.pcContent = txtMsg->content();

        request.pList.append(msg);
        QByteArray packData = UCSPackage::PackSendMsgRequest(&request);

        sendRes = sendData(REQ_SEND_MSG, packData);
    }
    else if (pMessage->messageType == UCS_IM_IMAGE)
    {
        UCSImageMsg *imgMsg = static_cast<UCSImageMsg*>(pMessage->content);

//        QString imagePath = UCSIMHelper::saveImage(msgId, imgMsg->originalImage);
//        imgMsg->imageLocalPath = imagePath;
        chatEntity.content = imgMsg->imageLocalPath;
        chatEntity.thumbnaiUrl = imgMsg->thumbnailLocalPath;

        UCSUploadMsgImgRequest_t request;
        request.pcClientMsgId = msgId;
        request.pcFromUserName = userId();
        request.pcToUserName = toUserName;
        request.iStartPos = 0;
        request.iDataLen = imgMsg->originalImage.size();
        request.iTotalLen = imgMsg->originalImage.size();
        request.tData = imgMsg->originalImage;

        ///< 图片大小64K，分多次上传 >
        if (request.iTotalLen > UCS_MAX_IMG_SECTION_SIZE)
        {
            m_mapImgUpReq[msgId] = request;
            request.iDataLen = UCS_MAX_IMG_SECTION_SIZE;
        }

        QByteArray packData = UCSPackage::PackUploadMsgImgRequest(&request);

        sendRes = sendData(REQ_UPLOAD_MSGIMG, packData);
    }
    else if (pMessage->messageType == UCS_IM_Custom)
    {
        UCSCustomMsg *customMsg = static_cast<UCSCustomMsg*>(pMessage->content);
        if (customMsg->data().size() >= UCS_MAX_CUSTOM_MSG_SIZE)
        {
            UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMManager,
                    QStringLiteral("错误:自定义消息大小超过16K."));
            return false;
        }

        chatEntity.content = customMsg->data();

        UCSCustomMsgRequest_t request;
        request.pcClientMsgId = msgId;
        request.pcFromUserName = userId();
        request.pcToUserName = toUserName;
        request.tData = customMsg->data();
        request.pcContent = QString("This is a custom message.");
        request.iTotalLen = customMsg->data().size();
        request.iStartPos = 0;

        QByteArray packData = UCSPackage::PackCustomMsgRequest(&request);
        sendRes = sendData(REQ_SEND_CUSTOMMSG, packData);
    }

    m_msgSendQueue.insert(msgId, chatEntity);

    ///< 聊天消息保存到数据库 >
    UCSDBCenter::chatMgr()->addChat(pMessage->receivedId,
                                    pMessage->conversationType,
                                    chatEntity);

    UCSDBCenter::conversationMgr()->addConversation(&chatEntity, 0);

    if (!sendRes)
    {
        UCSEvent::postEvent(m_pReceiver,
                            new UCSMsgSendEvent(ErrorCode_NetworkNotConnected, msgId.toLongLong()));
    }

    return true;
}

bool UCSIMManager::doDownloadVoice(UCS_IM_ConversationType conversationType,
                                   QString targetId,
                                   quint64 messageId)
{
    VoiceInfoEntity voiceInfo;
    bool result = UCSDBCenter::voiceInfoMgr()->getVoiceInfo(QString::number(messageId), voiceInfo);
    if (!result)
    {
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kIMManager,
                QStringLiteral("未查询到指定语音消息"));
        return false;
    }

    ///< 保存供分片下载时使用 >
    m_voiceEntity[voiceInfo.iMsgId] = voiceInfo;

    VoiceDownloadEntity download;
    download.conversationType = QString::number(conversationType);
    download.messageId = QString::number(messageId);
    download.targetId = targetId;
    download.pcClientMsgId = voiceInfo.pcClientMsgId;
    m_voiceDown[voiceInfo.pcClientMsgId] = download;

    UCSDownloadVoiceRequest_t request;
    request.iMsgId = messageId;
    request.pcClientMsgId = voiceInfo.pcClientMsgId;
    request.iOffset = 0;
    request.iLenght = voiceInfo.iLength.toInt();

    QByteArray packData = UCSPackage::PackDownloadVoiceRequest(&request);

    sendData(REQ_DOWNLOAD_VOICE, packData);

    return true;
}

bool UCSIMManager::doCreateDiscussion(QString topic, QList<UCSUserInfo> memberList)
{
    UCSCreateGroupRequest_t request;
    request.tIntroduce = topic;
    request.iMemberCount = memberList.size();

    for (int i = 0; i < memberList.size(); i++)
    {
        request.ptMemberList.append(memberList.at(i).userId());
    }
    request.iMemberCount = request.ptMemberList.size();

    QByteArray packData = UCSPackage::PackCreateGroupRequest(&request);
    sendData(REQ_CREATE_GROUP, packData);

    UCSIMTimer::start(UCSIMTimer::kCreateGroupTimer);

    return true;
}

bool UCSIMManager::doAddMemberToDiscussion(QString discussionId, QList<UCSUserInfo> memberList)
{
    UCSAddGroupMemberRequest_t request;
    request.iChatRoomId = discussionId.toInt();
    for (int i = 0; i < memberList.size(); i++)
    {
        request.ptMemberList.append(memberList.at(i).userId());
    }
    request.iMemberCount = request.ptMemberList.size();

    QByteArray packData = UCSPackage::PackAddGroupMemberRequest(&request);
    sendData(REQ_ADD_GROUP_MEMBER, packData);

    UCSIMTimer::start(UCSIMTimer::kAddGroupMemberTimer);

    return true;
}

bool UCSIMManager::doRemoveMemberFromDiscussion(QString discussionId, QList<UCSUserInfo> memberList)
{
    UCSDelGroupMemberRequest_t request;

    request.iChatRoomId = discussionId.toInt();
    for (int i = 0; i < memberList.size(); i++)
    {
        request.ptMemberList.append(memberList.at(i).userId());
    }
    request.iMemberCount = request.ptMemberList.size();

    QByteArray packData = UCSPackage::PackDelGroupMemberRequest(&request);
    sendData(REQ_DEL_GROUP_MEMBER, packData);

    UCSIMTimer::start(UCSIMTimer::kDelGroupMemberTimer);

    return true;
}

bool UCSIMManager::doQuitDiscussion(QString discussionId)
{
    UCSQuitGroupRequest_t request;

    request.tQuitChatRoom.iChatRoomId = discussionId.toInt();
    request.tQuitChatRoom.tUserName = userId();

    QByteArray packData = UCSPackage::PackQuitGroupRequest(&request);
    sendData(REQ_QUIT_GROUP, packData);

    return true;
}

bool UCSIMManager::doSetDiscussionTopic(QString discussionId, QString newTopic)
{
    UCSModGroupTopicRequest_t request;

    request.iChatRoomId = discussionId.toInt();
    request.tChatRoomTopic = newTopic;

    /* create current sync key */
    UCSIMKeyVal_t curContactKey;
    curContactKey.iKey = 3;   ///<1:userProfile 2:msg 3:contact>
    curContactKey.iVal = UCSIMHelper::readSettings(UCS_IM_CONTACT_KEY, 0).toInt();
    request.tSyncKey.ptKey.append(curContactKey);

    QByteArray packData = UCSPackage::PackModGroupTopicRequest(&request);
    sendData(REQ_NEW_SYNC, packData);

    return true;
}

bool UCSIMManager::doGetDiscussionWithId(QString discussionId, UCSDiscussion &discussion)
{
    DiscussionEntity entity;
    if (UCSDBCenter::discussionMgr()->getDiscussion(discussionId, entity))
    {
        UCSDBEntity::convertDiscussionFromEntity(entity, discussion);
        return true;
    }
    return false;
}

bool UCSIMManager::doGetAllDiscussions(QList<UCSDiscussion> &discussionList)
{
    QList<DiscussionEntity> entityList;
    entityList = UCSDBCenter::discussionMgr()->getAllDiscussions();
    foreach (DiscussionEntity entity, entityList)
    {
        UCSDiscussion discussion;
        UCSDBEntity::convertDiscussionFromEntity(entity, discussion);
        discussionList.append(discussion);
    }

    return true;
}

bool UCSIMManager::doGetConversationList(UCS_IM_ConversationListType typeList,
                                         QList<UCSConversation*> &conversationList)
{
    QList<qint32> types;
    switch (typeList) {
    case soloChatOnly:
        types.append(UCS_IM_SOLOCHAT);
        break;

    case groupChatOnly:
        types.append(UCS_IM_GROUPCHAT);
        break;

    case discussionChatOnly:
        types.append(UCS_IM_DISCUSSIONCHAT);
        break;

    case broadcastChatOnly:
        types.append(UCS_IM_Broadcast);
        break;

    case UnknownConversationTypeOnly:
        types.append(UCS_IM_UnknownConversationType);
        break;

    case solochatAndGroupChat:
        types.append(UCS_IM_SOLOCHAT);
        types.append(UCS_IM_GROUPCHAT);
        break;

    case soloChatAndDiscussionChat:
        types.append(UCS_IM_SOLOCHAT);
        types.append(UCS_IM_DISCUSSIONCHAT);
        break;

    case groupChatAndDiscussionChat:
        types.append(UCS_IM_GROUPCHAT);
        types.append(UCS_IM_DISCUSSIONCHAT);
        break;

    case allChat:
        types.append(UCS_IM_SOLOCHAT);
        types.append(UCS_IM_GROUPCHAT);
        types.append(UCS_IM_DISCUSSIONCHAT);
        types.append(UCS_IM_Broadcast);
        types.append(UCS_IM_UnknownConversationType);
        break;

    default:
        types.append(UCS_IM_SOLOCHAT);
        break;
    }

    QList<UCSConversation*> topList;
    QList<UCSConversation*> normalList;

    QList<ConversationEntity> entityList;
    entityList = UCSDBCenter::conversationMgr()->getConversationList(types);
    foreach (ConversationEntity entity, entityList)
    {
        UCSConversation* conversation = new UCSConversation;
        UCSDBEntity::convertConversationFromEntity(entity, conversation);

        ///< 取出该会话的最近一条消息记录 >
        QList<UCSMessage*> messageList;
        doGetLastestMessages(conversation->conversationType(),
                             conversation->targetId(),
                             1,
                             messageList);

        if (!messageList.isEmpty())
        {
            conversation->setLastestMessage(messageList.first());
        }

        if (conversation->isTop())
        {
            topList.append(conversation);
        }
        else
        {
            normalList.append(conversation);
        }
    }

    conversationList.append(topList);
    conversationList.append(normalList);

    return true;
}

bool UCSIMManager::doRemoveConversation(UCS_IM_ConversationType type, QString targetId)
{
    return UCSDBCenter::conversationMgr()->delConversation(type, targetId);
}

bool UCSIMManager::doClearMessages(UCS_IM_ConversationType type, QString targetId)
{
    return UCSDBCenter::chatMgr()->clearChats(targetId, type);
}

bool UCSIMManager::doClearConversations()
{
    return UCSDBCenter::conversationMgr()->clearConversations();
}

bool UCSIMManager::doGetLastestMessages(UCS_IM_ConversationType type,
                                        QString targetId,
                                        quint32 count,
                                        QList<UCSMessage*> &messageList)
{
    QList<ChatEntity> chatList;
    chatList = UCSDBCenter::chatMgr()->getChats(targetId, type, count);
    foreach (ChatEntity chatEntity, chatList)
    {
        UCSMessage *message = new UCSMessage;
        UCSDBEntity::convertMessageFromChat(chatEntity, message);
        messageList.append(message);
    }

    return true;
}

bool UCSIMManager::doGetHistoryMessages(UCS_IM_ConversationType type,
                                        QString targetId,
                                        quint64 oldestMessageId,
                                        quint32 count,
                                        QList<UCSMessage*> &messageList)
{
    QList<ChatEntity> chatList;
    chatList = UCSDBCenter::chatMgr()->getHistoryChats(targetId, type, count, QString::number(oldestMessageId));
    foreach (ChatEntity chatEntity, chatList)
    {
        UCSMessage *message = new UCSMessage;
        UCSDBEntity::convertMessageFromChat(chatEntity, message);
        messageList.append(message);
    }

    return true;
}

bool UCSIMManager::doDeleteMessage(UCS_IM_ConversationType type, QString targetId, quint64 messageId)
{
    return UCSDBCenter::chatMgr()->delChat(targetId, type, QString::number(messageId));
}

qint32 UCSIMManager::doGetUnReadCount(UCS_IM_ConversationType type, QString targetId)
{
    return UCSDBCenter::conversationMgr()->getUnReadCount(type, targetId);
}

qint32 UCSIMManager::doGetAllUnReadCount()
{
    return UCSDBCenter::conversationMgr()->getTotalUnReadCount();
}

bool UCSIMManager::doClearUnReadCount(UCS_IM_ConversationType type, QString targetId)
{
    return UCSDBCenter::conversationMgr()->clearUnReadCount(type, targetId);
}

bool UCSIMManager::doSearchMessage(UCS_IM_ConversationType type,
                                   QString targetId,
                                   QString keyWord,
                                   QList<UCSMessage*> &messageList)
{
    QList<ChatEntity> chatList;
    chatList = UCSDBCenter::chatMgr()->searchChats(targetId, type, keyWord);
    foreach (ChatEntity chatEntity, chatList)
    {
        UCSMessage* message = new UCSMessage;
        UCSDBEntity::convertMessageFromChat(chatEntity, message);
        messageList.append(message);
    }

    return true;
}

bool UCSIMManager::doSetTop(UCS_IM_ConversationType type,
                            QString targetId, bool isTop)
{
    return UCSDBCenter::conversationMgr()->setTop(type, targetId, isTop);
}

bool UCSIMManager::doUpdateReceivedStatus(UCS_IM_ConversationType type,
                                          QString targetId, quint64 messageId, UCSReceivedStatus status)
{
    return UCSDBCenter::chatMgr()->updateChatReceivedStatus(targetId, type, QString::number(messageId), status);
}

bool UCSIMManager::doReset()
{
    UCSDBCenter::release();

    return true;
}

QString UCSIMManager::doGetDraft(UCS_IM_ConversationType type, QString targetId)
{
    return UCSDBCenter::conversationMgr()->getDraft(type, targetId);
}

bool UCSIMManager::doUpdateDraft(UCS_IM_ConversationType type,
                                 QString targetId,
                                 QString draft)
{
    if (draft.isEmpty())
    {
        return UCSDBCenter::conversationMgr()->clearDraft(type, targetId);
    }
    else
    {
        return UCSDBCenter::conversationMgr()->addDraft(type, targetId, draft);
    }
}

void UCSIMManager::initConnections()
{
    connect(UCSIMTimer::getInstance(), SIGNAL(sig_timeout(UCSIMTimer::ImTimerId)), this, SLOT(slot_timeOut(UCSIMTimer::ImTimerId)));
}

void UCSIMManager::doInitSyncRequest()
{
    UCSInitNewSyncRequest_t request;

    request.pcLanuage = QString("zh_CN");
    request.pcUserName = userId();

    /* create current sync key */
    UCSIMKeyVal_t curMsgKey;
    curMsgKey.iKey = 2;    ///<1:userProfile 2:msg 3:contact>
    curMsgKey.iVal = UCSIMHelper::readSettings(UCS_IM_MSG_KEY, 0).toInt();
    request.tCurrentSyncKey.ptKey.append(curMsgKey);

    UCSIMKeyVal_t curContactKey;
    curContactKey.iKey = 3;   ///<1:userProfile 2:msg 3:contact>
    curContactKey.iVal = UCSIMHelper::readSettings(UCS_IM_CONTACT_KEY, 0).toInt();
    request.tCurrentSyncKey.ptKey.append(curContactKey);

    /* create max sync key */
    UCSIMKeyVal_t maxMsgKey;
    maxMsgKey.iKey = 2;
    maxMsgKey.iVal = UCSIMHelper::readSettings(UCS_IM_MSG_KEY, 0).toInt();
    request.tMaxSyncKey.ptKey.append(maxMsgKey);

    UCSIMKeyVal_t maxContactKey;
    maxContactKey.iKey = 3;
    maxContactKey.iVal = UCSIMHelper::readSettings(UCS_IM_CONTACT_KEY, 0).toInt();
    request.tMaxSyncKey.ptKey.append(maxContactKey);

    QByteArray dataArray = UCSPackage::PackInitNewSyncRequest(&request);

    sendData(REQ_INIT, dataArray);
}

void UCSIMManager::doNewSyncRequest(quint32 selector)
{
    UCSNewSyncRequest_t request;

    request.iSelector = selector;

    /* create current sync key */
    UCSIMKeyVal_t curMsgKey;
    curMsgKey.iKey = 2;    ///<1:userProfile 2:msg 3:contact>
    curMsgKey.iVal = UCSIMHelper::readSettings(UCS_IM_MSG_KEY, 0).toInt();
    request.tCurrentSyncKey.ptKey.append(curMsgKey);

    UCSIMKeyVal_t curContactKey;
    curContactKey.iKey = 3;   ///<1:userProfile 2:msg 3:contact>
    curContactKey.iVal = UCSIMHelper::readSettings(UCS_IM_CONTACT_KEY, 0).toInt();
    request.tCurrentSyncKey.ptKey.append(curContactKey);

    QByteArray dataArray = UCSPackage::PackNewSyncRequest(&request);

    sendData(REQ_NEW_SYNC, dataArray);
}

void UCSIMManager::doNewSyncCheckRequest()
{
    UCSNewSyncCheckRequest_t request;

    /* create current sync key */
    UCSIMKeyVal_t curMsgKey;
    curMsgKey.iKey = 2;    ///<1:userProfile 2:msg 3:contact>
    curMsgKey.iVal = UCSIMHelper::readSettings(UCS_IM_MSG_KEY, 0).toInt();
    request.tSyncKeyBuf.ptKey.append(curMsgKey);

    UCSIMKeyVal_t curContactKey;
    curContactKey.iKey = 3;   ///<1:userProfile 2:msg 3:contact>
    curContactKey.iVal = UCSIMHelper::readSettings(UCS_IM_CONTACT_KEY, 0).toInt();
    request.tSyncKeyBuf.ptKey.append(curContactKey);

    QByteArray dataArray = UCSPackage::PackNewSyncCheckRequest(&request);

    sendData(REQ_NEW_SYNCCHK, dataArray);
}

void UCSIMManager::handleSendMessageResponse(QByteArray recvData)
{
    if (recvData.size() < 0)
    {
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kIMManager,
                QString("handleSendMessageResponse empty recv data."));
        return;
    }

    UCSSendMsgResponse_t response;

    UCSPackage::UnpackSendMsgResponse(recvData, &response);

    UCSMicroMsgResponse_t msgResp = response.pList.at(0);

    if (!msgResp.pcClientMsgId.isEmpty())
    {
        removeMsgFromMap(msgResp.pcClientMsgId);
    }

    if (msgResp.iRet == 0) ///< 发送成功 >
    {
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMManager,
                QStringLiteral("收到消息发送成功响应"));

        updateMsgSendStatus(msgResp.pcClientMsgId,
                            SendStatus_success,
                            QString::number(msgResp.iCreateTime),
                            msgResp.tToUserName);

        UCSEvent::postEvent(m_pReceiver,
                            new UCSMsgSendEvent(ErrorCode_NoError,
                                                msgResp.pcClientMsgId.toLongLong()));
    }
    else    ///< 发送失败 >
    {
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kIMManager,
                QStringLiteral("收到消息发送失败响应"));

        updateMsgSendStatus(msgResp.pcClientMsgId,
                            SendStatus_fail,
                            QString::number(msgResp.iCreateTime),
                            msgResp.tToUserName);

        UCSEvent::postEvent(m_pReceiver,
                            new UCSMsgSendEvent(ErrorCode_SendMessageFail,
                                                msgResp.pcClientMsgId.toLongLong()));
    }
}

void UCSIMManager::handleInitSyncResponse(QByteArray recvData)
{
    UCSInitNewSyncResponse_t response;

    UCSPackage::UnpackInitNewSyncResponse(recvData, &response);
    if (response.tBaseResponse.iRet != 0)
    {
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kIMManager,
                QStringLiteral("登录初始化同步失败"));
        return;
    }

    /* save sync keys */
    for (int i = 0; i < response.tCurrentSyncKey.ptKey.size(); i++)
    {
        UCSIMKeyVal_t keyVal;
        keyVal = response.tCurrentSyncKey.ptKey.at(i);
        if (keyVal.iKey == 2)
        {
            UCSIMHelper::writeSettings(UCS_IM_MSG_KEY, keyVal.iVal);
        }
        if (keyVal.iKey == 3)
        {
            UCSIMHelper::writeSettings(UCS_IM_CONTACT_KEY, keyVal.iVal);
        }
    }

    ///< 讨论组变化处理 >
    foreach (UCSIMModContact_t modContact, response.tContactList)
    {
        DiscussionEntity discussion;
        UCSDBEntity::convertEntityFromContact(modContact, discussion);

        ///< 保存讨论组信息 >
        UCSDBCenter::discussionMgr()->addDiscussioin(&discussion);

        ///< 更新会话 >
        UCSDBCenter::conversationMgr()->updateConversation(discussion);
    }

    ///< 处理被踢出讨论组 >
    foreach (UCSIMDelContact_t contact, response.tDeleteList)
    {
        ///< 先判断是主动退出还是被踢出，如果主动退出，本地数据库已经删除了对应讨论组 >
        /// < 如果是被踢出，需要删除本地数据库对应讨论组记录，同时通知用户 >
        if (contact.tUserName.isEmpty())
        {
            continue;
        }

        DiscussionEntity entity;
        bool result = UCSDBCenter::discussionMgr()->getDiscussion(contact.tUserName, entity);
        if (!result ||
            entity.discussionId.isEmpty() ||
            entity.discussionId.isNull())
        {
            ///< 查不到记录说明应该是主动退出 >
            continue;
        }

        ///< 从数据库中删除当前用户被踢出的讨论组 >
        UCSDBCenter::discussionMgr()->delDiscussion(contact.tUserName);

        ///< 通知用户 >
    }

    ///< 处理消息记录 >
    if (response.tMsgList.size() > 0)
    {
        handleRecivedMsgList(response.tMsgList);
    }

    if (response.iContinueFlag != 0)
    {
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMManager,
                QStringLiteral("待同步数据太多，分批同步"));

        doInitSyncRequest();
    }
}

void UCSIMManager::handleNewSyncResponse(QByteArray recvData)
{
    UCSNewSyncResponse_t response;
    UCSPackage::UnpackNewSyncResponse(recvData, &response);

    if (response.tBaseResponse.iRet != 0)
    {
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kIMManager,
                QStringLiteral("同步拉取消息失败"));
        return;
    }

    /* save sync keys */
    for (int i = 0; i < response.tCurrentSyncKey.ptKey.size(); i++)
    {
        UCSIMKeyVal_t keyVal;
        keyVal = response.tCurrentSyncKey.ptKey.at(i);
        if (keyVal.iKey == 2)
        {
            UCSIMHelper::writeSettings(UCS_IM_MSG_KEY, keyVal.iVal);
        }
        if (keyVal.iKey == 3)
        {
            UCSIMHelper::writeSettings(UCS_IM_CONTACT_KEY, keyVal.iVal);
        }
    }

    ///< 讨论组变化处理 >
    foreach (UCSIMModContact_t modContact, response.tContactList)
    {
        DiscussionEntity discussion;
        UCSDBEntity::convertEntityFromContact(modContact, discussion);

        ///< 保存讨论组信息 >
        UCSDBCenter::discussionMgr()->addDiscussioin(&discussion);

        ///< 更新会话 >
        bool result = UCSDBCenter::conversationMgr()->updateConversation(discussion);
        ///< 不存在会话，则新增 >
        if (!result)
        {
            UCSDBCenter::conversationMgr()->addConversation(discussion);
        }
    }

    ///< 处理被踢出讨论组 >
    foreach (UCSIMDelContact_t contact, response.tDeleteList)
    {
        ///< 先判断是主动退出还是被踢出，如果主动退出，本地数据库已经删除了对应讨论组 >
        /// < 如果是被踢出，需要删除本地数据库对应讨论组记录，同时通知用户 >
        if (contact.tUserName.isEmpty())
        {
            continue;
        }

        DiscussionEntity entity;
        bool result = UCSDBCenter::discussionMgr()->getDiscussion(contact.tUserName, entity);
        if (!result ||
            entity.discussionId.isEmpty() ||
            entity.discussionId.isNull())
        {
            ///< 查不到记录说明应该是主动退出 >
            continue;
        }

        ///< 从数据库中删除当前用户被踢出的讨论组 >
        UCSDBCenter::discussionMgr()->delDiscussion(contact.tUserName);

        ///< 通知用户 >
    }

    ///< 处理消息记录 >
    if (response.tMsgList.size() > 0)
    {
        handleRecivedMsgList(response.tMsgList);
    }

    if (response.iContinueFlag != 0)
    {
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMManager,
                QStringLiteral("待同步数据太多，分批同步"));

//        doNewSyncRequest(response.iContinueFlag);
    }
}

void UCSIMManager::handleNewNotifyRequest(QByteArray recvData)
{
    UCSNewNotify_t notify;
    UCSPackage::UnpackNewNotifyRequest(recvData, &notify);

    if (notify.selector > 0)
    {
        doNewSyncRequest(notify.selector);
    }
}

void UCSIMManager::handleNewSyncCheckResponse(QByteArray recvData)
{
    UCSNewSyncCheckResponse_t response;
    UCSPackage::UnpackNewSyncCheckResponse(recvData, &response);

    if (response.selector > 0)
    {
        doNewSyncRequest(response.selector);
    }
}

void UCSIMManager::handleUploadVoiceResponse(QByteArray recvData)
{
    Q_UNUSED(recvData);
}

void UCSIMManager::handleDownloadVoiceResponse(QByteArray recvData)
{
    UCSDownloadVoiceResponse_t response;
    UCSPackage::UnpackDownloadVoiceReponse(recvData, &response);


}

void UCSIMManager::handleUploadImgResponse(QByteArray recvData)
{
    UCSUploadMsgImgResponse_t response;
    UCSPackage::UnpackUploadMsgImgResponse(recvData, &response);

    if (response.tBaseResponse.iRet != 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMManager,
                QStringLiteral("图片上传失败"));

        QMap<QString, UCSUploadMsgImgRequest_t>::iterator it = m_mapImgUpReq.find(response.pcClientMsgId);
        if (it != m_mapImgUpReq.end())
        {
            /// find it and delete
            m_mapImgUpReq.erase(it);
        }

        ///< 发送队列中移除相应的图片上传消息 >
        if (!response.pcClientMsgId.isEmpty())
        {
            removeMsgFromMap(response.pcClientMsgId);
        }

        updateMsgSendStatus(response.pcClientMsgId,
                            SendStatus_fail,
                            QString::number(response.iCreateTime),
                            response.pcToUserName);
        return;
    }

    ///< 续传分片的图片数据 >
    ImgUploadReq::iterator it = m_mapImgUpReq.find(response.pcClientMsgId);;
    if (it != m_mapImgUpReq.end())
    {
        /// find it
        UCSUploadMsgImgRequest_t request = it.value();

        ///< 如果响应包的起始位置与请求数据的总长度相等，说明已经全部上传成功 >
        if (response.iStartPos == request.iTotalLen)
        {
            m_mapImgUpReq.erase(it);
        }
        else
        {
            request.iStartPos = response.iStartPos;
            if ((request.iTotalLen - response.iStartPos) > UCS_MAX_IMG_SECTION_SIZE)
            {
                request.iDataLen = UCS_MAX_IMG_SECTION_SIZE;
            }
            else
            {
                request.iDataLen = request.iTotalLen - response.iStartPos;
            }

            QByteArray packData = UCSPackage::PackUploadMsgImgRequest(&request);

            sendData(REQ_UPLOAD_MSGIMG, packData);
        }
    }

    ///< 图片数据全部上传成功 >
    /// ///< 发送队列中移除相应的图片上传消息 >
    if (!response.pcClientMsgId.isEmpty())
    {
        removeMsgFromMap(response.pcClientMsgId);
    }

    updateMsgSendStatus(response.pcClientMsgId,
                        SendStatus_success,
                        QString::number(response.iCreateTime),
                        response.pcToUserName);


    UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMManager,
            QStringLiteral("图片上传成功"));
}

void UCSIMManager::handleDownloadImgResponse(QByteArray recvData)
{
    UCSDownloadMsgImgResponse_t response;
    UCSPackage::UnpackDownloadMsgImgResponse(recvData, &response);

}

void UCSIMManager::handleSendCustomMsgResponse(QByteArray recvData)
{
    UCSCustomMsgResponse_t response;
    UCSPackage::UnpackCustomMsgResponse(recvData, &response);

    removeMsgFromMap(response.pcClientMsgId);

    if (response.tBaseResponse.iRet != 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMManager,
                QStringLiteral("自定义消息发送失败"));

        updateMsgSendStatus(response.pcClientMsgId,
                            SendStatus_fail,
                            QString::number(response.iCreateTime),
                            response.pcToUserName);
    }
    else
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMManager,
                QStringLiteral("自定义消息发送成功"));

        updateMsgSendStatus(response.pcClientMsgId,
                            SendStatus_success,
                            QString::number(response.iCreateTime),
                            response.pcToUserName);
    }
}

void UCSIMManager::handleCreateGroupResponse(QByteArray recvData)
{
    UCSCreateGroupResponse_t response;
    UCSPackage::UnpackCreateGroupResponse(recvData, &response);

    if (response.tBaseResponse.iRet != 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMManager,
                QStringLiteral("创建讨论组失败"));

        return;
    }


}

void UCSIMManager::handleAddGroupMemberResponse(QByteArray recvData)
{
    UCSAddGroupMemberResponse_t response;
    UCSPackage::UnpackAddGroupMemberResponse(recvData, &response);

    if (response.tBaseResponse.iRet != 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMManager,
                QStringLiteral("讨论组添加成员失败"));

        return;
    }
}

void UCSIMManager::handleDelGroupMemberResponse(QByteArray recvData)
{
    UCSDelGroupMemberResponse_t response;
    UCSPackage::UnpackDelGroupMemberResponse(recvData, &response);

    if (response.tBaseResponse.iRet != 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMManager,
                QStringLiteral("删除讨论组成员失败"));

        return;
    }
}

void UCSIMManager::handleQuitGroupResponse(QByteArray recvData)
{
    UCSQuitGroupResponse_t response;
    UCSPackage::UnpackQuitGroupResponse(recvData, &response);

    if (response.tBaseResponse.iRet != 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMManager,
                QStringLiteral("退出讨论组失败"));

        return;
    }

}

bool UCSIMManager::sendData(quint32 cmd, QByteArray dataArray)
{
    if (m_tcpState != TcpConnected &&
        m_tcpState != TcpReConnected)
    {
        return false;
    }

    if (dataArray.size() > 0)
    {
        UCSEvent::sendData(new UCSSendDataEvent(cmd, dataArray));
    }

    return true;
}

QString UCSIMManager::userId() const
{
    return m_userId;
}

void UCSIMManager::sendMsgTimeout()
{
    if (m_msgSendQueue.isEmpty())
    {
        return;
    }

    qint64 timeNow = UCSClock::TimeInMicroseconds();
    qint32 timeout = UCSIMTimer::timeout(UCSIMTimer::kMsgSendTimer);
    foreach (ChatEntity entity, m_msgSendQueue)
    {
        if ((timeNow - entity.sendTime.toLongLong()) <= timeout)
        {
            continue;
        }

        MapConditions conditions;
        MapValues values;
        conditions.insert("msgId", entity.msgId);
        values.insert("sendStatus", QString::number(SendStatus_fail));

        UCSDBCenter::chatMgr()->updateChat(entity.targetId,
                                           (UCS_IM_ConversationType)entity.categoryId.toInt(),
                                           values, conditions);

        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kIMManager,
                QStringLiteral("消息发送超时."));

        removeMsgFromMap(entity.msgId);
    }
}

void UCSIMManager::removeMsgFromMap(QString msgId)
{
    if (m_msgSendQueue.contains(msgId))
    {
        m_msgSendQueue.remove(msgId);
    }
}

void UCSIMManager::updateMsgSendStatus(QString msgId,
                                       UCSSendStatus status,
                                       QString time,
                                       QString toUserName)
{
    MapValues values;
    MapConditions conditions;

    conditions.insert("msgId", msgId);
    values.insert("sendStatus", QString::number(status));
    values.insert("receiveTime", time);

    if (toUserName.contains("@chatroom"))
    {
        QString targetId = toUserName.section('@', 0, 0);
        UCSDBCenter::chatMgr()->updateChat(targetId, UCS_IM_GROUPCHAT, values, conditions);
    }
    else if (toUserName.contains("@group"))
    {
        QString targetId = toUserName.section('@', 0, 0);
        UCSDBCenter::chatMgr()->updateChat(targetId, UCS_IM_DISCUSSIONCHAT, values, conditions);
    }
    else
    {
        if (!toUserName.isEmpty())
        {
            UCSDBCenter::chatMgr()->updateChat(toUserName, UCS_IM_SOLOCHAT, values, conditions);
        }
    }
}

void UCSIMManager::handleRecivedMsgList(const QList<UCSIMAddMsg_t> msgList)
{
    if (msgList.isEmpty())
    {
        return;
    }

#if 0
    QList<UCSMessage*> messageList;
#endif

    QMap<QString, qint32> msgCountMap;
    QMap<QString, QList<ChatEntity>> soloList;
    QMap<QString, QList<ChatEntity>> groupList;
    QMap<QString, QList<ChatEntity>> discussionList;
    QMap<QString, QList<ChatEntity>> broadcastList;
    QMap<QString, QList<ChatEntity>> unknownList;

    for (int idx = 0; idx < msgList.size(); ++idx)
    {
        UCSIMAddMsg_t msg = msgList.at(idx);

        if (msg.tFromUserName.isEmpty() ||
            msg.tToUserName.isEmpty() ||
            msg.iCreateTime <= 0 ||
            msg.iMsgType <= 0 ||
            msg.iMsgId <= 0)
        {
            continue;
        }

        ///< 生成通用属性 >
        QString msgId = UCSIMHelper::recvMsgId(idx);
        QString fromId = UCSIMHelper::getFromName(msg.tFromUserName);
        QString senderId = "";
        QString senderNickName = "";

        ///< pcMsgSource格式为: "sendId+sendUserName", 通知消息时，pcMsgSource可能为空 >
        if (msg.pcMsgSource.size())
        {
            if (msg.pcMsgSource.contains("+"))
            {
                senderId = msg.pcMsgSource.section('+', 0, 0);
                senderNickName = msg.pcMsgSource.section('+', 1, 1);
            }
            else
            {
                senderId = msg.pcMsgSource;
            }
        }

        UCS_IM_ConversationType type = UCSIMHelper::getConvTypeByFromUserName(msg.tFromUserName);
#if 0
        UCSMessage *recvMsg = new UCSMessage;
#endif
        ChatEntity chatEntity;
        chatEntity.msgId = msgId;
        chatEntity.msgType = QString::number(msg.iMsgType);
        chatEntity.categoryId = QString::number(type);
        chatEntity.imsgId = QString::number(msg.iMsgId);
        chatEntity.targetId = fromId;
        chatEntity.senderId = senderId;
        chatEntity.senderNickName = senderNickName;
        chatEntity.isFromMySelf = QStringUtil::fromBool(false);
        chatEntity.sendTime = QString::number(msg.iCreateTime);
        chatEntity.receivedTime = QString::number(UCSClock::TimeInMicroseconds());
        chatEntity.readStatus = QString::number(ReceivedStatus_UNREAD);
        chatEntity.sendStatus = QString::number(SendStatus_fail);
        chatEntity.msgLength = "0";

#if 0
        recvMsg->messageId = msgId.toLongLong();
        recvMsg->senderUserId = senderId;
        recvMsg->senderNickName = senderNickName;
        recvMsg->messageDirection = MessageDirection_RECEIVE;
        recvMsg->sendStatus = SendStatus_success;
        recvMsg->conversationType = type;
        recvMsg->time = msg.iCreateTime;
        recvMsg->messageType = (UCS_IM_MsgType)(msg.iMsgType);
#endif

        switch (msg.iMsgType)
        {
        case UCS_IM_TEXT:
        {
            chatEntity.content = msg.tContent;
            chatEntity.msgLength = QString::number(msg.tContent.size());

            UCSTextMsg *txtMsg = new UCSTextMsg;
            txtMsg->content() = msg.tContent;
            txtMsg->pushContent = msg.pcPushContent;
#if 0
            recvMsg->content = txtMsg;
#endif
        }
            break;

        case UCS_IM_IMAGE:
        {
            if (msg.tImgBuf.isEmpty())
            {
                UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kIMManager,
                        QStringLiteral("图片消息解析失败"));
                continue;
            }

            QString thumbnaiPath = UCSIMHelper::saveImage(fromId, msgId, msg.tImgBuf.toLocal8Bit());
            QString remoteUrl = UCSIMHelper::getBigImgUrl(msg.tContent);

            chatEntity.thumbnaiUrl = thumbnaiPath;
            chatEntity.content = remoteUrl;

            UCSImageMsg *imgMsg = new UCSImageMsg;
            imgMsg->imageRemoteUrl = remoteUrl;
            imgMsg->thumbnailLocalPath = thumbnaiPath;
            imgMsg->pushContent = msg.pcPushContent;
#if 0
            recvMsg->content = imgMsg;
#endif
        }
            break;

        case UCS_IM_VOICE:
        {
            qint32 lenInBytes = 0;
            qint32 duration = 0;
            QString pcMsgId;
            UCSIMHelper::getVoiceDownloadInfo(msg.tContent,
                                              lenInBytes,
                                              duration,
                                              pcMsgId);

            if (pcMsgId.isEmpty() ||
                lenInBytes <= 0 ||
                duration <= 0)
            {
                UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kIMManager,
                        QStringLiteral("语音消息解析失败"));
                continue;
            }

            chatEntity.msgLength = QString::number(lenInBytes);
            chatEntity.pcClientMsgId = pcMsgId;
            chatEntity.content = msg.pcPushContent;
            chatEntity.readStatus = QString::number(ReceivedStatus_DOWNLOADING);

#if 0
            UCSVoiceMsg *voiceMsg = new UCSVoiceMsg;
            voiceMsg->pushContent = msg.pcPushContent;
            voiceMsg->duration = duration;            
            recvMsg->content = voiceMsg;
            recvMsg->receivedStatus = ReceivedStatus_DOWNLOADING;
#endif

            ///< 保存语音信息，用于下载失败后的主动下载 >
            VoiceInfoEntity entity;
            entity.iLength = QString::number(lenInBytes);
            entity.iMsgId = QString::number(msg.iMsgId);
            entity.messageId = msgId;
            entity.pcClientMsgId = pcMsgId;
            UCSDBCenter::voiceInfoMgr()->addVoiceInfo(&entity);

            ///< 保存语音信息，用于分包下载 >
            m_voiceEntity.insert(QString::number(msg.iMsgId), entity);

            ///< 启动语音下载 >
            UCSDownloadVoiceRequest_t request;
            request.iMsgId = msgId.toLongLong();
            request.pcClientMsgId = entity.pcClientMsgId;
            request.iOffset = 0;
            request.iLenght = entity.iLength.toInt();

            QByteArray packData = UCSPackage::PackDownloadVoiceRequest(&request);

            sendData(REQ_DOWNLOAD_VOICE, packData);
        }
            break;

        case UCS_IM_Custom:
        {
            QByteArray customData = msg.tImgBuf.toLocal8Bit();
            if (customData.size() <= 0)
            {
                UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kIMManager,
                        QStringLiteral("自定义消息解析失败"));
                continue;
            }

            chatEntity.content = msg.pcPushContent;
            chatEntity.customData = customData;

#if 0
            UCSCustomMsg *customMsg = new UCSCustomMsg;
            customMsg->setData(customData);
            customMsg->pushContent = msg.pcPushContent;
            recvMsg->content = customMsg;
#endif
        }
            break;

        case UCS_IM_System: ///< 系统通知 >
        {
            chatEntity.content = msg.tContent;

#if 0
            UCSDiscussionNotification *notify = new UCSDiscussionNotification;
            notify->m_extension = msg.tContent;
            notify->m_operatorId = recvMsg->receivedId;
            recvMsg->content = notify;
#endif
        }
            break;

        default:
        {
            ///< 当前版本不支持的类型 >
            chatEntity.content = QStringLiteral("当前版本不支持查看此消息，请升级最新版本");
            chatEntity.msgType = QString::number(UCS_IM_UnknownType);
        }
            break;
        }

        switch (type) {
        case UCS_IM_SOLOCHAT:
        {
#if 0
            recvMsg->receivedId = msg.tToUserName; ///< 单聊时接收者是自己 >
#endif
            soloList[fromId].append(chatEntity);
        }
            break;

        case UCS_IM_GROUPCHAT:
        {
#if 0
            recvMsg->receivedId = fromId; ///< 群组消息的接收者是群组Id。类似于123@chatroom >
#endif
            groupList[fromId].append(chatEntity);
        }
            break;

        case UCS_IM_DISCUSSIONCHAT:
        {
#if 0
            recvMsg->receivedId = fromId;  ///< 讨论组消息的接收者是讨论组Id。类似于123@group >
#endif
            discussionList[fromId].append(chatEntity);
        }
            break;

        case UCS_IM_Broadcast:
        {
#if 0
            ///< 广播消息的接收者是自己 >
            recvMsg->receivedId = UCSIMHelper::readSettings(UCS_LOGIN_USERID_KEY).toString();
#endif
            broadcastList[fromId].append(chatEntity);
        }
            break;

        case UCS_IM_UnknownConversationType:
        {
#if 0
            recvMsg->receivedId = fromId;    ///< 未知会话一样是 @xx >
#endif
            unknownList[fromId].append(chatEntity);
        }
            break;

        case UCS_IM_SYSTEMMSG:   ///< 未使用 >
        default:
            break;
        }
#if 0
        messageList.append(recvMsg);
#endif
        ++msgCountMap[chatEntity.targetId];
    }

    ///< 聊天记录入库 >
    saveConversationAndChat(soloList, UCS_IM_SOLOCHAT);
    saveConversationAndChat(groupList, UCS_IM_GROUPCHAT);
    saveConversationAndChat(discussionList, UCS_IM_DISCUSSIONCHAT);
    saveConversationAndChat(broadcastList, UCS_IM_Broadcast);
    saveConversationAndChat(unknownList, UCS_IM_UnknownConversationType);

    ///< 消息回调 >
    if (msgCountMap.size() > 0)
    {
        UCSEvent::postEvent(m_pReceiver,
                            new UCSMsgSyncEvent(ErrorCode_NoError,
                                                msgCountMap));
    }

#if 0
    if (messageList.size() > 0)
    {
        UCSEvent::postEvent(m_pReceiver,
                            new UCSMsgSyncEvent(ErrorCode_NoError,
                                                messageList));
    }

    qDeleteAll(messageList.begin(), messageList.end());
    messageList.clear();
#endif
}

void UCSIMManager::saveConversationAndChat(QMap<QString, QList<ChatEntity> > chatListMap, UCS_IM_ConversationType type)
{
    QMapIterator<QString, QList<ChatEntity>> iter(chatListMap);
    while (iter.hasNext())
    {
        iter.next();
        QString targetId = iter.key();
        QList<ChatEntity> chatList = iter.value();
        if (!chatList.isEmpty())
        {
            UCSDBCenter::chatMgr()->addChat(targetId, type, chatList);

            ChatEntity entity = chatList.last();
            UCSDBCenter::conversationMgr()->addConversation(&entity, chatList.size());
        }
    }
}

void UCSIMManager::slot_timeOut(UCSIMTimer::ImTimerId id)
{
    switch (id) {
    case UCSIMTimer::kMsgSendTimer:
    {
        sendMsgTimeout();
    }
        break;

    case UCSIMTimer::kCreateGroupTimer:
    {

    }
        break;

    case UCSIMTimer::kAddGroupMemberTimer:
    {

    }
        break;

    case UCSIMTimer::kDelGroupMemberTimer:
    {

    }
        break;

    case UCSIMTimer::kQuitGroupTimer:
    {

    }
        break;

    case UCSIMTimer::kSetGroupTopicTimer:
    {

    }
        break;

    default:
        break;
    }
}
