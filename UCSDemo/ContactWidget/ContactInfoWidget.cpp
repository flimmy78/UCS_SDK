#include "ContactInfoWidget.h"
#include <QHeaderView>
#include "UCSLogger.h"

ContactInfoWidget::ContactInfoWidget(QWidget *parent)
    : BaseWidget(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCursor(Qt::ArrowCursor);

    setObjectName("ContactInfoWidget");

    initLayout();
    initConnection();
}

void ContactInfoWidget::initLayout()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    QHBoxLayout *pDetailLayout = new QHBoxLayout;
    QHBoxLayout *pBtnLayout = new QHBoxLayout;

    m_pLblHeaderImg = new MaskLabel(this);
    m_pLblUserName = new MyLabel(this);
    m_pLblUserSex = new QLabel(this);
    m_pLblUserPhone = new MyLabel(this);
    m_pLblSectionName = new MyLabel(this);
    m_pLblPhoneTitle = new MyLabel(this);
    m_pLblSectionTitle = new MyLabel(this);

    m_pBtnDirectAudio = new QToolButton(this);
    m_pBtnFreeAudio = new QToolButton(this);
    m_pBtnFreeVideo = new QToolButton(this);
    m_pBtnConfCall = new QToolButton(this);
    m_pBtnIM = new QToolButton(this);

    m_pLblHeaderImg->setObjectName("contactHead");
    m_pLblUserName->setObjectName("contactUser");
    m_pLblUserSex->setObjectName("contactSex");
    m_pLblUserPhone->setObjectName("contactPhone");
    m_pLblSectionName->setObjectName("contactSection");
    m_pLblSectionTitle->setObjectName("contactSectionTitle");
    m_pLblPhoneTitle->setObjectName("contactPhoneTitle");

    m_pBtnDirectAudio->setObjectName("btnDirectAudio");
    m_pBtnFreeAudio->setObjectName("btnFreeAudio");
    m_pBtnFreeVideo->setObjectName("btnFreeVideo");
    m_pBtnConfCall->setObjectName("btnConfCall");
    m_pBtnIM->setObjectName(("btnIM"));

    m_pLblHeaderImg->setFixedSize(100, 100);
    m_pLblUserPhone->setFixedSize(100, 30);
    m_pLblHeaderImg->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pLblUserName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLblUserSex->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pLblUserPhone->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pLblSectionName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_pBtnDirectAudio->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnFreeAudio->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnFreeVideo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnConfCall->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnIM->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_pLblPhoneTitle->setText(QStringLiteral("手机"));
    m_pLblSectionTitle->setText(QStringLiteral("部门"));
    m_pLblHeaderImg->setPixmap(QPixmap(":/images/midright/u3901.png"));

    pDetailLayout->addStretch();
    pDetailLayout->addWidget(m_pLblPhoneTitle, 0, Qt::AlignHCenter);
    pDetailLayout->addWidget(m_pLblUserPhone, 0, Qt::AlignHCenter);
    pDetailLayout->addSpacing(36);
    pDetailLayout->addWidget(m_pLblSectionTitle, 0, Qt::AlignHCenter);
    pDetailLayout->addWidget(m_pLblSectionName, 0, Qt::AlignHCenter);
    pDetailLayout->setSpacing(8);
    pDetailLayout->addStretch();
    pDetailLayout->setContentsMargins(0, 0, 0, 0);

    m_pBtnDirectAudio->setIcon(QIcon(":/images/midright/u3923.png"));
    m_pBtnDirectAudio->setIconSize(QSize(32, 32));
    m_pBtnDirectAudio->setText(QStringLiteral("高清电话"));
    m_pBtnDirectAudio->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_pBtnFreeAudio->setIcon(QIcon(":/images/midright/u2344.png"));
    m_pBtnFreeAudio->setIconSize(QSize(32, 32));
    m_pBtnFreeAudio->setText(QStringLiteral("网络电话"));
    m_pBtnFreeAudio->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_pBtnFreeVideo->setIcon(QIcon(":/images/midright/u2358.png"));
    m_pBtnFreeVideo->setIconSize(QSize(32, 32));
    m_pBtnFreeVideo->setText(QStringLiteral("视频"));
    m_pBtnFreeVideo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_pBtnConfCall->setIcon(QIcon(":/images/midright/u3941.png"));
    m_pBtnConfCall->setIconSize(QSize(32, 32));
    m_pBtnConfCall->setText(QStringLiteral("电话会议"));
    m_pBtnConfCall->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_pBtnIM->setIcon(QIcon(":/images/midright/u3931.png"));
    m_pBtnIM->setIconSize(QSize(32, 32));
    m_pBtnIM->setText(QStringLiteral("消息"));
    m_pBtnIM->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    pBtnLayout->addStretch();
    pBtnLayout->addWidget(m_pBtnDirectAudio);
//    pBtnLayout->addStretch();
    pBtnLayout->addWidget(m_pBtnFreeAudio);
//    pBtnLayout->addStretch();
    pBtnLayout->addWidget(m_pBtnFreeVideo);
//    pBtnLayout->addStretch();
    pBtnLayout->addWidget(m_pBtnConfCall);
//    pBtnLayout->addStretch();
    pBtnLayout->addWidget(m_pBtnIM);
    pBtnLayout->addStretch();
    pBtnLayout->setSpacing(16);
    pBtnLayout->setContentsMargins(0, 0, 0, 0);

    pMainLayout->addStretch();
    pMainLayout->addWidget(m_pLblHeaderImg, 0, Qt::AlignHCenter);
    pMainLayout->addWidget(m_pLblUserName, 0, Qt::AlignHCenter);
//    pMainLayout->addStretch();
    pMainLayout->addSpacing(16);
    pMainLayout->addLayout(pDetailLayout);
    pMainLayout->addStretch();
    pMainLayout->addLayout(pBtnLayout);
    pMainLayout->addStretch();
    pMainLayout->setContentsMargins(20, 0, 20, 0);
}

void ContactInfoWidget::initConnection()
{
    connect(m_pBtnDirectAudio, SIGNAL(clicked(bool)), this, SLOT(onDirectAudioClick(bool)));
    connect(m_pBtnFreeAudio, SIGNAL(clicked(bool)), this, SLOT(onFreeAudioClick(bool)));
    connect(m_pBtnFreeVideo, SIGNAL(clicked(bool)), this, SLOT(onFreeVideoClick(bool)));
    connect(m_pBtnConfCall, SIGNAL(clicked(bool)), this, SLOT(onConfCallClick(bool)));
    connect(m_pBtnIM, SIGNAL(clicked(bool)), this, SLOT(onImClick(bool)));
}

void ContactInfoWidget::contactShow()
{
    m_pLblUserName->setText(m_contact.userName);

    QSize headSize = m_pLblHeaderImg->size();
    m_pLblHeaderImg->setPixmap(QPixmap(m_contact.headPath).scaled(headSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));


    /* 旋转45° */
//    QMatrix mrix;
//    mrix.rotate(45);
//    m_pLblUserSex->setPixmap(QPixmap(":/images/midright/u3903.png").transformed(mrix, Qt::SmoothTransformation));

    m_pLblUserPhone->setText(m_contact.userId);
    m_pLblSectionName->setText(m_contact.parentName);
}

void ContactInfoWidget::onContactItemClicked(ContactItem contact)
{
    UCS_LOG(UCSLogger::kTraceDebug, this->objectName(),
            QString("onContactItemClicked() contactId: %1 userId: %2 sectionName: %3")
                    .arg(contact.contactId).arg(contact.userId).arg(contact.parentName));

    m_contact = contact;

    contactShow();
}

void ContactInfoWidget::onDirectAudioClick(bool)
{

}

void ContactInfoWidget::onFreeAudioClick(bool)
{

}

void ContactInfoWidget::onFreeVideoClick(bool)
{

}

void ContactInfoWidget::onConfCallClick(bool)
{

}

void ContactInfoWidget::onImClick(bool)
{
    emit sigOpenIMPage(m_contact);
}
