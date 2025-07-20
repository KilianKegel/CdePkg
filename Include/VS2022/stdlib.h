/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    stdlib.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=318

Author:

    Kilian Kegel

--*/
#ifndef _CDE_STDLIB_H_
#define _CDE_STDLIB_H_

#ifndef CDEABI
#   define CDEABI __declspec(dllimport)
#endif//CDEABI

#include <stddef.h>

typedef struct _div_t
{
    int quot;
    int rem;
} div_t;

typedef struct _ldiv_t
{
    long quot;
    long rem;
} ldiv_t;

typedef struct _lldiv_t
{
    long long quot;
    long long rem;
} lldiv_t;  /* C99 */

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define RAND_MAX 0x7fff
#define MB_CUR_MAX 1 /* C locale only */

CDEABI double atof( const char *nptr );
CDEABI int atoi( const char *nptr );
CDEABI long int atol( const char *nptr );
CDEABI double strtod( const char * nptr, char ** endptr );
CDEABI long int strtol( const char * nptr, char ** endptr, int base );
CDEABI unsigned long int strtoul( const char * nptr, char ** endptr, int base );
CDEABI int rand( void );
CDEABI void srand( unsigned int seed );
CDEABI void *calloc( size_t nmemb, size_t size );
CDEABI void free( void *ptr );
CDEABI void *malloc( size_t size );
CDEABI void *realloc( void *ptr, size_t size );
CDEABI void abort( void );
CDEABI int atexit( void ( *func )( void ) );
CDEABI void exit( int status );
CDEABI char *getenv( const char *name );
CDEABI int system( const char *string );
CDEABI void *bsearch( const void *key, const void *base, size_t nmemb, size_t size, int ( *compar )( const void *, const void * ) );
CDEABI void qsort( void *base, size_t nmemb, size_t size, int ( *compar )( const void *, const void * ) );
CDEABI int abs( int j );
CDEABI long int labs( long int j );
CDEABI div_t div( int numer, int denom );
CDEABI ldiv_t ldiv( long int numer, long int denom );
CDEABI int mblen( const char *s, size_t n );
CDEABI int mbtowc( wchar_t * pwc, const char * s, size_t n );
CDEABI int wctomb( char *s, wchar_t wchar );
CDEABI size_t mbstowcs( wchar_t * pwcs, const char * s, size_t n );
CDEABI size_t wcstombs( char * s, const wchar_t * pwcs, size_t n );

CDEABI wchar_t* _ultow(unsigned long _Value, wchar_t* _Buffer, int _Radix);
CDEABI wchar_t* _ui64tow(unsigned long long _Value, wchar_t* _Buffer, int _Radix);
CDEABI char* _ultoa(unsigned long _Value, char* _Buffer, int _Radix);
CDEABI char* _ui64toa(unsigned long long _Value, char* _Buffer, int _Radix);
CDEABI wchar_t* _ltow(long _Value, wchar_t* _Buffer, int _Radix);
CDEABI char* _ltoa(long _Value, char* _Buffer, int _Radix);
CDEABI wchar_t* _itow(int _Value, wchar_t* _Buffer, int _Radix);
CDEABI char* _itoa(int _Value, char* _Buffer, int _Radix);
CDEABI wchar_t* _i64tow(long long _Value, wchar_t* _Buffer, int _Radix);
CDEABI char* _i64toa(long long _Value, char* _Buffer, int _Radix);

CDEABI long long int strtoll(const char* nptr, char** endptr, int base);
CDEABI unsigned long long int strtoull(const char* nptr,char** endptr,int base);

#endif//_CDE_STDLIB_H_
