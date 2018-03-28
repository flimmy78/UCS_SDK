#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>
#include <QObject>
#include <qglobal.h>

class BaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseDialog(QWidget *parent = 0);

protected:
    void loadStyleSheet(const QString &stylePath);
};

#endif // BASEDIALOG_H
