/*!
@copyright
    Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

@file main.c

@brief
    This module implements <em>POST diagnostic for PCI</em>
@todo

@mainpage
    POST diagnostic for PCI

@section intro_sec Introduction
    POST diagnostic for PCI sample implementation

@subsection Drv_sec Driver and Application matrix
    <table>
                <tr>
                    <Th></th>
                    <Th>PREMem PEI</th>
                    <Th>PEI</th>
                    <Th>DXE</th>
                 </tr>
                <tr><th>NAME</th>
                    <TD>CdeDiagPCIPRE.efi</td>
                    <TD>CdeDiagPCIPei.efi</td>
                    <TD>CdeDiagPCIDxe.efi</td>
                </tr>
                <tr><th>GUID</th>
                    <TD>CDE000FF-4B2F-416B-99C1-6EF1EC221E81</td>
                    <TD>CDE000FF-7F36-43CE-84F6-066F22750640</td>
                    <TD>CDE000FF-CF70-4B97-83BA-88F49422717F</td>
                <tr>
                    <th>parameter</th>
                    <td colspan="3">/count <em>num</em> : test <em>num</em> ASCII characters</td></tr>
    </table>

@subsection ref_sec References
    <a href="https://www.mouser.com/datasheet/2/612/atom-e3800-family-datasheet-1522396.pdf#page=3179">PCI Express</a>\n
    <a href="https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf">ANSI C Specification</a>\n

@subsection Parm_sec Command line parameters
    n/a


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CDE.h>

/*!
    @fn unsigned long GetPci8(void *pAddr)

    @brief read a BYTE from a given PCI location

    @details

    @param[in] *pAddr

    @return byte read
*/
unsigned long long GetPci8(unsigned long long pAddr)
{
    unsigned long nRet;
    unsigned reg = (unsigned long long)pAddr & 0xFF;        // register
    unsigned bdf = (unsigned)((unsigned long long)pAddr >> 12);         // bus dev fun at CF8

    outpd(0xCF8, 0x80000000 + (bdf << 8) + (reg & ~3));;
    nRet = 0xFF & inp(0xCFC + (reg & 3));
    return nRet;

}

/*!
    @fn unsigned long GetPci16(void *pAddr)

    @brief read a WORD from a given PCI location

    @details

    @param[in] *pAddr

    @return word read
*/
unsigned long long GetPci16(unsigned long long pAddr)
{
    unsigned long nRet;
    unsigned reg = (unsigned long long)pAddr & 0xFF;        // register
    unsigned bdf = (unsigned)((unsigned long long)pAddr >> 12);         // bus dev fun at CF8

    outpd(0xCF8, 0x80000000 + (bdf << 8) + (reg & ~3));;
    nRet = 0xFFFF & inpw(0xCFC + (reg & 3));
    return nRet;

}

/*!
    @fn unsigned long GetPci32(void *pAddr)

    @brief read a DWORD from a given PCI location

    @details

    @param[in] *pAddr

    @return dword read
*/
unsigned long long GetPci32(unsigned long long pAddr)
{
    unsigned long nRet;
    unsigned reg = (unsigned long long)pAddr & 0xFF;        // register
    unsigned bdf = (unsigned)((unsigned long long)pAddr >> 12);         // bus dev fun at CF8

    outpd(0xCF8, 0x80000000 + (bdf << 8) + (reg & ~3));;
    nRet = 0xFFFFFFFF & inpd(0xCFC + (reg & 3));
    return nRet;

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
    CDEMOFINE((MFNINF(1) "########################## CdePkg driver CdeDiagPCI %s %s\n", CDE_CONFIGURATION_STRING, CDE_PLATFORM_STRING));
    CDEMOFINE((MFNINF(1) "##################################################################\n"));
    exit(3);

    do {

        int bus, dev, fun, bdf/* bus dev fun */, vendev, headertype;

        for (bus = 0; bus < 1; bus++)										    // scan all busses ...
            for (dev = 0; dev < 32; dev++)										// ...all devices on each bus..
                for (fun = 0; fun < 8; fun++) {									// ...and all functions on each device

                    bdf = (bus << 8) + (dev << 3) + (fun << 0);

                    vendev = (int)GetPci32((unsigned long long)bdf);

                    if ((-1 == vendev) || (0 == vendev))											// continue when no device found (0xFFFFFFFF)
                        continue;

                    CDEMOFINE((MFNINF(1) "Bus %02X, Dev %02X, Fun %02X:\n", bus, dev, fun));


                    nRet = _CdeXDump((XDUMPPARM) { .reg = 0, .bit.ElmsPerLine = 4, .bit.AddrSize = 3, .bit.ElmSizeMin1 = 3}, 256, bdf, GetPci32, WriteString, buffer, sizeof(buffer));

                    headertype = (int)GetPci32((__int64)(bdf + 0xC));						// check header type ...

                    headertype >>= 16;

                    if (fun == 0 && !(headertype & 0x80/*multi fun bit*/))		// ... if function 0 to be a multifuction device
                        break;//for(fun = 0 ; fun < 8 ; fun ++)					// ... continue with next device if not
                }

    } while (0);

    return nRet;
}