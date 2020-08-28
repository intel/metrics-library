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

@file ml_override_null_hardware.h

@brief Enables null hardware override.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for OverrideNullHardwareTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct OverrideNullHardwareTrait : DdiObject<T, TT::Overrides::NullHardware, OverrideHandle_1_0, ObjectType::OverrideNullHardware>
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( OverrideNullHardwareTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( OverrideNullHardwareTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Types.
            //////////////////////////////////////////////////////////////////////////
            using Base = DdiObject<T, TT::Overrides::NullHardware, OverrideHandle_1_0, ObjectType::OverrideNullHardware>;

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "OverrideNullHardwareTrait<Traits> (Windows)";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes null hardware override command to command buffer.
            /// @param  buffer  command buffer.
            /// @param  enable  flag that indicates whether enable or disable override.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode Write(
                CommandBuffer& buffer,
                const bool     enable )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                ML_FUNCTION_CHECK( buffer.m_Type == GpuCommandBufferType::Render );

                return log.m_Result = enable
                    ? Activate( buffer )
                    : Deactivate( buffer );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Activates null hardware override.
            /// @param  buffer  target command buffer.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode Activate( CommandBuffer& buffer )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const auto&  registers     = T::GpuRegisters::GetNullHardwareEnablingRegisters();
                const size_t registerCount = registers.size();

                ML_FUNCTION_CHECK( registerCount % 2 == 0 );

                for( size_t i = 0; i < registerCount; i += 2 )
                {
                    ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                        buffer,
                        registers[i],
                        registers[i + 1] ) );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Deactivates null hardware override.
            /// @param  buffer  target command buffer.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode Deactivate( CommandBuffer& buffer )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const auto&  registers     = T::GpuRegisters::GetNullHardwareDisablingRegisters();
                const size_t registerCount = registers.size();

                ML_FUNCTION_CHECK( registerCount % 2 == 0 );

                for( size_t i = 0; i < registerCount; i += 2 )
                {
                    ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                        buffer,
                        registers[i],
                        registers[i + 1] ) );
                }

                return log.m_Result;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct OverrideNullHardwareTrait : BASE::OverrideNullHardwareTrait<T>
        {
            ML_DECLARE_TRAIT( OverrideNullHardwareTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct OverrideNullHardwareTrait : GEN9::OverrideNullHardwareTrait<T>
        {
            ML_DECLARE_TRAIT( OverrideNullHardwareTrait, GEN9 );

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes null hardware override command to command buffer.
            /// @param  buffer  command buffer.
            /// @param  enable  flag that indicates whether enable or disable override.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode Write(
                CommandBuffer& buffer,
                const bool     enable )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Render command streamer.
                if( buffer.m_Type == GpuCommandBufferType::Render )
                {
                    return log.m_Result = Base::Write( buffer, enable );
                }

                // Posh command streamer.
                ML_FUNCTION_CHECK( buffer.m_Type == GpuCommandBufferType::Posh );

                return log.m_Result = enable
                    ? Activate( buffer )
                    : Deactivate( buffer );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Activates null hardware override.
            /// @param  buffer  target command buffer.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode Activate( CommandBuffer& buffer )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const auto&  registers     = T::GpuRegisters::GetNullHardwareEnablingPoshRegisters();
                const size_t registerCount = registers.size();

                ML_FUNCTION_CHECK( registerCount % 2 == 0 );

                for( size_t i = 0; i < registerCount; i += 2 )
                {
                    ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                        buffer,
                        registers[i],
                        registers[i + 1] ) );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Deactivates null hardware override.
            /// @param  buffer  target command buffer.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode Deactivate( CommandBuffer& buffer )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const auto&  registers     = T::GpuRegisters::GetNullHardwareDisablingPoshRegisters();
                const size_t registerCount = registers.size();

                ML_FUNCTION_CHECK( registerCount % 2 == 0 );

                for( size_t i = 0; i < registerCount; i += 2 )
                {
                    ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                        buffer,
                        registers[i],
                        registers[i + 1] ) );
                }

                return log.m_Result;
            }
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct OverrideNullHardwareTrait : GEN11::OverrideNullHardwareTrait<T>
        {
            ML_DECLARE_TRAIT( OverrideNullHardwareTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
