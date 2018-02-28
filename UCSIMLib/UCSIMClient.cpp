#include "UCSIMClient.h"
#include <QMutexLocker>
#include "UCSTcp/UCSLogger.h"
#include "UCSTcp/UCSTcpSDK.h"
#include "UCSTcp/UCSEvent.h"
#include "UCSIMVersion.h"
#include "UCSIMManager.h"
#include "Common/UCSIMHelper.h"

QMutex UCSIMClient::m_Mutex;
QSharedPointer<UCSIMClient> UCSIMClient::m_pInstance = Q_NULLPTR;

QSharedPointer<UCSIMClient> &UCSIMClient::Instance()
{
    if (m_pInstance.isNull())
    {
        QMutexLocker locker(&m_Mutex);
        if (m_pInstance.isNull())
        {
            m_pInstance = QSharedPointer<UCSIMClient>(new UCSIMClient);
        }
    }

    return m_pInstance;
}

UCSIMClient::UCSIMClient(QObject *parent)
    : QObject(parent)
{
    m_pIManager = new UCSIMManager();

    initConnections();
}

void UCSIMClient::init()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    UCSTcpClient::Instance()->setIMClient(this);

    m_pIManager->init();
}

UCSIMClient::~UCSIMClient()
{
    if (m_pIManager != Q_NULLPTR)
    {
        delete m_pIManager;
        m_pIManager = NULL;
    }
}

QString UCSIMClient::version()
{
    return UCS_IM_CLIENT_VERSION;
}

void UCSIMClient::setIMReceiver(QObject *receiver)
{
    m_pIManager->setIMReceiver(receiver);
}

bool UCSIMClient::sendMessage(UCSMessage *pMessage)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    if (pMessage == Q_NULLPTR)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("发送消息参数异常"));
        return false;
    }

    if (!UCSIMHelper::checkConversationType(pMessage->conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return false;
    }

    if (!UCSIMHelper::checkMsgType(pMessage->messageType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的发送消息类型"));

        return false;
    }

    if ((pMessage->receivedId.size() <= 0) ||
         pMessage->content == Q_NULLPTR)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("发送消息接收者或内容错误"));

        return false;
    }

    return m_pIManager->doSendMessage(pMessage);
}

bool UCSIMClient::downloadVoiceWithConversationType(UCS_IM_ConversationType conversationType,
                                                    QString targetId,
                                                    quint64 messageId)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return false;
    }

    if (targetId.isEmpty() || messageId <= 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("语音消息下载参数无效"));
        return false;
    }

    return m_pIManager->doDownloadVoice(conversationType, targetId, messageId);
}

bool UCSIMClient::createDiscussionWithTopic(QString topic,
                                            QList<UCSUserInfo> memberList)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    if (topic.isEmpty() || memberList.size() == 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("讨论组创建参数无效"));
        return false;
    }
    return m_pIManager->doCreateDiscussion(topic, memberList);
}

bool UCSIMClient::addMemberToDiscussionWithDiscussionId(QString discussionId,
                                                        QList<UCSUserInfo> memberList)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    if (discussionId.isEmpty() || memberList.size() == 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("讨论组成员添加参数无效"));
        return false;
    }

    return m_pIManager->doAddMemberToDiscussion(discussionId, memberList);
}

bool UCSIMClient::removeMemberFromDiscussionWithDiscussionId(QString discussionId,
                                                             QList<UCSUserInfo> memberList)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    if (discussionId.isEmpty() || memberList.size() == 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("讨论组成员踢除参数无效"));
        return false;
    }

    return m_pIManager->doRemoveMemberFromDiscussion(discussionId, memberList);
}

bool UCSIMClient::quitDiscussionWithDiscussionId(QString discussionId)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    if (discussionId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("退出讨论组参数无效"));

        return false;
    }

    return m_pIManager->doQuitDiscussion(discussionId);
}

bool UCSIMClient::setDiscussionTopicWithDiscussionId(QString discussionId,
                                                     QString newTopic)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    if (discussionId.isEmpty() || newTopic.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("修改讨论组名称参数无效"));

        return false;
    }

    return m_pIManager->doSetDiscussionTopic(discussionId, newTopic);
}

UCSDiscussion UCSIMClient::getDiscussionInfoWithDiscussionId(QString targetId)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);
    UCSDiscussion discussion;
    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("讨论组Id参数无效"));
        return discussion;
    }

    m_pIManager->doGetDiscussionWithId(targetId, discussion);

    return discussion;
}

QList<UCSDiscussion> UCSIMClient::getDiscussions()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    QList<UCSDiscussion> discussionList;

    m_pIManager->doGetAllDiscussions(discussionList);

    return discussionList;
}

QList<UCSConversation*> UCSIMClient::getConversationList(UCS_IM_ConversationListType conversationListType)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    QList<UCSConversation*> conversationList;

    m_pIManager->doGetConversationList(conversationListType, conversationList);
    return conversationList;
}

bool UCSIMClient::removeConversation(UCS_IM_ConversationType conversationType,
                                     QString targetId)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return false;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return false;
    }

    return m_pIManager->doRemoveConversation(conversationType, targetId);
}

bool UCSIMClient::clearMessages(UCS_IM_ConversationType conversationType,
                                QString targetId)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return false;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return false;
    }

    return m_pIManager->doClearMessages(conversationType, targetId);
}

bool UCSIMClient::clearConversations()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    return m_pIManager->doClearConversations();
}

QList<UCSMessage*> UCSIMClient::getLatestMessages(UCS_IM_ConversationType conversationType,
                                                   QString targetId,
                                                   quint32 count)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    QList<UCSMessage*> messageList;

    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return messageList;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return messageList;
    }

    if (count == 0)
    {
        count = 20;
    }

    m_pIManager->doGetLastestMessages(conversationType,
                                     targetId,
                                     count,
                                     messageList);

    return messageList;
}

QList<UCSMessage*> UCSIMClient::getHistoryMessages(UCS_IM_ConversationType conversationType,
                                                    QString targetId,
                                                    quint64 oldestMessageId,
                                                    quint32 count)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    QList<UCSMessage*> messageList;

    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return messageList;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return messageList;
    }


    m_pIManager->doGetHistoryMessages(conversationType,
                                     targetId,
                                     oldestMessageId,
                                     count,
                                     messageList);

    return messageList;
}

bool UCSIMClient::deleteMessage(UCS_IM_ConversationType conversationType,
                                 QString targetId,
                                 quint64 messageId)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);
    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return false;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return false;
    }

    return m_pIManager->doDeleteMessage(conversationType, targetId, messageId);
}

qint32 UCSIMClient::getUnreadCount(UCS_IM_ConversationType conversationType,
                                   QString targetId)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);
    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return 0;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return 0;
    }

    return m_pIManager->doGetUnReadCount(conversationType, targetId);
}

qint32 UCSIMClient::getTotalUnreadCount()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    return m_pIManager->doGetAllUnReadCount();
}

bool UCSIMClient::clearConversationsUnreadCount(UCS_IM_ConversationType conversationType,
                                                QString targetId)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);
    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return 0;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return 0;
    }

    return m_pIManager->doClearUnReadCount(conversationType, targetId);
}

QList<UCSMessage*> UCSIMClient::checkChatMessages(UCS_IM_ConversationType conversationType,
                                                   QString targetId,
                                                   QString keyWord)
{    
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    QList<UCSMessage*> messageList;
    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return messageList;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return messageList;
    }

    m_pIManager->doSearchMessage(conversationType, targetId, keyWord, messageList);

    return messageList;
}

bool UCSIMClient::setConversationToTop(UCS_IM_ConversationType conversationType,
                                       QString targetId,
                                       bool isTop)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);
    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return false;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return false;
    }

    return m_pIManager->doSetTop(conversationType, targetId, isTop);
}

bool UCSIMClient::setMessageReceivedStatus(UCS_IM_ConversationType conversationType,
                                           QString targetId,
                                           quint64 messageId,
                                           UCSReceivedStatus receivedStatus)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);
    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return false;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return false;
    }

    return m_pIManager->doUpdateReceivedStatus(conversationType,
                                               targetId,
                                               messageId,
                                               receivedStatus);
}

bool UCSIMClient::reset()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);

    UCSTcpClient::Instance()->doLogout();

    return m_pIManager->doReset();
}

QString UCSIMClient::getTextMessageDraft(UCS_IM_ConversationType conversationType,
                                         QString targetId)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);
    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return "";
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return "";
    }
    return m_pIManager->doGetDraft(conversationType, targetId);
}

bool UCSIMClient::saveTextMessageDraft(UCS_IM_ConversationType conversationType,
                                       QString targetId,
                                       QString content)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);
    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return false;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return false;
    }

    return m_pIManager->doUpdateDraft(conversationType, targetId, content);
}

bool UCSIMClient::clearTextMessageDraft(UCS_IM_ConversationType conversationType,
                                        QString targetId)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kIMClient, __FUNCTION__);
    if (!UCSIMHelper::checkConversationType(conversationType))
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("无效的会话类型"));

        return false;
    }

    if (targetId.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kIMClient,
                QStringLiteral("会话Id参数无效"));
        return false;
    }

    return m_pIManager->doUpdateDraft(conversationType, targetId, "");
}


void UCSIMClient::customEvent(QEvent *event)
{
    if (event->type() == kIMDataEvent)
    {
        UCSRecvDataEvent *recvEvt = (UCSRecvDataEvent*)event;
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMClient,
                QString("customEvent kIMDataEvent cmd = %1").arg(recvEvt->cmd()));

        m_pIManager->processRecvData(recvEvt->cmd(), recvEvt->data());
    }
    else if(event->type() == kTcpStateEvent)
    {
        UCSTcpStateEvent *evt = (UCSTcpStateEvent*)event;
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMClient,
                QString("customEvent kTcpStateEvent state = %1")
                .arg(evt->state()));

        m_pIManager->updateTcpState(evt->state());
    }
    else if (event->type() == kLoginStateEvent)
    {
        UCSLoginStateEvent *evt = (UCSLoginStateEvent*)event;
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kIMClient,
                QString("customEvent kLoginStateEvent state = %1 userid = %2")
                .arg(evt->state())
                .arg(evt->userId()));

        m_pIManager->updateLoginState(evt->state(), evt->userId());
    }
}

void UCSIMClient::initConnections()
{

}
