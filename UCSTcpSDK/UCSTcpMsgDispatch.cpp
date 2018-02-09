#include "UCSTcpMsgDispatch.h"
#include <QMutexLocker>
#include <QThread>
#include <QDebug>
#include <QtEndian>
#include <QCoreApplication>
#include "UCSIMStack/ProxyProtocol.h"
#include "Common/UCSLogger.h"
#include "UCSEvent.h"

UCSTcpMsgDispatch::UCSTcpMsgDispatch(QObject *parent)
    : QObject(parent)
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kMsgDispatch,
            "UCSTcpMsgDispatch ctor");

    m_bStopped = false;
}

UCSTcpMsgDispatch::~UCSTcpMsgDispatch()
{
    UCS_LOG(UCSLogger::kTraceApiCall, UCSLogger::kMsgDispatch,
            "UCSTcpMsgDispatch ~dtor");

    QMutexLocker locker(&m_Mutex);

    m_bStopped = true;

    emit sig_finished();
}

void UCSTcpMsgDispatch::receivedPacket(QByteArray dataArray)
{
    QMutexLocker locker(&m_Mutex);
    m_packetsVec.append(dataArray);
}

void UCSTcpMsgDispatch::parseMsgHeader(QByteArray dataArray, TcpFrameHeader *pHeader)
{
    const char *packet = dataArray.constData();

    memset(pHeader, 0x00, TCP_FRAME_HEADER_LEN);
    memcpy(pHeader, packet, TCP_FRAME_HEADER_LEN);

    pHeader->packetLen = qFromBigEndian(pHeader->packetLen);
    pHeader->headerLen = qFromBigEndian(pHeader->headerLen);
    pHeader->protocolVer = qFromBigEndian(pHeader->protocolVer);
    pHeader->cmd = qFromBigEndian(pHeader->cmd);
    pHeader->seq = qFromBigEndian(pHeader->seq);

    UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kMsgDispatch,
            QString(QStringLiteral("收到数据 cmd: %1")).arg(pHeader->cmd));
}

void UCSTcpMsgDispatch::slot_msgDispatch()
{
    while(true)
    {
//        qDebug("%s running", __FUNCTION__);
        {
            if (m_bStopped)
            {
                emit sig_finished();
                break;
            }
        }

        if (m_packetsVec.size() > 0)
        {
            QMutexLocker locker(&m_Mutex);
            if (m_packetsVec.size() > 0)
            {
                QByteArray dataArray(m_packetsVec.at(0));
                TcpFrameHeader header;

                parseMsgHeader(dataArray, &header);

                emit sig_postMessage(header.cmd, dataArray);

                m_packetsVec.erase(m_packetsVec.begin());
            }
        }
        QThread::msleep(20);
    }

    emit sig_finished();
}
