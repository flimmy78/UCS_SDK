#include "UPlusLogin.h"

#include <QRegExpValidator>
#include <QVBoxLayout>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDateTime>
#include <QtConcurrent>
#include "UCSLogger.h"
#include "UCSTcpSDK.h"
#include "CommonHelper.h"
#include "DownloadNetworkManager.h"

UPlusLogin::UPlusLogin(QWidget *parent)
    : QDialog(parent)
    , m_doReLogin(false)
{
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setFixedSize(270, 440);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setObjectName("UPlusLogin");

    m_pRestApi = new UPlusRestApi;

    initLayout();
    initConnections();
    readSettings();

    UCSTcpClient::Instance()->registerEventListener(kUCSLoginEvent, this);
}

UPlusLogin::~UPlusLogin()
{
    UCS_LOG(UCSLogger::kTraceApiCall, this->objectName(),
            "~dtor()");
    UCSTcpClient::Instance()->unRegisterEventListener(kUCSLoginEvent, this);

    if (m_pRestApi)
    {
        delete m_pRestApi;
        m_pRestApi = Q_NULLPTR;
    }
}

void UPlusLogin::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        onBtnLogin();
        break;
    default:
        break;
    }
}

void UPlusLogin::customEvent(QEvent *event)
{
    if (event->type() == kUCSLoginEvent)
    {
        UCSLoginEvent *loginEvt = (UCSLoginEvent*)event;
        if (loginEvt->error() == ErrorCode_NoError)
        {
            UCSTcpClient::Instance()->unRegisterEventListener(kUCSLoginEvent, this);

            this->accept();
        }
        else
        {
            m_pBtnLoginOn->setEnabled(true);
            m_pLoginTip->setText(QString(QStringLiteral("登录失败(%1)")).arg(loginEvt->error()));
        }
    }
}

void UPlusLogin::initLayout()
{
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    QHBoxLayout *pChkLayout = new QHBoxLayout;

    m_titleBar = new MyTitleBar(this);
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setMoveParentWindowFlag(true);
    m_titleBar->setBackgroundColor(33, 157, 201, false);
    m_titleBar->setTitleContentFontSize(11);
    m_titleBar->setTitleContent("U+");
//    m_titleBar->setTitleContentFontSize(12);

    m_pLineUserName = new QLineEdit(this);
    m_pLineUserName->setObjectName("loginUserName");
    m_pLineUserName->setPlaceholderText(QStringLiteral("请输入手机号"));
    m_pLineUserName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pLineUserName->setMaxLength(20);

    QRegExp reg("[0-9]+$");
    QRegExpValidator *pValidator = new QRegExpValidator(this);
    pValidator->setRegExp(reg);
    m_pLineUserName->setValidator(pValidator);

    m_pLinePassword = new QLineEdit(this);
    m_pLinePassword->setObjectName("loginPassword");
    m_pLinePassword->setEchoMode(QLineEdit::Password);
    m_pLinePassword->setPlaceholderText(QStringLiteral("请输入密码"));
    m_pLinePassword->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pLinePassword->setMaxLength(20);

    QRegExp pwdReg("[a-zA-Z0-9]+$");
    QRegExpValidator *pPwdValidator = new QRegExpValidator(this);
    pPwdValidator->setRegExp(pwdReg);
    m_pLinePassword->setValidator(pPwdValidator);

    m_pChkKeepPwd = new QCheckBox(QStringLiteral("记住密码"), this);
    m_pChkOnLine = new QCheckBox(QStringLiteral("测试环境"), this);
    m_pChkKeepPwd->setObjectName("ChkPwd");
    m_pChkOnLine->setObjectName("ChkOnLine");
    pChkLayout->addWidget(m_pChkKeepPwd, 0, Qt::AlignHCenter);
    pChkLayout->addWidget(m_pChkOnLine, 0, Qt::AlignHCenter);
    pChkLayout->setSpacing(16);

    m_pBtnLoginOn = new QPushButton(this);
    m_pBtnLoginOn->setObjectName("btnLogin");
    m_pBtnLoginOn->setText(QStringLiteral("登录"));
    m_pBtnLoginOn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pBtnLoginOn->setFlat(true);

    m_pLoginTip = new QLabel(this);
    m_pLoginTip->setObjectName("lblLoginTip");
    m_pLoginTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    m_pLoginTip->setText("test");

    pMainLayout->addWidget(m_titleBar);
    pMainLayout->addStretch(0.2);
    pMainLayout->addWidget(m_pLineUserName, 0, Qt::AlignHCenter);
    pMainLayout->addSpacing(8);
    pMainLayout->addWidget(m_pLinePassword, 0, Qt::AlignHCenter);
    pMainLayout->addSpacing(8);
    pMainLayout->addLayout(pChkLayout);
    pMainLayout->addStretch(0.3);
    pMainLayout->addWidget(m_pBtnLoginOn, 0, Qt::AlignHCenter);
    pMainLayout->addStretch(0.2);
    pMainLayout->addWidget(m_pLoginTip, 0, Qt::AlignHCenter);
    pMainLayout->addStretch(0.2);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
}

void UPlusLogin::initConnections()
{
    connect(m_titleBar, SIGNAL(signalBtnCloseClicked()), this, SLOT(onBtnClosed()));
    connect(m_titleBar, SIGNAL(signalBtnMinClicked()), this, SLOT(onBtnMin()));
    connect(m_pChkKeepPwd, SIGNAL(clicked()), this, SLOT(onCheckedChanged()));
    connect(m_pChkOnLine, SIGNAL(clicked()), this, SLOT(onCheckedChanged()));
    connect(m_pBtnLoginOn, SIGNAL(clicked()), this, SLOT(onBtnLogin()));

    connect(m_pRestApi, SIGNAL(sigOnLoginReply(QByteArray,int)), this, SLOT(onLoginReply(QByteArray,int)));
    connect(m_pRestApi, SIGNAL(sigOnReLoginReply(QByteArray,int)), this, SLOT(onReLoginReply(QByteArray,int)));

    // test
    connect(m_pRestApi, SIGNAL(sigOnUploadHeaderImgReply(QByteArray,int)), this, SLOT(onUploadHeaderImgReply(QByteArray,int)));
}

void UPlusLogin::readSettings()
{
    QString userId = CommonHelper::readSetting("Login", "userId", "").toString();
    QString pwd = CommonHelper::readUserPwd("pwd");

//    QString name = CommonHelper::readSetting("Login", "name", "").toString();
//    UCS_LOG(UCSLogger::kTraceInfo, this->objectName(),
//            QString("readSettings: userId(%1) pwd(%2) name(%3)")
//                    .arg(userId).arg(pwd).arg(name));

    int chkState = CommonHelper::readSetting("Login", "KeepPwd", 0).toInt();
    m_pChkKeepPwd->setChecked(chkState > 0);

    m_doReLogin = false;
    if (m_pChkKeepPwd->checkState() == Qt::Unchecked)
    {
        pwd.clear();
    }

    if (!pwd.isEmpty())
    {
        m_doReLogin = true;
    }

    m_pLineUserName->setText(userId);
    m_pLinePassword->setText(pwd);

    m_pChkOnLine->setChecked(true);
    UCSTcpClient::Instance()->DoUseOnLineEnv(false);
}

void UPlusLogin::onLoginSuccess()
{
    ///< 登录 Paas >
    QString token = CommonHelper::readSetting("Login", "token", "").toString();
    UCSTcpClient::Instance()->doLogin(token);

    ///< 异步下载个人头像 >
    QString headUrl = CommonHelper::readSetting("Login", "headUrl", "").toString();
    QStringList headList;
    headList.append(headUrl);
    QtConcurrent::mapped(headList, UPlusLogin::downloadHeadImg);
}

void UPlusLogin::uploadHeaderImg()
{
    QString userId = CommonHelper::readSetting("Login", "userId", "").toString();
    QString pwd = CommonHelper::readUserPwd("pwd");
    QFile file(":/Resources/emptyBG.jpg");
    if (!file.open(QIODevice::ReadOnly) || file.size() == 0)
    {
        file.close();
        return;
    }

    QByteArray imgData = file.readAll();
    if (imgData.isEmpty())
    {
        file.close();
        return;
    }

    m_pRestApi->doUploadHeaderImg(userId, pwd, imgData, "jpg");
}

QString UPlusLogin::downloadHeadImg(const QString &headUrl)
{
    QString filepath = HttpDownloadPicture::downloadBlock(headUrl, CommonHelper::userTempPath());
    return filepath;
}

void UPlusLogin::onBtnClosed()
{
    this->close();
}

void UPlusLogin::onBtnMin()
{
    this->showMinimized();
}

void UPlusLogin::onBtnLogin()
{
    QString userId = m_pLineUserName->text().trimmed();
    QString pwd = m_pLinePassword->text().trimmed();

    if (userId.isEmpty() || pwd.isEmpty())
    {
        return;
    }

    if (m_pRestApi)
    {
//        UCS_LOG(UCSLogger::kTraceInfo, this->objectName(),
//                QString("doLogin: userId[%1] pwd[%2]")
//                    .arg(userId).arg(pwd));

        m_pBtnLoginOn->setEnabled(false);

        if (m_doReLogin)
        {
            m_pRestApi->doReLogin(userId, pwd);
        }
        else
        {
            m_pRestApi->doLogin(userId, pwd);
        }

        CommonHelper::saveSetting("Login", "userId", userId);
        CommonHelper::saveUserPwd("pwd", pwd);
        CommonHelper::saveSetting("Login", "KeepPwd", m_pChkKeepPwd->checkState());
    }
}

void UPlusLogin::onCheckedChanged()
{
    QCheckBox *chkBox = qobject_cast<QCheckBox*>(sender());
    if (chkBox->objectName().contains("ChkPwd"))
    {

    }

    if (chkBox->objectName().contains("ChkOnLine"))
    {
        if (chkBox->checkState())
        {
            UCSTcpClient::Instance()->DoUseOnLineEnv(false);
        }
        else
        {
            UCSTcpClient::Instance()->DoUseOnLineEnv(true);
        }
    }
}

void UPlusLogin::onLoginReply(QByteArray replyData, int code)
{
    if (code == HTTP_OK && !replyData.isEmpty())
    {
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(replyData, &jsonError);
        if (jsonError.error == QJsonParseError::NoError)
        {
            int ret = -1;
            QString token;
            QString submitPwd;
            QString name;
            int sex;
            QString headImg;
            QString retMsg;

            if (document.isObject())
            {
                QJsonObject jsonObj = document.object();
                if (jsonObj.contains("ret"))
                {
                    ret = jsonObj["ret"].toInt();
                }

                if (jsonObj.contains("retMsg"))
                {
                    retMsg = jsonObj["retMsg"].toString();
                }

                if (jsonObj.contains("token"))
                {
                    token = jsonObj["token"].toString();
                }

                if (jsonObj.contains("summitpwd"))
                {
                    submitPwd = jsonObj["summitpwd"].toString();
                }

                if (jsonObj.contains("name"))
                {
                    name = jsonObj["name"].toString();
                }

                if (jsonObj.contains("sex"))
                {
                    sex = jsonObj["sex"].toString().toInt();
                }

                if (jsonObj.contains("headimg"))
                {
                    headImg = jsonObj["headimg"].toString();
                }
            }

            if (ret == 0)
            {
                CommonHelper::saveSetting("Login", "name", name);
                CommonHelper::saveSetting("Login", "token", token);
                CommonHelper::saveSetting("Login", "sex", sex);
                CommonHelper::saveUserPwd("summitpwd", submitPwd);
                CommonHelper::saveSetting("Login", "headUrl", headImg);
                CommonHelper::saveSetting("Login", "time", QDateTime::currentSecsSinceEpoch());

                onLoginSuccess();
            }
            else
            {
                m_pLoginTip->setText(retMsg);
            }
        }
        else
        {
            m_pLoginTip->setText(QStringLiteral("登录响应格式错误"));
            m_pBtnLoginOn->setEnabled(true);
        }
    }
    else
    {
        UCS_LOG(UCSLogger::kTraceError, this->objectName(),
                QString("login request failed(%1).").arg(code));

        m_pLoginTip->setText(QString(QStringLiteral("登录请求响应失败(%1)")).arg(code));
        m_pBtnLoginOn->setEnabled(true);
    }
}

void UPlusLogin::onReLoginReply(QByteArray replyData, int code)
{
    if (code == HTTP_OK && !replyData.isEmpty())
    {
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(replyData, &jsonError);
        if (jsonError.error == QJsonParseError::NoError)
        {
            int ret = -1;
            QString retMsg;

            if (document.isObject())
            {
                QJsonObject jsonObj = document.object();
                if (jsonObj.contains("ret"))
                {
                    ret = jsonObj["ret"].toInt();
                }

                if (jsonObj.contains("retMsg"))
                {
                    retMsg = jsonObj["retMsg"].toString();
                }

                if (ret == 0)
                {
                    onLoginSuccess();
                }
                else if (ret == 3)
                {
                    m_pLinePassword->clear();
                    m_pLinePassword->setFocus();
                }
            }

            if (ret != 0)
            {
                m_pLoginTip->setText(retMsg);
            }
        }
        else
        {
            m_pLoginTip->setText(QStringLiteral("登录响应格式错误"));
            m_pBtnLoginOn->setEnabled(true);
        }
    }
    else
    {
        UCS_LOG(UCSLogger::kTraceError, this->objectName(),
                QString("login request failed(%1).").arg(code));

        m_pLoginTip->setText(QStringLiteral("登录请求响应失败"));
        m_pBtnLoginOn->setEnabled(true);
    }
}

void UPlusLogin::onUploadHeaderImgReply(QByteArray replyData, int code)
{
    if (code != HTTP_OK && replyData.isEmpty())
    {
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(replyData, &jsonError);
    if (jsonError.error == QJsonParseError::NoError)
    {
        int ret = -1;
        QString headimgurl;
        QString retMsg;

        if (document.isObject())
        {
            QJsonObject jsonObj = document.object();
            if (jsonObj.contains("ret"))
            {
                ret = jsonObj["ret"].toInt();
            }

            if (jsonObj.contains("retMsg"))
            {
                retMsg = jsonObj["retMsg"].toString();
            }

            if (jsonObj.contains("headimgurl"))
            {
                headimgurl = jsonObj["headimgurl"].toString();
            }

            if (ret == 0)
            {
                CommonHelper::saveSetting("Login", "headUrl", headimgurl);
            }
        }
    }
}
