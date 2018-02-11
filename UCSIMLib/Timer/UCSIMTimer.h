#ifndef UCSIMTIMER_H
#define UCSIMTIMER_H

#include <QEvent>
#include <QObject>
#include <qglobal.h>
#include <QMutex>

class UCSIMTimer : public QObject
{
    Q_OBJECT
public:    
    static UCSIMTimer *getInstance();
    static void release();

    typedef enum ImTimerId
    {
        kMsgSendTimer = 0,      ///< 消息发送超时定时器 >
        kCreateGroupTimer,      ///< 创建讨论组超时定时器 >
        kAddGroupMemberTimer,   ///< 添加讨论组成员超时定时器 >
        kDelGroupMemberTimer,   ///< 删除讨论组成员超时定时器 >
        kQuitGroupTimer,        ///< 退出讨论组超时定时器 >
        kSetGroupTopicTimer,    ///< 修改讨论组主题超时定时器 >
        kTimerNum,
        kAllTimers
    } ImTimerId;

    typedef struct
    {
        qint32      timerId;
        quint32     timeout;
        QString     name;
    } ImTimer_t;

    /*!
     * \brief start 启动指定定时器
     * \param id
     */
    static void start(ImTimerId id);

    /*!
     * \brief kill 停止指定定时器
     * \param id
     */
    static void kill(ImTimerId id);

    /*!
     * \brief setTime 设置指定定时器的超时时间
     * \param id
     * \param timeout 定时器值，单位: 毫秒
     */
    static void setTime(ImTimerId id, qint32 timeout);

    /*!
     * \brief timeout 返回定时器超时时间
     * \param id
     * \return 超时时间, 单位：毫秒
     */
    static qint32 timeout(ImTimerId id);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    explicit UCSIMTimer(QObject *parent = 0);
    UCSIMTimer(const UCSIMTimer &other);
    UCSIMTimer &operator =(const UCSIMTimer &other);
    ~UCSIMTimer();

    /*!
     * \brief initTimers 初始化所有定时器
     */
    void initTimers();

    /*!
     * \brief timerStart 启动指定定时器
     * \param id
     */
    void timerStart(ImTimerId id);

    /*!
     * \brief timerStop 停止指定定时器
     * \param id
     */
    void timerStop(ImTimerId id);

    /*!
     * \brief setTimeout 设置指定定时器的超时时间
     * \param id
     * \param timeout 定时器值，单位: 毫秒
     */
    void setTimeout(ImTimerId id, qint32 timeout);

    /*!
     * \brief timerIndex 根据系统定时器Id反查自定义Id
     * \param timerId
     * \return
     */
    ImTimerId timerIndex(qint32 timerId);

    /*!
     * \brief timeoutValue 返回定时器超时时间
     * \param id
     * \return 超时时间, 单位：毫秒
     */
    qint32 timeoutValue(ImTimerId id);

signals:
    void sig_timeout(UCSIMTimer::ImTimerId);

public slots:

private:
    static QMutex m_mutex;
    static UCSIMTimer *m_instance;

    ImTimer_t m_timers[kTimerNum];
};

#endif // UCSIMTIMER_H
