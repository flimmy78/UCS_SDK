/**
 * @brief 通过socket通讯实现程序单例运行
 * @author Vinton.Liu
 * @date 2017-12-27
*/

#ifndef QSINGLEAPPLICATION_H
#define QSINGLEAPPLICATION_H

#include <QApplication>
#include <QObject>
//#include <QLocalServer>
#include <QWidget>


class QLocalServer;

class QSingleApplication : public QApplication
{
    Q_OBJECT

public:
    QSingleApplication(int &argc, char** argv);

    bool isRunning();

public:
    QWidget *_wid;

private slots:
    /* 有新连接时触发 */
    void newLocalConnection();

private:
    /* 初始化本地连接 */
    void initLocalConnection();

    /* 创建服务端 */
    void newLocalServer();

    /* 激活窗口 */
    void activeWindow();

private:
    bool _isRunning; /* 是否已经有实例运行 */
    QLocalServer* _localServer; /* 本地socket server */
    QString _serverName;
};

#endif // QSINGLEAPPLICATION_H
