/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

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
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base functions for DdiContext.
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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( createData != nullptr );
                ML_FUNCTION_CHECK( handle != nullptr );

                return log.m_Result = T::Context::Create( clientType, *createData, *handle );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Generic implementation for ContextDelete_1_0
            /// @param handle
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static StatusCode ML_STDCALL ContextDelete_1_0(
                const ContextHandle_1_0 handle )
            {
                return T::Context::Delete( handle );
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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( type != nullptr );
                ML_FUNCTION_CHECK( value != nullptr );

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

                    case ParameterType::QueryPipelineTimestampsReportApiSize:
                        *type              = ValueType::Uint32;
                        value->ValueUInt32 = sizeof( TT::Layouts::PipelineTimestamps::ReportApi );
                        break;

                    case ParameterType::QueryPipelineTimestampsReportGpuSize:
                        *type              = ValueType::Uint32;
                        value->ValueUInt32 = sizeof( TT::Layouts::PipelineTimestamps::ReportGpu );
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

                return log.m_Result;
            }
        };
    } // namespace BASE

#pragma region Platform Specific Functions for DdiContext
    namespace GEN9
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN9-specific functions for DdiContext.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiContext = BASE::DdiContext<T>;
    } // namespace GEN9

    namespace GEN11
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN11-specific functions for DdiContext.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiContext = GEN9::DdiContext<T>;
    } // namespace GEN11

    namespace GEN12
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN12-specific functions for DdiContext.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiContext = GEN11::DdiContext<T>;
    } // namespace GEN12

#pragma endregion
} // namespace ML
