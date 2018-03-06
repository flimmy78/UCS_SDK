#ifndef CHATMSGTABLEMODEL_H
#define CHATMSGTABLEMODEL_H

#include <QAbstractTableModel>
#include <qglobal.h>
#include "Interface/UCSIMSDKPublic.h"

typedef enum
{
    MessageBodyType_Text = 1,   ///< 文本类型 >
    MessageBodyType_Image,      ///< 图片类型 >
    MessageBodyType_Video,      ///< 视频类型 >
    MessageBodyType_Location,   ///< 定位类型 >
    MessageBodyType_Voice,      ///< 语音类型 >
    MessageBodyType_File,       ///< 文件类型 >
    MessageBodyType_Command,    ///< 命令类型 >
    MessageBodyType_Notify,     ///< 通知类型 >
    MessageBodyType_Custom,     ///< 自定义类型 >
} MessageBodyType;

typedef enum
{
    MessageSendState_Pending = 0,   ///< 待发送 >
    MessageSendState_Sending,       ///< 正在发送 >
    MessageSendState_Successed,     ///< 发送成功 >
    MessageSendState_Failed,        ///< 发送失败 >
} MessageSendState;

typedef struct
{
    quint64 msgId;
    QString headerUrl;
    QString nickName;
    QString userName;

    MessageBodyType type;           ///< 消息类型 >
    MessageSendState sendState;     ///< 发送状态 >
    QString from;       ///< 消息发送者 >
    QString to;         ///< 消息接收者 >
    bool isSender;      ///< 是否是发送者 >
    bool isRead;        ///< 是否已读 >
    bool isGroupChat;   ///< 是否群聊 >
    qint64 timestamp;  ///< 消息的发送或者接收的时间 >

    ///< text >
    QString content;

    ///< image >
    QSize size;
    QSize thumbnailSize;
    QString imageLocalPath;     ///< 图片本地路径 >
    QString imageRemoteUrl;     ///< 大图远程url >
    QString thumbnailLocalPath; ///< 缩略图本地路径 >
    QString thumbnailRemoteUrl; ///< 缩略图远程url >
    QImage image;               ///< 大图 >
    QImage thumbnailImage;      ///< 缩略图 >

    ///< audio >
    QString audioLocalPath;         ///< 语音本地路径 >
    QString audioRemoteUrl;         ///< 语音远程url >
    quint32 audioDuration;          ///< 语音时长 >
    bool    isPlaying;
    bool    isPlayed;
    UCSReceivedStatus downloadStatus;   ///< 语音下载状态 >

    ///< location >
    QString address;
    double latitude;
    double longitude;
    QImage locationImage;   ///< 地图缩略图 >
    QString coord_type;     ///< 坐标类型 >

    ///< video >
    QString videoPath;      ///< 视频本地路径 >
} ChatMsgItem;

enum ChatMsgRole
{
    HeaderRole = Qt::UserRole,
    NickNameRole,
    UserNameRole,
    SendTimeRole,
    DirectionRole,
    MsgTypeRole,
    ContentRole,
    thumbnailRole,
    imageRole
};

class ChatMsgTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ChatMsgTableModel(QObject *parent = 0);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    void setModelData(QList<ChatMsgItem> *pChatMsgList);
    void refreshModel();

private:
    QList<ChatMsgItem> *m_pMsgList;

};

#endif // CHATMSGTABLEMODEL_H
