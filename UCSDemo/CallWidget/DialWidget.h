#ifndef PHONECALLSTACKWIDGET_H
#define PHONECALLSTACKWIDGET_H

#include <QObject>
#include <QWidget>
#include "common/qtheaders.h"
#include "BaseWidget.h"


class DialWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit DialWidget(QWidget *parent = 0);

private:
    void initLayout();
    void initConnection();
    void initMisc();

signals:

public slots:

private slots:
    void onBtnDigitClicked(bool);
    void onBtnDelNumber(bool);
    void onBtnDialOut(bool);

private:
    QLineEdit *m_pEdtNumber;

    enum { NumDigitButtons = 12 };
    QPushButton *m_pBtnDigits[NumDigitButtons];

    QPushButton *m_pBtnDial;
};

#endif // PHONECALLSTACKWIDGET_H
