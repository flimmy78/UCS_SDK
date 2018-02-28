#ifndef UCSIMAGEMSG_H
#define UCSIMAGEMSG_H

#include <qglobal.h>
#include "UCSMsgContent.h"

/*!
 * \class UCSImageMsg
 * \brief 图片类型消息
*/
class UCSIMLIBSHARED_EXPORT UCSImageMsg : public UCSMsgContent
{
public:
    UCSImageMsg();
    ~UCSImageMsg() {}

public:
    /*!
     * \brief thumbnailLocalPath 缩略图本地路径，发送时需填写
     */
    QString thumbnailLocalPath;

    /*!
     * \brief imageRemoteUrl 大图的远程url。自己发送的图片消息，这个字段为空
     */
    QString imageRemoteUrl;

    /*!
     * \brief imageLocalPath 大图的本地路径，发送时需填写。收到的图片消息，这个字段为空.
     *
     */
    QString imageLocalPath;

    /*!
     * \brief originalImage 原图数据。
     * 发送时必填。
     * 当收到别人的图片消息时，这个字段为空。获取的图片可以通过 imageRemoteUrl
     * 这个参数自定义下载。
     * 当取出聊天的历史消息时，无论消息是发送的还是接收的，这个参数都为空，
     * 自己发送的图片可以通过 imageLocalPath 这个参数去获取；
     * 接收的图片可以通过imageRemoteUrl这个参数去获取。
     */
    QByteArray originalImage;
};

#endif // UCSIMAGEMSG_H
