#ifndef MYLABEL_H
#define MYLABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT

public:
    explicit MyLabel(QWidget *parent = Q_NULLPTR);
    explicit MyLabel(const QString &text, QWidget *parent = Q_NULLPTR);
    ~MyLabel();
};

#endif // MYLABEL_H
