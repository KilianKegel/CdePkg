/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    CdeServices.h

Abstract:

    Definition of CDE-specific internal structs and types

Author:

    Kilian Kegel

--*/
#ifndef _CDE_SERVICES_H_
#define _CDE_SERVICES_H_
//
// select, if neccesarry, one of definitions below BEFORE including this file
//

/*
    #define OS_EFI // operating system EFI
    #define OS_WIN // operating system Windows
    #define OS_LNX // operating system Linux
*/
#ifdef OS_EFI 
#include <uefi.h>
#include <Protocol\SmmCpuIo2.h>      //IntelFrameWorkPkg\Include\Protocol\SmmCpuIo.h
#include <Protocol\CpuIo2.h>         //IntelFrameWorkPkg\Include\Protocol\CpuIo.h
#include <Pi\PiPeiCis.h>            //MdePkg\Include\Pi\PiPeiCis.h
#include <Protocol\StatusCode.h>    // EFI_STATUS_CODE_PROTOCOL C:\MyWorkSpace\MdePkg\Include\Protocol\StatusCode.h:
#include <Protocol\SimpleFileSystem.h>  // file i/o
#include <Protocol/smmbase2.h>
#endif//OS_EFI 

#include <stdarg.h>
#include <stddef.h>

#include <stdint.h>

#define _INC_SETJMPEX           /* prevent Microsoft specific definition of setjmp macro                            */
#define setjmp  msft_setjmp     /* rename original  setjmp() to establish my own "__declspec(dllimport)  setjmp()"  */
#define longjmp msft_longjmp    /* rename original longjmp() to establish my own "__declspec(dllimport) longjmp()"  */
#include <setjmp.h>
#undef longjmp
#undef setjmp

#include <fcntl.h>

#ifndef fpos_t
typedef long long fpos_t;
#endif//fpos_t

extern void* __cdeGetIOBuffer(unsigned i);

//
// Global configuration
//
#define CDE_OPTIONAL/*CDE replacement for OPTIONAL macro*/
#define CDE_FREE_MEMORY_ALLOCATION_ON_EXIT TRUE     /* TRUE per specification */
#define CDE_ATEXIT_REGISTRATION_NUM 32              /* min 32 per specification */
#define CDE_CARRIAGERETURN_AFTER_LINEFEED TRUE      /* TRUE per specification */
#define CDE_ARGV_MAX 24                             /* maximum argv */
#define CDE_MAPV_MAX 8                              /* maximum drive mappings */
#define CDE_VOLV_MAX 32                             /* maximum volumes supported */
#define CDE_DRIVEYNAME_SIZE sizeof("FS999:")        /* maximum length of drive name FSxyz:\ */
//                                                     1               2               3               4               5               6               7               8               9               A               B               C               D               E               F               x
#define CDE_FILEDIRNAME_SIZE sizeof("\\D123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDE\\")
#define CDE_FILESYSNAME_SIZE_MAX (CDE_DRIVEYNAME_SIZE + CDE_FILEDIRNAME_SIZE - sizeof(""))
#define CDE_FILESYSNAME_LEN_MAX  (CDE_DRIVEYNAME_SIZE + CDE_FILEDIRNAME_SIZE - 2 * sizeof("") )
#ifndef CDE_FILEV_MAX
#   define CDE_FILEV_MAX 64                         /* maximum files open */
#endif//CDE_FILEV_MAX
#ifndef CDE_FILEV_MIN
#   define CDE_FILEV_MIN 3                          /* minimum number of file buffers: stdin, stdout, stderr */
#endif//CDE_FILEV_MIN
#define CDE_REMOVEPATHFROMFILENAME 1                /* remove path from CDE debug traces */
#define CDE_STATUSCODE2COM1BYCMDLN 1                /* /REPORTSTATUSCODE:COM1 */
#define CDE_STATUSCODE2STDOUTBYCMDLN 1              /* /REPORTSTATUSCODE:STDOUT */
#define CDE_UNICODE_NAC ((unsigned short)0xFFFF)    /* FFFF NOT A CHARACTER */
#define CDE_STDIN  ((CDEFILE*)__cdeGetIOBuffer(0)/*&_iob[0]*/) /* stdin/out/err for internal usage, make independant from different stdio.h-definitions */
#define CDE_STDOUT ((CDEFILE*)__cdeGetIOBuffer(1)/*&_iob[1]*/) /* stdin/out/err for internal usage, make independant from different stdio.h-definitions */
#define CDE_STDERR ((CDEFILE*)__cdeGetIOBuffer(2)/*&_iob[2]*/) /* stdin/out/err for internal usage, make independant from different stdio.h-definitions */
#define CDE_MEMALLOC_PAGES_MAX 0x100000             /* 1M * 4096 maximum for memory allocation */
#define CDE_CMD_LINE_LEN 256                        /* max. length of the command line */
#define CDE_SIGNAL_NUM 8                            /* 8 --> SIGINT, SIGILL, SIGFPE, SIGSEGV, SIGTERM, SIGBREAK, SIGABRT, SIGABRT_COMPAT */
#define CDE_COMSPEC_LEN 64                          /* sizeof("c:\windows\system32\cmd.exe") */
#define CDE_SLASHCEXTENDETCOMMAND_LEN 260           /* sizeof("/c ") + szCommand*/
#define IS64BITCODE (sizeof(void*) == 8)
#define CDE_TMPBUF_WCHAR_LEN 257                    /* temporary buffer to store TMPNAM or expand buffer for scanf scanset %[]*/
#ifndef IN
#define IN
#endif//IN
#ifndef OUT
#define OUT
#endif//OUT
#ifndef FALSE
#define FALSE 0
#endif//FALSE
#ifndef TRUE
#define TRUE (!FALSE)
#endif//TRUE
#define CDE_STATUS size_t
#define CDE_SUCCESS 0
#define CDE_DEVICE_ERROR 7

#define CDE_WMAIN_LIBDFLT "CDE_WMAIN_LIBDFLT"
#define CDE_MAIN_LIBDFLT "CDE_MAIN_LIBDFLT"

//
// Forward declaration for the CDE_SERVICES.
//
typedef struct _CDE_SERVICES CDE_SERVICES;
typedef enum _OSIF {
    PEIIF, DXEIF, SMMIF, SHELLIF, WINNTIF, LINUXIF
}OSIF;
typedef struct _CDE_APP_IF CDE_APP_IF;
//#ifdef OS_EFI
typedef struct tagCDESYSTEMVOLUMES CDESYSTEMVOLUMES;
//#endif//def OS_EFI

//----------------------------------------------------------------------------
// Definition of helper structs CDE protocol
//----------------------------------------------------------------------------
//
// NOTE KG20220419: "fpos_t" is used for fsetpos() as well as for fseek()/_fseeki64().
// 
//                  1.  "fpos_t" is a signed value "long long" in the Microsoft implementation.
// 
//                  2.  fsetpos() for negative seek positions values MOVES THE INTERNAL SEEK POINTER 
//                      TO THE END OF FILE and reports errno 22 / EINVAL / "Invalid argument"
// 
//                  3.  fseek()/_fseeki64() uses "fpost_t *pos" as a true signed offset
//
//                      a) "fseek(fp,-1,SEEK_END)"
//                      b) "fseek(fp,-1,SEEK_CUR)"
//                      c) "fseek(fp,-1,SEEK_SET)"
//                      
//                      are valid arguments, as long as the requested seek position is not _before_ 
//                      the begin of file, of course case c) will always fail.
//
//                      In case of failure (negative offset relative to begin of file) fseek()
//                      reports errno 22 / EINVAL / "Invalid argument" and DOES NOT MOVE THE SEEK POINTER
//                      in contrast to the fsetpos() counterpart
//
//                      UEFI supports FAT/FAT32 file system. File size is limited to 4GB == 32bit.
//                      "fpos_t"/"long long" is a signed 64 bit integer.
//                  
//                  Since "unsigned" fsetpos() is internally used for "signed" fseek(), and there is
//                  a different handling with seek positions with bit 63 set (negative value)
//                  CDEFPOS_T/CDEFPOSTTYPE is introduced internally to pass additional the SEEK_SET/SEEK_END
//                  (SEEK_CUR not needed) to the fsetpos() and the underlying OSIF 
//                  osifUefiShellFileSetPos()/osifWinNTFileSetPos() function.
//                  
//                  * __cdeIsCdeFposType()
//                  * __cdeGetBiasCdeFposType()
//                  * __cdeGetOffsetCdeFposType()
// 
//                  The above functions deal correctly with original fpos_t and CDEFPOS_T seek pointers.
//
enum CDEFPOSTTYPE {
    CDE_SEEK_BIAS_LESS_POS              /* 000b */,
    CDE_SEEK_BIAS_MSK = 3               /* 011b */,
    CDE_SEEK_BIAS_SET = 4               /* 100b */,
    CDE_SEEK_BIAS_APPEND                /* 101b */, /* NOTE: This is the APPEND marker in "bpos". Despite the bpos offset, ALL WRITES ARE DONE TO EOF*/
    CDE_SEEK_BIAS_END                   /* 110b */,
    CDE_SEEK_BIAS_LESS_NEG,             /* 111b */
};

typedef union tagCDEFPOS_T // CDE DEBUG FILE POINTER
{
    int64_t fpos64;
    struct {
        uint64_t Offs : 60;
        uint64_t Sign : 1;
        uint64_t Bias : 3;
    }CdeFposBias;

}CDEFPOS_T;// CDE fpos_t + BIAS

//
// CDE_APP_IF CDE application interface provides for each driver in PEI and DXE/SMM the CdeServices and FileHandle,**PeiServices / ImageHandle,*pSystemTable
//
typedef struct _HEAPDESC {
    void* CrumbleFront;
    unsigned long long  qwMagic;
#define FREEMEM 0x4D454552463E3E3EL     /* >>>FREEM */
#define ALLOCMEM 0x434F4C4C413E3E3EL    /* >>>ALLOC */
#define ENDOFMEM 0x464F444E453E3E3EL    /* >>>ENDOF */
    unsigned char fInalterable;
    struct _HEAPDESC* pPred;
    struct _HEAPDESC* pSucc;
    unsigned long long/*EFI_PHYSICAL_ADDRESS*/ PageBase;
    unsigned long  Pages;
    void* CrumbleRear;
}HEAPDESC, * PHEAPDESC;


#define DFL (0 << 0)                /* default preset */
#define PIP (1 << 0)                /* fPointerIsParm:1;       /*filepointer/memorypointer */
#define PIF (1 << 1)                /* fPointerIsFilePointer:1;/*if pointer is passed, it is a filepointer */
#define CIP (1 << 2)                /* fCountIsParm:1;         /*it is a n-function */
#define ADI (1 << 3)                /* fAjustDifference:1;     /*for MS complience - not used */
#define WID (1 << 4)                /* fWide:1;                /*it is a w-function */
#define END (1 << 5)                /* EndMarker:1;            /* 0 for '\0' memory, 1 for EOF for file access */
#define INV (1 << 6)                /* fInverted:1;*/
#define CAS (1 << 7)                /* fCaseSensitive:1;*/
#define BOZ (1 << 8)                /* fBreakOnZero */
#define TDN (1 << 9)                /* top down -> decrement -> copy / move from higher to lower memory */

#define PRESET_PIP (preset & PIP)   /* fPointerIsParm:1;       /*filepointer/memorypointer*/
#define PRESET_PIF (preset & PIF)   /* fPointerIsFilePointer:1;/*if pointer is passed, it is a filepointer*/
#define PRESET_CIP (preset & CIP)   /* fCountIsParm:1;         /*it is a n-function*/
#define PRESET_ADI (preset & ADI)   /* fAjustDifference:1;     /*for MS complience - not used*/
#define PRESET_WID (preset & WID)   /* fWide:1;                /*it is a w-function*/
#define PRESET_END (preset & END)   /* EndMarker:1;            /* 0 for '\0' memory, 1 for EOF for file access*/
#define PRESET_INV (preset & INV)   /* fInverted:1;*/
#define PRESET_CAS (preset & CAS)   /* fCaseSensitive:1;*/
#define PRESET_BOZ (preset & BOZ)   /* fBreakOnZero */
#define PRESET_TDN (preset & TDN)   /* top down -> decrement -> copy / move from higher to lower memory */

typedef struct _ROMPARM_WCSSTRTOK {
    unsigned char fForceToDataSeg;        /*always 1*/
    unsigned char fWide;
}ROMPARM_WCSSTRTOK;


typedef struct _ROMPARM_VWXPRINTF {
    unsigned char fForceToDataSeg;        /*always 1*/
    unsigned char fPointerIsParm;         /*filepointer/memorypointer*/
    unsigned char fPointerIsFilePointer;  /*if pointer is passed, it is a filepointer*/
    unsigned char fCountIsParm;           /*it is a n-function*/
    unsigned char fAjustDifference;       /*for MS complience - not used*/
    unsigned char fWide;                  /*it is a w-function*/
    unsigned char fUEFIFormat;            /*0 Standard C Format string*/
}ROMPARM_VWXPRINTF;

typedef struct _ROMPARM_VWXSCANF {
    unsigned char fForceToDataSeg;        /*always 1*/
    unsigned char fPointerIsParm;         /*filepointer/memorypointer*/
    unsigned char fPointerIsFilePointer;  /*if pointer is passed, it is a filepointer*/
    unsigned char fCountIsParm;           /*it is a n-function*/
    unsigned char fAjustDifference;       /*for MS complience - not used*/
    unsigned char fWide;                  /*it is a w-function*/
    int     EndMarker;              /* 0 for memory, EOF for file access*/
}ROMPARM_VWXSCANF;

typedef struct _ROMPARM_WCSSTRPBRKSPN {
    unsigned char fForceToDataSeg;        /*always 1*/
    unsigned char fInverted;
    unsigned char fWide;
}ROMPARM_WCSSTRPBRKSPN;

typedef struct _ROMPARM_MEMSTRXCMP {
    unsigned char fForceToDataSeg;        /*always 1*/
    unsigned char fCountIsParm;
    unsigned char fCaseSensitive;
    unsigned char fBreakOnZero;
    unsigned char fAjustDifference/*for MS complience*/;
    unsigned char fWide;
}ROMPARM_MEMSTRXCMP;

#ifdef OS_EFI
typedef struct _PEIMPARM {
    OSIF OSIf;
    EFI_PEI_FILE_HANDLE     FileHandle;
    EFI_PEI_SERVICES** PeiServices;
}PEIMPARM;

typedef struct _BS_DRIVERPARM /*BOOT SERVICE DRIVER PARAMETER*/ {
    OSIF OSIf;
    EFI_HANDLE ImageHandle;
    EFI_SYSTEM_TABLE* pSystemTable;
    EFI_SMM_SYSTEM_TABLE2* pSmmSystemTable2;
}BS_DRIVERPARM;
#endif//def OS_EFI

typedef struct _COMM_DRIVERPARM {
    OSIF OSIf;
    void* CommHandle;
    void* pCommAPI;
    void* pCommAPISMM;
}COMM_DRIVERPARM;

typedef union _DRIVERPARM {
    COMM_DRIVERPARM CommParm;
#ifdef OS_EFI
    PEIMPARM        PeimParm;
    BS_DRIVERPARM   BsDriverParm;
#endif//def OS_EFI
}DRIVERPARM;

typedef union _CPUIOXYZ {   //EFI_CPU_IO_PROTOCOL / EFI_SMM_CPU_IO_INTERFACE
#ifdef OS_EFI
    EFI_PEI_CPU_IO_PPI* pPei;
    EFI_SMM_CPU_IO2_PROTOCOL* pSmm;
    EFI_CPU_IO2_PROTOCOL* pShell;
    EFI_CPU_IO2_PROTOCOL* pDxe;
#else//OS_EFI
    void* pDummy;
#endif//def OS_EFI
}CPUIOXYZ;

typedef union _REPORTSTATUSCODE {   //EFI_CPU_IO_PROTOCOL / EFI_SMM_CPU_IO_INTERFACE
#ifdef OS_EFI
    EFI_PEI_REPORT_STATUS_CODE  pPei;
    EFI_REPORT_STATUS_CODE      pSmm;
    EFI_REPORT_STATUS_CODE      pShell;
    EFI_REPORT_STATUS_CODE* pDxe;
#else//OS_EFI
    void* pDummy;
#endif//def OS_EFI
}REPORT_STATUS_CODE;

//
// Function declaration
//
typedef
void*
WCSSTRPBRKSPN(
    IN signed preset,
    IN void* pszStr,
    IN const void* pszSet
    );

typedef
void*
WCSSTRTOK(
    CDE_APP_IF* pCdeAppIf,
    IN void* pszStr,
    IN const void* pszSet,
    IN OUT void** ppLast,
    IN ROMPARM_WCSSTRTOK* pParm
    );

typedef
long
VWXSCANF(
    CDE_APP_IF* pCdeAppIf,
    IN ROMPARM_VWXSCANF* pRomParm,
    IN const char* pszFormat,
    IN int (*pfnDevGetChar)(void** ppSrc),
    IN int (*pfnDevUngetChar)(int c, void** ppDst),
    IN unsigned char* pSrc,
    IN unsigned long dwCount,
    OUT unsigned char** ppEnd,// EndPtr for strtol() and strtoul()
    IN va_list ap
    );


typedef
int
VWXPRINTF(
    CDE_APP_IF* pCdeAppIf,
    IN ROMPARM_VWXPRINTF* pRomParm,
    IN const void* pszFormat,
    IN void (*pfnDevPutChar)(int/*wchar_t*/c, void** ppDest/*address of pDest*/),
    IN void* pDest, /*pointer to helper structure pCdeAppIf (for debug) or memorypointer */
    IN unsigned int dwCount,
    IN va_list ap
    );

typedef
void*
MEMREALLOC(
    CDE_APP_IF* pCdeAppIf,
    IN void* ptr,   /* input pointer for realloc */
    IN size_t size, /*  input size for realloc*/
    IN HEAPDESC* pHeapStart
    );

typedef void* MEMSTRXCPY(
    IN int preset,
    OUT void* pszDest,
    IN  const void* pszSource,
    IN  size_t count
    );

typedef long MEMSTRXCMP(
    IN ROMPARM_MEMSTRXCMP* pRomParm,
    IN const char* pszDest,
    IN const char* pszSource,
    IN size_t count
    );

typedef void _PUTCHAR(
    int c,
    void** ppDes
    );

typedef int _GETCHAR(
    void** ppSrc
    );

#ifdef OS_EFI 
    typedef EFI_STATUS CRT0SERVICE(

        IN CDE_APP_IF* pCdeAppIf,

        ///IN EFI_STATUS(*pMainEntryPoint)                 (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable),

        IN void (*pProcessLibraryConstructorList)       (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable),
        IN EFI_STATUS(*pProcessModuleEntryPointList)    (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable),
        IN void (*pProcessLibraryDestructorList)        (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
    );
#else   //OS_EFI 
    typedef void CRT0SERVICE(void);
#endif  //OS_EFI 

// ----- OSIf - operating system interface
typedef struct tagCDEFILE CDEFILE;          //prototype
typedef struct tagCDEFILEINFO CDEFILEINFO;  //prototype
typedef struct tagCDESTAT64I32 CDESTAT64I32;//prototype

typedef long long           OSIFGETTIME(IN CDE_APP_IF* pCdeAppIf);
typedef long long           OSIFSETTIME(IN CDE_APP_IF* pCdeAppIf, long long);
typedef unsigned long long  OSIFGETTSCPERSEC(IN CDE_APP_IF* pCdeAppIf, unsigned short port);
typedef unsigned long long  OSIFGETTSC(IN CDE_APP_IF* pCdeAppIf);

typedef HEAPDESC*   OSIFMEMALLOC(IN CDE_APP_IF* pCdeAppIf, IN size_t Pages);
typedef void        OSIFMEMFREE(IN CDE_APP_IF* pCdeAppIf, IN unsigned long long /*EFI_PHYSICAL_ADDRESS*/ Memory, IN size_t Pages);

typedef CDEFILE*    OSIFFOPEN(IN CDE_APP_IF* pCdeAppIf, const wchar_t* filename, const char* mode, int fFileExists/*0 no, 1 yes, -1 unk */, CDEFILE* pCdeFile);
typedef int         OSIFFCLOSE(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile);
typedef int         OSIFFDELETE(IN CDE_APP_IF* pCdeAppIf, const char* filename, CDEFILE* pCdeFile);
typedef size_t      OSIFFREAD(IN CDE_APP_IF* pCdeAppIf, void* ptr, size_t nelem, CDEFILE* pCdeFile);
typedef size_t      OSIFFWRITE(IN CDE_APP_IF* pCdeAppIf, void* ptr, size_t nelem, CDEFILE* pCdeFile);
typedef int         OSIFFSETPOS(IN CDE_APP_IF* pCdeAppIf, CDEFILE* pCdeFile, CDEFPOS_T* pos);
typedef int         OSIFFRENAME(IN CDE_APP_IF* pCdeAppIf, wchar_t* pwcsOld, wchar_t* pwcsNew);
typedef CDEFILEINFO*OSIFFFINDALL(IN CDE_APP_IF* pCdeAppIf, IN  char* pstrDrvPthDir, IN OUT int* pCountOrError);
typedef int         OSIFFGETSTATUS(IN CDE_APP_IF* pCdeAppIf, void* pFpOrFname, CDESTAT64I32* pStat64i32, void* pBuf400);
typedef int         OSIFDIRCREATE(IN CDE_APP_IF* pCdeAppIf, IN wchar_t* pwcsDirName);
typedef int         OSIFDIRREMOVE(IN CDE_APP_IF* pCdeAppIf, IN wchar_t* pwcsDirName);
typedef int         OSIFCMDEXEC(IN CDE_APP_IF* pCdeAppIf, const char* szCommand);
typedef char*       OSIFGETENV(IN CDE_APP_IF* pCdeAppIf, const char* szEnvar);
typedef char*       OSIFGETDCWD(IN CDE_APP_IF* pCdeAppIf, IN OUT char* pstrDrvCwdBuf); // get drive current working directory

typedef size_t  COREFILERW(CDE_APP_IF* pCdeAppIf, const void* ptr, size_t size, size_t nelem, CDEFILE* stream);     // complete core fread(), complete core fwrite()
typedef int     CORESETPOS(CDE_APP_IF* pCdeAppIf, CDEFILE* stream, const fpos_t* pos);;                             // complete core fsetpos()
typedef int     COREFFLUSH(CDE_APP_IF* pCdeAppIf, CDEFILE* stream);

struct _CDE_LCONV_LANGUAGE
{
    char* szLanguageCountry;
    struct lconv* pLconv;
    struct lconv* pCopy;    // copy returned for localeconv()
};

typedef struct _CDE_APP_IF
{
    //
    // the CdeServices is present once only in each POST stage (PEI/DXE/SMM)
    //
    CDE_SERVICES* pCdeServices;
    //
    // the stuff below is present per App
    //
    DRIVERPARM DriverParm;                      // FileHandle,**PeiServices / ImageHandle,*pSystemTable

//
// r/w space for C library internal usage
//
    void* pStrtokStaticInternal;                            // scratch buffer for strtok()
    int nNextRandStaticInternal;                            // scratch buffer for rand()
    jmp_buf exit_buf;                                       // jmp_buf for exit
    int     exit_status;                                    // status returned by main in case of exit()
    void (*rgcbAtexit[CDE_ATEXIT_REGISTRATION_NUM])(void);
    char szAscTimeStaticInternal[26];                       // static duration for asctime()
    int  StructTmLocalTimeStaticInternal[9];                // static duration for localtime() == sizeof struct tm
    int  StructTmGmTimeStaticInternal[9];                   // static duration for localtime() == sizeof struct tm
    char szTmpBuf[CDE_TMPBUF_WCHAR_LEN * sizeof(wchar_t)/*NOTE: extend szTmpBuf to 512 byte to expand buffer for scanf scanset %[] too *//*260/*L_tmpnam*/];
    int lTmpNamNum;
    void (*rgfnSignal[CDE_SIGNAL_NUM])(int);
    CDEFILE* pIob;                                          // pointer to _iob[0]
    int  cIob;                                              // count _iob
    char STDOUT816BitMode;                                  // CDE internal:    1 : force character size conversion to 8Bit for STDOUT and STDERR, 
                                                            //                  0 : leave UEFI Shell default 16Bit
    void* pArgvEx[CDE_ARGV_MAX + 2];                        // pointer to +2extended argv[]
    int cArgvEx;                                            // count argv-ex, that is -2 and -1 extendet argv[argc]

    enum RUNTIMEFLAGS{  TIANOCOREDEBUG = 1,         /* enable/disable DebugLib CDE override at runtime */
        CASESENSITIVEFILENAME = 2,                  /* */
        MALLOCFAILISFATAL = 4                       /* memory allocation error is fatal and terminates the program*/
    }bmRuntimeFlags;  // runtime flags


    //
    // locale relateted
    //
    struct _CDE_LCONV_LANGUAGE ActiveLocale;

    //
    // errno
    //
    int nErrno;
    size_t nEfiErrno;

}CDE_APP_IF;

typedef struct _CDE_SERVICES {
    unsigned short wVerMajor;
    unsigned short wVerMinor;
    unsigned char fx64Opcode;
    unsigned char fMemoryDiscovered;        // flag EfiPeiMemoryDiscovered, always true for DXE, SMM
    HEAPDESC HeapStart;

    unsigned long long TSClocksAtSystemStart;
    unsigned long long TSClocksAtCdeTrace;
    unsigned long long TSClocksPerSec;

    long long TimeAtSystemStart;            // epoch time / UNIX time / POSIX time at systemstart
    char fCOM1Timeout;

    REPORT_STATUS_CODE ReportStatusCode;
    void* pvEfiShellProtocol;
    CDESYSTEMVOLUMES* pCdeSystemVolumes;    // system volumes for CdePkg
    CRT0SERVICE* pCRT0Service;

    _GETCHAR* pGetConIn;                    // FNDECL_GETSTDIN(*pgetstdin);            // STDIN  - COM1
    _PUTCHAR* pPutConOut;                   // FNDECL_PUTSTDOUT(*pputstdout);          // STDOUT - COM1
    _PUTCHAR* pPutDebug;                    // FNDECL_PUTDBGOUT(*pputdbgout);          // DBGOUT - COM1
    VWXPRINTF* pVwxPrintf;                  // protocol function 0
    VWXSCANF* pVwxScanf;                    // protocol function 1
    MEMREALLOC* pMemRealloc;
    MEMSTRXCPY* pMemStrxCpy;                //    FNDECL_MEMSTRXNCPY(*pmemstrxncpy);
    MEMSTRXCMP* pMemStrxCmp;                //    FNDECL_MEMSTRXNCMP(*pmemstrxncmp);
//
// string processing functions
//
    WCSSTRPBRKSPN* pWcsStrPbrkSpn;//    FNDECL_MEMSTRXPBRK(*pmemstrxpbrk);
    WCSSTRTOK* pWcsStrTok;//    FNDECL_MEMSTRXTOK(*pmemstrxtok);

    // ----- core C functions, running on driver side

    COREFILERW* pFReadCORE;                 // core fread()
    COREFILERW* pFWriteCORE;                // core fwrite()
    CORESETPOS* pFSetPosCORE;               // core fsetpos()
    COREFFLUSH* pFFlushCORE;                // core fflush()    int _coreFflush(CDE_APP_IF* pCdeAppIf, FILE* stream)

//
// OSIF - operating system interface
//
    OSIFGETTIME* pGetTime;
    OSIFSETTIME* pSetRtcTime;
    OSIFGETTSCPERSEC* pGetTscPerSec;
    OSIFGETTSC* pGetTsc;
//
// memory
//
    OSIFMEMALLOC* pMemAlloc;
    OSIFMEMFREE* pMemFree;
//
// file
//
    OSIFFOPEN* pFopen;
    OSIFFCLOSE* pFclose;
    OSIFFREAD* pFread;
    OSIFFWRITE* pFwrite;
    OSIFFSETPOS* pFsetpos;
    OSIFFDELETE* pFdelete;
    OSIFFRENAME* pFrename;
    OSIFFFINDALL* pFfindall;
    OSIFFGETSTATUS* pFgetstatus;
    // ----- directory
    OSIFDIRCREATE* pDirCreate;
    OSIFDIRREMOVE* pDirRemove;
    // ----- exec/system
    OSIFCMDEXEC* pCmdExec;
    // ----- getenv
    OSIFGETENV* pGetEnv;
    // ----- get drive current working directory
    OSIFGETDCWD* pGetDrvCwd;
    // 
    //void* pOSIFR4FX0; // R4FX: reserved for future extensions
    //void* pOSIFR4FX1; // R4FX: reserved for future extensions
    //void* pOSIFR4FX2; // R4FX: reserved for future extensions
    //void* pOSIFR4FX3; // R4FX: reserved for future extensions
    //void* pOSIFR4FX4; // R4FX: reserved for future extensions
    //void* pOSIFR4FX5; // R4FX: reserved for future extensions
    //void* pOSIFR4FX6; // R4FX: reserved for future extensions
    //void* pOSIFR4FX7; // R4FX: reserved for future extensions
    //void* pDIAGR4FX0; // R4FX: reserved for future extensions
    //void* pDIAGR4FX1; // R4FX: reserved for future extensions
    //void* pDIAGR4FX2; // R4FX: reserved for future extensions
    //void* pDIAGR4FX3; // R4FX: reserved for future extensions

}CDE_SERVICES;

extern void _CdeMemPutWChar(int c, void** ppDest);
extern void _CdeMemPutNada(int c, void** ppDest);
extern void _CdeMemPutChar(int c, void** ppDest);
extern void* __cdeGetAppIf(void);


// ----- definitions for file i/o
//

#define O_CDEWCSZONLY   (1 << 16)/*Bug in FileHandleWrappers.c: FileInterfaceStdOutWrite() does not use BufferSize, instead it writes until ZERO*/
#define O_CDENOSEEK     (1 << 17)/* file does't support seek/tell related functions*/
#define O_CDEREDIR      (1 << 18)/* this is STDIN/STDOUT/STDERR redirected to file (<>|). If NOT SET, EOF is NOT THE END OF THE STREAM, It is a real keyboard */
                                 /* O_CDEREDIR is only used with STDIN/STDOUT/STDERR */
                                 /* NOTE: A REDIRected file is provided by the OS shell when using one of  > < | "operators" from outside. It can not be closed */
#define O_CDEREOPEN     (1 << 19)/* this is STDOUT/STDERR "freopen()"-ed */
                                 /* NOTE: A REOPENED file is provided by the freopen() internally by the program. It must be closed on exit */
#define O_CDESTDIN      (1 << 20)/* this is STDIN  */
#define O_CDESTDOUT     (2 << 20)/* this is STDOUT */
#define O_CDESTDERR     (3 << 20)/* this is STDERR */
#define O_CDESTDMASK    (3 << 20)/* this is mask stdin/out/err */
#define O_CDEWIDTH16    (1 << 22)/* width of the stream 0 = 8 Bit, 1 = 16 Bit */
#define O_CDEDETECTED   (1 << 23)/* stream width detected */

//#define CDE_FPOS_SEEKEND (1LL << 63)

typedef struct tagCDEFILE {
    unsigned char fRsv;                         // 0 if free, 1 if taken /reserved / occupied / allocated
    int     openmode;
    void* emufp;                                // true FILE fp used for emulation in Windows. In true EFI it is the CDEFILE fp
    //long   fpos;                              // fpos -> what ftell returns relative to SEEK_SET
    char* Buffer;                               // internal buffer
    int   bufPosEOF;                            // buffer EOF: If buffer isn't filled completely by the read operation, EOF appears in the buffer at position X
    int    bsiz;                                // sizeof internal buffer
    fpos_t  bpos;                               // buffers position equivalent to file pointer position. bpos[63] == CDE_SEEK_BIAS_APPEND marker CDE_FPOS_SEEKEND!!!
    int     bidx;                               // index into Buffer[] ranges from 0 .. bufsiz
    int     bvld;                               // number of valid bytes in the buffer beginning from
    unsigned char bdirty;                       // buffer is dirty -> the buffer conatains unwritten characters
    unsigned char bclean;                       // buffer is clean -> the buffer conatains characters read from the stream
        //NOTE: dirty and clean TRUE together, implements the microsoft strategy for common read/write data held in a buffer
        //      If CLEAN AND DIRTY are set simultanously, file writes inhibited
    unsigned char fEof;                         // EOF flag for file
    unsigned char  Eof;                         // EOF flag for POSIX
    unsigned char fErr;                         // ERR flag for file
    unsigned char fCtrlZ;                       // END OF TEXT FILE
    int           cntSkipCtrlZChk;              // skip CtrlZ check if > 0
    char* tmpfilename;                          // initialized to NULL by fopen(), set to tmpfilename for tmpfiles
#ifdef OS_EFI
    EFI_FILE_PROTOCOL* pRootProtocol;
    EFI_FILE_PROTOCOL* pFileProtocol;
    wchar_t* pwcsFileDrive;
    wchar_t* pwcsFilePath;
    wchar_t* pwcsFileName;
#else// OS_EFI
    void* pRootProtocol;
    void* pFileProtocol;
    void* pwcsFileDrive;
    void* pwcsFilePath;
    void* pwcsFileName;
#endif//def OS_EFI
    // KG20220418 gap of non-initialized disk space
    // UEFI BUG: file positioning bug, if data written behind EOF, data range between old EOF and 
    // new data contains medium data / garbage, instead of 0
    //
    fpos_t gappos;                              // gap position /*KG20220418 gap of non-initialized disk space*/
    size_t gapsize;                             // gap size     /*KG20220418 gap of non-initialized disk space*/
}CDEFILE;

#ifdef OS_EFI
typedef struct tagCDEFSVOLUME {
    EFI_HANDLE hSimpleFileSystem;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* pSimpleFileSystemProtocol;
    EFI_DEVICE_PATH_PROTOCOL* pDevicePathProtocol;
    unsigned short* pwcsDevicePathText;
    EFI_FILE_PROTOCOL* pRootProtocol;
    int     nVolumeMap;
    unsigned short* rgpVolumeMap[CDE_MAPV_MAX];
    unsigned short* pwcsMap;/**/
}CDEFSVOLUME;

typedef struct tagCDESYSTEMVOLUMES {
    UINTN nVolumeCount;
    //const unsigned short* pwcsAppPath;
    //const unsigned short* pwcsAppName;
    CDEFSVOLUME rgFsVolume[CDE_VOLV_MAX];
}CDESYSTEMVOLUMES;
#endif//def OS_EFI

typedef struct tagCDEFILEINFO 
{
    time_t      time_write;
    uint64_t    size;
    uint8_t     attrib;             // from IO.H: _A_NORMAL _A_RDONLY _A_HIDDEN _A_SYSTEM _A_SUBDIR _A_ARCH
    char        strFileName[0];     // max 260, w/o "EXT" 
    //C99 Spec: "except that the last member of a structure with more than one named member may have incomplete array type"
}CDEFILEINFO;

typedef struct tagCDESTAT64I32  // Microsofts "struct _stat64i32" analogon
{
    uint32_t/*_dev_t*/  st_dev;
    uint16_t/*_ino_t*/  st_ino;
    unsigned short      st_mode;
    short               st_nlink;
    short               st_uid;
    short               st_gid;
    uint32_t/*_dev_t*/  st_rdev;
    int32_t/*_off_t*/   st_size;
    int64_t             st_atime;
    int64_t             st_mtime;
    int64_t             st_ctime;
}CDESTAT64I32;

#define CDEDOUBLE_EXPPOS 52
#define CDEDOUBLE_SIGPOS 63
typedef union tagCDEDOUBLE
{
    double dbl;
    uint64_t uint64;
    struct {
        uint64_t mant : 52;
        uint64_t exp : 11;      // highest (bit - 5)[0..2] 28..30/60..62 is the debug message encoding
        uint64_t sign : 1;      // highest bit - 1 31/63 is the debug message enable
    }member;
}CDEDOUBLE;

//  https://www.intel.com/content/www/us/en/content-details/782158/intel-64-and-ia-32-architectures-software-developer-s-manual-combined-volumes-1-2a-2b-2c-2d-3a-3b-3c-3d-and-4.html?docid=782158#page=211
//  https://www.amd.com/content/dam/amd/en/documents/processor-tech-docs/programmer-references/24592.pdf#page=356
//  https://0x04.net/~mwk/doc/intel/80387dx.pdf#page=11
//Table 2.5.Condition Code Defining Operand Class
//  C3 C2 C1 C0 Value at TOP
//  0  0  0  0  + Unsupported
//  0  0  0  1  + NaN
//  0  0  1  0  - Unsupported
//  0  0  1  1  - NaN
//  0  1  0  0  + Normal
//  0  1  0  1  + Infinity
//  0  1  1  0  - Normal
//  0  1  1  1  - Infinity
//  1  0  0  0  + 0
//  1  0  0  1  + Empty
//  1  0  1  0  - 0
//  1  0  1  1  - Empty
//  1  1  0  0  + Denormal
//  1  1  1  0  - Denormal
//  C3 C2 C0 C1 Meaning
//  0  0  0  0  +unsupported
//  0  0  0  1  -unsupported
//  0  0  1  0  +NAN
//  0  0  1  1  -NAN
//  0  1  0  0  +normal
//  0  1  0  1  -normal
//  0  1  1  0  +infinity
//  0  1  1  1  -infinity
//  1  0  0  0  +0
//  1  0  0  1  -0
//  1  0  1  0  +empty
//  1  0  1  1  -empty
//  1  1  0  0  +denormal
//  1  1  0  1  -denormal
typedef enum {
    SIGN = 1,
    ZERO = 2,
    NORMAL = 4,
    DENORMAL = 8,
    INFIN = 16,
    NANUM = 32,
    EMPTY = 64,
    UNSUPPORTED = 128,
    UNKNOWN = 256
}CDEXAMDOUBLE_T;//examine

//
// externals
//
extern size_t __cdeOnErrSet_status(size_t num);
extern char* _strefierror(size_t errcode);

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

//
//  NOTE: To simplify a unified DLLIMPORT interface generation, the interface type is void*
//          except for vfprintf(), that is already declared in CDE.H a full interface type
//          is needed.
// 
#if   defined(_M_AMD64)
#   define MKCDEABI(fn) void* __imp_##fn = (void*)fn##CDEABI/* CDE MAKE DLL IMPORT */
#else//   defined(_M_AMD64)
#   define MKCDEABI(fn) void* _imp__##fn = (void*)fn##CDEABI/* CDE MAKE DLL IMPORT */
#endif//  defined(_M_AMD64)

#endif//_CDE_SERVICES_H_
