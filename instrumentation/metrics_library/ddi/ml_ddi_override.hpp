/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_override.hpp

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
    /// @brief Base functions for DdiOverride.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct DdiOverride
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for OverrideCreate_1_0
        /// @param createData
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL OverrideCreate_1_0(
            const OverrideCreateData_1_0* createData,
            OverrideHandle_1_0*           handle )
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
                case ObjectType::OverrideUser:
                    log.m_Result = T::Overrides::User::Create( context, *handle );
                    break;

                case ObjectType::OverrideNullHardware:
                    log.m_Result = StatusCode::NotImplemented;
                    break;

                case ObjectType::OverrideFlushCaches:
                    log.m_Result = StatusCode::NotImplemented;
                    break;

                case ObjectType::OverridePoshQuery:
                    log.m_Result = T::Overrides::PoshQuery::Create( context, *handle );
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    log.m_Result = StatusCode::IncorrectParameter;
                    break;
            }

            ML_ASSERT( log.m_Result == StatusCode::Success );

            // Print output values.
            log.Output( *handle );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for OverrideDelete_1_0
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL OverrideDelete_1_0(
            const OverrideHandle_1_0 handle )
        {
            switch( BaseObject::GetType( handle ) )
            {
                case ObjectType::OverrideUser:
                {
                    auto& context = T::Overrides::User::FromHandle( handle ).m_Context;
                    ML_FUNCTION_LOG( StatusCode::Success, &context );

                    // Print input values.
                    log.Input( handle );

                    log.m_Result = T::Overrides::User::Delete( handle );
                    ML_ASSERT( log.m_Result == StatusCode::Success );

                    return log.m_Result;
                }
                case ObjectType::OverrideNullHardware:
                {
                    ML_FUNCTION_LOG_STATIC( StatusCode::NotImplemented );

                    // Print input values.
                    log.Input( handle );

                    ML_ASSERT( log.m_Result == StatusCode::Success );

                    return log.m_Result;
                }
                case ObjectType::OverrideFlushCaches:
                {
                    ML_FUNCTION_LOG_STATIC( StatusCode::NotImplemented );

                    // Print input values.
                    log.Input( handle );

                    ML_ASSERT( log.m_Result == StatusCode::Success );

                    return log.m_Result;
                }
                case ObjectType::OverridePoshQuery:
                {
                    auto& context = T::Overrides::PoshQuery::FromHandle( handle ).m_Context;
                    ML_FUNCTION_LOG( StatusCode::Success, &context );

                    // Print input values.
                    log.Input( handle );

                    log.m_Result = T::Overrides::PoshQuery::Delete( handle );
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
} // namespace ML::BASE

#pragma region Platform Specific Functions for DdiOverride
namespace ML::GEN9
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN9-specific functions for DdiOverride.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiOverride = BASE::DdiOverride<T>;
} // namespace ML::GEN9

namespace ML::GEN11
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN11-specific functions for DdiOverride.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiOverride = GEN9::DdiOverride<T>;
} // namespace ML::GEN11

namespace ML::XE_LP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_LP-specific functions for DdiOverride.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiOverride = GEN11::DdiOverride<T>;
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HP-specific functions for DdiOverride.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiOverride = XE_LP::DdiOverride<T>;
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPG-specific functions for DdiOverride.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiOverride = XE_HP::DdiOverride<T>;
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPC-specific functions for DdiOverride.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiOverride = XE_HPG::DdiOverride<T>;
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE2_HPG-specific functions for DdiOverride.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiOverride = XE_HPG::DdiOverride<T>;
} // namespace ML::XE2_HPG

#pragma endregion
