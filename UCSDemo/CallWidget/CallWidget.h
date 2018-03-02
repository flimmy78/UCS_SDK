#ifndef CALLWIDGET_H
#define CALLWIDGET_H

#include <QWidget>
#include <qglobal.h>
#include "BaseWidget.h"
#include "searchLineEdit.h"
#include "midLeftCallListWidget.h"
#include "midRightDialStackWidget.h"
#include "topWidget.h"

class CallWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit CallWidget(QWidget *parent = 0);

signals:

public slots:

private:
    void initLayout();
    void initConnections();

    void updateListData();
private:
    TopWidget *m_pTitleBar;
    SearchLineEdit *m_pSearchEdit;
    MidLeftCallListWidget *m_pCallListWidget;
    MidRightDialStackWidget *m_pDialWidget;
};

#endif // CALLWIDGET_H
