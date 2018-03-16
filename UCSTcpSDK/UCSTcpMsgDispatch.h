/*!
 * \file UCSTcpMsgDispatch.h
 * \brief 接收TCP消息，并分发到各注册模块
 * \author VintonLiu
 * \date 2018-01-22
*/

#ifndef UCSTCPMSGDISPATCH_H
#define UCSTCPMSGDISPATCH_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QMap>
#include "UCSEvent.h"

#pragma pack(push, 1)
typedef struct
{
    quint32 packetLen;      /* 4bytes, packet length, include header */
    qint16  headerLen;      /* 2bytes, header length */
    quint16 protocolVer;    /* 2bytes, protocol version */
    quint32 cmd;            /* 4bytes, command code */
    quint32 seq;            /* 4bytes, sequence number */
} TcpFrameHeader;
#define TCP_FRAME_HEADER_LEN (sizeof(TcpFrameHeader))
#pragma pack(pop)

/*!
 * \class UCSTcpMsgDispatch
 * \brief 接收TCP消息，并分发到各注册模块
*/
class UCSTcpMsgDispatch : public QObject
{
    Q_OBJECT
public:
    explicit UCSTcpMsgDispatch(QObject *parent = 0);
    ~UCSTcpMsgDispatch();

    void receivedPacket(QByteArray dataArray);

private:
    void parseMsgHeader(QByteArray dataArray, TcpFrameHeader *pHeader);

signals:
    void sig_finished();
    void sig_postMessage(quint32 cmd, QByteArray dataArray);

public slots:
//    void slot_receivedPacket(QByteArray dataArray);
    void slot_msgDispatch();

private:
    QMutex m_Mutex;
    bool m_bStopped;
    QVector<QByteArray> m_packetsVec;
    QByteArray m_buffer;
};

#endif // UCSTCPMSGDISPATCH_H
