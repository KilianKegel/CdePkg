/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    locale.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=216

Author:

    Kilian Kegel

--*/
#ifndef _CDE_LOCALE_H_
#define _CDE_LOCALE_H_

//#pragma warning( disable : 4996 4311 4312 4101)

// Locale categories
#define LC_ALL          0
#define LC_COLLATE      1
#define LC_CTYPE        2
#define LC_MONETARY     3
#define LC_NUMERIC      4
#define LC_TIME         5

#define LC_MIN          LC_ALL
#define LC_MAX          LC_TIME

struct lconv {
	char*    decimal_point;
	char*    thousands_sep;
	char*    grouping;
	char*    int_curr_symbol;
	char*    currency_symbol;
	char*    mon_decimal_point;
	char*    mon_thousands_sep;
	char*    mon_grouping;
	char*    positive_sign;
	char*    negative_sign;
	char     int_frac_digits;
	char     frac_digits;
	char     p_cs_precedes;
	char     p_sep_by_space;
	char     n_cs_precedes;
	char     n_sep_by_space;
	char     p_sign_posn;
	char     n_sign_posn;
	wchar_t* _W_decimal_point;
	wchar_t* _W_thousands_sep;
	wchar_t* _W_int_curr_symbol;
	wchar_t* _W_currency_symbol;
	wchar_t* _W_mon_decimal_point;
	wchar_t* _W_mon_thousands_sep;
	wchar_t* _W_positive_sign;
	wchar_t* _W_negative_sign;
};

struct lconv* localeconv( void );
char* setlocale( int category, char const* locale );

#endif//_CDE_LOCALE_H_
