/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_constants.h

@brief Constants.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Constants object.
    //////////////////////////////////////////////////////////////////////////
    struct Constants
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( Constants );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( Constants );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "Constants";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief String constants.
        //////////////////////////////////////////////////////////////////////////
        struct String
        {
            static constexpr uint32_t m_MaxLength = 1024;
            static constexpr uint32_t m_MaxPath   = 260;
            using Buffer                          = char[m_MaxLength];
            using Path                            = char[m_MaxPath];
            using WideBuffer                      = wchar_t[m_MaxLength];
            using WidePath                        = wchar_t[m_MaxPath];
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Time units constants.
        //////////////////////////////////////////////////////////////////////////
        struct Time
        {
            static constexpr uint32_t m_Megahertz           = 1000000;
            static constexpr uint64_t m_SecondInNanoseconds = 1000000000;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Data sizes.
        //////////////////////////////////////////////////////////////////////////
        struct Data
        {
            static constexpr uint32_t m_Byte     = 8;
            static constexpr uint64_t m_Megabyte = 1048576;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Combo timestamps constants.
        //////////////////////////////////////////////////////////////////////////
        struct ComboTimestamp
        {
            static constexpr uint32_t m_CpuShift  = 32u;
            static constexpr uint64_t m_GpuMask56 = 0x00FFFFFFFFFFFFFFLL; // Only 56 bits are available in gpu timestamp in 64-bit counter.
            static constexpr uint64_t m_GpuMask32 = 0x00000000FFFFFFFFLL; // Only 32 bits are available in gpu timestamp in 32-bit counter.
            static constexpr uint64_t m_GpuMask29 = 0x000000001FFFFFFFLL; // We need to extend the range, thus we use also cpu timestamp to create combo value.
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Gpu counters configuration constants.
        //////////////////////////////////////////////////////////////////////////
        struct Configuration
        {
            static constexpr uint32_t m_InvalidHandle        = 0;
            static constexpr uint32_t m_MaxOaProlog          = 3;
            static constexpr uint32_t m_MaxOaEpilog          = 1;
            static constexpr uint32_t m_MaxRegistersBase     = 4096;
            static constexpr uint32_t m_MaxRegisters         = m_MaxRegistersBase + m_MaxOaEpilog + m_MaxOaProlog;
            static constexpr uint32_t m_MaxOverrideRegisters = 256;
            static constexpr uint32_t m_MaxUserRegisters     = 16;
            static constexpr uint32_t m_MaxInitRegisters     = 20;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa buffer constants.
        //////////////////////////////////////////////////////////////////////////
        struct OaBuffer
        {
            static constexpr uint32_t m_InvalidOffset = 0xFFFFFFFF;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Stream marker constants.
        //////////////////////////////////////////////////////////////////////////
        struct StreamMarker
        {
            static constexpr uint32_t m_HighBitsShift = 25;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Query constants.
        //////////////////////////////////////////////////////////////////////////
        struct Query
        {
            static constexpr uint32_t m_DummyContextId = 0xFFFFFFFF;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Log constants.
        //////////////////////////////////////////////////////////////////////////
        struct Log
        {
            static constexpr const char  m_ScopeCharacter            = ':';
            static constexpr const char* m_EnteredFunctionTag        = "Enter:";
            static constexpr const char* m_ExitingFunctionTag        = "Exit :";
            static constexpr uint32_t    m_IndentSize                = 4;
            static constexpr bool        m_SimplifyFunctionName      = false;
            static constexpr uint32_t    m_SimplifyFunctionNameLevel = 0;
            static constexpr uint32_t    m_MaxFunctionNameLength     = 25;
            static constexpr uint32_t    m_MaxReportReasonLength     = 2;  //                   64.
            static constexpr uint32_t    m_MaxReportIdLength         = 4;  //                 4096.
            static constexpr uint32_t    m_MaxUint32Length           = 10; //           4294967295.
            static constexpr uint32_t    m_MaxUint64Length           = 20; // 18446744073709551615.
            static constexpr uint32_t    m_MaxLength                 = 4096;
            static constexpr uint32_t    m_MaxIndent                 = 10;
            static constexpr uint32_t    m_MaxMessageLength          = m_IndentSize * m_MaxIndent + 50;
            static constexpr bool        m_ShowDecimal               = true;
            static constexpr bool        m_ShowHexadecimal           = true;
            static constexpr bool        m_FilterEnabled             = false;
            static constexpr const char* m_FilteredString            = "";
            static constexpr uint32_t    m_FilteredLines             = 0;
            static constexpr bool        m_FlushEachLog              = true;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Library constants.
        //////////////////////////////////////////////////////////////////////////
        struct Library
        {
            static constexpr const char* m_Name        = "MetricsLibrary";
            static constexpr const char* m_GpuOverride = "Gpu";
            static constexpr const char* m_ApiOverride = "Api";
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Object constants.
        //////////////////////////////////////////////////////////////////////////
        struct Object
        {
            static constexpr uint32_t m_MagicHeader = 0xF1E2D3C4;
        };
    };
} // namespace ML
