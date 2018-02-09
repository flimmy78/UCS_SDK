#ifndef UCSDBCENTER_H
#define UCSDBCENTER_H

#include <QObject>
#include <qglobal.h>
#include <QMutex>
#include "UCSTcp/UCSTcpDefine.h"
#include <UCSUserInfoDBManager.h>
#include <UCSVoiceInfoDBManager.h>
#include <UCSGroupDBManager.h>
#include <UCSDiscussionDBManager.h>
#include <UCSConversationDBManager.h>
#include <UCSChatDBManager.h>

class UCSDBCenter : public QObject
{
    Q_OBJECT
public:    
    static void release();
    static UserInfoDBManager *userInfoMgr();
    static VoiceInfoDBManager *voiceInfoMgr() ;
    static GroupDBManager *groupMgr();
    static DiscussionDBManager *discussionMgr();
    static ConversationDBManager *conversationMgr();
    static ChatDBManager *chatMgr();
    static void updateLoginState(UcsLoginState state);

protected:
    void customEvent(QEvent *event) override;    

private:
    static UCSDBCenter *getInstance();

    explicit UCSDBCenter(QObject *parent = 0);
    /* do nothing, ensure single instance */
    UCSDBCenter(const UCSDBCenter &);
    UCSDBCenter& operator =(const UCSDBCenter&);

    virtual ~UCSDBCenter();

signals:

public slots:

private:
    UserInfoDBManager       *m_pUserMgr;
    VoiceInfoDBManager      *m_pVoiceMgr;
    GroupDBManager          *m_pGroupMgr;
    DiscussionDBManager     *m_pDiscussionMgr;
    ConversationDBManager   *m_pConversationMgr;
    ChatDBManager           *m_pChatMgr;

private:
    static UCSDBCenter *m_pInstance;
    static QMutex m_Mutex;
};

#endif // UCSDBCENTER_H
