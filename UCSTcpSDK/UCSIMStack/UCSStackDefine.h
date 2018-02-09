#ifndef UCSSTACKDEFINE_H
#define UCSSTACKDEFINE_H
#include <QString>
#include <QObject>

/*!
 * \struct UCSBaseRequest_t
 * \brief  基本请求结构
 */
typedef struct
{
    QString sSessionKey;        ///< SESSION KEY >
    quint32 iUin;
    QString cDeviceId;          ///< 设备ID >
    qint32  iClientVersion;     ///< 客户端版本号 >
    QString sDeviceType;        ///< 设备类型 >
    quint32 iScene;             ///< 场景标识符（参考enSceneStatus宏定义） >
    quint32 iSeq;
} UCSBaseRequest_t;

/*!
 * \struct UCSBaseResponse_t
 * \brief 基本响应结构
*/
typedef struct
{
    qint32 iRet;
    QString tErrMsg;
    quint32 iSeq;
} UCSBaseResponse_t;

/*!
 * \struct UCSAuthRequest_t
 * \brief 登录请求结构
 */
typedef struct
{
    QString tAccount;           ///< 登录账号(用户名目前支持绑定邮箱或手机) >
    QString tPwd;               ///< 登录密码(需要MD5)，如果是第三方用户则填写token >
    QString pcTimeZone;         ///< 时区 >
    QString pcLanguage;         ///< 语言 >
    QString pcAuthTicket;       ///< 保留 >
    QString pcRealCountry;      ///< 登录时所在的国家 >
    QString tRandomEncryKey;    ///< 客户端产生的随机加密密钥 >
    QString tDevSid;            ///< 开发者ID >
    QString tDevPwd;            ///< 开发者token >
    QString tLoginToken;        ///< 从AS获取的登录token >
} UCSAuthRequest_t;

/*!
 * \struct UCSAuthResponse_t
 * \brief 登录响应结构
*/
typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    quint32 iUin;               ///< 用户的Uin >
    QString tUserName;          ///< AS 用户体系userid >
    QString tNickName;          ///< 用户昵称 >
    QString tBindEmail;         ///< 用户绑定的邮箱 >
    QString tBindMobile;        ///< 用户绑定的手机 >
    qint32 iStatus;             ///< 参照enIGGStatus >
    QString sSessionKey;        ///< 服务端产生的session key >
    QString tImgBuf;            ///< 用户的小头像信息 >
    quint32 iNewVersion;        ///< 新版本信息，0表示当前客户端已经是最新 >
    quint32 iRegType;           ///< 用户账号的类型，参照enIGGRegType >
    quint32 iProfileFlag;       ///< 默认填1 >
    quint32 iUserSex;           ///< 用户性别 >
    quint32 iUserStatus;        ///< 用户状态,参照enUserStatus >
    QString tFirstName;         ///< first name >
    QString tLastName;          ///< last name >
    QString tBirthdate;         ///< 用户出生日期 >
    QString tWebProxyInfo;      ///< 短连接服务器的IP信息 >
    QString tSafeUserName;      ///< 用户的safeusername >
    QString tClientNumber;
    QString tAppid;
}  UCSAuthResponse_t;

/*!
 * \struct UCSReAuthRequest_t
 * \brief 重登陆请求结构
*/
typedef struct
{
    UCSBaseRequest_t tBaseRequest;
} UCSReAuthRequest_t;

/*!
 * \struct UCSReAuthResponse_t
 * \brief 重登陆响应结构
*/
typedef struct
{
    UCSBaseResponse_t tBaseReponse;
    quint32 iUin;
    quint32 iNewVersion;
    QString tWebProxyInfo;
} UCSReAuthResponse_t;

/*!
 * \struct UCSMicroMsgRequest_t
 * \brief 基本消息发送请求结构
*/
typedef struct
{
    QString tFromUserName;      ///< 客户端当前登录的用户名字,优先填写userid,若无填clientNumber >
    QString tToUserName;        ///< 此条消息发送的目标用户名字 >
    quint32 iType;              ///< 消息类型，详见mmsyncdef.h >
    QString pcContent;          ///< 表情数据，客户端根据用户输入获取 >
    quint32 iCreateTime;        ///< 创建此条消息的时间戳 >
    QString pcClientMsgId;      ///< 消息唯一标识，暂用(mmtestproxy_fuin_tuin_时间戳(精确到毫秒,以免发送同一秒内多条消息标识一样的情况)) >
    QString pcMsgSource;        ///< 消息内容，用户输入的数据 >
    quint32 iEmojiFlag;         ///< 是否是自定义表情，客户端判断 >
} UCSMicroMsgRequest_t;

typedef struct
{
    UCSBaseRequest_t tBaseRequest;  /* 用户信息 */
    quint32 iCount;
    QList<UCSMicroMsgRequest_t> pList;
} UCSSendMsgRequest_t;

/*!
 * \struct UCSMMSendMsgRequest_t
 * \brief 消息发送响应结构
*/
typedef struct
{
    qint32 iRet;            ///< 消息成功结果，0为成功 >
    QString tFromUserName;  ///< 发送方名字 >
    QString tToUserName;    ///< 接收方名字 >
    quint32 iMsgId;         ///< 此消息的Index编号 >
    QString pcClientMsgId;  ///< 消息的唯一标识 >
    quint32 iCreateTime;    ///< 创建时间 >
    quint32 iType;          ///< 消息类型 >
} UCSMicroMsgResponse_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;    ///< 用户信息 >
    quint32 iCount;
    QList<UCSMicroMsgResponse_t> pList;
} UCSSendMsgResponse_t;

/*!
 * \struct UCSNewNotify_t
 * \brief 服务端Notify请求结构体
*/
#pragma pack(push, 1)
typedef struct
{
    quint32 packageLength;  ///< 包长度 >
    quint16 headerLength;   ///< 包头长度 >
    quint16 version;        ///<协议版本号>
    quint32 cmd;            ///<命令号 REQ_NEW_NOTIFY>
    quint32 seq;            ///<包序号>
    quint32 selector;       ///<需要newsync的selector>
} UCSNewNotify_t;
#pragma pack(pop)

/////////////////////// 初始化（联系人列表/离线消息等等） ///////////////////
typedef struct
{
    qint32 iMsgId;
    QString tFromUserName;
    QString tToUserName;
    qint32 iMsgType;
    QString tContent;
    quint32 iStatus;
    QString tImgBuf;
    quint32 iCreateTime;
    QString pcMsgSource;
    QString pcPushContent;
} UCSIMAddMsg_t;

typedef struct
{
    QString tUserName;
} UCSIMDelContact_t;

typedef struct
{
    QString tMemberName;
    QString tNickName;
    qint32 iSex;
} UCSIMChatRoomMember_t;

/*!
 * 暂时是讨论组实体
*/
typedef struct
{
    QString tUserName;      ///< 用户名 >
    QString tNickName;      ///< 昵称 >
    quint32 iSex;           ///< 性别 >
    quint32 iContactType;   ///< 好友类型（参考enIMContactType宏，现在都是默认值IM_CONTACT_IM） >
    quint32 iSource;        ///< 好友添加途径 >
    QString pcSignature;    ///< 签名 >
    quint32 iRoomMemberCount;   ///< 群成员数 >
    QList<UCSIMChatRoomMember_t> ptRoomMemberList;  ///< 群成员列表 >
    QString pcChatRoomOwner;    ///< 群主Username >
    quint32 iMaxMemberCount;    ///< 群组成员上限 >
    qint32 iType;               ///< 群类型 >
} UCSIMModContact_t;

typedef struct
{
    quint32 iKey;   ///< 类型 1:userProfile 2:msg 3:contact >
    quint32 iVal;   ///< 值 >
} UCSIMKeyVal_t;

typedef struct
{
    quint32 iKeyCount;
    QList<UCSIMKeyVal_t> ptKey;
} UCSIMSyncKey_t;

typedef struct
{
    qint32 iCmdId;
    QString tCmdBuf;
} UCSCmdItem_t;

typedef struct
{
    quint32 iCount;
    QList<UCSCmdItem_t> ptList;
} UCSCmdList_t;

typedef struct
{
    UCSBaseRequest_t tBaseRequest;
    QString pcUserName;         ///< 必填 >
    QString pcLanuage;          ///< 语言类型，必填，即服务端返回的语言类型 >
    UCSIMSyncKey_t tCurrentSyncKey;
    UCSIMSyncKey_t tMaxSyncKey;
} UCSInitNewSyncRequest_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    UCSIMSyncKey_t tCurrentSyncKey;
    UCSIMSyncKey_t tMaxSyncKey;
    quint32 iContinueFlag;
    quint32 iSelectBitmap;
    QList<UCSIMAddMsg_t> tMsgList;
    QList<UCSIMModContact_t> tContactList;
    QList<UCSIMDelContact_t> tDeleteList;
} UCSInitNewSyncResponse_t;

/*!
 * 同步有效key检查
*/
typedef struct
{
    UCSIMSyncKey_t tSyncKeyBuf;
} UCSNewSyncCheckRequest_t;

typedef struct
{
    quint32 packageLength;  ///< 包长度 >
    quint16 headerLength;   ///< 包头长度 >
    quint16 version;        ///<协议版本号>
    quint32 cmd;            ///<命令号 REQ_NEW_NOTIFY>
    quint32 seq;            ///<包序号>
    quint32 selector;       ///<需要newsync的selector>
} UCSNewSyncCheckResponse_t;

/*!
 * \brief 点对点消息同步请求
*/

typedef struct
{
    UCSBaseRequest_t tBaseRequest;
    UCSCmdList_t tOplog;
    quint32 iSelector;              ///< 选择同步的内容 >
    QString tKeyBuf;
    UCSIMSyncKey_t tCurrentSyncKey;
    quint32 iScene;                 ///<同步的场景，定义于mmsyncdef.h>
} UCSNewSyncRequest_t;

/*!
 * \brief 点对点消息同步应答
*/
typedef struct
{
    qint32 iCmdId;
    qint32 iRet;
} UCSOpCmdRest_t;

typedef struct
{
    quint32 iCount;
    QList<UCSOpCmdRest_t> ptList;
} UCSOpCmdRespList_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    qint32 iRet;                ///< 同步结果标记 >
    quint32 iContinueFlag;      ///< 还需同步的iSelector, 0为不需要再NewSync >
    quint32 iState;             ///< 同步状态，定义于mysyncdef.h >
    UCSIMSyncKey_t tCurrentSyncKey;
    QList<UCSIMAddMsg_t> tMsgList;
    QList<UCSIMModContact_t> tContactList;
    QList<UCSIMDelContact_t> tDeleteList;
    bool bModifyTopic;      ///< 讨论组主题修改响应信息 >
} UCSNewSyncResponse_t;

/////////////////////// 图片上传下载 ///////////////////
typedef struct
{
    QString pcClientMsgId;      ///< 消息ID >
    QString pcFromUserName;     ///< 发起者 >
    QString pcToUserName;       ///< 接收者 >
    quint32 iTotalLen;          ///< 总大小 >
    quint32 iStartPos;          ///< 起始位置 >
    quint32 iDataLen;           ///< 数据长度 >
    QByteArray tData;           ///< BUFF（数据+数据长度） >
    quint32 iMsgType;           ///< 消息图片的类型 >
    QString pcMsgSource;
    quint32 iCompressType;      ///< 是否是原图发送 >
    qint32 iNetType;            ///< 暂不用 >
    qint32 iPhotoFrom;          ///< 暂不用 >
    QString pcMediaId;          ///< 暂不用 >
    QString pcCDNBigImgUrl;     ///< CDN 原图URL >
    QString pcCDNMidImgUrl;     ///< CDN 大图URL >
    QString pcAESKey;           ///< CDN 公钥 >
    qint32 iEncryVer;           ///< CDN 相关 >
    qint32 iCDNBigImgSize;      ///< 上传CDN 的原图大小 >
    qint32 iCDNMidImgSize;      ///< 上传CDN 的大图大小 >
    QString pcMD5;              ///< 图片MD5 >
} UCSUploadMsgImgRequest_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    quint32 iMsgId;                     ///< 服务端产生的MSGID >
    QString pcClientMsgId;              ///< (Req) >
    QString pcFromUserName;             ///< (Req) >
    QString pcToUserName;               ///< (Req) >
    quint32 iTotalLen;                  ///< (Req) >
    quint32 iStartPos;                  ///< 下一个包的起始位置 >
    quint32 iDataLen;                   ///< (Req) >
    quint32 iCreateTime;                ///< 完整接收之后产生的时间戳 >
} UCSUploadMsgImgResponse_t;

typedef struct
{
    quint32 iMsgId;
    QString pcFromUserName;     ///< 发起者 >
    QString pcToUserName;       ///< 接收者 >
    quint32 iTotalLen;          ///< 总长度（第一个包为0） >
    quint32 iStartPos;          ///< 起始位置（第一个为0） >
    quint32 iDataLen;           ///< 数据长度 >
    quint32 iCompressType;      ///< 是否需要原图 >
} UCSDownloadMsgImgRequest_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    quint32 iMsgId;                     ///< 服务端产生的MSGID >
    QString pcFromUserName;             ///< (Req) >
    QString pcToUserName;               ///< (Req) >
    quint32 iTotalLen;                  ///< 总长度 >
    quint32 iStartPos;                  ///< (Req) >
    quint32 iDataLen;                   ///< (Req) >
    QByteArray tData;                   ///< BUFF（数据+数据长度） >
} UCSDownloadMsgImgResponse_t;

/////////////////////// 语音消息下载 ///////////////////
typedef struct
{
    quint32 iMsgId;         ///< 从服务器得到的msgId >
    quint32 iOffset;        ///< 起始位置 >
    quint32 iLenght;        ///< 请求长度 >
    QString pcClientMsgId;  ///< 从服务器得到的ClientMsgId >
} UCSDownloadVoiceRequest_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    quint32 iMsgId;                     ///< (req) >
    quint32 iOffset;                    ///< (req) >
    quint32 iLength;                    ///< 实际发送的长度 >
    quint32 iVoiceLength;               ///< 播放长度 >
    QString pcClientMsgId;              ///< (req) >
    QByteArray tData;                   ///< 语音BUFF >
    quint32 iEndFlag;                   ///< 结束标志 >
    quint32 iCancelFlag;                ///< 取消标志 >
} UCSDownloadVoiceResponse_t;

/////////////////////// 讨论组 ///////////////////
/*!
 * 创建讨论组
 */
typedef struct
{
    QString tMemberName;
    quint32 iMemberStatus;
    QString tNickName;
    QString tPYInitial;
    QString tQuanPin;
    qint32 iSex;
    QString tRemark;
    quint32 tContactType;
    QString pcProvince;
    QString pcCity;
    QString pcSignature;
    QString iVerifyFlag;
    QString pcVerifyInfo;
    QString pcCountry;
    QString tBirthday;
} UCSMemberResp_t;

typedef struct
{
    QString tIntroduce;         ///< 讨论组主题，由用户输入或者选择 >
    quint32 iMemberCount;       ///< 创建讨论组时的成员数(不包括创建者本身) >
    QStringList ptMemberList;         ///< 讨论组成员列表(不包含创建者本身) >
} UCSCreateGroupRequest_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    QString tIntroduce;                 ///< 讨论组的主题 >
    QString tPYInitial;                 ///< 主题的拼音 >
    QString tQuanPin;                   ///< 讨论组主题的全拼 >
    quint32 iMemberCount;               ///< 讨论组成员数 >
    QList<UCSMemberResp_t> ptMemberList;
    quint32 iChatRoomId;                ///< 讨论组ID >
    QString tImgBuf;                    ///< 暂不使用，群头像由客户端取成员头像拼装 >
    QString pcBigHeadImgUrl;            ///< 暂不使用，群头像由客户端取成员头像拼装 >
    QString pcSmallHeadImgUrl;          ///< 暂不使用，群头像由客户端取成员头像拼装 >
    quint32 iMaxMemberCount;            ///< 暂不使用，群头像由客户端取成员头像拼装 >
} UCSCreateGroupResponse_t;

/*!
 *  讨论组加人
*/
typedef struct
{
    quint32 iMemberCount;           ///< 邀请加入的成员数 >
    QStringList ptMemberList;       ///< 邀请加入的成员列表 >
    quint32 iChatRoomId;            ///< 讨论组ID >
} UCSAddGroupMemberRequest_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    quint32 iMemberCount;               ///< 已处理的成员数 >
    QList<UCSMemberResp_t> ptMemberList; ///< 已处理的成员列表 >
    quint32 iChatRoomId;
} UCSAddGroupMemberResponse_t;

/*!
 * 讨论组踢人
*/
typedef struct
{
    quint32 iMemberCount;           ///< 踢出的成员数 >
    QStringList ptMemberList;       ///< 踢出的成员列表 >
    quint32 iChatRoomId;            ///< 讨论组ID >
} UCSDelGroupMemberRequest_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    quint32 iMemberCount;           ///< 踢出的成员数 >
    QStringList ptMemberList;       ///< 踢出的成员列表 >
    quint32 iChatRoomId;            ///< 讨论组ID >
} UCSDelGroupMemberResponse_t;

/*!
 * 讨论组成员退出
 */
typedef struct
{
    quint32 iChatRoomId;
    QString tUserName;
} UCSQuitChatRoom_t;

typedef struct
{
    UCSQuitChatRoom_t tQuitChatRoom;
} UCSQuitGroupRequest_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    UCSQuitChatRoom_t tQuitChatRoom;
} UCSQuitGroupResponse_t;

/*!
 * 修改讨论组信息
*/
typedef struct
{
    quint32 iChatRoomId;
    quint32 iChatRoomType;
    QString tChatRoomName;
    QString tChatRoomTopic;
    UCSIMSyncKey_t tSyncKey;
} UCSModGroupTopicRequest_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    qint32 iRet;
} UCSModGroupTopicResponse_t;

/*!
 * \brief 自定义消息
*/
typedef struct
{
    QString pcClientMsgId;      ///< 消息Id >
    QString pcFromUserName;     ///< 发起者 >
    QString pcToUserName;       ///< 接收者 >
    QString pcContent;          ///< 置空 >
    quint32 iTotalLen;          ///< 总长度 >
    quint32 iStartPos;          ///< 起始位置 >
    QByteArray tData;           ///< Buff 数据+数据长度 >
    quint32 iMsgType;           ///< 消息类型 >
    QString pcMsgSource;        ///< 来源，不填 >
    QString pcMD5;              ///< 图片MD5 >
} UCSCustomMsgRequest_t;

typedef struct
{
    UCSBaseResponse_t tBaseResponse;
    quint32 iMsgId;             ///< 服务端产生的msgId >
    QString pcClientMsgId;      ///< Req >
    QString pcFromUserName;     ///< Req >
    QString pcToUserName;       ///< Req >
    qint32 iTotalLen;           ///< Req >
    qint32 iStartPos;           ///< 下一个包的起始位置 >
    qint32 iDataLen;            ///< Req >
    qint32 iCreateTime;
} UCSCustomMsgResponse_t;
#endif // UCSSTACKDEFINE_H
