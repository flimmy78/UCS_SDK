#ifndef UCSTCPDEFINE_H
#define UCSTCPDEFINE_H

#include <QString>

enum UcsTcpError
{
    NoError,
    TokenEmpty,
    InvalidToken,
    RequestUrlError,
    ResponseError,
    JsonFormatError,
    /* 更新proxy列表，返回空列表 */
    ProxyListEmpty,
    /* 更新proxy返回错误 */
    ProxyQueryError,
};

enum UcsTcpState
{
    TcpConnecting,
    TcpConnected,
    TcpDisconnected,
    TcpReConnecting,
    TcpReConnected,
    TcpWaitProxyUpdate
};

enum UcsLoginState
{
    LoginSuccessed,
    LoginFailed,
    ReLoginSuccessed,
    ReLoginFailed,
    LogoutSuccessed,
    NoopNoResponse,
};

#endif // UCSTCPDEFINE_H
