/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

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

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "GpuRegistersTrait";
        }

        // Common registers.
        static constexpr uint32_t m_ContextId           = 0x2364;
        static constexpr uint32_t m_NopId               = 0x2094;
        static constexpr uint32_t m_GpuTicks            = 0x2910;
        static constexpr uint32_t m_CoreFrequency       = 0xA01C;
        static constexpr uint32_t m_TimestampLow        = 0x2358;
        static constexpr uint32_t m_TimestampHigh       = 0x235C;
        static constexpr uint32_t m_OaContextControl    = 0x2360;
        static constexpr uint32_t m_OaStatus            = 0x2B08;
        static constexpr uint32_t m_OaHead              = 0x2B0C;
        static constexpr uint32_t m_OaTail              = 0x2B10;
        static constexpr uint32_t m_OaBuffer            = 0x2B14;
        static constexpr uint32_t m_GpCounter           = 0x91b8;
        static constexpr uint32_t m_OaTrigger2          = 0x2744;
        static constexpr uint32_t m_OaTrigger6          = 0x2754;
        static constexpr uint32_t m_RpstatFrequencyMask = 0xFF800000;

        // Oa counters low.
        static constexpr uint32_t m_OaPerfA0  = 0x2800;
        static constexpr uint32_t m_OaPerfA1  = 0x2808;
        static constexpr uint32_t m_OaPerfA2  = 0x2810;
        static constexpr uint32_t m_OaPerfA3  = 0x2818;
        static constexpr uint32_t m_OaPerfA4  = 0x2820;
        static constexpr uint32_t m_OaPerfA5  = 0x2828;
        static constexpr uint32_t m_OaPerfA6  = 0x2830;
        static constexpr uint32_t m_OaPerfA7  = 0x2838;
        static constexpr uint32_t m_OaPerfA8  = 0x2840;
        static constexpr uint32_t m_OaPerfA9  = 0x2848;
        static constexpr uint32_t m_OaPerfA10 = 0x2850;
        static constexpr uint32_t m_OaPerfA11 = 0x2858;
        static constexpr uint32_t m_OaPerfA12 = 0x2860;
        static constexpr uint32_t m_OaPerfA13 = 0x2868;
        static constexpr uint32_t m_OaPerfA14 = 0x2870;
        static constexpr uint32_t m_OaPerfA15 = 0x2878;
        static constexpr uint32_t m_OaPerfA16 = 0x2880;
        static constexpr uint32_t m_OaPerfA17 = 0x2888;
        static constexpr uint32_t m_OaPerfA18 = 0x2890;
        static constexpr uint32_t m_OaPerfA19 = 0x2898;
        static constexpr uint32_t m_OaPerfA20 = 0x28A0;
        static constexpr uint32_t m_OaPerfA21 = 0x28A8;
        static constexpr uint32_t m_OaPerfA22 = 0x28B0;
        static constexpr uint32_t m_OaPerfA23 = 0x28B8;
        static constexpr uint32_t m_OaPerfA24 = 0x28C0;
        static constexpr uint32_t m_OaPerfA25 = 0x28C8;
        static constexpr uint32_t m_OaPerfA26 = 0x28D0;
        static constexpr uint32_t m_OaPerfA27 = 0x28D8;
        static constexpr uint32_t m_OaPerfA28 = 0x28E0;
        static constexpr uint32_t m_OaPerfA29 = 0x28E8;
        static constexpr uint32_t m_OaPerfA30 = 0x28F0;
        static constexpr uint32_t m_OaPerfA31 = 0x28F8;
        static constexpr uint32_t m_OaPerfA32 = 0x2900;
        static constexpr uint32_t m_OaPerfA33 = 0x2904;
        static constexpr uint32_t m_OaPerfA34 = 0x2908;
        static constexpr uint32_t m_OaPerfA35 = 0x290C;

        // Oa counters high.
        static constexpr uint32_t m_OaPerfA0Upper  = 0x2804;
        static constexpr uint32_t m_OaPerfA1Upper  = 0x280C;
        static constexpr uint32_t m_OaPerfA2Upper  = 0x2814;
        static constexpr uint32_t m_OaPerfA3Upper  = 0x281C;
        static constexpr uint32_t m_OaPerfA4Upper  = 0x2824;
        static constexpr uint32_t m_OaPerfA5Upper  = 0x282C;
        static constexpr uint32_t m_OaPerfA6Upper  = 0x2834;
        static constexpr uint32_t m_OaPerfA7Upper  = 0x283C;
        static constexpr uint32_t m_OaPerfA8Upper  = 0x2844;
        static constexpr uint32_t m_OaPerfA9Upper  = 0x284C;
        static constexpr uint32_t m_OaPerfA10Upper = 0x2854;
        static constexpr uint32_t m_OaPerfA11Upper = 0x285C;
        static constexpr uint32_t m_OaPerfA12Upper = 0x2864;
        static constexpr uint32_t m_OaPerfA13Upper = 0x286C;
        static constexpr uint32_t m_OaPerfA14Upper = 0x2874;
        static constexpr uint32_t m_OaPerfA15Upper = 0x287C;
        static constexpr uint32_t m_OaPerfA16Upper = 0x2884;
        static constexpr uint32_t m_OaPerfA17Upper = 0x288C;
        static constexpr uint32_t m_OaPerfA18Upper = 0x2894;
        static constexpr uint32_t m_OaPerfA19Upper = 0x289C;
        static constexpr uint32_t m_OaPerfA20Upper = 0x28A4;
        static constexpr uint32_t m_OaPerfA21Upper = 0x28AC;
        static constexpr uint32_t m_OaPerfA22Upper = 0x28B4;
        static constexpr uint32_t m_OaPerfA23Upper = 0x28BC;
        static constexpr uint32_t m_OaPerfA24Upper = 0x28C4;
        static constexpr uint32_t m_OaPerfA25Upper = 0x28CC;
        static constexpr uint32_t m_OaPerfA26Upper = 0x28D4;
        static constexpr uint32_t m_OaPerfA27Upper = 0x28DC;
        static constexpr uint32_t m_OaPerfA28Upper = 0x28E4;
        static constexpr uint32_t m_OaPerfA29Upper = 0x28EC;
        static constexpr uint32_t m_OaPerfA30Upper = 0x28F4;
        static constexpr uint32_t m_OaPerfA31Upper = 0x28FC;

        // Noa counters.
        static constexpr uint32_t m_OaPerfB0 = 0x2920;
        static constexpr uint32_t m_OaPerfB1 = 0x2924;
        static constexpr uint32_t m_OaPerfB2 = 0x2928;
        static constexpr uint32_t m_OaPerfB3 = 0x292C;
        static constexpr uint32_t m_OaPerfB4 = 0x2930;
        static constexpr uint32_t m_OaPerfB5 = 0x2934;
        static constexpr uint32_t m_OaPerfB6 = 0x2938;
        static constexpr uint32_t m_OaPerfB7 = 0x293C;
        static constexpr uint32_t m_OaPerfC0 = 0x2940;
        static constexpr uint32_t m_OaPerfC1 = 0x2944;
        static constexpr uint32_t m_OaPerfC2 = 0x2948;
        static constexpr uint32_t m_OaPerfC3 = 0x294C;
        static constexpr uint32_t m_OaPerfC4 = 0x2950;
        static constexpr uint32_t m_OaPerfC5 = 0x2954;
        static constexpr uint32_t m_OaPerfC6 = 0x2958;
        static constexpr uint32_t m_OaPerfC7 = 0x295C;

        // Stream markers registers.
        static constexpr uint32_t m_StreamMarker        = m_OaPerfA20;
        static constexpr uint32_t m_StreamMarkerRender  = m_OaPerfA20;
        static constexpr uint32_t m_StreamMarkerPosh    = m_OaPerfA19;
        static constexpr uint32_t m_StreamMarkerCompute = m_OaPerfA18;

        // Oa / Noa counters array.
        using OaCountersLow  = std::array<uint32_t, 36>;
        using OaCountersHigh = std::array<uint32_t, 32>;
        using NoaCounters    = std::array<uint32_t, 16>;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa counters registers (lower bits).
        /// @return oa counters registers (lower bits).
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const OaCountersLow& GetOaCountersLow()
        {
            static constexpr OaCountersLow oaCountersLow =
            {
                m_OaPerfA0,
                m_OaPerfA1,
                m_OaPerfA2,
                m_OaPerfA3,
                m_OaPerfA4,
                m_OaPerfA5,
                m_OaPerfA6,
                m_OaPerfA7,
                m_OaPerfA8,
                m_OaPerfA9,
                m_OaPerfA10,
                m_OaPerfA11,
                m_OaPerfA12,
                m_OaPerfA13,
                m_OaPerfA14,
                m_OaPerfA15,
                m_OaPerfA16,
                m_OaPerfA17,
                m_OaPerfA18,
                m_OaPerfA19,
                m_OaPerfA20,
                m_OaPerfA21,
                m_OaPerfA22,
                m_OaPerfA23,
                m_OaPerfA24,
                m_OaPerfA25,
                m_OaPerfA26,
                m_OaPerfA27,
                m_OaPerfA28,
                m_OaPerfA29,
                m_OaPerfA30,
                m_OaPerfA31,
                m_OaPerfA32,
                m_OaPerfA33,
                m_OaPerfA34,
                m_OaPerfA35
            };

            return oaCountersLow;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa counters registers (upper bits).
        /// @return oa counters registers (upper bits).
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const OaCountersHigh& GetOaCountersHigh()
        {
            static constexpr OaCountersHigh oaCountersHigh =
            {
                m_OaPerfA0Upper,
                m_OaPerfA1Upper,
                m_OaPerfA2Upper,
                m_OaPerfA3Upper,
                m_OaPerfA4Upper,
                m_OaPerfA5Upper,
                m_OaPerfA6Upper,
                m_OaPerfA7Upper,
                m_OaPerfA8Upper,
                m_OaPerfA9Upper,
                m_OaPerfA10Upper,
                m_OaPerfA11Upper,
                m_OaPerfA12Upper,
                m_OaPerfA13Upper,
                m_OaPerfA14Upper,
                m_OaPerfA15Upper,
                m_OaPerfA16Upper,
                m_OaPerfA17Upper,
                m_OaPerfA18Upper,
                m_OaPerfA19Upper,
                m_OaPerfA20Upper,
                m_OaPerfA21Upper,
                m_OaPerfA22Upper,
                m_OaPerfA23Upper,
                m_OaPerfA24Upper,
                m_OaPerfA25Upper,
                m_OaPerfA26Upper,
                m_OaPerfA27Upper,
                m_OaPerfA28Upper,
                m_OaPerfA29Upper,
                m_OaPerfA30Upper,
                m_OaPerfA31Upper
            };

            return oaCountersHigh;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns noa counters registers.
        /// @return noa counters registers .
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const NoaCounters& GetNoaCounters()
        {
            static constexpr NoaCounters noaCounters =
            {
                m_OaPerfB0,
                m_OaPerfB1,
                m_OaPerfB2,
                m_OaPerfB3,
                m_OaPerfB4,
                m_OaPerfB5,
                m_OaPerfB6,
                m_OaPerfB7,
                m_OaPerfC0,
                m_OaPerfC1,
                m_OaPerfC2,
                m_OaPerfC3,
                m_OaPerfC4,
                m_OaPerfC5,
                m_OaPerfC6,
                m_OaPerfC7
            };

            return noaCounters;
        }

        // Overrides registers.
        static constexpr uint32_t m_NullHardwareRegister = 0x20D8;
        static constexpr uint32_t m_NullHardwareEnable   = 0x00030003;
        static constexpr uint32_t m_NullHardwareDisable  = 0x00030000;

        static constexpr uint32_t m_CullAtCsRegister = 0x27F4;
        static constexpr uint32_t m_CullAtCsEnable   = 0x00010001;
        static constexpr uint32_t m_CullAtCsDisable  = 0x00010000;

        static constexpr uint32_t m_CullAtVfRegister = m_CullAtCsRegister;
        static constexpr uint32_t m_CullAtVfEnable   = 0x00020002;
        static constexpr uint32_t m_CullAtVfDisable  = 0x00020000;

        static constexpr uint32_t m_CullAtVsRegister = m_CullAtCsRegister;
        static constexpr uint32_t m_CullAtVsEnable   = 0x00040004;
        static constexpr uint32_t m_CullAtVsDisable  = 0x00040000;

        static constexpr uint32_t m_CullAtHsRegister = m_CullAtCsRegister;
        static constexpr uint32_t m_CullAtHsEnable   = 0x00080008;
        static constexpr uint32_t m_CullAtHsDisable  = 0x00080000;

        static constexpr uint32_t m_CullAtTeRegister = m_CullAtCsRegister;
        static constexpr uint32_t m_CullAtTeEnable   = 0x00100010;
        static constexpr uint32_t m_CullAtTeDisable  = 0x00100000;

        static constexpr uint32_t m_CullAtDsRegister = m_CullAtCsRegister;
        static constexpr uint32_t m_CullAtDsEnable   = 0x00200020;
        static constexpr uint32_t m_CullAtDsDisable  = 0x00200000;

        static constexpr uint32_t m_CullAtGsRegister = m_CullAtCsRegister;
        static constexpr uint32_t m_CullAtGsEnable   = 0x00400040;
        static constexpr uint32_t m_CullAtGsDisable  = 0x00400000;

        static constexpr uint32_t m_CullAtSolRegister = m_CullAtCsRegister;
        static constexpr uint32_t m_CullAtSolEnable   = 0x00800080;
        static constexpr uint32_t m_CullAtSolDisable  = 0x00800000;

        static constexpr uint32_t m_CullAtClRegister = m_CullAtCsRegister;
        static constexpr uint32_t m_CullAtClEnable   = 0x01000100;
        static constexpr uint32_t m_CullAtClDisable  = 0x01000000;

        static constexpr uint32_t m_CullAtSfRegister = m_CullAtCsRegister;
        static constexpr uint32_t m_CullAtSfEnable   = 0x02000200;
        static constexpr uint32_t m_CullAtSfDisable  = 0x02000000;

        static constexpr uint32_t m_CullAtWmfeRegister = 0x7030;
        static constexpr uint32_t m_CullAtWmfeEnable   = 0x04000400;
        static constexpr uint32_t m_CullAtWmfeDisable  = 0x04000000;

        static constexpr uint32_t m_CullAtWmbeRegister = m_CullAtWmfeRegister;
        static constexpr uint32_t m_CullAtWmbeEnable   = 0x08000800;
        static constexpr uint32_t m_CullAtWmbeDisable  = 0x08000000;

        static constexpr uint32_t m_CullAtIzRegister = m_CullAtWmfeRegister;
        static constexpr uint32_t m_CullAtIzEnable   = 0x10001000;
        static constexpr uint32_t m_CullAtIzDisable  = 0x10000000;

        static constexpr uint32_t m_CullAtBcRegister = m_CullAtWmfeRegister;
        static constexpr uint32_t m_CullAtBcEnable   = 0x20002000;
        static constexpr uint32_t m_CullAtBcDisable  = 0x20000000;

        static constexpr uint32_t m_CullAtPsdRegister = m_CullAtWmfeRegister;
        static constexpr uint32_t m_CullAtPsdEnable   = 0x40004000;
        static constexpr uint32_t m_CullAtPsdDisable  = 0x40000000;

        static constexpr uint32_t m_SamplerDataFetchThrottlingRegister = 0xE184;
        static constexpr uint32_t m_SamplerDataFetchThrottlingEnable   = 0xE0000000;
        static constexpr uint32_t m_SamplerDataFetchThrottlingDisable  = 0xE0000000;

        static constexpr uint32_t m_SamplerPipeThrottlingRegister = m_SamplerDataFetchThrottlingRegister;
        static constexpr uint32_t m_SamplerPipeThrottlingEnable   = 0x1C000000;
        static constexpr uint32_t m_SamplerPipeThrottlingDisable  = 0x1C000000;

        static constexpr uint32_t m_EuThrottlingRegister = 0xE4F0;
        static constexpr uint32_t m_EuThrottlingEnable   = 0x001C0000;
        static constexpr uint32_t m_EuThrottlingDisable  = 0x001C0000;

        static constexpr uint32_t m_RcpbeThrottlingRegister = 0x7014;
        static constexpr uint32_t m_RcpbeThrottlingEnable   = 0xC0000000;
        static constexpr uint32_t m_RcpbeThrottlingDisable  = 0xC0000000;

        static constexpr uint32_t m_SfWmThrottlingRegister = 0x6B1C;
        static constexpr uint32_t m_SfWmThrottlingEnable   = 0x07000004;
        static constexpr uint32_t m_SfWmThrottlingDisable  = 0x07000004;

        static constexpr uint32_t m_SfSbeThrottlingRegister = m_SfWmThrottlingRegister;
        static constexpr uint32_t m_SfSbeThrottlingEnable   = 0x07000002;
        static constexpr uint32_t m_SfSbeThrottlingDisable  = 0x07000002;

        static constexpr uint32_t m_ClSfThrottlingRegister = m_SfWmThrottlingRegister;
        static constexpr uint32_t m_ClSfThrottlingEnable   = 0x07000001;
        static constexpr uint32_t m_ClSfThrottlingDisable  = 0x07000001;

        static constexpr uint32_t m_WmfeWmbeThrottlingRegister = 0x5584;
        static constexpr uint32_t m_WmfeWmbeThrottlingEnable   = 0x00070000;
        static constexpr uint32_t m_WmfeWmbeThrottlingDisable  = 0x00070000;

        static constexpr uint32_t m_HizThrottlingRegister = 0x7018;
        static constexpr uint32_t m_HizThrottlingEnable   = 0x1C000000;
        static constexpr uint32_t m_HizThrottlingDisable  = 0x1C000000;

        // Overrides array.
        using NullHardwareEnable                = std::array<uint32_t, 2>;
        using NullHardwareDisable               = std::array<uint32_t, 2>;
        using CullAtCsEnable                    = std::array<uint32_t, 2>;
        using CullAtCsDisable                   = std::array<uint32_t, 2>;
        using CullAtVfEnable                    = std::array<uint32_t, 2>;
        using CullAtVfDisable                   = std::array<uint32_t, 2>;
        using CullAtVsEnable                    = std::array<uint32_t, 2>;
        using CullAtVsDisable                   = std::array<uint32_t, 2>;
        using CullAtHsEnable                    = std::array<uint32_t, 2>;
        using CullAtHsDisable                   = std::array<uint32_t, 2>;
        using CullAtTeEnable                    = std::array<uint32_t, 2>;
        using CullAtTeDisable                   = std::array<uint32_t, 2>;
        using CullAtDsEnable                    = std::array<uint32_t, 2>;
        using CullAtDsDisable                   = std::array<uint32_t, 2>;
        using CullAtGsEnable                    = std::array<uint32_t, 2>;
        using CullAtGsDisable                   = std::array<uint32_t, 2>;
        using CullAtSolEnable                   = std::array<uint32_t, 2>;
        using CullAtSolDisable                  = std::array<uint32_t, 2>;
        using CullAtClEnable                    = std::array<uint32_t, 2>;
        using CullAtClDisable                   = std::array<uint32_t, 2>;
        using CullAtSfEnable                    = std::array<uint32_t, 2>;
        using CullAtSfDisable                   = std::array<uint32_t, 2>;
        using CullAtWmfeEnable                  = std::array<uint32_t, 2>;
        using CullAtWmfeDisable                 = std::array<uint32_t, 2>;
        using CullAtWmbeEnable                  = std::array<uint32_t, 2>;
        using CullAtWmbeDisable                 = std::array<uint32_t, 2>;
        using CullAtIzEnable                    = std::array<uint32_t, 2>;
        using CullAtIzDisable                   = std::array<uint32_t, 2>;
        using CullAtBcEnable                    = std::array<uint32_t, 2>;
        using CullAtBcDisable                   = std::array<uint32_t, 2>;
        using CullAtPsdEnable                   = std::array<uint32_t, 2>;
        using CullAtPsdDisable                  = std::array<uint32_t, 2>;
        using SamplerDataFetchThrottlingEnable  = std::array<uint32_t, 2>;
        using SamplerDataFetchThrottlingDisable = std::array<uint32_t, 2>;
        using SamplerPipeThrottlingEnable       = std::array<uint32_t, 2>;
        using SamplerPipeThrottlingDisable      = std::array<uint32_t, 2>;
        using EuThrottlingEnable                = std::array<uint32_t, 2>;
        using EuThrottlingDisable               = std::array<uint32_t, 2>;
        using RcpbeThrottlingEnable             = std::array<uint32_t, 2>;
        using RcpbeThrottlingDisable            = std::array<uint32_t, 2>;
        using SfWmThrottlingEnable              = std::array<uint32_t, 2>;
        using SfWmThrottlingDisable             = std::array<uint32_t, 2>;
        using SfSbeThrottlingEnable             = std::array<uint32_t, 2>;
        using SfSbeThrottlingDisable            = std::array<uint32_t, 2>;
        using ClSfThrottlingEnable              = std::array<uint32_t, 2>;
        using ClSfThrottlingDisable             = std::array<uint32_t, 2>;
        using WmfeWmbeThrottlingEnable          = std::array<uint32_t, 2>;
        using WmfeWmbeThrottlingDisable         = std::array<uint32_t, 2>;
        using HizThrottlingEnable               = std::array<uint32_t, 2>;
        using HizThrottlingDisable              = std::array<uint32_t, 2>;

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

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at compute shader stage override enabling
        ///         registers and values.
        /// @return cull at compute shader stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtCsEnable& GetCullAtCsEnablingRegisters()
        {
            static constexpr CullAtCsEnable cullAtCsEnable =
            {
                m_CullAtCsRegister,
                m_CullAtCsEnable
            };

            return cullAtCsEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at compute shader stage override disabling
        ///         registers and values.
        /// @return cull at compute shader stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtCsDisable& GetCullAtCsDisablingRegisters()
        {
            static constexpr CullAtCsDisable cullAtCsDisable =
            {
                m_CullAtCsRegister,
                m_CullAtCsDisable
            };

            return cullAtCsDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at vertex fetch stage override enabling registers
        ///         and values.
        /// @return cull at vertex fetch stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtVfEnable& GetCullAtVfEnablingRegisters()
        {
            static constexpr CullAtVfEnable cullAtVfEnable =
            {
                m_CullAtVfRegister,
                m_CullAtVfEnable
            };

            return cullAtVfEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at vertex fetch stage override disabling registers
        ///         and values.
        /// @return cull at vertex fetch stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtVfDisable& GetCullAtVfDisablingRegisters()
        {
            static constexpr CullAtVfDisable cullAtVfDisable =
            {
                m_CullAtVfRegister,
                m_CullAtVfDisable
            };

            return cullAtVfDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at vertex shader stage override enabling registers
        ///         and values.
        /// @return cull at vertex shader stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtVsEnable& GetCullAtVsEnablingRegisters()
        {
            static constexpr CullAtVsEnable cullAtVsEnable =
            {
                m_CullAtVsRegister,
                m_CullAtVsEnable
            };

            return cullAtVsEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at vertex shader stage override disabling registers
        ///         and values.
        /// @return cull at vertex shader stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtVsDisable& GetCullAtVsDisablingRegisters()
        {
            static constexpr CullAtVsDisable cullAtVsDisable =
            {
                m_CullAtVsRegister,
                m_CullAtVsDisable
            };

            return cullAtVsDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at hull shader stage override enabling registers
        ///         and values.
        /// @return cull at hull shader stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtHsEnable& GetCullAtHsEnablingRegisters()
        {
            static constexpr CullAtHsEnable cullAtHsEnable =
            {
                m_CullAtHsRegister,
                m_CullAtHsEnable
            };

            return cullAtHsEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at hull shader stage override disabling registers
        ///         and values.
        /// @return cull at hull shader stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtHsDisable& GetCullAtHsDisablingRegisters()
        {
            static constexpr CullAtHsDisable cullAtHsDisable =
            {
                m_CullAtHsRegister,
                m_CullAtHsDisable
            };

            return cullAtHsDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at tessellator stage override enabling registers
        ///         and values.
        /// @return cull at tessellator stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtTeEnable& GetCullAtTeEnablingRegisters()
        {
            static constexpr CullAtTeEnable cullAtTeEnable =
            {
                m_CullAtTeRegister,
                m_CullAtTeEnable
            };

            return cullAtTeEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at tessellator stage override disabling registers
        ///         and values.
        /// @return cull at tessellator stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtTeDisable& GetCullAtTeDisablingRegisters()
        {
            static constexpr CullAtTeDisable cullAtTeDisable =
            {
                m_CullAtTeRegister,
                m_CullAtTeDisable
            };

            return cullAtTeDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at domain shader stage override enabling registers
        ///         and values.
        /// @return cull at domain shader stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtDsEnable& GetCullAtDsEnablingRegisters()
        {
            static constexpr CullAtDsEnable cullAtDsEnable =
            {
                m_CullAtDsRegister,
                m_CullAtDsEnable
            };

            return cullAtDsEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at domain shader stage override disabling registers
        ///         and values.
        /// @return cull at domain shader stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtDsDisable& GetCullAtDsDisablingRegisters()
        {
            static constexpr CullAtDsDisable cullAtDsDisable =
            {
                m_CullAtDsRegister,
                m_CullAtDsDisable
            };

            return cullAtDsDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at geometry shader stage override enabling
        ///         registers and values.
        /// @return cull at geometry shader stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtGsEnable& GetCullAtGsEnablingRegisters()
        {
            static constexpr CullAtGsEnable cullAtGsEnable =
            {
                m_CullAtGsRegister,
                m_CullAtGsEnable
            };

            return cullAtGsEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at geometry shader stage override disabling
        ///         registers and values.
        /// @return cull at geometry shader stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtGsDisable& GetCullAtGsDisablingRegisters()
        {
            static constexpr CullAtGsDisable cullAtGsDisable =
            {
                m_CullAtGsRegister,
                m_CullAtGsDisable
            };

            return cullAtGsDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at stream output logic stage override enabling
        ///         registers and values.
        /// @return cull at stream output logic stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtSolEnable& GetCullAtSolEnablingRegisters()
        {
            static constexpr CullAtSolEnable cullAtSolEnable =
            {
                m_CullAtSolRegister,
                m_CullAtSolEnable
            };

            return cullAtSolEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at stream output logic stage override disabling
        ///         registers and values.
        /// @return cull at stream output logic stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtSolDisable& GetCullAtSolDisablingRegisters()
        {
            static constexpr CullAtSolDisable cullAtSolDisable =
            {
                m_CullAtSolRegister,
                m_CullAtSolDisable
            };

            return cullAtSolDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at clip stage override enabling registers
        ///         and values.
        /// @return cull at clip stage override enabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtClEnable& GetCullAtClEnablingRegisters()
        {
            static constexpr CullAtClEnable cullAtClEnable =
            {
                m_CullAtClRegister,
                m_CullAtClEnable
            };

            return cullAtClEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at clip stage override disabling registers
        ///         and values.
        /// @return cull at clip stage override disabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtClDisable& GetCullAtClDisablingRegisters()
        {
            static constexpr CullAtClDisable cullAtClDisable =
            {
                m_CullAtClRegister,
                m_CullAtClDisable
            };

            return cullAtClDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at strips and fans stage override enabling
        ///         registers and values.
        /// @return cull at strips and fans stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtSfEnable& GetCullAtSfEnablingRegisters()
        {
            static constexpr CullAtSfEnable cullAtSfEnable =
            {
                m_CullAtSfRegister,
                m_CullAtSfEnable
            };

            return cullAtSfEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at strips and fans stage override disabling
        ///         registers and values.
        /// @return cull at strips and fans stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtSfDisable& GetCullAtSfDisablingRegisters()
        {
            static constexpr CullAtSfDisable cullAtSfDisable =
            {
                m_CullAtSfRegister,
                m_CullAtSfDisable
            };

            return cullAtSfDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at windower front-end stage override enabling
        ///         registers and values.
        /// @return cull at windower front-end stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtWmfeEnable& GetCullAtWmfeEnablingRegisters()
        {
            static constexpr CullAtWmfeEnable cullAtWmfeEnable =
            {
                m_CullAtWmfeRegister,
                m_CullAtWmfeEnable
            };

            return cullAtWmfeEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at windower front-end stage override disabling
        ///         registers and values.
        /// @return cull at windower front-end stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtWmfeDisable& GetCullAtWmfeDisablingRegisters()
        {
            static constexpr CullAtWmfeDisable cullAtWmfeDisable =
            {
                m_CullAtWmfeRegister,
                m_CullAtWmfeDisable
            };

            return cullAtWmfeDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at windower back-end stage override enabling
        ///         registers and values.
        /// @return cull at windower back-end stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtWmbeEnable& GetCullAtWmbeEnablingRegisters()
        {
            static constexpr CullAtWmbeEnable cullAtWmbeEnable =
            {
                m_CullAtWmbeRegister,
                m_CullAtWmbeEnable
            };

            return cullAtWmbeEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at windower back-end stage override disabling
        ///         registers and values.
        /// @return cull at windower back-end stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtWmbeDisable& GetCullAtWmbeDisablingRegisters()
        {
            static constexpr CullAtWmbeDisable cullAtWmbeDisable =
            {
                m_CullAtWmbeRegister,
                m_CullAtWmbeDisable
            };

            return cullAtWmbeDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at intermediate z stage override enabling
        ///         registers and values.
        /// @return cull at intermediate z stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtIzEnable& GetCullAtIzEnablingRegisters()
        {
            static constexpr CullAtIzEnable cullAtIzEnable =
            {
                m_CullAtIzRegister,
                m_CullAtIzEnable
            };

            return cullAtIzEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at intermediate z stage override disabling
        ///         registers and values.
        /// @return cull at intermediate z stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtIzDisable& GetCullAtIzDisablingRegisters()
        {
            static constexpr CullAtIzDisable cullAtIzDisable =
            {
                m_CullAtIzRegister,
                m_CullAtIzDisable
            };

            return cullAtIzDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at bc stage override enabling registers and values.
        /// @return cull at bc stage override enabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtBcEnable& GetCullAtBcEnablingRegisters()
        {
            static constexpr CullAtBcEnable cullAtBcEnable =
            {
                m_CullAtBcRegister,
                m_CullAtBcEnable
            };

            return cullAtBcEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at bc stage override disabling registers and values.
        /// @return cull at bc stage override disabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtBcDisable& GetCullAtBcDisablingRegisters()
        {
            static constexpr CullAtBcDisable cullAtBcDisable =
            {
                m_CullAtBcRegister,
                m_CullAtBcDisable
            };

            return cullAtBcDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at psd stage override enabling registers and values.
        /// @return cull at psd stage override enabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtPsdEnable& GetCullAtPsdEnablingRegisters()
        {
            static constexpr CullAtPsdEnable cullAtPsdEnable =
            {
                m_CullAtPsdRegister,
                m_CullAtPsdEnable
            };

            return cullAtPsdEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at psd stage override disabling registers and values.
        /// @return cull at psd stage override disabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtPsdDisable& GetCullAtPsdDisablingRegisters()
        {
            static constexpr CullAtPsdDisable cullAtPsdDisable =
            {
                m_CullAtPsdRegister,
                m_CullAtPsdDisable
            };

            return cullAtPsdDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns sampler data fetch throttling override enabling
        ///         registers and values.
        /// @return sampler data fetch throttling override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const SamplerDataFetchThrottlingEnable& GetSamplerDataFetchThrottlingEnablingRegisters()
        {
            static constexpr SamplerDataFetchThrottlingEnable samplerDataFetchThrottlingEnable =
            {
                m_SamplerDataFetchThrottlingRegister,
                m_SamplerDataFetchThrottlingEnable
            };

            return samplerDataFetchThrottlingEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns sampler data fetch throttling override disabling
        ///         registers and values.
        /// @return sampler data fetch throttling override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const SamplerDataFetchThrottlingDisable& GetSamplerDataFetchThrottlingDisablingRegisters()
        {
            static constexpr SamplerDataFetchThrottlingDisable samplerDataFetchThrottlingDisable =
            {
                m_SamplerDataFetchThrottlingRegister,
                m_SamplerDataFetchThrottlingDisable
            };

            return samplerDataFetchThrottlingDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns sampler pipe throttling override enabling registers
        ///         and values.
        /// @return sampler pipe throttling override enabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const SamplerPipeThrottlingEnable& GetSamplerPipeThrottlingEnablingRegisters()
        {
            static constexpr SamplerPipeThrottlingEnable samplerPipeThrottlingEnable =
            {
                m_SamplerPipeThrottlingRegister,
                m_SamplerPipeThrottlingEnable
            };

            return samplerPipeThrottlingEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns sampler pipe throttling override disabling registers
        ///         and values.
        /// @return sampler pipe throttling override disabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const SamplerPipeThrottlingDisable& GetSamplerPipeThrottlingDisablingRegisters()
        {
            static constexpr SamplerPipeThrottlingDisable samplerPipeThrottlingDisable =
            {
                m_SamplerPipeThrottlingRegister,
                m_SamplerPipeThrottlingDisable
            };

            return samplerPipeThrottlingDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns execution unit throttling override enabling registers
        ///         and values.
        /// @return execution unit throttling override enabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const EuThrottlingEnable& GetEuThrottlingEnablingRegisters()
        {
            static constexpr EuThrottlingEnable EuThrottlingEnable =
            {
                m_EuThrottlingRegister,
                m_EuThrottlingEnable
            };

            return EuThrottlingEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns execution unit throttling override disabling registers
        ///         and values.
        /// @return execution unit throttling override disabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const EuThrottlingDisable& GetEuThrottlingDisablingRegisters()
        {
            static constexpr EuThrottlingDisable EuThrottlingDisable =
            {
                m_EuThrottlingRegister,
                m_EuThrottlingDisable
            };

            return EuThrottlingDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns stall a given number of upstream units override
        ///         enabling registers and values.
        /// @return stall a given number of upstream units override enabling
        ///         registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const RcpbeThrottlingEnable& GetRcpbeThrottlingEnablingRegisters()
        {
            static constexpr RcpbeThrottlingEnable RcpbeThrottlingEnable =
            {
                m_RcpbeThrottlingRegister,
                m_RcpbeThrottlingEnable
            };

            return RcpbeThrottlingEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns stall a given number of upstream units override
        ///         disabling registers and values.
        /// @return stall a given number of upstream units override disabling
        ///         registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const RcpbeThrottlingDisable& GetRcpbeThrottlingDisablingRegisters()
        {
            static constexpr RcpbeThrottlingDisable RcpbeThrottlingDisable =
            {
                m_RcpbeThrottlingRegister,
                m_RcpbeThrottlingDisable
            };

            return RcpbeThrottlingDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns strips and fans - windower interface throttling
        ///         override enabling registers and values.
        /// @return strips and fans - windower interface throttling override
        ///         enabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const SfWmThrottlingEnable& GetSfWmThrottlingEnablingRegisters()
        {
            static constexpr SfWmThrottlingEnable SfWmThrottlingEnable =
            {
                m_SfWmThrottlingRegister,
                m_SfWmThrottlingEnable
            };

            return SfWmThrottlingEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns strips and fans - windower interface throttling
        ///         override disabling registers and values.
        /// @return strips and fans - windower interface throttling override
        ///         disabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const SfWmThrottlingDisable& GetSfWmThrottlingDisablingRegisters()
        {
            static constexpr SfWmThrottlingDisable SfWmThrottlingDisable =
            {
                m_SfWmThrottlingRegister,
                m_SfWmThrottlingDisable
            };

            return SfWmThrottlingDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns strips and fans - sbe interface throttling override
        ///         enabling registers and values.
        /// @return strips and fans - sbe interface throttling override enabling
        ///         registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const SfSbeThrottlingEnable& GetSfSbeThrottlingEnablingRegisters()
        {
            static constexpr SfSbeThrottlingEnable SfSbeThrottlingEnable =
            {
                m_SfSbeThrottlingRegister,
                m_SfSbeThrottlingEnable
            };

            return SfSbeThrottlingEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns strips and fans - sbe interface throttling override
        ///         disabling registers and values.
        /// @return strips and fans - sbe interface throttling override disabling
        ///         registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const SfSbeThrottlingDisable& GetSfSbeThrottlingDisablingRegisters()
        {
            static constexpr SfSbeThrottlingDisable SfSbeThrottlingDisable =
            {
                m_SfSbeThrottlingRegister,
                m_SfSbeThrottlingDisable
            };

            return SfSbeThrottlingDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns clip - strips and fans interface throttling override
        ///         enabling registers and values.
        /// @return clip - strips and fans interface throttling override enabling
        ///         registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const ClSfThrottlingEnable& GetClSfThrottlingEnablingRegisters()
        {
            static constexpr ClSfThrottlingEnable ClSfThrottlingEnable =
            {
                m_ClSfThrottlingRegister,
                m_ClSfThrottlingEnable
            };

            return ClSfThrottlingEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns clip - strips and fans interface throttling override
        ///         disabling registers and values.
        /// @return clip - strips and fans interface throttling override disabling
        ///         registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const ClSfThrottlingDisable& GetClSfThrottlingDisablingRegisters()
        {
            static constexpr ClSfThrottlingDisable ClSfThrottlingDisable =
            {
                m_ClSfThrottlingRegister,
                m_ClSfThrottlingDisable
            };

            return ClSfThrottlingDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns windower front-end - back-end performance throttling
        ///         override enabling registers and values.
        /// @return windower front-end - back-end performance throttling override
        ///         enabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const WmfeWmbeThrottlingEnable& GetWmfeWmbeThrottlingEnablingRegisters()
        {
            static constexpr WmfeWmbeThrottlingEnable WmfeWmbeThrottlingEnable =
            {
                m_WmfeWmbeThrottlingRegister,
                m_WmfeWmbeThrottlingEnable
            };

            return WmfeWmbeThrottlingEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns windower front-end - back-end performance throttling
        ///         override disabling registers and values.
        /// @return windower front-end - back-end performance throttling override
        ///         disabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const WmfeWmbeThrottlingDisable& GetWmfeWmbeThrottlingDisablingRegisters()
        {
            static constexpr WmfeWmbeThrottlingDisable WmfeWmbeThrottlingDisable =
            {
                m_WmfeWmbeThrottlingRegister,
                m_WmfeWmbeThrottlingDisable
            };

            return WmfeWmbeThrottlingDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns hiz performance throttling override enabling registers
        ///         and values.
        /// @return hiz performance throttling override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const HizThrottlingEnable& GetHizThrottlingEnablingRegisters()
        {
            static constexpr HizThrottlingEnable HizThrottlingEnable =
            {
                m_HizThrottlingRegister,
                m_HizThrottlingEnable
            };

            return HizThrottlingEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns hiz performance throttling override disabling registers
        ///         and values.
        /// @return hiz performance throttling override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const HizThrottlingDisable& GetHizThrottlingDisablingRegisters()
        {
            static constexpr HizThrottlingDisable HizThrottlingDisable =
            {
                m_HizThrottlingRegister,
                m_HizThrottlingDisable
            };

            return HizThrottlingDisable;
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct GpuRegistersTrait : BASE::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct GpuRegistersTrait : GEN9::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, GEN9 );

        // Overrides registers.
        static constexpr uint32_t m_NullHardwarePoshRegister = 0x180D8;
        static constexpr uint32_t m_StreamMarker             = Base::m_OaPerfA19;

        // Overrides array.
        using NullHardwarePoshEnable  = std::array<uint32_t, 2>;
        using NullHardwarePoshDisable = std::array<uint32_t, 2>;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns null hardware override for posh enabling registers
        ///         and values.
        /// @return null hardware override for posh enabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const NullHardwarePoshEnable& GetNullHardwareEnablingPoshRegisters()
        {
            static constexpr NullHardwarePoshEnable nullHardwarePoshEnable =
            {
                m_NullHardwarePoshRegister,
                Base::m_NullHardwareEnable
            };

            return nullHardwarePoshEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns null hardware override for posh disabling registers
        ///         and values.
        /// @return null hardware override for posh disabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const NullHardwarePoshDisable& GetNullHardwareDisablingPoshRegisters()
        {
            static constexpr NullHardwarePoshDisable nullHardwarePoshDisable =
            {
                m_NullHardwarePoshRegister,
                Base::m_NullHardwareDisable
            };

            return nullHardwarePoshDisable;
        }
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct GpuRegistersTrait : GEN11::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, GEN11 );

        // Render registers.
        static constexpr uint32_t m_RenderMmioRangeBegin = 0x2000;
        static constexpr uint32_t m_RenderMmioRangeEnd   = 0x27FF;

        // Oag registers.
        static constexpr uint32_t m_OagBuffer   = 0xDB08;
        static constexpr uint32_t m_OagStatus   = 0xDAFC;
        static constexpr uint32_t m_OagHead     = 0xDB00;
        static constexpr uint32_t m_OagTail     = 0xDB04;
        static constexpr uint32_t m_OagTrigger2 = 0xD924;
        static constexpr uint32_t m_OagTrigger6 = 0xD934;
        static constexpr uint32_t m_OagGpuTicks = 0xDA90;

        // Query registers.
        static constexpr uint32_t m_OaBuffer   = m_OagBuffer;
        static constexpr uint32_t m_OaStatus   = m_OagStatus;
        static constexpr uint32_t m_OaHead     = m_OagHead;
        static constexpr uint32_t m_OaTail     = m_OagTail;
        static constexpr uint32_t m_OaTrigger2 = m_OagTrigger2;
        static constexpr uint32_t m_OaTrigger6 = m_OagTrigger6;

        // Oag counters low.
        static constexpr uint32_t m_OagPerfA0  = 0xD980;
        static constexpr uint32_t m_OagPerfA1  = 0xD988;
        static constexpr uint32_t m_OagPerfA2  = 0xD990;
        static constexpr uint32_t m_OagPerfA3  = 0xD998;
        static constexpr uint32_t m_OagPerfA4  = 0xD9A0;
        static constexpr uint32_t m_OagPerfA5  = 0xD9A8;
        static constexpr uint32_t m_OagPerfA6  = 0xD9B0;
        static constexpr uint32_t m_OagPerfA7  = 0xD9B8;
        static constexpr uint32_t m_OagPerfA8  = 0xD9C0;
        static constexpr uint32_t m_OagPerfA9  = 0xD9C8;
        static constexpr uint32_t m_OagPerfA10 = 0xD9D0;
        static constexpr uint32_t m_OagPerfA11 = 0xD9D8;
        static constexpr uint32_t m_OagPerfA12 = 0xD9E0;
        static constexpr uint32_t m_OagPerfA13 = 0xD9E8;
        static constexpr uint32_t m_OagPerfA14 = 0xD9F0;
        static constexpr uint32_t m_OagPerfA15 = 0xD9F8;
        static constexpr uint32_t m_OagPerfA16 = 0xDA00;
        static constexpr uint32_t m_OagPerfA17 = 0xDA08;
        static constexpr uint32_t m_OagPerfA18 = 0xDA10;
        static constexpr uint32_t m_OagPerfA19 = 0xDA18;
        static constexpr uint32_t m_OagPerfA20 = 0xDA20;
        static constexpr uint32_t m_OagPerfA21 = 0xDA28;
        static constexpr uint32_t m_OagPerfA22 = 0xDA30;
        static constexpr uint32_t m_OagPerfA23 = 0xDA38;
        static constexpr uint32_t m_OagPerfA24 = 0xDA40;
        static constexpr uint32_t m_OagPerfA25 = 0xDA48;
        static constexpr uint32_t m_OagPerfA26 = 0xDA50;
        static constexpr uint32_t m_OagPerfA27 = 0xDA58;
        static constexpr uint32_t m_OagPerfA28 = 0xDA60;
        static constexpr uint32_t m_OagPerfA29 = 0xDA68;
        static constexpr uint32_t m_OagPerfA30 = 0xDA70;
        static constexpr uint32_t m_OagPerfA31 = 0xDA78;
        static constexpr uint32_t m_OagPerfA32 = 0xDA80;
        static constexpr uint32_t m_OagPerfA33 = 0xDA84;
        static constexpr uint32_t m_OagPerfA34 = 0xDA88;
        static constexpr uint32_t m_OagPerfA35 = 0xDA8C;

        // Oag counters high.
        static constexpr uint32_t m_OagPerfA0Upper  = 0xD984;
        static constexpr uint32_t m_OagPerfA1Upper  = 0xD98C;
        static constexpr uint32_t m_OagPerfA2Upper  = 0xD994;
        static constexpr uint32_t m_OagPerfA3Upper  = 0xD99C;
        static constexpr uint32_t m_OagPerfA4Upper  = 0xD9A4;
        static constexpr uint32_t m_OagPerfA5Upper  = 0xD9AC;
        static constexpr uint32_t m_OagPerfA6Upper  = 0xD9B4;
        static constexpr uint32_t m_OagPerfA7Upper  = 0xD9BC;
        static constexpr uint32_t m_OagPerfA8Upper  = 0xD9C4;
        static constexpr uint32_t m_OagPerfA9Upper  = 0xD9CC;
        static constexpr uint32_t m_OagPerfA10Upper = 0xD9D4;
        static constexpr uint32_t m_OagPerfA11Upper = 0xD9DC;
        static constexpr uint32_t m_OagPerfA12Upper = 0xD9E4;
        static constexpr uint32_t m_OagPerfA13Upper = 0xD9EC;
        static constexpr uint32_t m_OagPerfA14Upper = 0xD9F4;
        static constexpr uint32_t m_OagPerfA15Upper = 0xD9FC;
        static constexpr uint32_t m_OagPerfA16Upper = 0xDA04;
        static constexpr uint32_t m_OagPerfA17Upper = 0xDA0C;
        static constexpr uint32_t m_OagPerfA18Upper = 0xDA14;
        static constexpr uint32_t m_OagPerfA19Upper = 0xDA1C;
        static constexpr uint32_t m_OagPerfA20Upper = 0xDA24;
        static constexpr uint32_t m_OagPerfA21Upper = 0xDA2C;
        static constexpr uint32_t m_OagPerfA22Upper = 0xDA34;
        static constexpr uint32_t m_OagPerfA23Upper = 0xDA3C;
        static constexpr uint32_t m_OagPerfA24Upper = 0xDA44;
        static constexpr uint32_t m_OagPerfA25Upper = 0xDA4C;
        static constexpr uint32_t m_OagPerfA26Upper = 0xDA54;
        static constexpr uint32_t m_OagPerfA27Upper = 0xDA5C;
        static constexpr uint32_t m_OagPerfA28Upper = 0xDA64;
        static constexpr uint32_t m_OagPerfA29Upper = 0xDA6C;
        static constexpr uint32_t m_OagPerfA30Upper = 0xDA74;
        static constexpr uint32_t m_OagPerfA31Upper = 0xDA7C;

        // Noag counters.
        static constexpr uint32_t m_OagPerfB0 = 0xDA94;
        static constexpr uint32_t m_OagPerfB1 = 0xDA98;
        static constexpr uint32_t m_OagPerfB2 = 0xDA9C;
        static constexpr uint32_t m_OagPerfB3 = 0xDAA0;
        static constexpr uint32_t m_OagPerfB4 = 0xDAA4;
        static constexpr uint32_t m_OagPerfB5 = 0xDAA8;
        static constexpr uint32_t m_OagPerfB6 = 0xDAAC;
        static constexpr uint32_t m_OagPerfB7 = 0xDAB0;
        static constexpr uint32_t m_OagPerfC0 = 0xDAB4;
        static constexpr uint32_t m_OagPerfC1 = 0xDAB8;
        static constexpr uint32_t m_OagPerfC2 = 0xDABC;
        static constexpr uint32_t m_OagPerfC3 = 0xDAC0;
        static constexpr uint32_t m_OagPerfC4 = 0xDAC4;
        static constexpr uint32_t m_OagPerfC5 = 0xDAC8;
        static constexpr uint32_t m_OagPerfC6 = 0xDACC;
        static constexpr uint32_t m_OagPerfC7 = 0xDAD0;

        // Stream markers registers.
        static constexpr uint32_t m_StreamMarker        = m_OagPerfA19;
        static constexpr uint32_t m_StreamMarkerRender  = m_OagPerfA20;
        static constexpr uint32_t m_StreamMarkerPosh    = m_OagPerfA19;
        static constexpr uint32_t m_StreamMarkerCompute = m_OagPerfA18;

        // Overrides registers.
        static constexpr uint32_t m_CullAtVfRegister = 0x6100;
        static constexpr uint32_t m_CullAtVfEnable   = 0x08020802;
        static constexpr uint32_t m_CullAtVfDisable  = 0x08020000;

        static constexpr uint32_t m_CullAtVsRegister = m_CullAtVfRegister;
        static constexpr uint32_t m_CullAtVsEnable   = 0x00040004;
        static constexpr uint32_t m_CullAtVsDisable  = 0x00040000;

        static constexpr uint32_t m_CullAtHsRegister = m_CullAtVfRegister;
        static constexpr uint32_t m_CullAtHsEnable   = 0x00080008;
        static constexpr uint32_t m_CullAtHsDisable  = 0x00080000;

        static constexpr uint32_t m_CullAtTeRegister = m_CullAtVfRegister;
        static constexpr uint32_t m_CullAtTeEnable   = 0x00100010;
        static constexpr uint32_t m_CullAtTeDisable  = 0x00100000;

        static constexpr uint32_t m_CullAtDsRegister = m_CullAtVfRegister;
        static constexpr uint32_t m_CullAtDsEnable   = 0x00200020;
        static constexpr uint32_t m_CullAtDsDisable  = 0x00200000;

        static constexpr uint32_t m_CullAtGsRegister = m_CullAtVfRegister;
        static constexpr uint32_t m_CullAtGsEnable   = 0x00400040;
        static constexpr uint32_t m_CullAtGsDisable  = 0x00400000;

        static constexpr uint32_t m_CullAtSolRegister = m_CullAtVfRegister;
        static constexpr uint32_t m_CullAtSolEnable   = 0x00800080;
        static constexpr uint32_t m_CullAtSolDisable  = 0x00800000;

        static constexpr uint32_t m_CullAtClRegister = m_CullAtVfRegister;
        static constexpr uint32_t m_CullAtClEnable   = 0x01000100;
        static constexpr uint32_t m_CullAtClDisable  = 0x01000000;

        static constexpr uint32_t m_CullAtSfRegister = m_CullAtVfRegister;
        static constexpr uint32_t m_CullAtSfEnable   = 0x02000200;
        static constexpr uint32_t m_CullAtSfDisable  = 0x02000000;

        // Overrides array.
        using CullAtCsEnable   = std::array<uint32_t, 0>;
        using CullAtCsDisable  = std::array<uint32_t, 0>;
        using CullAtVfEnable   = std::array<uint32_t, 2>;
        using CullAtVfDisable  = std::array<uint32_t, 2>;
        using CullAtVsEnable   = std::array<uint32_t, 2>;
        using CullAtVsDisable  = std::array<uint32_t, 2>;
        using CullAtHsEnable   = std::array<uint32_t, 2>;
        using CullAtHsDisable  = std::array<uint32_t, 2>;
        using CullAtTeEnable   = std::array<uint32_t, 2>;
        using CullAtTeDisable  = std::array<uint32_t, 2>;
        using CullAtDsEnable   = std::array<uint32_t, 2>;
        using CullAtDsDisable  = std::array<uint32_t, 2>;
        using CullAtGsEnable   = std::array<uint32_t, 2>;
        using CullAtGsDisable  = std::array<uint32_t, 2>;
        using CullAtSolEnable  = std::array<uint32_t, 2>;
        using CullAtSolDisable = std::array<uint32_t, 2>;
        using CullAtClEnable   = std::array<uint32_t, 2>;
        using CullAtClDisable  = std::array<uint32_t, 2>;
        using CullAtSfEnable   = std::array<uint32_t, 2>;
        using CullAtSfDisable  = std::array<uint32_t, 2>;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  cull at compute shader stage override is not supported on Gen12.
        /// @return empty array.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtCsEnable& GetCullAtCsEnablingRegisters()
        {
            static constexpr CullAtCsEnable cullAtCsEnable = {};

            return cullAtCsEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  cull at compute shader stage override is not supported on Gen12.
        /// @return empty array.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtCsDisable& GetCullAtCsDisablingRegisters()
        {
            static constexpr CullAtCsDisable cullAtCsDisable = {};

            return cullAtCsDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at vertex fetch stage override enabling registers
        ///         and values.
        /// @return cull at vertex fetch stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtVfEnable& GetCullAtVfEnablingRegisters()
        {
            static constexpr CullAtVfEnable cullAtVfEnable =
            {
                m_CullAtVfRegister,
                m_CullAtVfEnable
            };

            return cullAtVfEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at vertex fetch stage override disabling registers
        ///         and values.
        /// @return cull at vertex fetch stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtVfDisable& GetCullAtVfDisablingRegisters()
        {
            static constexpr CullAtVfDisable cullAtVfDisable =
            {
                m_CullAtVfRegister,
                m_CullAtVfDisable
            };

            return cullAtVfDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at vertex shader stage override enabling registers
        ///         and values.
        /// @return cull at vertex shader stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtVsEnable& GetCullAtVsEnablingRegisters()
        {
            static constexpr CullAtVsEnable cullAtVsEnable =
            {
                m_CullAtVsRegister,
                m_CullAtVsEnable
            };

            return cullAtVsEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at vertex shader stage override disabling registers
        ///         and values.
        /// @return cull at vertex shader stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtVsDisable& GetCullAtVsDisablingRegisters()
        {
            static constexpr CullAtVsDisable cullAtVsDisable =
            {
                m_CullAtVsRegister,
                m_CullAtVsDisable
            };

            return cullAtVsDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at hull shader stage override enabling registers
        ///         and values.
        /// @return cull at hull shader stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtHsEnable& GetCullAtHsEnablingRegisters()
        {
            static constexpr CullAtHsEnable cullAtHsEnable =
            {
                m_CullAtHsRegister,
                m_CullAtHsEnable
            };

            return cullAtHsEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at hull shader stage override disabling registers
        ///         and values.
        /// @return cull at hull shader stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtHsDisable& GetCullAtHsDisablingRegisters()
        {
            static constexpr CullAtHsDisable cullAtHsDisable =
            {
                m_CullAtHsRegister,
                m_CullAtHsDisable
            };

            return cullAtHsDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at tessellator stage override enabling registers
        ///         and values.
        /// @return cull at tessellator stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtTeEnable& GetCullAtTeEnablingRegisters()
        {
            static constexpr CullAtTeEnable cullAtTeEnable =
            {
                m_CullAtTeRegister,
                m_CullAtTeEnable
            };

            return cullAtTeEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at tessellator stage override disabling registers
        ///         and values.
        /// @return cull at tessellator stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtTeDisable& GetCullAtTeDisablingRegisters()
        {
            static constexpr CullAtTeDisable cullAtTeDisable =
            {
                m_CullAtTeRegister,
                m_CullAtTeDisable
            };

            return cullAtTeDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at domain shader stage override enabling registers
        ///         and values.
        /// @return cull at domain shader stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtDsEnable& GetCullAtDsEnablingRegisters()
        {
            static constexpr CullAtDsEnable cullAtDsEnable =
            {
                m_CullAtDsRegister,
                m_CullAtDsEnable
            };

            return cullAtDsEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at domain shader stage override disabling registers
        ///         and values.
        /// @return cull at domain shader stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtDsDisable& GetCullAtDsDisablingRegisters()
        {
            static constexpr CullAtDsDisable cullAtDsDisable =
            {
                m_CullAtDsRegister,
                m_CullAtDsDisable
            };

            return cullAtDsDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at geometry shader stage override enabling
        ///         registers and values.
        /// @return cull at geometry shader stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtGsEnable& GetCullAtGsEnablingRegisters()
        {
            static constexpr CullAtGsEnable cullAtGsEnable =
            {
                m_CullAtGsRegister,
                m_CullAtGsEnable
            };

            return cullAtGsEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at geometry shader stage override disabling
        ///         registers and values.
        /// @return cull at geometry shader stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtGsDisable& GetCullAtGsDisablingRegisters()
        {
            static constexpr CullAtGsDisable cullAtGsDisable =
            {
                m_CullAtGsRegister,
                m_CullAtGsDisable
            };

            return cullAtGsDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at stream output logic stage override enabling
        ///         registers and values.
        /// @return cull at stream output logic stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtSolEnable& GetCullAtSolEnablingRegisters()
        {
            static constexpr CullAtSolEnable cullAtSolEnable =
            {
                m_CullAtSolRegister,
                m_CullAtSolEnable
            };

            return cullAtSolEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at stream output logic stage override disabling
        ///         registers and values.
        /// @return cull at stream output logic stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtSolDisable& GetCullAtSolDisablingRegisters()
        {
            static constexpr CullAtSolDisable cullAtSolDisable =
            {
                m_CullAtSolRegister,
                m_CullAtSolDisable
            };

            return cullAtSolDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at clip stage override enabling registers
        ///         and values.
        /// @return cull at clip stage override enabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtClEnable& GetCullAtClEnablingRegisters()
        {
            static constexpr CullAtClEnable cullAtClEnable =
            {
                m_CullAtClRegister,
                m_CullAtClEnable
            };

            return cullAtClEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at clip stage override disabling registers
        ///         and values.
        /// @return cull at clip stage override disabling registers and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtClDisable& GetCullAtClDisablingRegisters()
        {
            static constexpr CullAtClDisable cullAtClDisable =
            {
                m_CullAtClRegister,
                m_CullAtClDisable
            };

            return cullAtClDisable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at strips and fans stage override enabling
        ///         registers and values.
        /// @return cull at strips and fans stage override enabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtSfEnable& GetCullAtSfEnablingRegisters()
        {
            static constexpr CullAtSfEnable cullAtSfEnable =
            {
                m_CullAtSfRegister,
                m_CullAtSfEnable
            };

            return cullAtSfEnable;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns cull at strips and fans stage override disabling
        ///         registers and values.
        /// @return cull at strips and fans stage override disabling registers
        ///         and values.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const CullAtSfDisable& GetCullAtSfDisablingRegisters()
        {
            static constexpr CullAtSfDisable cullAtSfDisable =
            {
                m_CullAtSfRegister,
                m_CullAtSfDisable
            };

            return cullAtSfDisable;
        }
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct GpuRegistersTrait : XE_LP::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, XE_LP );

        // Oar A0 used as render context id cache.
        static constexpr uint32_t m_ContextId = Base::m_OaPerfA0;

        // Oac A0 used as compute context id cache.
        static constexpr uint32_t m_ContextIdCompute = 0x15000;

        // Oag mmio trigger.
        static constexpr uint32_t m_OagTrigger = 0xDB1C;

        // Oag counters low.
        static constexpr uint32_t m_OagPerfA36 = 0xDB20;
        static constexpr uint32_t m_OagPerfA37 = 0xDB28;

        // Oag counters high.
        // A32 - A35 upper registers are missing on XeHP.
        static constexpr uint32_t m_OagPerfA36Upper = 0xDB24;
        static constexpr uint32_t m_OagPerfA37Upper = 0xDB2C;

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
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct GpuRegistersTrait : XE_HP::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, XE_HP );

        // Oag registers.
        static constexpr uint32_t m_OagGpuTicks      = 0xDB70;
        static constexpr uint32_t m_OagGpuTicksUpper = 0xDB74;

        // Oag counters low.
        // Different offsets than previous gens.
        static constexpr uint32_t m_OagPerfA33 = 0xDA88;
        static constexpr uint32_t m_OagPerfA34 = 0xDB60;
        static constexpr uint32_t m_OagPerfA35 = 0xDB68;

        // Oag counters high.
        static constexpr uint32_t m_OagPerfA32Upper = 0xDA84;
        static constexpr uint32_t m_OagPerfA33Upper = 0xDA8C;
        static constexpr uint32_t m_OagPerfA34Upper = 0xDB64;
        static constexpr uint32_t m_OagPerfA35Upper = 0xDB6C;

        // Oar registers.
        static constexpr uint32_t m_OarGpuTicks      = 0x29C0;
        static constexpr uint32_t m_OarGpuTicksUpper = 0x29C4;

        // Oar counters low.
        // Different offsets than previous gens.
        static constexpr uint32_t m_OarPerfA33 = 0x2908;
        static constexpr uint32_t m_OarPerfA34 = 0x29B0;
        static constexpr uint32_t m_OarPerfA35 = 0x29B8;

        // Oar counters high.
        // Different offsets than previous gens.
        static constexpr uint32_t m_OarPerfA32Upper = 0x2904;
        static constexpr uint32_t m_OarPerfA33Upper = 0x290C;
        static constexpr uint32_t m_OarPerfA34Upper = 0x29B4;
        static constexpr uint32_t m_OarPerfA35Upper = 0x29BC;
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct GpuRegistersTrait : XE_HPG::GpuRegistersTrait<T>
    {
        ML_DECLARE_TRAIT( GpuRegistersTrait, XE_HPG );

        // Oac registers.
        static constexpr uint32_t m_OacControl = 0x15114;
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

// clang-format on
