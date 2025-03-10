/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    stdint.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=267

Author:

    Kilian Kegel

--*/
#ifndef _CDE_STDINT_H_
#define _CDE_STDINT_H_

#if   defined(_M_AMD64)
#   define INTPTR_MIN       INT64_MIN
#   define INTPTR_MAX       INT64_MAX
#   define UINTPTR_MAX      UINT64_MAX
#else
#   define INTPTR_MIN       INT32_MIN
#   define INTPTR_MAX       INT32_MAX
#   define UINTPTR_MAX      UINT32_MAX
#endif

#ifndef SIZE_MAX
#   if defined(_M_AMD64)
#       define SIZE_MAX     0xffffffffffffffffui64
#   else
#       define SIZE_MAX     0xffffffffui32
#   endif
#endif

typedef signed char         int8_t;
typedef short               int16_t;
typedef int                 int32_t;
typedef long long           int64_t;
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

typedef int8_t              int_least8_t;
typedef int16_t             int_least16_t;
typedef int32_t             int_least32_t;
typedef int64_t             int_least64_t;
typedef uint8_t             uint_least8_t;
typedef uint16_t            uint_least16_t;
typedef uint32_t            uint_least32_t;
typedef uint64_t            uint_least64_t;

typedef int8_t              int_fast8_t;
typedef int32_t             int_fast16_t;
typedef int32_t             int_fast32_t;
typedef int64_t             int_fast64_t;
typedef uint8_t             uint_fast8_t;
typedef uint32_t            uint_fast16_t;
typedef uint32_t            uint_fast32_t;
typedef uint64_t            uint_fast64_t;

typedef int64_t             intmax_t;
typedef uint64_t            uintmax_t;

#define INT8_MIN            (-127i8 - 1)
#define INT16_MIN           (-32767i16 - 1)
#define INT32_MIN           (-2147483647i32 - 1)
#define INT64_MIN           (-9223372036854775807i64 - 1)
#define INT8_MAX            127i8
#define INT16_MAX           32767i16
#define INT32_MAX           2147483647i32
#define INT64_MAX           9223372036854775807i64
#define UINT8_MAX           0xffui8
#define UINT16_MAX          0xffffui16
#define UINT32_MAX          0xffffffffui32
#define UINT64_MAX          0xffffffffffffffffui64

#define INT_LEAST8_MIN      INT8_MIN
#define INT_LEAST16_MIN     INT16_MIN
#define INT_LEAST32_MIN     INT32_MIN
#define INT_LEAST64_MIN     INT64_MIN
#define INT_LEAST8_MAX      INT8_MAX
#define INT_LEAST16_MAX     INT16_MAX
#define INT_LEAST32_MAX     INT32_MAX
#define INT_LEAST64_MAX     INT64_MAX
#define UINT_LEAST8_MAX     UINT8_MAX
#define UINT_LEAST16_MAX    UINT16_MAX
#define UINT_LEAST32_MAX    UINT32_MAX
#define UINT_LEAST64_MAX    UINT64_MAX

#define INT_FAST8_MIN       INT8_MIN
#define INT_FAST16_MIN      INT32_MIN
#define INT_FAST32_MIN      INT32_MIN
#define INT_FAST64_MIN      INT64_MIN
#define INT_FAST8_MAX       INT8_MAX
#define INT_FAST16_MAX      INT32_MAX
#define INT_FAST32_MAX      INT32_MAX
#define INT_FAST64_MAX      INT64_MAX
#define UINT_FAST8_MAX      UINT8_MAX
#define UINT_FAST16_MAX     UINT32_MAX
#define UINT_FAST32_MAX     UINT32_MAX
#define UINT_FAST64_MAX     UINT64_MAX


#define INTMAX_MIN          INT64_MIN
#define INTMAX_MAX          INT64_MAX
#define UINTMAX_MAX         UINT64_MAX

#define PTRDIFF_MIN         INTPTR_MIN
#define PTRDIFF_MAX         INTPTR_MAX


#define SIG_ATOMIC_MIN      INT32_MIN
#define SIG_ATOMIC_MAX      INT32_MAX

#define WCHAR_MIN           0x0000
#define WCHAR_MAX           0xffff

#define WINT_MIN            0x0000
#define WINT_MAX            0xffff

#define INT8_C(x)           (x)
#define INT16_C(x)          (x)
#define INT32_C(x)          (x)
#define INT64_C(x)          (x ## LL)

#define UINT8_C(x)          (x)
#define UINT16_C(x)         (x)
#define UINT32_C(x)         (x ## U)
#define UINT64_C(x)         (x ## ULL)

#define INTMAX_C(x)         INT64_C(x)
#define UINTMAX_C(x)        UINT64_C(x)

#endif//_CDE_STDINT_H_
