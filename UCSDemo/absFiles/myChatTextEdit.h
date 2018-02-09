/**
 * @brief 即时消息窗口发送文本框自定义，实现Enter 或 Ctrl+Enter 发送
 *          Enter 键发送时，Ctrl + Enter组合键为换行;
 *          Ctrl + Enter 组合键发送时，Enter键为换行.
 * @author Vinton.Liu
 * @date
*/

#ifndef MYCHATTEXTEDIT_H
#define MYCHATTEXTEDIT_H

#include "common/qtheaders.h"

enum CustomSendingMode
{
    /* Enter key to sending, Ctrl + Enter for line wrap */
    ENTER_SEND = 0,
    /* Ctrl + Enter key to sending, Enter for line wrap */
    CTRL_ENTER_SEND = 1
};

class MyChatTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyChatTextEdit(QWidget *parent = Q_NULLPTR);

    void setSendingMode(CustomSendingMode mode);
    CustomSendingMode sendingMode() const;

protected:
    void keyPressEvent(QKeyEvent *e) override;

signals:
    void sig_msgSending();

private:
    CustomSendingMode m_sendingMode;
};

#endif // MYCHATTEXTEDIT_H
