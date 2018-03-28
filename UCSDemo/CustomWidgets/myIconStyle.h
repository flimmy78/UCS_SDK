#ifndef MYICONSTYLE_H
#define MYICONSTYLE_H

#include "common/qtheaders.h"

class MyIconStyle : public QCommonStyle
{
    Q_OBJECT

public:
    explicit MyIconStyle();
    ~MyIconStyle(){}

    void setIconSize(int size);

protected:
    int pixelMetric(PixelMetric m, const QStyleOption *opt, const QWidget *widget) const;

private:
    int m_nSize;
};

#endif // MYICONSTYLE_H
