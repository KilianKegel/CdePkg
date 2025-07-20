/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    setjmp.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=255

Author:

    Kilian Kegel

--*/
#ifndef _CDE_SETJMP_H_
#define _CDE_SETJMP_H_

#ifndef CDEABI
#   define CDEABI __declspec(dllimport)
#endif//CDEABI

#ifdef jmp_buf
#undef jmp_buf
#endif//ndef jmp_buf
#if   defined(_M_AMD64)
    #define JMPBUF_DWORD_SIZE (256/4)   // internal register symbols not provided on C level, as microsoft does
#else
    #define JMPBUF_DWORD_SIZE (64/4)    // internal register symbols not provided on C level, as microsoft does
#endif

typedef unsigned jmp_buf[JMPBUF_DWORD_SIZE];

CDEABI int _setjmp( jmp_buf env );
CDEABI void longjmp( jmp_buf env, int val );

#ifndef _INC_SETJMPEX/* Microsoft */
#   define setjmp _setjmp
#endif

#endif//_CDE_SETJMP_H_

