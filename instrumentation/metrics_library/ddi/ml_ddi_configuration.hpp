/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

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

namespace ML
{
    namespace BASE
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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( handle != nullptr );
                ML_FUNCTION_CHECK( createData != nullptr );
                ML_FUNCTION_CHECK( T::Context::IsValid( createData->HandleContext ) );

                auto& context = T::Context::FromHandle( createData->HandleContext );

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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( activateData != nullptr );

                switch( BaseObject::GetType( handle ) )
                {
                    case ObjectType::ConfigurationHwCountersOa:
                        log.m_Result = T::Configurations::HwCountersOa::FromHandle( handle ).Activate( *activateData );
                        break;

                    case ObjectType::ConfigurationHwCountersUser:
                        log.m_Result = StatusCode::Failed;
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        log.m_Result = StatusCode::IncorrectObject;
                        break;
                }

                return log.m_Result;
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
                        return T::Configurations::HwCountersOa::FromHandle( handle ).Deactivate();

                    case ObjectType::ConfigurationHwCountersUser:
                        return StatusCode::Failed;

                    default:
                        ML_ASSERT_ALWAYS();
                        return StatusCode::IncorrectObject;
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
                        return T::Configurations::HwCountersOa::Delete( handle );

                    case ObjectType::ConfigurationHwCountersUser:
                        return T::Configurations::HwCountersUser::Delete( handle );

                    default:
                        ML_ASSERT_ALWAYS();
                        return StatusCode::IncorrectObject;
                }
            }
        };
    } // namespace BASE

#pragma region Platform Specific Functions for DdiConfiguration
    namespace GEN9
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN9-specific functions for DdiConfiguration.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiConfiguration = BASE::DdiConfiguration<T>;
    } // namespace GEN9

    namespace GEN11
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN11-specific functions for DdiConfiguration.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiConfiguration = GEN9::DdiConfiguration<T>;
    } // namespace GEN11

    namespace GEN12
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN12-specific functions for DdiConfiguration.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiConfiguration = GEN11::DdiConfiguration<T>;
    } // namespace GEN12

#pragma endregion
} // namespace ML
