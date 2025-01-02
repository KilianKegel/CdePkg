/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    wchar.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=360

Author:

    Kilian Kegel

--*/
#ifndef _CDE_WCHAR_H_
#define _CDE_WCHAR_H_

#ifndef CDEABI
#   define CDEABI __declspec(dllimport)
#endif//CDEABI

#include <stddef.h>
#include <stdarg.h>
#include <time.h>

typedef unsigned short wint_t;

typedef struct _Mbstatet
{ // state of a multibyte translation
    unsigned long _Wchar;
    unsigned short _Byte, _State;
} _Mbstatet;

typedef _Mbstatet mbstate_t;

#ifndef NULL
    #define NULL ((void *)0)
#endif//NULL

#define WCHAR_MIN 0x0000
#define WCHAR_MAX 0xffff

#ifndef WEOF
    #define WEOF ((wint_t)(0xFFFF))
#endif//WEOF


CDEABI int fwprintf( FILE * stream, const wchar_t * format, ... );
CDEABI int fwscanf( FILE * stream, const wchar_t * format, ... );
CDEABI int wprintf( const wchar_t * format, ... );
CDEABI int wscanf( const wchar_t * format, ... );
CDEABI int swprintf( wchar_t * s, size_t n, const wchar_t * format, ... );
CDEABI int swscanf( const wchar_t * s, const wchar_t * format, ... );
CDEABI int vfwprintf( FILE * stream, const wchar_t * format, va_list arg );
CDEABI int vfwscanf(FILE* stream, const wchar_t* pwcsFormat, va_list ap);
CDEABI int vwprintf( const wchar_t * format, va_list arg );
CDEABI int vswprintf( wchar_t * s, size_t n, const wchar_t * format, va_list arg );
CDEABI wint_t fgetwc( FILE *stream );
CDEABI wchar_t *fgetws( wchar_t * s, int n, FILE * stream );
CDEABI wint_t fputwc( wchar_t c, FILE *stream );
CDEABI int fputws( const wchar_t * s, FILE * stream );
CDEABI wint_t getwc( FILE *stream );
CDEABI wint_t getwchar( void );
CDEABI wint_t putwc( wchar_t c, FILE *stream );
CDEABI wint_t putwchar( wchar_t c );
CDEABI wint_t ungetwc( wint_t c, FILE *stream );
CDEABI int fwide( FILE *stream, int mode );
//not yet implemented double wcstod( const wchar_t * nptr, wchar_t ** endptr );
CDEABI long wcstol( const wchar_t * nptr, wchar_t ** endptr, int base );
CDEABI unsigned long wcstoul( const wchar_t * nptr, wchar_t ** endptr, int base );
CDEABI long long wcstoll(const wchar_t* strSource,wchar_t** endptr,int base);
CDEABI unsigned long long wcstoull(const wchar_t* s, wchar_t** endptr, int base);
CDEABI wchar_t *wcscpy( wchar_t * s1, const wchar_t * s2 );
CDEABI wchar_t *wcsncpy( wchar_t * s1, const wchar_t * s2, size_t n );
CDEABI wchar_t *wcscat( wchar_t * s1, const wchar_t * s2 );
CDEABI wchar_t *wcsncat( wchar_t * s1, const wchar_t * s2, size_t n );
CDEABI int wcscmp( const wchar_t *s1, const wchar_t *s2 );
CDEABI int wcscoll( const wchar_t *s1, const wchar_t *s2 );
CDEABI int wcsncmp( const wchar_t *s1, const wchar_t *s2, size_t n );
CDEABI size_t wcsxfrm( wchar_t * s1, const wchar_t * s2, size_t n );
CDEABI wchar_t *wcschr( const wchar_t *s, wchar_t c );
CDEABI size_t wcscspn( const wchar_t *s1, const wchar_t *s2 );
CDEABI wchar_t *wcspbrk( const wchar_t *s1, const wchar_t *s2 );
CDEABI wchar_t *wcsrchr( const wchar_t *s, wchar_t c );
CDEABI size_t wcsspn( const wchar_t *s1, const wchar_t *s2 );
CDEABI wchar_t *wcsstr( const wchar_t *s1, const wchar_t *s2 );
CDEABI wchar_t *wcstok( wchar_t * s1, const wchar_t * s2, wchar_t ** ptr );
CDEABI size_t wcslen( const wchar_t *s );
CDEABI wchar_t *wmemchr( const wchar_t *s, wchar_t c, size_t n );
CDEABI int wmemcmp( const wchar_t *s1, const wchar_t *s2, size_t n );
CDEABI wchar_t *wmemcpy( wchar_t * s1, const wchar_t * s2, size_t n );
CDEABI wchar_t *wmemmove( wchar_t *s1, const wchar_t *s2, size_t n );
CDEABI wchar_t *wmemset( wchar_t *s, wchar_t c, size_t n );
CDEABI size_t wcsftime( wchar_t * s, size_t maxsize, const wchar_t * format, const struct tm * timeptr );
CDEABI wint_t btowc( int c );
CDEABI int wctob( wint_t c );
CDEABI int mbsinit( const mbstate_t *ps );
CDEABI size_t mbrlen(const char* s, size_t n, mbstate_t* ps);
CDEABI size_t mbrtowc( wchar_t * s1, const char * s2, size_t n, mbstate_t * ps );
CDEABI size_t wcrtomb(char* s1, wchar_t wc, mbstate_t* ps);
CDEABI size_t mbsrtowcs(wchar_t* s1, const char** s2, size_t n, mbstate_t* ps);
CDEABI size_t wcsrtombs( char * dst, const wchar_t ** src, size_t len, mbstate_t * ps );

#endif//_CDE_WCHAR_H_
