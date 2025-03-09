/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    math.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=224

Author:

    Kilian Kegel

--*/
#ifndef _CDE_MATH_H_
#define _CDE_MATH_H_

#ifndef CDEABI
#   define CDEABI __declspec(dllimport)
#endif//CDEABI

#define _HUGE_ENUF  1e+300
#define INFINITY   ((float)(_HUGE_ENUF))
#define HUGE_VAL   ((double)INFINITY)

CDEABI double acos(double x);
CDEABI double asin(double x);
CDEABI double atan(double x);
CDEABI double atan2(double y, double x);
CDEABI double cos(double x);
CDEABI double sin(double x);
CDEABI double tan(double x);
CDEABI double cosh(double x);
CDEABI double sinh(double x);
CDEABI double tanh(double x);
CDEABI double exp(double x);
CDEABI double frexp(double value, int* exp);
CDEABI double ldexp(double x, int exp);
CDEABI double log(double x);
CDEABI double log10(double x);
CDEABI double modf(double value, double* iptr);
CDEABI double pow(double x, double y);
CDEABI double sqrt(double x);
CDEABI double ceil(double x);
CDEABI double fabs(double x);
CDEABI double floor(double x);
CDEABI double fmod(double x, double y);

#endif//_CDE_MATH_H_
