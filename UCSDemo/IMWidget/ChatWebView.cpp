#include "ChatWebView.h"
#include <QMenu>
#include <QWebEnginePage>
#include "CommonHelper.h"

ChatWebView::ChatWebView(QWidget *parent)
    : QWebEngineView(parent)
    , m_isLoadFinished(false)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWebEnginePage *page = new QWebEnginePage(this);
    page->setUrl(QUrl("qrc:/Resources/Chat/messageBox.html"));
    this->setPage(page);

    this->setObjectName("ChatWebView");
    initConnections();
}

void ChatWebView::msgShow(const ChatMsgModel &msg)
{
    if (msg.isSender)
    {
        sendMsgShow(msg);
    }
    else
    {
        recvMsgShow(msg);
    }
}

void ChatWebView::sendMsgShow(const ChatMsgModel &msg)
{
    QString time = CommonHelper::timeToString(msg.timestamp, "yyyy-MM-dd hh:mm:ss dddd");
    QString name = QString("%1 <a href=\"javascript:void(0);\" onclick=\"nameClick(this);return false;\" userId=\"%2\">%3</a>")
                        .arg(time).arg(msg.userName).arg(msg.nickName);

    QString content;
    if (msg.type == MessageBodyType_Image)
    {
        content = QString("<img src=\"%1\" width=\"80px\" heigth=\"100px\"/>").arg(msg.thumbnailLocalPath);
    }
    else
    {
        content = msg.content;
    }
    QString html = QString("appendMyMessage('%1', '%2');scrollBottom();")
                        .arg(name)
                        .arg(content);


    this->page()->runJavaScript(html);
}

void ChatWebView::recvMsgShow(const ChatMsgModel &msg)
{
    QString time = CommonHelper::timeToString(msg.timestamp, "yyyy-MM-dd hh:mm:ss dddd");
//    QString otherHead = QString("<img src=qrc:/images/u29.png width='50px' heigth='50px'/><span>%1</span>")
//                                .arg(time);
    QString othername = QString("<a href=\"javascript:void(0);\" onclick=\"nameClick(this);return false;\" userId=\"%1\">%2</a> %3")
                        .arg(msg.userName).arg(msg.nickName).arg(time);

    QString html = QString("appendOtherMessage('%1', '%2');scrollBottom();")
                            .arg(othername)
                            .arg(msg.content);
    this->page()->runJavaScript(html);
}

bool ChatWebView::isLoadFinished() const
{
    return m_isLoadFinished;
}

void ChatWebView::clearContent()
{
    QString html = QString("clear();");
    this->page()->runJavaScript(html);
}

QWebEngineView *ChatWebView::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type);

//    return this;
    return NULL;
}

void ChatWebView::onLoadFinished(bool isFinished)
{
    m_isLoadFinished = isFinished;

    if (isFinished)
    {
        emit sigLoadFinished();
    }
}

void ChatWebView::initConnections()
{
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
}

void ChatWebView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);

    QAction *pAct = new QAction(QStringLiteral("清空聊天记录"));
    QMenu *pMenu = new QMenu(this);
    pMenu->addAction(pAct);
    pMenu->exec(QCursor::pos());
}
