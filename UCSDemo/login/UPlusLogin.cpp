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
#include "DBCenter.h"

UPlusLogin::UPlusLogin(QWidget *parent)
    : QDialog(parent)
    , m_doReLogin(false)
    , m_bPwdShowing(false)
{
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setFixedSize(300, 440);
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
    m_titleBar->setBackgroundColor(33, 157, 201, true);
    m_titleBar->setTitleContent(QStringLiteral("登录"));

    m_pLineUserId = new QLineEdit(this);
    m_pLineUserId->setObjectName("loginUserName");
    m_pLineUserId->setPlaceholderText(QStringLiteral("请输入手机号"));
    m_pLineUserId->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pLineUserId->setMaxLength(20);

    QRegExp reg("[0-9]+$");
    QRegExpValidator *pValidator = new QRegExpValidator(this);
    pValidator->setRegExp(reg);
    m_pLineUserId->setValidator(pValidator);

#if 0
    m_pLinePassword = new QLineEdit(this);
    m_pLinePassword->setObjectName("loginPassword");
    m_pLinePassword->setEchoMode(QLineEdit::Password);
    m_pLinePassword->setPlaceholderText(QStringLiteral("请输入密码"));
    m_pLinePassword->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pLinePassword->setMaxLength(20);

//    QRegExp pwdReg("[a-zA-Z0-9]+$");
//    QRegExpValidator *pPwdValidator = new QRegExpValidator(this);
//    pPwdValidator->setRegExp(pwdReg);
//    m_pLinePassword->setValidator(pPwdValidator);

    m_pBtnPwdShow = new PwdShowButton(this);
    QHBoxLayout *pTxtLayout = new QHBoxLayout;
    QMargins margins = m_pLinePassword->textMargins();
    m_pLinePassword->setTextMargins(margins.left() + 5,
                   margins.top(),
                   m_pBtnPwdShow->width() + 5,
                   margins.bottom());

    pTxtLayout->addStretch();
    pTxtLayout->addWidget(m_pBtnPwdShow);
    pTxtLayout->setSpacing(0);
    pTxtLayout->setContentsMargins(0, 0, 5, 0);
    m_pLinePassword->setLayout(pTxtLayout);
#else
    m_pLinePassword = new PwdLineEdit(this);
    m_pLinePassword->setObjectName("loginPassword");
    m_pLinePassword->setPlaceholderText(QStringLiteral("请输入密码"));
    m_pLinePassword->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pLinePassword->setMaxLength(20);
#endif

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

    QLabel *pLogo = new QLabel;
    pLogo->setFixedSize(94, 94);
    pLogo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pLogo->setObjectName("lblLoginLogo");

    pMainLayout->addWidget(m_titleBar);
    pMainLayout->addStretch();
    pMainLayout->addWidget(pLogo, 0, Qt::AlignHCenter);
    pMainLayout->addStretch();
    pMainLayout->addWidget(m_pLineUserId, 0, Qt::AlignHCenter);
    pMainLayout->addWidget(m_pLinePassword, 0, Qt::AlignHCenter);
    pMainLayout->addSpacing(22);
    pMainLayout->addLayout(pChkLayout);
    pMainLayout->addSpacing(22);
    pMainLayout->addWidget(m_pBtnLoginOn, 0, Qt::AlignHCenter);
    pMainLayout->addSpacing(8);
    pMainLayout->addWidget(m_pLoginTip, 0, Qt::AlignHCenter);
    pMainLayout->addSpacing(10);
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
    connect(m_pLineUserId, SIGNAL(textChanged(QString)), this, SLOT(onLineUserIdTextChanged(QString)));

    connect(m_pRestApi, SIGNAL(sigOnLoginReply(QByteArray,int)), this, SLOT(onLoginReply(QByteArray,int)));
    connect(m_pRestApi, SIGNAL(sigOnReLoginReply(QByteArray,int)), this, SLOT(onReLoginReply(QByteArray,int)));
    // test
    connect(m_pRestApi, SIGNAL(sigOnUploadHeaderImgReply(QByteArray,int)), this, SLOT(onUploadHeaderImgReply(QByteArray,int)));
}

void UPlusLogin::readSettings()
{
    bool chkState = CommonHelper::readSetting(kSettingLoginEnv).toBool();
    m_pChkOnLine->setChecked(chkState);
    if (chkState)
    {
        UCSTcpClient::Instance()->DoUseOnLineEnv(false);
        m_pRestApi->doSwitchEnv(false);
    }
    else
    {
        UCSTcpClient::Instance()->DoUseOnLineEnv(true);
        m_pRestApi->doSwitchEnv(true);
    }

//    QString userId = CommonHelper::readSetting(kSettingLoginUserId).toString();
//    QString pwd = CommonHelper::readSetting(kSettingLoginPwd).toString();

//    chkState = CommonHelper::readSetting(kSettingLoginKeepPwd).toBool();

    LoginEntity entity;
    LoginEntityList loginList;
    DBCenter::loginMgr()->getAllLoginInfo(loginList);
    if (loginList.empty())
    {
        return;
    }

    entity = loginList.at(0);

    QString pwd = CommonHelper::decryptPwd(entity.userPwd);
    m_pChkKeepPwd->setChecked(entity.isKeepPwd);
    m_doReLogin = false;
    if (m_pChkKeepPwd->checkState() == Qt::Unchecked)
    {
        pwd.clear();
    }

    if (!pwd.isEmpty())
    {
        m_doReLogin = true;
    }

    m_pLineUserId->setText(entity.userId);
    m_pLinePassword->setText(pwd);
}

void UPlusLogin::onLoginSuccess()
{
    QString userId = m_pLineUserId->text().trimmed();
    LoginEntity entity;
    DBCenter::loginMgr()->getLoginInfo(userId, entity);

    ///< 登录 Paas >
//    QString token = CommonHelper::readSetting(kSettingLoginToken).toString();
    if (entity.token.size())
    {
        UCSTcpClient::Instance()->doLogin(entity.token);
    }

    ///< 异步下载个人头像 >
    QStringList userIdList;
    userIdList.append(entity.userId);
    QtConcurrent::mapped(userIdList, UPlusLogin::downloadHeadImg);
}

void UPlusLogin::uploadHeaderImg()
{
    QString userId = CommonHelper::readSetting(kSettingLoginUserId).toString();
    QString pwd = CommonHelper::readSetting(kSettingLoginPwd).toString();
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

QString UPlusLogin::downloadHeadImg(const QString &userId)
{
    LoginEntity entity;
    DBCenter::loginMgr()->getLoginInfo(userId, entity);
    QString filepath = HttpDownloadPicture::downloadBlock(entity.headUrl, CommonHelper::userTempPath());

    MapConditions condition;
    MapValues values;
    condition.insert("userId", entity.userId);
    values.insert("headPath", filepath);
    DBCenter::loginMgr()->updateLoginInfo(condition, values);
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
    QString userId = m_pLineUserId->text().trimmed();
    QString pwd = m_pLinePassword->text().trimmed();

    if (userId.isEmpty() || pwd.isEmpty())
    {
        return;
    }

    if (m_pRestApi)
    {
        m_pBtnLoginOn->setEnabled(false);

        if (m_doReLogin)
        {
            m_pRestApi->doReLogin(userId, pwd);
        }
        else
        {
            m_pRestApi->doLogin(userId, pwd);
        }

        CommonHelper::saveSetting(kSettingLoginUserId, userId);
//        CommonHelper::saveSetting(kSettingLoginPwd, pwd);
//        CommonHelper::saveSetting(kSettingLoginKeepPwd, m_pChkKeepPwd->isChecked());
        CommonHelper::saveSetting(kSettingLoginEnv, m_pChkOnLine->isChecked());
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
        m_doReLogin = false;

        if (chkBox->checkState())
        {
            m_pRestApi->doSwitchEnv(false);
            UCSTcpClient::Instance()->DoUseOnLineEnv(false);            
        }
        else
        {
            m_pRestApi->doSwitchEnv(true);
            UCSTcpClient::Instance()->DoUseOnLineEnv(true);
        }
    }
}

void UPlusLogin::onLineUserIdTextChanged(QString userId)
{
    Q_UNUSED(userId);

    m_doReLogin = false;
    m_pLinePassword->clear();
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
                CommonHelper::saveSetting(kSettingLoginUserName, name);
//                CommonHelper::saveSetting(kSettingLoginToken, token);
//                CommonHelper::saveSetting(kSettingSummitPwd, submitPwd);
//                CommonHelper::saveSetting(kSettingLoginHeadUrl, headImg);
//                CommonHelper::saveSetting(kSettingLoginTime, QDateTime::currentSecsSinceEpoch());

                LoginEntity entity;
                entity.userId = m_pLineUserId->text().trimmed();
                entity.userPwd = CommonHelper::encryptPwd(m_pLinePassword->text().trimmed());
                entity.userName = name;
                entity.summitPwd = CommonHelper::encryptPwd(submitPwd);
                entity.token = token;
                entity.headUrl = headImg;
                entity.userSex = sex;
                entity.isKeepPwd = m_pChkKeepPwd->isChecked();
                DBCenter::loginMgr()->addLoginInfo(entity);

                onLoginSuccess();
            }
            else
            {
                m_pLoginTip->setText(retMsg);
                m_pBtnLoginOn->setEnabled(true);
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
                    ///< update login time >
                    QString userId = m_pLineUserId->text().trimmed();
                    QString time = QString::number(QDateTime::currentSecsSinceEpoch());
                    MapConditions condition;
                    MapValues values;
                    condition.insert("userId", userId);
                    values.insert("time", time);
                    DBCenter::loginMgr()->updateLoginInfo(condition, values);

                    onLoginSuccess();
                }
                else
                {
                    m_doReLogin = false;
                    m_pLoginTip->setText(retMsg);
                    m_pLinePassword->clear();
                    m_pLinePassword->setFocus();
                    m_pBtnLoginOn->setEnabled(true);
                }
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
                CommonHelper::saveSetting(kSettingLoginHeadUrl, headimgurl);
            }
        }
    }
}
