/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    float.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=209

Author:

    Kilian Kegel

--*/
#ifndef _CDE_FLOAT_H_
#define _CDE_FLOAT_H_

#ifndef CDEABI
#   define CDEABI __declspec(dllimport)
#endif//CDEABI

//#pragma warning( disable : 4996 4311 4312 4101)

#error FLOATING POINT SUPPORT STILL MISSING

#endif//_CDE_FLOAT_H_
