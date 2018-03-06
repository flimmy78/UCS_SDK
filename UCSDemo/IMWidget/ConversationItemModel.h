#ifndef CONVERSATIONITEMMODEL_H
#define CONVERSATIONITEMMODEL_H

#include <QAbstractItemModel>
#include <qglobal.h>
#include "Interface/UCSIMDefine.h"

typedef enum
{
    SoloTypeConversation = 0,   ///< 单聊会话 >
    GroupTypeConversation,      ///< 群聊会话，包括群组或讨论组 >
    OtherTypeConversation,      ///< 其他会话类型 >
} ConversationItemType;

/*!
 * \brief IM聊天会话项
*/
typedef struct
{
    QString conversationId;     ///< 会话Id >
    UCS_IM_ConversationType conversationType;   ///< 会话类型 >
    QString headerPath;         ///< 会话图像 >
    QString conversationTitle;   ///< 会话名称 >
    QString lastMsgTime;        ///< 会话最后一条消息时间 >
    QString lastMsgContent;     ///< 会话最后一条消息内容 >
    quint32 unReadCount;        ///< 会话未读消息数 >
    bool isTop;                 ///< 是否置顶会话 >
    ConversationItemType itemType;  ///< 会话项类型 >
} ConversationItem;

class ConversationItemModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ConversationItemModel(QObject *parent = 0);

    // Basic functionality
//    QModelIndex index(int row, int column,
//                      const QModelIndex &parent = QModelIndex()) const override;
//    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // remove data
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // refresh model
    void refreshModel();

    void setModelData(QList<ConversationItem> *dataList);

private:
    QList<ConversationItem> *m_pDataList;
};

#endif // CONVERSATIONITEMMODEL_H
