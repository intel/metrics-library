/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_configuration.hpp

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
    /// @brief Base functions for DdiConfiguration.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct DdiConfiguration
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for ConfigurationCreate_1_0
        /// @param createData
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL ConfigurationCreate_1_0(
            const ConfigurationCreateData_1_0* createData,
            ConfigurationHandle_1_0*           handle )
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
                case ObjectType::ConfigurationHwCountersOa:
                    log.m_Result = T::Configurations::HwCountersOa::Create( context, *handle );
                    break;

                case ObjectType::ConfigurationHwCountersUser:
                    log.m_Result = T::Configurations::HwCountersUser::Create( context, *handle );
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
        /// @brief Generic implementation for ConfigurationActivate_1_0
        /// @param handle
        /// @param activateData
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL ConfigurationActivate_1_0(
            const ConfigurationHandle_1_0        handle,
            const ConfigurationActivateData_1_0* activateData )
        {
            ML_FUNCTION_CHECK_STATIC( activateData != nullptr );

            switch( BaseObject::GetType( handle ) )
            {
                case ObjectType::ConfigurationHwCountersOa:
                {
                    auto& config = T::Configurations::HwCountersOa::FromHandle( handle );
                    ML_FUNCTION_LOG( StatusCode::Success, &config.m_Context );

                    // Print input values.
                    log.Input( handle );
                    log.Input( *activateData );

                    log.m_Result = config.Activate( *activateData );
                    ML_ASSERT( log.m_Result == StatusCode::Success );

                    return log.m_Result;
                }
                case ObjectType::ConfigurationHwCountersUser:
                {
                    ML_FUNCTION_LOG_STATIC( StatusCode::Failed );

                    // Print input values.
                    log.Input( handle );
                    log.Input( *activateData );

                    ML_ASSERT( log.m_Result == StatusCode::Success );

                    return log.m_Result;
                }
                default:
                {
                    ML_FUNCTION_LOG_STATIC( StatusCode::IncorrectObject );
                    ML_ASSERT_ALWAYS();

                    // Print input values.
                    log.Input( handle );
                    log.Input( *activateData );

                    return log.m_Result;
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for ConfigurationDeactivate_1_0
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL ConfigurationDeactivate_1_0(
            const ConfigurationHandle_1_0 handle )
        {
            switch( BaseObject::GetType( handle ) )
            {
                case ObjectType::ConfigurationHwCountersOa:
                {
                    auto& config = T::Configurations::HwCountersOa::FromHandle( handle );
                    ML_FUNCTION_LOG( StatusCode::Success, &config.m_Context );

                    // Print input values.
                    log.Input( handle );

                    log.m_Result = config.Deactivate();
                    ML_ASSERT( log.m_Result == StatusCode::Success );

                    return log.m_Result;
                }
                case ObjectType::ConfigurationHwCountersUser:
                {
                    ML_FUNCTION_LOG_STATIC( StatusCode::Failed );

                    // Print input values.
                    log.Input( handle );

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

        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for ConfigurationDelete_1_0
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL ConfigurationDelete_1_0(
            const ConfigurationHandle_1_0 handle )
        {
            switch( BaseObject::GetType( handle ) )
            {
                case ObjectType::ConfigurationHwCountersOa:
                {
                    auto& context = T::Configurations::HwCountersOa::FromHandle( handle ).m_Context;
                    ML_FUNCTION_LOG( StatusCode::Success, &context );

                    // Print input values.
                    log.Input( handle );

                    log.m_Result = T::Configurations::HwCountersOa::Delete( handle );
                    ML_ASSERT( log.m_Result == StatusCode::Success );

                    return log.m_Result;
                }
                case ObjectType::ConfigurationHwCountersUser:
                {
                    auto& context = T::Configurations::HwCountersUser::FromHandle( handle ).m_Context;
                    ML_FUNCTION_LOG( StatusCode::Success, &context );

                    // Print input values.
                    log.Input( handle );

                    log.m_Result = T::Configurations::HwCountersUser::Delete( handle );
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

#pragma region Platform Specific Functions for DdiConfiguration
namespace ML::GEN9
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN9-specific functions for DdiConfiguration.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiConfiguration = BASE::DdiConfiguration<T>;
} // namespace ML::GEN9

namespace ML::GEN11
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN11-specific functions for DdiConfiguration.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiConfiguration = GEN9::DdiConfiguration<T>;
} // namespace ML::GEN11

namespace ML::XE_LP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_LP-specific functions for DdiConfiguration.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiConfiguration = GEN11::DdiConfiguration<T>;
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HP-specific functions for DdiConfiguration.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiConfiguration = XE_LP::DdiConfiguration<T>;
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPG-specific functions for DdiConfiguration.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiConfiguration = XE_HP::DdiConfiguration<T>;
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPC-specific functions for DdiConfiguration.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiConfiguration = XE_HPG::DdiConfiguration<T>;
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE2_HPG-specific functions for DdiConfiguration.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiConfiguration = XE_HPG::DdiConfiguration<T>;
} // namespace ML::XE2_HPG

#pragma endregion
