#ifndef CONFERENCESTACKWIDGET_H
#define CONFERENCESTACKWIDGET_H

#include "common/qtheaders.h"
#include "absFiles/myPushButton.h"

class ConferenceStackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConferenceStackWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initLayout();
    void initConnection();

signals:


public slots:

public:
    MyPushButton *m_pStartConfBtn;
    MyPushButton *m_pBookConfBtn;

private:
    QLabel *m_pLblIcon;
};

#endif // CONFERENCESTACKWIDGET_H
