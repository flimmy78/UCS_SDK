#ifndef UCSTCPDEFINE_H
#define UCSTCPDEFINE_H

#include <QString>

typedef enum UcsTcpError
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
} UcsTcpError;

typedef enum UcsTcpState
{
    TcpConnecting,
    TcpConnected,
    TcpDisconnected,
    TcpReConnecting,
    TcpReConnected,
    TcpWaitProxyUpdate
} UcsTcpState;

typedef enum UcsLoginState
{
    LoginSuccessed,
    LoginFailed,
    ReLoginSuccessed,
    ReLoginFailed,
    LogoutSuccessed,
    NoopNoResponse,
} UcsLoginState;

#endif // UCSTCPDEFINE_H
