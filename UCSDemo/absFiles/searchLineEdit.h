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
    void sig_FocusChanged(bool);

public slots:
    void slot_clearText();

private:
    void initLayout();
    void initConnection();

private:
    QPushButton *m_pBtn;
    bool m_bFocusIn;
};

#endif // SEARCHLINEEDIT_H
