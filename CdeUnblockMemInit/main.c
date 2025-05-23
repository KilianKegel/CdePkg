/*!
@copyright
    Copyright (c) 2020-2025, Kilian Kegel. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

@file main.c

@brief C Development Environment (CDE) unblock MemoryInit.EFI on the MinnowBoard platform\n

@details For demonstration purpose only the MemoryInit.efi driver depex' on 
    CDE_PEI_TMPTEST0_GUID {0xCDE00055, 0xb0ff, 0x498b, { 0xb1, 0x7c, 0xed, 0xb3, 0xa0, 0x2e, 0x7f, 0x6e }}
    that is installed by this driver.

@todo

@mainpage
    CdeUnblockMemInit driver for MinnowBoard

@section intro_sec Introduction
For demonstration purpose only the MemoryInit.efi driver depex' on\n
    CDE_PEI_TMPTEST0_GUID {0xCDE00055, 0xb0ff, 0x498b, { 0xb1, 0x7c, 0xed, 0xb3, 0xa0, 0x2e, 0x7f, 0x6e }}\n
    that is installed by this driver.\n
    <b>ATTENTION</b>: CdeUnblockMemInit has to be the last driver in the list to be started as Cde-PRE-memory driver in the related flash file descriptor .FDF file\n
    The .FDF file is located here: overrides\edk2-platforms\PlatformPkg.fdf\n

@subsection Parm_sec Command line parameters
    1. no parameters required
*/
#include <CDE.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#undef NULL
#include <Uefi.h>
#include <PiPei.h>
#include <Pi\PiStatusCode.h>

extern char* strefierror(EFI_STATUS Status);

int main(int argc, char** argv)
{
    EFI_STATUS Status = 0LL;
    EFI_PEI_SERVICES** PeiServices = (void*)(argv[-1]);     //PeiServices is passed in argv[-1]
    //EFI_PEI_FILE_HANDLE FileHandle = (void*)(argv[-2]);   //FileHandle  is passed in argv[-2]
    static EFI_GUID gCdePeiTmpTest0Guid = CDE_PEI_TMPTEST0_GUID;
    static EFI_PEI_PPI_DESCRIPTOR CdeLoadOptionsPpiList = {/*Flags*/(EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),/**Guid*/&gCdePeiTmpTest0Guid, /*Ppi*/0 };

    //CDEMOFINE((MFNINF(1) "##################################################################\n"));
    //CDEMOFINE((MFNINF(1) "########################## CdeUnblockMemInitPRE()\n"));
    //CDEMOFINE((MFNINF(1) "##################################################################\n"));
    
    Status = (*PeiServices)->InstallPpi(PeiServices, &CdeLoadOptionsPpiList);
    Status &= 0xFFFFFFFFULL;
    //CDEMOFINE((MFNBAR(1) "====================> %s %llX\n", strefierror(Status), Status));

    return Status;
}
