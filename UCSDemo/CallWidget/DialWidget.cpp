#include "DialWidget.h"

DialWidget::DialWidget(QWidget *parent)
    : BaseWidget(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCursor(Qt::ArrowCursor);

    setObjectName("DialWidget");

    initLayout();
    initConnection();
    initMisc();
}

bool DialWidget::eventFilter(QObject *watched, QEvent *event)
{
    return QObject::eventFilter(watched, event);
}

void DialWidget::initLayout()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);

    QLabel *pLabel = new QLabel(this);
    pLabel->setObjectName("lblDialPanel");
    pLabel->setText(QStringLiteral("拨号盘"));

    m_pKeypadWid = new KeyPadWidget(this);
    m_pKeypadWid->setBtnIcon(QIcon(":/images/midright/u165.png"), QSize(25, 25));
    m_pKeypadWid->setBtnStyle("QPushButton{background-image: url(:/images/midright/u398.png); color: white; border:none; font: 12pt;}");
    m_pKeypadWid->setBtnText(QStringLiteral("拨打"));
    m_pKeypadWid->setBtnSize(QSize(143, 43));

    pMainLayout->addStretch(0.1);
    pMainLayout->addWidget(pLabel, 0, Qt::AlignHCenter);
    pMainLayout->addSpacing(16);
    pMainLayout->addWidget(m_pKeypadWid, 0.3, Qt::AlignHCenter);
    pMainLayout->addStretch(0.2);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
}

void DialWidget::initConnection()
{
    connect(m_pKeypadWid, SIGNAL(sigBtnClicked(QString)), this, SLOT(onKeypadClicked(QString)));
}

void DialWidget::initMisc()
{

}

void DialWidget::onKeypadClicked(QString text)
{
    QString txt = QStringLiteral("正在呼叫 ") + text;
    QMessageBox::information(NULL, QStringLiteral("发起呼叫"), txt, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}
