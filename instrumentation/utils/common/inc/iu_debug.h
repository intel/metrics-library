/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file iu_debug.h

@brief Instrumentation Utils debug macros and structures definitions.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>

#if defined( __cplusplus )
extern "C"
{
#endif

///////////////////////////////////////////////////////////////////////////////
// MACRO: IU_DEBUG_LOGS - enabling debug & info logs
//        The default logs settings are defined in g_IuLogsControl ->
//        (feel free to adjust it according to your needs)
//        Here is an example (all available logs with all details):
//           AssertEnable   = false (assert are printed, but without breakpoints)
//           LogLayerEnable = IU_DBG_LAYER_ALL,                   // MD & MDAPI & INSTR & IU & ML
//           LogLevel       = IU_DBG_SEV_DEBUG | IU_DBG_SHOW_ALL, // severity <= DEBUG
//
///////////////////////////////////////////////////////////////////////////////
#if defined( _DEBUG ) || defined( _RELEASE_INTERNAL )
    #define IU_DEBUG_LOGS 1
#else
// Uncomment following line to compile debug & info logs also in release build
// Note, that it is not needed for critical & error logs - they are always included
// #define IU_DEBUG_LOGS 1
#endif

///////////////////////////////////////////////////////////////////////////////
// MACRO: Log settings names in config files / registry
///////////////////////////////////////////////////////////////////////////////
#define IU_CONFIG_NAME_ASSERT_ENABLE    L"AssertEnable"
#define IU_CONFIG_NAME_LOG_LAYER_ENABLE L"DebugEnable"
#define IU_CONFIG_NAME_LOG_LEVEL        L"DebugLevel"

#if defined( _DEBUG )
    #define IU_CONFIG_DEFAULT_ASSERT_ENABLE    true
    #define IU_CONFIG_DEFAULT_LOG_LAYER_ENABLE IU_DBG_LAYER_ALL
    #define IU_CONFIG_DEFAULT_LOG_LEVEL        IU_DBG_SEV_ERROR | IU_DBG_SEV_CRITICAL | 0
#elif defined( _RELEASE_INTERNAL )
    #define IU_CONFIG_DEFAULT_ASSERT_ENABLE    false
    #define IU_CONFIG_DEFAULT_LOG_LAYER_ENABLE IU_DBG_LAYER_ALL
    #define IU_CONFIG_DEFAULT_LOG_LEVEL        IU_DBG_SEV_WARNING | IU_DBG_SEV_ERROR | IU_DBG_SEV_CRITICAL | IU_DBG_SHOW_ALL | IU_DBG_ALIGNED
#else // RELEASE
    #define IU_CONFIG_DEFAULT_ASSERT_ENABLE    false
    #define IU_CONFIG_DEFAULT_LOG_LAYER_ENABLE IU_DBG_LAYER_ALL
    #define IU_CONFIG_DEFAULT_LOG_LEVEL        IU_DBG_SEV_ERROR | IU_DBG_SHOW_TAG | IU_DBG_ALIGNED
#endif

///////////////////////////////////////////////////////////////////////////////
// MACRO: Local log settings constants and helpers
///////////////////////////////////////////////////////////////////////////////
#define IU_CONFIG_FILE_PATH_SIZE_MAX 260
#define IU_CONFIG_FILE_LINE_SIZE_MAX 100
#define IU_CONFIG_FILE_LINE_LAYOUT   L"\"%99l[^\"]\"=dword:%99ls\n" // input  - '"ExampleKey"=dword:FFFFFFFF'                              \
                                                                    // output - 'ExampleKey' and 'FFFFFFFF'                                \
                                                                    // "\     - if a quotation mark occurred, start an expression matching \
                                                                    // [^\"]  - continue matching the expression until the quotation mark reappears
#define IU_CONFIG_FILE_NAME          "iu.config"

///////////////////////////////////////////////////////////////////////////////
// MACRO: Default adapter id
///////////////////////////////////////////////////////////////////////////////
#define IU_ADAPTER_ID_UNKNOWN 0xFFFFFFFF

    ///////////////////////////////////////////////////////////////////////////////
    // FUNCTION: IuLogCheckLayer (compares an incoming layer to IU settings)
    ///////////////////////////////////////////////////////////////////////////////
    bool IuLogCheckLayer( const uint32_t layer );

    ///////////////////////////////////////////////////////////////////////////////
    // FUNCTION: IuLogCheckLevel (compares an incoming logLevel and layer to IU settings)
    ///////////////////////////////////////////////////////////////////////////////
    bool IuLogCheckLevel( const uint32_t dbgLevel, const uint32_t layer );

    ///////////////////////////////////////////////////////////////////////////////
    // FUNCTION: IuLogCheckShowMode (verifies if given showMode is enabled in the IU settings)
    ///////////////////////////////////////////////////////////////////////////////
    bool IuLogCheckShowMode( const uint32_t showMode );

    ///////////////////////////////////////////////////////////////////////////////
    // FUNCTION: IuLogGetSettings (to override default log settings)
    ///////////////////////////////////////////////////////////////////////////////
    void IuLogGetSettings( void* deviceContext );

    ///////////////////////////////////////////////////////////////////////////////
    // FUNCTION: __IuLogPrint (for internal use only)
    ///////////////////////////////////////////////////////////////////////////////
    void __IuLogPrint( const uint32_t adapterId, const char sevTag, const char* layerTag, const char* fncName, const char* formatString, ... );

    ///////////////////////////////////////////////////////////////////////////////
    // STRUCT: IU_LOGS_CONTROL
    ///////////////////////////////////////////////////////////////////////////////
    typedef struct IU_LOGS_CONTROL_REC
    {
        // enabling real asserts, not applicable to the related logs
        bool AssertEnable;

        // enabling debug output in selected layers (MDAPI/MD/INSTR/IU/ML)
        uint32_t LogLayerEnable;

        // selecting severity level of debug output
        // &
        // selecting which log fields are shown (tag, moduleName, functionName)
        uint32_t LogLevel;
    } IU_LOGS_CONTROL;
    extern IU_LOGS_CONTROL g_IuLogsControl;

///////////////////////////////////////////////////////////////////////////////
// MACROS: related to debug severity, shown fields, debug layers
///////////////////////////////////////////////////////////////////////////////
// severity
#define IU_DBG_ALL 0x000007FF
#define IU_DBG_OFF 0x00000000

#define IU_DBG_SEV_CRITICAL 0x00000001 // unexpected unrecoverable errors, used for asserts only
#define IU_DBG_SEV_ERROR    0x00000002 // unexpected recoverable errors
#define IU_DBG_SEV_WARNING  0x00000004 // expected errors & warnings
#define IU_DBG_SEV_INFO     0x00000008 // information messages
#define IU_DBG_SEV_DEBUG    0x00000010 // debug messages

// ML exclusive severity
#define IU_DBG_SEV_TRAITS  0x00000020 // traits messages
#define IU_DBG_SEV_ENTERED 0x00000040 // entered a function
#define IU_DBG_SEV_EXITING 0x00000080 // exiting a function
#define IU_DBG_SEV_INPUT   0x00000100 // input parameter
#define IU_DBG_SEV_OUTPUT  0x00000200 // output parameter
#define IU_DBG_SEV_CSV     0x00000400 // csv file dumping

#define IU_DBG_SEV_MASK 0x000007FF // severity mask

// if following flag is set, logs will be aligned in output
#define IU_DBG_ALIGNED 0x08000000

// shown fields
#define IU_DBG_SHOW_TAG       0x04000000
#define IU_DBG_SHOW_MODULE    0x02000000
#define IU_DBG_SHOW_FUNCTION  0x01000000
#define IU_DBG_SHOW_THREAD_ID 0x00800000 // Thread id is not shown in case of IU_DBG_SHOW_ALL
#define IU_DBG_SHOW_ALL       ( IU_DBG_SHOW_TAG | IU_DBG_SHOW_MODULE | IU_DBG_SHOW_FUNCTION )

// if following flag is set, logs will be additionally printed on console
#define IU_DBG_CONSOLE_DUMP 0x80000000

// if following flag is set, a new line sign will be inserted at the end of each log
#define IU_DBG_EOL 0x40000000

// if following flag is set, each debug log will be flushed
#define IU_DBG_CONSOLE_FLUSH 0x20000000

// if following flag is set, each debug log will be saved in the file
#define IU_DBG_FILE_DUMP 0x10000000

// layers
#define IU_DBG_LAYER_INSTR 0x00000001
#define IU_DBG_LAYER_MD    0x00000002
#define IU_DBG_LAYER_MDAPI 0x00000004
#define IU_DBG_LAYER_IU    0x00000008
#define IU_DBG_LAYER_ML    0x00000010
#define IU_DBG_LAYER_ALL   ( IU_DBG_LAYER_INSTR | IU_DBG_LAYER_MD | IU_DBG_LAYER_MDAPI | IU_DBG_LAYER_IU | IU_DBG_LAYER_ML )

///////////////////////////////////////////////////////////////////////////////
// MACROS: related to logs printing
///////////////////////////////////////////////////////////////////////////////
// error & warning logs are available in any driver
#define F_IU_DBG_SEV_CRITICAL( adapter, level, layer, ... ) \
    if( IuLogCheckLevel( level, layer ) )                   \
    __IuLogPrint( adapter, 'C', __VA_ARGS__ )
#define F_IU_DBG_SEV_ERROR( adapter, level, layer, ... ) \
    if( IuLogCheckLevel( level, layer ) )                \
    __IuLogPrint( adapter, 'E', __VA_ARGS__ )
#define F_IU_DBG_SEV_WARNING( adapter, level, layer, ... ) \
    if( IuLogCheckLevel( level, layer ) )                  \
    __IuLogPrint( adapter, 'W', __VA_ARGS__ )
// debug logs are not available in release driver
#if IU_DEBUG_LOGS
    #define F_IU_DBG_SEV_INFO( adapter, level, layer, ... ) \
        if( IuLogCheckLevel( level, layer ) )               \
        __IuLogPrint( adapter, 'I', __VA_ARGS__ )
    #define F_IU_DBG_SEV_DEBUG( adapter, level, layer, ... ) \
        if( IuLogCheckLevel( level, layer ) )                \
        __IuLogPrint( adapter, 'D', __VA_ARGS__ )
    #define F_IU_DBG_SEV_TRAITS( adapter, level, layer, ... ) \
        if( IuLogCheckLevel( level, layer ) )                 \
        __IuLogPrint( adapter, 'T', __VA_ARGS__ )
    #define F_IU_DBG_SEV_ENTERED( adapter, level, layer, ... ) \
        if( IuLogCheckLevel( level, layer ) )                  \
        __IuLogPrint( adapter, '>', __VA_ARGS__ )
    #define F_IU_DBG_SEV_EXITING( adapter, level, layer, ... ) \
        if( IuLogCheckLevel( level, layer ) )                  \
        __IuLogPrint( adapter, '<', __VA_ARGS__ )
    #define F_IU_DBG_SEV_INPUT( adapter, level, layer, ... ) \
        if( IuLogCheckLevel( level, layer ) )                \
        __IuLogPrint( adapter, '>', __VA_ARGS__ )
    #define F_IU_DBG_SEV_OUTPUT( adapter, level, layer, ... ) \
        if( IuLogCheckLevel( level, layer ) )                 \
        __IuLogPrint( adapter, '<', __VA_ARGS__ )
#else
    #define F_IU_DBG_SEV_INFO( adapter, level, layer, ... ) \
        (void) adapter;
    #define F_IU_DBG_SEV_DEBUG( adapter, level, layer, ... ) \
        (void) adapter;
    #define F_IU_DBG_SEV_TRAITS( adapter, level, layer, ... ) \
        (void) adapter;
    #define F_IU_DBG_SEV_ENTERED( adapter, level, layer, ... ) \
        (void) adapter;
    #define F_IU_DBG_SEV_EXITING( adapter, level, layer, ... ) \
        (void) adapter;
    #define F_IU_DBG_SEV_INPUT( adapter, level, layer, ... ) \
        (void) adapter;
    #define F_IU_DBG_SEV_OUTPUT( adapter, level, layer, ... ) \
        (void) adapter;
#endif

// generic
#define _IU_LOG( F, adapter, level, layer, tag, function, ... )          F( adapter, level, layer, tag, function, __VA_ARGS__ )
#define IU_DBG_PRINT_TAGGED( adapter, level, layer, tag, function, ... ) _IU_LOG( F_##level, adapter, level, layer, tag, function, __VA_ARGS__ )

///////////////////////////////////////////////////////////////////////////////
// compiler-dependent definitions for debug macros
// intrinsic functions to cause debug break
///////////////////////////////////////////////////////////////////////////////
#if defined( _MSC_VER )
    #define IU_DBG_BREAK __debugbreak()
#elif defined( __GNUC__ )
    #define IU_DBG_BREAK __builtin_trap()
#else
    #include <assert.h>
    #define IU_DBG_BREAK assert( false )
#endif

///////////////////////////////////////////////////////////////////////////////
// MACRO: IU_IS_ASSERT_ENABLED
///////////////////////////////////////////////////////////////////////////////
#define IU_IS_ASSERT_ENABLED ( g_IuLogsControl.AssertEnable )

///////////////////////////////////////////////////////////////////////////////
// MACRO: IU_ASSERT
///////////////////////////////////////////////////////////////////////////////
// generic
#if IU_DEBUG_LOGS
    #define IU_ASSERT_TAGGED( adapter, expr, layer, tag )                                                                                                   \
        if( !( expr ) )                                                                                                                                     \
        {                                                                                                                                                   \
            IU_DBG_PRINT_TAGGED( adapter, IU_DBG_SEV_CRITICAL, layer, tag, __FUNCTION__, "!!! ASSERTION FAILURE: !!! -> %s:%d", __FILE__, __LINE__ );       \
            IU_DBG_PRINT_TAGGED( adapter, IU_DBG_SEV_CRITICAL, layer, tag, __FUNCTION__, "!!! ASSERTION DETAILS: !!! -> %s(): (" #expr ")", __FUNCTION__ ); \
            if( IU_IS_ASSERT_ENABLED )                                                                                                                      \
            {                                                                                                                                               \
                IU_DBG_BREAK;                                                                                                                               \
            }                                                                                                                                               \
        }
#else
    #define IU_ASSERT_TAGGED( adapter, expr, layer, tag )
#endif

///////////////////////////////////////////////////////////////////////////////
// MACROS: print function enter/exit
///////////////////////////////////////////////////////////////////////////////
// generic
#define IU_DBG_FUNCTION_ENTER_TAGGED( adapter, level, layer, tag ) IU_DBG_PRINT_TAGGED( adapter, _##level, layer, tag, __FUNCTION__, "ENTERED >>>" )
#define IU_DBG_FUNCTION_EXIT_TAGGED( adapter, level, layer, tag )  IU_DBG_PRINT_TAGGED( adapter, _##level, layer, tag, __FUNCTION__, "EXITING <<<" );

///////////////////////////////////////////////////////////////////////////////
//  INSTRUMENTATION UTILS LAYER LOGS
///////////////////////////////////////////////////////////////////////////////
// Tag to prefix log messages with
#if !defined( IU_LOG_TAG )
    #define IU_LOG_TAG "[IU]"
#endif

#define IU_ASSERT( expr )                    IU_ASSERT_TAGGED( IU_ADAPTER_ID_UNKNOWN, expr, IU_DBG_LAYER_IU, IU_LOG_TAG )
#define IU_DBG_PRINT( level, ... )           IU_DBG_PRINT_TAGGED( IU_ADAPTER_ID_UNKNOWN, _##level, IU_DBG_LAYER_IU, IU_LOG_TAG, __FUNCTION__, __VA_ARGS__ )
#define IU_DBG_FUNCTION_ENTER( level )       IU_DBG_FUNCTION_ENTER_TAGGED( IU_ADAPTER_ID_UNKNOWN, _##level, IU_DBG_LAYER_IU, IU_LOG_TAG );
#define IU_DBG_FUNCTION_EXIT( level )        IU_DBG_FUNCTION_EXIT_TAGGED( IU_ADAPTER_ID_UNKNOWN, _##level, IU_DBG_LAYER_IU, IU_LOG_TAG );
#define IU_DBG_FUNCTION_INPUT( level, x )    IU_DBG_PRINT( _##level, "IN: %s = %#010x = %u", #x, x, x );
#define IU_DBG_FUNCTION_INPUT64( level, x )  IU_DBG_PRINT( _##level, "IN: %s = %#018llx = %llu", #x, x, x );
#define IU_DBG_FUNCTION_OUTPUT( level, x )   IU_DBG_PRINT( _##level, "OUT: %s = %#010x = %u", #x, x, x );
#define IU_DBG_FUNCTION_OUTPUT64( level, x ) IU_DBG_PRINT( _##level, "OUT: %s = %#018llx = %llu", #x, x, x );

///////////////////////////////////////////////////////////////////////////////
// MACROS: following macros are required for compatibility with compilers on
//         different systems.
//         Example: GCC doesn't allow concatenation with empty strings - has to be
//         '_##level' instead of '##level'.
///////////////////////////////////////////////////////////////////////////////
#define _IU_DBG_SEV_CRITICAL IU_DBG_SEV_CRITICAL
#define _IU_DBG_SEV_ERROR    IU_DBG_SEV_ERROR
#define _IU_DBG_SEV_WARNING  IU_DBG_SEV_WARNING
#define _IU_DBG_SEV_INFO     IU_DBG_SEV_INFO
#define _IU_DBG_SEV_DEBUG    IU_DBG_SEV_DEBUG
#define _IU_DBG_SEV_TRAITS   IU_DBG_SEV_TRAITS
#define _IU_DBG_SEV_ENTERED  IU_DBG_SEV_ENTERED
#define _IU_DBG_SEV_EXITING  IU_DBG_SEV_EXITING
#define _IU_DBG_SEV_INPUT    IU_DBG_SEV_INPUT
#define _IU_DBG_SEV_OUTPUT   IU_DBG_SEV_OUTPUT
#define _IU_DBG_CONSOLE_DUMP IU_DBG_CONSOLE_DUMP

#define __IU_DBG_SEV_CRITICAL IU_DBG_SEV_CRITICAL
#define __IU_DBG_SEV_ERROR    IU_DBG_SEV_ERROR
#define __IU_DBG_SEV_WARNING  IU_DBG_SEV_WARNING
#define __IU_DBG_SEV_INFO     IU_DBG_SEV_INFO
#define __IU_DBG_SEV_DEBUG    IU_DBG_SEV_DEBUG
#define __IU_DBG_SEV_TRAITS   IU_DBG_SEV_TRAITS
#define __IU_DBG_SEV_ENTERED  IU_DBG_SEV_ENTERED
#define __IU_DBG_SEV_EXITING  IU_DBG_SEV_EXITING
#define __IU_DBG_SEV_INPUT    IU_DBG_SEV_INPUT
#define __IU_DBG_SEV_OUTPUT   IU_DBG_SEV_OUTPUT
#define __IU_DBG_CONSOLE_DUMP IU_DBG_CONSOLE_DUMP

#define F__IU_DBG_SEV_CRITICAL( adapter, level, layer, ... ) F_IU_DBG_SEV_CRITICAL( adapter, level, layer, __VA_ARGS__ )
#define F__IU_DBG_SEV_ERROR( adapter, level, layer, ... )    F_IU_DBG_SEV_ERROR( adapter, level, layer, __VA_ARGS__ )
#define F__IU_DBG_SEV_WARNING( adapter, level, layer, ... )  F_IU_DBG_SEV_WARNING( adapter, level, layer, __VA_ARGS__ )
#define F__IU_DBG_SEV_INFO( adapter, level, layer, ... )     F_IU_DBG_SEV_INFO( adapter, level, layer, __VA_ARGS__ )
#define F__IU_DBG_SEV_DEBUG( adapter, level, layer, ... )    F_IU_DBG_SEV_DEBUG( adapter, level, layer, __VA_ARGS__ )
#define F__IU_DBG_SEV_TRAITS( adapter, level, layer, ... )   F_IU_DBG_SEV_TRAITS( adapter, level, layer, __VA_ARGS__ )
#define F__IU_DBG_SEV_ENTERED( adapter, level, layer, ... )  F_IU_DBG_SEV_ENTERED( adapter, level, layer, __VA_ARGS__ )
#define F__IU_DBG_SEV_EXITING( adapter, level, layer, ... )  F_IU_DBG_SEV_EXITING( adapter, level, layer, __VA_ARGS__ )
#define F__IU_DBG_SEV_INPUT( adapter, level, layer, ... )    F_IU_DBG_SEV_INPUT( adapter, level, layer, __VA_ARGS__ )
#define F__IU_DBG_SEV_OUTPUT( adapter, level, layer, ... )   F_IU_DBG_SEV_OUTPUT( adapter, level, layer, __VA_ARGS__ )
#define F__IU_DBG_CONSOLE_DUMP( adapter, level, layer, ... ) F_IU_DBG_SEV_DEBUG( adapter, level, layer, __VA_ARGS__ )

#define F___IU_DBG_SEV_CRITICAL( adapter, level, layer, ... ) F_IU_DBG_SEV_CRITICAL( adapter, level, layer, __VA_ARGS__ )
#define F___IU_DBG_SEV_ERROR( adapter, level, layer, ... )    F_IU_DBG_SEV_ERROR( adapter, level, layer, __VA_ARGS__ )
#define F___IU_DBG_SEV_WARNING( adapter, level, layer, ... )  F_IU_DBG_SEV_WARNING( adapter, level, layer, __VA_ARGS__ )
#define F___IU_DBG_SEV_INFO( adapter, level, layer, ... )     F_IU_DBG_SEV_INFO( adapter, level, layer, __VA_ARGS__ )
#define F___IU_DBG_SEV_DEBUG( adapter, level, layer, ... )    F_IU_DBG_SEV_DEBUG( adapter, level, layer, __VA_ARGS__ )
#define F___IU_DBG_SEV_TRAITS( adapter, level, layer, ... )   F_IU_DBG_SEV_TRAITS( adapter, level, layer, __VA_ARGS__ )
#define F___IU_DBG_SEV_ENTERED( adapter, level, layer, ... )  F_IU_DBG_SEV_ENTERED( adapter, level, layer, __VA_ARGS__ )
#define F___IU_DBG_SEV_EXITING( adapter, level, layer, ... )  F_IU_DBG_SEV_EXITING( adapter, level, layer, __VA_ARGS__ )
#define F___IU_DBG_SEV_INPUT( adapter, level, layer, ... )    F_IU_DBG_SEV_INPUT( adapter, level, layer, __VA_ARGS__ )
#define F___IU_DBG_SEV_OUTPUT( adapter, level, layer, ... )   F_IU_DBG_SEV_OUTPUT( adapter, level, layer, __VA_ARGS__ )
#define F___IU_DBG_CONSOLE_DUMP( adapter, level, layer, ... ) F_IU_DBG_SEV_DEBUG( adapter, level, layer, __VA_ARGS__ )

#if defined( __cplusplus )
}
#endif
