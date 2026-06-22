/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2026 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_tools_os.h

@brief Contains a set of os specific helper functions.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for ToolsOsTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ToolsOsTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ToolsOsTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ToolsOsTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Reads system settings.
        /// @param  name    system variable name.
        /// @return data    system variable value.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename Result>
        ML_INLINE static StatusCode GetSystemVariable(
            const char* name,
            Result&     data )
        {
            const std::string variableName = std::string( Constants::Library::m_Name ) + name;
            const char*       rawData      = std::getenv( variableName.c_str() );

            if( rawData == nullptr )
            {
                return StatusCode::Failed;
            }

            data = static_cast<Result>( std::atoi( rawData ) );

            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns current time as string.
        /// @return current time.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static std::string GetCurrentTime()
        {
            std::ostringstream timeOutput;
            tm                 timeLocal;
            time_t             timeCurrent = time( nullptr );

            timeLocal = *localtime( &timeCurrent );

            timeOutput << std::put_time( &timeLocal, "%d-%m-%Y_%H-%M-%S" );

            return timeOutput.str();
        }
    };
} // namespace ML
