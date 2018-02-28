#ifndef UCSDISCUSSIONNOTIFICATION_H
#define UCSDISCUSSIONNOTIFICATION_H

#include <QObject>
#include <qglobal.h>
#include <UCSMsgContent.h>

/*!
 * \brief The UCSDiscussionNotification class 讨论组通知类
 */
class UCSIMLIBSHARED_EXPORT UCSDiscussionNotification : public UCSMsgContent
{
public:
    explicit UCSDiscussionNotification();
    explicit UCSDiscussionNotification(QString operatorId,
                                       QString extension);
    ~UCSDiscussionNotification(){}

public:
    ///< 讨论组Id >
    QString m_operatorId;
    ///< 扩展字段，用于存储服务器下发的扩展信息。 >
    QString m_extension;
};

#endif // UCSDISCUSSIONNOTIFICATION_H
