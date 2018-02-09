#include "UCSPackage.h"
#include <QDebug>
#include <QtEndian>
#include <QCryptographicHash>
#include "UCSSystemWrapper.h"
#include "UCSTool.h"
#include "Common/UCSLogger.h"
#include "package.h"

quint32 UCSPackage::m_iUin = 0;
QString UCSPackage::m_sessionKey = "";
QString UCSPackage::m_userName = "";
quint32 UCSPackage::m_seq = 0;

UCSPackage::UCSPackage()
{
}

void PackBaseRequest(IMBaseRequest_t *pBaseReq)
{
    pBaseReq->iSeq = (++UCSPackage::m_seq) % 65535;
    pBaseReq->iScene = 1;
    pBaseReq->iClientVersion = 1;
    pBaseReq->iUin = UCSPackage::m_iUin;

    QStringToChar((char*)pBaseReq->cDeviceID,
           UCSSystemWrapper::prettyProductName(),
           sizeof(pBaseReq->cDeviceID));

    QStringToChar((char*)pBaseReq->sDeviceType,
           UCSSystemWrapper::productType(),
           sizeof(pBaseReq->sDeviceType));

    QStringToChar(pBaseReq->sSessionKey,
                  UCSPackage::m_sessionKey,
                  sizeof(pBaseReq->sSessionKey));
}

void UnPackBaseResponse(const IMBaseResponse_t *pBaseResp, UCSBaseResponse_t *pResp)
{
    pResp->iRet = pBaseResp->iRet;
    pResp->iSeq = pBaseResp->iSeq;
    pResp->tErrMsg = charToQString(pBaseResp->tErrMsg.pcBuff);
}

void PackBodyHeader(BodyHead_t *pHeader)
{
    pHeader->ServerId = NULL;
    pHeader->Uin = UCSPackage::m_iUin;
    QStringToChar((char*)pHeader->DeviceId,
           UCSSystemWrapper::prettyProductName(),
           sizeof(pHeader->DeviceId));
}

QByteArray UCSPackage::PackAuthRequest(UCSAuthRequest_t *authReq)
{
    char packBuf[4096] = { 0 };
    quint32 packSize = sizeof(packBuf);
    BodyHead_t bodyHeader;

    memset(&bodyHeader, 0x00, sizeof(BodyHead_t));
    PackBodyHeader(&bodyHeader);

    Comm::SKSmartPickleStruct<IMMsgPickle, IMAuthRequest_t> bodyData;
    memset(&bodyData, 0x00, sizeof(bodyData));

    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);

    /* auth about */
    bodyData.tRandomEncryKey.pcBuff = strdup("1234567890abcdef");
    bodyData.tLoginToken.pcBuff = strdup((authReq->tLoginToken).toLocal8Bit().data());

#if 0
    qDebug("deviceId: %s \ndeviceType: %s \nrandom: %s \ntoken: %s",
           bodyHeader.DeviceId,
           bodyData.tBaseRequest.sDeviceType,
           bodyData.tRandomEncryKey.pcBuff,
           bodyData.tLoginToken.pcBuff);
#endif

    pack(ProxyProtocol::REQ_AUTH, &bodyHeader,
         (void*)&bodyData, (char*)&packBuf, packSize);

//    qDebug() << "bufSize = " << packSize;
    return QByteArray((char*)&packBuf, packSize);
}

void UCSPackage::UnPackAuthResponse(const QByteArray dataArray, UCSAuthResponse_t *pAuthRsp)
{
    BodyHead_t bodyHeader;
    IMAuthResponse_t authResp;

    ByteBuffer data;
    data.clear();

    const char *unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();
    unpack(ProxyProtocol::RESP_AUTH, &bodyHeader, &authResp, (char*)unpackBuf, unpackSize);

    UnPackBaseResponse(&authResp.tBaseResponse, &(pAuthRsp->tBaseResponse));

//    qDebug() << "Auth response ret: " << pAuthRsp->tBaseResponse.iRet;
//    qDebug() << "Auth response errmsg: " << pAuthRsp->tBaseResponse.tErrMsg;
    if (authResp.tBaseResponse.iRet == 0)
    {
        m_iUin = authResp.iUin;
        m_sessionKey = charToQString(authResp.sSessionKey);
        m_userName = charToQString(authResp.tUserName.pcBuff);

        pAuthRsp->iUin = authResp.iUin;
        pAuthRsp->tUserName = charToQString(authResp.tUserName.pcBuff);
        pAuthRsp->tNickName = charToQString(authResp.tNickName.pcBuff);
        pAuthRsp->tBindEmail = charToQString(authResp.tBindEmail.pcBuff);
        pAuthRsp->tBindMobile = charToQString(authResp.tBindMobile.pcBuff);
        pAuthRsp->iStatus = authResp.iStatus;
        pAuthRsp->sSessionKey = charToQString(authResp.sSessionKey);
        pAuthRsp->tImgBuf = charToQString(authResp.tImgBuf.pcBuff);
        pAuthRsp->iNewVersion = authResp.iNewVersion;
        pAuthRsp->iRegType = authResp.iRegType;
        pAuthRsp->iProfileFlag = authResp.iProfileFlag;
        pAuthRsp->iUserSex = authResp.iUserSex;
        pAuthRsp->iUserStatus = authResp.iUserStatus;
        pAuthRsp->tFirstName = charToQString(authResp.tFirstName.pcBuff);
        pAuthRsp->tLastName = charToQString(authResp.tLastName.pcBuff);
        pAuthRsp->tBirthdate = QString("%1-%2-%3")
                                    .arg(authResp.tBirthday.iYear)
                                    .arg(authResp.tBirthday.iMonth)
                                    .arg(authResp.tBirthday.iDay);
        pAuthRsp->tSafeUserName = charToQString(authResp.tSafeUserName.pcBuff);
        pAuthRsp->tClientNumber = charToQString(authResp.tClientNumber.pcBuff);
        pAuthRsp->tAppid = charToQString(authResp.tAppid.pcBuff);

#if 0
        qDebug("Uin: %u",authResp.iUin);
        qDebug("Seq: %u", authResp.tBaseResponse.iSeq);
        qDebug("Sessionkey: %s", authResp.sSessionKey);
        qDebug("UserName: %s", authResp.tUserName.pcBuff);
        qDebug("SafeUserName: %s", authResp.tSafeUserName.pcBuff);
        qDebug("NickName: %s", authResp.tNickName.pcBuff);
        qDebug("FirstName: %s", authResp.tFirstName.pcBuff);
        qDebug("LastName: %s", authResp.tLastName.pcBuff);
        qDebug("BindEmail: %s", authResp.tBindEmail.pcBuff);
        qDebug("BindMobile: %s", authResp.tBindMobile.pcBuff);
        qDebug("RegType: %u", authResp.iRegType);
        qDebug("IpCount: %d", authResp.tWebProxyInfo.iIPCount);
#endif
    }
}

QByteArray UCSPackage::PackReAuthRequest(UCSReAuthRequest_t *pReAuthReq)
{
    Q_UNUSED(pReAuthReq);

    char packBuf[4096] = { 0 };
    quint32 packSize = sizeof(packBuf);
    BodyHead_t bodyHeader;

    memset(&bodyHeader, 0x00, sizeof(BodyHead_t));
    PackBodyHeader(&bodyHeader);

    Comm::SKSmartPickleStruct<IMMsgPickle, IMAuthBySKRequest_t> bodyData;
    memset(&bodyData, 0x00, sizeof(bodyData));

    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);
    bodyData.tBaseRequest.iScene = UCS_SCENE_REAUTH;

    pack(ProxyProtocol::REQ_REAUTH, &bodyHeader, (void*)&bodyData, packBuf, packSize);

    return QByteArray((char*)&packBuf, packSize);
}

void UCSPackage::UnPackReAuthResponse(const QByteArray dataArray, UCSReAuthResponse_t *pReAuthRsp)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMAuthBySKResponse_t> reAuthResp;

    ByteBuffer data;
    data.clear();

    const char *unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();
    unpack(ProxyProtocol::RESP_REAUTH, &bodyHeader, &reAuthResp, (char*)unpackBuf, unpackSize);

    UnPackBaseResponse(&reAuthResp.tBaseResponse, &(pReAuthRsp->tBaseReponse));
    pReAuthRsp->iUin = reAuthResp.iUin;
}

QByteArray UCSPackage::PackNoopRequest()
{
    char packBuf[4096] = { 0 };
    quint32 packSize = sizeof(packBuf);

    pack(ProxyProtocol::REQ_NOOP, NULL, NULL, packBuf, packSize);

    return QByteArray((char*)&packBuf, packSize);
}

QByteArray UCSPackage::PackSendMsgRequest(const UCSSendMsgRequest_t *pRequest)
{
    UCSMicroMsgRequest_t request = pRequest->pList.at(0);

    BodyHead_t bodyHeader;
    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    Comm::SKSmartPickleStruct<IMMsgPickle, IMSendMsgRequest_t> bodyData;
    memset(&bodyData, 0x00, sizeof(bodyData));

    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);

    bodyData.iCount = 1;
    bodyData.ptList = (IMMsgRequest_t*)calloc(bodyData.iCount, sizeof(IMMsgRequest_t));

    bodyData.ptList[0].iType = request.iType;
    bodyData.ptList[0].iCreateTime = request.iCreateTime;
    bodyData.ptList[0].iEmojiFlag = request.iEmojiFlag;
    bodyData.ptList[0].tFromUserName.pcBuff = strdup(m_userName.toStdString().c_str());
    bodyData.ptList[0].tToUserName.pcBuff = strdup(request.tToUserName.toStdString().c_str());
    bodyData.ptList[0].pcClientMsgId = strdup(request.pcClientMsgId.toStdString().c_str());
    bodyData.ptList[0].pcContent = strdup(request.pcContent.toStdString().c_str());

    char packBuf[10 * 1024] = { 0 };
    quint32 packSize = sizeof(packBuf);

    pack(ProxyProtocol::REQ_SEND_MSG, &bodyHeader,
         (void*)&bodyData, (char*)&packBuf, packSize);

//    qDebug() << "bufSize = " << packSize;

    return QByteArray((char*)&packBuf, packSize);
}

void UCSPackage::UnpackSendMsgResponse(const QByteArray dataArray, UCSSendMsgResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMSendMsgResponse_t> bodyData;

    ByteBuffer data;
    data.clear();

    const char *unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_SEND_MSG, &bodyHeader,
           &bodyData, (char*)unpackBuf, unpackSize);

    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));
    if (bodyHeader.Ret == 0)
    {
        pResponse->iCount = bodyData.iCount;

        if (bodyData.iCount > 0)
        {
            UCSMicroMsgResponse_t resp;
            resp.iRet = bodyData.tBaseResponse.iRet;
            resp.iCreateTime = bodyData.ptList[0].iCreateTime;
            resp.iType = bodyData.ptList[0].iType;
            resp.iMsgId = bodyData.ptList[0].iMsgId;

            resp.pcClientMsgId = charToQString(bodyData.ptList[0].pcClientMsgId);
            resp.tFromUserName = charToQString(bodyData.ptList[0].tFromUserName.pcBuff);
            resp.tToUserName = charToQString(bodyData.ptList[0].tToUserName.pcBuff);

            pResponse->pList.append(resp);
        }
    }
}

QByteArray UCSPackage::PackInitNewSyncRequest(const UCSInitNewSyncRequest_t *pRequest)
{
    Comm::SKSmartPickleStruct<IMMsgPickle, IMNewInitRequest_t> bodyData;
    BodyHead_t bodyHeader;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);

    bodyData.pcUserName = strdup(pRequest->pcUserName.toLocal8Bit().data());
    bodyData.pcLanguage = strdup("zh_CN");

    UCSIMKeyVal_t msgKeyVal = pRequest->tCurrentSyncKey.ptKey.at(0);
    UCSIMKeyVal_t contactKeyVal = pRequest->tCurrentSyncKey.ptKey.at(1);

    Comm::SKSmartPickleStruct<IMMsgPickle, IMSyncKey_t> curSyncKey;
    memset(&curSyncKey, 0x00, sizeof(curSyncKey));
    curSyncKey.iKeyCount = 2;
    curSyncKey.ptKey = (IMKeyVal_t*)calloc(curSyncKey.iKeyCount, sizeof(IMKeyVal_t));
    curSyncKey.ptKey[0].iKey = msgKeyVal.iKey;
    curSyncKey.ptKey[0].iVal = msgKeyVal.iVal;
    curSyncKey.ptKey[1].iKey = contactKeyVal.iKey;
    curSyncKey.ptKey[1].iVal = contactKeyVal.iVal;

    SKBuffer keyBuff;
    IMMsgPickle::ToBuffer(&curSyncKey, &keyBuff);
    bodyData.tCurrentSynckey.pcBuff = strdup((char*)keyBuff.GetBuffer());
    bodyData.tCurrentSynckey.iLen = keyBuff.GetLen();

    msgKeyVal = pRequest->tMaxSyncKey.ptKey.at(0);
    contactKeyVal = pRequest->tMaxSyncKey.ptKey.at(1);

    Comm::SKSmartPickleStruct<IMMsgPickle, IMSyncKey_t> maxSyncKey;
    memset(&maxSyncKey, 0x00, sizeof(maxSyncKey));
    maxSyncKey.iKeyCount = 2;
    maxSyncKey.ptKey = (IMKeyVal_t*)calloc(maxSyncKey.iKeyCount, sizeof(IMSyncKey_t));
    maxSyncKey.ptKey[0].iKey = msgKeyVal.iKey;
    maxSyncKey.ptKey[0].iVal = msgKeyVal.iVal;
    maxSyncKey.ptKey[1].iKey = contactKeyVal.iKey;
    maxSyncKey.ptKey[1].iVal = contactKeyVal.iVal;

    SKBuffer keyBuff2;
    IMMsgPickle::ToBuffer(&maxSyncKey, &keyBuff2);
    bodyData.tCurrentSynckey.pcBuff = strdup((char*)keyBuff2.GetBuffer());
    bodyData.tCurrentSynckey.iLen = keyBuff2.GetLen();

    char packBuf[2048] = {0};
    quint32 packSize = sizeof(packBuf);
    pack(ProxyProtocol::REQ_INIT, &bodyHeader, (void*)&bodyData,
         (char*)&packBuf, packSize);

    qDebug() << "bufSize = " << packSize;

    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackInitNewSyncResponse(const QByteArray dataArray, UCSInitNewSyncResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMNewInitResponse_t> bodyData;
    ByteBuffer data;

    data.clear();
    memset(&bodyData, 0x00, sizeof(bodyData));

    const char* unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_INIT, &bodyHeader, (void*)&bodyData, (char*)unpackBuf, unpackSize);
    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));

    if (bodyHeader.Ret != 0)
    {
        pResponse->tBaseResponse.iRet = kBodyHeader_t_errorcode;
        return;
    }
    pResponse->iContinueFlag = bodyData.iContinueFlag;

    /* current sync key */
    Comm::SKSmartPickleStruct<IMMsgPickle, IMSyncKey_t> curSyncKey;
    SKBuffer keyBuff;
    keyBuff.Write(bodyData.tCurrentSynckey.pcBuff, bodyData.tCurrentSynckey.iLen);
    int ret = IMMsgPickle::FromBuffer(&keyBuff, &curSyncKey);

    if (ret != 0)
    {
        pResponse->tBaseResponse.iRet = -1;
        return;
    }

    for(quint32 i = 0; i < curSyncKey.iKeyCount; ++i)
    {
        UCSIMKeyVal_t keyVal;
        keyVal.iKey = curSyncKey.ptKey[i].iKey;
        keyVal.iVal = curSyncKey.ptKey[i].iVal;
        if (keyVal.iKey == 2 || keyVal.iKey == 3)
        {
            pResponse->tCurrentSyncKey.ptKey.append(keyVal);
        }
    }
    pResponse->tCurrentSyncKey.iKeyCount = pResponse->tCurrentSyncKey.ptKey.size();


    /* max sync key */
    keyBuff.Reset();
    Comm::SKSmartPickleStruct<IMMsgPickle, IMSyncKey_t> maxSyncKey;
    keyBuff.Write(bodyData.tMaxSynckey.pcBuff, bodyData.tMaxSynckey.iLen);
    ret = IMMsgPickle::FromBuffer(&keyBuff, &maxSyncKey);
    if (ret != 0)
    {
        pResponse->tBaseResponse.iRet = -1;
        return;
    }
    for (quint32 i = 0; i < maxSyncKey.iKeyCount; ++i)
    {
        UCSIMKeyVal_t keyVal;
        keyVal.iKey = maxSyncKey.ptKey[i].iKey;
        keyVal.iVal = maxSyncKey.ptKey[i].iVal;
        pResponse->tMaxSyncKey.ptKey.append(keyVal);
    }
    pResponse->tMaxSyncKey.iKeyCount = pResponse->tMaxSyncKey.ptKey.size();

    for (quint32 i = 0; i < bodyData.iCmdCount; ++i)
    {
        if (bodyData.ptCmdList[i].iCmdId == UCS_SYNCCMD_ADDMSG)
        {
            SKBuffer msgBuf;
            Comm::SKSmartPickleStruct<IMMsgPickle, IMAddMsg_t> msgItem;
            memset(&msgItem, 0x00, sizeof(msgItem));

            msgBuf.Write(bodyData.ptCmdList[i].tCmdBuf.pcBuff,
                         bodyData.ptCmdList[i].tCmdBuf.iLen);
            IMMsgPickle::FromBuffer(&msgBuf, &msgItem);

            UCSIMAddMsg_t msg;
            msg.iMsgId = msgItem.iMsgId;
            msg.tFromUserName = charToQString(msgItem.tFromUserName.pcBuff);
            msg.tToUserName = charToQString(msgItem.tToUserName.pcBuff);
            msg.iMsgType = msgItem.iMsgType;
            msg.tContent = charToQString(msgItem.tContent.pcBuff);
            msg.iStatus = msgItem.iStatus;
            msg.tImgBuf = charToQString(msgItem.tImgBuf.pcBuff);
            msg.iCreateTime = msgItem.iCreateTime;
            msg.pcMsgSource = charToQString(msgItem.pcMsgSource);
            msg.pcPushContent = charToQString(msgItem.pcPushContent);

            pResponse->tMsgList.append(msg);
        }
        else if (bodyData.ptCmdList[i].iCmdId == UCS_SYNCCMD_MODCONTACT)
        {
            SKBuffer msgBuf;
            Comm::SKSmartPickleStruct<IMMsgPickle, IMModContact_t> modContact;
            memset(&modContact, 0x00, sizeof(modContact));

            msgBuf.Write(bodyData.ptCmdList[i].tCmdBuf.pcBuff,
                         bodyData.ptCmdList[i].tCmdBuf.iLen);
            IMMsgPickle::FromBuffer(&msgBuf, &modContact);

            UCSIMModContact_t contact;
            contact.tUserName = charToQString(modContact.tUserName.pcBuff);
            contact.tNickName = charToQString(modContact.tNickName.pcBuff);
            contact.iSex = modContact.iSex;
            contact.iContactType = modContact.iContactType;
            contact.iSource = modContact.iSource;
            contact.iRoomMemberCount = modContact.iRoomMemberCount;
            contact.iMaxMemberCount = modContact.iMaxMemberCount;
            contact.pcChatRoomOwner = charToQString(modContact.pcChatRoomOwner);
            contact.iType = modContact.iType;

            for (quint32 idx = 0; idx < modContact.iRoomMemberCount; ++idx)
            {
                UCSIMChatRoomMember_t member;
                member.tMemberName = charToQString(modContact.ptRoomMemberList[idx].tMemberName.pcBuff);
                member.tNickName = charToQString(modContact.ptRoomMemberList[idx].tNickName.pcBuff);
                member.iSex = modContact.ptRoomMemberList[idx].iSex;
                contact.ptRoomMemberList.append(member);
            }
        }
        else if (bodyData.ptCmdList[i].iCmdId == UCS_SYNCCMD_DELCONTACT)
        {
            SKBuffer msgBuf;
            Comm::SKSmartPickleStruct<IMMsgPickle, IMDelContact_t> delContact;
            memset(&delContact, 0x00, sizeof(delContact));

            msgBuf.Write(bodyData.ptCmdList[i].tCmdBuf.pcBuff,
                         bodyData.ptCmdList[i].tCmdBuf.iLen);
            IMMsgPickle::FromBuffer(&msgBuf, &delContact);

            if (NULL != delContact.tUserName.pcBuff)
            {
                QString userName = charToQString(delContact.tUserName.pcBuff);
                if (userName.contains("@group"))
                {
                    UCSIMDelContact_t contact;
                    contact.tUserName = userName.section('@', 0, 0);
                    pResponse->tDeleteList.append(contact);
                }
            }
        }
    }
}

void UCSPackage::UnpackNewNotifyRequest(const QByteArray dataArray, UCSNewNotify_t *pNotify)
{
    memcpy(pNotify, dataArray, sizeof(UCSNewNotify_t));

    pNotify->packageLength = qFromBigEndian(pNotify->packageLength);
    pNotify->headerLength = qFromBigEndian(pNotify->headerLength);
    pNotify->version = qFromBigEndian(pNotify->version);
    pNotify->cmd = qFromBigEndian(pNotify->cmd);
    pNotify->seq = qFromBigEndian(pNotify->seq);
    pNotify->selector = qFromBigEndian(pNotify->selector);
}

QByteArray UCSPackage::PackNewSyncRequest(const UCSNewSyncRequest_t *pRequest)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMNewSyncRequest_t> bodyData;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);
    bodyData.tBaseRequest.iScene = 0;

    bodyData.iScene = 1;
    bodyData.iSelector = pRequest->iSelector;

    UCSIMKeyVal_t msgKeyVal = pRequest->tCurrentSyncKey.ptKey.at(0);
    UCSIMKeyVal_t contactKeyVal = pRequest->tCurrentSyncKey.ptKey.at(1);

    Comm::SKSmartPickleStruct<IMMsgPickle, IMSyncKey_t> curSyncKey;
    memset(&curSyncKey, 0x00, sizeof(curSyncKey));
    curSyncKey.iKeyCount = 2;
    curSyncKey.ptKey = (IMKeyVal_t*)calloc(curSyncKey.iKeyCount, sizeof(IMKeyVal_t));
    curSyncKey.ptKey[0].iKey = msgKeyVal.iKey;
    curSyncKey.ptKey[0].iVal = msgKeyVal.iVal;
    curSyncKey.ptKey[1].iKey = contactKeyVal.iKey;
    curSyncKey.ptKey[1].iVal = contactKeyVal.iVal;

    SKBuffer keyBuff;
    IMMsgPickle::ToBuffer(&curSyncKey, &keyBuff);
    bodyData.tKeyBuf.pcBuff = strdup((char*)keyBuff.GetBuffer());
    bodyData.tKeyBuf.iLen = keyBuff.GetLen();

    char packBuf[2048] = {0};
    quint32 packSize = sizeof(packBuf);
    pack(ProxyProtocol::REQ_NEW_SYNC, &bodyHeader, (void*)&bodyData,
         (char*)&packBuf, packSize);

    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackNewSyncResponse(const QByteArray dataArray, UCSNewSyncResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMNewSyncResponse_t> bodyData;
    ByteBuffer data;

    data.clear();
    memset(&bodyData, 0x00, sizeof(bodyData));

    const char* unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_NEW_SYNC, &bodyHeader, (void*)&bodyData, (char*)unpackBuf, unpackSize);
    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));

    if (pResponse->tBaseResponse.iRet != 0)
    {
        return;
    }

    if (bodyHeader.Ret != 0)
    {
        pResponse->tBaseResponse.iRet = kBodyHeader_t_errorcode;
        return;
    }

    /* current sync key */
    Comm::SKSmartPickleStruct<IMMsgPickle, IMSyncKey_t> curSyncKey;
    SKBuffer keyBuff;
    keyBuff.Write(bodyData.tKeyBuf.pcBuff, bodyData.tKeyBuf.iLen);
    int ret = IMMsgPickle::FromBuffer(&keyBuff, &curSyncKey);

    if (ret != 0)
    {
        pResponse->tBaseResponse.iRet = -1;
        return;
    }

    for(quint32 i = 0; i < curSyncKey.iKeyCount; ++i)
    {
        UCSIMKeyVal_t keyVal;
        keyVal.iKey = curSyncKey.ptKey[i].iKey;
        keyVal.iVal = curSyncKey.ptKey[i].iVal;
        if (keyVal.iKey == 2 || keyVal.iKey == 3)
        {
            pResponse->tCurrentSyncKey.ptKey.append(keyVal);
        }
    }
    pResponse->tCurrentSyncKey.iKeyCount = pResponse->tCurrentSyncKey.ptKey.size();

    for (quint32 i = 0; i < bodyData.tCmdList.iCount; ++i)
    {
        if (bodyData.tCmdList.ptList[i].iCmdId == UCS_SYNCCMD_ADDMSG)
        {
            SKBuffer msgBuf;
            Comm::SKSmartPickleStruct<IMMsgPickle, IMAddMsg_t> msgItem;
            memset(&msgItem, 0x00, sizeof(msgItem));

            msgBuf.Write(bodyData.tCmdList.ptList[i].tCmdBuf.pcBuff,
                         bodyData.tCmdList.ptList[i].tCmdBuf.iLen);
            IMMsgPickle::FromBuffer(&msgBuf, &msgItem);

            UCSIMAddMsg_t msg;
            msg.iMsgId = msgItem.iMsgId;
            msg.tFromUserName = charToQString(msgItem.tFromUserName.pcBuff);
            msg.tToUserName = charToQString(msgItem.tToUserName.pcBuff);
            msg.iMsgType = msgItem.iMsgType;
            msg.tContent = charToQString(msgItem.tContent.pcBuff);
            msg.iStatus = msgItem.iStatus;
            msg.tImgBuf = charToQString(msgItem.tImgBuf.pcBuff);
            msg.iCreateTime = msgItem.iCreateTime;
            msg.pcMsgSource = charToQString(msgItem.pcMsgSource);
            msg.pcPushContent = charToQString(msgItem.pcPushContent);

            pResponse->tMsgList.append(msg);
        }
        else if (bodyData.tCmdList.ptList[i].iCmdId == UCS_SYNCCMD_MODCONTACT)
        {
            if (bodyData.tOpCmdResp.ptList[0].iCmdId == UCS_SYNCCMD_MODGROUPTOPIC &&
                bodyData.tOpCmdResp.ptList[0].iRet == 0)
            {
                pResponse->bModifyTopic = true;
            }

            SKBuffer msgBuf;
            Comm::SKSmartPickleStruct<IMMsgPickle, IMModContact_t> modContact;
            memset(&modContact, 0x00, sizeof(modContact));

            msgBuf.Write(bodyData.tCmdList.ptList[i].tCmdBuf.pcBuff,
                         bodyData.tCmdList.ptList[i].tCmdBuf.iLen);
            IMMsgPickle::FromBuffer(&msgBuf, &modContact);

            UCSIMModContact_t contact;
            contact.tUserName = charToQString(modContact.tUserName.pcBuff);
            contact.tNickName = charToQString(modContact.tNickName.pcBuff);
            contact.iSex = modContact.iSex;
            contact.iContactType = modContact.iContactType;
            contact.iSource = modContact.iSource;
            contact.iRoomMemberCount = modContact.iRoomMemberCount;
            contact.iMaxMemberCount = modContact.iMaxMemberCount;
            contact.pcChatRoomOwner = charToQString(modContact.pcChatRoomOwner);
            contact.iType = modContact.iType;

            for (quint32 idx = 0; idx < modContact.iRoomMemberCount; ++idx)
            {
                UCSIMChatRoomMember_t member;
                member.tMemberName = charToQString(modContact.ptRoomMemberList[idx].tMemberName.pcBuff);
                member.tNickName = charToQString(modContact.ptRoomMemberList[idx].tNickName.pcBuff);
                member.iSex = modContact.ptRoomMemberList[idx].iSex;
                contact.ptRoomMemberList.append(member);
            }
        }
        else if (bodyData.tCmdList.ptList[i].iCmdId == UCS_SYNCCMD_DELCONTACT)
        {
            SKBuffer msgBuf;
            Comm::SKSmartPickleStruct<IMMsgPickle, IMDelContact_t> delContact;
            memset(&delContact, 0x00, sizeof(delContact));

            msgBuf.Write(bodyData.tCmdList.ptList[i].tCmdBuf.pcBuff,
                         bodyData.tCmdList.ptList[i].tCmdBuf.iLen);
            IMMsgPickle::FromBuffer(&msgBuf, &delContact);

            if (NULL != delContact.tUserName.pcBuff)
            {
                QString userName = charToQString(delContact.tUserName.pcBuff);
                if (userName.contains("@group"))
                {
                    UCSIMDelContact_t contact;
                    contact.tUserName = userName.section('@', 0, 0);
                    pResponse->tDeleteList.append(contact);
                }
            }
        }
    }

    if (bodyData.tOpCmdResp.ptList[0].iCmdId == UCS_SYNCCMD_MODGROUPTOPIC &&
        bodyData.tOpCmdResp.ptList[0].iRet != 0)
    {
        pResponse->bModifyTopic = false;
    }
}

QByteArray UCSPackage::PackNewSyncCheckRequest(const UCSNewSyncCheckRequest_t *pRequest)
{
#if 0
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMNewSyncCheckRequest_t> bodyData;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));

    bodyData.iUin = UCSPackage::m_iUin;

    UCSIMKeyVal_t msgKeyVal = pRequest->tSyncKeyBuf.ptKey.at(0);
    UCSIMKeyVal_t contactKeyVal = pRequest->tSyncKeyBuf.ptKey.at(1);

    Comm::SKSmartPickleStruct<IMMsgPickle, IMSyncKey_t> curSyncKey;
    memset(&curSyncKey, 0x00, sizeof(curSyncKey));
    curSyncKey.iKeyCount = 2;
    curSyncKey.ptKey = (IMKeyVal_t*)calloc(curSyncKey.iKeyCount, sizeof(IMKeyVal_t));
    curSyncKey.ptKey[0].iKey = msgKeyVal.iKey;
    curSyncKey.ptKey[0].iVal = msgKeyVal.iVal;
    curSyncKey.ptKey[1].iKey = contactKeyVal.iKey;
    curSyncKey.ptKey[1].iVal = contactKeyVal.iVal;

    SKBuffer keyBuff;
    IMMsgPickle::ToBuffer(&curSyncKey, &keyBuff);
    bodyData.iSyncKeyLen = keyBuff.GetLen();
    bodyData.tSyncKeyBuf.pcBuff = strdup((char*)keyBuff.GetBuffer());

    char packBuf[2048] = {0};
    quint32 packSize = sizeof(packBuf);
    pack(ProxyProtocol::REQ_NEW_SYNCCHK, &bodyHeader, (void*)&bodyData,
         (char*)&packBuf, packSize);

    return QByteArray(packBuf, packSize);
#else
    Q_UNUSED(pRequest);
    return QByteArray();
#endif
}

void UCSPackage::UnpackNewSyncCheckResponse(const QByteArray dataArray, UCSNewSyncCheckResponse_t *pResponse)
{
    memcpy(pResponse, dataArray, sizeof(UCSNewSyncCheckResponse_t));

    pResponse->packageLength = qFromBigEndian(pResponse->packageLength);
    pResponse->headerLength = qFromBigEndian(pResponse->headerLength);
    pResponse->version = qFromBigEndian(pResponse->version);
    pResponse->cmd = qFromBigEndian(pResponse->cmd);
    pResponse->seq = qFromBigEndian(pResponse->seq);
    pResponse->selector = qFromBigEndian(pResponse->selector);
}

QByteArray UCSPackage::PackUploadMsgImgRequest(const UCSUploadMsgImgRequest_t *pRequest)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMUploadMsgImgRequest_t> bodyData;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);

    bodyData.iDataLen = pRequest->iDataLen;
    bodyData.iStartPos = pRequest->iStartPos;
    bodyData.iTotalLen = pRequest->iTotalLen;
    bodyData.iMsgType = UCS_DATA_IMG;
    bodyData.iCompressType = UCS_MSGIMG_WITH_COMPRESS;

    bodyData.pcClientMsgId = strdup(pRequest->pcClientMsgId.toLocal8Bit().data());
    bodyData.pcFromUserName = strdup(pRequest->pcFromUserName.toLocal8Bit().data());
    bodyData.pcToUserName = strdup(pRequest->pcToUserName.toLocal8Bit().data());

    QByteArray data = pRequest->tData.mid(pRequest->iStartPos, pRequest->iDataLen);
    bodyData.tData.pcBuff = strdup(data.data());
    bodyData.tData.iLen = pRequest->tData.size();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    bodyData.pcMD5 = strdup(hash.data());

#define IMG_UPLOAD_SIZE (64 * 1024 + 1024)
    char packBuf[IMG_UPLOAD_SIZE] = { 0 };
    quint32 packSize = IMG_UPLOAD_SIZE;

    pack(ProxyProtocol::REQ_UPLOAD_MSGIMG, &bodyHeader, (void*)&bodyData,
         (char*)&packBuf, packSize);


    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackUploadMsgImgResponse(const QByteArray dataArray, UCSUploadMsgImgResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMUploadMsgImgResponse_t> bodyData;
    ByteBuffer data;

    data.clear();
    memset(&bodyData, 0x00, sizeof(bodyData));

    const char* unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_UPLOAD_MSGIMG, &bodyHeader, (void*)&bodyData, (char*)unpackBuf, unpackSize);
    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));

    if (pResponse->tBaseResponse.iRet != 0)
    {
        return;
    }

    if (bodyHeader.Ret != 0)
    {
        pResponse->tBaseResponse.iRet = kBodyHeader_t_errorcode;
        return;
    }

    pResponse->iMsgId = bodyData.iMsgId;
    pResponse->pcClientMsgId = charToQString(bodyData.pcClientMsgId);
    pResponse->pcFromUserName = charToQString(bodyData.pcFromUserName);
    pResponse->pcToUserName = charToQString(bodyData.pcToUserName);
    pResponse->iTotalLen = bodyData.iTotalLen;
    pResponse->iStartPos = bodyData.iStartPos;
    pResponse->iDataLen = bodyData.iDataLen;
    pResponse->iCreateTime = bodyData.iCreateTime;
}

QByteArray UCSPackage::PackDownloadMsgImgRequest(const UCSDownloadMsgImgRequest_t *pRequest)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMDownloadMsgImgRequest_t> bodyData;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);

    bodyData.iMsgId = pRequest->iMsgId;
    bodyData.pcFromUserName = strdup(pRequest->pcFromUserName.toLocal8Bit().data());
    bodyData.iTotalLen = pRequest->iTotalLen;
    bodyData.iStartPos = pRequest->iStartPos;
    bodyData.iCompressType = pRequest->iCompressType;

    char packBuf[1024] = { 0 };
    quint32 packSize = sizeof(packBuf);

    pack(ProxyProtocol::REQ_DOWNLOAD_MSGIMG, &bodyHeader, (void*)&bodyData, (char*)&packBuf, packSize);

    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackDownloadMsgImgResponse(const QByteArray dataArray, UCSDownloadMsgImgResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMDownloadMsgImgResponse_t> bodyData;
    ByteBuffer data;

    data.clear();
    memset(&bodyData, 0x00, sizeof(bodyData));

    const char* unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_DOWNLOAD_MSGIMG, &bodyHeader, (void*)&bodyData, (char*)unpackBuf, unpackSize);
    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));

    if (pResponse->tBaseResponse.iRet != 0)
    {
        return;
    }

    if (bodyHeader.Ret != 0)
    {
        pResponse->tBaseResponse.iRet = kBodyHeader_t_errorcode;
        return;
    }

    pResponse->iMsgId = bodyData.iMsgId;
    pResponse->iTotalLen = bodyData.iTotalLen;
    pResponse->iStartPos = bodyData.iStartPos;
    pResponse->iDataLen = bodyData.iDataLen;
    pResponse->pcFromUserName = charToQString(bodyData.pcFromUserName);
    pResponse->pcToUserName = charToQString(bodyData.pcToUserName);
    pResponse->tData = QByteArray(bodyData.tData.pcBuff, bodyData.tData.iLen);
}

QByteArray UCSPackage::PackDownloadVoiceRequest(const UCSDownloadVoiceRequest_t *pRequest)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMDownloadVoiceRequest_t> bodyData;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);

    bodyData.iMsgId = pRequest->iMsgId;
    bodyData.iOffset = pRequest->iOffset;
    bodyData.iLength = pRequest->iLenght;
    bodyData.pcClientMsgId = strdup(pRequest->pcClientMsgId.toLocal8Bit().data());

    char packBuf[1024] = { 0 };
    quint32 packSize = sizeof(packBuf);

    pack(ProxyProtocol::REQ_DOWNLOAD_VOICE, &bodyHeader, (void*)&bodyData, (char*)&packBuf, packSize);

    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackDownloadVoiceReponse(const QByteArray dataArray, UCSDownloadVoiceResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMDownloadVoiceResponse_t> bodyData;
    ByteBuffer data;

    data.clear();
    memset(&bodyData, 0x00, sizeof(bodyData));

    const char* unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_DOWNLOAD_VOICE, &bodyHeader, (void*)&bodyData, (char*)unpackBuf, unpackSize);
    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));

    if (pResponse->tBaseResponse.iRet != 0)
    {
        return;
    }

    if (bodyHeader.Ret != 0)
    {
        pResponse->tBaseResponse.iRet = kBodyHeader_t_errorcode;
        return;
    }

    pResponse->iMsgId = bodyData.iMsgId;
    pResponse->iOffset = bodyData.iOffset;
    pResponse->iLength = bodyData.iLength;
    pResponse->iVoiceLength = bodyData.iVoiceLength;
    pResponse->pcClientMsgId = charToQString(bodyData.pcClientMsgId);
    pResponse->tData = QByteArray(bodyData.tData.pcBuff, bodyData.tData.iLen);
    pResponse->iEndFlag = bodyData.iEndFlag;
    pResponse->iCancelFlag = bodyData.iCancelFlag;
}

QByteArray UCSPackage::PackCreateGroupRequest(const UCSCreateGroupRequest_t *pRequest)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMCreateGroupRequest_t> bodyData;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);

    bodyData.tIntroDuce.pcBuff = strdup(pRequest->tIntroduce.toLocal8Bit().data());
    bodyData.iMemberCount = pRequest->ptMemberList.size();
    bodyData.ptMemberList = (IMMemberReq_t*)calloc(bodyData.iMemberCount, sizeof(IMMemberReq_t));
    for (quint32 i = 0; i < bodyData.iMemberCount; ++i)
    {
        QString userName = pRequest->ptMemberList.at(i);
        bodyData.ptMemberList[i].tMemberName.pcBuff =
                strdup(userName.toLocal8Bit().data());
    }

    char packBuf[2048] = { 0 };
    quint32 packSize = sizeof(packBuf);

    pack(ProxyProtocol::REQ_CREATE_GROUP, &bodyHeader, (void*)&bodyData, (char*)&packBuf, packSize);

    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackCreateGroupResponse(const QByteArray dataArray, UCSCreateGroupResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMCreateGroupResponse_t> bodyData;
    ByteBuffer data;

    data.clear();
    memset(&bodyData, 0x00, sizeof(bodyData));

    const char* unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_CREATE_GROUP, &bodyHeader, (void*)&bodyData, (char*)unpackBuf, unpackSize);
    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));

    if (pResponse->tBaseResponse.iRet != 0)
    {
        return;
    }

    if (bodyHeader.Ret != 0)
    {
        pResponse->tBaseResponse.iRet = kBodyHeader_t_errorcode;
        return;
    }

    pResponse->tIntroduce = charToQString(bodyData.tIntroDuce.pcBuff);
    pResponse->tPYInitial = charToQString(bodyData.tPYInitial.pcBuff);
    pResponse->tQuanPin = charToQString(bodyData.tQuanPin.pcBuff);
    pResponse->iMemberCount = bodyData.iMemberCount;
    pResponse->iChatRoomId = bodyData.iChatRoomId;

    for (quint32 i = 0; i < bodyData.iMemberCount; ++i)
    {
        UCSMemberResp_t member;
        member.tMemberName = charToQString(bodyData.ptMemberList[i].tMemberName.pcBuff);
        member.iMemberStatus = bodyData.ptMemberList[i].iMemberStatus;
        member.tNickName = charToQString(bodyData.ptMemberList[i].tNickName.pcBuff);
        member.iSex = bodyData.ptMemberList[i].iSex;

        pResponse->ptMemberList.append(member);
    }
}

QByteArray UCSPackage::PackAddGroupMemberRequest(const UCSAddGroupMemberRequest_t *pRequest)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMAddGroupMemberRequest_t> bodyData;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);

    bodyData.iChatRoomId = pRequest->iChatRoomId;
    bodyData.iMemberCount = pRequest->ptMemberList.size();
    bodyData.ptMemberList = (IMMemberReq_t*)calloc(bodyData.iMemberCount, sizeof(IMMemberReq_t));
    for (quint32 i = 0; i < bodyData.iMemberCount; ++i)
    {
        QString userName = pRequest->ptMemberList.at(i);
        bodyData.ptMemberList[i].tMemberName.pcBuff =
                strdup(userName.toLocal8Bit().data());
    }

    char packBuf[2048] = { 0 };
    quint32 packSize = sizeof(packBuf);

    pack(ProxyProtocol::REQ_ADD_GROUP_MEMBER, &bodyHeader, (void*)&bodyData, (char*)&packBuf, packSize);

    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackAddGroupMemberResponse(const QByteArray dataArray, UCSAddGroupMemberResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMAddGroupMemberResponse_t> bodyData;
    ByteBuffer data;

    data.clear();
    memset(&bodyData, 0x00, sizeof(bodyData));

    const char* unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_ADD_GROUP_MEMBER, &bodyHeader, (void*)&bodyData, (char*)unpackBuf, unpackSize);
    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));

    if (pResponse->tBaseResponse.iRet != 0)
    {
        return;
    }

    if (bodyHeader.Ret != 0)
    {
        pResponse->tBaseResponse.iRet = kBodyHeader_t_errorcode;
        return;
    }

    pResponse->iMemberCount = bodyData.iMemberCount;
    pResponse->iChatRoomId = bodyData.iChatRoomId;

    for (quint32 i = 0; i < bodyData.iMemberCount; ++i)
    {
        UCSMemberResp_t member;
        member.tMemberName = charToQString(bodyData.ptMemberList[i].tMemberName.pcBuff);
        member.iMemberStatus = bodyData.ptMemberList[i].iMemberStatus;
        member.tNickName = charToQString(bodyData.ptMemberList[i].tNickName.pcBuff);
        member.iSex = bodyData.ptMemberList[i].iSex;

        pResponse->ptMemberList.append(member);
    }
}

QByteArray UCSPackage::PackDelGroupMemberRequest(const UCSDelGroupMemberRequest_t *pRequest)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMDelGroupMemberRequest_t> bodyData;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);

    bodyData.iChatRoomId = pRequest->iChatRoomId;
    bodyData.iMemberCount = pRequest->ptMemberList.size();
    bodyData.ptMemberList = (IMDelMemberReq_t*)calloc(bodyData.iMemberCount, sizeof(IMDelMemberReq_t));
    for (quint32 i = 0; i < bodyData.iMemberCount; ++i)
    {
        QString userName = pRequest->ptMemberList.at(i);
        bodyData.ptMemberList[i].tMemberName.pcBuff =
                strdup(userName.toLocal8Bit().data());
    }

    char packBuf[2048] = { 0 };
    quint32 packSize = sizeof(packBuf);

    pack(ProxyProtocol::REQ_DEL_GROUP_MEMBER, &bodyHeader, (void*)&bodyData, (char*)&packBuf, packSize);

    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackDelGroupMemberResponse(const QByteArray dataArray, UCSDelGroupMemberResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMDelGroupMemberResponse_t> bodyData;
    ByteBuffer data;

    data.clear();
    memset(&bodyData, 0x00, sizeof(bodyData));

    const char* unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_DEL_GROUP_MEMBER, &bodyHeader, (void*)&bodyData, (char*)unpackBuf, unpackSize);
    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));

    if (pResponse->tBaseResponse.iRet != 0)
    {
        return;
    }

    if (bodyHeader.Ret != 0)
    {
        pResponse->tBaseResponse.iRet = kBodyHeader_t_errorcode;
        return;
    }

    pResponse->iMemberCount = bodyData.iMemberCount;
    pResponse->iChatRoomId = bodyData.iChatRoomId;

    for (quint32 i = 0; i < bodyData.iMemberCount; ++i)
    {
        QString userName = charToQString(bodyData.ptMemberList[i].tMemberName.pcBuff);
        pResponse->ptMemberList.append(userName);
    }
}

QByteArray UCSPackage::PackQuitGroupRequest(const UCSQuitGroupRequest_t *pRequest)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMQuitGroupRequest_t> bodyData;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);

    bodyData.tQuitChatRoom.iChatRoomId = pRequest->tQuitChatRoom.iChatRoomId;
    bodyData.tQuitChatRoom.tUserName.pcBuff =
            strdup(pRequest->tQuitChatRoom.tUserName.toLocal8Bit().data());

    char packBuf[1024] = { 0 };
    quint32 packSize = sizeof(packBuf);

    pack(ProxyProtocol::REQ_QUIT_GROUP, &bodyHeader, (void*)&bodyData, (char*)&packBuf, packSize);

    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackQuitGroupResponse(const QByteArray dataArray, UCSQuitGroupResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMQuitGroupResponse_t> bodyData;
    ByteBuffer data;

    data.clear();
    memset(&bodyData, 0x00, sizeof(bodyData));

    const char* unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_DEL_GROUP_MEMBER, &bodyHeader, (void*)&bodyData, (char*)unpackBuf, unpackSize);
    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));

    if (pResponse->tBaseResponse.iRet != 0)
    {
        return;
    }

    if (bodyHeader.Ret != 0)
    {
        pResponse->tBaseResponse.iRet = kBodyHeader_t_errorcode;
        return;
    }

    pResponse->tQuitChatRoom.iChatRoomId = bodyData.tQuitChatRoom.iChatRoomId;
    pResponse->tQuitChatRoom.tUserName = charToQString(bodyData.tQuitChatRoom.tUserName.pcBuff);
}

QByteArray UCSPackage::PackModGroupTopicRequest(const UCSModGroupTopicRequest_t *pRequest)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMNewSyncRequest_t> bodyData;

    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);
    bodyData.tBaseRequest.iScene = UCS_NEWSYNC_SCENE_OTHER;
    bodyData.tBaseRequest.iSeq = pRequest->iChatRoomId;

    bodyData.iScene = UCS_NEWSYNC_SCENE_OTHER;
    bodyData.iSelector = UCS_NEWSYNC_IMCONTACT;

    /* contact sync key */
    UCSIMKeyVal_t contactKeyVal = pRequest->tSyncKey.ptKey.at(0);
    Comm::SKSmartPickleStruct<IMMsgPickle, IMSyncKey_t> curSyncKey;
    memset(&curSyncKey, 0x00, sizeof(curSyncKey));
    curSyncKey.iKeyCount = 1;
    curSyncKey.ptKey = (IMKeyVal_t*)calloc(curSyncKey.iKeyCount, sizeof(IMKeyVal_t));
    curSyncKey.ptKey[0].iKey = UCS_NEWSYNC_KEY_WXCONTACT;
    curSyncKey.ptKey[0].iVal = contactKeyVal.iVal;

    SKBuffer keyBuff;
    IMMsgPickle::ToBuffer(&curSyncKey, &keyBuff);
    bodyData.tKeyBuf.pcBuff = strdup((char*)keyBuff.GetBuffer());
    bodyData.tKeyBuf.iLen = keyBuff.GetLen();

    /* mod topic */
    Comm::SKSmartPickleStruct<IMMsgPickle, IMModGroupTopic_t> modTopic;
    memset(&modTopic, 0x00, sizeof(modTopic));
    modTopic.iChatRoomId = pRequest->iChatRoomId;
    modTopic.tChatRoomTopic.pcBuff = strdup(pRequest->tChatRoomTopic.toLocal8Bit().data());
    SKBuffer topicBuf;
    IMMsgPickle::ToBuffer(&modTopic, &topicBuf);

    bodyData.tOplog.iCount = 1;
    bodyData.tOplog.ptList = (IMCmdItem_t*)calloc(bodyData.tOplog.iCount, sizeof(IMCmdItem_t));
    bodyData.tOplog.ptList[0].iCmdId = UCS_SYNCCMD_MODGROUPTOPIC;
    bodyData.tOplog.ptList[0].tCmdBuf.pcBuff =
            strdup((char*)topicBuf.GetBuffer());
    bodyData.tOplog.ptList[0].tCmdBuf.iLen = topicBuf.GetLen();

    char packBuf[2048] = {0};
    quint32 packSize = sizeof(packBuf);
    pack(ProxyProtocol::REQ_NEW_SYNC, &bodyHeader, (void*)&bodyData,
         (char*)&packBuf, packSize);

    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackModGroupTopicReponse(const QByteArray dataArray, UCSModGroupTopicResponse_t *pResponse)
{
    Q_UNUSED(dataArray);
    Q_UNUSED(pResponse);
}

QByteArray UCSPackage::PackCustomMsgRequest(const UCSCustomMsgRequest_t *pRequest)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMSendLocationRequest_t> bodyData;
    memset(&bodyHeader, 0x00, sizeof(bodyHeader));
    PackBodyHeader(&bodyHeader);

    memset(&bodyData, 0x00, sizeof(bodyData));
    /* base request */
    PackBaseRequest(&bodyData.tBaseRequest);
    bodyData.pcClientMsgId = strdup(pRequest->pcClientMsgId.toLocal8Bit().data());
    bodyData.pcFromUserName = strdup(pRequest->pcFromUserName.toLocal8Bit().data());
    bodyData.pcToUserName = strdup(pRequest->pcToUserName.toLocal8Bit().data());
    bodyData.iStartPos = pRequest->iStartPos;
    bodyData.iTotalLen = pRequest->iTotalLen;
    bodyData.pcMsgSource = strdup("");
    bodyData.pcContent = NULL;

    QByteArray data = pRequest->tData.mid(pRequest->iStartPos, pRequest->iTotalLen);
    bodyData.tData.pcBuff = strdup(data.data());
    bodyData.tData.iLen = pRequest->tData.size();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    bodyData.pcMD5 = strdup(hash.data());

#define CUSTOM_MSG_LEN  (512 + 1024 * 16)
    char packBuf[CUSTOM_MSG_LEN] = { 0 };
    quint32 packSize = CUSTOM_MSG_LEN;

    pack(ProxyProtocol::REQ_SEND_CUSTOMMSG, &bodyHeader, (void*)&bodyData,
         (char*)&packBuf, packSize);

    return QByteArray(packBuf, packSize);
}

void UCSPackage::UnpackCustomMsgResponse(const QByteArray dataArray, UCSCustomMsgResponse_t *pResponse)
{
    BodyHead_t bodyHeader;
    Comm::SKSmartPickleStruct<IMMsgPickle, IMUploadMsgImgResponse_t> bodyData;
    ByteBuffer data;

    data.clear();
    memset(&bodyData, 0x00, sizeof(bodyData));

    const char* unpackBuf = dataArray.constData();
    qint32 unpackSize = dataArray.size();

    unpack(ProxyProtocol::RESP_UPLOAD_MSGIMG, &bodyHeader, (void*)&bodyData, (char*)unpackBuf, unpackSize);
    UnPackBaseResponse(&(bodyData.tBaseResponse), &(pResponse->tBaseResponse));

    if (pResponse->tBaseResponse.iRet != 0)
    {
        return;
    }

    if (bodyHeader.Ret != 0)
    {
        pResponse->tBaseResponse.iRet = kBodyHeader_t_errorcode;
        return;
    }

    pResponse->pcClientMsgId = charToQString(bodyData.pcClientMsgId);
    pResponse->pcFromUserName = charToQString(bodyData.pcFromUserName);
    pResponse->pcToUserName = charToQString(bodyData.pcToUserName);
    pResponse->iTotalLen = bodyData.iTotalLen;
    pResponse->iStartPos = bodyData.iStartPos;
    pResponse->iDataLen = bodyData.iDataLen;
    pResponse->iCreateTime = bodyData.iCreateTime;
    pResponse->iMsgId = bodyData.iMsgId;
}
