#include "ChatTextEdit.h"

#include <QFileInfo>
#include <QImageReader>

ChatTextEdit::ChatTextEdit(QWidget *parent)
    : QTextEdit(parent)
    , m_sendingMode(ENTER_SEND)
{
    this->setLineWrapMode(WidgetWidth);
    this->setFrameShadow(QFrame::Plain);
    this->setFrameShape(QFrame::NoFrame);
    this->setLineWrapMode(QTextEdit::WidgetWidth);
    this->setContentsMargins(16, 8, 16, 8);
}

void ChatTextEdit::setSendingMode(CustomSendingMode mode)
{
    m_sendingMode = mode;
}

CustomSendingMode ChatTextEdit::sendingMode() const
{
    return m_sendingMode;
}

void ChatTextEdit::keyPressEvent(QKeyEvent *e)
{
    e->ignore();

    if (e->key() == Qt::Key_Return)
    {
        auto modify = e->modifiers();
        if (m_sendingMode == CTRL_ENTER_SEND)
        {
            if (modify == Qt::ControlModifier)
            {
                emit sigSendMessage();
                this->clear();
                e->accept();
            }
        }
        else
        {
            if (modify == Qt::ControlModifier)
            {
                /* Ctrl + Enter to wrap line */
                e->setModifiers(modify^Qt::ControlModifier);
                e->accept();
            }
            else
            {
                emit sigSendMessage();
                this->clear();
                e->accept();
                return;
            }
        }
    }

    QTextEdit::keyPressEvent(e);
}

bool ChatTextEdit::canInsertFromMimeData(const QMimeData *source) const
{
    return source->hasImage() || source->hasUrls() ||
            QTextEdit::canInsertFromMimeData(source);
}

void ChatTextEdit::insertFromMimeData(const QMimeData *source)
{
    if (source->hasImage())
    {
        static int i = 1;
        QUrl url(QString("dropped_image_%1").arg(i++));
        dropImage(url, qvariant_cast<QImage>(source->imageData()));
    }
    else if (source->hasUrls())
    {
        foreach (QUrl url, source->urls())
        {
            QFileInfo info(url.toLocalFile());
            if (QImageReader::supportedImageFormats().contains(info.suffix().toLower().toLatin1()))
            {
                dropImage(url, QImage(info.filePath()));
            }
            else
            {
                dropTextFile(url);
            }
        }
    }
    else
    {
        QTextEdit::insertFromMimeData(source);
    }
}

void ChatTextEdit::dropImage(const QUrl &url, const QImage &image)
{
    if (!image.isNull())
    {
        document()->addResource(QTextDocument::ImageResource, url, image);
        textCursor().insertImage(url.toString());
    }
}

void ChatTextEdit::dropTextFile(const QUrl &url)
{
    QFile file(url.toLocalFile());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        textCursor().insertText(file.readAll());
    }
    file.close();
}
