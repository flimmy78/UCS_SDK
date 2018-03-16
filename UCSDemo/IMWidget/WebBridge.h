#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QObject>
#include <qglobal.h>
#include "IMChatWidget.h"

/*
    An instance of this class gets published over the WebChannel and is then accessible to HTML clients.
*/
class WebBridge : public QObject
{
    Q_OBJECT
public:
    explicit WebBridge(IMChatWidget *pChatWidget, QObject *parent = nullptr);
    void initConnections();

signals:
    void SigAppendMyMessage(QString name, QString message);
    void SigAppendOtherMessage(QString name, QString message);

public slots:
    void onUserNameClicked(const QString &userName);

private:
    IMChatWidget *m_pChatWidget;
};

#endif // WEBBRIDGE_H
