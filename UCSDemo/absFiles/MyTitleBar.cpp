#include "MyTitleBar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QDebug>
#include <QStyleOption>

#define BUTTON_HEIGHT   30  ///< 按钮高度 >
#define BUTTON_WIDTH    30  ///< 按钮宽度 >
#define TITLE_HEIGHT    30  ///< 标题栏高度 >

MyTitleBar::MyTitleBar(QWidget *parent)
    : BaseWidget(parent)
    , m_colorR(153)
    , m_colorG(153)
    , m_colorB(153)
    , m_isPressed(false)
    , m_windowBorderWidth(0)
    , m_isTransparent(false)
    , m_isMoveParentWindow(true)
{
    this->setFixedHeight(TITLE_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    initLayout();
    initConnections();
    loadStyleSheet(":/Resources/MyTitleBar/MyTitleBar.qss");

    setButtonType(MIN_MAX_BUTTON);
}

MyTitleBar::~MyTitleBar()
{

}

void MyTitleBar::setBackgroundColor(int r, int g, int b, bool isTransparent)
{
    m_colorR = r;
    m_colorG = g;
    m_colorB = b;
    m_isTransparent = isTransparent;

    ///< 重新绘制(调用paintEvent事件) >
    update();
}

void MyTitleBar::setTitleIcon(QString filePath)
{
    QPixmap titleIcon(filePath);
    m_pIcon->setPixmap(titleIcon.scaled(25, 25));
}

void MyTitleBar::setTitleContentFontSize(int fontSize)
{
    ///< 设置标题字体大小 >
    QFont font = m_pTitleContent->font();
    font.setPointSize(fontSize);
    m_pTitleContent->setFont(font);
}

void MyTitleBar::setTitleContent(QString titleContent)
{
    ///< 设置标题内容 >
    m_pTitleContent->setText(titleContent);
    m_titleContent = titleContent;
}

void MyTitleBar::setTitleWidth(int width)
{
//    this->setFixedWidth(width);
    this->setMinimumWidth(width);
}

void MyTitleBar::setButtonType(ButtonType buttonType)
{
    m_buttonType = buttonType;

    switch (buttonType) {
    case MIN_BUTTON:
    {
        m_pBtnRestore->setVisible(false);
        m_pBtnMax->setVisible(false);
    }
        break;

    case MIN_MAX_BUTTON:
    {
        m_pBtnRestore->setVisible(false);
    }
        break;

    case ONLY_CLOSE_BUTTON:
    {
        m_pBtnMin->setVisible(false);
        m_pBtnMax->setVisible(false);
        m_pBtnRestore->setVisible(false);
    }
        break;

    default:
        break;
    }
}

void MyTitleBar::setTitleRoll()
{
    m_titleRollTimer.start(200);
}

void MyTitleBar::setWindowBorderWidth(int borderWidth)
{
    m_windowBorderWidth = borderWidth;
}

void MyTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
    m_restorePos = point;
    m_restoreSize = size;
}

void MyTitleBar::getRestoreInfo(QPoint &point, QSize &size)
{
    point = m_restorePos;
    size = m_restoreSize;
}

void MyTitleBar::setMoveParentWindowFlag(bool isMoveParentWindow)
{
    m_isMoveParentWindow = isMoveParentWindow;
}

void MyTitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (!m_isTransparent)
    {
        QPainter painter(this);
        QPainterPath pathBack;

        pathBack.setFillRule(Qt::WindingFill);
        pathBack.addRoundedRect(QRect(0, 0, this->width(), 3), 3, 3);
        pathBack.addRect(QRect(0, 3, this->width(), this->height() - 3));
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillPath(pathBack, QBrush(QColor(m_colorR, m_colorG, m_colorB)));
    }
}

void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        ///< 只有存在最大化、还原按钮时双击才有效 >
        if (m_buttonType == MIN_MAX_BUTTON)
        {
            ///< 通过最大化按钮状态判定当前窗口是处于最大化还是原始大小 >
            if (m_pBtnMax->isVisible())
            {
                slotOnBtnMaxClicked();
            }
            else
            {
                slotOnBtnRestoreClicked();
            }
        }
    }
}

void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_WindowPos = this->parentWidget()->pos();
        if (m_buttonType == MIN_MAX_BUTTON)
        {
            ///< 窗口最大化时禁止拖动窗口 >
            if (m_pBtnMax->isVisible())
            {
                m_isPressed = true;
                m_startMovePos = event->globalPos();
            }
        }
        else
        {
            m_isPressed = true;
            m_startMovePos = event->globalPos();
        }
    }
}

void MyTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed && m_isMoveParentWindow)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = m_WindowPos + movePoint;

        this->parentWidget()->move(widgetPos);
    }
}

void MyTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_isPressed = false;
}

void MyTitleBar::initLayout()
{
    m_pIcon = new QLabel;
    m_pTitleContent = new QLabel;

    m_pBtnMin = new QPushButton;
    m_pBtnRestore = new QPushButton;
    m_pBtnMax = new QPushButton;
    m_pBtnClose = new QPushButton;

    m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_pBtnMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pBtnRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pBtnMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pBtnClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

    m_pTitleContent->setObjectName("TitleContent");
    m_pBtnMin->setObjectName("BtnMin");
    m_pBtnRestore->setObjectName("BtnRestore");
    m_pBtnMax->setObjectName("BtnMax");
    m_pBtnClose->setObjectName("BtnClose");

    m_pBtnMin->setToolTip(QStringLiteral("最小化"));
    m_pBtnRestore->setToolTip(QStringLiteral("还原"));
    m_pBtnMax->setToolTip(QStringLiteral("最大化"));
    m_pBtnClose->setToolTip(QStringLiteral("关闭"));

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_pIcon);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(m_pTitleContent);

    mainLayout->addWidget(m_pBtnMin);
    mainLayout->addWidget(m_pBtnRestore);
    mainLayout->addWidget(m_pBtnMax);
    mainLayout->addWidget(m_pBtnClose);

    mainLayout->setContentsMargins(5, 0, 0, 0);
    mainLayout->setSpacing(1);
}

void MyTitleBar::initConnections()
{
    connect(m_pBtnMin, SIGNAL(clicked()), this, SLOT(slotOnBtnMinClicked()));
    connect(m_pBtnRestore, SIGNAL(clicked()), this, SLOT(slotOnBtnRestoreClicked()));
    connect(m_pBtnMax, SIGNAL(clicked()), this, SLOT(slotOnBtnMaxClicked()));
    connect(m_pBtnClose, SIGNAL(clicked()), this, SLOT(slotOnBtnCloseClicked()));

    connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(slotOnRollTitle()));
}

void MyTitleBar::slotOnBtnMinClicked()
{
    emit signalBtnMinClicked();
}

void MyTitleBar::slotOnBtnRestoreClicked()
{
    m_pBtnRestore->setVisible(false);
    m_pBtnMax->setVisible(true);
    emit signalBtnRestoreClicked();
}

void MyTitleBar::slotOnBtnMaxClicked()
{
    m_pBtnMax->setVisible(false);
    m_pBtnRestore->setVisible(true);
    emit signalBtnMaxClicked();
}

void MyTitleBar::slotOnBtnCloseClicked()
{
    emit signalBtnCloseClicked();
}

void MyTitleBar::slotOnRollTitle()
{
    static int nPos = 0;
    QString titleContent = m_titleContent;

    ///< 当截取的位置比字符串长时，从头开始 >
    if (nPos > titleContent.length())
    {
        nPos = 0;
    }

    m_pTitleContent->setText(titleContent.mid(nPos));
    nPos++;
}
