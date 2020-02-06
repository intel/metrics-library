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

@file ml_override_user.h

@brief Creates, activates and deactivates override by writing configuration
       to registers.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
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
                TT::Context& /*context*/,
                OverrideHandle_1_0& /*handle*/ )
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
                CommandBuffer& /*buffer*/,
                const CommandBufferOverride_1_0& /*data*/ )
            {
                return StatusCode::NotImplemented;
            }

            ////////////////////////////////////////////////////////////////////////
            /// @brief  Return override reports.
            /// @return getData data requested by client.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static StatusCode GetData( const GetReportOverride_1_0& /*getData*/ )
            {
                return StatusCode::NotImplemented;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct OverrideUserTrait : BASE::OverrideUserTrait<T>
        {
            ML_DECLARE_TRAIT( OverrideUserTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct OverrideUserTrait : GEN9::OverrideUserTrait<T>
        {
            ML_DECLARE_TRAIT( OverrideUserTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct OverrideUserTrait : GEN11::OverrideUserTrait<T>
        {
            ML_DECLARE_TRAIT( OverrideUserTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
