/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file iu_debug.c

@brief Instrumentation Utils common debug routines and structures.
*/

#include "iu_debug.h"
#include "iu_std.h"
#include "iu_os.h"

#include <stdarg.h>

///////////////////////////////////////////////////////////////////////////////
// GLOBAL VAR: g_IuLogsControl - default debug logs settings
//
// LogLevel.severity - for debug & info logs don't forget to enable
///////////////////////////////////////////////////////////////////////////////
IU_LOGS_CONTROL g_IuLogsControl = {
    IU_CONFIG_DEFAULT_ASSERT_ENABLE,
    IU_CONFIG_DEFAULT_LOG_LAYER_ENABLE,
    IU_CONFIG_DEFAULT_LOG_LEVEL,
};

///////////////////////////////////////////////////////////////////////////////
// FUNCTION: IuLogCheckLayer
///////////////////////////////////////////////////////////////////////////////
bool IuLogCheckLayer( const uint32_t layer )
{
    return g_IuLogsControl.LogLayerEnable & layer;
}

///////////////////////////////////////////////////////////////////////////////
// FUNCTION: IuLogCheckLevel
///////////////////////////////////////////////////////////////////////////////
bool IuLogCheckLevel(
    const uint32_t dbgLevel,
    const uint32_t layer )
{
    uint32_t dbgLevelSet = 0;

    const bool isLayerEnabled = IuLogCheckLayer( layer );

    if( !isLayerEnabled )
    {
        return false;
    }

    dbgLevelSet = g_IuLogsControl.LogLevel;
    if( ( dbgLevel & IU_DBG_CONSOLE_DUMP ) || ( dbgLevelSet & dbgLevel & IU_DBG_SEV_MASK ) )
    {
        return true;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
// FUNCTION: IuLogCheckShowMode
///////////////////////////////////////////////////////////////////////////////
bool IuLogCheckShowMode(
    const uint32_t showMode )
{
    return ( g_IuLogsControl.LogLevel & showMode );
}

///////////////////////////////////////////////////////////////////////////////
// Function: __IuLogGetModuleInfo
///////////////////////////////////////////////////////////////////////////////
static const char* __IuLogGetModuleInfo()
{
#if _RELEASE

    return IuOsGetModuleInfo( NULL );

#else

    static const char* const IU_EMPTY_MODULE_INFO = "";
    static const char*       pModuleInfo          = NULL;

    if( !pModuleInfo )
    {
        static char moduleInfo[IU_MODULE_NAME_SIZE_MAX] = { 0 };
        const char* dlName                              = NULL;
        char*       processName                         = NULL;

        iu_zeromem( moduleInfo, sizeof( moduleInfo ) );

        dlName = IuOsGetModuleInfo( &processName );

        if( ( processName ) && ( ( iu_strnlen_s( moduleInfo, IU_MODULE_NAME_SIZE_MAX ) + iu_strnlen_s( processName, IU_MODULE_NAME_SIZE_MAX ) + 1 ) // 1 => '\0'
                                 < IU_MODULE_NAME_SIZE_MAX ) )
        {
            iu_strcat_s( moduleInfo, sizeof( moduleInfo ), processName );
        }

        if( ( dlName ) && ( ( iu_strnlen_s( moduleInfo, IU_MODULE_NAME_SIZE_MAX ) + iu_strnlen_s( dlName, IU_MODULE_NAME_SIZE_MAX ) + 3 ) // 3 => '(' + ')' + '\0'
                            < IU_MODULE_NAME_SIZE_MAX ) )
        {
            iu_strcat_s( moduleInfo, sizeof( moduleInfo ), "(" );
            iu_strcat_s( moduleInfo, sizeof( moduleInfo ), dlName );
            iu_strcat_s( moduleInfo, sizeof( moduleInfo ), ")" );
        }

        if( iu_strnlen_s( moduleInfo, IU_MODULE_NAME_SIZE_MAX ) )
        {
            pModuleInfo = moduleInfo;
        }
        else
        {
            pModuleInfo = IU_EMPTY_MODULE_INFO;
        }
    }

    return pModuleInfo;

#endif
}

///////////////////////////////////////////////////////////////////////////////
// Function:       __IuLogPrint
//
// Description:    Outputs debug information.
///////////////////////////////////////////////////////////////////////////////
void __IuLogPrint(
    const uint32_t adapterId,
    const char     sevTag,
    const char*    layerTag,
    const char*    fncName, // this parameter isn't used in release driver
    const char*    inFormat,
    ... )
{
#define IU_BUF_SIZE    512
#define IU_FORMAT_SIZE 512

#define IU_TAG_ALIGNMENT      -7
#define IU_MODULE_ALIGNMENT   -14
#define IU_FUNCTION_ALIGNMENT -50

    va_list ap;

    char   buf[IU_BUF_SIZE];
    char   outFormat[IU_FORMAT_SIZE];
    size_t outFormatOffset = 0;

    if( g_IuLogsControl.LogLevel & IU_DBG_SHOW_THREAD_ID )
    {
        iu_snprintf( outFormat + outFormatOffset, IU_FORMAT_SIZE, "[ThreadId:%u]", iu_get_thread_id() );
        outFormatOffset = iu_strnlen_s( outFormat, sizeof( outFormat ) );
    }

    // adapter Id
    if( adapterId == IU_ADAPTER_ID_UNKNOWN )
    {
        iu_snprintf( outFormat + outFormatOffset, IU_FORMAT_SIZE, "(A?)" );
    }
    else
    {
        iu_snprintf( outFormat + outFormatOffset, IU_FORMAT_SIZE, "(A%u)", adapterId );
    }

    // severity tag
    outFormatOffset = iu_strnlen_s( outFormat, sizeof( outFormat ) );
    iu_snprintf( outFormat + outFormatOffset, IU_FORMAT_SIZE, ":%c", sevTag );

    // layer tag
    if( g_IuLogsControl.LogLevel & IU_DBG_SHOW_TAG )
    {
        int32_t tagAlignment = ( g_IuLogsControl.LogLevel & IU_DBG_ALIGNED ) ? IU_TAG_ALIGNMENT : 0;
        outFormatOffset      = iu_strnlen_s( outFormat, sizeof( outFormat ) );
        iu_snprintf( outFormat + outFormatOffset, IU_FORMAT_SIZE - outFormatOffset, ":%*s", tagAlignment, layerTag );
    }

    // module name
    if( g_IuLogsControl.LogLevel & IU_DBG_SHOW_MODULE )
    {
        const char* moduleInfo = __IuLogGetModuleInfo();
        if( moduleInfo && moduleInfo[0] != '\0' )
        {
            int32_t moduleAlignment = ( g_IuLogsControl.LogLevel & IU_DBG_ALIGNED ) ? IU_MODULE_ALIGNMENT : 0;
            outFormatOffset         = iu_strnlen_s( outFormat, sizeof( outFormat ) );
            iu_snprintf( outFormat + outFormatOffset, IU_FORMAT_SIZE - outFormatOffset, ":%*s", moduleAlignment, moduleInfo );
        }
    }

#if IU_DEBUG_LOGS // function names strings cannot be compiled into a release drv
    // function name
    if( g_IuLogsControl.LogLevel & IU_DBG_SHOW_FUNCTION )
    {
        int32_t functionAlignment = ( g_IuLogsControl.LogLevel & IU_DBG_ALIGNED ) ? IU_FUNCTION_ALIGNMENT : 0;
        outFormatOffset           = iu_strnlen_s( outFormat, sizeof( outFormat ) );

        if( functionAlignment )
        {
            // TODO: The '*s' printf parameter doesn't work in KMD drv - used a hardcoded version instead
            // iu_snprintf(outFormat+outFormatOffset, IU_FORMAT_SIZE - outFormatOffset, ":%*s", functionAlignment, fncName);
            iu_snprintf( outFormat + outFormatOffset, IU_FORMAT_SIZE - outFormatOffset, ":%-50s", fncName );
        }
        else
        {
            iu_snprintf( outFormat + outFormatOffset, IU_FORMAT_SIZE - outFormatOffset, ":%s", fncName );
        }
    }
#else
    (void) fncName;
#endif

    // log body
    outFormatOffset = iu_strnlen_s( outFormat, sizeof( outFormat ) );
    iu_snprintf( outFormat + outFormatOffset, IU_FORMAT_SIZE - outFormatOffset, ": %s ", inFormat );

    // new line sign
    if( g_IuLogsControl.LogLevel & IU_DBG_EOL )
    {
        outFormatOffset = iu_strnlen_s( outFormat, sizeof( outFormat ) );
        iu_snprintf( outFormat + outFormatOffset, IU_BUF_SIZE - outFormatOffset, "\n" );
    }

    va_start( ap, inFormat );
    iu_vsnprintf( buf, IU_BUF_SIZE, outFormat, ap );
    va_end( ap );

    bool dumpToFile = ( g_IuLogsControl.LogLevel & IU_DBG_FILE_DUMP );
    if( dumpToFile )
    {
        dumpToFile = iu_log_file( buf );
    }

    if( !dumpToFile )
    {
        iu_log( buf );

        if( ( g_IuLogsControl.LogLevel & IU_DBG_CONSOLE_DUMP ) )
        {
            iu_printf( buf, !( g_IuLogsControl.LogLevel & IU_DBG_EOL ), ( g_IuLogsControl.LogLevel & IU_DBG_CONSOLE_FLUSH ) );
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Function:    IuLogGetSettings
//
// Description: Overrides default (complied) log settings
///////////////////////////////////////////////////////////////////////////////
void IuLogGetSettings(
    void* deviceContext )
{
    IuOsLogGetSystemSettings(
        deviceContext,
        &g_IuLogsControl.AssertEnable,
        &g_IuLogsControl.LogLayerEnable,
        &g_IuLogsControl.LogLevel );

    IuOsLogGetLocalSettings(
        &g_IuLogsControl.AssertEnable,
        &g_IuLogsControl.LogLayerEnable,
        &g_IuLogsControl.LogLevel );

    IU_DBG_PRINT( IU_DBG_SEV_INFO, "&g_IuLogsControl = %#010x", g_IuLogsControl );
}
