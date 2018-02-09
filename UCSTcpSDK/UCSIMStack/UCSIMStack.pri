#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T16:48:14
#
#-------------------------------------------------


SOURCES += \
    $$PWD/bignum.cpp \
    $$PWD/Crypt.cpp \
    $$PWD/IMmsgstruct.cpp \
    $$PWD/iMsgBodyXP.cpp \
    $$PWD/iMsgHeadXP.cpp \
    $$PWD/iSKTLVBuffer.cpp \
    $$PWD/iSKTLVMergePack.cpp \
    $$PWD/iSKTLVPack.cpp \
    $$PWD/iTLVFastReader.cpp \
    $$PWD/iTLVItem.cpp \
    $$PWD/iTlvMsgXP.cpp \
    $$PWD/iTLVPack.cpp \
    $$PWD/iVByte.cpp \
    $$PWD/package.cpp \
    $$PWD/rbbuffer.cpp \
    $$PWD/rbdes.cpp \
    $$PWD/rsa.cpp \
    $$PWD/sha1.cpp \
    $$PWD/sk_builtin.cpp \
    $$PWD/skalloc.cpp \
    $$PWD/skbuffer.cpp \
    $$PWD/skbuildinclass.cpp \
    $$PWD/skbuiltintype.cpp \
    $$PWD/skfakemetastruct.cpp \
    $$PWD/skinnerstruct.cpp \
    $$PWD/skmetainfofinder.cpp \
    $$PWD/skmetautils.cpp \
    $$PWD/skpbdecoder.cpp \
    $$PWD/skpbencoder.cpp \
    $$PWD/skpbhelper.cpp \
    $$PWD/skpbpickle.cpp \
    $$PWD/skprotohelper.cpp \
    $$PWD/sktlvhelper.cpp \
    $$PWD/sktlvpickle.cpp \
    $$PWD/typexp.cpp \
    $$PWD/ucscompress.cpp \
    $$PWD/ucspkg.cpp \
    $$PWD/aes.c \
    $$PWD/PackageHeader.cpp \
    $$PWD/oldrbdes.cpp \
    $$PWD/UCSPackage.cpp

HEADERS += \
    $$PWD/aes.h \
    $$PWD/bignum.h \
    $$PWD/config.h \
    $$PWD/Crypt.h \
    $$PWD/iErrno.h \
    $$PWD/iHash.h \
    $$PWD/iLog.h \
    $$PWD/IMmsgstruct.h \
    $$PWD/iMsgBodyXP.h \
    $$PWD/iMsgHeadXP.h \
    $$PWD/iProtocolXP.h \
    $$PWD/iQsUin.h \
    $$PWD/iSKTLVBuffer.h \
    $$PWD/iSKTLVMergePack.h \
    $$PWD/iSKTLVPack.h \
    $$PWD/iTLVFastReader.h \
    $$PWD/iTLVItem.h \
    $$PWD/iTlvMsgXP.h \
    $$PWD/iTLVPack.h \
    $$PWD/iTLVPickle.h \
    $$PWD/iTypeXP.h \
    $$PWD/iVByte.h \
    $$PWD/package.h \
    $$PWD/padlock.h \
    $$PWD/ProxyProtocol.h \
    $$PWD/rbbuffer.h \
    $$PWD/rbdes.h \
    $$PWD/rsa.h \
    $$PWD/sha1.h \
    $$PWD/sk_builtin.h \
    $$PWD/skalloc.h \
    $$PWD/skbuffer.h \
    $$PWD/skbuildinclass.h \
    $$PWD/skbuiltintype.h \
    $$PWD/skcomm.h \
    $$PWD/skfakemetastruct.h \
    $$PWD/skinnerstruct.h \
    $$PWD/skmetainfo.h \
    $$PWD/skmetainfofinder.h \
    $$PWD/skmetautils.h \
    $$PWD/skpbdecoder.h \
    $$PWD/skpbencoder.h \
    $$PWD/skpbhelper.h \
    $$PWD/skpbpickle.h \
    $$PWD/skprotohelper.h \
    $$PWD/sktlvhelper.h \
    $$PWD/sktlvpickle.h \
    $$PWD/tlvport.h \
    $$PWD/typexp.h \
    $$PWD/ucscompress.h \
    $$PWD/ucspkg.h \
    $$PWD/ucssyncdef.h \
    $$PWD/xyssl.h \
    $$PWD/PackageHeader.h \
    $$PWD/oldrbdes.h \
    $$PWD/UCSPackage.h \
    $$PWD/UCSStackDefine.h


win32: LIBS += -L$$PWD/libs/ -lzlibwapi

INCLUDEPATH += $$PWD/zlib
DEPENDPATH += $$PWD/zlib

# win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/zlibwapi.lib
# else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/libzlibwapi.a
