#ifndef UCSUSERINFO_H
#define UCSUSERINFO_H

#include <QObject>
#include <qglobal.h>
#include "ucsimlib_global.h"

/*!
 * \class UCSUserInfo
 * \brief 用户信息
 */
class UCSIMLIBSHARED_EXPORT UCSUserInfo
{

public:
    explicit UCSUserInfo();

    /*!
     * \brief UCSUserInfo
     * \param userid 用户Id(必填)
     * \param nickName 昵称(选填)
     * \param portraitUri 头像URL(选填)
     * \param parent
     */
    explicit UCSUserInfo(QString userId,
                         QString nickName,
                         QString portraitUri);

    QString userId() const;
    void setUserId(const QString &userId);

    QString portraitUri() const;
    void setPortraitUri(const QString &portraitUri);

    QString nickName() const;
    void setNickName(const QString &nickName);

private:
    QString m_userId;
    QString m_nickName;
    QString m_portraitUri;
};

#endif // UCSUSERINFO_H
