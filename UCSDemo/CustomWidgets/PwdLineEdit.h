#ifndef PWDLINEEDIT_H
#define PWDLINEEDIT_H

#include <QWidget>
#include <qglobal.h>
#include <QPushButton>
#include <QLineEdit>

class PwdShowButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PwdShowButton(QWidget* parent = Q_NULLPTR);
    void setShow(bool isShow);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_enter;
    bool m_isShow;
};

class PwdLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit PwdLineEdit(QWidget *parent = nullptr);

protected:
//    void paintEvent(QPaintEvent *event) override;

private:
    void initLayout();
    void initConnections();

signals:

public slots:

private slots:
    void onBtnPwdShowClicked(bool);

private:
    PwdShowButton *m_pBtnShow;
    bool m_bPwdShowing;
};

#endif // PWDLINEEDIT_H
