#ifndef UCSLOGINMANAGER_H
#define UCSLOGINMANAGER_H

#include <QEvent>
#include <QObject>
#include <QThread>
#include <QMutex>
#include "ucsTcpDefine.h"

class UCSLoginManager : public QObject
{
    Q_OBJECT
public:
    explicit UCSLoginManager(QObject *parent = 0);
    ~UCSLoginManager();

    void doAuth();
    void doReAuth();
    void doNoop();

    UcsLoginState state() const;

    void setImToken(const QString &imToken);
    void setTcpConnected(bool bConnected);

    UcsLoginState lastState() const;

protected:
    void timerEvent(QTimerEvent *event) override;
    void customEvent(QEvent *event);

private:
    enum LoginTimerId
    {
        kNoopTimer,   /* 心跳定时器 */
        kLoginTimer,  /* 登录超时定时器 */
        kTimerMaxNum,
        kAllTimer
    };

    typedef struct
    {
        qint32      timerId;
        quint32     timeout;
        QString     name;
    } LoginTimer;

    void processRecvData(int cmd, const QByteArray dataArray);
    void parseAuthResp(const QByteArray dataArray);
    void parseReAuthResp(const QByteArray dataArray);
    void initTimer();
    void timerStart(LoginTimerId id);
    void timerStop(LoginTimerId id);

    void setState(UcsLoginState state);

signals:
    void sig_stateChanged(UcsLoginState state);

public slots:


private:
    QMutex m_Mutex;
    QString m_imToken;

    UcsLoginState m_lastState;
    UcsLoginState m_state;

    quint32 m_noopTimes;   /* 心跳无响应次数 */
    quint32 m_loginTimes;  /* 登录尝试次数 */
    bool m_bTcpConnected;

    LoginTimer m_timers[kTimerMaxNum];
};

#endif // UCSLOGINMANAGER_H
