/*!
@copyright
	Copyright (c) 2020-2025, Kilian Kegel. All rights reserved.<BR>
	SPDX-License-Identifier: BSD-2-Clause-Patent

@file CdeLoadOptionsDxe.c

@brief C Development Environment (CDE) command line parameter driver in <em>CdePkg</em>\n

@details

@todo

@mainpage
	CdeLoadOptionsDxe sample driver for MinnowBoard

@section intro_sec Introduction
	CdeLoadOptionsDxe is an open source reference implementation to demonstrate\n
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
#include <uefi.h>
#include <CDE.h>
#include <library\debuglib.h>

extern char* strefierror(EFI_STATUS errcode);                           // Toro C extention according to strerror()
extern char __cdeGetCurrentPrivilegeLevel(void);

typedef struct _NVRAMCOMMANDLINE {
	int rejectStart;            //  1 -> suppress start of driver, even if registered with EFI_CALLER_ID_GUID. 0 -> start driver and pass command line to it
	char CommandLine[0];		/*  assigned command line includeing filename*/
}NVRAMCOMMANDLINE;


char* GetLoadOptions(void* PeiDxeInterface, COMM_GUID* pEfiCallerIdGuid, char* pVarBuf);	//prototype
CDE_LOADOPTIONS_PROTOCOL CdeLoadOptionsProtocol = { GetLoadOptions };

EFI_GUID gCdeLoadOptionsProtocolGuid = CDE_LOAD_OPTIONS_PROTOCOL_GUID;
EFI_SYSTEM_TABLE* gST;                              // UEFI: Pointer to system table
EFI_HANDLE        gImageHandle;                     // UEFI: image handel

//
// include commandlines / load options of all drivers, register additional drivers in "CdeLoadOptionsDxe.h"
//
COMMANDLINE CommandLine[] = {
	/*
	edit this file to change command line for each CDE driver:
	https://github.com/MinnowWare/CdePkg/blob/master/Include/CdeLoadOptions.h
	*/
	#include <CdeLoadOptions.h> 
};

char* GetLoadOptions(void* PeiDxeInterface, COMM_GUID* pEfiCallerIdGuid, char* pVarBuf) {
	EFI_SYSTEM_TABLE* SystemTable = PeiDxeInterface;
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
		return i == sizeof(CommandLine) / sizeof(CommandLine[0]) ? "unknownCdeDriverDxe" : (CommandLine[i].rejectStart ? NULL : CommandLine[i].szCommandLine);
	}
	else {

		EFI_STATUS Status;
		NVRAMCOMMANDLINE* pNvram = (NVRAMCOMMANDLINE*)pVarBuf;
		UINTN Size = 128;


		Status = SystemTable->RuntimeServices->GetVariable(L"CdeLoadOption", (EFI_GUID*)pEfiCallerIdGuid, NULL, &Size, pNvram);
		
		if (EFI_SUCCESS == Status) {
			//sndstr(DBGFILE "-->Status2 EFI_SUCCESS\r\n");
		}
		else {
			//sndstr(DBGFILE "-->Status2 EFI_FAIL\r\n");
		}

		return EFI_SUCCESS != Status ? "unknownCdeDriverDxe" : (pNvram->rejectStart ? NULL : pNvram->CommandLine);
	}
}

EFI_STATUS EFIAPI _Main(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
//int main(int argc, char **argv)
{
	EFI_STATUS Status = 0;
	//__debugbreak();

	Status = gST->BootServices->InstallMultipleProtocolInterfaces(
		&gImageHandle,                                           // Image Handle
		&gCdeLoadOptionsProtocolGuid, &CdeLoadOptionsProtocol,	// Guid / Protocol pair
		NULL                                                    // End of list
	);

	return EFI_SUCCESS;
}
