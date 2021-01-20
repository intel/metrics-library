/******************************************************************************\

Copyright © 2021, Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

@file ml_marker_stream_user.h

@brief Allows to create, write marker stream user and get data from the marker.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
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
                ML_FUNCTION_LOG( StatusCode::Success );

                const uint32_t marker = T::Policy::StreamMarker::m_Use32bitValue
                    ? data.Value | ( data.Reserved << Constants::StreamMarker::m_HighBitsShift )
                    : data.Value;

                // Load a value to A19 counter.
                ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_StreamMarker,
                    marker ) );

                // Trigger report with report reason 4.
                ML_FUNCTION_CHECK( T::GpuCommands::TriggerStreamReport( buffer ) );

                return log.m_Result;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct MarkerStreamUserTrait : BASE::MarkerStreamUserTrait<T>
        {
            ML_DECLARE_TRAIT( MarkerStreamUserTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct MarkerStreamUserTrait : GEN9::MarkerStreamUserTrait<T>
        {
            ML_DECLARE_TRAIT( MarkerStreamUserTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct MarkerStreamUserTrait : GEN11::MarkerStreamUserTrait<T>
        {
            ML_DECLARE_TRAIT( MarkerStreamUserTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
