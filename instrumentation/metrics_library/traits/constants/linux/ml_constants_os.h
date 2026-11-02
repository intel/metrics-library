/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2026 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_constants_os.h

@brief Os specific constants.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Os based constants.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ConstantsOsTrait : public Constants
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ConstantsOsTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ConstantsOsTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Time based sampling constants.
        //////////////////////////////////////////////////////////////////////////
        struct Tbs
        {
            static constexpr uint64_t    m_TimerPeriod                  = 0xFFFFFFFFFFFFFFFF;
            static constexpr uint32_t    m_ConfigGuidSubDeviceLegacy    = 0x42a7;
            static constexpr uint32_t    m_ConfigGuidPrefixOa           = 0x2f01b241;
            static constexpr const char* m_ConfigGuidPrefixOaString     = "2f01b241";
            static constexpr uint32_t    m_ConfigGuidPrefixOaMert       = 0x8c3d5e9a;
            static constexpr const char* m_ConfigGuidPrefixOaMertString = "8c3d5e9a";
            static constexpr const char* m_ConfigGuidFormat             = "%08x-%04x-%04x-%04x-%012" PRIx64;
            static constexpr const char* m_ConfigsPath                  = "/sys/class/drm/card%d/metrics";
            static constexpr const char* m_ConfigPath                   = "/sys/class/drm/card%d/metrics/%s/id";
            static constexpr const char* m_ConfigPathRelative           = "%s/id";
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Direct rendering manager constants.
        //////////////////////////////////////////////////////////////////////////
        struct Drm
        {
            ////////////////////////////////////////////////////////////////////////
            /// @brief Drm revision.
            //////////////////////////////////////////////////////////////////////////
            enum class Revision : uint32_t
            {
                Unsupported         = 0,
                OaSupported         = 1, // I915_PERF_IOCTL_ENABLE & I915_PERF_IOCTL_DISABLE
                OaConfiguration     = 2, // I915_PERF_IOCTL_CONFIG
                HoldPreemption      = 3, // DRM_I915_PERF_PROP_HOLD_PREEMPTION
                AllowedSsEu         = 4, // DRM_I915_PERF_PROP_ALLOWED_SSEU
                PollOaPeriod        = 5, // DRM_I915_PERF_PROP_POLL_OA_PERIOD
                EngineClassInstance = 6, // DRM_I915_PERF_PROP_OA_ENGINE_CLASS & DRM_I915_PERF_PROP_OA_ENGINE_INSTANCE
                OamSupported        = 7, // OAM support

                PrelimOaBufferMap         = PRELIM_PERF_VERSION + 0, // PRELIM_I915_PERF_IOCTL_GET_OA_BUFFER_INFO
                PrelimOaBufferSize        = PRELIM_PERF_VERSION + 1, // PRELIM_DRM_I915_PERF_PROP_OA_BUFFER_SIZE
                PrelimEngineClassInstance = PRELIM_PERF_VERSION + 2, // PRELIM_DRM_I915_PERF_PROP_OA_ENGINE_CLASS & PRELIM_DRM_I915_PERF_PROP_OA_ENGINE_INSTANCE
                PrelimMmioTrgQueueFull    = PRELIM_PERF_VERSION + 3, // PRELIM_DRM_I915_PERF_RECORD_OA_MMIO_TRG_Q_FULL
                PrelimOamSupported        = PRELIM_PERF_VERSION + 4, // OAM support
                PrelimOacSupported        = PRELIM_PERF_VERSION + 5, // OAC support
                PrelimEussSupported       = PRELIM_PERF_VERSION + 6, // EUSS support
                PrelimMpesConfiguration   = PRELIM_PERF_VERSION + 7, // MPES configuration support
                PrelimOaNotifyNumReports  = PRELIM_PERF_VERSION + 8, // PRELIM_DRM_I915_PERF_PROP_OA_NOTIFY_NUM_REPORTS
            };

            static constexpr int32_t     m_Invalid = -1;
            static constexpr const char* m_Path    = "/sys/dev/char/%d:%d/device/drm";
        };
    };
} // namespace ML
