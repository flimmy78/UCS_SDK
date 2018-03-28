#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H

#include <QWidget>
#include <qglobal.h>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include "BaseWidget.h"

enum ButtonType
{
    MIN_BUTTON = 0,     ///< 最小化和关闭按钮 >
    MIN_MAX_BUTTON,     ///< 最小化，最大化和关闭按钮 >
    ONLY_CLOSE_BUTTON,  ///< 只有关闭按钮 >
};

class MyTitleBar : public BaseWidget
{
    Q_OBJECT
public:
    explicit MyTitleBar(QWidget *parent = 0);
    ~MyTitleBar();

    /*!
     * \brief setBackgroundColor 设置标题栏背景色
     * \param r 颜色分量
     * \param g 颜色分量
     * \param b 颜色分量
     * \param isTransparent 是否透明
     */
    void setBackgroundColor(int r, int g, int b, bool isTransparent = false);

    /*!
     * \brief setTitleIcon 设置标题栏图标
     * \param filepath 图标资源路径
     */
    void setTitleIcon(QString filePath);

    /*!
     * \brief setTitleContent 设置标题内容
     * \param titleContent 标题内容
     */
    void setTitleContent(QString titleContent);

    /*!
     * \brief setTitleContentFontSize 设置标题内容字体大小
     * \param fontSize
     */
    void setTitleContentFontSize(int fontSize = 9);

    /*!
     * \brief setTitleWidth 设置标题栏长度
     * \param width 标题栏长度
     */
    void setTitleWidth(int width);

    /*!
     * \brief setButtonType 设置标题栏上按钮类型
     * \param buttonType
     */
    void setButtonType(ButtonType buttonType);

    /*!
     * \brief setTitleRoll 设置标题栏中的标题是否会滚动
     */
    void setTitleRoll();

    /*!
     * \brief setWindowBorderWidth 设置窗口边框宽度
     * \param borderWidth
     */
    void setWindowBorderWidth(int borderWidth);

    ///< 保存/获取 最大化前窗口的位置及大小 >
    void saveRestoreInfo(const QPoint point, const QSize size);
    void getRestoreInfo(QPoint &point, QSize &size);

    /*!
     * \brief setMoveParentWindowFlag 设置是否通过标题栏移动窗口
     * \param isMoveParentWindow
     */
    void setMoveParentWindowFlag(bool isMoveParentWindow);

private:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    /*!
     * \brief initLayout 初始化控件布局
     */
    void initLayout();

    /*!
     * \brief initConnections 信号槽绑定
     */
    void initConnections();

signals:
    ///< 按钮触发的信号 >
    void signalBtnMinClicked();
    void signalBtnRestoreClicked();
    void signalBtnMaxClicked();
    void signalBtnCloseClicked();

private slots:
    ///< 按钮触发的槽 >
    void slotOnBtnMinClicked();
    void slotOnBtnRestoreClicked();
    void slotOnBtnMaxClicked();
    void slotOnBtnCloseClicked();
    void slotOnRollTitle();

private:
    QLabel *m_pIcon;                ///< 标题栏图标 >
    QLabel *m_pTitleContent;        ///< 标题栏内容 >
    QPushButton *m_pBtnMin;         ///< 最小化按钮 >
    QPushButton *m_pBtnRestore;     ///< 最大化还原按钮 >
    QPushButton *m_pBtnMax;         ///< 最大化按钮 >
    QPushButton *m_pBtnClose;       ///< 关闭按钮 >

    ///< 标题栏背景色 >
    int m_colorR;
    int m_colorG;
    int m_colorB;

    ///< 最大化，最小化变量 >
    QPoint m_restorePos;
    QSize m_restoreSize;

    ///< 移动窗口的变量 >
    bool m_isPressed;
    QPoint m_WindowPos;
    QPoint m_startMovePos;

    ///< 标题栏跑马灯效果时钟 >
    QTimer m_titleRollTimer;

    ///< 标题栏内容 >
    QString m_titleContent;

    ///< 按钮类型 >
    ButtonType m_buttonType;

    ///< 窗口边框宽度 >
    int m_windowBorderWidth;

    ///< 标题栏是否透明 >
    bool m_isTransparent;

    ///< 是否通过标题栏移动窗口 >
    bool m_isMoveParentWindow;
};

#endif // MYTITLEBAR_H
