/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_macros.h

@brief Macros.
*/

#pragma once

//////////////////////////////////////////////////////////////////////////
/// @brief Type for dummy traits (if traits are not available).
//////////////////////////////////////////////////////////////////////////
struct TraitsDummy
{
};
#define ML_TRAITS_DUMMY                                 TraitsDummy

//////////////////////////////////////////////////////////////////////////
/// @brief typename T shortcut macro.
//////////////////////////////////////////////////////////////////////////
#define TT                                              typename T

//////////////////////////////////////////////////////////////////////////
/// @brief ML_LOG_TAG
//////////////////////////////////////////////////////////////////////////
#define ML_LOG_TAG                                      "[ML]"

//////////////////////////////////////////////////////////////////////////
/// @brief ML_LOG
//////////////////////////////////////////////////////////////////////////
#define ML_LOG( adapter, level, functionName, message ) IU_DBG_PRINT_TAGGED( adapter, _##level, IU_DBG_LAYER_ML, ML_LOG_TAG, functionName, message )

//////////////////////////////////////////////////////////////////////////
/// @brief ML_FUNCTION_NAME_PRETTY
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_NAME_PRETTY                         1

//////////////////////////////////////////////////////////////////////////
/// @brief ML_FUNCTION_NAME
//////////////////////////////////////////////////////////////////////////
#if ML_DEBUG || ML_INTERNAL
    #if ML_FUNCTION_NAME_PRETTY
        #define ML_FUNCTION_NAME __func__
    #else
        #define ML_FUNCTION_NAME __FUNCTION__
    #endif
#else
    #define ML_FUNCTION_NAME ""
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief ML_LOG_ERROR_STATIC
//////////////////////////////////////////////////////////////////////////
#if ML_DEBUG || ML_INTERNAL
    #define ML_LOG_ERROR_STATIC( condition, result ) T::FunctionLogStatic::ErrorStatic( ML_FUNCTION_NAME, result, "Invalid condition", #condition );
#else
    #define ML_LOG_ERROR_STATIC( condition, result ) T::FunctionLogStatic::ErrorStatic( "Invalid condition", #condition );
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Enter/exit tracking for functions that returns a result.
///        Macro will create a local variable called "result"
///        and will assign provided "value".
///        Result will be automatically logged for every exit point.
///
///        bool Function()
///        {
///             ML_FUNCTION_LOG( true, context );     -> bool log.m_Result = true;
///
///             if( true )
///             {
///                 return log.m_Result;              -> will log "result = true"
///             }
///
///             return log.m_Result = false;          -> will log "result = false"
///        }
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_LOG( value, context )     \
    using FunctionResult = decltype( value ); \
    TT::template FunctionLog<FunctionResult> log( ML_FUNCTION_NAME, value, context );

//////////////////////////////////////////////////////////////////////////
/// @brief Static implementation of ML_FUNCTION_LOG.
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_LOG_STATIC( value )       \
    using FunctionResult = decltype( value ); \
    TT::template FunctionLog<FunctionResult> log( ML_FUNCTION_NAME, value, nullptr );

//////////////////////////////////////////////////////////////////////////
/// @brief Condition (bool/StatusCode) sanity check.
///        Can be used only with ML_FUNCTION_LOG or ML_FUNCTION_LOG_STATIC.
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_CHECK( condition )                       \
    log.m_Result = Validation::CheckResult( ( condition ) ); \
    if( log.m_Result != StatusCode::Success )                \
    {                                                        \
        log.Error( "Invalid condition", #condition );        \
        return log.m_Result;                                 \
    }

//////////////////////////////////////////////////////////////////////////
/// @brief Condition (bool/StatusCode) sanity check with custom error code.
///        Can be used only with ML_FUNCTION_LOG or ML_FUNCTION_LOG_STATIC.
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_CHECK_ERROR( condition, error )                       \
    if( Validation::CheckResult( ( condition ) ) != StatusCode::Success ) \
    {                                                                     \
        log.Error( "Invalid condition", #condition );                     \
        return log.m_Result = error;                                      \
    }

//////////////////////////////////////////////////////////////////////////
/// @brief Static implementation of ML_FUNCTION_CHECK.
///        Can be used as standalone macro.
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_CHECK_STATIC( condition )                            \
    if( const StatusCode ret = Validation::CheckResult( ( condition ) ); \
        ret != StatusCode::Success )                                     \
    {                                                                    \
        ML_LOG_ERROR_STATIC( condition, ret )                            \
        return ret;                                                      \
    }

//////////////////////////////////////////////////////////////////////////
/// @brief Static implementation of ML_FUNCTION_CHECK_ERROR.
///        Can be used as standalone macro.
//////////////////////////////////////////////////////////////////////////
#define ML_FUNCTION_CHECK_ERROR_STATIC( condition, error )                \
    if( Validation::CheckResult( ( condition ) ) != StatusCode::Success ) \
    {                                                                     \
        ML_LOG_ERROR_STATIC( condition, error )                           \
        return error;                                                     \
    }

//////////////////////////////////////////////////////////////////////////
/// @brief Success / Failed macros.
//////////////////////////////////////////////////////////////////////////
#define ML_SUCCESS( result )                      ( ( result ) == StatusCode::Success )
#define ML_FAIL( result )                         ( ( result ) != StatusCode::Success )
#define ML_STATUS( result )                       ( ( result ) ? StatusCode::Success : StatusCode::Failed )

//////////////////////////////////////////////////////////////////////////
/// @brief Size32 macro.
//////////////////////////////////////////////////////////////////////////
#define ML_SIZE32( x )                            ( static_cast<uint32_t>( sizeof( x ) / sizeof( uint32_t ) ) )

//////////////////////////////////////////////////////////////////////////
/// @brief Array size macro.
//////////////////////////////////////////////////////////////////////////
#define ML_ARRAY_SIZE( x )                        ( sizeof( x ) / sizeof( ( x )[0] ) )

//////////////////////////////////////////////////////////////////////////
/// @brief Bitfield macros.
//////////////////////////////////////////////////////////////////////////
#define ML_BITFIELD_BIT( bit )                    1
#define ML_BITFIELD_RANGE( start, end )           ( ( end ) - ( start ) + 1 )
#define ML_BITMASK( n )                           ( ~( static_cast<uint64_t>( -1 ) << ( n ) ) )
#define ML_BIT( bit )                             ( 1ull << ( bit ) )

//////////////////////////////////////////////////////////////////////////
/// @brief Assert definition.
//////////////////////////////////////////////////////////////////////////
#define ML_ASSERT_ADAPTER( condition, adapterId ) IU_ASSERT_TAGGED( adapterId, ( condition ), IU_DBG_LAYER_ML, ML_LOG_TAG )
#define ML_ASSERT_ALWAYS_ADAPTER( adapterId )     ML_ASSERT_ADAPTER( false, adapterId )

#define ML_ASSERT_NO_ADAPTER( condition )         ML_ASSERT_ADAPTER( ( condition ), IU_ADAPTER_ID_UNKNOWN )
#define ML_ASSERT_ALWAYS_NO_ADAPTER()             ML_ASSERT_NO_ADAPTER( false )

#define ML_ASSERT( condition )                    ML_ASSERT_ADAPTER( ( condition ), log.m_Context ? log.m_Context->m_AdapterId : IU_ADAPTER_ID_UNKNOWN )
#define ML_ASSERT_ALWAYS()                        ML_ASSERT( false )

//////////////////////////////////////////////////////////////////////////
/// @brief Static assert macros.
//////////////////////////////////////////////////////////////////////////
#define ML_STATIC_ASSERT( condition, message )    static_assert( ( condition ), "Error: " #message )
#define ML_STATIC_ASSERT_ALWAYS( message )        static_assert( true, "Error: " #message )

//////////////////////////////////////////////////////////////////////////
/// @brief Metrics library object declaration macros
//////////////////////////////////////////////////////////////////////////
#define ML_DELETE_DEFAULT_CONSTRUCTOR( T )        T() = delete;

#define ML_DELETE_DEFAULT_COPY( TypeName )           \
    TypeName( const TypeName& )            = delete; \
    TypeName& operator=( const TypeName& ) = delete;

#define ML_DELETE_DEFAULT_MOVE( TypeName )      \
    TypeName( TypeName&& )            = delete; \
    TypeName& operator=( TypeName&& ) = delete;

#define ML_DELETE_DEFAULT_COPY_AND_MOVE( TypeName ) \
    ML_DELETE_DEFAULT_COPY( TypeName )              \
    ML_DELETE_DEFAULT_MOVE( TypeName )

#define ML_INHERIT_CONSTRUCTORS( This, Base ) \
    template <typename... Args>               \
    This( Args&&... args )                    \
        : Base( args... )                     \
    {                                         \
    }

#define ML_DECLARE_TRAIT( This, BaseTypeName ) \
    using Base = BaseTypeName::This<T>;        \
    ML_INHERIT_CONSTRUCTORS( This, Base );     \
    ML_DELETE_DEFAULT_COPY_AND_MOVE( This )

#define ML_DECLARE_TRAIT_WITH_COPY_AND_MOVE( This, BaseTypeName ) \
    using Base = BaseTypeName::This<T>;                           \
    ML_INHERIT_CONSTRUCTORS( This, Base );

//////////////////////////////////////////////////////////////////////////
/// @brief Structure pack definition macros.
//////////////////////////////////////////////////////////////////////////
#define ML_STRUCTURE_PACK_BEGIN( x )                   ML_PRAGMA( pack( push, x ) )
#define ML_STRUCTURE_PACK_END()                        ML_PRAGMA( pack( pop ) )

//////////////////////////////////////////////////////////////////////////
/// @brief Following macros are required for compatibility between
///        IU debug framework and current metrics library logs.
//////////////////////////////////////////////////////////////////////////
#define _LOG_CRITICAL                                  IU_DBG_SEV_CRITICAL
#define _LOG_ERROR                                     IU_DBG_SEV_ERROR
#define _LOG_WARNING                                   IU_DBG_SEV_WARNING
#define _LOG_INFO                                      IU_DBG_SEV_INFO
#define _LOG_DEBUG                                     IU_DBG_SEV_DEBUG
#define _LOG_TRAITS                                    IU_DBG_SEV_TRAITS
#define _LOG_ENTERED                                   IU_DBG_SEV_ENTERED
#define _LOG_EXITING                                   IU_DBG_SEV_EXITING
#define _LOG_INPUT                                     IU_DBG_SEV_INPUT
#define _LOG_OUTPUT                                    IU_DBG_SEV_OUTPUT

#define __LOG_CRITICAL                                 IU_DBG_SEV_CRITICAL
#define __LOG_ERROR                                    IU_DBG_SEV_ERROR
#define __LOG_WARNING                                  IU_DBG_SEV_WARNING
#define __LOG_INFO                                     IU_DBG_SEV_INFO
#define __LOG_DEBUG                                    IU_DBG_SEV_DEBUG
#define __LOG_TRAITS                                   IU_DBG_SEV_TRAITS
#define __LOG_ENTERED                                  IU_DBG_SEV_ENTERED
#define __LOG_EXITING                                  IU_DBG_SEV_EXITING
#define __LOG_INPUT                                    IU_DBG_SEV_INPUT
#define __LOG_OUTPUT                                   IU_DBG_SEV_OUTPUT

#define F__LOG_CRITICAL( adapter, level, layer, ... )  F_IU_DBG_SEV_CRITICAL( adapter, level, layer, __VA_ARGS__ )
#define F__LOG_ERROR( adapter, level, layer, ... )     F_IU_DBG_SEV_ERROR( adapter, level, layer, __VA_ARGS__ )
#define F__LOG_WARNING( adapter, level, layer, ... )   F_IU_DBG_SEV_WARNING( adapter, level, layer, __VA_ARGS__ )
#define F__LOG_INFO( adapter, level, layer, ... )      F_IU_DBG_SEV_INFO( adapter, level, layer, __VA_ARGS__ )
#define F__LOG_DEBUG( adapter, level, layer, ... )     F_IU_DBG_SEV_DEBUG( adapter, level, layer, __VA_ARGS__ )
#define F__LOG_TRAITS( adapter, level, layer, ... )    F_IU_DBG_SEV_TRAITS( adapter, level, layer, __VA_ARGS__ )
#define F__LOG_ENTERED( adapter, level, layer, ... )   F_IU_DBG_SEV_ENTERED( adapter, level, layer, __VA_ARGS__ )
#define F__LOG_EXITING( adapter, level, layer, ... )   F_IU_DBG_SEV_EXITING( adapter, level, layer, __VA_ARGS__ )
#define F__LOG_INPUT( adapter, level, layer, ... )     F_IU_DBG_SEV_INPUT( adapter, level, layer, __VA_ARGS__ )
#define F__LOG_OUTPUT( adapter, level, layer, ... )    F_IU_DBG_SEV_OUTPUT( adapter, level, layer, __VA_ARGS__ )

#define F___LOG_CRITICAL( adapter, level, layer, ... ) F_IU_DBG_SEV_CRITICAL( adapter, level, layer, __VA_ARGS__ )
#define F___LOG_ERROR( adapter, level, layer, ... )    F_IU_DBG_SEV_ERROR( adapter, level, layer, __VA_ARGS__ )
#define F___LOG_WARNING( adapter, level, layer, ... )  F_IU_DBG_SEV_WARNING( adapter, level, layer, __VA_ARGS__ )
#define F___LOG_INFO( adapter, level, layer, ... )     F_IU_DBG_SEV_INFO( adapter, level, layer, __VA_ARGS__ )
#define F___LOG_DEBUG( adapter, level, layer, ... )    F_IU_DBG_SEV_DEBUG( adapter, level, layer, __VA_ARGS__ )
#define F___LOG_TRAITS( adapter, level, layer, ... )   F_IU_DBG_SEV_TRAITS( adapter, level, layer, __VA_ARGS__ )
#define F___LOG_ENTERED( adapter, level, layer, ... )  F_IU_DBG_SEV_ENTERED( adapter, level, layer, __VA_ARGS__ )
#define F___LOG_EXITING( adapter, level, layer, ... )  F_IU_DBG_SEV_EXITING( adapter, level, layer, __VA_ARGS__ )
#define F___LOG_INPUT( adapter, level, layer, ... )    F_IU_DBG_SEV_INPUT( adapter, level, layer, __VA_ARGS__ )
#define F___LOG_OUTPUT( adapter, level, layer, ... )   F_IU_DBG_SEV_OUTPUT( adapter, level, layer, __VA_ARGS__ )
