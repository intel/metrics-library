/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file iu_debug.c

@brief Instrumentation Utils header with OS specific standard
       functions and macros.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

///////////////////////////////////////////////////////////////////////////////
//
// Description:
//     OS dependent standard functions.
//
///////////////////////////////////////////////////////////////////////////////
#if defined( __cplusplus )
extern "C"
{
#endif

    // Memory
    bool iu_zeromem( void* dest, size_t destSize );
    bool iu_memset( void* dest, int32_t value, size_t count );
    bool iu_memcpy_s( void* dest, size_t destSize, const void* src, size_t count );
    bool iu_memcmp( const void* ptr1, const void* ptr2, size_t bytesToCompare );

    // C-strings
    bool    iu_strcpy_s( char* destStr, size_t destSize, const char* srcStr );
    bool    iu_strncpy_s( char* destStr, size_t destSize, const char* srcStr, size_t count );
    bool    iu_strcat_s( char* destStr, size_t destSize, const char* srcStr );
    char*   iu_strtok_s( char* str, const char* delimiters, char** context );
    size_t  iu_strnlen_s( const char* str, size_t strSize );
    int32_t iu_strncmp( const char* str1, const char* str2, size_t count );

    // Wide-char C-strings
    bool    iu_wstrcat_s( wchar_t* destWstr, size_t destSize, const wchar_t* srcWstr );
    bool    iu_wstrncat_s( wchar_t* destWstr, size_t destSize, const wchar_t* srcWstr, size_t maxAppend );
    bool    iu_wstrncpy_s( wchar_t* destWstr, size_t destSize, const wchar_t* srcWstr, size_t count );
    size_t  iu_wstrnlen( const wchar_t* wstr, size_t wstrSize );
    int32_t iu_wstrncmp( const wchar_t* wstr1, const wchar_t* wstr2, size_t count );
    size_t  iu_wstrtombs_s( char* destStr, size_t destSize, const wchar_t* srcWstr, size_t count );

    // Printing
    int32_t iu_sprintf_s( char* destStr, size_t destSize, const char* format, ... );
    bool    iu_snprintf( char* str, size_t size, const char* format, ... );
    bool    iu_vsnprintf( char* str, size_t size, const char* format, va_list ap );

    // Debug printing
    void iu_log( const char* msg );
    void iu_printfln( const char* msg );

    // Files
    bool   iu_fopen_s( FILE** pFile, const char* filename, const char* mode );
    size_t iu_fread_s( void* buff, size_t buffSize, size_t elemSize, size_t count, FILE* stream );

#if defined( __cplusplus )
} // extern "C"
#endif
