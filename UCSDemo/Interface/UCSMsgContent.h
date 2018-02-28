#ifndef UCSMSGCONTENT_H
#define UCSMSGCONTENT_H

#include <qglobal.h>
#include <QObject>
#include "ucsimlib_global.h"

class UCSIMLIBSHARED_EXPORT UCSMsgContent
{
public:
    UCSMsgContent();
    ~UCSMsgContent(){}

public:
    /*!
     * \brief pushContent push消息内容
     */
    QString pushContent;

    /*!
     * \brief extra 保留字段
     */
    QString extra;
};

#endif // UCSMSGCONTENT_H
