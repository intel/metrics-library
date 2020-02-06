/******************************************************************************\

Copyright © 2020, Intel Corporation

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

@file ml_marker_stream_user_extended.h

@brief Allows to create, write marker stream user extended and get data
       from the marker.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
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
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "MarkerStreamUserExtendedTrait<Traits>";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes marker stream user extended commands to command buffer.
            /// @param  buffer  target command buffer.
            /// @param  data    marker stream user extended data.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode Write(
                CommandBuffer&                                   /*buffer*/,
                const CommandBufferMarkerStreamUserExtended_1_0& /*data*/ )
            {
                ML_FUNCTION_LOG( StatusCode::NotImplemented );
                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes marker stream user extended commands to a chosen
            ///         graphics pipeline.
            /// @param  buffer          target command buffer.
            /// @param  data            marker stream user extended data.
            /// @param  registerAddress register address.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode WriteToPipeline(
                CommandBuffer&                                   buffer,
                const CommandBufferMarkerStreamUserExtended_1_0& data,
                const uint32_t                                   registerAddress )
            {
                ML_FUNCTION_LOG( StatusCode::NotImplemented );
                return log.m_Result;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct MarkerStreamUserExtendedTrait : BASE::MarkerStreamUserExtendedTrait<T>
        {
            ML_DECLARE_TRAIT( MarkerStreamUserExtendedTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct MarkerStreamUserExtendedTrait : GEN9::MarkerStreamUserExtendedTrait<T>
        {
            ML_DECLARE_TRAIT( MarkerStreamUserExtendedTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct MarkerStreamUserExtendedTrait : GEN11::MarkerStreamUserExtendedTrait<T>
        {
            ML_DECLARE_TRAIT( MarkerStreamUserExtendedTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
