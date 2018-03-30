#include "ContactCard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "DBCenter.h"
#include "CommonHelper.h"

ContactCard::ContactCard(QWidget *parent)
    : BaseWidget(parent)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setCursor(Qt::ArrowCursor);

//    setFixedSize(418, 242);
    setObjectName("ContactCard");

    initLayout();
    initConnections();
}

void ContactCard::initLayout()
{
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    QVBoxLayout *pRightLayout = new QVBoxLayout;
    QHBoxLayout *pNameLayout = new QHBoxLayout;
    QHBoxLayout *pBtnBox = new QHBoxLayout;

    m_pLblImage = new QLabel(this);
    m_pLblName = new MyLabel(this);
    m_pLblPhone = new MyLabel(this);
    m_pLblSection = new MyLabel(this);
    m_pBtnDirect = new MyPushButton(this);
    m_pBtnAudio = new MyPushButton(this);
    m_pBtnVideo = new MyPushButton(this);
    m_pBtnConf = new MyPushButton(this);
    m_pBtnIM = new MyPushButton(this);

    m_pLblImage->setObjectName("lblCardImage");
    m_pLblName->setObjectName("lblCardName");
    m_pLblPhone->setObjectName("lblCardPhone");
    m_pLblSection->setObjectName("lblCardSection");
    m_pBtnDirect->setObjectName("btnCardDirect");
    m_pBtnAudio->setObjectName("btnCardAudio");
    m_pBtnVideo->setObjectName("btnCardVideo");
    m_pBtnConf->setObjectName("btnCardConf");
    m_pBtnIM->setObjectName("btnCardIM");

    m_pLblImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pLblName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLblPhone->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLblSection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    m_pBtnDirect->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnAudio->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnVideo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnConf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnIM->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

//    m_pLblImage->setFixedSize(152, 242);
    m_pLblSection->setWordWrap(true);
    m_pLblSection->adjustSize();
//    m_pBtnDirect->setFixedSize(30, 30);
//    m_pBtnAudio->setFixedSize(30, 30);
//    m_pBtnVideo->setFixedSize(30, 30);
//    m_pBtnConf->setFixedSize(30, 30);
//    m_pBtnIM->setFixedSize(30, 30);

    m_pBtnAudio->setToolTip(QStringLiteral("网络电话"));
    m_pBtnDirect->setToolTip(QStringLiteral("高清电话"));
    m_pBtnConf->setToolTip(QStringLiteral("电话会议"));
    m_pBtnVideo->setToolTip(QStringLiteral("视频聊天"));
    m_pBtnIM->setToolTip(QStringLiteral("消息"));

    pNameLayout->addWidget(m_pLblName);
    pNameLayout->addSpacing(8);
    pNameLayout->addWidget(m_pLblPhone);
    pNameLayout->setSpacing(0);

    pBtnBox->addWidget(m_pBtnAudio);
    pBtnBox->addWidget(m_pBtnVideo);
    pBtnBox->addWidget(m_pBtnDirect);
    pBtnBox->addWidget(m_pBtnIM);
    pBtnBox->addWidget(m_pBtnConf);
    pBtnBox->setSpacing(16);

    pRightLayout->addLayout(pNameLayout);
    pRightLayout->addSpacing(8);
    pRightLayout->addWidget(m_pLblSection);
    pRightLayout->addStretch();
    pRightLayout->addLayout(pBtnBox);
    pRightLayout->setContentsMargins(24, 40, 24, 36);

    pMainLayout->addWidget(m_pLblImage);
    pMainLayout->addLayout(pRightLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
}

void ContactCard::initConnections()
{
    connect(m_pBtnDirect, SIGNAL(clicked(bool)), this, SLOT(onBtnDirectClicked(bool)));
    connect(m_pBtnAudio, SIGNAL(clicked(bool)), this, SLOT(onBtnAudioClicked(bool)));
    connect(m_pBtnVideo, SIGNAL(clicked(bool)), this, SLOT(onBtnVideoClicked(bool)));
    connect(m_pBtnIM, SIGNAL(clicked(bool)), this, SLOT(onBtnIMClicked(bool)));
    connect(m_pBtnConf, SIGNAL(clicked(bool)), this, SLOT(onBtnConfClicked(bool)));
}

void ContactCard::onShowContact(ContactItem contact)
{
    QPixmap pixmap = QPixmap(contact.headPath).scaled(m_pLblImage->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_pLblImage->setPixmap(pixmap);

    m_pLblName->setText(contact.userName);
    m_pLblPhone->setText(contact.userId);
    m_pLblSection->setText(contact.parentName);

    m_contact = contact;
}

void ContactCard::onBtnDirectClicked(bool)
{

}

void ContactCard::onBtnAudioClicked(bool)
{

}

void ContactCard::onBtnVideoClicked(bool)
{

}

void ContactCard::onBtnConfClicked(bool)
{

}

void ContactCard::onBtnIMClicked(bool)
{
    emit sigOpenImPage(m_contact);
}
