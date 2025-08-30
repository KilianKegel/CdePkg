/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    CdeCompilerCtrl.h

Abstract:

    Compiler specific adjustments for CDE

Author:

    Kilian Kegel

--*/
#ifndef _CDE_COMPILER_CTRL_H_
#define _CDE_COMPILER_CTRL_H_

#ifdef __cplusplus
#   define EXTRN extern "C"
#else
#   define EXTRN extern
#endif//__cplusplus


#ifdef __clang__
//
// clang compiler
//
#pragma clang diagnostic ignored "-Wcomment"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wbuiltin-requires-header"
#pragma clang diagnostic ignored "-Wmicrosoft-static-assert"
#pragma clang diagnostic ignored "-Wnonportable-include-path"
#pragma clang diagnostic ignored "-Wmacro-redefined"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Winitializer-overrides"
#pragma clang diagnostic ignored "-Wdangling-else"
#pragma clang diagnostic ignored "-Wvarargs"
#pragma clang diagnostic ignored "-Wunused-function"
//#pragma clang diagnostic ignored "-Wintel-compat"
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#pragma clang diagnostic ignored "-Wchar-subscripts"
#pragma clang diagnostic ignored "-Wincompatible-library-redeclaration"
#pragma clang diagnostic ignored "-Wdll-attribute-on-redeclaration"


#define __CDEINTERRUPT_DISABLE __asm__ volatile("cli");
#define __CDEINTERRUPT_ENABLE __asm__ volatile("sti");

//__INTRIN_INLINE void __outbyte(unsigned short p, unsigned char v)
//{
//    __asm__ __volatile__("outb %b[v], %w[p]" : : [p] "Nd" (p), [v] "a" (v));
//}
//
//__INTRIN_INLINE void __outword(unsigned short p, unsigned short v)
//{
//    __asm__ __volatile__("outw %w[v], %w[p]" : : [p] "Nd" (p), [v] "a" (v));
//}
//
//__INTRIN_INLINE void __outdword(unsigned short p, unsigned long v)
//{
//    __asm__ __volatile__("outl %k[v], %w[p]" : : [p] "Nd" (p), [v] "a" (v));
//}

#elif defined(_MSC_VER) 
//
// microsoft compiler
//
EXTRN void _disable(void);
EXTRN void _enable(void);

#pragma intrinsic (_disable, _enable)

#define __CDEINTERRUPT_DISABLE _disable();
#define __CDEINTERRUPT_ENABLE  _enable();

#endif//__clang__

//
// intrinsic replacements
//
#ifdef _WIN64
    EXTRN unsigned __int64 __cdeGetEFLAGS(void);
#else
    EXTRN unsigned int __cdeGetEFLAGS(void);
#endif
EXTRN unsigned char _cdeINByte(unsigned short p);
EXTRN unsigned short _cdeINWord(unsigned short p);
EXTRN unsigned long _cdeINDWord(unsigned short p);
EXTRN void _cdeOUTByte(unsigned short p, unsigned char v);
EXTRN void _cdeOUTWord(unsigned short p, unsigned short v);
EXTRN void _cdeOUTDWord(unsigned short p, unsigned long v);

#endif//_CDE_COMPILER_CTRL_H_
