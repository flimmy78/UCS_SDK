/**
 * @brief 自定义拨号盘 Widget
 * @author Vinton.Liu
 * @date 2018-01-11
*/

#ifndef KEYPADWIDGET_H
#define KEYPADWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "absFiles/myToolButton.h"
#include "absFiles/myPushButton.h"

/* 自定义拨号盘 Widget */
class KeyPadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeyPadWidget(QWidget *parent = 0);

    /* set button style */
    void setBtnStyle(QString style);
    /* set button icon and icon size */
    void setBtnIcon(QIcon icon, QSize iconSize);
    /* set button text */
    void setBtnText(QString text);
    /* set button size */
    void setBtnSize(QSize size);

protected:

private:
    void initLayout();
    void initConnection();

signals:
    void sigBtnClicked(QString text);

private slots:
    void onBtnDigitClicked();
    void onBtnBackspaceClicked();
    void onBtnClicked();

private:
    QLineEdit *m_pDisplayLine;

    enum { NumDigitButtons = 10 };
    MyToolButton *m_pDigitBtn[NumDigitButtons];
    MyToolButton *m_pStarBtn;
    MyToolButton *m_pPoundBtn;

    MyPushButton m_customBtn;
};

#endif // KEYPADWIDGET_H
