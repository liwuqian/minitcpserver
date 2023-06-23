#pragma once

// define import or export macro
// NOTE: the file must include before build config
// for example:
// #ifdef MACRO
// #define __SELF
// #endif
// #include "my_imp_exp.h"

#if defined (_WIN32) || defined (WIN32) || defined (_WIN64) || defined (WIN64)
    #define __EXTERN extern
    #define __EXTERN_C extern "C"
    #define __EXPORT __declspec(dllexport)
    #define __IMPORT __declspec(dllimport)

    #ifdef __SELF
        #define __IM_EX_FUNC __EXTERN __EXPORT
        #define __IM_EX_FUNC_C __EXTERN_C __EXPORT
        #define __IM_EX_CLASS __EXPORT
    #else
        #define __IM_EX_FUNC __EXTERN __IMPORT
        #define __IM_EX_FUNC_C __EXTERN_C __IMPORT
        #define __IM_EX_CLASS __IMPORT
    #endif
#else
#endif