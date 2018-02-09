//
//  UCSIMPublicDef.h
//  UXinClient
//
//  Created by Liam Peng on 12-8-27.
//  Copyright (c) 2012年 UXIN. All rights reserved.
//



#ifndef ucsapisdk_UCSIMPublicDef_h
#define ucsapisdk_UCSIMPublicDef_h

#define IMRecvMsgSoundName_1              "IMRecvNewMsg_1"     // IM 接收到新信息提示音(非聊天窗口)
#define IMRecvMsgSoundName_2              "IMRecvNewMsg_2"     // IM 接收到新信息提示音(聊天窗口)
#define IMSendMsgOKSoundName              "SentMessageOK"      // IM 发送信息成功提示音

#define IMContentActionFlag_Begin         "[UXinIMCAF]"        // IM信息内容跳转标识开始
#define IMContentActionFlag_End           "[/UXinIMCAF]"       // IM信息内容跳转标识结束

#define IMContentPicURLFlag               "[UXinIMPicURL]"     // IM图片信息URL标识
#define IMContentPicNameFlag              "[UXinIMPicName]"    // IM图片名称

#define IMLocationTag                     "location"           // IM位置-容器
#define IMLocationLongitudeTag            "longitude"          // IM位置-经度字段
#define IMLocationLatitudeTag             "latitude"           // IM位置-纬度字段
#define IMLocationDescriptionTag          "description"        // IM位置-描述
#define IMLocationDeviceTypeTag           "dtype"              // IM位置-设备类型
#define IMLocationDeviceTypeiOSValue      "1"

#define IMSendSuccess                     0                     // IM发送成功
#define IMSendLimite                      1000                  // IM发送失败(给非有信好友发送SMS次数受限)
#define IMSendFiledNoNet                  1001                  // IM发送失败(无网络)
#define IMSendFiledOther                  1002                  // IM发送失败(其他原因)

//// IM 信息类型
//typedef enum 
//{
//    IMMT_Recv   = 0,    // 接收
//    IMMT_Send   = 1,    // 发送
//    IMMT_Default
//}IMMessageType;
//
//// IM 信息内容类型
//typedef  enum
//{
//    IMMCT_Text     = 1,    // 文字
//    IMMCT_Picture  = 2,    // 图片
//    IMMCT_Voice    = 3,    // 语音
//    IMMCT_Video    = 4,    // 视频
//    IMMCT_Location = 5,    // 地理位置
//    IMMCT_Richtext = 6,    // 富媒体
//    IMMCT_MissCall = 8,    // 未接来电
//    IMMCT_NewUXin  = 17,   // 好友加入有信通知
//    IMMCT_UxinNews = 22,   // 有信富媒体
//    IMMCT_ShareNews = 23,  // 分享富媒体信息
//    IMMCT_Default
//}IMMessageContentType;
enum {
    RESP_NONE					= 10000,		/**< ŒﬁœÏ”¶ */
    
    
    REQ_AUTH					= 1,		/**< Auth «Î«Û */
    RESP_AUTH       			= 30001,  	/**< Auth œÏ”¶ */
    REQ_REAUTH       			= 2,		/**< ReAuth«Î«Û  π”√SK */
    RESP_REAUTH       			= 30002,		/**< ReAuthœÏ”¶  π”√SK */
    
    REQ_SEND_MSG    			= 3,		/**< SendMsg «Î«Û */
    RESP_SEND_MSG   			= 30003,		/**< SendMsg œÏ”¶ */
    REQ_NEW_SYNC        		= 600011,		/**< New Sync «Î«Û */
    RESP_NEW_SYNC       		= 10600011,		/**< New Sync œÏ”¶ */
    
    REQ_NEW_NOTIFY  			= 600016,      /**< NewNotify «Î«Û */
    REQ_NEW_SYNCCHK 			= 6,      /**< NewSyncCheck «Î«Û */
    RESP_NEW_SYNCCHK			= 30006,		/**< NewSyncCheck «Î«Û */
    
    REQ_UPLOAD_VOICE  			= 600035,      /**< UploadVoice «Î«Û */
    RESP_UPLOAD_VOICE 			= 10600035,		/**< UploadVoice œÏ”¶ */
    REQ_DOWNLOAD_VOICE     		= 600036,      /**< DownloadVoice «Î«Û */
    RESP_DOWNLOAD_VOICE   		= 10600036,		/**< DownloadVoice œÏ”¶ */
    
    REQ_UPLOAD_MSGIMG  			= 600030,      /**< UploadMsgImg «Î«Û */
    RESP_UPLOAD_MSGIMG 			= 10600030,		/**< UploadMsgImg œÏ”¶ */
    REQ_DOWNLOAD_MSGIMG     	= 600031,      /**< DownloadMsgImg «Î«Û */
    RESP_DOWNLOAD_MSGIMG    	= 10600031,    /**< DownloadMsgImg œÏ”¶ */
    
    REQ_UPLOAD_VIDEO     		= 11,      /**< UploadVideo «Î«Û */
    RESP_UPLOAD_VIDEO    		= 30011,		/**< UploadVideo œÏ”¶ */
    REQ_DOWNLOAD_VIDEO     		= 12,      /**< DownloadVideo «Î«Û */
    RESP_DOWNLOAD_VIDEO    		= 30012,		/**< DownloadVideo œÏ”¶ */
    
    REQ_CREATE_GROUP			= 13,		/**< CreateGroup «Î«Û*/
    RESP_CREATE_GROUP			= 30013,		/**< CreateGroup œÏ”¶*/
    
    REQ_ADD_GROUP_MEMBER		= 14,		/**< AddGroupMember «Î«Û*/
    RESP_ADD_GROUP_MEMBER		= 30014,		/**< AddGroupMember œÏ”¶*/
    
    REQ_DEL_GROUP_MEMBER		= 15,		/**< DelGroupMember «Î«Û*/
    RESP_DEL_GROUP_MEMBER		= 30015,		/**< DelGroupMember œÏ”¶*/
    
    REQ_QUIT_GROUP				= 16,		/**< QuitGroup «Î«Û*/
    RESP_QUIT_GROUP				= 30016,		/**< QuitGroup œÏ”¶*/
    
    
    REQ_REG         			= 600001,      /**< Reg «Î«Û */
    RESP_REG        			= 10600001,		/**< Reg œÏ”¶ */
    
    
    REQ_INIT					= 600010,		/**< Init «Î«Û */
    RESP_INIT					= 10600010,
    
    REQ_SYNC_FRIEND				= 600013,		/**< SyncFriend «Î«Û*/
    RESP_SYNC_FRIEND			= 10600013,		/**< SyncFriend œÏ”¶*/
    REQ_USERINFOSET 			= 600014,      /**< UserInfoSet «Î«Û */
    RESP_USERINFOSET			= 10600014,		/**< UserInfoSet œÏ”¶ */
    REQ_GETPROFILE				= 600015,		/**< GetProfile «Î«Û */
    RESP_GETPROFILE				= 10600015,		/**< GetProfile œÏ”¶ */
    
    
    REQ_SEARCH_CONTACT 			= 600020, 		/**< SearchContact «Î«Û*/
    RESP_SEARCH_CONTACT			= 10600020,		/**< SearchContact œÏ”¶*/
    REQ_VERIFY_USER				= 600021,		/**< VerifyUser «Î«Û*/
    RESP_VERIFY_USER			= 10600021,		/**< VerifyUser œÏ”¶*/
    REQ_GET_CONTACT				= 600022,		/**< GetContact «Î«Û*/
    RESP_GET_CONTACT			= 10600022,		/**< GetContact œÏ”¶*/
    
    
    REQ_UPLOAD_HEAD_HDIMG  		= 600032,      /**< UploadHeadHDImg «Î«Û */
    RESP_UPLOAD_HEAD_HDIMG 		= 10600032,		/**< UploadHeadHDImg œÏ”¶ */
    REQ_GET_HEAD_HDIMG     		= 600033,      /**< GetHeadHDImg «Î«Û */
    RESP_GET_HEAD_HDIMG    		= 10600033,		/**< GetHeadHDImg œÏ”¶ */
    REQ_GET_HEAD_IMG     		= 600034,      /**< GetHeadImg «Î«Û */
    RESP_GET_HEAD_IMG    		= 10600034,		/**< GetHeadImg œÏ”¶ */
    
    
    REQ_UPLOAD_EMOJI     		= 600039,      /**< UploadEmoji «Î«Û */
    RESP_UPLOAD_EMOJI    		= 10600039,		/**< UploadEmoji œÏ”¶ */
    REQ_DOWNLOAD_EMOJI    		= 600040,      /**< DownloadEmoji «Î«Û */
    RESP_DOWNLOAD_EMOJI    		= 10600040,		/**< DownloadEmoji œÏ”¶ */
    REQ_UPLOADCOVERIMG			= 600041,		/**< UploadCoverImg «Î«Û */
    RESP_UPLOADCOVERIMG			= 10600041,		/**< UploadCoverImg œÏ”¶ */
    REQ_GETCOVERIMG				= 600042,		/**< GetCoverImg «Î«Û */
    RESP_GETCOVERIMG			= 10600042,		/**< GetCoverImg œÏ”¶ */
    
    REQ_LBSFIND					= 600060,		/**< LbsFind «Î«Û */
    RESP_LBSFIND				= 10600060,		/**< LbsFind œÏ”¶ */
    REQ_GETLOCATION				= 600061,		/**< GetLocation «Î«Û */
    RESP_GETLOCATION			= 10600061,		/**< GetLocation œÏ”¶ */
    REQ_LBSMATCH				= 600062,		/**< LbsMatch «Î«Û */
    RESP_LBSMATCH				= 10600062,		/**< LbsMatch œÏ”¶ */
    REQ_SYNCLBSMATCH			= 600063,		/**< SyncLbsMatch «Î«Û */
    RESP_SYNCLBSMATCH			= 10600063,		/**< SyncLbsMatch œÏ”¶ */
    
    //=========================================================
    
    REQ_MOBILE_OPT				= 600070,
    RESP_MOBILE_OPT				= 10600070,
    REQ_EMAIL_OPT				= 600071,
    RESP_EMAIL_OPT				= 10600071,
    REQ_UPLOAD_PLUGFRIENDS		= 600072,
    RESP_UPLOAD_PLUGFRIENDS		= 10600072,
    REQ_PUSHSETTING				= 600073,
    RESP_PUSHSETTING			= 10600073,
    
    REQ_MODUSERNAME				= 600074,
    RESP_MODUSERNAME			= 10600074,
    
    REQ_MODPWD					= 600075,
    RESP_MODPWD					= 10600075,
    
    REQ_MODPWD_BYMOBILE			= 600076,
    RESP_MODPWD_BYMOBILE		= 10600076,
    
    REQ_GET_DNS 				= 600077,      /**< ªÒ»° ip «Î«Û */
    RESP_GET_DNS 				= 10600077,		/**< ªÒ»° ip œÏ”¶ */
    
    //==========================»∫◊Èœ‡πÿø™ º==================
    
    REQ_CREATE_CHATROOM			= 600080,		/**< CreateChatRoom «Î«Û*/
    RESP_CREATE_CHATROOM		= 10600080,		/**< CreateChatRoom œÏ”¶*/
    
    REQ_ADD_CHATROOM_MEMBER		= 600081,		/**< AddChatRoomMember «Î«Û*/
    RESP_ADD_CHATROOM_MEMBER	= 10600081,		/**< AddChatRoomMember œÏ”¶*/
    
    REQ_DEL_CHATROOM_MEMBER		= 600082,		/**< DelChatRoomMember «Î«Û*/
    RESP_DEL_CHATROOM_MEMBER	= 10600082,		/**< DelChatRoomMember œÏ”¶*/
    
    REQ_QUIT_CHATROOM			= 600083,		/**< QuitChatRomm «Î«Û */
    RESP_QUIT_CHATROOM			= 10600083,		/**< QuitChatRomm œÏ”¶*/
    
    REQ_SEARCH_CHATROOM			= 600084,		/**< SearchChatRoom «Î«Û*/
    RESP_SEARCH_CHATROOM		= 10600084,		/**< SearchChatRoom œÏ”¶*/
    
    REQ_LBSCHATROOMFIND			= 600085,		/**< LbsChatRoomFind «Î«Û */
    RESP_LBSCHATROOMFIND		= 10600085,		/**< LbsChatRoomFind œÏ”¶ */
    
    REQ_VERIFYCHATROOMMEMBER	= 600086,		/**< VerifyChatRoomMember«Î«Û */
    RESP_VERIFYCHATROOMMEMBER	= 10600086,		/**< VerifyChatRoomMember œÏ”¶ */
    
    REQ_INVITECHATROOMMEMBER	= 600087,		/**< InviteChatRoomMember«Î«Û */
    RESP_INVITECHATROOMMEMBER	= 10600087,		/**< InviteChatRoomMember œÏ”¶ */
    
    REQ_CLOSECHATROOM			= 600088,		/**< CloseChatRoom«Î«Û */
    RESP_CLOSECHATROOM			= 10600088,		/**< CloseChatRoom œÏ”¶ */
    
    
    //==========================»∫œ‡πÿΩ· ¯==================
    
    REQ_REPORT_PROFILE			= 600094,		/**< ReportProfile «Î«Û*/
    RESP_REPORT_PROFILE			= 10600094,		/**< ReportProfile œÏ”¶*/
    
    REQ_REPORT_GROUP			= 600095,		/**< ReportGroup «Î«Û*/
    RESP_REPORT_GROUP			= 10600095,		/**< ReportGroup œÏ”¶*/
    
    REQ_NOOP        			= 600100,      /**< Noop «Î«Û */
    RESP_NOOP       			= 10600100,		/**< Noop œÏ”¶ */
    REQ_QUIT        			= 600101,      /**< Quit «Î«Û */
    REQ_DIRECT_SEND 			= 600102,      /**< DirectSend «Î«Û */
    
    REQ_SYNC_CHECK		= 600103,
    RESP_SYNC_CHECK		= 10600103,
    REQ_FIX_SYNCCHK		= 600104,
    RESP_FIX_SYNCCHK	= 10600104,
    
    REQ_BULLETIN    			= 700000,      /**< π ’œπ´∏Ê */
    REQ_SPEED_TEST_NOTIFY 		= 700001,     	/**< ≤‚ÀŸÕ®÷™ */
    REQ_SPEED_TEST  			= 700002,      /**< ≤‚ÀŸ«Î«Û */
    RESP_SPEED_TEST 			= 10700002,		/**< ≤‚ÀŸœÏ”¶ */
    REQ_RPT_SPEED_TEST 			= 700003,      /**< …œ±®≤‚ÀŸΩ·π˚«Î«Û */
    RESP_RPT_SPEED_TEST 		= 10700003,		/**< …œ±®≤‚ÀŸΩ·π˚œÏ”¶ */
    
    REQ_VOIP_INVITE_NOTIFY 		= 700010,   	/**< VoipInviteNotify «Î«Û */
    REQ_NEWSYNC_SYNCWAIT 		= 700011,		/**< ÷ß≥÷syncwaitµƒnewsync«Î«Û*/
    RESP_NEWSYNC_SYNCWAIT 		= 10700011,		/**< ÷ß≥÷syncwaitµƒnewsyncœÏ”¶*/
    REQ_NEWSYNC_NOTIFYDATA 		= 700012,		/**< newsync notifydata«Î«Û*/
    REQ_NEWSYNC_ACK 			= 10700012,		/**< newsync ack œ‡”¶*/
    
    RESP_VOIP_SYNC			= 10700013,
    RESP_SNS_SYNC			= 10700014,
    REQ_NOTIFY			= 700015,
    REQ_VOIP_NOTIFY			= 700016,
    
    REQ_CONN        			= 700300,   	/**< ƒ⁄≤ø÷∏¡Ó: ¡¨Ω” */
    REQ_DISCONN     			= 700301,   	/**< ƒ⁄≤ø÷∏¡Ó: ∂œœﬂ */
    REQ_ONLINE      			= 700302,   	/**< ƒ⁄≤ø÷∏¡Ó: …œœﬂ */
    REQ_OFFLINE     			= 700303,   	/**< ƒ⁄≤ø÷∏¡Ó: œ¬œﬂ */
    REQ_SWITCH      			= 700304,   	/**< ƒ⁄≤ø÷∏¡Ó: «–ªª */
    REQ_CLI_NOT_FOUND 			= 700305,      	/**< ƒ⁄≤ø÷∏¡Ó: øÕªß∂À≤ª¥Ê‘⁄ */

    REQ_SEND_LOCATION		= 50,
    RESP_SEND_LOCATION		= 30050,
    
    REQ_SEND_CUSTOMMSG					= 51,
    RESP_SEND_CUSTOMMSG					= 30051,
    

};


typedef enum
{
    UCS_Express=1,
    UCS_Cipher=2,
    UCS_Express_Addr_Port=3,
    UCS_Cipher_Addr_Port=4
}UCSLoginType;


typedef  enum
{
    UCSIM_Text     = 1,    // 文字
    UCSIM_Picture  = 2,    // 图片
    UCSIM_Voice    = 3,    // 语音
    UCSIM_Video    = 4,    // 视频
    UCSIM_Location = 5,    // 地理位置
    UCSIM_Richtext = 6,    // 富媒体
    UCSIM_MissCall = 8,    // 未接来电
    UCSIM_Attach   = 9,    // 附件
    UCSIM_NewUXin  = 90,   // 好友加入有信通知
    UCSIM_UxinNews = 91,   // 有信富媒体
    UCSIM_ShareNews = 92,  // 分享富媒体信息
    UCSIM_Custom_Text10     = 10,    // 自定义文字
    UCSIM_Custom_Text11     = 11,    // 自定义文字
    UCSIM_Custom_Text12     = 12,    // 自定义文字
    UCSIM_Custom_Text13     = 13,    // 自定义文字
    UCSIM_Custom_Text14     = 14,    // 自定义文字
    UCSIM_Custom_Text15     = 15,    // 自定义文字
    UCSIM_Custom_Text16     = 16,    // 自定义文字
    UCSIM_Custom_Text17     = 17,    // 自定义文字
    UCSIM_Custom_Text18     = 18,    // 自定义文字
    UCSIM_Custom_Text19     = 19,    // 自定义文字
    UCSIM_Custom_Attach20   = 20,    // 自定义附件
    UCSIM_Custom_Attach21   = 21,    // 自定义附件
    UCSIM_Custom_Attach22   = 22,    // 自定义附件
    UCSIM_Custom_Attach23   = 23,    // 自定义附件
    UCSIM_Custom_Attach24   = 24,    // 自定义附件
    UCSIM_Custom_Attach25   = 25,    // 自定义附件
    UCSIM_Custom_Attach26   = 26,    // 自定义附件
    UCSIM_Custom_Attach27   = 27,    // 自定义附件
    UCSIM_Custom_Attach28   = 28,    // 自定义附件
    UCSIM_Custom_Attach29   = 29,    // 自定义附件
    UCSIM_Default
}UCSIMMsgContentType;

// IM 信息状态
typedef enum
{
    // for 发送的信息
    UCSIM_Success    = 0,    // 成功
    UCSIM_Failed     = 1,    // 失败
    UCSIM_Draft      = 2,    // 草稿
    // for接收的信息
    UCSIM_UNRead     = 3,    // 未读(文字/图片/表情等)
    UCSIM_Read       = 4,    // 已读
    UCSIM_UNReadMedia= 5,    // 未读且未播放(语音/视频)
    UCSIM_UNPlayMedia= 6,    // 已读但未播放(语音/视频)
    UCSIM_UNDownLoad = 7,    // 未下载(语音/图片/视频)
    // for 共有
    UCSIM_Processing = 8,    // 处理中
    UCSIMMsg_Default
}UCSIMMsgStatus;

// IM 信息类型
typedef enum
{
    UCSIMMT_Recv   = 0,    // 接收
    UCSIMMT_Send   = 1,    // 发送
    UCSIMMT_Default
}UCSIMMessageType;
//
//// IM 系统消息类型
//typedef enum
//{
//    IMMST_Common                    = 0,    // 普通系统消息
//    IMMST_Invite_Success            = 1,    // 保留
//    IMMST_InternationCall_On        = 2,    // 保留
//    IMMST_Charge_Note               = 3,    // 充值反馈消息
//    IMMST_Have_Money                = 4,    // 领取奖励系统通知
//    IMMST_Contact_Changed           = 5,    // 通讯录更改通知
//    IMMST_New_Version               = 6,    // 版本更新通知
//    IMMST_Buy_VIP                   = 7,    // 购买会员通知
//    IMMST_VIP_Will_Expire           = 8,    // 会员即将到期提醒
//    IMMST_VIP_Expire                = 9,    // 会员到期提醒
//    IMMST_Buy_NumberShow            = 10,   // 单独购买来显
//    IMMST_NumberShow_Will_Expire    = 11,   // 来显即将到期
//    IMMST_NumberShow_Expire         = 12,   // 来显到期
//    IMMST_New_UXin                  = 13,   // 好友加入有信通知(后台接口调用)
//    IMMST_New_VIP_Note              = 14,   // 有信好友申请VIP会员(后台接口调用)
//    IMMST_Info_Changed              = 15,   // 个人签名和头像修改
//    
//    IMMST_Default
//    
//}IMMessageSystemType;
//
//typedef enum EIMINPUTINGSTATUS {
//    IMINPUT_Inputing_NONE,      //未知
//    IMINPUT_Inputing_text = 1,  //正在输入
//    IMINPUT_Cancel_text,        //取消输入
//    IMINPUT_Inputing_voice,     //正在录制音频
//    IMINPUT_Cancel_voice        //取消录制音频
//}eImInputingStatus;

// IM 信息内容类型
//typedef  enum
//{
//    UCSIM_Text     = 1,    // 文字
//    UCSIM_Picture  = 2,    // 图片
//    UCSIM_Voice    = 3,    // 语音
//    UCSIM_Video    = 4,    // 视频
//    UCSIM_Location = 5,    // 地理位置
//    UCSIM_Richtext = 6,    // 富媒体
//    UCSIM_MissCall = 8,    // 未接来电
//    UCSIM_NewUXin  = 17,   // 好友加入有信通知
//    UCSIM_UxinNews = 22,   // 有信富媒体
//    UCSIM_ShareNews = 23,  // 分享富媒体信息
//    UCSIM_Default
//}UCSIMContentType;

#define IMMJSON_KEY_INPUT @"_inputMethod"

#endif
