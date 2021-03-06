/*!
@copyright
    Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

@file CdeWelcomePei.c

@brief C Development Environment (CDE) Welcome sample driver that is controlled by\n
    command line / load options and blinks the D2 LED on the MinnowBoard.

@details The command line parameter are provided by the <em>CdeLoadOptionsPei</em> driver.\n
            Before <em>main</em> is invoked by the startup code (crt0), <em>CdeLoadOptionsPei</em>\n
            is queried for the command line that belong to that paricular FILE_GUID/EfiCallerIdGuid.\n\n
            In emulation mode the GPIO access is skipped, but the argc/argv can be observed too.

@todo

@mainpage
    CdeWelcomePei sample driver for MinnowBoard

@section intro_sec Introduction
    This sample program exhibits the usage of the Standard C Library usage in UEFI PEI POST driver.\n
    ANSI C requires that the function called at propram startup is named <b><em>int main(int argc, char **argv)</em></b>.\n
    That sample is running on the MinnowBoard, a BayTrail (BYT) open source PC motherboard.\n\n
    <b><em>CdeLib</em></b> is the Standard C Library implementation used in the <b><em>CdePkg</em></b>\n

@subsection ref_sec References
    <a href="https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf">ANSI C Specification</a>\n
    <a href="https://github.com/JoaquinConoBolillo/torito-C-Library/blob/master/implemented.md">functions supported in CdeLib</a>\n
    <a href="https://docs.microsoft.com/en-us/cpp/c-language/c-language-reference?view=vs-2020">Microsoft C Language Reference</a>\n
    <a href="https://minnowboard.org/compare-boards/">MinnowBoard</a>\n
    @image html StdCLibCover.jpg

@subsection Parm_sec Command line parameters
    1. no parameter: blink the D2 LED 10 times at 16Hz
    2. /frq x /count y: blinks the D2 LED y times at a frequency of xHz
*/
#pragma warning(disable:4101)
#pragma warning(disable:4189)
#pragma warning(disable:4456)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#undef NULL
#include <CDE.h>

#include <PiPei.h>
#include <Pi\PiStatusCode.h>

#ifndef NULL
    #define NULL ((void *)0)
#endif//NULL
#define BLINKCOUNT 5
#define BLINKFRQ   3

#define /*Gpio Dfx0 Pad Cfg           */gpio_dfx0           0x2170

extern char __cdeGetCurrentPrivilegeLevel(void);
typedef struct _DEBUGNFO {
    EFI_STATUS_CODE_DATA Header;
    unsigned  ErrorLevel;
    unsigned long long            parm[12];
    char            pszText[256];
}DEBUGNFO;

int main(int argc, char** argv)
{
    EFI_STATUS Status = EFI_SUCCESS;
    char buffer[64];
    volatile int* pGPIO_D2_LED_CTRL;        //access 32 bit of GPIO
    clock_t clk;
    int i;
    int blinkcount = BLINKCOUNT;
    int frq = BLINKFRQ;
    EFI_PEI_SERVICES** PeiServices = (void*)(argv[-1]);
    EFI_PEI_FILE_HANDLE       FileHandle = (void*)(argv[-2]);
    DEBUGNFO DbgNfo = {
                    sizeof(EFI_STATUS_CODE_DATA),
                    sizeof(DEBUGNFO) - sizeof(EFI_STATUS_CODE_DATA),
                    { 0x9A4E9246, 0xD553, 0x11D5, { 0x87, 0xE2, 0x00, 0x06, 0x29, 0x45, 0xC3, 0xB9 }},
                    0xFF,
                    {0,0,0,0,0,0,0,0,0,0,0,0},
                    ""
    };
    //__debugbreak();
    //
    // Say HELLO to the WORLD of UEFI using various interfaces
    //

    // Cde MOdule FIle liNE
    CDEMOFINE((MFNINF(1) "\n \
######################################################################""\n \
##### Welcome, to the jungle of PEI, FileHandle %p ##############""\n \
#######################################################################\n",FileHandle));


    // UEFI ReportStatusCode
    strcpy(DbgNfo.pszText, "REPORTSTATUSCODE: Welcome, to the jungle...\r\n");
    (*PeiServices)->ReportStatusCode(
        PeiServices,
        EFI_DEBUG_CODE,
        0,
        0,
        NULL,
        (void*)&DbgNfo);

    // ANSI C formatted print
    printf("PRINTF(over ReportStatusCode): Welcome, to the jungle...\n");	// console is not available during DXE, so you won't see it :-(
    //
    // get command line parameter "/frq 123" and "/count 13"
    //

    for (i = 0; i < argc; i++) {

        if (0 == strcmp("/frq", argv[i])) {
            frq = atoi(argv[i + 1]);
        }

        if (0 == strcmp("/count", argv[i])) {
            blinkcount = atoi(argv[i + 1]);
        }

    }

    snprintf(buffer, sizeof(buffer), "%d %s %s %s %s %s\n", argc, argv[0], argc > 1 ? argv[1] : "", argc > 2 ? argv[2] : "", argc > 3 ? argv[3] : "", argc > 4 ? argv[4] : "");
    CDEMOFINE((MFNINF(1) "%s\n", buffer));

    if (0 == __cdeGetCurrentPrivilegeLevel()) {/*don't do that in emulation mode*/
        //
        // get the GPIO base from the LPC device 0/1F/0 register 0x4C
        //
        //MINNOBOARD  ONLY unsigned gpiobase;                      // PCI 0/1F/0/4C[31:2]
        //MINNOBOARD  ONLY outpd(0xCF8, 0x80000000 + (0 << 16) + (0x1F << 11) + (0 << 8) + 0x4C);
        //MINNOBOARD  ONLY gpiobase = ~3 & inpd(0xCFC);

        //MINNOBOARD  ONLY pGPIO_D2_LED_CTRL = (void*)((size_t)gpiobase + gpio_dfx0 + 8 /* value offset */);

        for (i = blinkcount; i > -1; i--) {
            CDEMOFINE((MFNBAR(1) "."));
            clk = CLOCKS_PER_SEC / frq + clock();
            while (clk > clock())
                ;

            //MINNOBOARD  ONLY *pGPIO_D2_LED_CTRL ^= 1;
            //MINNOBOARD  ONLY printf("\b\b\b%3d", i);     // print remaining blinks
        }
        //MINNOBOARD  ONLY *pGPIO_D2_LED_CTRL = 0;         // LED ON when exit
    }
    CDEMOFINE((MFNBAR(1) "\n"));

    return Status;
}
