#ifndef VOICEINFODBMANAGER_H
#define VOICEINFODBMANAGER_H

#include <QObject>
#include <qglobal.h>
#include <UCSDBEntity.h>

class VoiceInfoDBManager
{
public:
    explicit VoiceInfoDBManager();

    /*!
     * \brief addVoiceInfo 保存语音下载信息
     * \param pVoice
     * \return true for success, else false
     */
    bool addVoiceInfo(const VoiceInfoEntity *pVoice);

    /*!
     * \brief delVoiceInfo 删除指定的语音下载信息
     * \param messageId
     * \return true for success, else false
     */
    bool delVoiceInfo(const QString messageId);

    /*!
     * \brief getVoiceInfo 获取指定的语音下载信息
     * \param messageId
     * \param voiceInfo 获取到的指定语音下载信息
     * \return true for success, else false
     */
    bool getVoiceInfo(const QString messageId, VoiceInfoEntity &voiceInfo);

private:
    QString m_createSql;
};

#endif // VOICEINFODBMANAGER_H
