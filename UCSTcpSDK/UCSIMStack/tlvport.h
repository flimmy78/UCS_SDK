#pragma once

// linux platform
//#ifdef linux
#include "iLog.h"
#include "iErrno.h"
#include "iQsUin.h"

#define TLV_LOG_ERR       LogErr
#define TLV_LOG_DEBUG     LogDebug
#define TLV_LOG_IN        LogInternal
#define TLV_WRITE_TO_FILE WriteToFile
//#endif


#ifdef android
#include "iErrno.h"
#include <string.h>

#define TLV_LOG_ERR       
#define TLV_LOG_DEBUG     
#define TLV_LOG_IN
inline int WriteToFile(const char* pFile, const char* pInput, int nLen)
{
		return -1;
}
#define TLV_WRITE_TO_FILE WriteToFile
#endif

#ifdef _WIN32
#include <stdint.h>
#include <io.h>

#include "typexp.h"

#if defined(_MSC_VER) && (_MSC_VER < 1900)
#define snprintf _snprintf
#endif
#define strcasecmp _stricmp

inline int WriteToFile(const char* pFile, const char* pInput, int nLen)
{
    TLV_UNUSED(pFile);
    TLV_UNUSED(pInput);
    TLV_UNUSED(nLen);

    return -1;
}
#endif
