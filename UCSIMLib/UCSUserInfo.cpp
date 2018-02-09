#include "UCSUserInfo.h"

UCSUserInfo::UCSUserInfo()
{

}

UCSUserInfo::UCSUserInfo(QString userid,
                         QString nickName,
                         QString portraitUri)
    : m_userId(userid)
    , m_nickName(nickName)
    , m_portraitUri(portraitUri)
{

}

QString UCSUserInfo::userId() const
{
    return m_userId;
}

void UCSUserInfo::setUserId(const QString &userid)
{
    m_userId = userid;
}

QString UCSUserInfo::portraitUri() const
{
    return m_portraitUri;
}

void UCSUserInfo::setPortraitUri(const QString &portraitUri)
{
    m_portraitUri = portraitUri;
}

QString UCSUserInfo::nickName() const
{
    return m_nickName;
}

void UCSUserInfo::setNickName(const QString &nickName)
{
    m_nickName = nickName;
}
