#include "searchLineEdit.h"

SearchLineEdit::SearchLineEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_bFocusIn(false)
{
//    setMinimumHeight(26);
    setFixedSize(220, 26);
    setMouseTracking(true);
//    setContextMenuPolicy(Qt::NoContextMenu);

    setObjectName("SearchLineEdit");

    setMaxLength(60);
    setPlaceholderText(QStringLiteral("搜索"));

    installEventFilter(this);

    initLayout();
    initConnection();
}

bool SearchLineEdit::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
    {
        m_pBtn->setIcon(QIcon(":/images/btn_del.png"));
        m_bFocusIn = true;
        emit sig_FocusChanged(true);
    }
    else if(event->type() == QEvent::FocusOut)
    {
//        m_pBtn->setIcon(QIcon(":/images/btn_search_normal.png"));
//        emit sig_FocusChanged(false);
    }
    return QObject::eventFilter(watched, event);
}

void SearchLineEdit::slot_clearText()
{
    if (m_bFocusIn)
    {
        this->clear();
    }
    m_bFocusIn = false;
    m_pBtn->setIcon(QIcon(":/images/btn_search_normal.png"));
    emit sig_FocusChanged(false);
}

void SearchLineEdit::initLayout()
{
    QHBoxLayout* pLayout = new QHBoxLayout;

    m_pBtn = new QPushButton(this);
    m_pBtn->setFixedSize(30, this->height());
    m_pBtn->setIcon(QIcon(":/images/btn_search_normal.png"));
    m_pBtn->setCursor(Qt::ArrowCursor);
    m_pBtn->setFlat(true);
    m_pBtn->setStyleSheet("QPushButton{border:none;}");

    /* 防止文本框输入内容位于按钮之下 */
    QMargins margins = this->textMargins();
    setTextMargins(margins.left() + 5,
                   margins.top(),
                   m_pBtn->width() + 5,
                   margins.bottom());

    pLayout->addStretch();
    pLayout->addWidget(m_pBtn);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 5, 0);

    setLayout(pLayout);
}

void SearchLineEdit::initConnection()
{
    connect(m_pBtn, SIGNAL(clicked(bool)), this, SLOT(slot_clearText()));
}
