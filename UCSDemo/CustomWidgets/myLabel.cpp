#include "myLabel.h"

MyLabel::MyLabel(QWidget *parent)
    : QLabel(parent)
{
    /* text cursor */
    setCursor(Qt::ArrowCursor);
    /* text can select for copy */
    setTextInteractionFlags(this->textInteractionFlags() | Qt::TextSelectableByMouse);

    this->setContentsMargins(8, 0, 8, 0);
}

MyLabel::MyLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    /* text cursor */
    setCursor(Qt::IBeamCursor);
    /* text can select for copy */
    setTextInteractionFlags(this->textInteractionFlags() | Qt::TextSelectableByMouse);

    this->setContentsMargins(8, 0, 8, 0);
}

MyLabel::~MyLabel()
{

}
