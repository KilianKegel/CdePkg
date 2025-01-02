/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    CdeServicesSmm

Abstract:

	C Development Environment services driver for UEFI SMM

Author:

    Kilian Kegel

--*/
#define OS_EFI
#include <time.h>
typedef __int64                       __time64_t;
#include <CdeServices.h>
#include <cde.h>
#include <stdio.h>

#include <Protocol/smmbase2.h>
#include <Uefi/UefiSpec.h>
//#include <Pi/PiSmmCis.h>

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
extern OSIFMEMALLOC   _osifUefiSmmMemAlloc/*pMemAlloc*/;
extern OSIFMEMFREE    _osifUefiSmmMemFree/*pfREEPages*/;

extern char __cdeGetCurrentPrivilegeLevel(void);

extern EFI_GUID _gCdeSmmProtocolGuid;

//
// prototypes
//
static int _StdInGetChar(void** ppDest);
static void _StdOutPutChar(int c, void** ppDest);

//
// globals
//

EFI_GUID gEfiSmmStatusCodeProtocolGuid = { 0x6afd2b77, 0x98c1, 0x4acd, { 0xa6, 0xf9, 0x8a, 0x94, 0x39, 0xde, 0xf, 0xb1} };

//EFI_GUID gEfiCpuIoProtocolGuid = { 0xB0732526, 0x38C8, 0x4b40,{ 0x88, 0x77, 0x61, 0xC7, 0xB0, 0x6A, 0xAC, 0x45 } };
char* gEfiCallerBaseName = "CdeServicesSmm";

static CDE_SERVICES CdeServicesSmm = {
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
    _osifUefiSmmMemAlloc,//OSIFMEMALLOC* pMemAlloc;
    _osifUefiSmmMemFree,//OSIFMEMFREE* pMemFree;
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
            break;
        if (EOF == c)               // skip writing EOF that is send by printf() for flushing to stdout
            break;
        wcsSingle[0] = (wchar_t)c;
        wcsSingle[1] = '\0';

        pCdeAppIf->DriverParm.BsDriverParm.pSystemTable->ConOut->OutputString(pCdeAppIf->DriverParm.BsDriverParm.pSystemTable->ConOut, &wcsSingle[0]);
    } while (0);
}

EFI_STATUS EFIAPI _Main(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status, nRet = EFI_LOAD_ERROR;
    //
    // NOTE:    Sole purpose here for CdeAppIf here is to provide a "struct tm*" for osifIbmAtGetTime()
    //          when calling CdeServices.pGetTime(pCdeAppIf) below
    CDE_APP_IF* pCdeAppIf = &CdeAppIfDxe;
    EFI_SMM_SYSTEM_TABLE2* pSmmSystemTable2 = NULL;
    static EFI_GUID EfiSmmBase2ProtocolGuid = { 0xf4ccbfb7, 0xf6e0, 0x47fd, { 0x9d, 0xd4, 0x10, 0xa8, 0xf1, 0x50, 0xc1, 0x91 } };
    EFI_HANDLE NullHandle = 0;

    //__debugbreak();

    do {

        if (1)
        {
            EFI_SMM_BASE2_PROTOCOL* pSmmBase2 = NULL;

            Status = SystemTable->BootServices->LocateProtocol(
                &EfiSmmBase2ProtocolGuid,
                NULL,
                (VOID**)&pSmmBase2
            );

            if (EFI_SUCCESS != Status)
                break;
            //
            // We are in SMM, retrieve the pointer to SMM System Table
            //
            pSmmBase2->GetSmstLocation(pSmmBase2, &pSmmSystemTable2);
            //ASSERT(pSmmSystemTable2 != NULL);
            
            
            if (pSmmSystemTable2 == NULL)
                break;

            if (0 == pSmmBase2->InSmm)
                break;
        }
        //
        // locate the protocols needed to run CdeLib
        //
        if (0 != __cdeGetCurrentPrivilegeLevel())   // running in RING3/Emulation
            if (EFI_SUCCESS != SystemTable->BootServices->LocateProtocol(&gEfiSmmStatusCodeProtocolGuid, NULL, (void**)&CdeServicesSmm.ReportStatusCode.pSmm))
                break;

        //if ( EFI_SUCCESS != _cdegBS->LocateProtocol( &gEfiCpuIoProtocolGuid, NULL, (void**)&CdeServices.CpuIoXyz.pDxe ) )
        //	break;

        //
        // clock() initialization
        //
        CdeServicesSmm.TSClocksAtSystemStart = CdeServicesSmm.pGetTsc(pCdeAppIf);
        if (0 == __cdeGetCurrentPrivilegeLevel()) {                                      // running in RING0
            CdeServicesSmm.TSClocksPerSec = CdeServicesSmm.pGetTscPerSec(pCdeAppIf,0);
            CdeServicesSmm.TimeAtSystemStart = CdeServicesSmm.pGetTime(pCdeAppIf);
        }

        //
        // install the CdeServices for Smm
        //
        if (1)
        {
            Status = (EFI_STATUS)-1;
        }
        Status = pSmmSystemTable2->SmmInstallProtocolInterface(
            &NullHandle,                              // Image Handle
            &_gCdeSmmProtocolGuid,
            EFI_NATIVE_INTERFACE,
            &CdeServicesSmm
            );
        
        if (EFI_SUCCESS != Status)
            break;

        nRet = EFI_SUCCESS;

    } while (0);
    
    return nRet;
}
