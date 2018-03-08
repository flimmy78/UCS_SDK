#ifndef BASEMSGWIDGET_H
#define BASEMSGWIDGET_H

#include <QWidget>
#include <qglobal.h>

class BaseMsgWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseMsgWidget(QWidget *parent = 0);

    bool isSender() const;
    void setIsSender(bool isSender);

    void setNickName(const QString &nickName);

    QString userName() const;
    void setUserName(const QString &userName);

    QString time() const;
    void setTime(const QString &time);

    QString nickName() const;

private:
    void initLayout();
    void initConnections();

signals:

public slots:

private:
    QString m_nickName;
    QString m_userName;
    QString m_time;
    bool m_isSender;
};

#endif // BASEMSGWIDGET_H
