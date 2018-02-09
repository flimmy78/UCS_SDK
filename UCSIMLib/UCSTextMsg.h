#ifndef UCSTEXTMSG_H
#define UCSTEXTMSG_H

#include <qglobal.h>
#include "UCSMsgContent.h"

class UCSTextMsg : public UCSMsgContent
{
public:
    explicit UCSTextMsg();
    explicit UCSTextMsg(QString content);

public:
    /*!
     * \brief content 文本消息内容
     */
    QString m_content;
    QString content() const;
};

#endif // UCSTEXTMSG_H
