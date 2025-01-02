/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    CdeServicesPei

Abstract:

    C Development Environment services driver for UEFI PEI

Author:

    Kilian Kegel

--*/

#define OS_EFI
#include <time.h>
typedef __int64                       __time64_t;
#undef NULL

#include <Pi\PiDxeCis.h>
#include <PiPei.h>
#include <Pi\PiHob.h>

#include <CdeServices.h>
#include <cde.h>
#include <stdio.h>
//
// externs
//
extern _PUTCHAR         _CdeDbgPutChar;
extern VWXPRINTF        _cdeCoreVwxPrintf;
extern VWXSCANF         _cdeCoreVwxScanf;
extern WCSSTRTOK        _cdeCoreWcsStrTok;
extern WCSSTRPBRKSPN    _cdeWcsStrPbrkSpn;
extern VOID EFIAPI      DebugPrint (IN UINTN ErrorLevel, IN CONST CHAR8  *Format, ...);
//extern CDE_APP_IF *     __cdeGetAppIf (void);
extern MEMREALLOC       _cdeCoreMemRealloc;
extern MEMSTRXCPY       _cdeMemStrxCpy;
extern MEMSTRXCMP       _cdeMemStrxCmp;
extern OSIFGETTIME		_osifIbmAtGetTime;
extern OSIFSETTIME		_osifIbmAtSetTime;
extern OSIFGETTSCPERSEC _osifIbmAtGetTscPerSec;
extern OSIFGETTSC       _osifIbmAtGetTsc;
extern OSIFMEMALLOC     _osifUefiPeiMemAlloc/*pMemAlloc*/;
extern OSIFMEMFREE      _osifUefiPeiMemFree/*pfREEPages*/;

extern char __cdeGetCurrentPrivilegeLevel(void);
extern __declspec(dllimport) void* memset(void* s, int c, size_t n);
extern __declspec(dllimport) void* memcpy(void* s, const void* s2, size_t n);   // CdeServices independant!!!
//
// globals
//
static EFI_GUID gCdePeiProtocolGuid = CDE_PEI_PROTOCOL_GUID;          // The GUID for the protocol
char* gEfiCallerBaseName = "CdeServicesPei";

static int _StdInGetChar(void** ppDest) {
    return EOF;
}

typedef struct _CDE_HOB {
    EFI_HOB_GUID_TYPE       HobGuidType;
    EFI_PEI_PPI_DESCRIPTOR  PpiDesc;
    CDE_SERVICES            CdeServices;
}CDE_HOB;

CDE_HOB CdeHobRomDflt = 
{

    {                                                   /*EFI_HOB_GUID_TYPE*/
        {                                           /*EFI_HOB_GENERIC_HEADER*/
            EFI_HOB_TYPE_GUID_EXTENSION,    // HobType
            sizeof(CDE_HOB),                // HobLength
            0                               // Reserved
        },
        {/*EFI_GUID Name*/
            0x43A80440, 0x0157, 0x471B, 0x95, 0x90, 0x31, 0xD0, 0xB0, 0xE8, 0x16, 0xB5,
        }
    },
    {                                                       /*.PpiDesc*/
        EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
        &gCdePeiProtocolGuid,               // Guid
        NULL                                // Ppi* <- INIT when copied to HOB*/
    },

    0,//unsigned short wVerMajor;
    0,//unsigned short wVerMinor;
    8 == sizeof(void*),//unsigned char fx64Opcode;
    0,//unsigned char fMemoryDiscovered;        // flag EfiPeiMemoryDiscovered, always true for DXE, SMM
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
    _osifUefiPeiMemAlloc,//OSIFMEMALLOC* pMemAlloc;
    _osifUefiPeiMemFree,//OSIFMEMFREE* pMemFree;
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

EFI_STATUS EFIAPI _Main(IN EFI_PEI_FILE_HANDLE* FfsHeader, IN const EFI_PEI_SERVICES** pPeiServices) {

    CDE_HOB* pCdeHob = &CdeHobRomDflt;
    EFI_STATUS Status = EFI_UNSUPPORTED;
    CDE_APP_IF CdeAppIfPei;                                     // needed for pGetTime() only once
    char *pZero = (char*)&CdeAppIfPei;
    void *MemoryDiscoveredPpi;
    static EFI_GUID EfiPeiMemoryDiscoveredPpiGuid = {0xf894643d, 0xc449, 0x42d1, {0x8e, 0xa8, 0x85, 0xbd, 0xd8, 0xc6, 0x5b, 0xde } };


    do {/*while(0)*/
        //
        // needed for pGetTime() only once, init to zero
        //
        memset(pZero, 0, sizeof(CdeAppIfPei));
        //
        // allocate HOB space to get a small amount of r/w memory in PEI for placing the CDE protocol into that
        //
        Status = (*pPeiServices)->CreateHob(pPeiServices, EFI_HOB_TYPE_GUID_EXTENSION, sizeof(CDE_HOB), &pCdeHob);

        if (EFI_ERROR(Status))
            break;
        //
        // initialize that memory / copy predefined settings from ROM to CAR cache as ram
        //
        memcpy(pCdeHob, &CdeHobRomDflt, sizeof(CdeHobRomDflt));
        
        //
        //
        //
        Status = (*pPeiServices)->LocatePpi (pPeiServices, &EfiPeiMemoryDiscoveredPpiGuid, 0, NULL, (void**) &MemoryDiscoveredPpi);
        
        if(EFI_SUCCESS == Status){
            pCdeHob->CdeServices.fMemoryDiscovered = 1;
        }else{
            pCdeHob->CdeServices.fMemoryDiscovered = 0;
        }

        pCdeHob->PpiDesc.Ppi = &pCdeHob->CdeServices;
        //
        // clock() initialization
        //
        pCdeHob->CdeServices.TSClocksAtSystemStart = pCdeHob->CdeServices.pGetTsc(NULL);

        if (0 == __cdeGetCurrentPrivilegeLevel()) {                                      // running in RING0
            pCdeHob->CdeServices.TSClocksPerSec = pCdeHob->CdeServices.pGetTscPerSec(NULL,0);
            pCdeHob->CdeServices.TimeAtSystemStart = pCdeHob->CdeServices.pGetTime(&CdeAppIfPei); // get time from RTC
        }

        //
        // set CpuIo protocol to CdeServices, the driver's instance
        //
        //pCdeHob->CdeServices.CpuIoXyz.pPei = (*pPeiServices)->CpuIo;
        //
        // set pReportStatusCode protocol to CdeServices, the driver's instance
        //
        pCdeHob->CdeServices.ReportStatusCode.pPei = (*pPeiServices)->ReportStatusCode;
        //
        // install the CdeServices for PEI
        //
        Status = (*pPeiServices)->InstallPpi(pPeiServices, &pCdeHob->PpiDesc);

    } while (0);

    return Status;
}