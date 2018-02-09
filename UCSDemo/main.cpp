#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include "singleApplication/qSingleApplication.h"

int main(int argc, char *argv[])
{
    QSingleApplication app(argc, argv);
//    QApplication app(argc, argv);

    QTranslator translatorCN;
    if (translatorCN.load(":/res/qm/qt_zh_CN.qm"))
    {
        if (!app.installTranslator(&translatorCN))
        {
            qWarning("failed install zh-Cn translator");
        }
    }
    else
    {
        qWarning("failed loading qt_zh_CN qm");
    }

    QTranslator translator;
    if (translator.load(":/res/qm/widgets.qm"))
    {
        if (!app.installTranslator(&translator))
        {
            qWarning("failed install widgets translator");
        }
    }
    else
    {
        qWarning("failed loading widgets qm");
    }

//    QFont font("微软雅黑", 9, QFont::Normal, false);
//    QSingleApplication::setFont(font);

    if (!app.isRunning())
    {
        MainWindow *pMain = MainWindow::InitInstance();
        app._wid = pMain;
        pMain->show();
        pMain->setGeometry((QApplication::desktop()->width() - pMain->width()) / 2,
                            (QApplication::desktop()->height() - pMain->height()) / 2,
                           pMain->width(),
                           pMain->height());
        return app.exec();
    }

    return 0;
}
