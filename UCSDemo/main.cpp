#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include "singleApplication/qSingleApplication.h"
#include <login/UCSLogin.h>
#include "UPlusLogin.h"

int main(int argc, char *argv[])
{
    QSingleApplication app(argc, argv);
//    QApplication app(argc, argv);

    QTranslator translatorCN;
    if (translatorCN.load(":/resources/qm/qt_zh_CN.qm"))
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
    if (translator.load(":/resources/qm/widgets.qm"))
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
        MainWindow mainWindow;
        app._wid = &mainWindow;

//        UCSLogin login;
        UPlusLogin *login = new UPlusLogin;
        if (login->exec() != QDialog::Accepted)
        {
            delete login;
            return 0;
        }
        delete login;

        mainWindow.show();
        mainWindow.setGeometry((QApplication::desktop()->width() - mainWindow.width()) / 2,
                            (QApplication::desktop()->height() - mainWindow.height()) / 2,
                           mainWindow.width(),
                           mainWindow.height());

        return app.exec();
    }

    return 0;
}
