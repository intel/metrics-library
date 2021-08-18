/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_context.cpp

@brief Auto-generated file. Implements Metrics Library entry points
       for a given os / gen / api configuration.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_ddi
Template:           Tools/MetricsLibraryGenerator/templates/ddi.h
*/

#include "ml_pch.h"
#include "ml_ddi_context.hpp"

#if ML_ENABLE_GEN9

#include "ml_traits_gen9.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiContext.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN9
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextCreate_1_0(
                ClientType_1_0         clientType,
                ContextCreateData_1_0* createData,
                ContextHandle_1_0*     handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print traits properties.
                T::PrintBuildNumber();
                T::PrintClient();
                T::PrintGen();
                T::PrintTraits();

                // Print input values.
                log.Input( clientType );
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextCreate_1_0(
                    clientType,
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextDelete_1_0(
                const ContextHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetParameter_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetParameter_1_0(
                const ParameterType parameter,
                ValueType*          type,
                TypedValue_1_0*     value )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( parameter );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::GetParameter_1_0(
                    parameter,
                    type,
                    value );

                // Print output values.
                log.Output( type );
                log.Output( value );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextCreate_1_0(
                ClientType_1_0         clientType,
                ContextCreateData_1_0* createData,
                ContextHandle_1_0*     handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print traits properties.
                T::PrintBuildNumber();
                T::PrintClient();
                T::PrintGen();
                T::PrintTraits();

                // Print input values.
                log.Input( clientType );
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextCreate_1_0(
                    clientType,
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextDelete_1_0(
                const ContextHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetParameter_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetParameter_1_0(
                const ParameterType parameter,
                ValueType*          type,
                TypedValue_1_0*     value )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( parameter );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::GetParameter_1_0(
                    parameter,
                    type,
                    value );

                // Print output values.
                log.Output( type );
                log.Output( value );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace GEN9
} // namespace ML
#endif // ML_ENABLE_GEN9

#if ML_ENABLE_GEN11

#include "ml_traits_gen11.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiContext.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN11
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextCreate_1_0(
                ClientType_1_0         clientType,
                ContextCreateData_1_0* createData,
                ContextHandle_1_0*     handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print traits properties.
                T::PrintBuildNumber();
                T::PrintClient();
                T::PrintGen();
                T::PrintTraits();

                // Print input values.
                log.Input( clientType );
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextCreate_1_0(
                    clientType,
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextDelete_1_0(
                const ContextHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetParameter_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetParameter_1_0(
                const ParameterType parameter,
                ValueType*          type,
                TypedValue_1_0*     value )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( parameter );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::GetParameter_1_0(
                    parameter,
                    type,
                    value );

                // Print output values.
                log.Output( type );
                log.Output( value );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextCreate_1_0(
                ClientType_1_0         clientType,
                ContextCreateData_1_0* createData,
                ContextHandle_1_0*     handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print traits properties.
                T::PrintBuildNumber();
                T::PrintClient();
                T::PrintGen();
                T::PrintTraits();

                // Print input values.
                log.Input( clientType );
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextCreate_1_0(
                    clientType,
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextDelete_1_0(
                const ContextHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetParameter_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetParameter_1_0(
                const ParameterType parameter,
                ValueType*          type,
                TypedValue_1_0*     value )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( parameter );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::GetParameter_1_0(
                    parameter,
                    type,
                    value );

                // Print output values.
                log.Output( type );
                log.Output( value );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace GEN11
} // namespace ML
#endif // ML_ENABLE_GEN11

#if ML_ENABLE_GEN12

#include "ml_traits_gen12.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiContext.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN12
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextCreate_1_0(
                ClientType_1_0         clientType,
                ContextCreateData_1_0* createData,
                ContextHandle_1_0*     handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print traits properties.
                T::PrintBuildNumber();
                T::PrintClient();
                T::PrintGen();
                T::PrintTraits();

                // Print input values.
                log.Input( clientType );
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextCreate_1_0(
                    clientType,
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextDelete_1_0(
                const ContextHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetParameter_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetParameter_1_0(
                const ParameterType parameter,
                ValueType*          type,
                TypedValue_1_0*     value )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( parameter );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::GetParameter_1_0(
                    parameter,
                    type,
                    value );

                // Print output values.
                log.Output( type );
                log.Output( value );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextCreate_1_0(
                ClientType_1_0         clientType,
                ContextCreateData_1_0* createData,
                ContextHandle_1_0*     handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print traits properties.
                T::PrintBuildNumber();
                T::PrintClient();
                T::PrintGen();
                T::PrintTraits();

                // Print input values.
                log.Input( clientType );
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextCreate_1_0(
                    clientType,
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for ContextDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextDelete_1_0(
                const ContextHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::ContextDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetParameter_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetParameter_1_0(
                const ParameterType parameter,
                ValueType*          type,
                TypedValue_1_0*     value )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( parameter );

                // Call entry point implementation.
                log.m_Result = DdiContext<T>::GetParameter_1_0(
                    parameter,
                    type,
                    value );

                // Print output values.
                log.Output( type );
                log.Output( value );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace GEN12
} // namespace ML
#endif // ML_ENABLE_GEN12

