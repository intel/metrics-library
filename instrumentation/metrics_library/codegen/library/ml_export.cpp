/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_export.cpp

@brief Auto-generated file.
       Allows to export a specific Metrics Library entry points.
       Also to obtain Traits table for a given os / api / gen.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_export
Template:           Tools/MetricsLibraryGenerator/templates/export.h
*/

#include "ml_pch.h"
#include "ml_traits_include.h"
#include "ml_library_configuration.h"
#include "ml_export.h"
#include "ml_driver_store_os.h"
#if ML_ENABLE_GEN9
#include "ml_traits_gen9.h"
#endif
#if ML_ENABLE_GEN11
#include "ml_traits_gen11.h"
#endif
#if ML_ENABLE_XE_LP
#include "ml_traits_xe_lp.h"
#endif
#if ML_ENABLE_XE_HP
#include "ml_traits_xe_hp.h"
#endif
#if ML_ENABLE_XE_HPG
#include "ml_traits_xe_hpg.h"
#endif
#if ML_ENABLE_XE_HPC
#include "ml_traits_xe_hpc.h"
#endif
#if ML_ENABLE_XE2_HPG
#include "ml_traits_xe2_hpg.h"
#endif

using namespace ML;

//////////////////////////////////////////////////////////////////////////
/// @brief Retrieves a gen and client specific namespace.
/// @param client graphics api type.
//////////////////////////////////////////////////////////////////////////
#define ML_GET_NAMESPACE( GEN_NAMESPACE, CLIENT ) ML::GEN_NAMESPACE::CLIENT

//////////////////////////////////////////////////////////////////////////
/// @brief  Retrieves a function table based on the queried gen.
/// @param  clientType  client type.
/// @return             function table or nullptr if the gen has not been recognized.
//////////////////////////////////////////////////////////////////////////
const DdiFunctionTableBase* GetFunctionTable( const ClientType_1_0& clientType )
{
#if ML_ENABLE_GEN9
    if( clientType.Gen == ClientGen::Gen9 )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &ML_GET_NAMESPACE( GEN9, OpenCL )::DdiFunctionTable<ML_GET_NAMESPACE( GEN9, OpenCL )::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &ML_GET_NAMESPACE( GEN9, OneApi )::DdiFunctionTable<ML_GET_NAMESPACE( GEN9, OneApi )::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS_NO_ADAPTER();
                break;
        }
    }
#endif // ML_ENABLE_GEN9

#if ML_ENABLE_GEN11
    if( clientType.Gen == ClientGen::Gen11 )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &ML_GET_NAMESPACE( GEN11, OpenCL )::DdiFunctionTable<ML_GET_NAMESPACE( GEN11, OpenCL )::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &ML_GET_NAMESPACE( GEN11, OneApi )::DdiFunctionTable<ML_GET_NAMESPACE( GEN11, OneApi )::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS_NO_ADAPTER();
                break;
        }
    }
#endif // ML_ENABLE_GEN11

#if ML_ENABLE_XE_LP
    if( clientType.Gen == ClientGen::Gen12 )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &ML_GET_NAMESPACE( XE_LP, OpenCL )::DdiFunctionTable<ML_GET_NAMESPACE( XE_LP, OpenCL )::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &ML_GET_NAMESPACE( XE_LP, OneApi )::DdiFunctionTable<ML_GET_NAMESPACE( XE_LP, OneApi )::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS_NO_ADAPTER();
                break;
        }
    }
#endif // ML_ENABLE_XE_LP

#if ML_ENABLE_XE_HP
    if( clientType.Gen == ClientGen::XeHP )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &ML_GET_NAMESPACE( XE_HP, OpenCL )::DdiFunctionTable<ML_GET_NAMESPACE( XE_HP, OpenCL )::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &ML_GET_NAMESPACE( XE_HP, OneApi )::DdiFunctionTable<ML_GET_NAMESPACE( XE_HP, OneApi )::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS_NO_ADAPTER();
                break;
        }
    }
#endif // ML_ENABLE_XE_HP

#if ML_ENABLE_XE_HPG
    if( clientType.Gen == ClientGen::XeHPG )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &ML_GET_NAMESPACE( XE_HPG, OpenCL )::DdiFunctionTable<ML_GET_NAMESPACE( XE_HPG, OpenCL )::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &ML_GET_NAMESPACE( XE_HPG, OneApi )::DdiFunctionTable<ML_GET_NAMESPACE( XE_HPG, OneApi )::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS_NO_ADAPTER();
                break;
        }
    }
#endif // ML_ENABLE_XE_HPG

#if ML_ENABLE_XE_HPC
    if( clientType.Gen == ClientGen::XeHPC )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &ML_GET_NAMESPACE( XE_HPC, OpenCL )::DdiFunctionTable<ML_GET_NAMESPACE( XE_HPC, OpenCL )::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &ML_GET_NAMESPACE( XE_HPC, OneApi )::DdiFunctionTable<ML_GET_NAMESPACE( XE_HPC, OneApi )::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS_NO_ADAPTER();
                break;
        }
    }
#endif // ML_ENABLE_XE_HPC

#if ML_ENABLE_XE2_HPG
    if( clientType.Gen == ClientGen::Xe2HPG )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &ML_GET_NAMESPACE( XE2_HPG, OpenCL )::DdiFunctionTable<ML_GET_NAMESPACE( XE2_HPG, OpenCL )::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &ML_GET_NAMESPACE( XE2_HPG, OneApi )::DdiFunctionTable<ML_GET_NAMESPACE( XE2_HPG, OneApi )::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS_NO_ADAPTER();
                break;
        }
    }
#endif // ML_ENABLE_XE2_HPG

    return nullptr;
}

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief  Translates all unsupported gpu types to supported ones.
    /// @return clientType  supported gpu type for a given input gpu type.
    //////////////////////////////////////////////////////////////////////////
    ML_INLINE void TranslateToSupportedGpuType( [[maybe_unused]] ClientGen& clientGen )
    {
    #if( ML_ENABLE_GEN9 || ML_ENABLE_GEN11 )
        switch( clientGen )
        {
        #if ML_ENABLE_GEN9
            case ClientGen::Gen9LP:
                clientGen = ClientGen::Gen9;
                break;
        #endif // ML_ENABLE_GEN9

        #if ML_ENABLE_GEN11
            case ClientGen::Gen11LP:
                clientGen = ClientGen::Gen11;
                break;
        #endif // ML_ENABLE_GEN11

            default:
                break;
        }
    #endif
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Translates all unsupported apis to supported ones.
    /// @return clientType  supported api for a given input api.
    //////////////////////////////////////////////////////////////////////////
    ML_INLINE void TranslateToSupportedApiType( [[maybe_unused]] ClientApi& clientApi )
    {

    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Translates all unsupported configurations to supported ones.
    /// @return clientType  supported configuration for a given input configuration.
    //////////////////////////////////////////////////////////////////////////
    ML_INLINE void TranslateToSupportedClientType( ClientType_1_0& clientType )
    {
        TranslateToSupportedGpuType( clientType.Gen );
        TranslateToSupportedApiType( clientType.Api );
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Initializes function table with a correct function table for
    ///         the current gen, given in ClientType variable.
    /// @param  clientType  client type.
    /// @return             function table for the recognized gen or nullptr
    ///                     if the gen has not been recognized and
    ///                     the initialization failed.
    //////////////////////////////////////////////////////////////////////////
    const DdiFunctionTableBase* GetDdiFunctionTable( const ClientType_1_0& clientType )
    {
        return GetFunctionTable( clientType );
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Initializes function table with a correct function table for
    ///         the current gen, given in ContextHandle variable.
    /// @param  handle    handle to created context instance
    /// @return           function table for the recognized gen or nullptr
    ///                   if the gen has not been recognized and
    ///                   the initialization failed.
    //////////////////////////////////////////////////////////////////////////
    const DdiFunctionTableBase* GetDdiFunctionTable( const ContextHandle_1_0& handle )
    {
        return GetFunctionTable( BaseObject::GetClientType( handle ) );
    }
} // namespace ML

#if defined( __cplusplus )
extern "C" {
#endif // C++

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Export of metrics_library api entry ContextCreate_1_0.
    /// @param  clientType
    /// @param  createData
    /// @param  handle
    /// @return StatusCode
    //////////////////////////////////////////////////////////////////////////
    ML_EXPORT( StatusCode ) ContextCreate_1_0( ClientType_1_0 clientType, ContextCreateData_1_0* createData, ContextHandle_1_0* handle )
    {
        auto overriddenContextCreate_1_0 = ( ContextCreateFunction_1_0 )( DriverStoreOs::GetRedirectedDdi( METRICS_LIBRARY_CONTEXT_CREATE_1_0 ) );

        if( overriddenContextCreate_1_0 != nullptr )
        {
            return overriddenContextCreate_1_0( clientType, createData, handle );
        }
        else
        {
            // Initialize logging system.
            LibraryConfiguration::InitializeLogger();

            // Override api/gen settings.
            LibraryConfiguration::OverrideClientType( clientType );

            // Translate a given client type to a supported one.
            TranslateToSupportedClientType( clientType );

            // Obtain function table.
            auto functionTable = GetDdiFunctionTable( clientType );
            ML_ASSERT_NO_ADAPTER( functionTable != nullptr );

            return functionTable
                ? functionTable->pfnContextCreate_1_0( clientType, createData, handle )
                : StatusCode::NullPointer;
            }
        }


    //////////////////////////////////////////////////////////////////////////
    /// @brief  Export of metrics_library api entry ContextDelete_1_0.
    /// @param  handle
    /// @return StatusCode
    //////////////////////////////////////////////////////////////////////////
    ML_EXPORT( StatusCode ) ContextDelete_1_0( const ContextHandle_1_0 handle )
    {
        auto overriddenContextDelete_1_0 = ( ContextDeleteFunction_1_0 )( DriverStoreOs::GetRedirectedDdi( METRICS_LIBRARY_CONTEXT_DELETE_1_0 ) );

        if( overriddenContextDelete_1_0 != nullptr )
        {
            const StatusCode status = overriddenContextDelete_1_0( handle );
            DriverStoreOs::UnloadLibrary();
            return status;
        }
        else
        {
            // Obtain function table.
            auto functionTable = GetDdiFunctionTable( handle );
            ML_ASSERT_NO_ADAPTER( functionTable != nullptr );

            return functionTable
                ? functionTable->pfnContextDelete_1_0( handle )
                : StatusCode::NullPointer;
            }
        }


#if defined( __cplusplus )
}
#endif // C++
