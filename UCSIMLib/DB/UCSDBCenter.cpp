#include "UCSDBCenter.h"
#include <QMutexLocker>
#include <UCSConnectionPool.h>

UCSDBCenter *UCSDBCenter::m_pInstance = Q_NULLPTR;
QMutex UCSDBCenter::m_Mutex;

UCSDBCenter *UCSDBCenter::getInstance()
{
    if (m_pInstance == Q_NULLPTR)
    {
        QMutexLocker locker(&m_Mutex);
        if (m_pInstance == Q_NULLPTR)
        {
            m_pInstance = new UCSDBCenter();
        }
    }

    return m_pInstance;
}

UCSDBCenter::UCSDBCenter(QObject *parent)
    : QObject(parent)
{
    m_pUserMgr = new UserInfoDBManager();
    m_pVoiceMgr = new VoiceInfoDBManager();
    m_pGroupMgr = new GroupDBManager();
    m_pDiscussionMgr = new DiscussionDBManager();
    m_pConversationMgr = new ConversationDBManager(m_pDiscussionMgr);
    m_pChatMgr = new ChatDBManager();
}

UCSDBCenter::~UCSDBCenter()
{
    if (m_pUserMgr != Q_NULLPTR)
    {
        delete m_pUserMgr;
        m_pUserMgr = Q_NULLPTR;
    }

    if (m_pVoiceMgr != Q_NULLPTR)
    {
        delete m_pVoiceMgr;
        m_pVoiceMgr = Q_NULLPTR;
    }

    if (m_pGroupMgr != Q_NULLPTR)
    {
        delete m_pGroupMgr;
        m_pGroupMgr = Q_NULLPTR;
    }

    if (m_pDiscussionMgr != Q_NULLPTR)
    {
        delete m_pDiscussionMgr;
        m_pDiscussionMgr = Q_NULLPTR;
    }

    if (m_pConversationMgr != Q_NULLPTR)
    {
        delete m_pConversationMgr;
        m_pConversationMgr = Q_NULLPTR;
    }

    if (m_pChatMgr != Q_NULLPTR)
    {
        delete m_pChatMgr;
        m_pChatMgr = Q_NULLPTR;
    }

    UCSConnectionPool::release();
}

void UCSDBCenter::release()
{
    QMutexLocker locker(&m_Mutex);
    if (m_pInstance != Q_NULLPTR)
    {
        delete m_pInstance;
        m_pInstance = Q_NULLPTR;
    }
}

UserInfoDBManager *UCSDBCenter::userInfoMgr()
{
    UCSDBCenter *center = getInstance();
    return center->m_pUserMgr;
}

VoiceInfoDBManager *UCSDBCenter::voiceInfoMgr()
{
    UCSDBCenter *center = getInstance();
    return center->m_pVoiceMgr;
}

GroupDBManager *UCSDBCenter::groupMgr()
{
    UCSDBCenter *center = getInstance();
    return center->m_pGroupMgr;
}

DiscussionDBManager *UCSDBCenter::discussionMgr()
{
    UCSDBCenter *center = getInstance();
    return center->m_pDiscussionMgr;
}

ConversationDBManager *UCSDBCenter::conversationMgr()
{
    UCSDBCenter *center = getInstance();
    return center->m_pConversationMgr;
}

ChatDBManager *UCSDBCenter::chatMgr()
{
    UCSDBCenter *center = getInstance();
    return center->m_pChatMgr;
}

void UCSDBCenter::customEvent(QEvent *event)
{
    Q_UNUSED(event);
}

void UCSDBCenter::updateLoginState(UcsLoginState state)
{
    if (state != LoginSuccessed && state != ReLoginSuccessed)
    {
        ///< 退出登录，释放数据库连接池 >
        UCSConnectionPool::release();
    }
}

