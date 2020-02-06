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

@file ml_export.cpp

@brief Auto-generated file.
       Allows to export a specific Metrics Library entry points.
       Also to obtain Traits table for a given os / api / gen.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_export
Template:           Tools/MetricsLibraryGenerator/templates/export.h

\******************************************************************************/

#include "ml_pch.h"
#include "ml_export.h"
#if ML_ENABLE_GEN9
#include "ml_traits_gen9.h"
#endif
#if ML_ENABLE_GEN11
#include "ml_traits_gen11.h"
#endif
#if ML_ENABLE_GEN12
#include "ml_traits_gen12.h"
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
                ML_ASSERT_ALWAYS();
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
                ML_ASSERT_ALWAYS();
                break;
        }
    }
#endif // ML_ENABLE_GEN11

#if ML_ENABLE_GEN12
    if( clientType.Gen == ClientGen::Gen12 )
    {
        switch( clientType.Api )
        {
            #if ML_ENABLE_OPENCL
            case ClientApi::OpenCL:
                return &ML_GET_NAMESPACE( GEN12, OpenCL )::DdiFunctionTable<ML_GET_NAMESPACE( GEN12, OpenCL )::Traits>::GetInstance();
            #endif

            #if ML_ENABLE_ONEAPI
            case ClientApi::OneApi:
                return &ML_GET_NAMESPACE( GEN12, OneApi )::DdiFunctionTable<ML_GET_NAMESPACE( GEN12, OneApi )::Traits>::GetInstance();
            #endif

            default:
                ML_ASSERT_ALWAYS();
                break;
        }
    }
#endif // ML_ENABLE_GEN12

    return nullptr;
}

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief  Translates all unsupported gens to supported ones.
    /// @return clientType  supported gen for a given input gen.
    //////////////////////////////////////////////////////////////////////////
    ML_INLINE void TranslateToSupportedGenType( ClientGen& clientGen )
    {
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
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Translates all unsupported apis to supported ones.
    /// @return clientType  supported api for a given input api.
    //////////////////////////////////////////////////////////////////////////
    ML_INLINE void TranslateToSupportedApiType( ClientApi& clientApi )
    {
        switch( clientApi )
        {
            default:
                break;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Translates all unsupported configurations to supported ones.
    /// @return clientType  supported configuration for a given input configuration.
    //////////////////////////////////////////////////////////////////////////
    ML_INLINE void TranslateToSupportedClientType( ClientType_1_0& clientType )
    {
        TranslateToSupportedGenType( clientType.Gen );
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
        // Initialize logging system.
        LibraryConfiguration::IntializeLogger();

        // Override api/gen settings.
        LibraryConfiguration::OverrideClientType( clientType );

        // Translate a given client type to a supported one.
        TranslateToSupportedClientType( clientType );

        // Obtain function table.
        auto functionTable = GetDdiFunctionTable( clientType );
        ML_ASSERT( functionTable != nullptr );

        return functionTable
            ? functionTable->pfnContextCreate_1_0( clientType, createData, handle )
            : StatusCode::NullPointer;
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Export of metrics_library api entry ContextDelete_1_0.
    /// @param  handle
    /// @return StatusCode
    //////////////////////////////////////////////////////////////////////////
    ML_EXPORT( StatusCode ) ContextDelete_1_0( const ContextHandle_1_0 handle )
    {
        // Obtain function table.
        auto functionTable = GetDdiFunctionTable( handle );
        ML_ASSERT( functionTable != nullptr );

        return functionTable
            ? functionTable->pfnContextDelete_1_0( handle )
            : StatusCode::NullPointer;
    }

#if defined( __cplusplus )
}
#endif // C++
