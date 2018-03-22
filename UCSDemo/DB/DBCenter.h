#ifndef UCSDBCENTER_H
#define UCSDBCENTER_H

#include <QObject>
#include <qglobal.h>
#include <QMutex>
#include "LoginEntityManager.h"
#include "ContactEntityManager.h"

class DBCenter : public QObject
{
    Q_OBJECT
public:    
    static void release();

    static LoginEntityManager *loginMgr();

    static ContactEntityManager *contactMgr();

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

    LoginEntityManager *m_pLoginMgr;
    ContactEntityManager *m_pContactMgr;
};

#endif // UCSDBCENTER_H
