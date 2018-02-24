#ifndef UCSCOMMONTYPES_H
#define UCSCOMMONTYPES_H

#include <QObject>
#include <QEvent>
#include <qglobal.h>
#include "ucstcpsdk_global.h"

/*!
 * \brief UCSCustomEventType SDK自定义事件，使用事件类型范围为
 * (QEvent::User + 20000) -- (QEvent::User + 30000),
 * 请勿占用此范围内事件类型
 */
typedef enum
{
    UCSCustomEventBase = QEvent::User + 20600,
    /*
     * TCP 连接事件类型
    */
    kUCSConnectStatusEvent = UCSCustomEventBase,    ///< TCP状态通知 >

    /*
     * 登录事件类型
    */
    kUCSLoginEvent = UCSCustomEventBase + 1,   ///< 登录状态通知 >
} UCSCustomEventType;

typedef enum
{
    UCSConnectionStatus_LoginSuccess = 0,   ///< 登录成功 >
    UCSConnectionStatus_ConnectFail,        ///< 连接失败 >
    UCSConnectionStatus_AbnormalDisconnection,  ///< 异常断开 >
    UCSConnectionStatus_StartReConnect,     ///< 开始重连 >
    UCSConnectionStatus_ReConnecting,       ///< 正在重连 >
    UCSConnectionStatus_ReConnectSuccess,   ///< 重连成功 >
    UCSConnectionStatus_ReConnectFail,      ///< 重连失败 >
    UCSConnectionStatus_BeKickedOut,        ///< 被踢线 >
    UCSConnectionStatus_SignOut,            ///< 主动退出登录 >
} UCSConnectionStatus;

typedef enum
{
    ErrorCode_NoError   = 400000,       ///< 没有错误 >
    ErrorCode_UnKnown   = 401000,       ///< 未知错误 >

    ErrorCode_ConnectServerFail = 401100,   ///< 连接服务器失败 >
    ErrorCode_TimeOut           = 401101,   ///< 超时 >
    ErrorCode_BeKickedOut       = 401102,   ///< 被踢线 >
    ErrorCode_InvalidToken      = 401103,   ///< 无效的token或与appid不相符 >
    ErrorCode_InvalidUser       = 401104,   ///< 用户不存在 >
    ErrorCode_IncorrectPwd      = 401105,   ///< 密码错误 >

    ErrorCode_InvalidMessage            = 402300,   ///< 无效的消息 >
    ErrorCode_InvalidGroup              = 402301,   ///< 无效的群组，传入的群组不存在 >
    ErrorCode_InvalidDiscussion         = 402302,   ///< 无效的讨论组，传入的讨论组不存在 >
    ErrorCode_UpdateDiscussionNameFail  = 402303,   ///< 修改讨论组名称失败 >
    ErrorCode_CreateDiscussionFail      = 402304,   ///< 创建讨论组失败 >
    ErrorCode_MemberIsNotInGroup        = 402305,   ///< 用户不在群组内 >
    ErrorCode_MemberIsNotInDiscussion   = 402306,   ///< 用户不在讨论组内 >
    ErrorCode_DeleteMemberFail          = 402307,   ///< 踢除成员失败 >
    ErrorCode_AddMemberFail             = 402308,   ///< 邀请成员失败 >
    ErrorCode_UploadFileFail            = 402309,   ///< 上传文件失败 >
    ErrorCode_downloadFileFail          = 402310,   ///< 下载文件失败 >
    ErrorCode_RecoderTooShort           = 402311,   ///< 录音时间过短 >
    ErrorCode_FileFormatNotSupported    = 402312,   ///< 文件格式不支持 >
    ErrorCode_MessageLengthTooLong      = 402313,   ///< 消息内容过长 >
    ErrorCode_QueryDataBaseFail         = 402314,   ///< 查询数据库失败 >
    ErrorCode_UpateDataBaseFail         = 402315,   ///< 插入或更新数据库失败 >
    ErrorCode_SendMessageFail           = 402316,   ///< 消息发送失败 >
    ErrorCode_SendMessageTimeout        = 402317,   ///< 消息发送超时 >
    ErrorCode_MembersError              = 402318,   ///< 成员列表错误 >
    ErrorCode_QuitDiscussionFail        = 402319,   ///< 退出讨论组失败 >

    ErrorCode_MethodParamError          = 403600,   ///< 函数参数错误 >
    ErrorCode_MessageTypeError          = 403601,   ///< 消息格式错误 >
    ErrorCode_NetworkNotConnected       = 403602,   ///< 网络未连接 >
    ErrorCode_InitError                 = 403603,   ///< 初始化错误 >
    ErrorCode_ReConnectServerFail       = 403604,   ///< 重连服务器失败 >
    ErrorCode_IPAddressError            = 403605,   ///< 错误IP地址 >
    ErrorCode_PacketParseError          = 403606,   ///< 包解析错误 >
    ErrorCode_PullProxyError            = 403607,   ///< 拉取CPS数据失败 >
    ErrorCode_LoginInfoError            = 403608,   ///< 登录信息错误 >

    ///< 透传数据相关错误码 >
    ErrorCode_ReceiverOffLine           = 405000,   ///< 透传数据时对方离线 >
    ErrorCode_UnReachable               = 405001,   ///< 透传数据时数据不可达 >
    ErrorCode_LastTimeNoResponse        = 405002,   ///< 上一次透传请求未响应 >
    ErrorCode_ClearDataFail             = 405003,   ///< 清除数据失败 >
    ErrorCode_UnSupportVersion          = 405004,   ///< 对方版本不支持 >
    ErrorCode_TcpDisconnect             = 405005,   ///< TCP连接已断开 >
    ErrorCode_InvalidData               = 405006,   ///< 无效透传数据 >
    ErrorCode_EncryptDataFail           = 405007,   ///< 加密透传数据失败 >
    ErrorCode_EmptyData                 = 405008,   ///< 透传数据为空 >
    ErrorCode_QueryReceiverStatusFail   = 405009,   ///< 查询接收方在线状态失败 >
    ErrorCode_DataException             = 405010,   ///< 数据异常 >
} UCSErrorCode;

/*!
 * \class UCSConnectStatusEvent
 * \brief TCP 状态更新事件
*/
class UCSTCPSDKSHARED_EXPORT UCSConnectStatusEvent : public QEvent
{
public:
    explicit UCSConnectStatusEvent(UCSConnectionStatus status);
    ~UCSConnectStatusEvent() {}

    UCSConnectionStatus status() const;

private:
    UCSConnectionStatus m_status;
};

/*!
 * \class UCSLoginStatusEvent
 * \brief 登录 状态更新事件
*/
class UCSTCPSDKSHARED_EXPORT UCSLoginEvent : public QEvent
{
public:
    explicit UCSLoginEvent(UCSErrorCode error,
                                 QString userId);
    ~UCSLoginEvent() {}

    QString userId() const;

    UCSErrorCode error() const;

private:
    QString m_userId;
    UCSErrorCode m_error;
};

#endif // UCSCOMMONTYPES_H
