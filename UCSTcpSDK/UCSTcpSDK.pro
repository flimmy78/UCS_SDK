#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T09:51:04
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = UCSTcpSDK
TEMPLATE = lib

DEFINES += UCSTCPSDK_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += _WIN32

LIBS += -lws2_32

INCLUDEPATH += $$PWD Common
include(Common/Common.pri)

INCLUDEPATH += $$PWD UCSIMStack
include(UCSIMStack/UCSIMStack.pri)

SOURCES += \
    UCSLoginManager.cpp \
    UCSProxyRequestThread.cpp \
    UCSTcpMsgDispatch.cpp \
    UCSEvent.cpp \
    UCSTcpSocket.cpp \
    UCSTcpClient.cpp \
    UCSTcpManager.cpp \
    UCSCommonTypes.cpp

HEADERS +=\
    ucstcpsdk_global.h \
    UCSTcpDefine.h \
    ucstcpversion.h \
    UCSLoginManager.h \
    UCSProxyRequestThread.h \
    UCSTcpMsgDispatch.h \
    UCSEvent.h \
    UCSTcpSDK.h \
    UCSTcpSocket.h \
    UCSTcpClient.h \
    UCSTcpManager.h \
    UCSTcpPublicDef.h \
    UCSCommonTypes.h

CONFIG(debug, debug|release) {
    unix:TARGET=$$join(TARGET,,,_debug)
    win32:TARGET=$$join(TARGET,,,_d)
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

# QMAKE_CXXFLAGS_DEBUG += /NODEFAULTLIB:"LIBCMTD.lib" /NODEFAULTLIB:"LIBCMT.lib"
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:"LIBCMT.lib"
QMAKE_LFLAGS_RELEASE +=  /NODEFAULTLIB:"LIBCMT.lib"
