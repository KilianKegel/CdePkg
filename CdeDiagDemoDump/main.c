/*!
@copyright
    Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

@file main.c

@brief
    This module implements <em>POST diagnostic for a Dump demonstration</em>
@todo

@mainpage
    POST diagnostic for a Dump demonstration

@section intro_sec Introduction
    POST diagnostic for a Dump demonstration sample implementation

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
                    <TD>CDE000FF-EFD8-40D9-88EA-FA81F7D28732</td>
                    <TD>CDE000FF-D820-49BE-A829-839FCF26AE63</td>
                    <TD>CDE000FF-C263-48F3-BD6A-2E524D9AE131</td>
    </table>

@subsection ref_sec References
    <a href="https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf">ANSI C Specification</a>\n

@subsection Parm_sec Command line parameters
    n/a


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CDE.h>

#define BSIZE 0x800

/*!
    @fn unsigned long long GetMem8(void *pAddr)

    @brief read a BYTE from a given memory location

    @details

    @param[in] *pAddr

    @return byte read
*/
#pragma warning (disable:4305)
unsigned long long GetMem8(unsigned long long pAddr)
{
    unsigned char nRet, * p = (unsigned char*)pAddr;
    nRet = 0xFF & *p;
    return nRet;

}

unsigned long long GetMem16(unsigned long long pAddr)
{
    unsigned short nRet, * p = (unsigned short*)pAddr;
    nRet = 0xFFFF & *p;
    return nRet;

}

unsigned long long GetMem32(unsigned long long pAddr)
{
    unsigned int nRet, * p = (unsigned int*)pAddr;
    nRet = 0xFFFFFFFF & *p;
    return nRet;

}

unsigned long long GetMem64(unsigned long long pAddr)
{
    unsigned long long nRet, * p = (unsigned long long*)pAddr;
    nRet = *p;
    return nRet;

}

/*!
    @fn static unsigned WriteString(IN char *pszLineOfText)

    @brief print a text line

    @details

    @param[in] *pszLineOfText : pointer to ZERO terminated text line

    @return 0
*/
unsigned WriteString(char* pszLineOfText)
{

    CDEMOFINE((MFNBAR(1) pszLineOfText));

    return 0;
};

int main(int argc, char** argv) {
    int nRet = 0,i,n;
    char* xbuffer = malloc(BSIZE);
    char tmpbuf[120];

    CDEMOFINE((MFNINF(1) "##################################################################\n"));
    CDEMOFINE((MFNINF(1) "########################## CdePkg driver CdeDiagDemoDump %s %s\n", CDE_CONFIGURATION_STRING, CDE_PLATFORM_STRING));
    CDEMOFINE((MFNINF(1) "##################################################################\n"));

    CDEMOFINE((MFNBAR(1) "NOTE: This driver just demonstrates how to configure the\n"));
    CDEMOFINE((MFNBAR(1) "      CdeXDump() function for diagnostic mem/io/pci/whatever \"dump\"\n"));

    do {

        for (i = 0; i < BSIZE; i++)
            xbuffer[i] = (char)i;

        n = _CdeXDump((XDUMPPARM) { .reg = 0, .bit.ElmSizeMin1 = 0, .bit.ElmsPerLine = 16, .bit.Pitch = 1 }, BSIZE, (unsigned long long)& xbuffer[0], GetMem8, WriteString, tmpbuf, 120);
        CDEMOFINE((MFNFAT(0 != n) "1! Dump failed due to buffer limitations: required size is %d\n", n));

        n = _CdeXDump((XDUMPPARM) { .reg = 0, .bit.ElmSizeMin1 = 1, .bit.ElmsPerLine = 8, .bit.Pitch = 0 }, BSIZE, (unsigned long long)& xbuffer[0], GetMem16, WriteString, tmpbuf, sizeof(tmpbuf));
        CDEMOFINE((MFNFAT(0 != n) "2. Dump failed due to buffer limitations: required size is %d\n", n));

        n = _CdeXDump((XDUMPPARM) { .reg = 0, .bit.ElmSizeMin1 = 7, .bit.ElmsPerLine = 1, .bit.Pitch = 24 }, BSIZE, (unsigned long long)& xbuffer[0], GetMem64, WriteString, tmpbuf, sizeof(tmpbuf));
        CDEMOFINE((MFNFAT(0 != n) "3. Dump failed due to buffer limitations: required size is %d\n", n));

        n = _CdeXDump((XDUMPPARM) { .reg = 0, .bit.ElmSizeMin1 = 7, .bit.ElmsPerLine = 1, .bit.Pitch = 24 }, BSIZE, (unsigned long long)& xbuffer[0], GetMem64, WriteString, tmpbuf, 3);
        CDEMOFINE((MFNFAT(0 != n) "4. Dump failed due to buffer limitations: required size is %d\n", n));

    } while (0);

    return nRet;
}