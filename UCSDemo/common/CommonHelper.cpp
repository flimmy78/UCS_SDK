#include "CommonHelper.h"

#include <QFile>
#include <QApplication>

CommonHelper::CommonHelper()
{

}

void CommonHelper::loadStyle(const QString &style)
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
