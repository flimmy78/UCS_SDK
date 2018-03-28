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

void DialWidget::initLayout()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);

    QLabel *pLabel = new QLabel(this);
    pLabel->setObjectName("lblDialPanel");
    pLabel->setText(QStringLiteral("拨号盘"));

    QGridLayout *pGridLayout = new QGridLayout;
    QHBoxLayout *pDigitLayout = new QHBoxLayout;

    QRegExp exp("[0-9*#]+$");
    QRegExpValidator *pValidator = new QRegExpValidator(this);
    pValidator->setRegExp(exp);

    m_pEdtNumber = new QLineEdit(this);
    m_pEdtNumber->setValidator(pValidator);
    m_pEdtNumber->setMaxLength(16);
    m_pEdtNumber->setAlignment(Qt::AlignCenter);
    m_pEdtNumber->setObjectName("callEditNumber");
    m_pEdtNumber->setPlaceholderText(QStringLiteral("请输入您要拨打的号码"));

    QPushButton *pBtnDelNum = new QPushButton(this);
    pBtnDelNum->setObjectName("callBtnDelNum");
    pBtnDelNum->setFixedSize(40, 40);
    pBtnDelNum->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pBtnDelNum->setToolTip(QStringLiteral("删除"));
    pBtnDelNum->setCursor(Qt::ArrowCursor);

    connect(pBtnDelNum, SIGNAL(clicked(bool)), this, SLOT(onBtnDelNumber(bool)));

    QMargins margins = m_pEdtNumber->textMargins();
    m_pEdtNumber->setTextMargins(margins.left() + 5,
                                   margins.top(),
                                   pBtnDelNum->width() + 5,
                                   margins.bottom());
    QHBoxLayout *pLineLayout = new QHBoxLayout;
    pLineLayout->addStretch();
    pLineLayout->addWidget(pBtnDelNum, 0, Qt::AlignVCenter);
    pLineLayout->setSpacing(0);
    pLineLayout->setContentsMargins(0, 0, 5, 5);
    m_pEdtNumber->setLayout(pLineLayout);

    ///< digits >
    for (int i = 0; i < NumDigitButtons; i++)
    {
        m_pBtnDigits[i] = new QPushButton(QString::number(i), this);
        m_pBtnDigits[i]->setObjectName("callBtnDigits");
    }
    m_pBtnDigits[10]->setText("*");
    m_pBtnDigits[11]->setText("#");

    for (int i = 1; i < 10; i++)
    {
        int row = (i - 1) / 3;
        int column = (i - 1) % 3;
        pGridLayout->addWidget(m_pBtnDigits[i], row, column);
    }

    pGridLayout->addWidget(m_pBtnDigits[10], 3, 0);
    pGridLayout->addWidget(m_pBtnDigits[0], 3, 1);
    pGridLayout->addWidget(m_pBtnDigits[11], 3, 2);
    pGridLayout->setHorizontalSpacing(60);
    pGridLayout->setVerticalSpacing(10);

    pDigitLayout->addStretch();
    pDigitLayout->addLayout(pGridLayout);
    pDigitLayout->addStretch();

    m_pBtnDial = new QPushButton(this);
    m_pBtnDial->setObjectName("callBtnDialOut");
    m_pBtnDial->setToolTip(QStringLiteral("拨打"));
    m_pBtnDial->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    pMainLayout->addStretch(0.1);
    pMainLayout->addWidget(pLabel, 0, Qt::AlignHCenter);
    pMainLayout->addSpacing(94);
    pMainLayout->addWidget(m_pEdtNumber, 0, Qt::AlignHCenter);
    pMainLayout->addLayout(pDigitLayout, 0);
    pMainLayout->addWidget(m_pBtnDial, 0, Qt::AlignHCenter);
    pMainLayout->addStretch(0.3);
}

void DialWidget::initConnection()
{
    for (int i = 0; i < NumDigitButtons; i++)
    {
        connect(m_pBtnDigits[i], SIGNAL(clicked(bool)), this, SLOT(onBtnDigitClicked(bool)));
    }

    connect(m_pBtnDial, SIGNAL(clicked(bool)), this, SLOT(onBtnDialOut(bool)));
}

void DialWidget::initMisc()
{

}

void DialWidget::onKeypadClicked(QString text)
{
    QString txt = QStringLiteral("正在呼叫 ") + text;
    QMessageBox::information(NULL, QStringLiteral("发起呼叫"), txt, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void DialWidget::onBtnDigitClicked(bool)
{
    QPushButton *pBtnDigit = qobject_cast<QPushButton*>(sender());
    m_pEdtNumber->setText(m_pEdtNumber->text() + pBtnDigit->text());
}

void DialWidget::onBtnDelNumber(bool)
{
    QString text = m_pEdtNumber->text();
    text.chop(1);
    m_pEdtNumber->setText(text);
}

void DialWidget::onBtnDialOut(bool)
{
    QString txt = QStringLiteral("正在呼叫 ") + m_pEdtNumber->text().trimmed();
    QMessageBox::information(NULL, QStringLiteral("发起呼叫"), txt, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}
