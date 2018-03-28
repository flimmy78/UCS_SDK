#ifndef MASKLABEL_H
#define MASKLABEL_H

#include <QLabel>
#include <QWidget>
#include <qglobal.h>

/**
 * @brief 显示圆形头像
 * @author Vinton.Liu
 * @date
*/
class MaskLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MaskLabel(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:
};

#endif // MASKLABEL_H
