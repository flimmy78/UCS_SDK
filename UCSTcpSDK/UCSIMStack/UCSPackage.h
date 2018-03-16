#ifndef UCSPACKAGE_H
#define UCSPACKAGE_H

#include <QObject>
#include <QByteArray>
#include "UCSStackDefine.h"
#include "ucstcpsdk_global.h"

#define kBodyHeader_t_errorcode (-111)

/*
*  IM3.0 序列化及反序列化封装
*/
class UCSTCPSDKSHARED_EXPORT UCSPackage
{

public:
    UCSPackage();

    /*!
     * \brief cmdName 获取IM协议命令字名称
     * \param cmd
     * \return
     */
    static QString cmdName(quint32 cmd);

    /*!
     * \brief 登录请求
    */
    static QByteArray PackAuthRequest(UCSAuthRequest_t *authReq);

    /*!
     * \brief 登录响应
    */
    static void UnPackAuthResponse(const QByteArray dataArray,
                                   UCSAuthResponse_t *authRsp);

    /*!
     * \brief 重登陆请求
    */
    static QByteArray PackReAuthRequest(UCSReAuthRequest_t *pReAuthReq);

    /*!
     * \brief 重登陆响应
    */
    static void UnPackReAuthResponse(const QByteArray dataArray,
                                     UCSReAuthResponse_t *pReAuthRsp);

    /*!
     * \brief 心跳请求
    */
    static QByteArray PackNoopRequest();

    /*!
     * \brief 消息发送请求
    */
    static QByteArray PackSendMsgRequest(const UCSSendMsgRequest_t *pRequest);

    /*!
     * \brief 消息发送响应
    */
    static void UnpackSendMsgResponse(const QByteArray dataArray,
                                      UCSSendMsgResponse_t *pResponse);

    /*!
     * \brief 初始化消息同步请求
    */
    static QByteArray PackInitNewSyncRequest(const UCSInitNewSyncRequest_t *pRequest);

    /*!
     * \brief 初始化消息同步响应
    */
    static void UnpackInitNewSyncResponse(const QByteArray dataArray,
                                          UCSInitNewSyncResponse_t *pResponse);

    /*!
     * \brief 服务端新消息通知请求
    */
    static void UnpackNewNotifyRequest(const QByteArray dataArray,
                                       UCSNewNotify_t *pNotify);

    /*!
     * \brief 新消息同步请求
    */
    static QByteArray PackNewSyncRequest(const UCSNewSyncRequest_t *pRequest);

    /*!
     * \brief 新消息同步响应
    */
    static void UnpackNewSyncResponse(const QByteArray dataArray,
                                      UCSNewSyncResponse_t *pResponse);

    /*!
     * \brief 同步有效key检查请求
    */
    static QByteArray PackNewSyncCheckRequest(const UCSNewSyncCheckRequest_t *pRequest);

    /*!
     * \brief 同步有效key检查响应
    */
    static void UnpackNewSyncCheckResponse(const QByteArray dataArray,
                                           UCSNewSyncCheckResponse_t *pResponse);

    /*!
     * \brief PackUploadMsgImgRequest 上传图片请求
     * \param pRequest
     */
    static QByteArray PackUploadMsgImgRequest(const UCSUploadMsgImgRequest_t *pRequest);

    /*!
     * \brief UnpackUploadMsgImgResponse 上传图片响应
     * \param dataArray
     * \param pResponse
     */
    static void UnpackUploadMsgImgResponse(const QByteArray dataArray,
                                           UCSUploadMsgImgResponse_t *pResponse);

    /*!
     * \brief PackDownloadMsgImgRequest 下载图片请求
     * \param pRequest
     */
    static QByteArray PackDownloadMsgImgRequest(const UCSDownloadMsgImgRequest_t *pRequest);

    /*!
     * \brief UnpackDownloadMsgImgResponse 下载图片响应
     * \param dataArray
     * \param pResponse
     */
    static void UnpackDownloadMsgImgResponse(const QByteArray dataArray,
                                             UCSDownloadMsgImgResponse_t *pResponse);

    /*!
     * \brief PackDownloadVoiceRequest 当语音聊天时，接收端接收语音
     * \param pRequest
     * \return
     */
    static QByteArray PackDownloadVoiceRequest(const UCSDownloadVoiceRequest_t *pRequest);

    /*!
     * \brief UnpackDownloadVoiceReponse 服务器对应DownloadVoiceRequest的回应包
     * \param dataArray
     * \param pResponse
     */
    static void UnpackDownloadVoiceReponse(const QByteArray dataArray,
                                           UCSDownloadVoiceResponse_t *pResponse);

    /*!
     * \brief PackCreateGroupRequest 当客户端需要创建一个讨论组时，发送该请求
     * \param pRequest
     * \return
     */
    static QByteArray PackCreateGroupRequest(const UCSCreateGroupRequest_t *pRequest);

    /*!
     * \brief UnpackCreateGroupResponse 客户端发送创建讨论组的请求后，服务端的响应
     * \param dataArray
     * \param pResponse
     */
    static void UnpackCreateGroupResponse(const QByteArray dataArray,
                                          UCSCreateGroupResponse_t *pResponse);

    /*!
     * \brief PackAddGroupMemberRequest 当用户为讨论组拥有人，需要邀请好友加入讨论组时，发送该请求
     * \param pRequest
     * \return
     */
    static QByteArray PackAddGroupMemberRequest(const UCSAddGroupMemberRequest_t *pRequest);

    /*!
     * \brief UnpackAddGroupMemberResponse 客户端发送讨论组拥有者加人请求的响应
     * \param dataArray
     * \param pResponse
     */
    static void UnpackAddGroupMemberResponse(const QByteArray dataArray,
                                             UCSAddGroupMemberResponse_t *pResponse);

    /*!
     * \brief PackDelGroupMemberRequest 当用户为讨论组拥有者，需要踢人某个讨论组成员时，发送该请求
     * \param pRequest
     * \return
     */
    static QByteArray PackDelGroupMemberRequest(const UCSDelGroupMemberRequest_t *pRequest);

    /*!
     * \brief UnpackDelGroupMemberResponse 客户端发送踢人请求，服务端的响应
     * \param dataArray
     * \param pResponse
     */
    static void UnpackDelGroupMemberResponse(const QByteArray dataArray,
                                             UCSDelGroupMemberResponse_t *pResponse);

    /*!
     * \brief PackQuitGroupRequest
     * \param pRequest
     * \return
     */
    static QByteArray PackQuitGroupRequest(const UCSQuitGroupRequest_t *pRequest);

    /*!
     * \brief UnpackQuitGroupResponse
     * \param dataArray
     * \param pResponse
     */
    static void UnpackQuitGroupResponse(const QByteArray dataArray,
                                        UCSQuitGroupResponse_t *pResponse);

    /*!
     * \brief PackModGroupTopicRequest 当更改群信息时使用
     * \param pRequest
     * \return
     */
    static QByteArray PackModGroupTopicRequest(const UCSModGroupTopicRequest_t *pRequest);

    /*!
     * \brief UnpackModGroupTopicReponse
     * \param dataArray
     * \param pResponse
     */
    static void UnpackModGroupTopicReponse(const QByteArray dataArray,
                                           UCSModGroupTopicResponse_t *pResponse);

    /*!
     * \brief PackCustomMsgRequest 发送自定义消息请求
     * \param pRequest
     * \return
     */
    static QByteArray PackCustomMsgRequest(const UCSCustomMsgRequest_t *pRequest);

    /*!
     * \brief UnpackCustomMsgResponse 解析自定义消息请求的响应
     * \param dataArray
     * \param pResposne
     */
    static void UnpackCustomMsgResponse(const QByteArray dataArray,
                                        UCSCustomMsgResponse_t *pResponse);

public:
    static QString m_sessionKey;
    static quint32 m_iUin;
    static QString m_userName;
    static quint32 m_seq;
};

#endif // UCSPACKAGE_H
