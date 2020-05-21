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

@file ml_constants_os.h

@brief Os specific constants.

\******************************************************************************/

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
            enum class Revision : uint32_t
            {
                Unsupported     = 0,
                OaSupported     = 1,
                OaConfiguration = 2,
                OaBufferMapping = 4,
            };

            static constexpr bool        m_MappingRequired     = false;
            static constexpr bool        m_PrintOaBuffer       = false;
            static constexpr bool        m_PrintOaCache        = false;
            static constexpr uint32_t    m_CacheCapacity       = 100;
            static constexpr uint32_t    m_TimerPeriod         = 0xFFFFFFFF;
            static constexpr uint32_t    m_ReportType          = I915_OA_FORMAT_A32u40_A4u32_B8_C8;
            static constexpr int64_t     m_Invalid             = -1;
            static constexpr const char* m_ActiveMetricSetPath = "/sys/class/drm/card%d/metrics/%s/id";
            static constexpr const char* m_ActiveMetricSetGuid = "2f01b241-7014-42a7-9eb6-a925cad3daba";
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Direct rendering manager constants.
        //////////////////////////////////////////////////////////////////////////
        struct Drm
        {
            static constexpr int32_t     m_Invalid = -1;
            static constexpr const char* m_Name    = "i915";
            static constexpr const char* m_Path    = "/sys/dev/char/%d:%d/device/drm";
        };
    };
} // namespace ML
