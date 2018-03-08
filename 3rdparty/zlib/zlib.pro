#-------------------------------------------------
#
# Project created by QtCreator 2018-03-08T10:14:33
#
#-------------------------------------------------

QT       -= gui

TARGET = zlib
TEMPLATE = lib

DEFINES += ZLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += _WIN32 _CRT_NONSTDC_NO_DEPRECATE \
        _CRT_SECURE_NO_DEPRECATE \
        _CRT_NONSTDC_NO_WARNINGS \
        ZLIB_WINAPI ASMV ASMINF

#DEFINES -= WIN64

SOURCES += \
        $$PWD/adler32.c \
        $$PWD/compress.c \
        $$PWD/crc32.c \
        $$PWD/deflate.c \
        $$PWD/gzclose.c \
        $$PWD/gzlib.c \
        $$PWD/gzread.c \
        $$PWD/gzwrite.c \
        $$PWD/infback.c \
        $$PWD/inffast.c \
        $$PWD/inflate.c \
        $$PWD/inftrees.c \
        $$PWD/trees.c \
        $$PWD/uncompr.c \
        $$PWD/zutil.c \
        $$PWD/contrib/minizip/ioapi.c \
        $$PWD/contrib/minizip/iowin32.c \
        $$PWD/contrib/minizip/unzip.c \
        $$PWD/contrib/minizip/zip.c

HEADERS += \
        $$PWD/deflate.h \
#        $$PWD/infblock.h \
#        $$PWD/infcodes.h \
        $$PWD/inffast.h \
        $$PWD/inftrees.h \
#        $$PWD/infutil.h \
        $$PWD/zconf.h \
        $$PWD/zlib.h \
        $$PWD/zutil.h

DEF_FILE += $$PWD/contrib/vstudio/vc12/zlibvc.def

win32 {
    EXTRA_BINFILES += \
        $$PWD/contrib/masmx86/*.obj \
        $$PWD/contrib/masmx86/*.lst

    EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
    EXTRA_BINFILES_WIN ~= s,/,\\,g
    DESTDIR_WIN = $${PWD}
    DESTDIR_WIN ~= s,/,\\,g
    for(FILE,EXTRA_BINFILES_WIN){
#         QMAKE_PRE_LINK +=$$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
        system(copy /y $${FILE} $${DESTDIR_WIN})
    }
}

OBJECTS += \
        $$PWD/match686.obj \
        $$PWD/inffas32.obj

DESTDIR = $$PWD/../../Bin

CONFIG(debug, debug|release) {
    unix:TARGET=$$join(TARGET,,,_debug)
    win32:TARGET=$$join(TARGET,,,_d)
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
