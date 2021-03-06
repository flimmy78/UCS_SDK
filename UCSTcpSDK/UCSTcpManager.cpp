﻿#include "UCSTcpManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <Common/UCSLogger.h>
#include "UCSPackage.h"
#include "ProxyProtocol.h"

UCSTcpManager::UCSTcpManager(QObject* parent)
    : QObject(parent)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kTcpManager,
            "UCSConnectionManager ctor");

    m_pTcpSocket = new UCSTcpSocket();

    m_pProxyThread = new UCSProxyRequestThread(this);
    m_pLoginManager = new UCSLoginManager(this);
    m_pMsgDispatch = new UCSTcpMsgDispatch;

    m_pDispatchThread = new QThread;
    m_pDispatchThread->setObjectName("TcpMsgDispatchThread");    
    m_pMsgDispatch->moveToThread(m_pDispatchThread);

    init();
    initConnections();

    m_pDispatchThread->start();
}

UCSTcpManager::~UCSTcpManager()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kTcpManager,
            "UCSConnectionManager ~dtor");

    m_cusListenMap.clear();
    m_listenMap.clear();

    if (m_pProxyThread != Q_NULLPTR)
    {
        m_pProxyThread->deleteLater();
        m_pProxyThread = Q_NULLPTR;
    }

    if (m_pTcpSocket != Q_NULLPTR)
    {
//        m_pTcpClient->deleteLater();
        delete m_pTcpSocket;
        m_pTcpSocket = Q_NULLPTR;
    }

    if (m_pLoginManager != Q_NULLPTR)
    {
        m_pLoginManager->deleteLater();
        m_pLoginManager = Q_NULLPTR;
    }

    if (m_pMsgDispatch != Q_NULLPTR)
    {
//        m_pMsgDispatch->deleteLater();
        delete m_pMsgDispatch;
        m_pMsgDispatch = Q_NULLPTR;
    }

    if (m_pDispatchThread != Q_NULLPTR)
    {
        m_pDispatchThread->deleteLater();
        m_pDispatchThread = Q_NULLPTR;
    }
}

void UCSTcpManager::DoUseOnLineEnv(bool isOnLine)
{
    m_pProxyThread->setIsOnLine(isOnLine);
}

void UCSTcpManager::doLogin(QString imToken)
{
    if (m_pProxyThread->setImToken(imToken) == NoError)
    {
        m_tokenData = m_pProxyThread->tokenData();
        m_imToken = m_pProxyThread->imToken();
        m_pLoginManager->setImToken(imToken);

        /* 启动 proxy 列表更新线程 */
        m_pProxyThread->start();
    }
    else
    {
//        emit sig_onError(InvalidToken);

        QList<QObject*> receivers = m_cusListenMap.values(kUCSLoginEvent);
        foreach (QObject *receiver, receivers)
        {
            UCSEvent::postEvent(receiver,
                                new UCSLoginEvent(ErrorCode_InvalidToken, m_tokenData.userId));
        }
    }
}

void UCSTcpManager::doLogout()
{
    m_pLoginManager->doLogout();
    m_pTcpSocket->doDisConnect();
}

void UCSTcpManager::setIMClient(QObject *obj)
{
    QMutexLocker locker(&m_Mutex);
    if (obj != Q_NULLPTR)
    {
        m_listenMap.insert(UCSIMModule, obj);
    }
    else
    {
        m_listenMap.remove(UCSIMModule);
    }

    if (m_pLoginManager != Q_NULLPTR)
    {
        if ((m_pLoginManager->state() == LoginSuccessed) ||
            (m_pLoginManager->state() == ReLoginSuccessed))
        {
            UCSEvent::postEvent(m_listenMap[UCSIMModule],
                                new UCSLoginStateEvent(m_pLoginManager->state(), m_tokenData.userId));
        }
    }
}

void UCSTcpManager::setVoIPClient(QObject *obj)
{
    QMutexLocker locker(&m_Mutex);
    if (obj != Q_NULLPTR)
    {
        m_listenMap.insert(UCSVoIPModule, obj);
    }
    else
    {
        m_listenMap.remove(UCSVoIPModule);
    }

    if (m_pLoginManager != Q_NULLPTR)
    {
        if ((m_pLoginManager->state() == LoginSuccessed) ||
            (m_pLoginManager->state() == ReLoginSuccessed))
        {
            UCSEvent::postEvent(m_listenMap[UCSVoIPModule],
                                new UCSLoginStateEvent(m_pLoginManager->state(), m_tokenData.userId));
        }
    }
}

void UCSTcpManager::registerEventListener(UCSCustomEventType eventType, QObject *receiver)
{
    QMutexLocker locker(&m_Mutex);
    if (receiver != Q_NULLPTR)
    {
        m_cusListenMap.insert(eventType, receiver);
    }
}

void UCSTcpManager::unRegisterEventListener(UCSCustomEventType eventType, QObject *receiver)
{
    QMutexLocker locker(&m_Mutex);
    if (receiver != Q_NULLPTR)
    {
        m_cusListenMap.remove(eventType, receiver);
    }
}

void UCSTcpManager::init()
{
    UCSEvent::setSockObj(m_pTcpSocket);

    m_tokenData.accId.clear();
    m_tokenData.appId.clear();
    m_tokenData.userId.clear();
    m_imToken.clear();

    m_cusListenMap.clear();

    m_listenMap.clear();
    m_listenMap.insert(UCSLoginModule, m_pLoginManager);
}

void UCSTcpManager::initConnections()
{
    connect(m_pProxyThread, SIGNAL(sigFinished(int)), this, SLOT(onUpdateProxyFinished(int)));

    connect(m_pTcpSocket, SIGNAL(sigStateChanged(UcsTcpState)), this, SLOT(onTcpStateChanged(UcsTcpState)));
    connect(m_pTcpSocket, SIGNAL(sigReadReady(QByteArray)), this, SLOT(onReadReady(QByteArray)));

    connect(m_pLoginManager, SIGNAL(sigStateChanged(UcsLoginState)), this, SLOT(onLoginStateChanged(UcsLoginState)));

    connect(m_pDispatchThread, SIGNAL(started()), m_pMsgDispatch, SLOT(slot_msgDispatch()));
    connect(m_pMsgDispatch, SIGNAL(sigFinished()), m_pDispatchThread, SLOT(quit()));
    connect(m_pMsgDispatch, SIGNAL(sigPostMessage(quint32,QByteArray)), this, SLOT(onPostMessage(quint32,QByteArray)));
}

void UCSTcpManager::onUpdateProxyFinished(int error)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kTcpManager,
            QString("onUpdateProxyFinished(%1)").arg(error));

    if (error == NoError)
    {
        m_ProxyList = m_pProxyThread->proxyList();
        m_pTcpSocket->setProxyList(m_pProxyThread->proxyList());
        m_pTcpSocket->doConnect();
    }
    else
    {
        QList<QObject*> receivers = m_cusListenMap.values(kUCSLoginEvent);
        UCSErrorCode code;
        if (error == RequestUrlError || error == ResponseError)
        {
            code = ErrorCode_ConnectServerFail;
        }
        else if (error == JsonFormatError)
        {
            code = ErrorCode_PacketParseError;
        }
        else
        {
            code = ErrorCode_PullProxyError;
        }
        foreach (QObject *receiver, receivers)
        {
            UCSEvent::postEvent(receiver,
                                new UCSLoginEvent(code, m_tokenData.userId));
        }
    }
}

void UCSTcpManager::onTcpStateChanged(UcsTcpState state)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kTcpManager,
            QString("onTcpStateChanged state(%1)").arg(state));

    switch (state) {
    case TcpConnected:
    case TcpReConnected:
    {
        if (m_pLoginManager)
        {
            m_pLoginManager->setTcpConnected(true);
            if (m_pLoginManager->lastState() == LoginSuccessed)
            {
                m_pLoginManager->doReAuth();
            }
            else
            {
                m_pLoginManager->doAuth();
            }
        }

        QMutexLocker locker(&m_Mutex);
        if (m_listenMap[UCSIMModule])
        {
            UCSEvent::postEvent(m_listenMap[UCSIMModule],
                                new UCSTcpStateEvent(state));
        }

        if (state == TcpReConnected)
        {
            QList<QObject*> receivers = m_cusListenMap.values(kUCSConnectStatusEvent);
            foreach (QObject *receiver, receivers)
            {
                UCSEvent::postEvent(receiver,
                                    new UCSConnectStatusEvent(UCSConnectionStatus_ReConnectSuccess));
            }
        }
    }
        break;

    case TcpDisconnected:
    {
        if (m_pLoginManager)
        {
            m_pLoginManager->setTcpConnected(false);
        }

        QMutexLocker locker(&m_Mutex);
        if (m_listenMap[UCSIMModule])
        {
            UCSEvent::postEvent(m_listenMap[UCSIMModule],
                                new UCSTcpStateEvent(state));
        }

        QList<QObject*> receivers = m_cusListenMap.values(kUCSConnectStatusEvent);
        foreach (QObject *receiver, receivers)
        {
            UCSEvent::postEvent(receiver,
                                new UCSConnectStatusEvent(UCSConnectionStatus_ConnectFail));
        }
    }
        break;

    case TcpWaitProxyUpdate:
    {
        /* 启动 proxy 列表更新线程 */
        m_pProxyThread->start();
    }
        break;

    default:
        break;
    }
}

void UCSTcpManager::onReadReady(QByteArray dataArray)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kTcpManager,
            QString("onReadReady size(%1)").arg(dataArray.size()));

    m_pMsgDispatch->receivedPacket(dataArray);
}

void UCSTcpManager::onLoginStateChanged(UcsLoginState state)
{
    switch (state) {
    case LoginSuccessed:
    case ReLoginSuccessed:
    {
        UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kTcpManager,
                QStringLiteral("登录成功"));

        QMutexLocker locker(&m_Mutex);
        if (m_listenMap.contains(UCSIMModule))
        {
            UCSEvent::postEvent(m_listenMap[UCSIMModule],
                                new UCSLoginStateEvent(state, m_tokenData.userId));
        }

        {
            QList<QObject*> receivers = m_cusListenMap.values(kUCSConnectStatusEvent);
            foreach (QObject *receiver, receivers)
            {
                UCSEvent::postEvent(receiver,
                                    new UCSConnectStatusEvent(UCSConnectionStatus_LoginSuccess));
            }
        }

        {
            QList<QObject*> receivers = m_cusListenMap.values(kUCSLoginEvent);
            foreach (QObject *receiver, receivers)
            {
                UCSEvent::postEvent(receiver,
                                    new UCSLoginEvent(ErrorCode_NoError, m_tokenData.userId));
            }
        }
    }
        break;

    case LoginFailed:
    case ReLoginFailed:
    {
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kTcpManager,
                QStringLiteral("登录失败"));

        {
            QMutexLocker locker(&m_Mutex);
            if (m_listenMap.contains(UCSIMModule))
            {
                UCSEvent::postEvent(m_listenMap[UCSIMModule],
                                    new UCSLoginStateEvent(state, m_tokenData.userId));
            }

            QList<QObject*> receivers = m_cusListenMap.values(kUCSLoginEvent);
            foreach (QObject *receiver, receivers)
            {
                UCSEvent::postEvent(receiver,
                                    new UCSLoginEvent(ErrorCode_LoginInfoError, ""));
            }
        }

        if (m_pTcpSocket && m_pLoginManager->lastState() != LoginFailed)
        {
            m_pTcpSocket->forceChangeProxy();
            m_pTcpSocket->doReConnect();
        }
    }
        break;

    case NoopNoResponse:
    {
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kTcpManager,
                QStringLiteral("心跳无响应"));
        if (m_pTcpSocket)
        {
            m_pTcpSocket->doReConnect();
        }
    }
        break;

    default:
        break;
    }
}

void UCSTcpManager::onPostMessage(quint32 cmd, QByteArray dataArray)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kTcpManager,
            QString("post message (%1)").arg(UCSPackage::cmdName(cmd)));

    UCSModule module = UCSIMModule;

    if (cmd == ProxyProtocol::RESP_AUTH ||
        cmd == ProxyProtocol::RESP_REAUTH ||
        cmd == ProxyProtocol::RESP_NOOP)
    {
        module = UCSLoginModule;
    }
    else if (cmd == ProxyProtocol::VOIP_CALL_MSG ||
             cmd == ProxyProtocol::VOIP_CONF_MSG ||
             cmd == ProxyProtocol::VOIP_CSRV_MSG)
    {
        module = UCSVoIPModule;
    }

    QMutexLocker locker(&m_Mutex);
    if (m_listenMap.contains(module))
    {
        if (module == UCSLoginModule)
        {
            UCSEvent::postEvent(m_listenMap[module],
                                new UCSRecvDataEvent(cmd,
                                                     dataArray,
                                                     kLoginDataEvent));
        }
        else if (module == UCSVoIPModule)
        {
            UCSEvent::postEvent(m_listenMap[module],
                                new UCSRecvDataEvent(cmd,
                                                     dataArray,
                                                     kVoIPDataEvent));
        }
        else if (module == UCSIMModule)
        {
            UCSEvent::postEvent(m_listenMap[module],
                                new UCSRecvDataEvent(cmd,
                                                     dataArray,
                                                     kIMDataEvent));
        }
    }
    else
    {
        UCS_LOG(UCSLogger::kTraceWarning, UCSLogger::kTcpManager,
                QString("failed post message to unkonwn module(%1)").arg(module));
    }
}
