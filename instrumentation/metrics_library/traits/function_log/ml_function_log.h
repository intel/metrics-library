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

@file ml_function_log.h

@brief Provides functions to print logs out. Allows to log a function
       entering and exiting with a result returned by a function.
       This file is included in debug and release-internal build types.

\******************************************************************************/
#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief A structure that logs function entered/exiting.
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename Result>
    struct FunctionLogTrait : public T::template FunctionLogCommon<Result>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( FunctionLogTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( FunctionLogTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = TT::template FunctionLogCommon<Result>;
        using Base::m_Name;
        using Base::m_Result;

        //////////////////////////////////////////////////////////////////////////
        /// @brief FunctionLogTrait constructor.
        /// @param name     function name.
        /// @param result   function initial result value.
        //////////////////////////////////////////////////////////////////////////
        FunctionLogTrait(
            const char*  name,
            const Result result )
            : Base( name, result )
        {
            T::Tools::Log( LogType::Entered, m_Name, Constants::Log::m_EnteredFunctionTag + m_Name );
            T::Debug::IncrementIndentLevel();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief FunctionLogTrait destructor.
        //////////////////////////////////////////////////////////////////////////
        ~FunctionLogTrait()
        {
            T::Debug::DecrementIndentLevel();
            T::Tools::Log( LogType::Exiting, m_Name, Constants::Log::m_ExitingFunctionTag + m_Name + " = " + T::Debug::ToString( m_Result ) );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "FunctionLogTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs function input parameter.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Input( const Values&... values ) const
        {
            T::Tools::Log( LogType::Input, m_Name, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs function exit parameter.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Output( const Values&... values ) const
        {
            T::Tools::Log( LogType::Output, m_Name, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs traits.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE static void Traits( const Values&... values )
        {
            T::Tools::Log( LogType::Traits, "", values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs a debug message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Debug( const Values&... values ) const
        {
            T::Tools::Log( LogType::Debug, m_Name, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs an information message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Info( const Values&... values ) const
        {
            T::Tools::Log( LogType::Info, m_Name, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs a warning message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Warning( const Values&... values ) const
        {
            T::Tools::Log( LogType::Warning, m_Name, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs an error message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Error( const Values&... values ) const
        {
            T::Tools::Log( LogType::Error, m_Name, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs a critical error message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Critical( const Values&... values ) const
        {
            T::Tools::Log( LogType::Critical, m_Name, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs into a csv file.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Csv( const Values&... values ) const
        {
            T::Tools::WriteToCsv( values... );
        }
    };
} // namespace ML
