#include "middleLeftWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QDebug>
#include "absFiles/myPushButton.h"


MiddleLeftWidget::MiddleLeftWidget(QWidget *parent, int width)
    : QWidget(parent)
    , m_stackWid(this)
    , m_searchLine(this)
    , m_contractsSWid(this, width)
    , m_callListSWid(this)
    , m_imMsgSWid(this)
    , m_searchSWid(this)
{
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(242, 242, 242));
    setPalette(palette);
    setAutoFillBackground(true);

    setMouseTracking(true);
    setFixedWidth(width);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    installEventFilter(this);

    initLayout();
    initConnection();
    initMisc();
}

void MiddleLeftWidget::setMidRightWidget(MiddleRightWidget *p)
{
    m_pMidRightWid = p;
}

MiddleRightWidget *MiddleLeftWidget::midRightWidget()
{
    if ( m_pMidRightWid == Q_NULLPTR)
    {
        qDebug() << "m_pMidRightWid is NULL";
    }

    return m_pMidRightWid;
}

bool MiddleLeftWidget::eventFilter(QObject *watched, QEvent *event)
{
    return QObject::eventFilter(watched, event);
}

void MiddleLeftWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void MiddleLeftWidget::initLayout()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout;
    QHBoxLayout* pTopLayout = new QHBoxLayout;
    QHBoxLayout* pMidLayout = new QHBoxLayout;

    pTopLayout->addWidget(&m_searchLine);
    pTopLayout->setSpacing(0);
    pTopLayout->setContentsMargins(16, 14, 16, 8);

    m_stackWid.setFrameShadow(QFrame::Plain);
    m_stackWid.setFrameShape(QFrame::NoFrame);
    m_stackWid.addWidget(&m_callListSWid);
    m_stackWid.addWidget(&m_contractsSWid);
    m_stackWid.addWidget(&m_imMsgSWid);
    m_stackWid.addWidget(&m_searchSWid);

    pMidLayout->addWidget(&m_stackWid);
    pMidLayout->setSpacing(0);

    pMainLayout->addLayout(pTopLayout);
    pMainLayout->setSpacing(20);
    pMainLayout->addLayout(pMidLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void MiddleLeftWidget::initConnection()
{
    connect(&m_searchLine, SIGNAL(returnPressed()), this, SLOT(slot_lineSearch()));
    connect(&m_searchLine, SIGNAL(textChanged(QString)), this, SLOT(slot_lineSearch()));
    connect(&m_searchLine, SIGNAL(sig_FocusChanged(bool)), this, SLOT(slot_lineSearchFocusChanged(bool)));

}

void MiddleLeftWidget::initMisc()
{
    m_stackWid.setCurrentIndex(0);
    preIndex = 0;
}

void MiddleLeftWidget::slot_lineSearch()
{
    QLineEdit *ledit = (QLineEdit*)sender();
    QString text = ledit->text().simplified();

    qDebug() << "searching " << text;
}

void MiddleLeftWidget::slot_lineSearchFocusChanged(bool bFocusIn)
{
    if (bFocusIn)
    {
        m_stackWid.setCurrentIndex(3);
    }
    else
    {
        m_stackWid.setCurrentIndex(preIndex);
    }
}

void MiddleLeftWidget::slot_switchStack()
{
    StackButton *stackBtn = (StackButton*)sender();
    m_stackWid.setCurrentIndex(stackBtn->index());
    preIndex = stackBtn->index();
}
