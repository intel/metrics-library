/******************************************************************************\

Copyright © 2020, Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

@file ml_tools_os.h

@brief Contains a set of os specific helper functions.

\******************************************************************************/
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
            /// @brief  Returns a derived object from a given base object. Run-time
            ///         type information is disabled on Linux by driver, so
            ///         static_cast is used here.
            /// @param  base    base object.
            /// @return         derived object.
            //////////////////////////////////////////////////////////////////////////
            template <typename DerivedObject, typename BaseObject>
            ML_INLINE static DerivedObject* DynamicCast( BaseObject* base )
            {
                return static_cast<DerivedObject*>( base );
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

    namespace GEN12
    {
        template <typename T>
        struct ToolsOsTrait : GEN11::ToolsOsTrait<T>
        {
            ML_DECLARE_TRAIT( ToolsOsTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
