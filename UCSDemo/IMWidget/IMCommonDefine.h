#ifndef IMCOMMONDEFINE_H
#define IMCOMMONDEFINE_H

#include <QObject>
#include <qglobal.h>
#include "Interface/UCSIMDefine.h"

typedef enum
{
    SoloTypeConversation = 0,   ///< 单聊会话 >
    GroupTypeConversation,      ///< 群聊会话，包括群组或讨论组 >
    OtherTypeConversation,      ///< 其他会话类型 >
} ConversationItemType;

/*!
 * \brief IM聊天会话项
*/
typedef struct
{
    QString conversationId;     ///< 会话Id >
    UCS_IM_ConversationType conversationType;   ///< 会话类型 >
    QString headerPath;         ///< 会话图像 >
    QString conversationTitle;   ///< 会话名称 >
    QString lastMsgTime;        ///< 会话最后一条消息时间 >
    QString lastMsgContent;     ///< 会话最后一条消息内容 >
    quint32 unReadCount;        ///< 会话未读消息数 >
    bool isTop;                 ///< 是否置顶会话 >
    ConversationItemType itemType;  ///< 会话项类型 >
} ConversationItem;

#endif // IMCOMMONDEFINE_H
