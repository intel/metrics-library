/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

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
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base functions for DdiQuery.
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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( handle != nullptr );
                ML_FUNCTION_CHECK( createData != nullptr );
                ML_FUNCTION_CHECK( T::Context::IsValid( createData->HandleContext ) );

                switch( createData->Type )
                {
                    case ObjectType::QueryHwCounters:
                        log.m_Result = T::Queries::HwCounters::Create( *createData, *handle );
                        break;

                    case ObjectType::QueryPipelineTimestamps:
                        log.m_Result = T::Queries::PipelineTimestamps::Create( *createData, *handle );
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        log.m_Result = StatusCode::IncorrectParameter;
                        break;
                }

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
                        return T::Queries::HwCounters::Delete( handle );

                    case ObjectType::QueryPipelineTimestamps:
                        return T::Queries::PipelineTimestamps::Delete( handle );

                    default:
                        ML_ASSERT_ALWAYS();
                        return StatusCode::IncorrectObject;
                }
            }
        };
    } // namespace BASE

#pragma region Platform Specific Functions for DdiQuery
    namespace GEN9
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN9-specific functions for DdiQuery.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiQuery = BASE::DdiQuery<T>;
    } // namespace GEN9

    namespace GEN11
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN11-specific functions for DdiQuery.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiQuery = GEN9::DdiQuery<T>;
    } // namespace GEN11

    namespace XE_LP
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief XE_LP-specific functions for DdiQuery.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiQuery = GEN11::DdiQuery<T>;
    } // namespace XE_LP

#pragma endregion
} // namespace ML
