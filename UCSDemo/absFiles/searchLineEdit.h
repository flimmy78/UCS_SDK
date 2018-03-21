#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include "common/qtheaders.h"

class SearchLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit SearchLineEdit(QWidget *parent = 0);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void sigFocusChanged(bool);

public slots:
    void onClearText();

private:
    void initLayout();
    void initConnection();

private:
    QPushButton *m_pBtn;
    bool m_bFocusIn;
};

#endif // SEARCHLINEEDIT_H
