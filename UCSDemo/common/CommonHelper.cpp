#include "CommonHelper.h"

#include <QFile>
#include <QApplication>
#include <QDateTime>

CommonHelper::CommonHelper()
{

}

void CommonHelper::loadAppStyle(const QString &style)
{
    QFile file(style);
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = QString(file.readAll());
        qApp->setStyleSheet(styleSheet);
        file.close();
    }
}

QImage CommonHelper::toGray(QImage image)
{
    int height = image.height();
    int width = image.width();
    QImage ret = QImage(width, height, QImage::Format_Indexed8);

    ret.setColorCount(256);

    for (int i = 0; i < 256; i++)
    {
        ret.setColor(i, qRgb(i, i, i));
    }

    switch (image.format()) {
    case QImage::Format_Indexed8:
    {
        for (int i = 0; i < height; i++)
        {
            const uchar *pSrc = (uchar*)image.constScanLine(i);
            uchar *pDst = (uchar*)ret.scanLine(i);
            memcpy(pDst, pSrc, width);
        }
    }
        break;

    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        for (int i = 0; i < height; i++)
        {
            const QRgb *pSrc = (QRgb*)image.constScanLine(i);
            uchar *pDst = (uchar*)ret.scanLine(i);
            for (int j = 0; j < width; j++)
            {
                pDst[j] = qGray(pSrc[j]);
            }
        }
    }
        break;

    default:
        break;
    }

    return ret;
}

QString CommonHelper::timeToString(qint64 time, QString format)
{
    QDateTime dateTime;
    return dateTime.fromTime_t(time).toString(format);
}
