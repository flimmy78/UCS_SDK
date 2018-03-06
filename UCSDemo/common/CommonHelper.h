#ifndef COMMONHELPER_H
#define COMMONHELPER_H

#include <QImage>
#include <QObject>
#include <qglobal.h>

class CommonHelper
{
public:
    CommonHelper();

    /*!
     * \brief loadAppStyle 加载app样式表文件
     * \param style
     */
    static void loadAppStyle(const QString &style);

    /*!
     * \brief toGray 将彩色图片转换为灰度图片
     * \param image
     * \return
     */
    static QImage toGray(QImage image);

    static QString timeToString(qint64 time, QString format);
};

#endif // COMMONHELPER_H
