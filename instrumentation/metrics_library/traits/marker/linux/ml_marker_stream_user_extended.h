/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_marker_stream_user_extended.h

@brief Allows to create, write marker stream user extended and get data
       from the marker.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for MarkerStreamUserExtendedExtendedTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct MarkerStreamUserExtendedTrait : DdiObject<T, TT::Markers::StreamUserExtended, MarkerHandle_1_0, ObjectType::MarkerStreamUserExtended>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( MarkerStreamUserExtendedTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( MarkerStreamUserExtendedTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Markers::StreamUserExtended, MarkerHandle_1_0, ObjectType::MarkerStreamUserExtended>;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes marker stream user extended commands to command buffer.
        /// @param  buffer  target command buffer.
        /// @param  data    marker stream user extended data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode Write(
            CommandBuffer&                                                    buffer,
            [[maybe_unused]] const CommandBufferMarkerStreamUserExtended_1_0& data )
        {
            ML_FUNCTION_LOG( StatusCode::NotImplemented, &buffer.m_Context );
            return log.m_Result;
        }
    };
} // namespace ML
