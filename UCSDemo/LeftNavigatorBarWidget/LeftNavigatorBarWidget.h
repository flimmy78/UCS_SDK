﻿#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QObject>
#include <QWidget>
#include "absFiles/myPushButton.h"
#include <UCSRestManager.h>

class LeftNavigatorBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftNavigatorBarWidget(QWidget *parent = 0, int width = 94);
    ~LeftNavigatorBarWidget();

private:
    void initLayout();
    void initConnection();

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void onChangeButtonSelected(int index);
    void slot_personInfoButtonClick();
    void slot_settingButtonClick();

public:
    StackButton *m_pBtn[3];

private:
    MyPushButton *m_pBtnInfo;    
    MyPushButton *m_pBtnSetting;
    UCSRestManager m_restManager;
};

#endif // LEFTWIDGET_H