/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

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

namespace ML::BASE
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
            ML_FUNCTION_LOG_STATIC( StatusCode::Success );
            ML_FUNCTION_CHECK( createData != nullptr );
            ML_FUNCTION_CHECK( handle != nullptr );

            // Print traits properties.
            T::PrintBuildNumber();
            T::PrintClient();
            T::PrintGpuType();
            T::PrintTraits();

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
} // namespace ML::BASE

#pragma region Platform Specific Functions for DdiContext
namespace ML::GEN9
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN9-specific functions for DdiContext.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiContext = BASE::DdiContext<T>;
} // namespace ML::GEN9

namespace ML::GEN11
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN11-specific functions for DdiContext.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiContext = GEN9::DdiContext<T>;
} // namespace ML::GEN11

namespace ML::XE_LP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_LP-specific functions for DdiContext.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiContext = GEN11::DdiContext<T>;
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HP-specific functions for DdiContext.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiContext = XE_LP::DdiContext<T>;
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPG-specific functions for DdiContext.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiContext = XE_HP::DdiContext<T>;
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPC-specific functions for DdiContext.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiContext = XE_HPG::DdiContext<T>;
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE2_HPG-specific functions for DdiContext.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiContext = XE_HPG::DdiContext<T>;
} // namespace ML::XE2_HPG

#pragma endregion
