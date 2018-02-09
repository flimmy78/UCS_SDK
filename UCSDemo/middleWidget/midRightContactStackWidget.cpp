#include "midRightContactStackWidget.h"
#include <QHeaderView>

MidRightContactStackWidget::MidRightContactStackWidget(QWidget *parent)
    : QWidget(parent)
    , m_lblHeader(this)
    , m_lblName(this)
    , m_lblSex(this)
    , m_tableWid(this)
    , m_btnDial(this)
    , m_btnConfCall(this)
    , m_btnIM(this)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCursor(Qt::ArrowCursor);

    initLayout();
    initConnection();
    updateData();
}

void MidRightContactStackWidget::updateData()
{
    m_tableWid.clear();

    for (int row = 0; row < 4; row++)
    {
        m_tableWid.insertRow(row);
//        m_tableWid.setRowHeight(row, 52);
        for (int col = 0; col < m_tableWid.columnCount(); col++)
        {
            if (col % 2 == 0)
            {
//                QTableWidgetItem *item = new QTableWidgetItem();
//                item->setFlags(item->flags() & (~Qt::ItemIsEditable));
//                item->setTextColor(Qt::gray);
//                item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                QLabel *lbl = new QLabel(this);
                lbl->setStyleSheet("QLabel{color:gray;}");
                lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                lbl->setContentsMargins(8, 0, 8, 0);
                m_tableWid.setCellWidget(row, col, lbl);
            }
            else
            {
                MyLabel *lblDept = new MyLabel(this);
                m_tableWid.setCellWidget(row, col, lblDept);
            }
        }
    }

#if 0
    m_tableWid.item(0, 0)->setText(QStringLiteral("部门"));
    m_tableWid.item(0, 1)->setText(QStringLiteral("技术部"));
    m_tableWid.item(0, 2)->setText(QStringLiteral("职位"));
    m_tableWid.item(0, 3)->setText(QStringLiteral("软件工程师"));

    m_tableWid.item(1, 0)->setText(QStringLiteral("座机"));
    m_tableWid.item(1, 1)->setText(QStringLiteral("0755-5555555"));
    m_tableWid.item(1, 2)->setText(QStringLiteral("邮箱"));
    m_tableWid.item(1, 3)->setText(QStringLiteral("11111@ucpaas.com"));

    m_tableWid.item(2, 0)->setText(QStringLiteral("工作手机"));
    m_tableWid.item(2, 1)->setText(QStringLiteral("13666666666"));
    m_tableWid.item(2, 2)->setText(QStringLiteral("手机"));
    m_tableWid.item(2, 3)->setText(QStringLiteral("18111111111"));
#else
    ((QLabel*)m_tableWid.cellWidget(0, 0))->setText(QStringLiteral("部门"));
    ((QLabel*)m_tableWid.cellWidget(0, 1))->setText(QStringLiteral("技术部"));
    ((QLabel*)m_tableWid.cellWidget(0, 2))->setText(QStringLiteral("职位"));
    ((QLabel*)m_tableWid.cellWidget(0, 3))->setText(QStringLiteral("软件工程师"));

    ((QLabel*)m_tableWid.cellWidget(1, 0))->setText(QStringLiteral("座机"));
    ((QLabel*)m_tableWid.cellWidget(1, 1))->setText(QStringLiteral("0755-5555555"));
    ((QLabel*)m_tableWid.cellWidget(1, 2))->setText(QStringLiteral("邮箱"));
    ((QLabel*)m_tableWid.cellWidget(1, 3))->setText(QStringLiteral("11111@ucpaas.com"));

    ((QLabel*)m_tableWid.cellWidget(2, 0))->setText(QStringLiteral("工作手机"));
    ((QLabel*)m_tableWid.cellWidget(2, 1))->setText(QStringLiteral("13666666666"));
    ((QLabel*)m_tableWid.cellWidget(2, 2))->setText(QStringLiteral("手机"));
    ((QLabel*)m_tableWid.cellWidget(2, 3))->setText(QStringLiteral("18111111111"));
#endif
}

void MidRightContactStackWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

void MidRightContactStackWidget::initLayout()
{
    qDebug() << "MidRightContactStackWidget";
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    QHBoxLayout *pNameLayout = new QHBoxLayout;
    QHBoxLayout *pOptLayout = new QHBoxLayout;

    m_lblHeader.setPixmap(QPixmap(":/images/midright/u3901.png"));
    m_lblHeader.setFixedSize(100, 100);

    m_lblName.setText(QStringLiteral("小薇"));
    m_lblName.setFont(QFont(QStringLiteral("微软雅黑"), 13, QFont::Normal, false));
    m_lblName.adjustSize();

    /* 旋转45° */
    QMatrix mrix;
    mrix.rotate(45);
    m_lblSex.setPixmap(QPixmap(":/images/midright/u3903.png").transformed(mrix, Qt::SmoothTransformation));

    pNameLayout->addStretch();
    pNameLayout->addWidget(&m_lblName);
    pNameLayout->addWidget(&m_lblSex);
    pNameLayout->setSpacing(8);
    pNameLayout->addStretch();

    m_tableWid.setFrameShadow(QFrame::Plain);
    m_tableWid.setColumnCount(4);
    m_tableWid.setFrameShape(QFrame::NoFrame);
    m_tableWid.setShowGrid(false);
    m_tableWid.setFocusPolicy(Qt::NoFocus);
    m_tableWid.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tableWid.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tableWid.horizontalHeader()->hide();
    m_tableWid.verticalHeader()->hide();
    m_tableWid.horizontalHeader()->resizeSection(0, 90);
    m_tableWid.horizontalHeader()->resizeSection(1, 220);
    m_tableWid.horizontalHeader()->resizeSection(2, 90);
    m_tableWid.horizontalHeader()->resizeSection(3, 220);
    m_tableWid.horizontalHeader()->setStretchLastSection(true);
    m_tableWid.horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableWid.verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    m_tableWid.verticalHeader()->setStretchLastSection(true);
    m_tableWid.verticalHeader()->setDefaultSectionSize(52);
    m_tableWid.setMinimumWidth(625);
    m_tableWid.setEditTriggers(QTableWidget::NoEditTriggers);
    m_tableWid.setSelectionMode(QAbstractItemView::NoSelection);
    m_tableWid.setFont(QFont(QStringLiteral("微软雅黑"), 11, QFont::Normal, false));


    m_btnDial.setStyleSheet("QToolButton{border-image: url(:/images/midright/u3921.png); border:none; color: rgb(32, 155, 152);}");
    m_btnConfCall.setStyleSheet("QToolButton{border-image: url(:/images/midright/u3921.png); border:none; color: rgb(32, 155, 152);}");
    m_btnIM.setStyleSheet("QToolButton{border-image: url(:/images/midright/u3921.png); border:none; color: rgb(32, 155, 152);}");

    m_btnDial.setIcon(QIcon(":/images/midright/u3923.png"));
    m_btnDial.setIconSize(QSize(32, 32));
    m_btnDial.setText(QStringLiteral("拨打"));
    m_btnDial.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_btnConfCall.setIcon(QIcon(":/images/midright/u3941.png"));
    m_btnConfCall.setIconSize(QSize(32, 32));
    m_btnConfCall.setText(QStringLiteral("电话会议"));
    m_btnConfCall.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_btnIM.setIcon(QIcon(":/images/midright/u3931.png"));
    m_btnIM.setIconSize(QSize(32, 32));
    m_btnIM.setText(QStringLiteral("消息"));
    m_btnIM.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_btnDial.setFixedSize(80, 80);
    m_btnConfCall.setFixedSize(80, 80);
    m_btnIM.setFixedSize(80, 80);

    pOptLayout->addStretch();
    pOptLayout->addWidget(&m_btnDial);
    pOptLayout->addWidget(&m_btnConfCall);
    pOptLayout->addWidget(&m_btnIM);
    pOptLayout->addStretch();
    pOptLayout->setSpacing(16);

    pMainLayout->addStretch();
    pMainLayout->addWidget(&m_lblHeader, 0, Qt::AlignHCenter);
    pMainLayout->addLayout(pNameLayout);
    pMainLayout->addWidget(&m_tableWid, 0, Qt::AlignHCenter);
    pMainLayout->addLayout(pOptLayout);
    pMainLayout->setSpacing(16);
    pMainLayout->addStretch();
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void MidRightContactStackWidget::initConnection()
{

}
