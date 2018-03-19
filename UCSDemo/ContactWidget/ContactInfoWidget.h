#ifndef ADDRESSDETAILSTACKWIDGET_H
#define ADDRESSDETAILSTACKWIDGET_H

#include "common/qtheaders.h"
#include "absFiles/myLabel.h"
#include "BaseWidget.h"
#include "ContactDefine.h"

class ContactInfoWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit ContactInfoWidget(QWidget *parent = 0);

    void updateData();

protected:


private:
    void initLayout();
    void initConnection();

    void contactShow();
signals:

public slots:
    void onContactItemClicked(ContactItem contact);

private slots:
    void onDirectAudioClick(bool);
    void onFreeAudioClick(bool);
    void onFreeVideoClick(bool);
    void onConfCallClick(bool);
    void onImClick(bool);

private:
    QLabel *m_pLblHeaderImg;
    MyLabel *m_pLblUserName;
    QLabel *m_pLblUserSex;
    MyLabel *m_pLblUserPhone;
    MyLabel *m_pLblSectionName;
    MyLabel *m_pLblPhoneTitle;
    MyLabel *m_pLblSectionTitle;

    QToolButton *m_pBtnDirectAudio;
    QToolButton *m_pBtnFreeAudio;
    QToolButton *m_pBtnFreeVideo;
    QToolButton *m_pBtnConfCall;
    QToolButton *m_pBtnIM;

    ContactItem m_contact;
};

#endif // ADDRESSDETAILSTACKWIDGET_H
