#ifndef PHONECALLSTACKWIDGET_H
#define PHONECALLSTACKWIDGET_H

#include <QObject>
#include <QWidget>
#include <QStackedLayout>
#include "common/qtheaders.h"
#include "absFiles/myPushButton.h"
#include "KeypadWidget.h"
#include "ConferenceStackWidget.h"
#include "BaseWidget.h"


class DialWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit DialWidget(QWidget *parent = 0);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;


private:
    void initLayout();
    void initConnection();
    void initMisc();

signals:

public slots:
    void onSwitchPage();
    void slot_curStackChanged(int index);
    void slot_keypadClicked(QString text);
    void slot_startConference();
    void slot_bookConference();

private:
    MyPushButton m_btnArray[2];
//    QStackedWidget m_stackWid;
    KeyPadWidget *m_pKeypadWid;
    ConferenceStackWidget *m_pConfWid;
    int preIndex;
    QStackedLayout *m_pStackedLayout;
};

#endif // PHONECALLSTACKWIDGET_H
