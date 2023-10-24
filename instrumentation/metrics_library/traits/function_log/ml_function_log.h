/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2023 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_function_log.h

@brief Provides functions to print logs out. Allows to log a function
       entering and exiting with a result returned by a function.
       This file is included in debug and release-internal build types.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief A structure that logs function entered/exiting.
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename Result = uint32_t>
    struct FunctionLogTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( FunctionLogTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( FunctionLogTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        std::string  m_Name;
        Result       m_Result;
        TT::Context* m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief FunctionLogTrait constructor.
        /// @param name     function name.
        /// @param result   function initial result value.
        /// @param context  context.
        //////////////////////////////////////////////////////////////////////////
        FunctionLogTrait(
            const char*  name,
            const Result result,
            TT::Context* context )
            : m_Result( result )
            , m_Context( context )
        {
            if( T::Tools::CheckLogLayer() )
            {
                SetFunctionName( name );
            }

            if( m_Context != nullptr )
            {
                m_Context->m_Debug.IncrementIndentLevel();
            }

            T::Tools::Log( LogType::Entered, m_Name, m_Context, std::string( Constants::Log::m_EnteredFunctionTag ) );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief FunctionLogTrait destructor.
        //////////////////////////////////////////////////////////////////////////
        ~FunctionLogTrait()
        {
            T::Tools::Log( LogType::Exiting, m_Name, m_Context, std::string( Constants::Log::m_ExitingFunctionTag ), m_Result );

            if( m_Context != nullptr )
            {
                m_Context->m_Debug.DecrementIndentLevel();
            }
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
        /// @brief  Strips string of information that is not useful from
        ///         logging perspective (e.g. information has been already logged etc.).
        ///         Right now, namespace and template are both removed.
        /// @param  name    full function name (full path, namespace etc.).
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void SetFunctionName( const char* name )
        {
            // Store a function name.
            m_Name = name;

            // If requested, simplify it.
            if( !T::ConstantsOs::Log::m_SimplifyFunctionName )
            {
                return;
            }

            auto& functionNamesMap = T::Debug::GetFunctionNamesMap();
            auto& mutex            = T::Debug::GetMutex();

            std::lock_guard<std::mutex> guard( mutex );

            auto foundFunctionName = functionNamesMap.find( m_Name );
            if( foundFunctionName != functionNamesMap.end() )
            {
                m_Name = foundFunctionName->second;
                return;
            }

            std::string erasedString( m_Name );

            // Remove "ML::" namespace from function name.
            const auto& namespaceBegin = erasedString.find_first_of( "M" );
            if( namespaceBegin != std::string::npos )
            {
                const auto& namespaceEnd = erasedString.find_first_of( ":" ) + 2 - namespaceBegin;
                erasedString.erase( namespaceBegin, namespaceEnd );
            }

            // Remove template specialization from function name.
            const auto& bracketsBegin = erasedString.find_first_of( "<" );
            if( bracketsBegin != std::string::npos )
            {
                const auto& bracketsEnd = erasedString.find_first_of( ">" ) + 1 - bracketsBegin;
                erasedString.erase( bracketsBegin, bracketsEnd );
            }

            // Remove namespaces from function name.
            RemoveNamespaces( Constants::Log::m_SimplifyFunctionNameLevel, erasedString );

            // Cache result.
            functionNamesMap.insert( { m_Name, erasedString } );

            // Store a simplified function name.
            m_Name = erasedString;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Simplify function name by removing namespace or struct information.
        /// @param  level           level of simplifying string. By increasing the level
        ///                         the function name is more complex.
        /// @return functionName    a string with simplified function name.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void RemoveNamespaces( const uint32_t level, std::string& functionName )
        {
            if( level == 0 )
            {
                return;
            }

            constexpr std::string_view namespaceOperator( "::" );
            constexpr size_t           namespaceOperatorSize = namespaceOperator.size();
            constexpr size_t           namespaceBegin        = 0;
            size_t                     namespaceEnd          = functionName.size();

            for( uint32_t i = 0; i < level; ++i )
            {
                namespaceEnd = functionName.find_last_of( namespaceOperator, namespaceEnd );

                if( namespaceEnd == std::string::npos )
                {
                    return;
                }

                namespaceEnd -= namespaceOperatorSize;

                if( namespaceEnd == std::string::npos )
                {
                    break;
                }
            }

            functionName.erase( namespaceBegin, namespaceEnd + namespaceOperatorSize + 1 );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs function input parameter.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Input( const Values&... values ) const
        {
            T::Tools::Log( LogType::Input, m_Name, m_Context, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs function exit parameter.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Output( const Values&... values ) const
        {
            T::Tools::Log( LogType::Output, m_Name, m_Context, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs traits.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE static void Traits( const Values&... values )
        {
            T::Tools::Log( LogType::Traits, "", nullptr, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs a debug message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Debug( const Values&... values ) const
        {
            T::Tools::Log( LogType::Debug, m_Name, m_Context, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs an information message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Info( const Values&... values ) const
        {
            T::Tools::Log( LogType::Info, m_Name, m_Context, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs a warning message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Warning( const Values&... values ) const
        {
            T::Tools::Log( LogType::Warning, m_Name, m_Context, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs an error message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Error( const Values&... values ) const
        {
            T::Tools::Log( LogType::Error, m_Name, m_Context, values... );
        }

        ////////////////////////////////////////////////////////////////////////
        /// @brief Logs an error message. Static implementation.
        /// @param functioName   name of the function.
        /// @param result        result of the function.
        /// @param values        variable values to print out.
        ////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE static void ErrorStatic(
            const std::string& functionName,
            const StatusCode   result,
            const Values&... values )
        {
            T::Tools::Log( LogType::Entered, functionName, nullptr, std::string( Constants::Log::m_EnteredFunctionTag ) );
            T::Tools::Log( LogType::Error, functionName, nullptr, values... );
            T::Tools::Log( LogType::Exiting, functionName, nullptr, std::string( Constants::Log::m_ExitingFunctionTag ), result );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs a critical error message.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Critical( const Values&... values ) const
        {
            T::Tools::Log( LogType::Critical, m_Name, m_Context, values... );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs into a csv file.
        /// @param values   variable values to print out.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Values>
        ML_INLINE void Csv( const Values&... values ) const
        {
            T::Tools::WriteToCsv( m_Context, values... );
        }
    };
} // namespace ML
