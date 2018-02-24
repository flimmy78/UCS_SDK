#ifndef RECEIVEDDATAEVENT_H
#define RECEIVEDDATAEVENT_H

#include <QObject>
#include <QEvent>
#include <qglobal.h>
#include "ucstcpsdk_global.h"
#include "UCSTcpDefine.h"

enum UCSEventType
{
    UCSEventBase = QEvent::User + 20000,
    /*
     * 消息接收分发事件类型
    */
    kLoginDataEvent = UCSEventBase,   ///<登录相关消息 >
    kIMDataEvent = UCSEventBase + 1,             ///<IM 消息 >
    kVoIPDataEvent = UCSEventBase + 2,           ///<VoIP 消息 >

    /*
     * 消息发送事件类型
    */
    kSendDataEvent = UCSEventBase + 100,    ///< 发送消息 >

    /*
     * TCP 连接事件类型
    */
    kTcpStateEvent = UCSEventBase + 110,    ///< TCP状态通知 >

    /*
     * 登录事件类型
    */
    kLoginStateEvent = UCSEventBase + 120,   ///< 登录状态通知 >
};

typedef enum UCSModule
{
    UCSLoginModule,
    UCSIMModule,
    UCSVoIPModule
} UCSModule;

/*!
 * \class UCSEvent
 * \brief 对QCoreApplication 事件发送的简单封装
*/
class UCSTCPSDKSHARED_EXPORT UCSEvent
{
public:
    UCSEvent() {}
    ~UCSEvent() {}
    static void postEvent(QObject *receiver, QEvent *event);
    static void postEvent(QObject *receiver, QEvent *event, int priority);

    /*!
     * \fn sendData
     * \brief 发送 Tcp数据
    */
    static void sendData(QEvent *event);

    /*!
     * \fn setSockObj
     * \brief 设置Tcp socket对象,方便统一socket发送tcp数据
     * \retval void
    */
    static void setSockObj(QObject *pSockObj);

private:
    static QMutex m_Mutex;
    static QObject *m_pSockObj;
};

/*!
 * \class UCSRecvDataEvent
 * \brief TCP 接收数据分发到各模块事件
*/
class UCSTCPSDKSHARED_EXPORT UCSRecvDataEvent : public QEvent
{
public:
    explicit UCSRecvDataEvent(quint32 cmd, QByteArray dataArray, UCSEventType type);

    ~UCSRecvDataEvent();

    QByteArray data() const;

    quint32 cmd() const;

private:
    quint32 m_cmd;
    QByteArray m_data;
};

/*!
 * \class UCSSendDataEvent
 * \brief 发送TCP数据事件
*/
class UCSTCPSDKSHARED_EXPORT UCSSendDataEvent : public QEvent
{
public:
    explicit UCSSendDataEvent(quint32 cmd, QByteArray dataArray);
    ~UCSSendDataEvent();

    QByteArray data() const;

    quint32 cmd() const;

private:
    quint32 m_cmd;
    QByteArray m_data;
};

/*!
 * \class UCSTcpStateEvent
 * \brief TCP 状态更新事件
*/
class UCSTCPSDKSHARED_EXPORT UCSTcpStateEvent : public QEvent
{
public:
    explicit UCSTcpStateEvent(UcsTcpState state);
    ~UCSTcpStateEvent() {}

    UcsTcpState state() const;

private:
    UcsTcpState m_state;
};

/*!
 * \class UCSLoginStateEvent
 * \brief 登录 状态更新事件
*/
class UCSTCPSDKSHARED_EXPORT UCSLoginStateEvent : public QEvent
{
public:
    explicit UCSLoginStateEvent(UcsLoginState state, QString userId);
    ~UCSLoginStateEvent() {}

    UcsLoginState state() const;

    QString userId() const;

private:
    UcsLoginState m_state;
    QString m_userId;
};

#endif // RECEIVEDDATAEVENT_H
