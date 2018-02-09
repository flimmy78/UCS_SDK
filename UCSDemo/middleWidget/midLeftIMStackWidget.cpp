#include "midLeftIMStackWidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include "middleWidget/middleLeftWidget.h"

MidLeftIMStackWidget::MidLeftIMStackWidget(QWidget *parent)
    : MyScrollArea(parent)
    , m_imListWid(this)
{
    setMouseTracking(true);
    setFixedWidth(250);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    initLayout();
    initConnection();
    updateData();
}

void MidLeftIMStackWidget::updateData()
{
    for (int i = 0; i < 12; i++)
    {
        ImMsgItem item;
        if (i % 2)
        {
            item.headerPath = ":/images/u57.png";
            item.nickName = QStringLiteral("小美");
            item.msgTime = "2018/01/08";
            item.msgData = QStringLiteral("8点30分准时龙门例会");
        }
        else
        {
            item.headerPath = ":/images/u29.png";
            item.nickName = QStringLiteral("全球唯一云通讯能力提供商");
            item.msgTime = "10:04";
            item.msgData = QStringLiteral("刚才他说的话好逗哦，笑死俺老牛了。。。");
        }
        m_imItems.append(item);
        m_imListWid.addListItem(&item);
    }
    m_imListWid.setCurrentRow(1);
}

void MidLeftIMStackWidget::initLayout()
{
    qDebug() << "MidLeftIMStackWidget";
    QHBoxLayout *pMainLayout = new QHBoxLayout;

    pMainLayout->addWidget(&m_imListWid);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void MidLeftIMStackWidget::initConnection()
{
    connect(&m_imListWid, SIGNAL(sig_itemClicked(int)), this, SLOT(slot_itemClicked(int)));
}

QString MidLeftIMStackWidget::currentName()
{
    return m_imItems.at(m_imListWid.currentRow()).nickName;
}

void MidLeftIMStackWidget::slot_itemClicked(int row)
{
    QString name = m_imItems.at(row).nickName;
    qDebug() << "MidLeftIMStackWidget slot_itemClicked " << name;

    emit sig_itemClicked(name);
}
