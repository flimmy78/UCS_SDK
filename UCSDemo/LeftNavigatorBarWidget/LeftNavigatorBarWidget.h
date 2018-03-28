#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QObject>
#include <QWidget>
#include "myPushButton.h"
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
//    void slot_personInfoButtonClick();
    void onBtnSettingClicked();

private slots:
    void onActionFeedBack();
    void onActionVerUpdate();
    void onActionCommSetting();

public:
    StackButton *m_pBtn[3];

private:
//    MyPushButton *m_pBtnInfo;
    QPushButton *m_pBtnSetting;
};

#endif // LEFTWIDGET_H
