#include "keypadWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QRegExp>
#include <QRegExpValidator>
#include <QDebug>

KeyPadWidget::KeyPadWidget(QWidget *parent)
    : QWidget(parent)
    , m_customBtn(this)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    initLayout();
    initConnection();
}

void KeyPadWidget::setBtnStyle(QString style)
{
    m_customBtn.setStyleSheet(style);
}

void KeyPadWidget::setBtnIcon(QIcon icon, QSize iconSize)
{
    m_customBtn.setIcon(icon);
    m_customBtn.setIconSize(iconSize);
}

void KeyPadWidget::setBtnText(QString text)
{
    m_customBtn.setText(text);
}

void KeyPadWidget::setBtnSize(QSize size)
{
    m_customBtn.setMinimumSize(size);
}

void KeyPadWidget::initLayout()
{
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    QVBoxLayout *pVLayout = new QVBoxLayout;
    QGridLayout *pGridLayout = new QGridLayout;

    m_pDisplayLine = new QLineEdit(this);
    m_pDisplayLine->setMaxLength(15);
//    m_pDisplayLine->setFrame(false);
    m_pDisplayLine->setAlignment(Qt::AlignRight);
    m_pDisplayLine->setObjectName("dialNumber");
    m_pDisplayLine->setPlaceholderText(QStringLiteral("请输入您要拨打的号码"));

    QRegExp exp("[0-9*#]+$");
    QRegExpValidator *pValidator = new QRegExpValidator(this);
    pValidator->setRegExp(exp);
    m_pDisplayLine->setValidator(pValidator);

    MyToolButton *pTBtnBackSpace = new MyToolButton(this);
    pTBtnBackSpace->setObjectName("backspaceBtn");
    pTBtnBackSpace->setFixedSize(25, 25);
    pTBtnBackSpace->setIcon(QIcon(":/images/midright/u139.png"));
    pTBtnBackSpace->setIconSize(pTBtnBackSpace->size());
    pTBtnBackSpace->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pTBtnBackSpace->setToolTip(QStringLiteral("删除"));
    connect(pTBtnBackSpace, SIGNAL(clicked(bool)), this, SLOT(onBtnBackspaceClicked()));

    QMargins margins = m_pDisplayLine->textMargins();
    m_pDisplayLine->setTextMargins(margins.left() + 5,
                                   margins.top(),
                                   pTBtnBackSpace->width() + 5,
                                   margins.bottom());
    QHBoxLayout *pLineLayout = new QHBoxLayout;
    pLineLayout->addStretch();
    pLineLayout->addWidget(pTBtnBackSpace, 0, Qt::AlignVCenter);
    pLineLayout->setSpacing(0);
    pLineLayout->setContentsMargins(0, 0, 5, 5);
    m_pDisplayLine->setLayout(pLineLayout);

    for (int i = 0; i < NumDigitButtons; i++)
    {
        m_pDigitBtn[i] = new MyToolButton(QString::number(i));
        m_pDigitBtn[i]->setObjectName("digitBtn");
    }
    m_pStarBtn = new MyToolButton("*");
    m_pPoundBtn = new MyToolButton("#");
    m_pStarBtn->setObjectName("digitBtn");
    m_pPoundBtn->setObjectName("digitBtn");

    for (int i = 1; i < NumDigitButtons; i++)
    {
        int row = (i - 1) / 3;
        int column = (i - 1) % 3;
        pGridLayout->addWidget(m_pDigitBtn[i], row, column);
    }

    pGridLayout->addWidget(m_pStarBtn, 3, 0);
    pGridLayout->addWidget(m_pDigitBtn[0], 3, 1);
    pGridLayout->addWidget(m_pPoundBtn, 3, 2);
//    pGridLayout->setSizeConstraint(QLayout::SetFixedSize);
    pGridLayout->setHorizontalSpacing(16);
    pGridLayout->setVerticalSpacing(8);

    pVLayout->addStretch();
    pVLayout->addWidget(m_pDisplayLine);
    pVLayout->addSpacing(16);
    pVLayout->addLayout(pGridLayout);
    pVLayout->addSpacing(32);
    pVLayout->addWidget(&m_customBtn, 0, Qt::AlignHCenter);
    pVLayout->setSpacing(8);
    pVLayout->addStretch();
    pVLayout->setContentsMargins(0, 0, 0, 0);

//    pMainLayout->addStretch(0.1);
    pMainLayout->addLayout(pVLayout);
//    pMainLayout->addStretch(0.1);
}

void KeyPadWidget::initConnection()
{
    for (int i = 0; i < NumDigitButtons; i++)
    {
        connect(m_pDigitBtn[i], SIGNAL(clicked()), this, SLOT(onBtnDigitClicked()));
    }

    connect(m_pStarBtn, SIGNAL(clicked()), this, SLOT(onBtnDigitClicked()));
    connect(m_pPoundBtn, SIGNAL(clicked()), this, SLOT(onBtnDigitClicked()));

//    connect(pTBtnBackSpace, SIGNAL(clicked()), this, SLOT(onBtnBackspaceClicked()));
    connect(&m_customBtn, SIGNAL(clicked()), this, SLOT(onBtnClicked()));
}

void KeyPadWidget::onBtnDigitClicked()
{
    MyToolButton *clickedButton = qobject_cast<MyToolButton*>(sender());
    m_pDisplayLine->setText(m_pDisplayLine->text() + clickedButton->text());
}

void KeyPadWidget::onBtnBackspaceClicked()
{
    QString text = m_pDisplayLine->text();
    text.chop(1);
    m_pDisplayLine->setText(text);
}

void KeyPadWidget::onBtnClicked()
{
    emit sigBtnClicked(m_pDisplayLine->text().simplified());
}
