/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_tools_os.h

@brief Contains a set of os specific helper functions.
*/

#pragma once

namespace ML
{
    namespace BASE
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
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "ToolsOsTrait<Traits> (Linux)";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns process id.
            /// @return process id.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static uint32_t GetProcessId()
            {
                ML_ASSERT_ALWAYS();
                return getpid();
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns cpu timestamp.
            /// @return timestamp value.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static uint64_t GetCpuTimestamp()
            {
                struct timespec time = {};
                clock_gettime( CLOCK_MONOTONIC, &time );
                return (uint64_t) time.tv_nsec + (uint64_t) time.tv_sec * Constants::Time::m_SecondInNanoseconds;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Gets exact cpu timestamp frequency.
            /// @return cpu timestamp frequency.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static uint64_t GetCpuTimestampFrequency()
            {
                return Constants::Time::m_SecondInNanoseconds;
            }

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
                else
                {
                    uint32_t toInteger = std::atoi( rawData );
                    data               = static_cast<Result>( toInteger );
                }

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
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct ToolsOsTrait : BASE::ToolsOsTrait<T>
        {
            ML_DECLARE_TRAIT( ToolsOsTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct ToolsOsTrait : GEN9::ToolsOsTrait<T>
        {
            ML_DECLARE_TRAIT( ToolsOsTrait, GEN9 );
        };
    } // namespace GEN11

    namespace XE_LP
    {
        template <typename T>
        struct ToolsOsTrait : GEN11::ToolsOsTrait<T>
        {
            ML_DECLARE_TRAIT( ToolsOsTrait, GEN11 );
        };
    } // namespace XE_LP
} // namespace ML
