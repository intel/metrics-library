/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_override_user.h

@brief Creates, activates and deactivates override by writing configuration
       to registers.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for OverrideUserTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct OverrideUserTrait : DdiObject<T, TT::Overrides::User, OverrideHandle_1_0, ObjectType::OverrideUser>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( OverrideUserTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( OverrideUserTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Overrides::User, OverrideHandle_1_0, ObjectType::OverrideUser>;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "OverrideUserTrait<Traits> (Linux)";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Creates user override configuration.
        /// @param  context library context.
        /// @return handle  override handle.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode Create(
            [[maybe_unused]] TT::Context&        context,
            [[maybe_unused]] OverrideHandle_1_0& handle )
        {
            return StatusCode::NotImplemented;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes null hardware override commands to command buffer.
        /// @param  buffer  command buffer.
        /// @param  data    override data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode Write(
            [[maybe_unused]] CommandBuffer&                   buffer,
            [[maybe_unused]] const CommandBufferOverride_1_0& data )
        {
            return StatusCode::NotImplemented;
        }

        ////////////////////////////////////////////////////////////////////////
        /// @brief  Return override reports.
        /// @return getData data requested by client.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode GetData( [[maybe_unused]] const GetReportOverride_1_0& getData )
        {
            return StatusCode::NotImplemented;
        }
    };
} // namespace ML
