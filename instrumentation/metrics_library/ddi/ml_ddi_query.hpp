/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_query.hpp

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
    /// @brief Functions for DdiQuery.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct DdiQuery
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for QueryCreate_1_0
        /// @param createData
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL QueryCreate_1_0(
            const QueryCreateData_1_0* createData,
            QueryHandle_1_0*           handle )
        {
            ML_FUNCTION_CHECK_STATIC( handle != nullptr );
            ML_FUNCTION_CHECK_STATIC( createData != nullptr );
            ML_FUNCTION_CHECK_STATIC( T::Context::IsValid( createData->HandleContext ) );

            auto& context = T::Context::FromHandle( createData->HandleContext );
            ML_FUNCTION_LOG( StatusCode::Success, &context );

            // Print input values.
            log.Input( *createData );

            switch( createData->Type )
            {
                case ObjectType::QueryHwCounters:
                    log.m_Result = T::Queries::HwCounters::Create( *createData, *handle );
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    log.m_Result = StatusCode::IncorrectParameter;
                    break;
            }

            // Print input values.
            log.Output( *handle );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for QueryDelete_1_0
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL QueryDelete_1_0(
            const QueryHandle_1_0 handle )
        {
            switch( BaseObject::GetType( handle ) )
            {
                case ObjectType::QueryHwCounters:
                {
                    auto& context = T::Queries::HwCounters::FromHandle( handle ).m_Context;
                    ML_FUNCTION_LOG( StatusCode::Success, &context );

                    // Print input values.
                    log.Input( handle );

                    log.m_Result = T::Queries::HwCounters::Delete( handle );
                    ML_ASSERT( log.m_Result == StatusCode::Success );

                    return log.m_Result;
                }
                default:
                {
                    ML_FUNCTION_LOG_STATIC( StatusCode::IncorrectObject );
                    ML_ASSERT_ALWAYS();

                    // Print input values.
                    log.Input( handle );

                    return log.m_Result;
                }
            }
        }
    };
} // namespace ML
