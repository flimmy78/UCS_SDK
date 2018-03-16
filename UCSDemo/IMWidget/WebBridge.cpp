#include "WebBridge.h"
#include "UCSLogger.h"

WebBridge::WebBridge(IMChatWidget *pChatWidget, QObject *parent)
    : QObject(parent)
    , m_pChatWidget(pChatWidget)
{
    setObjectName("WebBridge");
    initConnections();
}

void WebBridge::initConnections()
{

}

void WebBridge::onUserNameClicked(const QString &userName)
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            "onUserNameClicked: userName: " + userName);
}
