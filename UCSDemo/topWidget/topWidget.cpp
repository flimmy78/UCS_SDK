#include "topWidget.h"
#include <QHBoxLayout>
#include <QRegExpValidator>
#include <QString>
#include <QDebug>

TopWidget::TopWidget(QWidget *parent) : QWidget(parent)
{
//    setMinimumSize(890, 48);
    setFixedHeight(48);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMouseTracking(true);
//    setStyleSheet("QWidget{background:transparent;}");

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255, 255, 255));
    setPalette(palette);
    setAutoFillBackground(true);
    installEventFilter(this);

    initWidget();
    initConnection();
}

void TopWidget::setTitle(QString title)
{
    m_lblTitle.setText(title);
}

void TopWidget::initWidget()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    QHBoxLayout *pCaptionLayout = new QHBoxLayout;

    /////////////////////////////////////////////////////////
    /* Title */
    m_lblTitle.adjustSize();
    m_lblTitle.setFixedHeight(26);
    m_lblTitle.setFont(QFont("微软雅黑", 13));

    ////////////////////////////////////////////////////////
    /* minimun, maximun, close */
    m_btnClose.setObjectName("btnClose");
    m_btnMax.setObjectName("btnMax");
    m_btnMin.setObjectName("btnMin");

    m_btnClose.setFixedSize(48, 48);
    m_btnMax.setFixedSize(48, 48);
    m_btnMin.setFixedSize(48, 48);

    m_btnClose.setStyleSheet("QPushButton{border-image:url(:/images/top/CloseNormal.png);}"
                             "QPushButton::hover{border-image:url(:/images/top/CloseFocus.png);}"
                             "QPushButton::pressed{border-image:url(:/images/top/CloseFocus.png);}");
    m_btnMax.setStyleSheet("QPushButton{border-image:url(:/images/top/MaxNormal.png);}"
                           "QPushButton::hover{border-image:url(:/images/top/MaxFocus.png);}"
                           "QPushButton::pressed{border-image:url(:/images/top/MaxFocus.png);}");
    m_btnMin.setStyleSheet("QPushButton{border-image:url(:/images/top/MinNormal.png);}"
                           "QPushButton::hover{border-image:url(:/images/top/MinFocus.png);}"
                           "QPushButton::pressed{border-image:url(:/images/top/MinFocus.png);}");

    m_btnClose.setToolTip(QStringLiteral("关闭"));
    m_btnMax.setToolTip(QStringLiteral("最大化"));
    m_btnMin.setToolTip(QStringLiteral("最小化"));

    QFrame* pVLine = new QFrame();
    pVLine->setFrameShape(QFrame::VLine);
    pVLine->setFrameShadow(QFrame::Plain);
    pVLine->setFixedSize(1, this->height());
    pVLine->setStyleSheet("QFrame{background-color: gray; border:none}");

    pCaptionLayout->addWidget(&m_lblTitle);
    pCaptionLayout->addSpacerItem(new QSpacerItem(20, this->height(), QSizePolicy::Expanding, QSizePolicy::Fixed));
    pCaptionLayout->addWidget(pVLine);
    pCaptionLayout->setSpacing(4);
    pCaptionLayout->addWidget(&m_btnMin);
    pCaptionLayout->addWidget(&m_btnMax);
    pCaptionLayout->addWidget(&m_btnClose);
    pCaptionLayout->setSpacing(0);
    pCaptionLayout->setContentsMargins(16, 0, 0, 0);

    QFrame* pHLine = new QFrame();
    pHLine->setFrameShape(QFrame::HLine);
    pHLine->setFrameShadow(QFrame::Plain);
    pHLine->setFixedHeight(1);
    pHLine->setMinimumWidth(this->width());
    pHLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pHLine->setStyleSheet("QFrame{background-color: gray; border:none}");

    pMainLayout->addLayout(pCaptionLayout);
    pMainLayout->addWidget(pHLine);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void TopWidget::initConnection()
{
    connect(&m_btnClose, SIGNAL(clicked(bool)), this, SLOT(slot_onClick()));
    connect(&m_btnMax, SIGNAL(clicked(bool)), this, SLOT(slot_onClick()));
    connect(&m_btnMin, SIGNAL(clicked(bool)), this, SLOT(slot_onClick()));
}

bool TopWidget::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
//    qDebug() << "event type: " << event->type();
    switch (event->type()) {
    case QEvent::WindowStateChange:
    case QEvent::Resize:
    {
        updateMaximize();
        return true;
    }
    default:
        break;
    }

    return QWidget::eventFilter(watched, event);
}

void TopWidget::slot_onClick()
{
    QPushButton *pButton = qobject_cast<QPushButton*>(this->sender());
    QWidget* pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton->objectName() == "btnClose")
        {
//            pWindow->close();
            pWindow->hide();
        }
        else if (pButton->objectName() == "btnMax")
        {
            if (pWindow->isMaximized())
            {
                pWindow->showNormal();
            }
            else
            {
                pWindow->showMaximized();
                pWindow->setGeometry(-4, -4, pWindow->width() + 8, pWindow->height() + 8);
            }
        }
        else if (pButton->objectName() == "btnMin")
        {
            pWindow->showMinimized();
        }
    }
}

void TopWidget::updateMaximize()
{
    QWidget* pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_btnMax.setToolTip(QStringLiteral("还原"));
//            m_btnMax->setProperty("maximizeProperty", "restore");
            m_btnMax.setStyleSheet("QPushButton{border-image:url(:/images/top/StoreNormal.png);}"
                                   "QPushButton::hover{border-image:url(:/images/top/StoreFouce.png);}"
                                   "QPushButton::pressed{border-image:url(:/images/top/StoreFouce.png);}");
        }
        else
        {
//            m_btnMax->setProperty("maximizeProperty", "maximize");
            m_btnMax.setToolTip(QStringLiteral("最大化"));
            m_btnMax.setStyleSheet("QPushButton{border-image:url(:/images/top/MaxNormal.png);}"
                                   "QPushButton::hover{border-image:url(:/images/top/MaxFocus.png);}"
                                   "QPushButton::pressed{border-image:url(:/images/top/MaxFocus.png);}");
        }

//        m_btnMax->setStyle(QApplication::style());
    }
}
