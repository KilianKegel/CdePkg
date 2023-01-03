/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2023, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    inttypes.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=210

Author:

    Kilian Kegel

--*/
#ifndef _CDE_INTTYPES_H_
#define _CDE_INTTYPES_H_

intmax_t strtoimax(const char* strSource,char** endptr,int base);
intmax_t wcstoimax(const wchar_t* strSource,wchar_t** endptr,int base);
uintmax_t strtoumax(const char* strSource, char** endptr, int base);
uintmax_t wcstoumax(const wchar_t* strSource, wchar_t** endptr, int base);

#endif//_CDE_INTTYPES_H_
