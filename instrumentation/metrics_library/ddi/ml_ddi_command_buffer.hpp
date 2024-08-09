/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_command_buffer.hpp

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
    /// @brief Base functions for DdiCommandBuffer.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct DdiCommandBuffer
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for CommandBufferGet_1_0
        /// @param data
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL CommandBufferGet_1_0(
            const CommandBufferData_1_0* data )
        {
            ML_FUNCTION_CHECK_STATIC( data != nullptr );
            ML_FUNCTION_CHECK_STATIC( T::Context::IsValid( data->HandleContext ) );

            auto& context = T::Context::FromHandle( data->HandleContext );
            ML_FUNCTION_LOG( StatusCode::Success, &context );

            // Print input values.
            log.Input( *data );

            TT::GpuCommandBuffer buffer = { data->Data, data->Size, data->Type, data->Allocation, context };

            switch( data->CommandsType )
            {
                case ObjectType::QueryHwCounters:
                    log.m_Result = T::Queries::HwCounters::Write( buffer, data->Allocation, data->QueryHwCounters );
                    break;

                case ObjectType::QueryHwCountersCopyReports:
                    log.m_Result = T::Queries::HwCounters::CopyReports( buffer, data->QueryHwCountersCopyReports );
                    break;

                case ObjectType::OverrideUser:
                    log.m_Result = T::Overrides::User::Write( buffer, data->Override );
                    break;

                case ObjectType::OverrideFlushCaches:
                    log.m_Result = T::Overrides::FlushCaches::Write( buffer );
                    break;

                case ObjectType::OverrideNullHardware:
                    log.m_Result = T::Overrides::NullHardware::Write( buffer, data->Override.Enable );
                    break;

                case ObjectType::MarkerStreamUser:
                    log.m_Result = T::Markers::StreamUser::Write( buffer, data->MarkerStreamUser );
                    break;

                case ObjectType::MarkerStreamUserExtended:
                    log.m_Result = T::Markers::StreamUserExtended::Write( buffer, data->MarkerStreamUserExtended );
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    log.m_Result = StatusCode::IncorrectObject;
                    break;
            }

            ML_ASSERT( log.m_Result == StatusCode::Success );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for CommandBufferGetSize_1_0
        /// @param data
        /// @param size
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL CommandBufferGetSize_1_0(
            const CommandBufferData_1_0* data,
            CommandBufferSize_1_0*       size )
        {
            ML_FUNCTION_CHECK_STATIC( data != nullptr );
            ML_FUNCTION_CHECK_STATIC( size != nullptr );
            ML_FUNCTION_CHECK_STATIC( T::Context::IsValid( data->HandleContext ) );

            auto& context = T::Context::FromHandle( data->HandleContext );
            ML_FUNCTION_LOG( StatusCode::Success, &context );

            // Print input values.
            log.Input( *data );

            TT::GpuCommandBufferCalculator buffer = { data->Type, context };

            switch( data->CommandsType )
            {
                case ObjectType::QueryHwCounters:
                    log.m_Result = T::Queries::HwCounters::Write( buffer, data->Allocation, data->QueryHwCounters );
                    break;

                case ObjectType::QueryHwCountersCopyReports:
                    log.m_Result = T::Queries::HwCounters::CopyReports( buffer, data->QueryHwCountersCopyReports );
                    break;

                case ObjectType::OverrideUser:
                    log.m_Result = T::Overrides::User::Write( buffer, data->Override );
                    break;

                case ObjectType::OverrideNullHardware:
                    log.m_Result = T::Overrides::NullHardware::Write( buffer, data->Override.Enable );
                    break;

                case ObjectType::OverrideFlushCaches:
                    log.m_Result = T::Overrides::FlushCaches::Write( buffer );
                    break;

                case ObjectType::MarkerStreamUser:
                    log.m_Result = T::Markers::StreamUser::Write( buffer, data->MarkerStreamUser );
                    break;

                case ObjectType::MarkerStreamUserExtended:
                    log.m_Result = T::Markers::StreamUserExtended::Write( buffer, data->MarkerStreamUserExtended );
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    log.m_Result = StatusCode::IncorrectObject;
                    break;
            }

            if( ML_SUCCESS( log.m_Result ) )
            {
                buffer.GetSizeRequirements( *size );

                // Print output values.
                log.Output( *size );
            }
            else
            {
                ML_ASSERT_ALWAYS();
            }

            return log.m_Result;
        }
    };
} // namespace ML::BASE

#pragma region Platform Specific Functions for DdiCommandBuffer
namespace ML::GEN9
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN9-specific functions for DdiCommandBuffer.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiCommandBuffer = BASE::DdiCommandBuffer<T>;
} // namespace ML::GEN9

namespace ML::GEN11
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN11-specific functions for DdiCommandBuffer.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiCommandBuffer = GEN9::DdiCommandBuffer<T>;
} // namespace ML::GEN11

namespace ML::XE_LP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_LP-specific functions for DdiCommandBuffer.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiCommandBuffer = GEN11::DdiCommandBuffer<T>;
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HP-specific functions for DdiCommandBuffer.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiCommandBuffer = XE_LP::DdiCommandBuffer<T>;
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPG-specific functions for DdiCommandBuffer.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiCommandBuffer = XE_HP::DdiCommandBuffer<T>;
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPC-specific functions for DdiCommandBuffer.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiCommandBuffer = XE_HPG::DdiCommandBuffer<T>;
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE2_HPG-specific functions for DdiCommandBuffer.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiCommandBuffer = XE_HPG::DdiCommandBuffer<T>;
} // namespace ML::XE2_HPG

#pragma endregion
