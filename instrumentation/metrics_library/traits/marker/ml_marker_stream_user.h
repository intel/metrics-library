/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_marker_stream_user.h

@brief Allows to create, write marker stream user and get data from the marker.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for MarkerStreamUserTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct MarkerStreamUserTrait : DdiObject<T, TT::Markers::StreamUser, MarkerHandle_1_0, ObjectType::MarkerStreamUser>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( MarkerStreamUserTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( MarkerStreamUserTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Markers::StreamUser, MarkerHandle_1_0, ObjectType::MarkerStreamUser>;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "MarkerStreamUserTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes marker stream user commands to command buffer.
        /// @param  buffer  target command buffer.
        /// @param  data    marker stream user data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode Write(
            CommandBuffer&                           buffer,
            const CommandBufferMarkerStreamUser_1_0& data )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            // Load a value to A20 (gen 9) or A19 (gen11+) counter.
            ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                buffer,
                T::GpuRegisters::m_StreamMarker,
                data.Value ) );

            // Trigger report with report reason 4.
            ML_FUNCTION_CHECK( T::GpuCommands::TriggerStreamReport(
                buffer,
                data.Value ) );

            return log.m_Result;
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct MarkerStreamUserTrait : BASE::MarkerStreamUserTrait<T>
    {
        ML_DECLARE_TRAIT( MarkerStreamUserTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct MarkerStreamUserTrait : GEN9::MarkerStreamUserTrait<T>
    {
        ML_DECLARE_TRAIT( MarkerStreamUserTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct MarkerStreamUserTrait : GEN11::MarkerStreamUserTrait<T>
    {
        ML_DECLARE_TRAIT( MarkerStreamUserTrait, GEN11 );
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct MarkerStreamUserTrait : XE_LP::MarkerStreamUserTrait<T>
    {
        ML_DECLARE_TRAIT( MarkerStreamUserTrait, XE_LP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes marker stream user commands to command buffer.
        /// @param  buffer  target command buffer.
        /// @param  data    marker stream user data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode Write(
            CommandBuffer&                           buffer,
            const CommandBufferMarkerStreamUser_1_0& data )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            // Trigger report, which will be stored in oa buffer, in oag.
            // A given marker value will be inserted into the context id field
            // of the generated report.
            ML_FUNCTION_CHECK( T::GpuCommands::TriggerStreamReport(
                buffer,
                data.Value ) );

            return log.m_Result;
        }
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct MarkerStreamUserTrait : XE_HP::MarkerStreamUserTrait<T>
    {
        ML_DECLARE_TRAIT( MarkerStreamUserTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct MarkerStreamUserTrait : XE_HPG::MarkerStreamUserTrait<T>
    {
        ML_DECLARE_TRAIT( MarkerStreamUserTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct MarkerStreamUserTrait : XE_HPG::MarkerStreamUserTrait<T>
    {
        ML_DECLARE_TRAIT( MarkerStreamUserTrait, XE_HPG );
    };
} // namespace ML::XE2_HPG
