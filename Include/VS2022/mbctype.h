/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    mbctype.h

Abstract:

    Implementation of the Microsoft C header file for the Microsoft C Compiler VS2022

Author:

    Kilian Kegel

--*/
#ifndef _CDE_MBCTYPE_H_
#define _CDE_MBCTYPE_H_

#ifndef CDEABI
#   define CDEABI __declspec(dllimport)
#endif//CDEABI

CDEABI int _getmbcp(void);
CDEABI int _setmbcp(int codepage);

#endif//_CDE_MBCTYPE_H_
