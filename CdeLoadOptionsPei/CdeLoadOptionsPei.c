/*!
@copyright
    Copyright (c) 2020-2025, Kilian Kegel. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

@file CdeLoadOptionsPei.c

@brief C Development Environment (CDE) command line parameter driver in <em>CdePkg</em>\n

@details

@todo

@mainpage
    CdeLoadOptionsPei sample driver for MinnowBoard

@section intro_sec Introduction
    CdeLoadOptionsPei is an open source reference implementation to demonstrate\n
    how to provide command line paramaters to an UEFI POST CdeDriver.\n
    Here is just for demonstration purpose only the FILE_GUID/command line pair\n
    stored in an array (CommandLine)\n
    In a real implementation the command line parameter shall be stored in a\n
    (protected)NVRAM area that can be accessed and changed on a finalized system\n
    without updating the BIOS code.

@subsection ref_sec References
    <a href="https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf">ANSI C Specification</a>\n
    <a href="https://msdn.microsoft.com/en-us/library/a1y7w461.aspx">C Command-Line Arguments</a>\n
    @image html StdCLibCover.jpg
*/
#include <PiPei.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library\PeiServicesLib.h>
#include <CDE.h>

extern char __cdeGetCurrentPrivilegeLevel(void);

typedef struct _NVRAMCOMMANDLINE {
    int rejectStart;            //  1 -> suppress start of driver, even if registered with EFI_CALLER_ID_GUID. 0 -> start driver and pass command line to it
    char CommandLine[0];		/*  assigned command line includeing filename*/
}NVRAMCOMMANDLINE;

COMMANDLINE CommandLine[] = {
    /*
    edit this file to change command line for each CDE driver:
    https://github.com/MinnowWare/CdePkg/blob/master/Include/CdeLoadOptions.h
    */
    #include <CdeLoadOptions.h> 
};


char* GetLoadOptions(void* PeiDxeInterface, COMM_GUID* pEfiCallerIdGuid, char *pVarBuf) {

    if (0 != __cdeGetCurrentPrivilegeLevel()) {                                      // running in EMULATION
        int i;
        //__debugbreak();
        for (i = 0; i < sizeof(CommandLine) / sizeof(CommandLine[0]); i++) {

            if (pEfiCallerIdGuid->Data1 == CommandLine[i].EfiCallerIdGuid.Data1 &&
                pEfiCallerIdGuid->Data2 == CommandLine[i].EfiCallerIdGuid.Data2 &&
                pEfiCallerIdGuid->Data3 == CommandLine[i].EfiCallerIdGuid.Data3 &&
                pEfiCallerIdGuid->Data4[0] == CommandLine[i].EfiCallerIdGuid.Data4[0] &&
                pEfiCallerIdGuid->Data4[1] == CommandLine[i].EfiCallerIdGuid.Data4[1] &&
                pEfiCallerIdGuid->Data4[2] == CommandLine[i].EfiCallerIdGuid.Data4[2] &&
                pEfiCallerIdGuid->Data4[3] == CommandLine[i].EfiCallerIdGuid.Data4[3] &&
                pEfiCallerIdGuid->Data4[4] == CommandLine[i].EfiCallerIdGuid.Data4[4] &&
                pEfiCallerIdGuid->Data4[5] == CommandLine[i].EfiCallerIdGuid.Data4[5] &&
                pEfiCallerIdGuid->Data4[6] == CommandLine[i].EfiCallerIdGuid.Data4[6] &&
                pEfiCallerIdGuid->Data4[7] == CommandLine[i].EfiCallerIdGuid.Data4[7]
                )
                break;

        }
        return i == sizeof(CommandLine) / sizeof(CommandLine[0]) ? "unknownCdeDriverPei" : (CommandLine[i].rejectStart ? NULL : CommandLine[i].szCommandLine);
    }
    else {

        EFI_STATUS Status;
        NVRAMCOMMANDLINE* pNvram = (NVRAMCOMMANDLINE*)pVarBuf;
        UINTN Size = 128;
        EFI_PEI_READ_ONLY_VARIABLE2_PPI* VariablePpi;
        EFI_PEI_SERVICES** pPeiServices = PeiDxeInterface;
        static EFI_GUID   EfiPeiReadOnlyVariable2PpiGuid = { 0x2ab86ef5, 0xecb5, 0x4134, { 0xb5, 0x56, 0x38, 0x54, 0xca, 0x1f, 0xe1, 0xb4 } };

        Status = (*pPeiServices)->LocatePpi(pPeiServices, &EfiPeiReadOnlyVariable2PpiGuid, 0, NULL, (VOID**)&VariablePpi);
        
        if (EFI_SUCCESS == Status)
        {
            Status = VariablePpi->GetVariable(VariablePpi, L"CdeLoadOption", (EFI_GUID*)pEfiCallerIdGuid, NULL, &Size, pNvram);
        }

        return EFI_SUCCESS != Status ? "unknownCdeDriverPei" : (pNvram->rejectStart ? NULL : pNvram->CommandLine);
    }
}

EFI_STATUS EFIAPI _Main(IN EFI_PEI_FILE_HANDLE* FfsHeader, IN const EFI_PEI_SERVICES** pPeiServices)
{
    EFI_STATUS Status = EFI_LOAD_ERROR;
    static CDE_LOADOPTIONS_PROTOCOL gCdeLoadOptionsProtocol = { GetLoadOptions };
    static EFI_GUID gCdeLoadOptionsProtocolGuid = CDE_LOAD_OPTIONS_PROTOCOL_GUID;
    //PEIM-to-PEIM Interface PPI
    static EFI_PEI_PPI_DESCRIPTOR CdeLoadOptionsPpiList = {/*Flags*/(EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),/**Guid*/&gCdeLoadOptionsProtocolGuid, /*Ppi*/&gCdeLoadOptionsProtocol };

    //__debugbreak();
    //
    // install the CdeProtocol for PEI
    //
    Status = (*pPeiServices)->InstallPpi(pPeiServices, &CdeLoadOptionsPpiList);
    
//    sends(gEfiCallerBaseName), sends("\\"__FILE__"\\"__FUNCTION__"()\r\n");
    
        return Status;
}
