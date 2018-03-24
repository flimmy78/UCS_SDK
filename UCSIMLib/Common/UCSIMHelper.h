#ifndef UCSIMHELPER_H
#define UCSIMHELPER_H

#include <qglobal.h>
#include <QSettings>
#include "UCSIMDefine.h"

#define UCS_IM_MSG_KEY          "msgKey"
#define UCS_IM_CONTACT_KEY      "contactKey"
#define UCS_LOGIN_TIME_KEY      "loginTime"
#define UCS_LOGIN_USERID_KEY    "loginId"

typedef enum UcsSettingsKey
{
    kUcsSettingsKeyLoginId,
    kUcsSettingsKeyLoginTime,
    kUcsSettingsKeyMsgSyncKey,
    kUcsSettingsKeyContactSyncKey,
} UcsSettingsKey;

/*!
 * \brief The UCSIMHelper class 辅助类，函数为静态函数，无需实例化
 */
class UCSIMHelper
{
public:
    UCSIMHelper();

    /*!
     * \brief tempDir 返回应用程序临时文件夹，若不存在则创建
     * \return 应用程序的临时文件夹
     */
    static QString tempDir();

    /*!
     * \brief userTempPath 返回每个用户的临时文件夹路径，若不存在则创建
     * \return
     */
    static QString userTempPath();

    /*!
     * \brief dataDir 返回应用程序数据文件夹，若不存在则创建
     * \return 应用程序数据存放文件夹
     */
    static QString dataDir();

    /*!
     * \brief pathForUserid 返回指定用户的本地目录路径
     * \return 返回每个userId专用目录路径
     */
    static QString userDataPath();

    /*!
     * \brief writeSettings 保存长久性配置，指定key-value对
     * \param key 键
     * \param value 值
     */
    static void saveSetting(const UcsSettingsKey &key, const QVariant &value);

    /*!
     * \brief readSettings 读取配置
     * \param key   键
     * \return 若对应键存在，则返回键值，否则返回默认值
     */
    static QVariant readSettings(const UcsSettingsKey &key);

    /*!
     * \brief checkConversationType 检查会话类型是否正确
     * \param conversationType 会话类型
     * \return true if conversation type valid, else false
     */
    static bool checkConversationType(UCS_IM_ConversationType conversationType);

    /*!
     * \brief checkMsgType 检查消息类型是否有效
     * \param msgType 消息类型
     * \return true if msg type valid, else false
     */
    static bool checkMsgType(UCS_IM_MsgType msgType);

    /*!
     * \brief messageId 生成发送message需要的messageId
     * \return 返回messageId
     */
    static QString messageId();

    /*!
     * \brief recvMsgId 生成接收消息的msgId
     * \return 返回msgId
     */
    static QString recvMsgId(qint32 index);

    /*!
     * \brief formatToUserName 根据会话类型格式化 targetId
     * \param targetId
     * \param conversationType
     * \return 格式化后的 targetId
     */
    static QString formatToUserName(QString targetId,
                                    UCS_IM_ConversationType conversationType);

    /*!
     * \brief getFromName 截取聊天消息内的fromId
     * \param fromUserName
     * \return
     */
    static QString getFromName(QString fromUserName);

    /*!
     * \brief getConvTypeByFromUserName 通过聊天消息内的fromUserName判断所属会话类型
     * \param fromUserName
     * \return 所属会话类型
     */
    static UCS_IM_ConversationType getConvTypeByFromUserName(const QString fromUserName);

    /*!
     * \brief saveImage 保存图片格式数据
     * \param msgId 消息Id
     * \param data 图片数据
     * \return 图片保存路径
     */
    static QString saveImage(const QString targetId,
                             const QString msgId,
                             const QByteArray data);

    /*!
     * \brief saveVoice 保存语音数据
     * \param msgId 消息Id
     * \param data 语音数据
     * \return 语音保存路径
     */
    static QString saveVoice(const QString targetId,
                             const QString msgId,
                             const QByteArray data);

    /*!
     * \brief getBigImgUrl 解析bigimgurl图片url
     * \param content
     * \return
     */
    static QString getBigImgUrl(QString content);

    /*!
     * \brief getVoiceDownloadInfo 解析语音消息内容各字段
     * \param content 语音消息xml格式内容
     * \param lenInBytes 语音数据长度
     * \param duration 语音时长
     * \param msgId 消息Id
     */
    static void getVoiceDownloadInfo(const QString content,
                                     qint32 &lenInBytes,
                                     qint32 &duration,
                                     QString &msgId);

private:

};

#endif // UCSIMHELPER_H
