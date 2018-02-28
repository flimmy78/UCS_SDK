#include "midLeftCallListStackWidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>


MidLeftCallListStackWidget::MidLeftCallListStackWidget(QWidget *parent, int width)
    : MyScrollArea(parent)
    , m_callListWid(this)
{
    setMouseTracking(true);
    setFixedWidth(width);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    initLayout();
    initConnection();
    updateData();
}

void MidLeftCallListStackWidget::updateData()
{
    for (int i = 0; i < 15; i++)
    {
        CallHistoryItem item;
        if (i % 2)
        {
            item.headerPath = ":/images/u57.png";
            item.nickName = QStringLiteral("小美");
            item.callTime = "2018/01/06";
            item.callType = kAudioCall;
        }
        else
        {
            item.headerPath = ":/images/u29.png";
            item.nickName = QStringLiteral("全球唯一云通讯能力提供商");
            item.callTime = "17:50";
            item.callType = kAudioConf;
        }

        m_callListWid.addCallListItem(&item);
    }
    m_callListWid.setCurrentRow(0);
}

void MidLeftCallListStackWidget::initLayout()
{
    qDebug() << "MidLeftCallListStackWidget";
    QHBoxLayout *pMainLayout = new QHBoxLayout;

//    pMainLayout->addWidget(&m_callListView);
    pMainLayout->addWidget(&m_callListWid);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pMainLayout);
}

void MidLeftCallListStackWidget::initConnection()
{

}
