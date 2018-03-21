#ifndef PHONECALLSTACKWIDGET_H
#define PHONECALLSTACKWIDGET_H

#include <QObject>
#include <QWidget>
#include <QStackedLayout>
#include "common/qtheaders.h"
#include "absFiles/myPushButton.h"
#include "KeypadWidget.h"
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
    void onKeypadClicked(QString text);

private:
    KeyPadWidget *m_pKeypadWid;
};

#endif // PHONECALLSTACKWIDGET_H
