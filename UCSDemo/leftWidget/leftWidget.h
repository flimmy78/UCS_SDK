#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QObject>
#include <QWidget>
#include "absFiles/myPushButton.h"
#include "rest/UCSRestManager.h"

class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(QWidget *parent = 0, int width = 94);
    ~LeftWidget();

private:
    void initLayout();
    void initConnection();

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void slot_changeButtonSelected(int index);
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
