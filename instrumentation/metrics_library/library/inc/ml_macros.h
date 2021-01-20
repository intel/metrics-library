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

@file ml_macros.h

@brief Macros.

\******************************************************************************/

#pragma once
// clang-format off

//////////////////////////////////////////////////////////////////////////
/// @brief typename T shortcut macro.
//////////////////////////////////////////////////////////////////////////
#define TT                                          typename T

//////////////////////////////////////////////////////////////////////////
/// @brief Type for dummy traits (if traits are not available).
//////////////////////////////////////////////////////////////////////////
struct TraitsDummy {};
#define ML_TRAITS_DUMMY                             TraitsDummy

//////////////////////////////////////////////////////////////////////////
/// @brief ML_LOG_TAG
//////////////////////////////////////////////////////////////////////////
#define ML_LOG_TAG                                  "[ML]"

//////////////////////////////////////////////////////////////////////////
/// @brief ML_LOG
//////////////////////////////////////////////////////////////////////////
#define ML_LOG( level, functionName, message )      IU_DBG_PRINT_TAGGED( _##level, IU_DBG_LAYER_ML, ML_LOG_TAG, functionName, message )

//////////////////////////////////////////////////////////////////////////
/// @brief ML_FUNCTION_NAME_PRETTY
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_NAME_PRETTY                     1

//////////////////////////////////////////////////////////////////////////
/// @brief ML_FUNCTION_NAME
//////////////////////////////////////////////////////////////////////////
#if ML_DEBUG || ML_INTERNAL
    #if ML_FUNCTION_NAME_PRETTY
        #define ML_FUNCTION_NAME                    __func__
    #else
        #define ML_FUNCTION_NAME                    __FUNCTION__
    #endif
#else
    #define ML_FUNCTION_NAME                        ""
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Enter/exit tracking for functions that returns a result.
///        Macro will create a local variable called "result"
///        and will assign provided "value".
///        Result will be automatically logged for every exit point.
///
///        bool Function()
///        {
///             ML_FUNCTION_LOG( true );     -> bool log.m_Result = true;
///
///             if( true )
///             {
///                 return log.m_Result;     -> will log "result = true"
///             }
///
///             return log.m_Result = false; -> will log "result = false"
///        }
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_LOG( value )                    using FunctionResult = decltype( value );                                \
                                                    TT::template FunctionLog<FunctionResult> log( ML_FUNCTION_NAME, value );

//////////////////////////////////////////////////////////////////////////
/// @brief Condition (bool/StatusCode) sanity check.
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_CHECK( condition )              log.m_Result = Validation::CheckResult( ( condition ) );    \
                                                    if( log.m_Result != StatusCode::Success )                   \
                                                    {                                                           \
                                                        log.Error( "Invalid condition", #condition);            \
                                                        return log.m_Result;                                    \
                                                    }

//////////////////////////////////////////////////////////////////////////
/// @brief Condition (bool/StatusCode) sanity check with custom error code.
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_CHECK_ERROR( condition, error ) if( Validation::CheckResult( ( condition ) ) != StatusCode::Success )    \
                                                    {                                                                        \
                                                        log.Error( "Invalid condition", #condition);                         \
                                                        return log.m_Result = error;                                         \
                                                    }

//////////////////////////////////////////////////////////////////////////
/// @brief Success / Failed macros.
//////////////////////////////////////////////////////////////////////////
#define ML_SUCCESS( result )                        ( ( result ) == StatusCode::Success )
#define ML_FAIL( result )                           ( ( result ) != StatusCode::Success )
#define ML_STATUS( result )                         ( ( result ) ? StatusCode::Success : StatusCode::Failed )

//////////////////////////////////////////////////////////////////////////
/// @brief Size32 macro.
//////////////////////////////////////////////////////////////////////////
#define ML_SIZE32( x )                              ( static_cast<uint32_t>( sizeof( x ) / sizeof( uint32_t ) ) )

//////////////////////////////////////////////////////////////////////////
/// @brief Array size macro.
//////////////////////////////////////////////////////////////////////////
#define ML_ARRAY_SIZE( x )                          ( sizeof( x ) / sizeof( ( x )[0] ) )

//////////////////////////////////////////////////////////////////////////
/// @brief Bitfield macros.
//////////////////////////////////////////////////////////////////////////
#define ML_BITFIELD_BIT( bit )                      1
#define ML_BITFIELD_RANGE( start, end )             ( ( end ) - ( start ) + 1 )
#define ML_BITMASK( n )                             ( ~( static_cast<uint64_t>( -1 ) << ( n ) ) )
#define ML_BIT( bit )                               ( 1ull << ( bit ) )

//////////////////////////////////////////////////////////////////////////
/// @brief Assert definition.
//////////////////////////////////////////////////////////////////////////
#define ML_ASSERT( condition )                      IU_ASSERT_TAGGED( ( condition ), IU_DBG_LAYER_ML, ML_LOG_TAG )
#define ML_ASSERT_ALWAYS()                          ML_ASSERT( false )

//////////////////////////////////////////////////////////////////////////
/// @brief Static assert macros.
//////////////////////////////////////////////////////////////////////////
#define ML_STATIC_ASSERT( condition, message )      static_assert( ( condition ), "Error: "#message )
#define ML_STATIC_ASSERT_ALWAYS( message )          static_assert( true, "Error: "#message )

//////////////////////////////////////////////////////////////////////////
/// @brief Metrics library object declaration macros
//////////////////////////////////////////////////////////////////////////
#define ML_DELETE_DEFAULT_CONSTRUCTOR( T )          T() = delete;

#define ML_DELETE_DEFAULT_COPY( TypeName )          TypeName( const TypeName& ) = delete;               \
                                                    TypeName& operator=( const TypeName& ) = delete;

#define ML_DELETE_DEFAULT_MOVE( TypeName )          TypeName( TypeName&& ) = delete;                    \
                                                    TypeName& operator=( TypeName&& ) = delete;

#define ML_DELETE_DEFAULT_COPY_AND_MOVE( TypeName ) ML_DELETE_DEFAULT_COPY( TypeName )                  \
                                                    ML_DELETE_DEFAULT_MOVE( TypeName )

#define ML_INHERIT_CONSTRUCTORS( This, Base )       template<typename... Args>                          \
                                                    This( Args&&... args ) : Base( args... )            \
                                                    {}

#define ML_DECLARE_TRAIT( This, BaseTypeName )      using Base = BaseTypeName::This<T>;                 \
                                                    ML_INHERIT_CONSTRUCTORS( This, Base );              \
                                                    ML_DELETE_DEFAULT_COPY_AND_MOVE( This )

//////////////////////////////////////////////////////////////////////////
/// @brief Structure pack definition macros.
//////////////////////////////////////////////////////////////////////////
#define ML_STRUCTURE_PACK_BEGIN( x )                ML_PRAGMA( pack( push, x ) )
#define ML_STRUCTURE_PACK_END()                     ML_PRAGMA( pack( pop ) )

//////////////////////////////////////////////////////////////////////////
/// @brief Following macros are required for compatibility between
///        IU debug framework and current metrics library logs.
//////////////////////////////////////////////////////////////////////////
#define _LOG_CRITICAL                               IU_DBG_SEV_CRITICAL
#define _LOG_ERROR                                  IU_DBG_SEV_ERROR
#define _LOG_WARNING                                IU_DBG_SEV_WARNING
#define _LOG_INFO                                   IU_DBG_SEV_INFO
#define _LOG_DEBUG                                  IU_DBG_SEV_DEBUG
#define _LOG_TRAITS                                 IU_DBG_SEV_TRAITS
#define _LOG_ENTERED                                IU_DBG_SEV_ENTERED
#define _LOG_EXITING                                IU_DBG_SEV_EXITING
#define _LOG_INPUT                                  IU_DBG_SEV_INPUT
#define _LOG_OUTPUT                                 IU_DBG_SEV_OUTPUT

#define __LOG_CRITICAL                              IU_DBG_SEV_CRITICAL
#define __LOG_ERROR                                 IU_DBG_SEV_ERROR
#define __LOG_WARNING                               IU_DBG_SEV_WARNING
#define __LOG_INFO                                  IU_DBG_SEV_INFO
#define __LOG_DEBUG                                 IU_DBG_SEV_DEBUG
#define __LOG_TRAITS                                IU_DBG_SEV_TRAITS
#define __LOG_ENTERED                               IU_DBG_SEV_ENTERED
#define __LOG_EXITING                               IU_DBG_SEV_EXITING
#define __LOG_INPUT                                 IU_DBG_SEV_INPUT
#define __LOG_OUTPUT                                IU_DBG_SEV_OUTPUT

#define F__LOG_CRITICAL( level, layer, ... )        F_IU_DBG_SEV_CRITICAL( level, layer, __VA_ARGS__ )
#define F__LOG_ERROR( level, layer, ... )           F_IU_DBG_SEV_ERROR( level, layer, __VA_ARGS__ )
#define F__LOG_WARNING( level, layer, ... )         F_IU_DBG_SEV_WARNING( level, layer, __VA_ARGS__ )
#define F__LOG_INFO( level, layer, ... )            F_IU_DBG_SEV_INFO( level, layer, __VA_ARGS__ )
#define F__LOG_DEBUG( level, layer, ... )           F_IU_DBG_SEV_DEBUG( level, layer, __VA_ARGS__ )
#define F__LOG_TRAITS( level, layer, ... )          F_IU_DBG_SEV_TRAITS( level, layer, __VA_ARGS__ )
#define F__LOG_ENTERED( level, layer, ... )         F_IU_DBG_SEV_ENTERED( level, layer, __VA_ARGS__ )
#define F__LOG_EXITING( level, layer, ... )         F_IU_DBG_SEV_EXITING( level, layer, __VA_ARGS__ )
#define F__LOG_INPUT( level, layer, ... )           F_IU_DBG_SEV_INPUT( level, layer, __VA_ARGS__ )
#define F__LOG_OUTPUT( level, layer, ... )          F_IU_DBG_SEV_OUTPUT( level, layer, __VA_ARGS__ )

#define F___LOG_CRITICAL( level, layer, ... )       F_IU_DBG_SEV_CRITICAL( level, layer, __VA_ARGS__ )
#define F___LOG_ERROR( level, layer, ... )          F_IU_DBG_SEV_ERROR( level, layer, __VA_ARGS__ )
#define F___LOG_WARNING( level, layer, ... )        F_IU_DBG_SEV_WARNING( level, layer, __VA_ARGS__ )
#define F___LOG_INFO( level, layer, ... )           F_IU_DBG_SEV_INFO( level, layer, __VA_ARGS__ )
#define F___LOG_DEBUG( level, layer, ... )          F_IU_DBG_SEV_DEBUG( level, layer, __VA_ARGS__ )
#define F___LOG_TRAITS( level, layer, ... )         F_IU_DBG_SEV_TRAITS( level, layer, __VA_ARGS__ )
#define F___LOG_ENTERED( level, layer, ... )        F_IU_DBG_SEV_ENTERED( level, layer, __VA_ARGS__ )
#define F___LOG_EXITING( level, layer, ... )        F_IU_DBG_SEV_EXITING( level, layer, __VA_ARGS__ )
#define F___LOG_INPUT( level, layer, ... )          F_IU_DBG_SEV_INPUT( level, layer, __VA_ARGS__ )
#define F___LOG_OUTPUT( level, layer, ... )         F_IU_DBG_SEV_OUTPUT( level, layer, __VA_ARGS__ )

// clang-format on
