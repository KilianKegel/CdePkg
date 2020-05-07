/*!
@copyright
    Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

@file main.c

@brief
    This module implements <em>POST diagnostic for EFI variables</em>
@todo

@mainpage
    POST diagnostic for EFI variables

@section intro_sec Introduction
    POST diagnostic for EFI variables

@subsection Drv_sec Driver and Application matrix
    <table>
                <tr>
                    <Th></th>
                    <Th>PREMem PEI</th>
                    <Th>PEI</th>
                    <Th>DXE</th>
                 </tr>
                <tr><th>NAME</th>
                    <TD>CdeDiagEFIVARPRE.efi</td>
                    <TD>CdeDiagEFIVARPei.efi</td>
                    <TD>CdeDiagEFIVARDxe.efi</td>
                </tr>
                <tr><th>GUID</th>
                    <TD>CDE000FF-EFD8-40D9-88EA-FA81F7D28732</td>
                    <TD>CDE000FF-1D13-4973-86E6-FDE74E151DFA</td>
                    <TD>CDE000FF-C263-48F3-BD6A-2E524D9AE131</td>
    </table>

@subsection ref_sec References
    <a href="https://uefi.org/sites/default/files/resources/UEFI_Spec_2_8_final.pdf#page=302">Variable Services</a>\n
    <a href="https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf">ANSI C Specification</a>\n

@subsection Parm_sec Command line parameters
    n/a


*/

#include <uefi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <CDE.h>
//#undef NULL

int guidcmp(const void* pLeft, const void* pRight);//prototype

typedef struct _VARITEM {
    GUID guid;
    int size;
    wchar_t* pname;
}VARITEM;

int main(int argc, char** argv) {
    EFI_SYSTEM_TABLE* SystemTable = (EFI_SYSTEM_TABLE*)(argv[-1]);      //SystemTable is passed in argv[-1]
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_GUID guid;
    UINTN datasize;
    int nRet = 0;
    wchar_t VarName[128] = L"";
    size_t VarNameSize = sizeof(VarName);
    int num = 0, found = 0;
    char fPrint = 0;
    VARITEM* pVarList = NULL;
    int i = -1, x;

    CDEMOFINE((MFNINF(1) "##################################################################\n"));
    CDEMOFINE((MFNINF(1) "########################## CdePkg driver CdeDiagEFIVAR %s %s\n", CDE_CONFIGURATION_STRING, CDE_PLATFORM_STRING));
    CDEMOFINE((MFNINF(1) "##################################################################\n"));

    memset(&guid, 0, sizeof(guid));

    do {

        VarNameSize = sizeof(VarName);

        Status = SystemTable->RuntimeServices->GetNextVariableName(
            &VarNameSize,
            VarName,
            &guid
            );
        if ((Status == EFI_NOT_FOUND)) {
            if (0 == found) {
                CDEMOFINE((MFNBAR(1)"No \"CdeLoadOption\" variable found in NVRAM\n"));
            }
            break;
        }

        if ((Status != EFI_SUCCESS)) {
            CDEMOFINE((MFNBAR(1) __FILE__"(%d): ""num %d -> %s\n", __LINE__, num, strefierror(Status)));
            break;
        }

        num++;

        fPrint = 1;// !wcscmp(VarName, L"CdeLoadOption");
        found = found + fPrint;

        if (Status == EFI_SUCCESS && fPrint == 1) {
            VarName[VarNameSize] = '\0';

            datasize = 0;
            Status = SystemTable->RuntimeServices->GetVariable(
                VarName             /* IN CHAR16 * VariableName */,
                &guid               /* IN EFI_GUID * VendorGuid */,
                NULL                /* OUT UINT32 * Attributes OPTIONAL*/,
                &datasize           /* IN OUT UINTN * DataSize */,
                NULL                /* OUT VOID * Data OPTIONAL*/
                );

            //CDEMOFINE((MFNBAR(1)"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X, size %04lld, name \"%S\"\n", 
            //    guid.Data1,
            //    guid.Data2,
            //    guid.Data3,
            //    guid.Data4[0],
            //    guid.Data4[5],
            //    guid.Data4[6],
            //    guid.Data4[7],
            //    datasize, 
            //    VarName));

            i++;
            pVarList = realloc(pVarList, (i + 1) * sizeof(VARITEM));
            pVarList[i].guid = guid;
            //pVarList[i].guid.Data1 &= 0x7FFFFFFF;
            pVarList[i].size = (int)datasize;
            pVarList[i].pname = malloc(VarNameSize);
            wcscpy(pVarList[i].pname, VarName);


        }
    } while ((Status == EFI_SUCCESS) || (EFI_BUFFER_TOO_SMALL == Status));

//before sorting    for (x = 0; x <= i; x++)
//before sorting        CDEMOFINE((MFNBAR(1)"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X, size %4d, name \"%S\"\n",
//before sorting            pVarList[x].guid.Data1,
//before sorting            pVarList[x].guid.Data2,
//before sorting            pVarList[x].guid.Data3,
//before sorting            pVarList[x].guid.Data4[5],
//before sorting            pVarList[x].guid.Data4[6],
//before sorting            pVarList[x].guid.Data4[7],
//before sorting            pVarList[x].size,
//before sorting            pVarList[x].pname));

    qsort(pVarList, i + 1, sizeof(VARITEM), guidcmp);

    CDEMOFINE((MFNINF(1)"-->\n"));

    for (x = 0; x <= i; x++)
        CDEMOFINE((MFNBAR(1)"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X, size %4d, name \"%S\"\n",
            pVarList[x].guid.Data1,
            pVarList[x].guid.Data2,
            pVarList[x].guid.Data3,
            pVarList[x].guid.Data4[0],
            pVarList[x].guid.Data4[1],
            pVarList[x].guid.Data4[2],
            pVarList[x].guid.Data4[3],
            pVarList[x].guid.Data4[4],
            pVarList[x].guid.Data4[5],
            pVarList[x].guid.Data4[6],
            pVarList[x].guid.Data4[7],
            pVarList[x].size,
            pVarList[x].pname));
    return nRet;
}

int guidcmp(const void* pLeft, const void* pRight) {
    unsigned long long DataL[11], DataR[11];
    GUID* pguidL = (GUID*)pLeft;
    GUID* pguidR = (GUID*)pRight;
    int i;
    long long diff;
    int nRet = 0;

    DataL[0] = 0xFFFFFFFFULL & pguidL->Data1;
    DataL[1] = 0xFFFFFFFFULL & pguidL->Data2;
    DataL[2] = 0xFFFFFFFFULL & pguidL->Data3;
    DataL[3] = 0xFFFFFFFFULL & pguidL->Data4[0];
    DataL[4] = 0xFFFFFFFFULL & pguidL->Data4[1];
    DataL[5] = 0xFFFFFFFFULL & pguidL->Data4[2];
    DataL[6] = 0xFFFFFFFFULL & pguidL->Data4[3];
    DataL[7] = 0xFFFFFFFFULL & pguidL->Data4[4];
    DataL[8] = 0xFFFFFFFFULL & pguidL->Data4[5];
    DataL[9] = 0xFFFFFFFFULL & pguidL->Data4[6];
    DataL[10] =0xFFFFFFFFULL &  pguidL->Data4[7];

    DataR[0] = 0xFFFFFFFFULL & pguidR->Data1;
    DataR[1] = 0xFFFFFFFFULL & pguidR->Data2;
    DataR[2] = 0xFFFFFFFFULL & pguidR->Data3;
    DataR[3] = 0xFFFFFFFFULL & pguidR->Data4[0];
    DataR[4] = 0xFFFFFFFFULL & pguidR->Data4[1];
    DataR[5] = 0xFFFFFFFFULL & pguidR->Data4[2];
    DataR[6] = 0xFFFFFFFFULL & pguidR->Data4[3];
    DataR[7] = 0xFFFFFFFFULL & pguidR->Data4[4];
    DataR[8] = 0xFFFFFFFFULL & pguidR->Data4[5];
    DataR[9] = 0xFFFFFFFFULL & pguidR->Data4[6];
    DataR[10] =0xFFFFFFFFULL &  pguidR->Data4[7];

    for (i = 0, diff = 0; (0 == diff) && (i < sizeof(DataL) / sizeof(DataL[0])); i++) {
        diff = DataL[i] - DataR[i];
    }
        if (diff < 0)
            nRet = -1;
        if (diff > 0)
            nRet = +1;
        return nRet;
}