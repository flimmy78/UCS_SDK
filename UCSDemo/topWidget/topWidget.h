#ifndef TOPWIDGETS_H
#define TOPWIDGETS_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include "absFiles/myPushButton.h"
#include "absFiles/myLabel.h"
#include "BaseWidget.h"

class TopWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit TopWidget(QWidget *parent = 0);

    void setTitle(QString title);

private:
    void initLayout();
    void initConnection();
    void updateMaximize();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

signals:

public slots:
    void slot_onClick();

private:
    MyPushButton m_btnClose;
    MyPushButton m_btnMin;
    MyPushButton m_btnMax;
    MyLabel m_lblTitle;
};

#endif // TOPWIDGETS_H
