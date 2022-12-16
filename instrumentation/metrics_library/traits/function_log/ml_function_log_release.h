/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_function_log_release.h

@brief Provides functions to print out logs. Allows to log a function
       entering and exiting with a result returned by a function.
       This file is included in release build type.
*/

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
        Result       m_Result;
        TT::Context* m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief FunctionLogReleaseTrait constructor.
        /// @param name     function name.
        /// @param result   function initial result value.
        //////////////////////////////////////////////////////////////////////////
        FunctionLogReleaseTrait(
            const char* /*name*/,
            const Result result,
            TT::Context* context )
            : m_Result( result )
            , m_Context( context )
        {
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
            T::Tools::Log( LogType::Warning, "", m_Context, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs an error message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Error( const Values&... values ) const
        {
            T::Tools::Log( LogType::Error, "", m_Context, values... );
        }

        ////////////////////////////////////////////////////////////////////////
        /// @brief Logs an error message. Static implementation.
        /// @param values        variable values to print out.
        ////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE static void ErrorStatic( const Values&... values )
        {
            T::Tools::Log( LogType::Error, "", nullptr, values... );
        }
        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs a critical error message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Critical( const Values&... values ) const
        {
            T::Tools::Log( LogType::Critical, "", m_Context, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs into a csv file.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Csv( [[maybe_unused]] const Values&... values ) const
        {
        }
    };
} // namespace ML
