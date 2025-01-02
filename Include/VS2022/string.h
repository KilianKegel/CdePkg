/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    string.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=337

Author:

    Kilian Kegel

--*/
#ifndef _CDE_STRING_H_
#define _CDE_STRING_H_

#ifndef CDEABI
#   define CDEABI __declspec(dllimport)
#endif//CDEABI

#include <stddef.h>

CDEABI void *memcpy( void * s1, const void * s2, size_t n );
CDEABI void *memmove( void *s1, const void *s2, size_t n );
CDEABI char *strcpy( char * s1, const char * s2 );
CDEABI char *strncpy( char * s1, const char * s2, size_t n );
CDEABI char *strcat( char * s1, const char * s2 );
CDEABI char *strncat( char * s1, const char * s2, size_t n );
CDEABI int memcmp( const void *s1, const void *s2, size_t n );
CDEABI int strcmp( const char *s1, const char *s2 );
CDEABI int strcoll( const char *s1, const char *s2 );
CDEABI int strncmp( const char *s1, const char *s2, size_t n );
CDEABI size_t strxfrm( char * s1, const char * s2, size_t n );
CDEABI void *memchr( const void *s, int c, size_t n );
CDEABI char *strchr( const char *s, int c );
CDEABI size_t strcspn( const char *s1, const char *s2 );
CDEABI char *strpbrk( const char *s1, const char *s2 );
CDEABI char *strrchr( const char *s, int c );
CDEABI size_t strspn( const char *s1, const char *s2 );
CDEABI char *strstr( const char *s1, const char *s2 );
CDEABI char *strtok( char * s1, const char * s2 );
CDEABI void *memset( void *s, int c, size_t n );
CDEABI char *strerror( int errnum );
CDEABI size_t strlen( const char *s );

#endif//_CDE_STRING_H_
