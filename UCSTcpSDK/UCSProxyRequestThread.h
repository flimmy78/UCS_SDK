#ifndef UCSPROXYREQUESTTHREAD_H
#define UCSPROXYREQUESTTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QMutex>

#include "ucsTcpDefine.h"

typedef struct
{
    QString accId;
    QString appId;
    QString userId;
} TokenData;

class UCSProxyRequestThread : public QThread
{
    Q_OBJECT

public:
    explicit UCSProxyRequestThread(QObject *parent = 0);
    ~UCSProxyRequestThread();

    QString imToken() const;
    /* set login token
     * if token valid return 0, else return other
    */
    int setImToken(const QString &imToken);

    QStringList proxyList() const;

    TokenData tokenData() const;

    bool isOnLine() const;
    void setIsOnLine(bool isOnLine);

protected:
    void run() override;

private:
    int parseToken(const QString &imToken);

    UcsTcpError doRequest(QString baseUrl);
    UcsTcpError parseReply(QByteArray dataArray);

signals:
    void sigFinished(int errCode);

private slots:


private:
    bool m_isOnLine;
    QString m_imToken;
    QStringList m_baseUrlList;
    QStringList m_proxyList;
    TokenData m_tokenData;

    int m_requestTimes;

    QMutex m_Mutex;
};

#endif // UCSPROXYREQUESTTHREAD_H
