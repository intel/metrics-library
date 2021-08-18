/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

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
    namespace BASE
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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( data != nullptr );

                switch( data->Type )
                {
                    case ObjectType::QueryHwCounters:
                        log.m_Result = T::Queries::HwCounters::GetData( data->Query );
                        break;

                    case ObjectType::QueryPipelineTimestamps:
                        log.m_Result = T::Queries::PipelineTimestamps::GetData( data->Query );
                        break;

                    case ObjectType::OverrideUser:
                        log.m_Result = T::Overrides::User::GetData( data->Override );
                        break;

                    case ObjectType::OverridePoshQuery:
                        log.m_Result = T::Overrides::PoshQuery::GetData( data->Override );
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        log.m_Result = StatusCode::IncorrectObject;
                        break;
                }

                return log.m_Result;
            }
        };
    } // namespace BASE

#pragma region Platform Specific Functions for DdiReport
    namespace GEN9
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN9-specific functions for DdiReport.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiReport = BASE::DdiReport<T>;
    } // namespace GEN9

    namespace GEN11
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN11-specific functions for DdiReport.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiReport = GEN9::DdiReport<T>;
    } // namespace GEN11

    namespace GEN12
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN12-specific functions for DdiReport.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiReport = GEN11::DdiReport<T>;
    } // namespace GEN12

#pragma endregion
} // namespace ML
