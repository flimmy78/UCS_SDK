#include "DialWidget.h"

QString btnNormalStyle = "QPushButton{color:rgb(68,68,68); border:none;}"
                         "QPushButton:hover{color:rgb(40,143,231); border:none;}";
QString btnSelectedStyle = "QPushButton{border-image: url(:/images/midright/btn_mask.png); color:rgb(40,143,231); border:none;}";

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
    QHBoxLayout *pTopLayout = new QHBoxLayout;
    m_pStackedLayout = new QStackedLayout;

    m_btnArray[0].setText(QStringLiteral("拨号盘"));
    m_btnArray[1].setText(QStringLiteral("电话会议"));

    m_btnArray[0].setFixedSize(80, 40);
    m_btnArray[1].setFixedSize(80, 40);

    m_btnArray[0].setStyleSheet(btnNormalStyle);
    m_btnArray[1].setStyleSheet(btnNormalStyle);

    pTopLayout->addStretch();
    pTopLayout->addWidget(&m_btnArray[0], 0, Qt::AlignHCenter);
    pTopLayout->addWidget(&m_btnArray[1], 0, Qt::AlignHCenter);
    pTopLayout->addStretch();
    pTopLayout->setSpacing(16);

    m_pKeypadWid = new KeyPadWidget(this);
    m_pConfWid = new ConferenceStackWidget(this);

    m_pKeypadWid->setBtnIcon(QIcon(":/images/midright/u165.png"), QSize(25, 25));
    m_pKeypadWid->setBtnStyle("QPushButton{background-image: url(:/images/midright/u398.png); color: white; border:none; font: 12pt;}");
    m_pKeypadWid->setBtnText(QStringLiteral("拨打"));
    m_pKeypadWid->setBtnSize(QSize(143, 43));

    m_pStackedLayout->addWidget(m_pKeypadWid);
    m_pStackedLayout->addWidget(m_pConfWid);

    pMainLayout->addStretch();
    pMainLayout->addLayout(pTopLayout);
    pMainLayout->addSpacerItem(new QSpacerItem(this->width(), 16, QSizePolicy::Expanding, QSizePolicy::Minimum));
    pMainLayout->addLayout(m_pStackedLayout);
    pMainLayout->addStretch();
    pMainLayout->setContentsMargins(0, 0, 0, 0);

//    setLayout(pMainLayout);
}

void DialWidget::initConnection()
{
    for (int i = 0; i < 2; i++)
    {
        m_btnArray[i].setFont(QFont(QStringLiteral("微软雅黑"), 12, QFont::Bold, false));
        m_btnArray[i].setObjectName(QString::number(i));
        connect(&m_btnArray[i], SIGNAL(clicked()), this, SLOT(onSwitchPage()));
    }

    connect(m_pKeypadWid, SIGNAL(sig_Clicked(QString)), this, SLOT(slot_keypadClicked(QString)));
    connect(m_pConfWid->m_pStartConfBtn, SIGNAL(clicked(bool)), this, SLOT(slot_startConference()));
    connect(m_pConfWid->m_pBookConfBtn, SIGNAL(clicked(bool)), this, SLOT(slot_bookConference()));
}

void DialWidget::initMisc()
{
    preIndex = 0;
    m_btnArray[0].setStyleSheet(btnSelectedStyle);
}

void DialWidget::onSwitchPage()
{
    int index = sender()->objectName().toInt();
    m_pStackedLayout->setCurrentIndex(index);

    for (int i = 0; i < 2; i++)
    {
        if (i == index)
        {
            m_btnArray[i].setStyleSheet(btnSelectedStyle);
        }
        else
        {
            m_btnArray[i].setStyleSheet(btnNormalStyle);
        }
    }
}

void DialWidget::slot_curStackChanged(int index)
{
    for (int i = 0; i < 2; i++)
    {
        if (i == index)
        {
            m_btnArray[i].setStyleSheet(btnSelectedStyle);
        }
        else
        {
            m_btnArray[i].setStyleSheet(btnNormalStyle);
        }
    }
}

void DialWidget::slot_keypadClicked(QString text)
{
    qDebug() << "keypad text " << text;
    QString txt = QStringLiteral("正在呼叫 ") + text;
    QMessageBox::information(NULL, QStringLiteral("发起呼叫"), txt, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void DialWidget::slot_startConference()
{
    qDebug("slot_startConference");
    QMessageBox::information(NULL, QStringLiteral("发起会议"), "user info", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void DialWidget::slot_bookConference()
{
    qDebug("slot_bookConference");
    QMessageBox::information(NULL, QStringLiteral("预约会议"), "user info", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}
