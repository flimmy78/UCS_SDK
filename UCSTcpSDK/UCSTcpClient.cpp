#include "UCSTcpClient.h"
#include <QMutexLocker>

#include "UCSTcpManager.h"
#include "ucstcpversion.h"

QMutex UCSTcpClient::m_Mutex;
QSharedPointer<UCSTcpClient> UCSTcpClient::m_pInstance = Q_NULLPTR;

QSharedPointer<UCSTcpClient> &UCSTcpClient::Instance()
{
    if(m_pInstance.isNull())
    {
        QMutexLocker lock(&m_Mutex);
        if (m_pInstance.isNull())
        {
            m_pInstance = QSharedPointer<UCSTcpClient>(new UCSTcpClient());
        }
    }

    return m_pInstance;
}

UCSTcpClient::UCSTcpClient(QObject* parent)
    :QObject(parent)
{
    m_pConnManager = new UCSTcpManager();
    initConnections();
}

void UCSTcpClient::initConnections()
{

}

UCSTcpClient::~UCSTcpClient()
{
    if (m_pConnManager != NULL)
    {
        delete m_pConnManager;
        m_pConnManager = NULL;
    }
}

void UCSTcpClient::init()
{

}

void UCSTcpClient::doLogin(QString imToken)
{
    m_pConnManager->doLogin(imToken);
}

void UCSTcpClient::doLogout()
{
    m_pConnManager->doLogout();
}

void UCSTcpClient::setIMClient(QObject *obj)
{
    m_pConnManager->setIMClient(obj);
}

void UCSTcpClient::setVoIPClient(QObject *obj)
{
    m_pConnManager->setVoIPClient(obj);
}

QString UCSTcpClient::getVersion()
{
    return TCP_SDK_VERSION;
}

