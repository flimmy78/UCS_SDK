/**
 * @brief 即时消息窗口发送文本框自定义，实现Enter 或 Ctrl+Enter 发送
 *          Enter 键发送时，Ctrl + Enter组合键为换行;
 *          Ctrl + Enter 组合键发送时，Enter键为换行.
 * @author Vinton.Liu
 * @date
*/

#ifndef CHATTEXTEDIT_H
#define CHATTEXTEDIT_H

#include "common/qtheaders.h"

#include <QMimeData>

enum CustomSendingMode
{
    /* Enter key to sending, Ctrl + Enter for line wrap */
    ENTER_SEND = 0,
    /* Ctrl + Enter key to sending, Enter for line wrap */
    CTRL_ENTER_SEND = 1
};

class ChatTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit ChatTextEdit(QWidget *parent = Q_NULLPTR);

    void setSendingMode(CustomSendingMode mode);
    CustomSendingMode sendingMode() const;

protected:
    void keyPressEvent(QKeyEvent *e) override;

    ///< drag and drop / copy-paste image or text >
    bool canInsertFromMimeData(const QMimeData *source) const override;
    void insertFromMimeData(const QMimeData *source) override;

private:
    void dropImage(const QUrl &url, const QImage &image);

    void dropTextFile(const QUrl &url);

signals:
    void sigSendMessage();

private:
    CustomSendingMode m_sendingMode;
};

#endif // CHATTEXTEDIT_H
