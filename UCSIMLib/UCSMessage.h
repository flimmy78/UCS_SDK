#ifndef UCSMESSAGE_H
#define UCSMESSAGE_H

#include <QObject>
#include <qglobal.h>
#include "ucsimlib_global.h"
#include "UCSIMDefine.h"
#include <UCSTextMsg.h>
#include <UCSImageMsg.h>
#include <UCSVoiceMsg.h>
#include <UCSDiscussionNotification.h>
#include <UCSCustomMsg.h>

/*!
 * \struct UCSMessage
 * \brief 每条消息的结构
*/
class UCSIMLIBSHARED_EXPORT UCSMessage
{
public:
    UCSMessage();
    ~UCSMessage();
//    UCSMessage &operator=(const UCSMessage& message);

public:
    /*!
     * \brief 消息对应的会话类型。单聊会话、讨论组会话、群会话
    */
    UCS_IM_ConversationType conversationType;

    /*!
     * \brief 消息的类型。文本、语音、图片、地图等
    */
    UCS_IM_MsgType messageType;

    /*!
     * \brief 目标ID。(如果这条消息是群组消息，receiveId是群Id;如果这条消息是单聊消息，
     * receiveId是消息接收者的Id;如果这条消息是讨论组消息,receiveId是讨论组Id)
    */
    QString receivedId;

    /*!
     * \brief 消息Id。这条消息的唯一标识
    */
    qint64 messageId;

    /*!
     * \brief 消息方向。判断是发送的消息,还是接收的消息
    */
    UCSMessageDirection messageDirection;

    /*!
     * \brief 发送者Id。这条消息是谁发的。
    */
    QString senderUserId;

    /*!
     * \brief 发送者昵称。发送这条消息的人的昵称。可能为空
    */
    QString senderNickName;

    /*!
     * @brief 接收状态。仅仅用于接收消息，判断接收状态
     */
    UCSReceivedStatus receivedStatus;

    /*!
     * \brief 发送状态。仅仅用于自己发送消息时，判断发送状态
     */
    UCSSendStatus sendStatus;

    /*!
     * \brief 时间
     */
    qint64 time;

    /*!
     * \brief 保留字段，暂时没有被使用
     */
    QString objectName;

    /*!
     * \brief 消息内容实例。
     * 如果这个会话对应的消息类型是文本，那么这个字段存储的就是UCSTextMsg
     * 类型的对象;如果是图片，就是一个UCSImageMsg类型的对象
     */
    UCSMsgContent *content;

    /*!
     * \brief 附加字段，暂未被使用
     */
    QString extra;
};

#endif // UCSMESSAGE_H
