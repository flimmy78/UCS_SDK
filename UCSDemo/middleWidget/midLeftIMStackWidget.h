#ifndef IMESSAGESTACKWIDGET_H
#define IMESSAGESTACKWIDGET_H

#include <QObject>
#include <QWidget>
#include <QList>
#include "absFiles/myScrollArea.h"
#include "MidLeftConversationListWidget.h"

class MiddleLeftWidget;

class MidLeftIMStackWidget : public MyScrollArea
{
    Q_OBJECT

public:
    explicit MidLeftIMStackWidget(QWidget *parent = 0, int width = 250);

    void setMidLeftWidget(MiddleLeftWidget *p) { m_pMidLeftWid = p; }
    void updateData();

private:
    void initLayout();
    void initConnection();
    QString currentName();

signals:
    void sig_itemClicked(QString name);

public slots:
    void slot_itemClicked(int row);

private:
    MiddleLeftWidget *m_pMidLeftWid;
    MidLeftConversationListWidget m_imListWid;

    QList<IMConversationItem> m_imItems;
};

#endif // IMESSAGESTACKWIDGET_H
