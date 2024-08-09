/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_override_null_hardware.h

@brief Enables null hardware override.
*/

#pragma once

namespace ML::BASE
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
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "OverrideNullHardwareTrait<Traits>";
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
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
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
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

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
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

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
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct OverrideNullHardwareTrait : BASE::OverrideNullHardwareTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideNullHardwareTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
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
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

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
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

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
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

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
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct OverrideNullHardwareTrait : GEN11::OverrideNullHardwareTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideNullHardwareTrait, GEN11 );
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct OverrideNullHardwareTrait : XE_LP::OverrideNullHardwareTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideNullHardwareTrait, XE_LP );
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct OverrideNullHardwareTrait : XE_HP::OverrideNullHardwareTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideNullHardwareTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct OverrideNullHardwareTrait : XE_HPG::OverrideNullHardwareTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideNullHardwareTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct OverrideNullHardwareTrait : BASE::OverrideNullHardwareTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideNullHardwareTrait, BASE );
    };
} // namespace ML::XE2_HPG
