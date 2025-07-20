/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    stdio.h

Abstract:

    Implementation of the Standard C header file for the Microsoft C Compiler VS2022
    http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf#page=274

Author:

    Kilian Kegel

--*/
#ifndef _CDE_STDIO_H_
#define _CDE_STDIO_H_

#ifndef CDEABI
#   define CDEABI __declspec(dllimport)
#endif//CDEABI

#include <stdarg.h>
#include <stddef.h>

#ifndef _FILE_DEFINED
    #define _FILE_DEFINED
    typedef struct _iobuf
    {
        void* _empty;
    } FILE;
#endif
typedef long long fpos_t;

FILE* __cdecl __acrt_iob_func(unsigned);

#define stdin  (__acrt_iob_func(0))
#define stdout (__acrt_iob_func(1))
#define stderr (__acrt_iob_func(2))

#define EOF    (-1)

#define _IOFBF 0x0000
#define _IOLBF 0x0040
#define _IONBF 0x0004

#define L_tmpnam   260 // _MAX_PATH

#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0


#define FILENAME_MAX    260
#define FOPEN_MAX       20
#define TMP_MAX         2147483647

#define BUFSIZ 512

#ifndef NULL
    #define NULL ((void *)0)
#endif//NULL

CDEABI int remove( const char *filename );
CDEABI int rename( const char *old, const char *new );
CDEABI FILE *tmpfile( void );
CDEABI char *tmpnam( char *s );
CDEABI int fclose( FILE *stream );
CDEABI int fflush( FILE *stream );
CDEABI FILE *fopen( const char * filename, const char * mode );
CDEABI FILE *freopen( const char * filename, const char * mode, FILE * stream );
CDEABI void setbuf( FILE * stream, char * buf );
CDEABI int setvbuf( FILE * stream, char * buf, int mode, size_t size );
CDEABI int fscanf( FILE * stream, const char * format, ... );
CDEABI int printf( const char * format, ... );
CDEABI int scanf( const char * format, ... );
CDEABI int snprintf( char * s, size_t n, const char * format, ... );        // C99, function is implemented
CDEABI int sprintf( char * s, const char * format, ... );
CDEABI int sscanf( const char * s, const char * format, ... );
CDEABI int vfprintf( FILE * stream, const char * format, va_list arg );
CDEABI int vprintf( const char * format, va_list arg );
CDEABI int vsprintf( char * s, const char * format, va_list arg ) ;
CDEABI int vfscanf( FILE * stream, const char * format, va_list arg );          // C99, function is implemented
CDEABI int vscanf( const char * format, va_list arg );                          // C99, function is implemented
CDEABI int vsnprintf( char * s, size_t n, const char * format, va_list arg );   // C99, function is implemented
CDEABI int vsscanf( const char * s, const char * format, va_list arg );         // C99, function is implemented
CDEABI int fgetc( FILE *stream );
CDEABI char *fgets( char * s, int n, FILE * stream );
CDEABI int fputc( int c, FILE *stream );
CDEABI int fputs( const char * s, FILE * stream );
CDEABI int getc( FILE *stream );
CDEABI int getchar( void );
CDEABI char *gets( char *s );
CDEABI int putc( int c, FILE *stream );
CDEABI int putchar( int c );
CDEABI int puts( const char *s );
CDEABI int ungetc( int c, FILE *stream );
CDEABI size_t fread( void * ptr, size_t size, size_t nmemb, FILE * stream );
CDEABI size_t fwrite( const void * ptr, size_t size, size_t nmemb, FILE * stream );
CDEABI int fgetpos( FILE * stream, fpos_t * pos );
CDEABI int fseek( FILE *stream, long int offset, int whence );
CDEABI int fsetpos( FILE *stream, const fpos_t *pos );
CDEABI long int ftell( FILE *stream );
CDEABI void rewind( FILE *stream );
CDEABI void clearerr( FILE *stream );
CDEABI int feof( FILE *stream );
CDEABI int ferror( FILE *stream );
CDEABI void perror( const char *s );

#ifndef _CDE_INLINE_FPRINTF_DEFINED
    CDEABI int fprintf(FILE* stream, const char* format, ...);
#endif//_CDE_INLINE_FPRINTF_DEFINED

#endif//_CDE_STDIO_H_
