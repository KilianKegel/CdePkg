/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    CdeServicesDxe

Abstract:

    C Development Environment services driver for UEFI DXE

Author:

    Kilian Kegel

--*/

#define OS_EFI
#include <time.h>
typedef __int64                       __time64_t;
#undef NULL
#include <CdeServices.h>
#include <cde.h>
#include <stdio.h>

//
// externs
//
extern _PUTCHAR  _CdeDbgPutChar;
extern VWXPRINTF _cdeCoreVwxPrintf;
extern VWXSCANF _cdeCoreVwxScanf;
extern WCSSTRTOK _cdeCoreWcsStrTok;
extern WCSSTRPBRKSPN _cdeWcsStrPbrkSpn;
extern VOID EFIAPI DebugPrint ( IN UINTN ErrorLevel, IN CONST CHAR8  *Format, ... );
extern MEMREALLOC _cdeCoreMemRealloc;
extern MEMSTRXCPY _cdeMemStrxCpy;
extern MEMSTRXCMP _cdeMemStrxCmp;
extern OSIFGETTIME       _osifIbmAtGetTime;
extern OSIFSETTIME       osifIbmAtSetTime;
extern OSIFGETTSCPERSEC _osifIbmAtGetTscPerSec;
extern OSIFGETTSC       _osifIbmAtGetTsc;
extern OSIFMEMALLOC   _osifUefiDxeMemAlloc/*pMemAlloc*/;
extern OSIFMEMFREE    _osifUefiDxeMemFree/*pfREEPages*/;

extern char __cdeGetCurrentPrivilegeLevel(void);

extern EFI_GUID _gCdeDxeProtocolGuid;

//
// prototypes
//
static int _StdInGetChar(void** ppDest);
static void _StdOutPutChar(int c, void** ppDest);

//
// globals
//
EFI_SYSTEM_TABLE* _cdegST;                                          // Pointer to system table
EFI_BOOT_SERVICES* _cdegBS;                                         // Pointer to boot services
EFI_HANDLE _cdegImageHandle;                                        // image handel
EFI_GUID gEfiStatusCodeRuntimeProtocolGuid = { 0xD2B2B828, 0x0826, 0x48A7,{ 0xB3, 0xDF, 0x98, 0x3C, 0x00, 0x60, 0x24, 0xF0 } };
char* gEfiCallerBaseName = "CdeServicesDxe";

CDE_SERVICES CdeServices = {/*CDE_SERVICES*/
    0,//unsigned short wVerMajor;
    0,//unsigned short wVerMinor;
    8 == sizeof(void*),//unsigned char fx64Opcode;
    1,//unsigned char fMemoryDiscovered;        // flag EfiPeiMemoryDiscovered, always true for DXE, SMM
    { (void*)-1,ENDOFMEM,1,NULL,NULL,0,0,(void*)-1 },//HEAPDESC HeapStart;
    0,//unsigned long long TSClocksAtSystemStart;
    0,//unsigned long long TSClocksAtCdeTrace;
    2000000000,//unsigned long long TSClocksPerSec; assuming 2GHz
    0,//long long TimeAtSystemStart;    // epoch time / UNIX time / POSIX time at systemstart
    FALSE,//fCOM1Timeout
    NULL,//REPORT_STATUS_CODE ReportStatusCode;

    NULL,//void* pvEfiShellProtocol;
    NULL,//CDESYSTEMVOLUMES* pCdeSystemVolumes;    // system volumes for CdePkg
    NULL,//CRT0SERVICE* pCRT0Service;
    
    _StdInGetChar,//_GETCHAR* pGetConIn;
    _CdeDbgPutChar,//_PUTCHAR* pPutConOut;
    _CdeDbgPutChar,//_PUTCHAR* pPutDebug;
    _cdeCoreVwxPrintf,//VWXPRINTF* pVwxPrintf;
    _cdeCoreVwxScanf,//VWXSCANF* pVwxScanf;
    _cdeCoreMemRealloc,//MEMREALLOC* pMemRealloc;
    _cdeMemStrxCpy,//MEMSTRXCPY* pMemStrxCpy;
    _cdeMemStrxCmp,//MEMSTRXCMP* pMemStrxCmp;
    _cdeWcsStrPbrkSpn,//WCSSTRPBRKSPN* pWcsStrPbrkSpn;
    _cdeCoreWcsStrTok,//WCSSTRTOK* pWcsStrTok;

    NULL,//COREFILERW* pFReadCORE;                 // core fread()
    NULL,//COREFILERW* pFWriteCORE;                // core fwrite()
    NULL,//CORESETPOS* pFSetPosCORE;               // core fsetpos()
    NULL,//COREFFLUSH* pFFlushCORE;                // core fflush()    int _coreFflush(CDE_APP_IF* pCdeAppIf, FILE* stream)

    _osifIbmAtGetTime,//OSIFGETTIME* pGetTime;
    NULL,//OSIFSETTIME* pSetRtcTime;
    _osifIbmAtGetTscPerSec,//OSIFGETTSCPERSEC* pGetTscPerSec;
    _osifIbmAtGetTsc,//OSIFGETTSC* pGetTsc;
    _osifUefiDxeMemAlloc,//OSIFMEMALLOC* pMemAlloc;
    _osifUefiDxeMemFree,//OSIFMEMFREE* pMemFree;
    NULL,//OSIFFOPEN* pFopen;
    NULL,//OSIFFCLOSE* pFclose;
    NULL,//OSIFFREAD* pFread;
    NULL,//OSIFFWRITE* pFwrite;
    NULL,//OSIFFSETPOS* pFsetpos;
    NULL,//OSIFFDELETE* pFdelete;
    NULL,//OSIFFRENAME* pFrename;
    NULL,//OSIFFFINDALL* pFfindall;
    NULL,//OSIFFGETSTATUS* pFgetstatus;
    NULL,//OSIFDIRCREATE* pDirCreate;
    NULL,//OSIFDIRREMOVE* pDirRemove;
    NULL,//OSIFCMDEXEC* pCmdExec;
    NULL,//OSIFGETENV* pGetEnv;
    NULL,//OSIFGETDCWD* pGetDrvCwd;
};

static CDE_APP_IF CdeAppIfDxe;  // initialized to 0

void* __cdeGetAppIf(void) {
    return &CdeAppIfDxe;
}

static int _StdInGetChar(void** ppDest) {
    return EOF;
}

static void _StdOutPutChar(int c, void** ppDest) {
    CDE_APP_IF* pCdeAppIf = (CDE_APP_IF*)*ppDest;
    wchar_t wcsSingle[2];
    do {
        if (NULL == pCdeAppIf->DriverParm.BsDriverParm.pSystemTable->ConOut)
        {
            _CdeDbgPutChar(c, ppDest);
            break;// _CdeDbgPutChar(c, ppDest);
        }
        if (EOF == c)               // skip writing EOF that is send by printf() for flushing to stdout
            break;
        wcsSingle[0] = (wchar_t)c;
        wcsSingle[1] = '\0';

        pCdeAppIf->DriverParm.BsDriverParm.pSystemTable->ConOut->OutputString(pCdeAppIf->DriverParm.BsDriverParm.pSystemTable->ConOut, &wcsSingle[0]);
    } while (0);
}

EFI_STATUS EFIAPI _Main(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status, nRet = EFI_ABORTED;
    //
    // NOTE:    Sole purpose here for CdeAppIf here is to provide a "struct tm*" for _osifIbmAtGetTime()
    //          when calling CdeServices.pGetTime(pCdeAppIf) below
    CDE_APP_IF *pCdeAppIf = &CdeAppIfDxe;

    do {

        _cdegST = SystemTable;
        _cdegBS = SystemTable->BootServices;
        _cdegImageHandle = ImageHandle;

        //
        // locate the protocols needed to run CdeLib
        //
        if (0 != __cdeGetCurrentPrivilegeLevel())   // running in RING3/Emulation
            if (EFI_SUCCESS != _cdegBS->LocateProtocol(&gEfiStatusCodeRuntimeProtocolGuid, NULL, (void**)&CdeServices.ReportStatusCode.pDxe))
                break;

        //if ( EFI_SUCCESS != _cdegBS->LocateProtocol( &gEfiCpuIoProtocolGuid, NULL, (void**)&CdeServices.CpuIoXyz.pDxe ) )
        //	break;

        //
        // clock() initialization
        //
        CdeServices.TSClocksAtSystemStart = CdeServices.pGetTsc(pCdeAppIf);
        if (0 == __cdeGetCurrentPrivilegeLevel()) {                                      // running in RING0
            CdeServices.TSClocksPerSec = CdeServices.pGetTscPerSec(pCdeAppIf,0);
            CdeServices.TimeAtSystemStart = CdeServices.pGetTime(pCdeAppIf);
        }

        //
        // install the CdeServices for DXE
        //
        Status = _cdegBS->InstallMultipleProtocolInterfaces(
            &_cdegImageHandle,                              // Image Handle
            &_gCdeDxeProtocolGuid, &CdeServices,        // Guid / Protocol pair
            NULL                                        // End of list
        );

        if (EFI_SUCCESS != Status)
            break;

        nRet = EFI_SUCCESS;

    } while (0);
    
    return nRet;
}
