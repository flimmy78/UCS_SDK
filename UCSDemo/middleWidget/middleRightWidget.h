#ifndef MIDDLEWIDGETRIGHT_H
#define MIDDLEWIDGETRIGHT_H

#include <QObject>
#include <QWidget>
#include <QStackedWidget>
#include "topWidget/topWidget.h"
#include "midRightDialStackWidget.h"
#include "midRightContactStackWidget.h"
#include "midRightChatStackWidget.h"

class MiddleRightWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MiddleRightWidget(QWidget *parent = 0, int width = 550);

    MidRightDialStackWidget* dialSWid() { return &m_dialSWid; }
    MidRightContactStackWidget* contactSWid() { return &m_contactSWid; }
    MidRightChatStackWidget* chatSWid() { return &m_chatSWid; }
    TopWidget* topWidget() { return &m_topWid; }



private:
    void initLayout();
    void initConnection();

signals:

public slots:
    void slot_switchStack();
    void slot_setTitle(QString title);

public:
    TopWidget m_topWid;
    QStackedWidget m_stackWid;

private:
    MidRightDialStackWidget m_dialSWid;
    MidRightContactStackWidget m_contactSWid;
    MidRightChatStackWidget m_chatSWid;
};

#endif // MIDDLEWIDGETRIGHT_H
