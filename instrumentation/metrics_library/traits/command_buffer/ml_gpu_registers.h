/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2026 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_gpu_registers.h

@brief Gpu registers.
*/

// clang-format off

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for GpuRegistersTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct GpuRegistersTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( GpuRegistersTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( GpuRegistersTrait );

        // Render registers.
        static constexpr uint32_t m_RenderMmioRangeBegin = 0x2000;
        static constexpr uint32_t m_RenderMmioRangeEnd   = 0x27FF;

        // Common registers.
        static constexpr uint32_t m_CoreFrequency        = 0xA01C;
        static constexpr uint32_t m_TimestampLow         = 0x2358;
        static constexpr uint32_t m_TimestampHigh        = 0x235C;
        static constexpr uint32_t m_OaTail               = 0xDB04;
        static constexpr uint32_t m_OaBuffer             = 0xDB08;
        static constexpr uint32_t m_OaTrigger2           = 0xD924;
        static constexpr uint32_t m_OaTrigger6           = 0xD934;
        static constexpr uint32_t m_RpstatFrequencyMask  = 0xFF800000;
        static constexpr uint32_t m_RpstatFrequencyShift = 23;

        // Stream markers registers.
        static constexpr uint32_t m_StreamMarker        = 0xDA18;
        static constexpr uint32_t m_StreamMarkerRender  = 0xDA20;
        static constexpr uint32_t m_StreamMarkerPosh    = 0xDA18;
        static constexpr uint32_t m_StreamMarkerCompute = 0xDA10;

        // Overrides registers.
        static constexpr uint32_t m_NullHardwareRegister = 0x20D8;
        static constexpr uint32_t m_NullHardwareEnable   = 0x00030003;
        static constexpr uint32_t m_NullHardwareDisable  = 0x00030000;

        // Overrides array.
        using NullHardwareEnable  = std::array<uint32_t, 2>;
        using NullHardwareDisable = std::array<uint32_t, 2>;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns null hardware override enabling registers and values.
        /// @return null hardware override enabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const NullHardwareEnable& GetNullHardwareEnablingRegisters()
        {
            static constexpr NullHardwareEnable nullHardwareEnable =
            {
                m_NullHardwareRegister,
                m_NullHardwareEnable
            };

            return nullHardwareEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns null hardware override disabling registers and values.
        /// @return null hardware override disabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const NullHardwareDisable& GetNullHardwareDisablingRegisters()
        {
            static constexpr NullHardwareDisable nullHardwareDisable =
            {
                m_NullHardwareRegister,
                m_NullHardwareDisable
            };

            return nullHardwareDisable;
        }
    };
} // namespace ML::BASE

namespace ML::XE_LP
{
    template <typename T>
    struct GpuRegistersTrait : BASE::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, BASE );
    };
} // namespace ML::XE_LP

namespace ML::XE_HPG
{
    template <typename T>
    struct GpuRegistersTrait : XE_LP::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, XE_LP );

        // Oag mmio trigger.
        static constexpr uint32_t m_OagTrigger = 0xDB1C;

        // General purpose registers per command streamer (typically used for math).
        static constexpr uint32_t m_GeneralPurposeRegisterRender   = 0x2600;
        static constexpr uint32_t m_GeneralPurposeRegisterCompute0 = 0x1A600;
        static constexpr uint32_t m_GeneralPurposeRegisterCompute1 = 0x1C600;
        static constexpr uint32_t m_GeneralPurposeRegisterCompute2 = 0x1E600;
        static constexpr uint32_t m_GeneralPurposeRegisterCompute3 = 0x26600;

        // Registers used to identity command streamers.
        static constexpr uint32_t m_IdentityRCS  = m_GeneralPurposeRegisterRender;
        static constexpr uint32_t m_IdentityCCS0 = m_GeneralPurposeRegisterCompute0;
        static constexpr uint32_t m_IdentityCCS1 = m_GeneralPurposeRegisterCompute1;
        static constexpr uint32_t m_IdentityCCS2 = m_GeneralPurposeRegisterCompute2;
        static constexpr uint32_t m_IdentityCCS3 = m_GeneralPurposeRegisterCompute3;
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct GpuRegistersTrait : XE_HPG::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct GpuRegistersTrait : XE_HPG::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, XE_HPG );

        static constexpr uint32_t m_OarPec0 = 0x14000;
        static constexpr uint32_t m_OacPec0 = 0x15200;
    };
} // namespace ML::XE2_HPG

namespace ML::XE3
{
    template <typename T>
    struct GpuRegistersTrait : XE2_HPG::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, XE2_HPG );
    };
} // namespace ML::XE3

namespace ML::XE3P
{
    template <typename T>
    struct GpuRegistersTrait : XE3::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, XE3 );
    };
} // namespace ML::XE3P

// clang-format on
