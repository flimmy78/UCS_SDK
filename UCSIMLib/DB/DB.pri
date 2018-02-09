
QT      += sql

#INCLUDEPATH += $$PWD Entity
#include(Entity/Entity.pri)

HEADERS += \
    $$PWD/UCSDBCenter.h \
    $$PWD/UCSConnectionPool.h \
    $$PWD/UCSDBEntity.h \
    $$PWD/UCSDBHelper.h \
    $$PWD/UCSChatDBManager.h \
    $$PWD/UCSConversationDBManager.h \
    $$PWD/UCSDiscussionDBManager.h \
    $$PWD/UCSGroupDBManager.h \
    $$PWD/UCSUserInfoDBManager.h \
    $$PWD/UCSVoiceInfoDBManager.h

SOURCES += \
    $$PWD/UCSDBCenter.cpp \
    $$PWD/UCSConnectionPool.cpp \
    $$PWD/UCSDBHelper.cpp \
    $$PWD/UCSDBEntity.cpp \
    $$PWD/UCSChatDBManager.cpp \
    $$PWD/UCSConversationDBManager.cpp \
    $$PWD/UCSDiscussionDBManager.cpp \
    $$PWD/UCSGroupDBManager.cpp \
    $$PWD/UCSUserInfoDBManager.cpp \
    $$PWD/UCSVoiceInfoDBManager.cpp
