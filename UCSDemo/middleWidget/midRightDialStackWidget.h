#ifndef PHONECALLSTACKWIDGET_H
#define PHONECALLSTACKWIDGET_H

#include <QObject>
#include <QWidget>
#include "common/qtheaders.h"
#include "absFiles/myPushButton.h"
#include "keypadWidget.h"
#include "conferenceStackWidget.h"

class MidRightDialStackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MidRightDialStackWidget(QWidget *parent = 0);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;


private:
    void initLayout();
    void initConnection();
    void initMisc();

signals:

public slots:
    void slot_btnClicked();
    void slot_curStackChanged(int index);
    void slot_keypadClicked(QString text);
    void slot_startConference();
    void slot_bookConference();

private:
    MyPushButton m_btnArray[2];
    QStackedWidget m_stackWid;
    KeyPadWidget *m_pKeypadWid;
    ConferenceStackWidget *m_pConfWid;
    int preIndex;
};

#endif // PHONECALLSTACKWIDGET_H
