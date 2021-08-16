/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

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

namespace ML
{
    namespace BASE
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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( data != nullptr );
                ML_FUNCTION_CHECK( T::Context::IsValid( data->HandleContext ) );

                TT::Context&         context = T::Context::FromHandle( data->HandleContext );
                TT::GpuCommandBuffer buffer  = { data->Data, data->Size, data->Type, data->Allocation, context };

                switch( data->CommandsType )
                {
                    case ObjectType::QueryHwCounters:
                        log.m_Result = T::Queries::HwCounters::Write( buffer, data->Allocation, data->QueryHwCounters );
                        break;

                    case ObjectType::QueryHwCountersCopyReports:
                        log.m_Result = T::Queries::HwCounters::CopyReports( buffer, data->QueryHwCountersCopyReports );
                        break;

                    case ObjectType::QueryPipelineTimestamps:
                        log.m_Result = T::Queries::PipelineTimestamps::Write( buffer, data->Allocation, *data );
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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( data != nullptr );
                ML_FUNCTION_CHECK( size != nullptr );
                ML_FUNCTION_CHECK( T::Context::IsValid( data->HandleContext ) );

                TT::Context&                   context = T::Context::FromHandle( data->HandleContext );
                TT::GpuCommandBufferCalculator buffer  = { data->Type, context };

                switch( data->CommandsType )
                {
                    case ObjectType::QueryHwCounters:
                        log.m_Result = T::Queries::HwCounters::Write( buffer, data->Allocation, data->QueryHwCounters );
                        break;

                    case ObjectType::QueryHwCountersCopyReports:
                        log.m_Result = T::Queries::HwCounters::CopyReports( buffer, data->QueryHwCountersCopyReports );
                        break;

                    case ObjectType::QueryPipelineTimestamps:
                        log.m_Result = T::Queries::PipelineTimestamps::Write( buffer, data->Allocation, *data );
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

                return ML_SUCCESS( log.m_Result )
                    ? buffer.GetSizeRequirements( *size )
                    : log.m_Result;
            }
        };
    } // namespace BASE

#pragma region Platform Specific Functions for DdiCommandBuffer
    namespace GEN9
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN9-specific functions for DdiCommandBuffer.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiCommandBuffer = BASE::DdiCommandBuffer<T>;
    } // namespace GEN9

    namespace GEN11
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN11-specific functions for DdiCommandBuffer.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiCommandBuffer = GEN9::DdiCommandBuffer<T>;
    } // namespace GEN11

    namespace GEN12
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN12-specific functions for DdiCommandBuffer.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiCommandBuffer = GEN11::DdiCommandBuffer<T>;
    } // namespace GEN12

#pragma endregion
} // namespace ML
