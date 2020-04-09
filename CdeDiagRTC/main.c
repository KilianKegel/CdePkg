/*!
@copyright
    Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

@file main.c

@brief
    This module implements <em>POST diagnostic for RTC</em>
@todo

@mainpage
    POST diagnostic for RTC

@section intro_sec Introduction
    POST diagnostic for RTC sample implementation

@subsection Drv_sec Driver and Application matrix
    <table>
                <tr>
                    <Th></th>
                    <Th>PREMem PEI</th>
                    <Th>PEI</th>
                    <Th>DXE</th>
                 </tr>
                <tr><th>NAME</th>
                    <TD>CdeDiagRTCPRE.efi</td>
                    <TD>CdeDiagRTCPei.efi</td>
                    <TD>CdeDiagRTCDxe.efi</td>
                </tr>
                <tr><th>GUID</th>
                    <TD>CDE000FF-BFBF-4590-AA1B-A1775E872026</td>
                    <TD>CDE000FF-941D-4C60-8B7B-31013A16EEAB</td>
                    <TD>CDE000FF-BA6D-4707-BC83-F22C71F6C700</td>
                <tr>
                    <th>parameter</th>
                    <td colspan="3">/count <em>num</em> : test <em>num</em> ASCII characters</td></tr>
    </table>

@subsection ref_sec References
    <a href="http://biblioteca.museo8bits.es/chips/datasheets/MC146818A.pdf">RTC Real Time Clock - original</a>\n
    <a href="https://www.nxp.com/docs/en/data-sheet/MC146818.pdf">RTC Real Time Clock - NXP clone</a>\n
    <a href="https://www.mouser.com/datasheet/2/612/atom-e3800-family-datasheet-1522396.pdf#page=4540">RTC Real Time Clock in BayTrail</a>\n
    <a href="https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf">ANSI C Specification</a>\n

@subsection Parm_sec Command line parameters
    n/a


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CDE.h>

unsigned long long GetRTC(unsigned long long qwAddr) {
    
    outp(0x70, (int)qwAddr);
    return inp(0x71);
}

unsigned WriteString(char* pszLineOfText)
{

    CDEMOFINE((MFNBAR(1) pszLineOfText));

    return 0;
};
unsigned rtcrd(int i) {
    outp(0x70, i);
    return inp(0x71);
}

int main(int argc, char** argv) {
    int nRet = 0;
    char buffer[128];

    CDEMOFINE((MFNINF(1) "##################################################################\n"));
    CDEMOFINE((MFNINF(1) "########################## CdePkg driver CdeDiagRTC %s %s\n", CDE_CONFIGURATION_STRING, CDE_PLATFORM_STRING));
    CDEMOFINE((MFNINF(1) "##################################################################\n"));
    
    do {

        nRet = _CdeXDump((XDUMPPARM) { .reg = 0 }, 128, 0, GetRTC, WriteString, buffer, sizeof(buffer));

    } while (0);

    return nRet;
}