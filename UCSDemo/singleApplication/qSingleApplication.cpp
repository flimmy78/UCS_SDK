#include "qSingleApplication.h"
#include <QLocalSocket>
#include <QLocalServer>
#include <QDebug>

#define TIME_OUT    (500)

QSingleApplication::QSingleApplication(int &argc, char** argv)
    :QApplication(argc, argv)
    , _wid(NULL)
    , _isRunning(false)
    , _localServer(NULL)
{
    QCoreApplication::setOrganizationName("UCPAAS");
    QCoreApplication::setApplicationName("UCSDemo");

    _serverName = QCoreApplication::organizationName()
            + QCoreApplication::applicationName();

    initLocalConnection();
}

bool QSingleApplication::isRunning()
{
    return _isRunning;
}

/*
 * 通过socket通讯实现程序单例运行，监听到新连接时触发该函数
*/
void QSingleApplication::newLocalConnection()
{
    QLocalSocket *socket = _localServer->nextPendingConnection();
    if (socket)
    {
        socket->waitForReadyRead(2 * TIME_OUT);
        delete socket;
        activeWindow();
    }
}

void QSingleApplication::initLocalConnection()
{
    _isRunning = false;

    QLocalSocket socket;
    socket.connectToServer(_serverName);
    if (socket.waitForConnected(TIME_OUT))
    {
        /* 如果socket连接上 */
        qInfo() << "%1 already running." << _serverName.toLocal8Bit().constData();
        _isRunning = true;

        return;
    }

    /* 连接不上服务端，创建一个 */
    newLocalServer();
}

void QSingleApplication::newLocalServer()
{
    _localServer = new QLocalServer(this);
    connect(_localServer, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));

    if (!_localServer->listen(_serverName))
    {
        /* 监听失败，可能是程序崩溃时，残留进程服务导致，移除并重新监听 */
        if (_localServer->serverError() == QAbstractSocket::AddressInUseError)
        {
            QLocalServer::removeServer(_serverName);
            _localServer->listen(_serverName);
        }
    }
}

/*
 * 激活主窗口
*/
void QSingleApplication::activeWindow()
{
    if (_wid)
    {
        _wid->showNormal();        
        _wid->activateWindow();
        _wid->raise();
    }
}


