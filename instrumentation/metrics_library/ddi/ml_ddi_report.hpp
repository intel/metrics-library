/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

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

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Functions for DdiReport.
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
} // namespace ML
