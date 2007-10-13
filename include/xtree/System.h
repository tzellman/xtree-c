#ifndef __XTREE_SYSTEM_H__
#define __XTREE_SYSTEM_H__

#ifdef __cplusplus
#   define EXTERN_C extern "C"
#   define CODE_GUARD {
#   define CODE_ENDGUARD }
#   define BOOL bool
#else
#   define EXTERN_C
#   define CODE_GUARD
#   define CODE_ENDGUARD
#   define BOOL int
#endif

#ifdef WIN32
#      if defined(WIN32_EXPORTS)
#          define PUBFUNC(RT)  EXTERN_C __declspec(dllexport) RT
#      else
#          define PUBFUNC(RT)  EXTERN_C __declspec(dllimport) RT
#      endif
#else
#      define PUBFUNC(RT)  EXTERN_C RT
#endif

/*  Code guard  */
#define CXX_GUARD     EXTERN_C CODE_GUARD
#define CXX_ENDGUARD  CODE_ENDGUARD

/*  Private declarations.  */
#define PRIVFUNC(RT) static RT

#endif
