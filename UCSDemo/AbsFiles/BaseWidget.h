#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include <qglobal.h>

class BaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) override;

    /*!
     * \brief loadStyleSheet 加载样式文件
     * \param stylePath 样式文件资源路径
     */
    void loadStyleSheet(const QString &stylePath);

signals:

public slots:
};

#endif // BASEWIDGET_H
