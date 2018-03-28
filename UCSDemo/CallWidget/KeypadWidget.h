/**
 * @brief 自定义拨号盘 Widget
 * @author Vinton.Liu
 * @date 2018-01-11
*/

#ifndef KEYPADWIDGET_H
#define KEYPADWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "myToolButton.h"
#include "myPushButton.h"

/* 自定义拨号盘 Widget */
class KeyPadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeyPadWidget(QWidget *parent = 0);

protected:

private:
    void initLayout();
    void initConnection();

private slots:
    void onBtnDigitClicked();
    void onBtnBackspaceClicked();

private:
    QLineEdit *m_pDisplayLine;

    enum { NumDigitButtons = 12 };
    QPushButton *m_pDigitBtn[NumDigitButtons];
};

#endif // KEYPADWIDGET_H
