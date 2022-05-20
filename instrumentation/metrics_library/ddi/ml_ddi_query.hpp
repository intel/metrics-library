/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

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
                ML_FUNCTION_CHECK_STATIC( handle != nullptr );
                ML_FUNCTION_CHECK_STATIC( createData != nullptr );
                ML_FUNCTION_CHECK_STATIC( T::Context::IsValid( createData->HandleContext ) );

                auto& context = T::Context::FromHandle( createData->HandleContext );
                ML_FUNCTION_LOG( StatusCode::Success, &context );

                // Print input values.
                log.Input( createData );
                log.Input( handle );

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
                    {
                        auto& context = T::Queries::HwCounters::FromHandle( handle ).m_Context;
                        ML_FUNCTION_LOG( StatusCode::Success, &context );

                        // Print input values.
                        log.Input( handle );

                        log.m_Result = T::Queries::HwCounters::Delete( handle );
                        ML_ASSERT( log.m_Result == StatusCode::Success );

                        return log.m_Result;
                    }
                    case ObjectType::QueryPipelineTimestamps:
                    {
                        auto& context = T::Queries::HwCounters::FromHandle( handle ).m_Context;
                        ML_FUNCTION_LOG( StatusCode::Success, &context );

                        // Print input values.
                        log.Input( handle );

                        log.m_Result = T::Queries::PipelineTimestamps::Delete( handle );
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

    namespace XE_HP
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief XE_HP-specific functions for DdiQuery.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiQuery = XE_LP::DdiQuery<T>;
    } // namespace XE_HP

    namespace XE_HPG
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief XE_HPG-specific functions for DdiQuery.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiQuery = XE_HP::DdiQuery<T>;
    } // namespace XE_HPG

    namespace XE_HPC
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief XE_HPC-specific functions for DdiQuery.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiQuery = XE_HPG::DdiQuery<T>;
    } // namespace XE_HPC

#pragma endregion
} // namespace ML
