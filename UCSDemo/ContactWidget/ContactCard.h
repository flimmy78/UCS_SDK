#ifndef CONTACTCARD_H
#define CONTACTCARD_H

#include <QLabel>
#include <QWidget>
#include <qglobal.h>
#include "ContactDefine.h"
#include "myPushButton.h"
#include "BaseWidget.h"
#include "myLabel.h"

class ContactCard : public BaseWidget
{
    Q_OBJECT
public:
    explicit ContactCard(QWidget *parent = nullptr);

private:
    void initLayout();
    void initConnections();

signals:
    void sigOpenImPage(ContactItem);

public slots:
    void onShowContact(ContactItem contact);

private slots:
    void onBtnDirectClicked(bool);
    void onBtnAudioClicked(bool);
    void onBtnVideoClicked(bool);
    void onBtnConfClicked(bool);
    void onBtnIMClicked(bool);

private:
    QLabel *m_pLblImage;
    MyLabel *m_pLblName;
    MyLabel *m_pLblPhone;
    MyLabel *m_pLblSection;

    MyPushButton *m_pBtnDirect;
    MyPushButton *m_pBtnAudio;
    MyPushButton *m_pBtnVideo;
    MyPushButton *m_pBtnConf;
    MyPushButton *m_pBtnIM;

    ContactItem m_contact;
};

#endif // CONTACTCARD_H
