#include "UCSLoginManager.h"
#include <QDebug>
#include <QMutexLocker>
#include "UCSIMStack/UCSPackage.h"
#include "UCSIMStack/ProxyProtocol.h"
#include "Common/UCSLogger.h"
#include "UCSEvent.h"

#define LOGIN_RETRY_TIMES   6

UCSLoginManager::UCSLoginManager(QObject *parent)
    : QObject(parent)
    , m_lastState(LoginFailed)
    , m_state(LoginFailed)
    , m_loginTimes(0)
    , m_bTcpConnected(true)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kLoginManager,
            "UCSLoginManager ctor");

    m_imToken.clear();

    initTimer();
}

UCSLoginManager::~UCSLoginManager()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kLoginManager,
            "UCSLoginManager ~dtor");

    timerStop(kAllTimer);
}

void UCSLoginManager::doAuth()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kLoginManager, __FUNCTION__);
    if (m_imToken.isEmpty())
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kLoginManager,
                QStringLiteral("无效登录token"));
        return;
    }
    if (!m_bTcpConnected)
    {
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kLoginManager,
                QStringLiteral("网络未连接，登录失败"));
        return;
    }

    UCSAuthRequest_t authReq;
    authReq.tLoginToken = m_imToken;
    QByteArray dataArray = UCSPackage::PackAuthRequest(&authReq);

    UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
            QString("doAuth size: %1").arg(dataArray.size()));

      UCSEvent::sendData(new UCSSendDataEvent(ProxyProtocol::REQ_AUTH,
                                             dataArray));

    /* 启动登录定时器 */
    timerStart(kLoginTimer);
}

void UCSLoginManager::doReAuth()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kLoginManager, __FUNCTION__);

    if (!m_bTcpConnected)
    {
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kLoginManager,
                QStringLiteral("网络未连接，重登陆失败"));
        return;
    }

    UCSReAuthRequest_t reAuthReq;
    QByteArray dataArray = UCSPackage::PackReAuthRequest(&reAuthReq);

    UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
            QString("doReAuth size: %1").arg(dataArray.size()));

//    emit sig_sendData(dataArray);
      UCSEvent::sendData(new UCSSendDataEvent(ProxyProtocol::REQ_REAUTH,
                                             dataArray));

    /* 启动登录定时器 */
    timerStart(kLoginTimer);
}

void UCSLoginManager::doNoop()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kLoginManager, __FUNCTION__);

    QByteArray dataArray = UCSPackage::PackNoopRequest();

    UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
            QString("doNoop size: %1").arg(dataArray.size()));

    UCSEvent::sendData(new UCSSendDataEvent(ProxyProtocol::REQ_NOOP,
                                         dataArray));

    ///< 启动心跳超时定时器 >
    timerStart(kNoopTimerT1);
}

UcsLoginState UCSLoginManager::state() const
{
    QMutexLocker locker((QMutex*)&m_Mutex);
    return m_state;
}

void UCSLoginManager::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timers[kNoopTimer].timerId)
    {
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kLoginManager,
                QString("Fired ").append(m_timers[kNoopTimer].name));

        /* 发送心跳 */
        doNoop();
    }
    else if (event->timerId() == m_timers[kNoopTimerT1].timerId)
    {
        timerStop(kNoopTimerT1);

        setState(NoopNoResponse);
        timerStop(kNoopTimer);
    }
    else if (event->timerId() == m_timers[kLoginTimer].timerId)
    {
        UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kLoginManager,
                QString("Fired ").append(m_timers[kLoginTimer].name));

        if ((++m_loginTimes) >= LOGIN_RETRY_TIMES ||
            (!m_bTcpConnected))
        {
            timerStop(kLoginTimer);

            if (m_state == LoginSuccessed)
            {
                setState(LoginFailed);
            }
            else
            {
                setState(ReLoginFailed);
            }
        }
        else
        {
            if (m_lastState == LoginSuccessed ||
                m_lastState == ReLoginSuccessed)
            {
                UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kLoginManager,
                        QStringLiteral("再次尝试重登陆..."));
                doReAuth();
            }
            else
            {
                UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kLoginManager,
                        QStringLiteral("再次尝试登录..."));
                doAuth();
            }
        }
    }
}

void UCSLoginManager::customEvent(QEvent *event)
{
    if (event->type() == kLoginDataEvent)
    {
        UCSRecvDataEvent *recvEvt = (UCSRecvDataEvent*)event;
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
                QString("customEvent recv cmd = %1").arg(recvEvt->cmd()));

        processRecvData(recvEvt->cmd(), recvEvt->data());
    }
}

void UCSLoginManager::processRecvData(int cmd, const QByteArray dataArray)
{
    switch (cmd) {
    case ProxyProtocol::RESP_AUTH:
    {
        parseAuthResp(dataArray);
    }
        break;

    case ProxyProtocol::RESP_REAUTH:
    {
        parseReAuthResp(dataArray);
    }
        break;

    case ProxyProtocol::RESP_NOOP:
    {
        UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kLoginManager,
                QStringLiteral("收到心跳响应..."));

        timerStop(kNoopTimerT1);
    }
        break;

    default:
        UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kLoginManager,
                QString("unknown cmd: %1").arg(cmd));
        break;
    }
}

void UCSLoginManager::parseAuthResp(const QByteArray dataArray)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kLoginManager, __FUNCTION__);

    UCSAuthResponse_t authRsp;
    UCSPackage::UnPackAuthResponse(dataArray, &authRsp);

    /* kill timer when received response */
    timerStop(kLoginTimer);

    if (authRsp.tBaseResponse.iRet == 0)
    {
        UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kLoginManager,
                QStringLiteral("登陆成功..."));

        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
                QString("iUin: %1").arg(authRsp.iUin));
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
                QString("tUserName: ").append(authRsp.tUserName));
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
                QString("tNickName: ").append(authRsp.tNickName));
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
                QString("tBindEmail: ").append(authRsp.tBindEmail));
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
                QString("tBindMobile: ").append(authRsp.tBindMobile));
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
                QString("iStatus: %1").arg(authRsp.iStatus));
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
                QString("sSessionKey: ").append(authRsp.sSessionKey));
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
                QString("tBirthdata: ").append(authRsp.tBirthdate));
        UCS_LOG(UCSLogger::kTraceDebug, UCSLogger::kLoginManager,
                QString("tClientNumber: ").append(authRsp.tClientNumber));

        setState(LoginSuccessed);
    }
    else
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kLoginManager,
                QStringLiteral("登陆失败..."));

        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kLoginManager,
                QString("iRet: %1").arg(authRsp.tBaseResponse.iRet));
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kLoginManager,
                QString("tErrMsg: ").append(authRsp.tBaseResponse.tErrMsg));

        timerStart(kLoginTimer);
    }
}

void UCSLoginManager::parseReAuthResp(const QByteArray dataArray)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kLoginManager, __FUNCTION__);

    UCSReAuthResponse_t reAuthRsp;
    UCSPackage::UnPackReAuthResponse(dataArray, &reAuthRsp);

    /* kill timer when received response */
    timerStop(kLoginTimer);

    if (reAuthRsp.tBaseReponse.iRet == 0)
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kLoginManager,
                QStringLiteral("重登陆成功..."));

        setState(ReLoginSuccessed);
    }
    else
    {
        UCS_LOG(UCSLogger::kTraceError, UCSLogger::kLoginManager,
                QString(QStringLiteral("重登陆失败..."))
                .append(" ret(%1)")
                .arg(reAuthRsp.tBaseReponse.iRet));

        timerStart(kLoginTimer);
    }
}

void UCSLoginManager::initTimer()
{
    m_timers[kNoopTimer].timerId = 0;
    m_timers[kNoopTimer].timeout = 30 * 1000; // 30s
    m_timers[kNoopTimer].name = "NoopTimer";

    m_timers[kNoopTimerT1].timerId = 0;
    m_timers[kNoopTimerT1].timeout = 5 * 1000; // 5s
    m_timers[kNoopTimerT1].name = "NoopTimerT1";

    m_timers[kLoginTimer].timerId = 0;
    m_timers[kLoginTimer].timeout = 3 * 1000; // 10s
    m_timers[kLoginTimer].name = "LoginTimer";
}

void UCSLoginManager::timerStart(UCSLoginManager::LoginTimerId id)
{
    /* kill before start */
    if (m_timers[id].timerId > 0)
    {
        killTimer(m_timers[id].timerId);
        m_timers[id].timerId = 0;
    }

    m_timers[id].timerId = startTimer(m_timers[id].timeout);

    UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kLoginManager,
            QString("start timer ").append(m_timers[id].name));
}

void UCSLoginManager::timerStop(UCSLoginManager::LoginTimerId id)
{
    if (id != kAllTimer)
    {
        if (m_timers[id].timerId > 0)
        {
            UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kLoginManager,
                    QString("kill timer ").append(m_timers[id].name));

            killTimer(m_timers[id].timerId);
            m_timers[id].timerId = 0;
        }
    }
    else
    {
        for (int idx = 0; idx < kTimerMaxNum; idx++)
        {
            if (m_timers[idx].timerId > 0)
            {
                UCS_LOG(UCSLogger::kTraceTimer, UCSLogger::kLoginManager,
                        QString("kill timer ").append(m_timers[idx].name));

                killTimer(m_timers[idx].timerId);
                m_timers[idx].timerId = 0;
            }
        }
    }
}

void UCSLoginManager::setState(UcsLoginState state)
{
    QMutexLocker locker(&m_Mutex);
    m_state = state;

    switch (state) {
    case LoginSuccessed:
    case ReLoginSuccessed:
    {
        /* 启动心跳定时器 */
        timerStart(kNoopTimer);

        /* 关闭登录定时器 */
        timerStop(kLoginTimer);

        m_lastState = state;
        emit sig_stateChanged(state);
    }
        break;

    case LoginFailed:
    case ReLoginFailed:
    {
        /* 登录失败，停止心跳 */
        timerStop(kNoopTimer);
        emit sig_stateChanged(state);
    }
        break;

    case NoopNoResponse:
    {
        /* 心跳无响应，默认为登录失败 */
        m_state = LoginFailed;
        emit sig_stateChanged(state);
    }
        break;

    default:
        break;
    }
}

UcsLoginState UCSLoginManager::lastState() const
{
    return m_lastState;
}

void UCSLoginManager::setTcpConnected(bool bConnected)
{
    m_bTcpConnected = bConnected;
    if (!bConnected)
    {
        timerStop(kAllTimer);
    }
}

void UCSLoginManager::setImToken(const QString &imToken)
{
    m_imToken = imToken;
}
