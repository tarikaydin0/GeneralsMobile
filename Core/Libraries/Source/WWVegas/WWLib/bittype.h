/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* $Header: /G/wwlib/bittype.h 4     4/02/99 1:37p Eric_c $ */
/***************************************************************************
 ***                  Confidential - Westwood Studios                    ***
 ***************************************************************************
 *                                                                         *
 *                 Project Name : Voxel Technology                         *
 *                                                                         *
 *                    File Name : BITTYPE.h                                *
 *                                                                         *
 *                   Programmer : Greg Hjelstrom                           *
 *                                                                         *
 *                   Start Date : 02/24/97                                 *
 *                                                                         *
 *                  Last Update : February 24, 1997 [GH]                   *
 *                                                                         *
 *-------------------------------------------------------------------------*
 * Functions:                                                              *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

typedef unsigned char	uint8;
typedef unsigned short	uint16;
#if defined(__ANDROID__) || defined(__linux__)
#include <stdint.h>
typedef uint32_t	uint32;
#else
typedef unsigned long	uint32;
#endif
typedef unsigned int    uint;

typedef signed char		sint8;
typedef signed short		sint16;
#if defined(__ANDROID__) || defined(__linux__)
typedef int32_t		sint32;
#else
typedef signed long		sint32;
#endif
typedef signed int      sint;

typedef float				float32;
typedef double				float64;

#if defined(__ANDROID__) || defined(__linux__)
typedef uint32_t   DWORD;
#else
typedef unsigned long   DWORD;
#endif
typedef unsigned short	WORD;
typedef unsigned char   BYTE;
typedef int             BOOL;
typedef unsigned short	USHORT;
typedef const char *		LPCSTR;
typedef unsigned int    UINT;
#if defined(__ANDROID__) || defined(__linux__)
typedef uint32_t   ULONG;
#else
typedef unsigned long   ULONG;
#endif

#if defined(_MSC_VER) && _MSC_VER < 1300
#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif
#endif

#if defined(__ANDROID__) || defined(__linux__)
#include <stdint.h>
#ifndef _WINDOWS_
#ifndef _WINDOWS_H
#ifndef _WINDOWS_H_
#ifndef LONG
typedef int32_t LONG;
#endif
#endif
#endif
#endif
#ifndef INT
typedef int INT;
#endif
#ifndef HWND
typedef void* HWND;
#endif
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} GUID;
#endif

#ifndef LPGUID
typedef GUID* LPGUID;
#endif

#ifndef REFGUID
#ifdef __cplusplus
#define REFGUID const GUID &
#else
#define REFGUID const GUID * const
#endif
#endif

#ifndef LPCGUID
typedef const GUID* LPCGUID;
#endif
#endif
