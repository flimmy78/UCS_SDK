#-------------------------------------------------
#
# Project created by QtCreator 2018-01-02T09:26:50
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $$PWD Common
include(Common/Common.pri)

INCLUDEPATH += $$PWD AbsFiles
include(AbsFiles/AbsFiles.pri)

INCLUDEPATH += $$PWD SingleApplication
include(SingleApplication/SingleApplication.pri)

INCLUDEPATH += $$PWD TrayIconMenu
include(TrayIconMenu/TrayIconMenu.pri)

INCLUDEPATH += $$PWD LeftNavigatorBarWidget
include(LeftNavigatorBarWidget/LeftNavigatorBarWidget.pri)

INCLUDEPATH += $$PWD TopWidget
include(TopWidget/TopWidget.pri)

INCLUDEPATH += $$PWD MiddleWidget
include(MiddleWidget/MiddleWidget.pri)

INCLUDEPATH += $$PWD ListWidgets
include(ListWidgets/ListWidgets.pri)

INCLUDEPATH += $$PWD TableWidgets
include(TableWidgets/TableWidgets.pri)

INCLUDEPATH += $$PWD ContactTreeWidget
include(ContactTreeWidget/ContactTreeWidget.pri)

INCLUDEPATH += $$PWD KeypadWidget
include(KeypadWidget/KeypadWidget.pri)

INCLUDEPATH += $$PWD ConferenceWidgets
include(ConferenceWidgets/ConferenceWidgets.pri)

INCLUDEPATH += $$PWD RestManager
include(RestManager/RestManager.pri)

INCLUDEPATH += $$PWD Login
include(Login/Login.pri)

INCLUDEPATH += $$PWD CallWidget
include(CallWidget/CallWidget.pri)

INCLUDEPATH += $$PWD ContactWidget
include(ContactWidget/ContactWidget.pri)

INCLUDEPATH += $$PWD IMWidget
include(IMWidget/IMWidget.pri)

#INCLUDEPATH += $$PWD Logger
#include(Logger/Logger.pri)

INCLUDEPATH += $$PWD Interface

TARGET = UCSDemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

DESTDIR = $$PWD/Bin/

CONFIG(debug, debug|release) {
    unix:TARGET=$$join(TARGET,,,_debug)
    win32:TARGET=$$join(TARGET,,,_d)
} else {

}

RESOURCES += \
    res_mainleft.qrc \
    res_icon.qrc \
    res_top.qrc \
    qm.qrc \
    res_midright.qrc \
    LoginWindow.qrc \
    TreeView.qrc \
    MyTitleBar.qrc \
    main_skin.qrc

VERSION = 1.0.0.0
RC_ICONS = images/yzx_logo.ico
QMAKE_TARGET_COMPANY = "UCPAAS"
QMAKE_TARGET_PRODUCT = "YZX Demo"


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build/UCSTcpSDK/Debug/release/ -lUCSTcpSDK
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/UCSTcpSDK/Debug/debug/ -lUCSTcpSDK_d

INCLUDEPATH += $$PWD/../build/UCSTcpSDK/Debug/debug
DEPENDPATH += $$PWD/../build/UCSTcpSDK/Debug/debug

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build/UCSIMLib/Debug/debug/ -lUCSIMLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/UCSIMLib/Debug/debug/ -lUCSIMLib_d

INCLUDEPATH += $$PWD/../build/UCSIMLib/Debug/debug
DEPENDPATH += $$PWD/../build/UCSIMLib/Debug/debug
