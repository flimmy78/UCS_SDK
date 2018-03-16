#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QObject>
#include <QWidget>
#include "absFiles/myPushButton.h"
#include "BaseWidget.h"

class LeftNavigatorBarWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit LeftNavigatorBarWidget(QWidget *parent = 0);
    ~LeftNavigatorBarWidget();

private:
    void initLayout();
    void initConnection();

protected:


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
};

#endif // LEFTWIDGET_H
