#include "ChatWebView.h"
#include <QWebEnginePage>
#include "CommonHelper.h"

ChatWebView::ChatWebView(QWidget *parent)
    : QWebEngineView(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWebEnginePage *page = new QWebEnginePage(this);
    page->setUrl(QUrl("qrc:/Resources/Chat/messageBox.html"));
    this->setPage(page);

    this->setObjectName("ChatWebView");
}

void ChatWebView::sendMsgShow(ChatMsgModel msg)
{
    QString time = CommonHelper::timeToString(msg.timestamp, "yyyy-MM-dd hh:mm:ss dddd");
//    QString MyHead = QString("<span>%1</span><img src=qrc:/images/u6.png width='50px' heigth='50px'/>")
//                            .arg(time);
    QString name = QString("%1 %2").arg(time, msg.userName);

//    QString html = QString("document.getElementById(\"content\").insertAdjacentHTML(\"beforeEnd\",\"<div style='overflow:hidden;'><p class='divMyHead'>%1</p><p class='triangle-right right'>%2</p></div>\")")
//                            .arg(name)
//                            .arg(msg.content.replace("\n", "</br>"));

    QString html = QString("appendMyMessage('%1', '%2');scrollBottom();")
                        .arg(name)
                        .arg(msg.content.replace("\n", "</br>"));

    this->page()->runJavaScript(html);
}

void ChatWebView::recvMsgShow(ChatMsgModel msg)
{
    QString time = CommonHelper::timeToString(msg.timestamp, "yyyy-MM-dd hh:mm:ss dddd");
//    QString otherHead = QString("<img src=qrc:/images/u29.png width='50px' heigth='50px'/><span>%1</span>")
//                                .arg(time);
    QString othername = QString("%1 %2").arg(msg.userName, time);
//    QString html = QString("document.getElementById(\"content\").insertAdjacentHTML(\"beforeEnd\",\"<div style='overflow:hidden;'><p class='divotherHead'>%1 </p><p class='triangle-left left'>%2</p></div>\")")
    QString html = QString("appendOtherMessage('%1', '%2');scrollBottom();")
                            .arg(othername)
                            .arg(msg.content.replace("\n", "</br>"));
    this->page()->runJavaScript(html);
}

void ChatWebView::clearContent()
{
    QString html = QString("clear();");
    this->page()->runJavaScript(html);
}

QWebEngineView *ChatWebView::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type);

    return this;
}
