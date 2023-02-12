/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    Cde.h

Abstract:

    C Development Environment base definitions

Author:

    Kilian Kegel

--*/
#ifndef _CDE_H_
#define _CDE_H_
#include <stdarg.h>
#include <stddef.h>

//#define NCDETRACE

////////////////////////////////////////////////////////////////////////////////////////////////////////
// provide import library interface for exit() and vfprintf() for internal usage, it works with original MSFT Library
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  NOTE: To simplify a unified DLLIMPORT interface generation, the interface type is void*
//          except for vfprintf(), that is already declared in CDE.H a full interface type
//          is needed.
// 
#if   defined(_M_AMD64)
    extern void (*__imp_exit)(int);
#   define CDEABI_EXIT(s) (*__imp_exit)(s)
    extern int (*__imp_vfprintf)(void* stream, const char* pszFormat, char* ap);
#   define CDEABI_VFPRINTF(s) (*__imp_vfprintf) s
#else//   defined(_M_AMD64)
    extern void (*_imp__exit)(int);
#   define CDEABI_EXIT(s) (*_imp__exit)(s)
    extern int (*_imp__vfprintf)(void* stream, const char* pszFormat, char* ap);
#   define CDEABI_VFPRINTF(s) (*_imp__vfprintf) s
#endif//  defined(_M_AMD64)

#ifndef _FILE_DEFINED
    #define _FILE_DEFINED
    typedef struct _iobuf
    {
        void* _empty;
    } FILE;
#endif

extern FILE* __acrt_iob_func(unsigned);

#ifndef stdout
#   define stdout (__acrt_iob_func(1))
#endif//stdout

////////////////////////////////////////////////////////////////////////////////////////////////////////
// override the "DebugLib.h" ///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CDE_DONT_OVERRIDE_DEBUG_TRACE
#define __DEBUG_LIB_H__ /*block "DebugLib.h" entirely*/
//
// Declare bits for PcdDebugPrintErrorLevel and the ErrorLevel parameter of DebugPrint()
//
#define DEBUG_INIT      0x00000001  // Initialization
#define DEBUG_WARN      0x00000002  // Warnings
#define DEBUG_LOAD      0x00000004  // Load events
#define DEBUG_FS        0x00000008  // EFI File system
#define DEBUG_POOL      0x00000010  // Alloc & Free (pool)
#define DEBUG_PAGE      0x00000020  // Alloc & Free (page)
#define DEBUG_INFO      0x00000040  // Informational debug messages
#define DEBUG_DISPATCH  0x00000080  // PEI/DXE/SMM Dispatchers
#define DEBUG_VARIABLE  0x00000100  // Variable
#define DEBUG_BM        0x00000400  // Boot Manager
#define DEBUG_BLKIO     0x00001000  // BlkIo Driver
#define DEBUG_NET       0x00004000  // Network Io Driver
#define DEBUG_UNDI      0x00010000  // UNDI Driver
#define DEBUG_LOADFILE  0x00020000  // LoadFile
#define DEBUG_EVENT     0x00080000  // Event messages
#define DEBUG_GCD       0x00100000  // Global Coherency Database changes
#define DEBUG_CACHE     0x00200000  // Memory range cachability changes
#define DEBUG_VERBOSE   0x00400000  // Detailed debug messages that may
                                    // significantly impact boot performance
#define DEBUG_ERROR     0x80000000  // Error

//
// Aliases of debug message mask bits
//
#define EFI_D_INIT      DEBUG_INIT
#define EFI_D_WARN      DEBUG_WARN
#define EFI_D_LOAD      DEBUG_LOAD
#define EFI_D_FS        DEBUG_FS
#define EFI_D_POOL      DEBUG_POOL
#define EFI_D_PAGE      DEBUG_PAGE
#define EFI_D_INFO      DEBUG_INFO
#define EFI_D_DISPATCH  DEBUG_DISPATCH
#define EFI_D_VARIABLE  DEBUG_VARIABLE
#define EFI_D_BM        DEBUG_BM
#define EFI_D_BLKIO     DEBUG_BLKIO
#define EFI_D_NET       DEBUG_NET
#define EFI_D_UNDI      DEBUG_UNDI
#define EFI_D_LOADFILE  DEBUG_LOADFILE
#define EFI_D_EVENT     DEBUG_EVENT
#define EFI_D_VERBOSE   DEBUG_VERBOSE
#define EFI_D_ERROR     DEBUG_ERROR

#ifdef DEBUG
#   undef DEBUG
#endif//DEBUG
#ifdef TRACE
#   undef TRACE
#endif//TRACE

extern void __cdeTianocoreDEBUGEna(void);
extern char* __cdeTianocoreDebugPrintErrolevel2Str(size_t ErrorLevel, const char* Format, ...);
#define DEBUG(Expression)   __cdeTianocoreDEBUGEna(),\
                                fprintf(stdout, "%s`%s(%d)`%s()`%s> ", gEfiCallerBaseName, __FILE__, __LINE__, __FUNCTION__, __cdeTianocoreDebugPrintErrolevel2Str Expression), \
                            DebugPrint Expression
#define TRACE DEBUG
#ifdef ASSERT_RETURN_ERROR
#   undef ASSERT_RETURN_ERROR
#endif//ASSERT_RETURN_ERROR

#define ASSERT_RETURN_ERROR(StatusParameter)

#ifdef ASSERT_EFI_ERROR
#   undef ASSERT_EFI_ERROR
#endif//ASSERT_EFI_ERROR
#define ASSERT_EFI_ERROR(StatusParameter)
//
extern void DebugPrint(size_t ErrorLevel, const char* pszFormat, ...);
extern void __cdecl _assert(char* pszExpession, char* pszFile, unsigned dwLine);

#ifdef ASSERT
#   undef ASSERT
#endif//ASSERT
#define ASSERT(Expression)  /*assert(Expression)*/

#else   //CDE_DONT_OVERRIDE_DEBUG_TRACE
#endif  //CDE_DONT_OVERRIDE_DEBUG_TRACE

//
// externs
//
extern char* gEfiCallerBaseName;
extern void* __cdeGetAppIf(void);
//
// ANSI C Library related extentions 
//
#define __CDEC_HOSTED__ (((void *)0)/*NULL*/ != __cdeGetAppIf())	// replacement for __STDC_HOSTED__ 
extern char* _strefierror(size_t errcode);           // strerror() replacement for UEFI. Convert EFI_STATUS to string


/////////////////////////////////////////////////////////////////////////////
// NOTE: CDETRACE() is THE SUCCESSOR OF CDEMOFINE
/////////////////////////////////////////////////////////////////////////////
typedef union _CDEDBGFP // CDE DEBUG FILE POINTER
{
    void* ptr;
    struct {
        size_t ptr : (sizeof(void*) * 8 - (3 + 1/*bitsizeof(En + Msg)*/ ));
        size_t Msg : 3;	    // highest (bit - 5)[0..2] 27..29/59..61 is the debug message encoding
        size_t En : 1;	    // highest bit - 1 30/62 is the debug message enable
    }CdeDbg;

}CDEDBGFP;// CDE DEBUG FILE POINTER

enum CDEDBGMSGID { BAR, INF, SUC, WAR, ERR, FAT, ASS }; // CDE DEBUG MESSAGE ID: "", "INFO", "SUCCESS", "WARNING", "ERROR", "FATAL", "ASSERT"

#ifndef CDEDBGMAGIC
#   define CDEDBGMAGIC 0x00CDEDB6
#endif
#define CDEDBGMAGICMASK (1LL << (sizeof(void*) * 8 - (3 + 1/*bitsizeof(En + Msg)*/ )))
#define CDEPOSTCODE(c,v)	if(c)outp(0x80,v)
#define CDEDEADLOOP(c,v)	if(c){volatile int abcxyz = v;while(v == abcxyz);}
#define CDEDEBUGBREAK(c,v)	if(c){volatile int abcxyz = v;while(v == abcxyz)__debugbreak();}

#define TRCBAR(cond)    /* bare trace */    (__cdeDbgFp.ptr = stdout, __cdeDbgFp.CdeDbg.Msg = BAR, __cdeDbgFp.CdeDbg.En = cond, __cdeDbgFp.ptr),
#define TRCINF(cond)    /* INFO       */    (__cdeDbgFp.ptr = stdout, __cdeDbgFp.CdeDbg.Msg = INF, __cdeDbgFp.CdeDbg.En = cond, __cdeDbgFp.ptr),
#define TRCSUC(cond)    /* SUCCESS    */    (__cdeDbgFp.ptr = stdout, __cdeDbgFp.CdeDbg.Msg = SUC, __cdeDbgFp.CdeDbg.En = cond, __cdeDbgFp.ptr),
#define TRCWAR(cond)    /* WARNING    */    (__cdeDbgFp.ptr = stdout, __cdeDbgFp.CdeDbg.Msg = WAR, __cdeDbgFp.CdeDbg.En = cond, __cdeDbgFp.ptr),
#define TRCERR(cond)    /* ERROR      */    (__cdeDbgFp.ptr = stdout, __cdeDbgFp.CdeDbg.Msg = ERR, __cdeDbgFp.CdeDbg.En = cond, __cdeDbgFp.ptr),
#define TRCFAT(cond)    /* FATAL      */    (__cdeDbgFp.ptr = stdout, __cdeDbgFp.CdeDbg.Msg = FAT, __cdeDbgFp.CdeDbg.En = cond, __cdeDbgFp.ptr),
#define TRCASS(cond)    /* ASSERT     */    (__cdeDbgFp.ptr = stdout, __cdeDbgFp.CdeDbg.Msg = ASS, __cdeDbgFp.CdeDbg.En = cond, __cdeDbgFp.ptr),

/* __cdeFatAss()

Synopsis
    #include <cde.h>
    static void __cdeFatAss(CDEDBGFP __cdeDbgFp);
Description
    Function deals with CDE FATAL and ASSERT message types:
    FATAL:  invoke exit() and terminate the process
    ASSERT: dead loop
Returns
    
*/
static void __cdeFatAss(CDEDBGFP __cdeDbgFp)
{
    if (FAT == __cdeDbgFp.CdeDbg.Msg)           // FATAL -> exit
        CDEABI_EXIT(3);

    if (ASS == __cdeDbgFp.CdeDbg.Msg)           // ASSERT -> DEADLOOP
    {
        volatile int x = 0xCDE0DEAD;
        while (0xCDE0DEAD == x)
            ;
    }
}

static CDEDBGFP __cdeGetDbgFp(void* p, const char* str, ...)
{
    CDEDBGFP __cdeDbgFp;

    __cdeDbgFp.ptr = p;

    return __cdeDbgFp;

}

static char* __cdeGetSeverityString(CDEDBGFP __cdeDbgFp)
{
    return  (BAR == __cdeDbgFp.CdeDbg.Msg) ? "" : \
        (INF == __cdeDbgFp.CdeDbg.Msg) ? "INFO" : \
        (SUC == __cdeDbgFp.CdeDbg.Msg) ? "SUCCESS" : \
        (WAR == __cdeDbgFp.CdeDbg.Msg) ? "WARNING" : \
        (ERR == __cdeDbgFp.CdeDbg.Msg) ? "ERROR" : \
        (FAT == __cdeDbgFp.CdeDbg.Msg) ? "FATAL" : \
        (ASS == __cdeDbgFp.CdeDbg.Msg) ? "ASSERT" : "UNKNOWN";
}

//
// NOTE/WARNING: CDETRACE macro ARGUMENTS are evaluated multiple times
//      DO NOT PLACE ++/-- operators or function calls in the parameter list!!!
//
#ifndef NCDETRACE
#define CDETRACE(dbgsig_msg)  \
do {\
    CDEDBGFP __cdeDbgFp = __cdeGetDbgFp dbgsig_msg; \
        if (0 == __cdeDbgFp.CdeDbg.En)\
            break;\
        if (BAR != __cdeDbgFp.CdeDbg.Msg) {\
            fprintf(__cdeDbgFp.ptr, "%s`%s(%d)`%s()`%s> ", gEfiCallerBaseName, __FILE__, __LINE__, __FUNCTION__, __cdeGetSeverityString(__cdeDbgFp));\
        }\
        fprintf dbgsig_msg;\
        __cdeFatAss(__cdeDbgFp);\
} while (0)
#else//NCDETRACE

    #define CDETRACE(dbgsig_msg) ((void)0)

#endif// NCDETRACE

//
// CDE GUID definitions
//
//
// Global Unique ID for CDE
//
#define CDE_PEI_TMPTEST0_GUID           {0xCDE00055, 0xb0ff, 0x498b, { 0xb1, 0x7c, 0xed, 0xb3, 0xa0, 0x2e, 0x7f, 0x6e }}
#define CDE_LOAD_OPTIONS_PROTOCOL_GUID  {0xCDE00000, 0x2c09, 0x4284, { 0x90, 0xf9, 0x9a, 0x98, 0x8a, 0xbd, 0xf6, 0xfb }}
#define CDE_PEI_PROTOCOL_GUID           {0xCDE00001, 0xb0ff, 0x498b, { 0xb1, 0x7c, 0xed, 0xb3, 0xa0, 0x2e, 0x7f, 0x6e }}
#define CDE_DXE_PROTOCOL_GUID           {0xCDE00002, 0xe988, 0x4697, { 0x8f, 0x36, 0x08, 0xf1, 0x3d, 0x8d, 0x3d, 0x39 }}
#define CDE_SMM_PROTOCOL_GUID           {0xCDE00003, 0xee5c, 0x4079, { 0xac, 0x93, 0xee, 0x48, 0xe2, 0x1d, 0x2a, 0x99 }}
#define CDE_HOB_GUID                    {0xCDE00004, 0x8801, 0x4cfb, { 0xb1, 0xca, 0xdc, 0x63, 0xde, 0xaa, 0x52, 0xdd }}
#define CDEPKG_TOKEN_SPACE_GUID         {0xCDE00005, 0x31d3, 0x40f5, { 0xb1, 0x0c, 0x53, 0x9b, 0x2d, 0xb9, 0x40, 0xcd }}
#define CDE_END_OF_DXE_GUID             {0xCDE00006, 0x0c2a, 0x4cb4, { 0x82, 0xe4, 0x5a, 0x0b, 0x6f, 0x2f, 0x5e, 0xf2 }}
#define CDE_UNKNOWN_DRIVER_FILE_GUID	{0xCDE00007, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}
#define CDE_APP_IF_HOB_GUID             {0xCDE00008, 0xaa95, 0x48af, { 0xb5, 0xac, 0xb9, 0x32, 0x33, 0xdd, 0xcd, 0x69 }}
#define CDE_UNKNOWN_DRIVER_FILE_NAME	"CDE_UNKNOWN_DRIVER_FILE_NAME"

//
// LoadOptions / command line support
//
typedef struct _COMM_GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} COMM_GUID;

typedef struct _COMMANDLINE {
    int rejectStart;            //  1 -> suppress start of driver, even if registered with EFI_CALLER_ID_GUID. 0 -> start driver and pass command line to it
    COMM_GUID EfiCallerIdGuid;	/*	EFI_CALLER_ID_GUID from AutoGen.h
                                    gEfiCallerIdGuid from AutoGen.c
                                    FILE_GUID from .INF	*/
    char* szCommandLine;		/*  assigned command line includeing filename*/
}COMMANDLINE;

typedef char* GETLOADOPTIONS (void* PeiDxeInterface, COMM_GUID * pEfiCallerIdGuid, char *pVarBuf/* of 128 elements */);

typedef struct _CDE_LOADOPTIONS_PROTOCOL {

    GETLOADOPTIONS* pGetLoadOptions;

}CDE_LOADOPTIONS_PROTOCOL;

#ifndef NCDETRACE
static int fprintfCDE_H(void* stream, const char* pszFormat, ...);//prototype

/* fprintf()

Synopsis
    #include <cde.h>
    static int fprintf(FILE* const stream, const char* const pszFormat, ...);
Description
    fprintf() default override for CDE usage.
    It does exactly the same as the original Standard C fprintf(), except,
    that it clears the upper most 4bits of the stream pointer before usage.
Returns
    number of bytes
*/
#define _CDE_INLINE_FPRINTF_DEFINED
static int fprintf(FILE* stream, const char* pszFormat, ...)
{
    int nRet = 0;
    va_list ap;
    CDEDBGFP __cdeDbgFp;

    __cdeDbgFp.ptr = stream;

    va_start(ap, pszFormat);

    nRet = CDEABI_VFPRINTF(((void*)__cdeDbgFp.CdeDbg.ptr, pszFormat, ap));

    va_end(ap);
    return nRet;
}
#endif//ifndef NCDETRACE
//
//  CDE helper macros
//
#ifndef CDEELC
#   define CDEELC/* CDE ELEMENT COUNT */(x) (sizeof(x)/sizeof(x[0]))
#endif//CDEELC
#ifndef ___WIDE2
#   define ___WIDE2(x) L##x
#endif//___WIDE2
#ifndef ___WIDE1
#   define ___WIDE1(x) ___WIDE2(x)
#endif//___WIDE1
#ifndef __CDEWCSFILE__
#   define __CDEWCSFILE__ ___WIDE1(__FILE__)
#endif//__CDEWCSFILE__
#ifndef __CDEWCSFUNCTION__
#   define __CDEWCSFUNCTION__ ___WIDE1(__FUNCTION__)
#endif//__CDEWCSFUNCTION__
#ifndef CDESTRINGIFY
#   define CDESTRINGIFY(n) #n
#endif//___STRINGIFY
#ifndef CDENUMTOSTR
#   define CDENUMTOSTR(n) CDESTRINGIFY(n)
#endif//CDENUMTOSTR
#ifndef CDENUMTOWCS
#   define CDENUMTOWCS(n) ___WIDE1(CDESTRINGIFY(n))
#endif//CDENUMTOWCS

#endif//_CDE_H_
