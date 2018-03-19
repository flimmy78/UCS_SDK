#include "UCSTcpMsgDispatch.h"
#include <QMutexLocker>
#include <QThread>
#include <QDebug>
#include <QtEndian>
#include <QCoreApplication>
#include <QDataStream>
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

    emit sigFinished();
}

void UCSTcpMsgDispatch::receivedPacket(QByteArray dataArray)
{
    QMutexLocker locker(&m_Mutex);
//    m_packetsVec.append(dataArray);

    m_buffer.append(dataArray);

    quint32 totalLen = m_buffer.size();
    while (totalLen)
    {
//        QDataStream packet(m_buffer);
//        packet.setByteOrder(QDataStream::BigEndian);

        if (totalLen < TCP_FRAME_HEADER_LEN)
        {
            break;
        }

        TcpFrameHeader header;
        parseMsgHeader(m_buffer, &header);
        if ( totalLen < header.packetLen)
        {
            break;
        }

        QByteArray packet = m_buffer.left(header.packetLen);
        m_packetsVec.append(packet);

        QByteArray remaining = m_buffer.right(totalLen - header.packetLen);
        totalLen = remaining.size();
        m_buffer = remaining;
    }
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
}

void UCSTcpMsgDispatch::slot_msgDispatch()
{
    while(true)
    {
//        qDebug("%s running", __FUNCTION__);
        {
            if (m_bStopped)
            {
                emit sigFinished();
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
                UCS_LOG(UCSLogger::kTraceInfo, UCSLogger::kMsgDispatch,
                        QString(QStringLiteral("收到数据 cmd: %1")).arg(header.cmd));

                emit sigPostMessage(header.cmd, dataArray);

                m_packetsVec.erase(m_packetsVec.begin());
            }
        }
        else
        {
            QThread::msleep(10);
        }
    }

    emit sigFinished();
}
