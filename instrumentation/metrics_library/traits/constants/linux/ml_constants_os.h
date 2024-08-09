/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

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
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "ConstantsOs<Traits> (Linux)";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Time based sampling constants.
        //////////////////////////////////////////////////////////////////////////
        struct Tbs
        {
            static constexpr bool        m_MappingRequired     = false;
            static constexpr bool        m_PrintOaBuffer       = false;
            static constexpr bool        m_PrintOaCache        = false;
            static constexpr uint32_t    m_CacheCapacity       = 100;
            static constexpr uint32_t    m_TimerPeriod         = 0xFFFFFFFF;
            static constexpr int64_t     m_Invalid             = -1;
            static constexpr const char* m_ActiveMetricSetPath = "/sys/class/drm/card%d/metrics/%s/id";
            static constexpr const char* m_ActiveMetricSetGuid = "2f01b241-7014-42a7-9eb6-a925cad3daba";
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
                Unsupported      = 0,
                OaSupported      = 1,
                OaConfiguration  = 2,
                HoldPreemption   = 3,
                AllowedSsEu      = 4,
                PollOaPeriod     = 5,
                TriggerReports   = 6,
                TriggerWhitelist = 7,
                OaBufferMapping  = 8,
                SubDevices       = 10
            };

            static constexpr int32_t     m_Invalid = -1;
            static constexpr const char* m_Path    = "/sys/dev/char/%d:%d/device/drm";
        };
    };
} // namespace ML
