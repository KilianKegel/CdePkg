/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2024, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    ctype.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=193

Author:

    Kilian Kegel

--*/
#ifndef _CDE_CTYPE_H_
#define _CDE_CTYPE_H_

#ifndef CDEABI
#   define CDEABI __declspec(dllimport)
#endif//CDEABI

//#pragma warning( disable : 4996 4311 4312 4101)
CDEABI int isalnum( int );
CDEABI int isalpha( int );
CDEABI int iscntrl( int );
CDEABI int isdigit( int );
CDEABI int isgraph( int );
CDEABI int islower( int );
CDEABI int isprint( int );
CDEABI int ispunct( int );
CDEABI int isspace( int );
CDEABI int isupper( int );
CDEABI int isxdigit( int );
CDEABI int tolower( int );
CDEABI int toupper( int );

// Microsoft - specific/start
// Bit masks for the possible character types
#define _UPPER   0x01     // uppercase letter
#define _LOWER   0x02     // lowercase letter
#define _DIGIT   0x04     // digit[0-9]
#define _SPACE   0x08     // tab, carriage return, newline, vertical tab, or form feed
#define _PUNCT   0x10     // punctuation character
#define _CONTROL 0x20     // control character
#define _BLANK   0x40     // space char (tab is handled separately)
#define _HEX     0x80     // hexadecimal digit

#define _LEADBYTE 0x8000                    // multibyte leadbyte
#define _ALPHA   (0x0100 | _UPPER | _LOWER) // alphabetic character

// Microsoft - specific/end

#endif//_CDE_CTYPE_H_
