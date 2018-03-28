#ifndef PINYIN_GLOBAL_H
#define PINYIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PINYIN_LIBRARY)
#  define PINYINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PINYINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PINYIN_GLOBAL_H
