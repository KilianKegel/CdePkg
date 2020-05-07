/*!
@copyright
    Copyright (c) 2020, Kilian Kegel. All rights reserved.<BR>
    SPDX-License-Identifier: BSD-2-Clause-Patent

@file main.c

@brief
    This module implements <em>POST diagnostic for TSC Time Stamp Counter</em>
@todo

@mainpage
    POST diagnostic for TSC Time Stamp Counter

@section intro_sec Introduction
    POST diagnostic for TSC Time Stamp Counter sample implementation

@subsection Drv_sec Driver and Application matrix
    <table>
                <tr>
                    <Th></th>
                    <Th>PREMem PEI</th>
                    <Th>PEI</th>
                    <Th>DXE</th>
                 </tr>
                <tr><th>NAME</th>
                    <TD>CdeDiagTSCSyncPRE.efi</td>
                    <TD>CdeDiagTSCSyncPei.efi</td>
                    <TD>CdeDiagTSCSyncDxe.efi</td>
                </tr>
                <tr><th>GUID</th>
                    <TD>CDE000FF-F7C7-4B2F-9250-ABD0FD57719C</td>
                    <TD>CDE000FF-8882-4391-B097-695F0C232946</td>
                    <TD>CDE000FF-13C6-4C8B-BF60-E8D1165E9D3B</td>
    </table>

@subsection ref_sec References
    <a href="http://biblioteca.museo8bits.es/chips/datasheets/MC146818A.pdf">RTC Real Time Clock - original</a>\n
    <a href="https://www.nxp.com/docs/en/data-sheet/MC146818.pdf">RTC Real Time Clock - NXP clone</a>\n
    <a href="https://www.mouser.com/datasheet/2/612/atom-e3800-family-datasheet-1522396.pdf#page=4540">RTC Real Time Clock in BayTrail</a>\n
    <a href="https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf">ANSI C Specification</a>\n

@subsection Parm_sec Command line parameters

    CdeDiagTSCSync [/bitbang] [/seccount num]

        /bitbang:   reprogram serial transmit pin TX on MinnowBoard to be GPIO and write
                    the time sync test strings via bit/bang, not by UART to the TX pin
                  
                    This demonstrates, that time is precise in &micro;s range simoultaniously in long time range,
                    using the TSC
        /seccount:  number of seconds to run the syncron test, e.g. 86400 to run an entire day
*/

#include <uefi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <CDE.h>
//#undef NULL

#define MILLISECSYNC 500
extern void _disable(void);
extern void _enable(void);
#pragma intrinsic (_disable, _enable)

static const size_t GPIOADDR = 0xFED0C530;

//bit bang character
int bbc(char c, unsigned long long time8680ns) {
    int i;
    /*volatile */unsigned long long tsc;
//    unsigned* pGPIO = (unsigned*)((unsigned long long)0xFED0C530ULL);
    unsigned* pGPIO = (unsigned*)GPIOADDR;
    //size_t eflags = __readeflags();
    char rgBuffer[] = { 0,1,1,1,1,1,1,1,1,1,1 }; /*1 Starbit := 0, 8 Databits, 1 Stoppbit := 1*/

    for (i = 1; i <= 8; i++)
        rgBuffer[i] = (c & (1 << (i - 1))) ? 1 : 0;

    _disable();

    for (i = 0; i < sizeof(rgBuffer); i++)
    {

        tsc = __rdtsc() + time8680ns;

        pGPIO[2] = 4 + rgBuffer[i];

        while (tsc > __rdtsc())
            ;
    }
    _enable();
    return 0;
}

//bit bang string
int bbs(char* pszSrc, unsigned long long time8680ns) {
    char c;
    
    while (*pszSrc) {
        c = *pszSrc++;
        if ('\n' == c) {
            bbc('\r', time8680ns);
            bbc(c, time8680ns);
        }
        else {
            bbc(c, time8680ns);
        }
    }
    return 0;
};

//set serial TX GPIO 
void SetTxGpioAtExit(void) {
//    unsigned* pGPIO = (unsigned*)((unsigned long long)0xFED0C530ULL);
    unsigned* pGPIO = (unsigned*)GPIOADDR;
    pGPIO[0] |= 1;
        CDEMOFINE((MFNINF(1) "UART TX GPIO reprogrammed for UART TX function\n"));
}

unsigned readRTC(int i) {
    outp(0x70, i);
    return inp(0x71);
}

static int bcd2int(int bcd) {
    int h, l;

    h = 0x0F & (bcd / 16);
    l = 0x0F & (bcd);
    return h * 10 + l;
}

int main(int argc, char** argv) {
    int nRet = 0,i;
    struct tm Tm, * pTm = &Tm;
    time_t  t;
    int oldsec = -1, num = 0;
    int rtcsec, rtcmin, rtchour, rtcmday, rtcmon, rtcyear;
    unsigned long long TSCTsc, RTCTsc;
    char strbuf[256];
    int SecCount = 10;  // default seconds count
    unsigned long long tsc, time8680ns = 0LL;
    clock_t clk;
//    unsigned* pGPIO = (unsigned*)((unsigned long long)0xFED0C530ULL);
    unsigned* pGPIO = (unsigned*)GPIOADDR;
    size_t eflags = __readeflags();
    int fBitBang = 0;

    CDEMOFINE((MFNINF(1) "##################################################################\n"));
    CDEMOFINE((MFNINF(1) "########################## CdePkg driver CdeDiagTSCSyncRTC %s %s\n", CDE_CONFIGURATION_STRING, CDE_PLATFORM_STRING));
    CDEMOFINE((MFNINF(1) "##################################################################\n"));
    
    do {
        //
        // check command line
        //
        for (i = 0; i < argc; i++) 
        {
            
            if (0 == strcmp("/bitbang", argv[i])) 
            {
                fBitBang = 1;

                _disable();
                clk = MILLISECSYNC + clock();
                
                tsc = __rdtsc();
                while (clk > clock())
                    ;
                tsc = __rdtsc() - tsc;

                tsc = (tsc * 1000) / MILLISECSYNC;

                if (0x200 & eflags)          // restore IF interrupt flag
                    _enable();
                time8680ns = tsc / 115200;
                CDEMOFINE((MFNINF(1) "TSC/Sec -> %lld, %lld\n", tsc, time8680ns));

                atexit(SetTxGpioAtExit);

                CDEMOFINE((MFNINF(1) "reprogram GPIO for native GPIO usage...\n"));


            }
            
            if (0 == strcmp("/seccount", argv[i])) {
                SecCount = (argc > (i + 1) ? atoi(argv[i + 1]) : 3);
                CDEMOFINE((MFNINF(1) "SecCount %d\n", SecCount));
                //CDEMOFINE((MFNINF(1) "%s\n", argv[i + 1]));
            }
            
            if (0 == strcmp("/?", argv[i]) || 0 == strcmp("/h", argv[i])) {
                CDEMOFINE((MFNINF(1) "CdeDiagTSCSync [/bitbang] [/seccount]\n"));
                break;
            }

        }
        
        do {

            t = time(NULL);                                 // get system time

            pTm = localtime(&t);
            TSCTsc = __rdtsc();

            if (oldsec != pTm->tm_sec) {
                oldsec = pTm->tm_sec;

                if (0x80 & readRTC(0x0A))
                    continue;                               // skip if update cycle is in progress
                rtcyear = bcd2int(readRTC(9));
                rtcyear += (100 * bcd2int(readRTC(0x32)));
                rtcmon = bcd2int(readRTC(8));
                rtcmday = bcd2int(readRTC(7));
                rtchour = bcd2int(readRTC(4));
                rtcmin = bcd2int(readRTC(2));
                rtcsec = bcd2int(readRTC(0));

                while (0 == (0x80 & readRTC(0x0A)))           // wait for update cycle
                    ;
                RTCTsc = __rdtsc();                         // get time diff RTCTsc vs. TSCTsc

                // do printf() while in update cycle
                snprintf(strbuf, sizeof(strbuf) - 2, "TSCTime: %d/%02d/%02d %02d:%02d,%02d vs. RTCTime: %04d/%02d/%02d %02d:%02d,%02d -> %5d sec, %lld, secdiff %d\n",
                    pTm->tm_year + 1900,
                    pTm->tm_mon + 1,
                    pTm->tm_mday,
                    pTm->tm_hour,
                    pTm->tm_min,
                    pTm->tm_sec,
                    rtcyear,
                    rtcmon,
                    rtcmday,
                    rtchour,
                    rtcmin,
                    rtcsec,
                    ++num,
                    RTCTsc - TSCTsc,
                    rtcsec - pTm->tm_sec
                );

                if(1 == fBitBang){

                    pGPIO[0] &= 0xFFFFFFFE;
                    pGPIO[2] = 4;


                    bbs("BitBang string -> ", time8680ns);
                    bbs(strbuf, time8680ns);
                }
                else {
                    CDEMOFINE((MFNINF(1) "%s", strbuf));
                }
                SecCount--;
            }
        } while (SecCount);
    } while (0);
    
    return nRet;
}
