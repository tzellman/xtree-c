#ifndef __XTREE_SYSTEM_H__
#define __XTREE_SYSTEM_H__

#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <string.h>

#ifdef __cplusplus
#   define EXTERN_C     extern "C"
#   define CXX_GUARD    EXTERN_C {
#   define CXX_ENDGUARD }
#   define XTREE_BOOL   bool
#   define XTREE_TRUE   true
#   define XTREE_FALSE  false
#else
#   define EXTERN_C
#   define CXX_GUARD
#   define CXX_ENDGUARD
#   define XTREE_BOOL   int
#   define XTREE_TRUE   1
#   define XTREE_FALSE  0
#endif

#ifdef WIN32
#   if defined(WIN32_EXPORTS)
#       define PUBFUNC(RT)      EXTERN_C __declspec(dllexport) RT
#   else
#       define PUBFUNC(RT)      EXTERN_C __declspec(dllimport) RT
#   endif
#else
#   define PUBFUNC(RT)  EXTERN_C RT
#endif
#define PRIVFUNC(RT) static RT

#endif

