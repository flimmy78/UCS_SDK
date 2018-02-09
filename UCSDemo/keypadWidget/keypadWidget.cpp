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
    QHBoxLayout *pMainLayout = new QHBoxLayout;
    QVBoxLayout *pVLayout = new QVBoxLayout;
    QHBoxLayout *pTopLayout = new QHBoxLayout;
    QGridLayout *pGridLayout = new QGridLayout;

    m_pDisplayLine = new QLineEdit(this);
    m_pDisplayLine->setMaxLength(15);
    m_pDisplayLine->setFrame(false);
    m_pDisplayLine->setAlignment(Qt::AlignRight);

    QFont font = m_pDisplayLine->font();
    font.setPointSize(font.pointSize() + 8);
    font.setBold(true);
    m_pDisplayLine->setFont(font);
    m_pDisplayLine->setPlaceholderText(QStringLiteral("请输入您要拨打的号码"));

    QRegExp exp("[0-9*#]+$");
    QRegExpValidator *pValidator = new QRegExpValidator(this);
    pValidator->setRegExp(exp);
    m_pDisplayLine->setValidator(pValidator);

    m_pBackSpaceBtn = new MyToolButton("", this);
    m_pBackSpaceBtn->setIcon(QIcon(":/images/midright/u139.png"));
    m_pBackSpaceBtn->setIconSize(QSize(25, 25));
    m_pBackSpaceBtn->setFixedSize(50, 30);
//    m_pBackSpaceBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_pBackSpaceBtn->setToolTip(QStringLiteral("删除"));
    pTopLayout->addWidget(m_pDisplayLine);
    pTopLayout->addWidget(m_pBackSpaceBtn);
    pTopLayout->setSpacing(8);
    pTopLayout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < NumDigitButtons; i++)
    {
        m_pDigitBtn[i] = new MyToolButton(QString::number(i));
//        m_pDigitBtn[i]->setMinimumSize(170, 68);
    }
    m_pStarBtn = new MyToolButton("*");
    m_pPoundBtn = new MyToolButton("#");
//    m_pStarBtn->setMinimumSize(170, 68);
//    m_pPoundBtn->setMinimumSize(170, 68);

    for (int i = 1; i < NumDigitButtons; i++)
    {
        int row = (i - 1) / 3;
        int column = (i - 1) % 3;
        pGridLayout->addWidget(m_pDigitBtn[i], row, column);
    }

    pGridLayout->addWidget(m_pStarBtn, 3, 0);
    pGridLayout->addWidget(m_pDigitBtn[0], 3, 1);
    pGridLayout->addWidget(m_pPoundBtn, 3, 2);
    pGridLayout->setSizeConstraint(QLayout::SetFixedSize);
    pGridLayout->setSpacing(20);

//    m_customBtn.setText(QStringLiteral("拨打"));
//    m_customBtn.setFixedSize(143, 43);
//    m_customBtn.setIcon(QIcon(":/images/midright/u165.png"));
//    m_customBtn.setIconSize(QSize(25, 25));
//    m_customBtn.setStyleSheet("QPushButton{background-image: url(:/images/midright/u398.png); color: white; border:none; font: 12pt;}");

    QFrame* pHLine = new QFrame();
    pHLine->setFrameShape(QFrame::HLine);
    pHLine->setFrameShadow(QFrame::Plain);
    pHLine->setFixedHeight(1);
    pHLine->setMinimumWidth(this->width());
    pHLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pHLine->setStyleSheet("QFrame{background-color: gray; border:none}");

    pVLayout->addStretch();
    pVLayout->addLayout(pTopLayout);
    pVLayout->addWidget(pHLine);
    pVLayout->addLayout(pGridLayout);
    pVLayout->addWidget(&m_customBtn, 0, Qt::AlignHCenter);
    pVLayout->setSpacing(8);
    pVLayout->addStretch();
    pVLayout->setContentsMargins(0, 0, 0, 0);

    pMainLayout->addStretch(0.2);
    pMainLayout->addLayout(pVLayout);
    pMainLayout->addStretch(0.2);

    setLayout(pMainLayout);
}

void KeyPadWidget::initConnection()
{
    for (int i = 0; i < NumDigitButtons; i++)
    {
        connect(m_pDigitBtn[i], SIGNAL(clicked()), this, SLOT(slot_digitClicked()));
    }

    connect(m_pStarBtn, SIGNAL(clicked()), this, SLOT(slot_digitClicked()));
    connect(m_pPoundBtn, SIGNAL(clicked()), this, SLOT(slot_digitClicked()));

    connect(m_pBackSpaceBtn, SIGNAL(clicked()), this, SLOT(slot_backspaceClicked()));
    connect(&m_customBtn, SIGNAL(clicked()), this, SLOT(slot_BtnClicked()));
}

void KeyPadWidget::slot_digitClicked()
{
    MyToolButton *clickedButton = qobject_cast<MyToolButton*>(sender());
    m_pDisplayLine->setText(m_pDisplayLine->text() + clickedButton->text());
}

void KeyPadWidget::slot_backspaceClicked()
{
    QString text = m_pDisplayLine->text();
    text.chop(1);
    m_pDisplayLine->setText(text);
}

void KeyPadWidget::slot_BtnClicked()
{
    emit sig_Clicked(m_pDisplayLine->text().simplified());
}
