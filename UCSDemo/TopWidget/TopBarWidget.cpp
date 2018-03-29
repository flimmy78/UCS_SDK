#include "TopBarWidget.h"
#include <QHBoxLayout>
#include <QRegExpValidator>
#include <QString>
#include <QDebug>
#include <QMenu>
#include "Interface/UCSLogger.h"
#include "DBCenter.h"
#include "CommonHelper.h"

TopBarWidget::TopBarWidget(QWidget *parent)
    : BaseWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMouseTracking(true);

    setObjectName("TopWidget");

    installEventFilter(this);

    initLayout();
    initConnection();
}

void TopBarWidget::setTitle(QString title)
{
    m_lblTitle.setText(title);
}

void TopBarWidget::updateTopBar()
{
    QString userId = CommonHelper::readSetting(kSettingLoginUserId).toString();
    ContactEntity contact;
    DBCenter::contactMgr()->getContact(userId, contact);
    if (!contact.headPath.isEmpty())
    {
        QSize size = m_pBtnHeader->iconSize();
        QPixmap pixmap = QPixmap(contact.headPath).scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        m_pBtnHeader->setIcon(QIcon(pixmap));
    }
}

void TopBarWidget::initLayout()
{
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);

    /////////////////////////////////////////////////////////
    /* Title */
    m_lblTitle.adjustSize();
    m_lblTitle.setFixedHeight(26);
    m_lblTitle.setFont(QFont("微软雅黑", 13));

    ////////////////////////////////////////////////////////
    /* minimun, maximun, close */
    m_pBtnMin = new MyPushButton(this);
    m_pBtnRestore = new MyPushButton(this);
    m_pBtnMax = new MyPushButton(this);
    m_pBtnClose = new MyPushButton(this);
    m_pBtnGroup = new MyPushButton(this);
    m_pBtnConf = new MyPushButton(this);
    m_pBtnHeader = new QPushButton(this);
    m_pBtnDropDown = new MyPushButton(this);

    m_pBtnClose->setObjectName("btnClose");
    m_pBtnMax->setObjectName("btnMax");
    m_pBtnMin->setObjectName("btnMin");
    m_pBtnRestore->setObjectName("btnRestore");
    m_pBtnGroup->setObjectName("topBtnGroup");
    m_pBtnConf->setObjectName("topBtnConf");
    m_pBtnHeader->setObjectName("topBtnHeader");
    m_pBtnDropDown->setObjectName("topBtnDrop");

    m_pBtnClose->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnMax->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnMin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnRestore->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnGroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnConf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnHeader->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnDropDown->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_pBtnClose->setToolTip(QStringLiteral("关闭"));
    m_pBtnMax->setToolTip(QStringLiteral("最大化"));
    m_pBtnMin->setToolTip(QStringLiteral("最小化"));
    m_pBtnRestore->setToolTip(QStringLiteral("还原"));
    m_pBtnGroup->setToolTip(QStringLiteral("发起群聊"));
    m_pBtnConf->setToolTip(QStringLiteral("发起会议"));

    m_pBtnHeader->setIconSize(QSize(34, 34));
    m_pBtnRestore->setVisible(false);

    pMainLayout->addStretch();
    pMainLayout->addWidget(m_pBtnGroup, 0, Qt::AlignVCenter);
    pMainLayout->addSpacing(8);
    pMainLayout->addWidget(m_pBtnConf, 0, Qt::AlignVCenter);
    pMainLayout->addSpacing(8);
    pMainLayout->addWidget(m_pBtnHeader, 0, Qt::AlignVCenter);
    pMainLayout->addWidget(m_pBtnDropDown, 0, Qt::AlignVCenter);
    pMainLayout->addSpacing(34);
    pMainLayout->addWidget(m_pBtnMin, 0, Qt::AlignVCenter);
    pMainLayout->addWidget(m_pBtnRestore, 0, Qt::AlignVCenter);
    pMainLayout->addWidget(m_pBtnMax, 0, Qt::AlignVCenter);
    pMainLayout->addWidget(m_pBtnClose, 0, Qt::AlignVCenter);
    pMainLayout->setSpacing(1);
    pMainLayout->setContentsMargins(0, 8, 8, 8);
}

void TopBarWidget::initConnection()
{
    connect(m_pBtnMin, SIGNAL(clicked()), this, SLOT(onBtnMinClicked()));
    connect(m_pBtnRestore, SIGNAL(clicked()), this, SLOT(onBtnRestoreClicked()));
    connect(m_pBtnMax, SIGNAL(clicked()), this, SLOT(onBtnMaxClicked()));
    connect(m_pBtnClose, SIGNAL(clicked()), this, SLOT(onBtnCloseClicked()));
    connect(m_pBtnGroup, SIGNAL(clicked(bool)), this, SLOT(onBtnGroupClicked()));
    connect(m_pBtnConf, SIGNAL(clicked(bool)), this, SLOT(onBtnConfClicked()));
    connect(m_pBtnHeader, SIGNAL(clicked(bool)), this,SLOT(onBtnHeaderClicked()));
    connect(m_pBtnDropDown, SIGNAL(clicked(bool)), this, SLOT(onBtnDropDownClicked()));
}

void TopBarWidget::onBtnMinClicked()
{
    QWidget* pWindow = this->window();
    pWindow->showMinimized();
}

void TopBarWidget::onBtnRestoreClicked()
{
    m_pBtnRestore->setVisible(false);
    m_pBtnMax->setVisible(true);

    QWidget* pWindow = this->window();
    pWindow->showNormal();
}

void TopBarWidget::onBtnMaxClicked()
{
    m_pBtnMax->setVisible(false);
    m_pBtnRestore->setVisible(true);

    QWidget* pWindow = this->window();
    pWindow->showMaximized();
    pWindow->setGeometry(-4, -4, pWindow->width() + 8, pWindow->height() + 8);
}

void TopBarWidget::onBtnCloseClicked()
{
    QWidget* pWindow = this->window();
    pWindow->hide();
}

void TopBarWidget::onBtnGroupClicked()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onBtnGroupClicked"));
}

void TopBarWidget::onBtnConfClicked()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onBtnConfClicked"));
}

void TopBarWidget::onBtnHeaderClicked()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onBtnHeaderClicked"));
}

void TopBarWidget::onBtnDropDownClicked()
{
    QMenu *pDropMenu = new QMenu(this);
    pDropMenu->setObjectName("topbarMenu");

    QAction *pActModPhone = new QAction(QStringLiteral("修改手机号码"));
    QAction *pActModPwd = new QAction(QStringLiteral("修改密码"));

    connect(pActModPhone, SIGNAL(triggered(bool)), this, SLOT(onActionModPhone()));
    connect(pActModPwd, SIGNAL(triggered(bool)), this, SLOT(onActionModPwd()));

    pDropMenu->addAction(pActModPhone);
    pDropMenu->addAction(pActModPwd);

    QPoint btnPos = mapToGlobal(m_pBtnDropDown->pos());
    int x = btnPos.x() - 116; ///< 116 is menu width >
    int y = btnPos.y() + m_pBtnDropDown->height() + 8;
    pDropMenu->exec(QPoint(x,y));
}

void TopBarWidget::onActionModPhone()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onActionModPhone"));
}

void TopBarWidget::onActionModPwd()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            QString("onActionModPwd"));
}
