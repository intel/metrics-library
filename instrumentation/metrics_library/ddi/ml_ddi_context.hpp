/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_context.hpp

@brief Partially auto-generated file. Provides implementation for Metrics Library
       entry points.

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_ddi
Template:           Tools/MetricsLibraryGenerator/templates/hpp.h
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Functions for DdiContext.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct DdiContext
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for ContextCreate_1_0
        /// @param clientType
        /// @param createData
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL ContextCreate_1_0(
            ClientType_1_0         clientType,
            ContextCreateData_1_0* createData,
            ContextHandle_1_0*     handle )
        {
            ML_FUNCTION_LOG_STATIC( StatusCode::Success );
            ML_FUNCTION_CHECK( createData != nullptr );
            ML_FUNCTION_CHECK( handle != nullptr );

            // Print build number.
            T::FunctionLogStatic::Traits( "Major number", METRICS_LIBRARY_MAJOR_NUMBER );
            T::FunctionLogStatic::Traits( "Minor number", METRICS_LIBRARY_MINOR_NUMBER );
            T::FunctionLogStatic::Traits( "Build number", METRICS_LIBRARY_BUILD_NUMBER );

            // Print input values.
            log.Input( clientType );
            log.Input( *createData );

            log.m_Result = T::Context::Create( clientType, *createData, *handle );

            // Print output values.
            log.Output( *createData );
            log.Output( *handle );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for ContextDelete_1_0
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL ContextDelete_1_0(
            const ContextHandle_1_0 handle )
        {
            ML_FUNCTION_CHECK_STATIC( T::Context::IsValid( handle ) );

            auto& context = T::Context::FromHandle( handle );
            ML_FUNCTION_LOG( StatusCode::Success, &context );

            // Print input values.
            log.Input( handle );

            log.m_Result  = T::Context::Delete( handle );
            log.m_Context = nullptr;

            ML_ASSERT( log.m_Result == StatusCode::Success );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for GetParameter_1_0
        /// @param parameter
        /// @param type
        /// @param value
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL GetParameter_1_0(
            const ParameterType parameter,
            ValueType*          type,
            TypedValue_1_0*     value )
        {
            ML_FUNCTION_LOG_STATIC( StatusCode::Success );
            ML_FUNCTION_CHECK( type != nullptr );
            ML_FUNCTION_CHECK( value != nullptr );

            // Print input values.
            log.Input( parameter );

            switch( parameter )
            {
                case ParameterType::QueryHwCountersReportApiSize:
                    *type              = ValueType::Uint32;
                    value->ValueUInt32 = sizeof( TT::Layouts::HwCounters::Query::ReportApi );
                    break;

                case ParameterType::QueryHwCountersReportGpuSize:
                    *type              = ValueType::Uint32;
                    value->ValueUInt32 = sizeof( TT::Layouts::HwCounters::Query::ReportGpu );
                    break;

                case ParameterType::LibraryBuildNumber:
                    *type              = ValueType::Uint32;
                    value->ValueUInt32 = METRICS_LIBRARY_BUILD_NUMBER;
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    log.m_Result = StatusCode::NotImplemented;
                    break;
            }

            value->Type = *type;

            // Print output values.
            log.Output( *type );
            log.Output( *value );

            ML_ASSERT( log.m_Result == StatusCode::Success );

            return log.m_Result;
        }
    };
} // namespace ML
