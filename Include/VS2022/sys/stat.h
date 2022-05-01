/*++

    CdePkg for UEFI
    https://github.com/KilianKegel/CdePkg

    Copyright (c) 2017-2021, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:

    assert.h

Abstract:

    Implementation of the POSIX C header file for the Microsoft C Compiler VS2022
    https://pubs.opengroup.org/onlinepubs/009604599/basedefs/sys/stat.h.html

Author:

    Kilian Kegel

--*/
#ifndef _CDE_STAT_H_
#define _CDE_STAT_H_

#define _S_IFMT   0xF000 /* File type mask                          */
#define _S_IFDIR  0x4000 /* Directory                               */
#define _S_IFCHR  0x2000 /* Character special                       */
#define _S_IFIFO  0x1000 /* Pipe                                    */
#define _S_IFREG  0x8000 /* Regular                                 */
#define _S_IREAD  0x0100 /* Read permission, owner                  */
#define _S_IWRITE 0x0080 /* Write permission, owner                 */
#define _S_IEXEC  0x0040 /* Execute/search permission, owner        */

#if defined(_CRT_INTERNAL_NONSTDC_NAMES) && _CRT_INTERNAL_NONSTDC_NAMES
#define S_IFMT   _S_IFMT
#define S_IFDIR  _S_IFDIR
#define S_IFCHR  _S_IFCHR
#define S_IFREG  _S_IFREG
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC  _S_IEXEC
#endif

#endif//_CDE_STAT_H_
