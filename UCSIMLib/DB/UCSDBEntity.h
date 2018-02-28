#ifndef UCSDBENTITY_H
#define UCSDBENTITY_H
#include <QObject>
#include <QString>
#include <QMap>
#include <QVariant>
#include <UCSIMDefine.h>
#include <UCSConversation.h>
#include <UCSTcp/UCSStackDefine.h>

///< 更新时用到的键值对，为此，所有实体的字段都用QString表示 >
typedef QMap<QString, QString> MapValues, MapConditions;

typedef enum
{
    UCSDiscussionState_normal = 0,  ///< 正常 >
    UCSDiscussionState_kickout = 1, ///< 被踢出 >
    UCSDiscussionState_quit = 2,    ///< 主动退出 >
} UCSDiscussionState;

typedef struct
{
    QString userId;
    QString userName;
    QString categoryId;
    QString portraitUrl;
    QString updateTime;
    QString userSettings;

    QString extra1;
    QString extra2;
    QString extra3;
    QString extra4;
    QString extra5;
    QString extra6;
} UserInfoEntity;

typedef struct
{
    QString groupId;
    QString groupName;
    QString categoryId;
    QString updateTime;

    QString extra1;
    QString extra2;
    QString extra3;
    QString extra4;
    QString extra5;
    QString extra6;
} GroupEntity;

typedef struct
{
    QString discussionId;       ///< targetId >
    QString discussionName;     ///< 讨论组名称 >
    QString categoryId;
    QString ownerId;            ///< 群主 >
    QString memberCount;
    QString members;
    QString sSettings;
    QString createTime;
    QString state;   ///< 当前在讨论组中的状态 >

    QString extra1;
    QString extra2;
    QString extra3;
    QString extra4;
    QString extra5;
    QString extra6;
} DiscussionEntity;

/*!
 * \struct ConversationEntity
 * \brief 最近聊天会话实体
 */
typedef struct
{
    QString indexId;            ///< 序列Id >
    QString targetId;           ///< 会话的Id >
    QString conversationTitle;  ///< 会话的名称 >
    QString categoryId;         ///< 会话的类型，单聊，群聊，讨论组 >
    QString draft;              ///< 会话待发送草稿 >
    QString isTop;              ///< 会话是否置顶 >
    QString lastTime;           ///< 最后一条消息的时间，会同步服务器时间，同步不成功，为本地时间。 >
                                ///< 用作UI显示，发送时为本地时间同步服务器时间后的时间，接收时为服务器时间 >
    QString topTime;            ///< 置顶时间 >
    QString unReadMsgCount;     ///< 未读消息数量 >
    QString lastestMessageId;   ///< 最后一条消息的Id,消息不存在时为空 >
    QString localTime;          ///< 会话的最后动态时间，以本地时间为准，用于本地排序 >

    QString extra1;
    QString extra2;
    QString extra3;
    QString extra4;
    QString extra5;
    QString extra6;
} ConversationEntity;

/*!
 * \struct ChatEntity
 * \brief 每条聊天消息的实体
*/
typedef struct
{
    QString msgId;                  ///< 消息的唯一标识 >
    QString imsgId;                 ///< 暂时不用，从服务器收到的每一条消息的imsgId是唯一的，所以可以用这个来区分消息 >
    QString targetId;               ///< 消息所在聊天会话的Id >
    QString senderId;               ///< 消息的发送者Id >
    QString senderNickName;         ///< 消息的发送者昵称 >
    QString categoryId;  ///< 消息所在聊天会话的类型，单聊，群聊，讨论组聊 >
    QString isFromMySelf;           ///< 是否自己发送的 >
    QString sendTime;               ///< 发送时间 >
    QString receivedTime;           ///< 接收时间 >
    QString msgType;         ///< 消息类型 >
    QString content;                ///< 消息的内容，文字为文字，图片为图片的本地路径，语音的本地路径。接收时，也作为图片的远程地址 >
    QString msgLength;              ///< 消息的长度 >
    QString sendStatus;       ///< 发送的状态 >
    QString readStatus;   ///< 读状态 >
    QString thumbnaiUrl;            ///< 缩略图的本地路径，包括地图缩略图 >
    QString pcClientMsgId;          ///< 从服务器拿到的标识,这里主要作用是语音下载完后的语音查找检验 >

    QString address;                ///< 地图的位置描述 >
    QString latitude;               ///< 纬度 >
    QString longitude;              ///< 经度 >
    QString coordType;              ///< 类型 >

    QString customData;          ///< 自定义消息体 >

    QString extra1;
    QString extra2;
    QString extra3;
    QString extra4;
    QString extra5;
    QString extra6;
} ChatEntity;

/*!
 * \brief 一条语音的下载信息
*/
typedef struct
{
    QString messageId;       /// < 对应的消息Id >
    QString iMsgId;         ///< 从服务器得到的imsgId >
    QString iLength;        ///< 请求长度，总长度 >
    QString pcClientMsgId;  ///< 从服务器得到的clientMsgId >
} VoiceInfoEntity;

/*!
 * \brief 用于保存接收到语音的时候，记录接收的内容，用于语音分包
*/
typedef struct
{
    QString targetId;
    QString messageId;
    QString pcClientMsgId;
    QString conversationType;
} VoiceDownloadEntity;

class UCSMessage;
class UCSDiscussion;

class UCSDBEntity
{
public:
    explicit UCSDBEntity() {}

    static void convertMessageFromChat(const ChatEntity &chatEntity, UCSMessage *message);

    static void convertDiscussionFromEntity(const DiscussionEntity entity, UCSDiscussion &discussion);

    static void convertConversationFromEntity(const ConversationEntity &entity, UCSConversation *conversation);

    static void convertEntityFromContact(const UCSIMModContact_t contact, DiscussionEntity &entity);
};

#endif // UCSDBENTITY_H
