/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_oa_buffer_layouts.h

@brief Layout for oa buffer.
*/

// clang-format off

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for OaBufferLayoutsTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct OaBufferLayoutsTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( OaBufferLayoutsTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( OaBufferLayoutsTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa report reason.
        //////////////////////////////////////////////////////////////////////////
        enum class ReportReason : uint32_t
        {
            Empty               = 0b0000000,
            Timer               = 0b0000001,
            User1               = 0b0000010,
            User2               = 0b0000100,
            ContextSwitch       = 0b0001000,
            C6                  = 0b0010000,
            FrequencyChange     = 0b0100000,
            MmioTrigger         = 0b1000000,

            // Aliases:
            ConfigurationUpdate = User1,
            WaBbContext         = User1,
            WaBbC6              = User2,
            TriggerQuery        = User1,
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Source id.
        //////////////////////////////////////////////////////////////////////////
        enum class SourceId : uint8_t
        {
            Other = 0b000000,
            Cpu   = 0b000001,
            Rcs   = 0b001000,
            Ccs   = 0b001001,
            Bcs   = 0b001010,
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa sampling mode enumeration.
        //////////////////////////////////////////////////////////////////////////
        enum class SamplingMode : uint32_t
        {
            Off = 0,
            Event,
            Timer,
            Dma,
            // ...
            Count
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa buffer sampling type enumeration.
        //////////////////////////////////////////////////////////////////////////
        enum class SamplingType : uint32_t
        {
            Disabled          = 0xFFFFFFFF,
            Tbs               = 1,
            QueryMultisampled = 0,
            QueryExtended     = 2
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Type enumeration related to oa buffer type.
        //////////////////////////////////////////////////////////////////////////
        enum class Type : uint32_t
        {
            Oa = 0
        };

        ML_STRUCTURE_PACK_BEGIN( 4 );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for oa buffer address structure.
        //////////////////////////////////////////////////////////////////////////
        struct Address
        {
            void*    m_CpuAddress;
            uint32_t m_GpuAddress;
        };

        ML_STRUCTURE_PACK_END();

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for UmdAddress union.
        ///        Pointers used in the interface between KMD and UMD. Please note,
        ///        that void is either 32 or 62-bit (depends on the OS).
        //////////////////////////////////////////////////////////////////////////
        union UmdAddress
        {
            uint64_t m_AsInteger;
            void*    m_AsPointer;
        };

        ML_STRUCTURE_PACK_BEGIN( 1 );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type to evaluate oa register value.
        //////////////////////////////////////////////////////////////////////////
        static constexpr uint32_t m_OaRegisterShift = 6;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for oa buffer register object.
        //////////////////////////////////////////////////////////////////////////
        struct Register
        {
            union
            {
                uint32_t m_Value;

                struct
                {
                    uint32_t m_MemorySelect             : ML_BITFIELD_BIT  (      0 );
                    uint32_t m_DisableOverrunMode       : ML_BITFIELD_BIT  (      1 );
                    uint32_t m_OaReportTriggerSelect    : ML_BITFIELD_BIT  (      2 );
                    uint32_t m_TriggerReportBufferSize  : ML_BITFIELD_RANGE(  3,  5 );
                    uint32_t m_ReportBufferOffset       : ML_BITFIELD_RANGE(  6, 31 );
                } All;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa buffer allocation offset.
            /// @return oa buffer allocation offset;
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetAllocationOffset() const
            {
                return All.m_ReportBufferOffset << m_OaRegisterShift;
            }
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for StatusRegister object.
        //////////////////////////////////////////////////////////////////////////
        struct StatusRegister
        {
            union
            {
                uint32_t m_Value;

                struct
                {
                    uint32_t m_ReportLostError        : ML_BITFIELD_BIT( 0 );         // Report Logic Request for error.
                    uint32_t m_BufferOverflow         : ML_BITFIELD_BIT( 1 );         // Tail - Head > Max buffer size.
                    uint32_t m_CounterOverflowError   : ML_BITFIELD_BIT( 2 );         // Set it a counter overflows.
                    uint32_t m_OverrunStatus          : ML_BITFIELD_BIT( 3 );         // Indicates the status of overrun for debug purpose.
                    uint32_t                          : ML_BITFIELD_RANGE( 4, 15 );
                    uint32_t m_HeadWrap               : ML_BITFIELD_BIT( 16 );
                    uint32_t m_TailWrap               : ML_BITFIELD_BIT( 17 );
                    uint32_t m_ReportTrigger2         : ML_BITFIELD_BIT( 18 );
                    uint32_t m_ReportTrigger1         : ML_BITFIELD_BIT( 19 );
                    uint32_t m_StartTrigger2          : ML_BITFIELD_BIT( 20 );
                    uint32_t m_StartTrigger1          : ML_BITFIELD_BIT( 21 );
                    uint32_t                          : ML_BITFIELD_RANGE( 22, 31 );
                } All;
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa tail register layout.
        //////////////////////////////////////////////////////////////////////////
        struct TailRegister
        {
            union
            {
                uint32_t m_Value;

                struct
                {
                    uint32_t          : ML_BITFIELD_RANGE( 0, 5 );
                    uint32_t m_Tail   : ML_BITFIELD_RANGE( 6, 31 ); // Virtual address of the internal trigger based buffer updated by software
                                                                    // after consuming from the report buffer. Updated by software.
                } All;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa tail offset.
            /// @return oa tail offset.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetOffset() const
            {
                return All.m_Tail << m_OaRegisterShift;
            }
        };

        ML_STRUCTURE_PACK_END();

        //////////////////////////////////////////////////////////////////////////
        /// @brief Common oa buffer state kept by hw counters query.
        //////////////////////////////////////////////////////////////////////////
        struct State
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            // Three members below refer to the oa buffer reports which timestamps are in query window.
            uint32_t                                m_CurrentOffset;       // Offset of current end report in the oa buffer.
            uint32_t                                m_NextAfterLastOffset; // Offset of next after last report from the oa buffer.
            uint32_t                                m_FirstOffset;         // Offset of oldest report in the oa buffer.

            uint32_t                                m_TailBeginOffset;     // First oa report after query begin.
            uint32_t                                m_TailEndOffset;       // First oa report after query end.

            uint32_t                                m_LogBeginOffset;      // Offset of the begin report from the current slot.
            uint32_t                                m_LogEndOffset;        // Offset of the end report from the current slot.

            TT::Layouts::HwCounters::ReportOa       m_ReportCopy[2];       // Buffer for begin & end reports.
            uint8_t                                 m_ReportCopyIndex;

            bool                                    m_ConfigurationValid;
            bool                                    m_ContextValid;

            //////////////////////////////////////////////////////////////////////////
            /// @brief State constructor.
            //////////////////////////////////////////////////////////////////////////
            State()
                : m_CurrentOffset( Constants::OaBuffer::m_InvalidOffset )
                , m_NextAfterLastOffset( Constants::OaBuffer::m_InvalidOffset )
                , m_FirstOffset( Constants::OaBuffer::m_InvalidOffset )
                , m_TailBeginOffset( 0 )
                , m_TailEndOffset( 0 )
                , m_LogBeginOffset( 0 )
                , m_LogEndOffset( 0 )
                , m_ReportCopy{}
                , m_ReportCopyIndex( 0 )
                , m_ConfigurationValid( true )
                , m_ContextValid( true )
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Resets oa buffer state.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void Reset()
            {
                m_FirstOffset         = Constants::OaBuffer::m_InvalidOffset;
                m_CurrentOffset       = Constants::OaBuffer::m_InvalidOffset;
                m_NextAfterLastOffset = Constants::OaBuffer::m_InvalidOffset;
                m_ConfigurationValid  = true;
                m_ContextValid        = true;
            }
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "OaBufferLayoutsTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Sanity check.
        //////////////////////////////////////////////////////////////////////////
        ML_STATIC_ASSERT( ML_SIZE32( StatusRegister ) == 1, "Wrong oa status register structure size" );
        ML_STATIC_ASSERT( ML_SIZE32( TailRegister ) == 1,   "Wrong oa tail register structure size" );
    };
} // namespace ML
// clang-format on
