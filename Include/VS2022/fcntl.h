/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2025, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    fcntl.h

Abstract:

    Implementation of the POSIX C header file for the Microsoft C Compiler VS2022
    https://pubs.opengroup.org/onlinepubs/000095399/basedefs/fcntl.h.html

Author:

    Kilian Kegel

--*/
#ifndef _CDE_FCNTL_H_
#define _CDE_FCNTL_H_

#ifndef CDEABI
#   define CDEABI __declspec(dllimport)
#endif//CDEABI

#define _O_RDONLY       0x0000  /* open for reading only */
#define _O_WRONLY       0x0001  /* open for writing only */
#define _O_RDWR         0x0002  /* open for reading and writing */
#define _O_APPEND       0x0008  /* writes done at fEof */

#define _O_CREAT        0x0100  /* create and open file */

/* O_TEXT files have <cr><lf> sequences translated to <lf> on read()'s,
** and <lf> sequences translated to <cr><lf> on write()'s
*/

#define _O_TEXT         0x4000  /* file mode is text (translated) */
#define _O_BINARY       0x8000  /* file mode is binary (untranslated) */

/* Temporary file bit - file is deleted when last handle is closed */

#define _O_TEMPORARY    0x0040  /* temporary file bit */

/* Non-ANSI names for compatibility */
#define O_RDONLY        _O_RDONLY
#define O_WRONLY        _O_WRONLY
#define O_RDWR          _O_RDWR
#define O_APPEND        _O_APPEND
#define O_CREAT         _O_CREAT
#define O_TEXT          _O_TEXT
#define O_BINARY        _O_BINARY
#define O_TEMPORARY     _O_TEMPORARY

#endif//_CDE_FCNTL_H_
