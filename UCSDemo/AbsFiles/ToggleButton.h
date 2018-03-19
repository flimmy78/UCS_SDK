#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QMouseEvent>
#include <QTimer>
#include <QWidget>
#include <qglobal.h>

class ToggleButton : public QWidget
{
    Q_OBJECT
public:
    explicit ToggleButton(QWidget *parent = nullptr);

    /*!
     * \brief setBackGroundColor 设置背景颜色
     * \param color
     */
    void setBackGroundColor(QColor color);

    /*!
     * \brief setCheckedColor 设置选中颜色
     * \param color
     */
    void setCheckedColor(QColor color);

    /*!
     * \brief setDisabledColor 设置不可用颜色
     * \param color
     */
    void setDisabledColor(QColor color);

    bool bChecked() const;
    void setBChecked(bool bChecked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void sigCheckedChanged(bool isChecked);

private slots:
    /*!
     * \brief onTimeout 状态切换时，用于产生滑动效果
     */
    void onTimeout();

private:
    bool m_bChecked;        ///< 是否选中 >
    QColor m_background;    ///< 背景颜色 >
    QColor m_checkedColor;  ///< 选中颜色 >
    QColor m_disabledColor; ///< 不可用颜色 >
    QColor m_thumbColor;    ///< 拇指颜色 >
    qreal  m_radius;        ///< 圆角 >
    qreal  m_nX;            ///< x 点坐标 >
    qreal  m_nY;            ///< y 点坐标 >
    qint16 m_nHeight;       ///< 高度 >
    qint16 m_nMargin;       ///< 外边距 >
    QTimer m_timer;         ///< 定时器 >
};

#endif // TOGGLEBUTTON_H
