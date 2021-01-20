/******************************************************************************\

Copyright © 2021, Intel Corporation

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

@file ml_function_log_release.h

@brief Provides functions to print out logs. Allows to log a function
       entering and exiting with a result returned by a function.
       This file is included in release build type.

\******************************************************************************/
#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief A structure that logs function entered/exiting.
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename Result>
    struct FunctionLogReleaseTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( FunctionLogReleaseTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( FunctionLogReleaseTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        Result m_Result;

        //////////////////////////////////////////////////////////////////////////
        /// @brief FunctionLogReleaseTrait constructor.
        /// @param name     function name.
        /// @param result   function initial result value.
        //////////////////////////////////////////////////////////////////////////
        FunctionLogReleaseTrait(
            const char*  name,
            const Result result )
            : m_Result( result )
        {
            (void) name;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "FunctionLogReleaseTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs function input parameter.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Input( const Values&... ) const
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs function exit parameter.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Output( const Values&... ) const
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs traits.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE static void Traits( const Values&... )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs a debug message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Debug( const Values&... ) const
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs an information message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Info( const Values&... ) const
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs a warning message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Warning( const Values&... values ) const
        {
            T::Tools::Log( LogType::Warning, "", values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs an error message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Error( const Values&... values ) const
        {
            T::Tools::Log( LogType::Error, "", values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs a critical error message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Critical( const Values&... values ) const
        {
            T::Tools::Log( LogType::Critical, "", values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs into a csv file.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Csv( const Values&... ) const
        {
        }
    };
} // namespace ML
