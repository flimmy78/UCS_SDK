#include "myIconStyle.h"

MyIconStyle::MyIconStyle()
{
    m_nSize = 20;
}

void MyIconStyle::setIconSize(int size)
{
    m_nSize = size;
}

int MyIconStyle::pixelMetric(QStyle::PixelMetric m, const QStyleOption *opt, const QWidget *widget) const
{
    int size = QCommonStyle::pixelMetric(m, opt, widget);
    if (m == QStyle::PM_SmallIconSize)
    {
        size = m_nSize;
    }

    return size;
}
