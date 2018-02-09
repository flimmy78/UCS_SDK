#ifndef MIDLEFTIMLISTWIDGET_H
#define MIDLEFTIMLISTWIDGET_H

#include "common/qtheaders.h"

typedef struct
{
    QString headerPath;
    QString nickName;
    QString msgTime;
    QString msgData;
} ImMsgItem;

class MidLeftIMRowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MidLeftIMRowWidget(ImMsgItem *pItem, QWidget *parent = 0);

private:
    void initLayout();


private:
    QLabel m_LblHeaderIcon;
    QLabel m_LblName;
    QLabel m_LblTime;
    QLabel m_LblData;
};

class MidLeftIMListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit MidLeftIMListWidget(QWidget *parent = 0);
    void addListItem(ImMsgItem *pItem);

protected:
    void contextMenuEvent(QContextMenuEvent *);

private:
    void initConnection();

signals:
    void sig_itemClicked(int row);

public slots:
    void slot_itemCliecked(QListWidgetItem *pItem);
    void slot_addItem(ImMsgItem *pItem);
    void slot_removeRow();
    void slot_setTopRow();

private:

};

#endif // MIDLEFTIMLISTWIDGET_H
