#ifndef ADDRESSDETAILSTACKWIDGET_H
#define ADDRESSDETAILSTACKWIDGET_H

#include "common/qtheaders.h"
#include "absFiles/myPushButton.h"
#include "absFiles/myToolButton.h"
#include "absFiles/myLabel.h"
#include "BaseWidget.h"

class MidRightContactStackWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit MidRightContactStackWidget(QWidget *parent = 0);

    void updateData();

protected:


private:
    void initLayout();
    void initConnection();

signals:

public slots:

private:
    QLabel m_lblHeader;
    MyLabel m_lblName;
    QLabel m_lblSex;

    QTableWidget m_tableWid;

    MyToolButton m_btnDial;
    MyToolButton m_btnConfCall;
    MyToolButton m_btnIM;
};

#endif // ADDRESSDETAILSTACKWIDGET_H
