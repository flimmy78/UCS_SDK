#-------------------------------------------------
#
# Project created by QtCreator 2018-01-29T09:14:40
#
#-------------------------------------------------

QT       += network xml

### use QImage
#QT       -= gui

INCLUDEPATH += $$PWD UCSTcp

INCLUDEPATH += $$PWD Common
include(Common/Common.pri)

INCLUDEPATH += $$PWD DB
include(DB/DB.pri)

INCLUDEPATH += $$PWD Timer
include(Timer/Timer.pri)

TARGET = UCSIMLib
TEMPLATE = lib

DEFINES += UCSIMLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += UCSIMClient.cpp \
    UCSIMManager.cpp \
    UCSMessage.cpp \
    UCSUserInfo.cpp \
    UCSDiscussion.cpp \
    UCSConversation.cpp \
    UCSMsgContent.cpp \
    UCSTextMsg.cpp \
    UCSImageMsg.cpp \
    UCSVoiceMsg.cpp \
    UCSIMError.cpp \
    UCSDiscussionNotification.cpp \
    UCSCustomMsg.cpp \
    UCSIMEvent.cpp

HEADERS += UCSIMClient.h\
        ucsimlib_global.h \
    UCSIMVersion.h \
    UCSIMManager.h \
    UCSIMPublicDef.h \
    UCSIMDefine.h \
    UCSMessage.h \
    UCSUserInfo.h \
    UCSDiscussion.h \
    UCSConversation.h \
    UCSMsgContent.h \
    UCSTextMsg.h \
    UCSImageMsg.h \
    UCSVoiceMsg.h \
    UCSIMError.h \
    UCSDiscussionNotification.h \
    UCSCustomMsg.h \
    UCSIMEvent.h \
    UCSIMSDKPublic.h

DESTDIR = $$PWD/../Bin/

CONFIG(debug, debug|release) {
    unix:TARGET=$$join(TARGET,,,_debug)
    win32:TARGET=$$join(TARGET,,,_d)
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}


win32:CONFIG(release, debug|release): LIBS += -L$$DESTDIR -lUCSTcpSDK
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DESTDIR -lUCSTcpSDK_d

INCLUDEPATH += $$PWD/../UCSTcpSDK
DEPENDPATH += $$PWD/../UCSTcpSDK
