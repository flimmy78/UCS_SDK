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

    void msgShow(const ChatMsgModel &msg);

    void clearContent();

    bool isLoadFinished() const;

protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void sigLoadFinished();

private slots:
    void onLoadFinished(bool isFinished);

private:
    void initConnections();

    void sendMsgShow(const ChatMsgModel &msg);

    void recvMsgShow(const ChatMsgModel &msg);

private:
    bool m_isLoadFinished;
};

#endif // CHATWEBVIEW_H
