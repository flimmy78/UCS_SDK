#include "UCSLogin.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <inc/UCSTcpClient.h>

UCSLogin::UCSLogin(QWidget *parent) : QDialog(parent)
{
    setMouseTracking(true);
    setFixedSize(445, 345);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

//    QPalette palette;
//    palette.setColor(QPalette::Background, QColor(32, 155, 152));
//    setPalette(palette);
//    setAutoFillBackground(true);

    initLayout();
    initConnections();

    m_timerId = 0;
    m_count = 60;
}

UCSLogin::~UCSLogin()
{
    if (m_timerId > 0)
    {
        killTimer(m_timerId);
        m_timerId = 0;
    }
}

void UCSLogin::initLayout()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    QHBoxLayout *pTitleLayout = new QHBoxLayout;
    QHBoxLayout *pMidLayout = new QHBoxLayout;
    QGridLayout *pLoginLayout = new QGridLayout;

    m_pBtnClosed = new MyPushButton(this);
    m_pBtnCodeReq = new MyPushButton(this);
    m_pBtnLoginOn = new MyPushButton(this);
    m_pLblHeaderImg = new QLabel;
    m_pLineUserName = new QLineEdit(this);
    m_pLineVerifyCode = new QLineEdit(this);

    m_pBtnClosed->setToolTip(QStringLiteral("关闭"));
    m_pBtnClosed->setFixedSize(27, 22);
    m_pBtnClosed->setStyleSheet("QPushButton{border-image:url(:/images/login/close.png);}"
                                "QPushButton::hover{border-image:url(:/images/login/closeHover.png);}"
                                "QPushButton::pressed{border-image:url(:/images/login/closePressed.png);}");

    pTitleLayout->addStretch();
    pTitleLayout->addWidget(m_pBtnClosed);
    pTitleLayout->setContentsMargins(0, 4, 4, 4);
    QWidget *titleWid = new QWidget(this);
    titleWid->setStyleSheet("QWidget{background-color:rgb(32, 155, 152);}");
    titleWid->setLayout(pTitleLayout);

    QLabel *lblMovie = new QLabel;
    lblMovie->setFixedHeight(150);
    lblMovie->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    lblMovie->setStyleSheet("QLabel{background-color:rgb(32, 155, 152);}");


    ///< 设置头像 >
    m_pLblHeaderImg->setFixedSize(50, 50);
    m_pLblHeaderImg->setPixmap(QPixmap(":/images/login/u6.png"));
    m_pLblHeaderImg->setScaledContents(true);

    m_pLineUserName->setPlaceholderText(QStringLiteral("手机号/邮箱"));
    m_pLineUserName->setFixedSize(200, 26);
    m_pLineUserName->setStyleSheet("QLineEdit{border: 1px solid gray; border-radius:2px;}"
                                   "QLineEdit::hover{border:1px solid rgb(37, 174, 154); border-radius:2px;}");
    m_pLineUserName->setFont(QFont(QStringLiteral("微软雅黑"), 10));

    m_pLineVerifyCode->setPlaceholderText(QStringLiteral("验证码"));
    m_pLineVerifyCode->setFixedSize(200, 26);
    m_pLineVerifyCode->setStyleSheet("QLineEdit{border: 1px solid gray; border-radius:2px;}"
                                     "QLineEdit::hover{border:1px solid rgb(37, 174, 154); border-radius:2px;}");
    m_pLineVerifyCode->setFont(QFont(QStringLiteral("微软雅黑"), 10));

    m_pBtnCodeReq->setText(QStringLiteral("获取验证码"));
    m_pBtnCodeReq->setFixedWidth(80);
    m_pBtnCodeReq->setObjectName("btnCodeReq");
    m_pBtnCodeReq->setFont(QFont(QStringLiteral("微软雅黑"), 10));

    m_pBtnLoginOn->setObjectName("btnLogin");
    m_pBtnLoginOn->setText(QStringLiteral("登录"));
    m_pBtnLoginOn->setFont(QFont(QStringLiteral("微软雅黑"), 10));
    m_pBtnLoginOn->setFixedHeight(30);
    m_pBtnLoginOn->setStyleSheet("QPushButton{background-color: rgb(32, 155, 152); border-radius:5px;}"
                                 "QPushButton::hover{background-color: rgb(37, 174, 154);}");

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

    pMainLayout->addWidget(titleWid);
    pMainLayout->addWidget(lblMovie);
    pMainLayout->addLayout(pMidLayout);
    pMainLayout->addStretch();

    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void UCSLogin::initConnections()
{
    connect(m_pBtnClosed, SIGNAL(clicked(bool)), SLOT(slot_onClosed()));
    connect(m_pBtnCodeReq, SIGNAL(clicked(bool)), SLOT(slot_onCodeReqClicked()));
    connect(m_pBtnLoginOn, SIGNAL(clicked(bool)), SLOT(slot_onLoginClicked()));

    connect(&m_restManager, SIGNAL(sig_onAuthSuccessed(int)), SLOT(slot_onAuthSuccess(int)));
    connect(&m_restManager, SIGNAL(sig_onLoginSuccessed(QString,QString)), SLOT(slot_onLoginSuccess(QString,QString)));
}

void UCSLogin::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId)
    {
        QString text = QString("%1 s").arg(--m_count);
        m_pBtnCodeReq->setText(text);
        if (m_count <= 0)
        {
            killTimer(m_timerId);
            m_pBtnCodeReq->setEnabled(true);
            m_pBtnCodeReq->setText(QStringLiteral("获取验证码"));
        }
    }
}

void UCSLogin::slot_onClosed()
{
    this->close();
}

void UCSLogin::slot_onCodeReqClicked()
{
    m_count = 60;

    QString text = QString("%1 s").arg(m_count);
    m_pBtnCodeReq->setEnabled(false);
    m_pBtnCodeReq->setText(text);
    m_timerId = startTimer(1 * 1000);

    QString username = m_pLineUserName->text();
    m_restManager.getAuthCode(username);
}

void UCSLogin::slot_onLoginClicked()
{
    qDebug() << "username: " << m_pLineUserName->text();
    qDebug() << "code: " << m_pLineVerifyCode->text();
    m_pBtnLoginOn->setEnabled(false);
    m_pBtnLoginOn->setText(QStringLiteral("登录中..."));

    m_restManager.login(m_pLineUserName->text(),
                        m_pLineVerifyCode->text());
}

void UCSLogin::slot_onAuthSuccess(int expired)
{
    qDebug() << "expired : " << expired;
//    if (m_timerId)
//    {
//        killTimer(m_timerId);
//        m_pBtnCodeReq->setEnabled(true);
//        m_pBtnCodeReq->setText(QStringLiteral("获取验证码"));
//    }
}

void UCSLogin::slot_onLoginSuccess(QString token, QString nickname)
{
    if (m_timerId)
    {
        killTimer(m_timerId);
        m_pBtnCodeReq->setEnabled(true);
        m_pBtnCodeReq->setText(QStringLiteral("获取验证码"));
    }

    qDebug() << "token: " << token;
    qDebug() << "nickname: " << nickname;

    UCSTcpClient::Instance()->doLogin(token);

    this->accept();
}

