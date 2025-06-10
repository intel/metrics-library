/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

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
#if ML_ENABLE_XE_LP
#include "ml_traits_xe_lp.h"
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
#if ML_ENABLE_XE3
#include "ml_traits_xe3.h"
#endif

using namespace ML;

//////////////////////////////////////////////////////////////////////////
/// @brief  Retrieves a function table based on the queried gen.
/// @param  clientType  client type.
/// @return             function table or nullptr if the gen has not been recognized.
//////////////////////////////////////////////////////////////////////////
const DdiFunctionTableBase* GetFunctionTable( const ClientType_1_0& clientType )
{
#if ML_ENABLE_XE_LP
    if( clientType.Gen == ClientGen::Gen12 )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &XE_LP::OpenCL::DdiFunctionTable<XE_LP::OpenCL::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &XE_LP::OneApi::DdiFunctionTable<XE_LP::OneApi::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS_NO_ADAPTER();
                break;
        }
    }
#endif // ML_ENABLE_XE_LP

#if ML_ENABLE_XE_HPG
    if( clientType.Gen == ClientGen::XeHPG )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &XE_HPG::OpenCL::DdiFunctionTable<XE_HPG::OpenCL::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &XE_HPG::OneApi::DdiFunctionTable<XE_HPG::OneApi::Traits>::GetInstance();
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
                return &XE_HPC::OpenCL::DdiFunctionTable<XE_HPC::OpenCL::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &XE_HPC::OneApi::DdiFunctionTable<XE_HPC::OneApi::Traits>::GetInstance();
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
                return &XE2_HPG::OpenCL::DdiFunctionTable<XE2_HPG::OpenCL::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &XE2_HPG::OneApi::DdiFunctionTable<XE2_HPG::OneApi::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS_NO_ADAPTER();
                break;
        }
    }
#endif // ML_ENABLE_XE2_HPG

#if ML_ENABLE_XE3
    if( clientType.Gen == ClientGen::Xe3 )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &XE3::OpenCL::DdiFunctionTable<XE3::OpenCL::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &XE3::OneApi::DdiFunctionTable<XE3::OneApi::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS_NO_ADAPTER();
                break;
        }
    }
#endif // ML_ENABLE_XE3

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
extern "C"
{
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
        auto overriddenContextCreate_1_0 = reinterpret_cast<ContextCreateFunction_1_0>( DriverStoreOs::GetRedirectedDdi( METRICS_LIBRARY_CONTEXT_CREATE_1_0 ) );

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
        auto overriddenContextDelete_1_0 = reinterpret_cast<ContextDeleteFunction_1_0>( DriverStoreOs::GetRedirectedDdi( METRICS_LIBRARY_CONTEXT_DELETE_1_0 ) );

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
