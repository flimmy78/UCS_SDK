#ifndef MIDDLEWIDGETLEFT_H
#define MIDDLEWIDGETLEFT_H

#include <QObject>
#include <QWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include "midLeftContactsStackWidget.h"
#include "midLeftCallListStackWidget.h"
#include "midLeftIMStackWidget.h"
#include "midLeftSearchStackWidget.h"
#include "absFiles/searchLineEdit.h"
#include "middleRightWidget.h"


class MiddleLeftWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MiddleLeftWidget(QWidget *parent = 0, int width = 250);

    void setMidRightWidget(MiddleRightWidget *p);
    MiddleRightWidget* midRightWidget();
    MidLeftIMStackWidget* imStackWid() { return &m_imMsgSWid; }

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void initLayout();
    void initConnection();
    void initMisc();

signals:
    void sig_setTitle(QString title);

private slots:
    void slot_lineSearch();
    void slot_lineSearchFocusChanged(bool bFocusIn);

    void slot_switchStack();


public:
    QStackedWidget m_stackWid;
    SearchLineEdit m_searchLine;
    MidLeftIMStackWidget m_imMsgSWid;

private:
    MidLeftContactsStackWidget m_contractsSWid;
    MidLeftCallListStackWidget m_callListSWid;
    MidLeftSearchStackWidget m_searchSWid;

    MiddleRightWidget *m_pMidRightWid;

    int preIndex;
};

#endif // MIDDLEWIDGETLEFT_H
