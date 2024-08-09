/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_report.hpp

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
    /// @brief Base functions for DdiReport.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct DdiReport
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for GetData_1_0
        /// @param data
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL GetData_1_0(
            GetReportData_1_0* data )
        {
            ML_FUNCTION_CHECK_STATIC( data != nullptr );

            switch( data->Type )
            {
                case ObjectType::QueryHwCounters:
                {
                    auto& context = T::Queries::HwCounters::FromHandle( data->Query.Handle ).m_Context;
                    ML_FUNCTION_LOG( StatusCode::Success, &context );

                    // Print input values.
                    log.Input( *data );

                    log.m_Result = T::Queries::HwCounters::GetData( data->Query );
                    ML_ASSERT( log.m_Result == StatusCode::Success || log.m_Result == StatusCode::ReportNotReady );

                    return log.m_Result;
                }
                case ObjectType::OverrideUser:
                {
                    auto& context = T::Overrides::User::FromHandle( data->Override.Handle ).m_Context;
                    ML_FUNCTION_LOG( StatusCode::Success, &context );

                    // Print input values.
                    log.Input( *data );

                    log.m_Result = T::Overrides::User::GetData( data->Override );
                    ML_ASSERT( log.m_Result == StatusCode::Success || log.m_Result == StatusCode::ReportNotReady );

                    return log.m_Result;
                }
                case ObjectType::OverridePoshQuery:
                {
                    auto& context = T::Overrides::PoshQuery::FromHandle( data->Override.Handle ).m_Context;
                    ML_FUNCTION_LOG( StatusCode::Success, &context );

                    // Print input values.
                    log.Input( *data );

                    log.m_Result = T::Overrides::PoshQuery::GetData( data->Override );
                    ML_ASSERT( log.m_Result == StatusCode::Success || log.m_Result == StatusCode::ReportNotReady );

                    return log.m_Result;
                }
                default:
                {
                    ML_FUNCTION_LOG_STATIC( StatusCode::IncorrectObject );
                    ML_ASSERT_ALWAYS();

                    // Print input values.
                    log.Input( *data );

                    return log.m_Result;
                }
            }
        }
    };
} // namespace ML::BASE

#pragma region Platform Specific Functions for DdiReport
namespace ML::GEN9
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN9-specific functions for DdiReport.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiReport = BASE::DdiReport<T>;
} // namespace ML::GEN9

namespace ML::GEN11
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN11-specific functions for DdiReport.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiReport = GEN9::DdiReport<T>;
} // namespace ML::GEN11

namespace ML::XE_LP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_LP-specific functions for DdiReport.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiReport = GEN11::DdiReport<T>;
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HP-specific functions for DdiReport.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiReport = XE_LP::DdiReport<T>;
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPG-specific functions for DdiReport.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiReport = XE_HP::DdiReport<T>;
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPC-specific functions for DdiReport.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiReport = XE_HPG::DdiReport<T>;
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE2_HPG-specific functions for DdiReport.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiReport = XE_HPG::DdiReport<T>;
} // namespace ML::XE2_HPG

#pragma endregion
