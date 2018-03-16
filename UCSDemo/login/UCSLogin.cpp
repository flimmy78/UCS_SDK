#include "UCSLogin.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include "Interface/UCSTcpClient.h"
#include <QMovie>
#include <QPainter>
#include <util.h>

UCSLogin::UCSLogin(QWidget *parent)
    : BaseDialog(parent)
    , m_timerId(0)
    , m_countDown(60)
    , m_isLoggin(false)
{
    setMouseTracking(true);
    setFixedSize(445, 345);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAttribute(Qt::WA_TranslucentBackground);

    initLayout();
    initConnections();
    loadStyleSheet(":/Resources/LoginWindow/LoginWindow.qss");

    UCSTcpClient::Instance()->registerEventListener(kUCSLoginEvent, this);
}

UCSLogin::~UCSLogin()
{
    qDebug() << "UCSLogin ~dtor()";

    if (m_timerId > 0)
    {
        killTimer(m_timerId);
        m_timerId = 0;
    }

    UCSTcpClient::Instance()->unRegisterEventListener(kUCSLoginEvent, this);
}

void UCSLogin::initLayout()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    QHBoxLayout *pMidLayout = new QHBoxLayout;
    QGridLayout *pLoginLayout = new QGridLayout;

    m_pBtnCodeReq = new MyPushButton(this);
    m_pBtnLoginOn = new MyPushButton(this);
    m_pLblHeaderImg = new QLabel;
    m_pLineUserName = new QLineEdit(this);
    m_pLineVerifyCode = new QLineEdit(this);

    m_titleBar = new MyTitleBar(this);
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setMoveParentWindowFlag(true);
    m_titleBar->setBackgroundColor(32, 155, 152, false);
    m_titleBar->setTitleContentFontSize(11);
//    m_titleBar->setTitleIcon(":/resources/MyTitleBar/head_0.png");
//    m_titleBar->setTitleContent(QStringLiteral("登录"));

    QLabel *lblMovie = new QLabel(this);
//    QMovie *movie = new QMovie;
//    movie->setFileName(":/resources/MyTitleBar/back.gif");
//    lblMovie->setMovie(movie);
//    movie->start();

    lblMovie->setFixedHeight(150);
    lblMovie->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    lblMovie->setStyleSheet("QLabel{background-color:rgb(32, 155, 152);}");

    ///< 设置头像 >
    m_pLblHeaderImg->setFixedSize(50, 50);
    m_pLblHeaderImg->setPixmap(QPixmap(":/Resources/LoginWindow/headImage/u6.png"));
    m_pLblHeaderImg->setScaledContents(true);
    m_pLblHeaderImg->setObjectName("userHeader");

    m_pLineUserName->setPlaceholderText(QStringLiteral("手机号/邮箱"));
    m_pLineUserName->setFixedSize(200, 26);

    m_pLineVerifyCode->setPlaceholderText(QStringLiteral("验证码"));
    m_pLineVerifyCode->setFixedSize(200, 26);

    m_pBtnCodeReq->setText(QStringLiteral("获取验证码"));
    m_pBtnCodeReq->setFixedWidth(80);
    m_pBtnCodeReq->setObjectName("btnCodeReq");

    m_pBtnLoginOn->setObjectName("btnLogin");
    m_pBtnLoginOn->setText(QStringLiteral("登录"));
    m_pBtnLoginOn->setFixedHeight(30);

    ///< 头像 第0行 第0列，占3行1列 >
    pLoginLayout->addWidget(m_pLblHeaderImg, 0, 0, 2, 1);

    ///< 用户名 第0行 第1列，占1行2列 >
    pLoginLayout->addWidget(m_pLineUserName, 0, 1, 1, 2);

    ///< 验证码 第1行 第1列，占1行2列 >
    pLoginLayout->addWidget(m_pLineVerifyCode, 1, 1, 1, 2);
    pLoginLayout->addWidget(m_pBtnCodeReq, 1, 4);

    ///< 登录按钮 第2行 第1列， 占1行2列 >
    pLoginLayout->addWidget(m_pBtnLoginOn, 2, 1, 1, 2);

    ///< 设置垂直和水平间距 >
    pLoginLayout->setHorizontalSpacing(16);
    pLoginLayout->setVerticalSpacing(4);

    ///< 设置外间距 >
    pLoginLayout->setContentsMargins(10, 10, 10, 10);

    pMidLayout->addStretch();
    pMidLayout->addLayout(pLoginLayout);
    pMidLayout->addStretch();

    pMainLayout->addWidget(m_titleBar);
    pMainLayout->addWidget(lblMovie);
    pMainLayout->addLayout(pMidLayout);
    pMainLayout->addStretch();

    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void UCSLogin::initConnections()
{
    connect(m_titleBar, SIGNAL(signalBtnCloseClicked()), this, SLOT(slot_onBtnClose()));
    connect(m_titleBar, SIGNAL(signalBtnMinClicked()), this, SLOT(slot_onBtnMin()));

    connect(m_pBtnCodeReq, SIGNAL(clicked(bool)), SLOT(slot_onCodeReqClicked()));
    connect(m_pBtnLoginOn, SIGNAL(clicked(bool)), SLOT(slot_onLoginClicked()));

    connect(&m_restApi, SIGNAL(sig_onAuthSuccessed(int)), SLOT(slot_onAuthSuccess(int)));
    connect(&m_restApi, SIGNAL(sig_onLoginSuccessed(QString,QString)), SLOT(slot_onLoginSuccess(QString,QString)));
    connect(&m_restApi, SIGNAL(sig_onFailed(UCSRestError)), SLOT(slot_onRestFailed(UCSRestError)));
}

void UCSLogin::onLoginFailed()
{
    m_pBtnLoginOn->setEnabled(true);
    m_pBtnLoginOn->setText(QStringLiteral("登录"));

    m_pBtnCodeReq->setEnabled(true);
    m_pBtnCodeReq->setText(QStringLiteral("获取验证码"));
}

void UCSLogin::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId)
    {
        QString text = QString("%1 s").arg(--m_countDown);
        m_pBtnCodeReq->setText(text);
        if (m_countDown <= 0)
        {
            killTimer(m_timerId);
            onLoginFailed();
            if (m_isLoggin)
            {
                UCSTcpClient::Instance()->doLogout();
            }
        }
    }
}

void UCSLogin::customEvent(QEvent *event)
{
    if (event->type() == kUCSLoginEvent)
    {
        UCSLoginEvent *loginEvt = (UCSLoginEvent*)event;
        if (loginEvt->error() == ErrorCode_NoError)
        {
            qDebug() << "login success. userId = " << loginEvt->userId();

            UCSTcpClient::Instance()->unRegisterEventListener(kUCSLoginEvent, this);

            Util::writeSetting("loginId", loginEvt->userId());
            if (m_timerId)
            {
                killTimer(m_timerId);
            }

            this->accept();
        }
        else
        {
            onLoginFailed();
            qDebug() << "login failed. error = " << loginEvt->error();
        }
    }
    else if (event->type() == kUCSConnectStatusEvent)
    {
        UCSConnectStatusEvent *connEvt = (UCSConnectStatusEvent*)event;
        if (connEvt->status() == UCSConnectionStatus_ConnectFail)
        {
            qDebug() << "connect status changed, status = " << connEvt->status();
        }
    }
}

void UCSLogin::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        slot_onLoginClicked();
        break;
    default:
        break;
    }
}

void UCSLogin::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPainterPath pathBack;
    pathBack.setFillRule(Qt::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(pathBack, QBrush(QColor(236, 241, 245)));
}

void UCSLogin::slot_onBtnClose()
{
    this->close();
}

void UCSLogin::slot_onBtnMin()
{
    this->showMinimized();
}

void UCSLogin::slot_onCodeReqClicked()
{
    QString username = m_pLineUserName->text();
    if (username.isEmpty())
    {
        return;
    }

    m_restApi.getAuthCode(username);
}

void UCSLogin::slot_onLoginClicked()
{
    QString username = m_pLineUserName->text();
    QString code = m_pLineVerifyCode->text();

    if (username.isEmpty() || code.isEmpty() || m_countDown <= 0)
    {
        return;
    }

    m_pBtnLoginOn->setEnabled(false);
    m_pBtnLoginOn->setText(QStringLiteral("登录中..."));

    m_restApi.login(m_pLineUserName->text(),
                        m_pLineVerifyCode->text());
}

void UCSLogin::slot_onAuthSuccess(int expired)
{
    qDebug() << "expired : " << expired;
    if (m_timerId)
    {
        killTimer(m_timerId);
    }

    m_countDown = 60;

    QString text = QString("%1 s").arg(m_countDown);
    m_pBtnCodeReq->setEnabled(false);
    m_pBtnCodeReq->setText(text);

    m_timerId = startTimer(1 * 1000);
}

void UCSLogin::slot_onLoginSuccess(QString token, QString nickname)
{
//    if (m_timerId)
//    {
//        killTimer(m_timerId);
//        m_pBtnCodeReq->setEnabled(true);
//        m_pBtnCodeReq->setText(QStringLiteral("获取验证码"));
//    }

    qDebug() << "token: " << token;
    qDebug() << "nickname: " << nickname;

    UCSTcpClient::Instance()->doLogin(token);
    m_isLoggin = true;
}

void UCSLogin::slot_onRestFailed(UCSRestError error)
{
    if (error != NoError)
    {
        onLoginFailed();
        qDebug() << "onRestFailed error = " << error;
    }
}

