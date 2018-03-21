#ifndef UCSDBCENTER_H
#define UCSDBCENTER_H

#include <QObject>
#include <qglobal.h>
#include <QMutex>
#include "UCSTcp/UCSTcpDefine.h"

class DBCenter : public QObject
{
    Q_OBJECT
public:    
    static void release();

protected:
    void customEvent(QEvent *event) override;    

private:
    static DBCenter *getInstance();

    explicit DBCenter(QObject *parent = 0);
    /* do nothing, ensure single instance */
    DBCenter(const DBCenter &);
    DBCenter& operator =(const DBCenter&);

    virtual ~DBCenter();

signals:

public slots:

private:

private:
    static DBCenter *m_pInstance;
    static QMutex m_Mutex;
};

#endif // UCSDBCENTER_H
