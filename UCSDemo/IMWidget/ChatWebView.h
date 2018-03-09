#ifndef CHATWEBVIEW_H
#define CHATWEBVIEW_H

#include <QWidget>
#include <qglobal.h>
#include <QWebEngineView>
#include <QWebChannel>
#include "MsgConvert.h"

class ChatWebView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit ChatWebView(QWidget* parent = 0);

    void sendMsgShow(ChatMsgModel msg);

    void recvMsgShow(ChatMsgModel msg);

    void clearContent();

protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

};

#endif // CHATWEBVIEW_H
