#ifndef UCSVOICEMSG_H
#define UCSVOICEMSG_H

#include <qglobal.h>
#include "UCSMsgContent.h"

class UCSVoiceMsg : public UCSMsgContent
{
public:
    UCSVoiceMsg();

public:
    /*!
     * \brief amrAudioData AMR格式的语音数据。(发送语音时必填)
     */
    QByteArray amrAudioData;

    /*!
     * \brief voicePath 语音的本地路径。(接收语音时，这个参数存放语音在本地的地址)
     */
    QString voicePath;

    /*!
     * \brief duration 语音时长。(发送语音时，必填)
     */
    quint32 duration;
};

#endif // UCSVOICEMSG_H
