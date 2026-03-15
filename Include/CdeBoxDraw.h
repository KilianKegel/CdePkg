/*++

    toro C Library
    https://github.com/KilianKegel/toro-C-Library#toro-c-library-formerly-known-as-torito-c-library

    Copyright (c) 2017-2026, Kilian Kegel. All rights reserved.
    SPDX-License-Identifier: GNU General Public License v3.0

Module Name:

    CdeBoxDraw.h

Abstract:

    Symbolic definitions for box drawing characters, block elements, 
    geometric shapes and arrow shapes required by the UEFI specification.

Author:

    Kilian Kegel

--*/

//
// Defines for required EFI Unicode Box Draw characters
//
#define BOXDRAW_HORIZONTAL                  L'─'
#define BOXDRAW_VERTICAL                    L'│'
#define BOXDRAW_DOWN_RIGHT                  L'┌'
#define BOXDRAW_DOWN_LEFT                   L'┐'
#define BOXDRAW_UP_RIGHT                    L'└'
#define BOXDRAW_UP_LEFT                     L'┘'
#define BOXDRAW_VERTICAL_RIGHT              L'├'
#define BOXDRAW_VERTICAL_LEFT               L'┤'
#define BOXDRAW_DOWN_HORIZONTAL             L'┬'
#define BOXDRAW_UP_HORIZONTAL               L'┴'
#define BOXDRAW_VERTICAL_HORIZONTAL         L'┼'
#define BOXDRAW_DOUBLE_HORIZONTAL           L'═'
#define BOXDRAW_DOUBLE_VERTICAL             L'║'
#define BOXDRAW_DOWN_RIGHT_DOUBLE           L'╒'
#define BOXDRAW_DOWN_DOUBLE_RIGHT           L'╓'
#define BOXDRAW_DOUBLE_DOWN_RIGHT           L'╔'
#define BOXDRAW_DOWN_LEFT_DOUBLE            L'╕'
#define BOXDRAW_DOWN_DOUBLE_LEFT            L'╖'
#define BOXDRAW_DOUBLE_DOWN_LEFT            L'╗'
#define BOXDRAW_UP_RIGHT_DOUBLE             L'╘'
#define BOXDRAW_UP_DOUBLE_RIGHT             L'╙'
#define BOXDRAW_DOUBLE_UP_RIGHT             L'╚'
#define BOXDRAW_UP_LEFT_DOUBLE              L'╛'
#define BOXDRAW_UP_DOUBLE_LEFT              L'╜'
#define BOXDRAW_DOUBLE_UP_LEFT              L'╝'
#define BOXDRAW_VERTICAL_RIGHT_DOUBLE       L'╞'
#define BOXDRAW_VERTICAL_DOUBLE_RIGHT       L'╟'
#define BOXDRAW_DOUBLE_VERTICAL_RIGHT       L'╠'
#define BOXDRAW_VERTICAL_LEFT_DOUBLE        L'╡'
#define BOXDRAW_VERTICAL_DOUBLE_LEFT        L'╢'
#define BOXDRAW_DOUBLE_VERTICAL_LEFT        L'╣'
#define BOXDRAW_DOWN_HORIZONTAL_DOUBLE      L'╤'
#define BOXDRAW_DOWN_DOUBLE_HORIZONTAL      L'╥'
#define BOXDRAW_DOUBLE_DOWN_HORIZONTAL      L'╦'
#define BOXDRAW_UP_HORIZONTAL_DOUBLE        L'╧'
#define BOXDRAW_UP_DOUBLE_HORIZONTAL        L'╨'
#define BOXDRAW_DOUBLE_UP_HORIZONTAL        L'╩'
#define BOXDRAW_VERTICAL_HORIZONTAL_DOUBLE  L'╪'
#define BOXDRAW_VERTICAL_DOUBLE_HORIZONTAL  L'╫'
#define BOXDRAW_DOUBLE_VERTICAL_HORIZONTAL  L'╬'
//
// EFI Required Block Elements Code Chart
//
#define BLOCKELEMENT_FULL_BLOCK             L'█'
#define BLOCKELEMENT_LIGHT_SHADE            L'░'
//
// EFI Required Geometric Shapes Code Chart
//
#define GEOMETRICSHAPE_UP_TRIANGLE          L'▲'
#define GEOMETRICSHAPE_RIGHT_TRIANGLE       L'►'
#define GEOMETRICSHAPE_DOWN_TRIANGLE        L'▼'
#define GEOMETRICSHAPE_LEFT_TRIANGLE        L'◄'
//
// EFI Required Arrow shapes
//
#define ARROW_LEFT                          L'←'
#define ARROW_UP                            L'↑'
#define ARROW_RIGHT                         L'→'
#define ARROW_DOWN                          L'↓'