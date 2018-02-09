#include "myChatTextEdit.h"

MyChatTextEdit::MyChatTextEdit(QWidget *parent)
    : QTextEdit(parent)
    , m_sendingMode(CTRL_ENTER_SEND)
{
    this->setLineWrapMode(WidgetWidth);
    this->setFrameShadow(QFrame::Plain);
    this->setFrameShape(QFrame::NoFrame);
    this->setLineWrapMode(QTextEdit::WidgetWidth);
    this->setContentsMargins(16, 8, 16, 8);
}

void MyChatTextEdit::setSendingMode(CustomSendingMode mode)
{
    m_sendingMode = mode;
}

CustomSendingMode MyChatTextEdit::sendingMode() const
{
    return m_sendingMode;
}

void MyChatTextEdit::keyPressEvent(QKeyEvent *e)
{
    e->ignore();

    if (e->key() == Qt::Key_Return)
    {
        auto modify = e->modifiers();
        if (m_sendingMode == CTRL_ENTER_SEND)
        {
            if (modify == Qt::ControlModifier)
            {
                emit sig_msgSending();
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
                emit sig_msgSending();
                this->clear();
                e->accept();
                return;
            }
        }
    }

    QTextEdit::keyPressEvent(e);
}
