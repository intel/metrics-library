/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_hw_counters_layouts.h

@brief Layout for hw counters.
*/

// clang-format off

#pragma once

ML_STRUCTURE_PACK_BEGIN( 1 );

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for HwCountersLayoutsTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct HwCountersLayoutsTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( HwCountersLayoutsTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( HwCountersLayoutsTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "HwCountersLayoutsTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Constants.
        //////////////////////////////////////////////////////////////////////////
        static constexpr uint32_t m_OaCountersCount            = 36;
        static constexpr uint32_t m_OaCounters40bitsCount      = 32;
        static constexpr uint32_t m_NoaCountersCount           = 16;
        static constexpr uint32_t m_UserCountersCount          = 16;
        static constexpr uint32_t m_ReportGpuAlignment         = 64;
        static constexpr uint32_t m_TriggeredReportGetAttempts = 10;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report gp.
        //////////////////////////////////////////////////////////////////////////
        struct ReportGp
        {
            uint64_t    m_Counter1;
            uint64_t    m_Counter2;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report user.
        //////////////////////////////////////////////////////////////////////////
        struct ReportUser
        {
            uint64_t    m_Counter[m_UserCountersCount];
            uint32_t    m_UserCounterConfigurationId;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report id.
        //////////////////////////////////////////////////////////////////////////
        struct ReportId
        {
            union
            {
                uint32_t        m_Value;

                struct
                {
                    uint32_t    m_QueryId            : ML_BITFIELD_RANGE( 0,  11 );
                    uint32_t    m_ContextId          : ML_BITFIELD_RANGE( 12, 31 );
                }; // For MI_RPC.

                struct
                {
                    uint32_t    m_FrequencyUnslice   : ML_BITFIELD_RANGE(  0,  8 );
                    uint32_t    m_FrequencySliceHigh : ML_BITFIELD_RANGE(  9, 10 );
                    uint32_t    m_NoaEvents          : ML_BITFIELD_RANGE( 11, 15 );
                    uint32_t    m_ContextValid       : ML_BITFIELD_BIT  (     16 );
                    uint32_t    m_ThresholdEnable    : ML_BITFIELD_BIT  (     17 );
                    uint32_t    m_StartTriggerEvent  : ML_BITFIELD_BIT  (     18 );
                    uint32_t    m_ReportReason       : ML_BITFIELD_RANGE( 19, 24 );
                    uint32_t    m_FrequencySliceLow  : ML_BITFIELD_RANGE( 25, 31 );
                }; // For time based sampling.
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report header.
        //////////////////////////////////////////////////////////////////////////
        struct ReportHeader
        {
            ReportId    m_ReportId;
            uint32_t    m_Timestamp;
            uint32_t    m_ContextId;
            uint32_t    m_GpuTicks;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa report data.
        //////////////////////////////////////////////////////////////////////////
        struct ReportOaData
        {
            uint32_t    m_OaCounter[m_OaCountersCount];             // A0 - A35.
            uint8_t     m_OaCounterHB[m_OaCounters40bitsCount];     // High bytes of A0 - A31.
            uint32_t    m_NoaCounter[m_NoaCountersCount];
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa report.
        //////////////////////////////////////////////////////////////////////////
        struct ReportOa
        {
            ReportHeader    m_Header;
            ReportOaData    m_Data;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Unaligned report.
        //////////////////////////////////////////////////////////////////////////
        struct ReportUnaligned
        {
            ReportHeader    m_Header;
            ReportOaData    m_DataOa;
            ReportGp        m_DataGp;       // 16 bytes.
            ReportUser      m_DataUser;     // 16 * 8 + 4 = 132 bytes.
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report alignment.
        //////////////////////////////////////////////////////////////////////////
        static constexpr uint32_t m_ReportAlignment = ( (sizeof ( ReportUnaligned ) % m_ReportGpuAlignment )
            ? ( ( sizeof( ReportUnaligned ) / m_ReportGpuAlignment + 1 ) * m_ReportGpuAlignment )
            : sizeof( ReportUnaligned ) );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for Report object.
        //////////////////////////////////////////////////////////////////////////
        struct Report
        {
            union
            {
                struct
                {
                    ReportOa      m_Oa;      // Oa.
                    ReportGp      m_Gp;      // 16 bytes.
                    ReportUser    m_User;    // 16 * 8 + 4 = 132 bytes.
                };

                // OA reports have to be aligned to multiple of 64 bytes.
                uint8_t m_Payload[m_ReportAlignment];
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Hw counters structures related to query.
        //////////////////////////////////////////////////////////////////////////
        struct Query
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Api report flags.
            //////////////////////////////////////////////////////////////////////////
            union ReportApiFlags
            {
                struct
                {
                    uint32_t    m_ReportLost                : ML_BITFIELD_BIT( 0 );
                    uint32_t    m_ReportInconsistent        : ML_BITFIELD_BIT( 1 );
                    uint32_t    m_ReportNotReady            : ML_BITFIELD_BIT( 2 );
                    uint32_t    m_ReportContextSwitchLost   : ML_BITFIELD_BIT( 3 );
                    uint32_t    m_ReportWithoutWorkload     : ML_BITFIELD_BIT( 4 );
                    uint32_t    m_ContextMismatch           : ML_BITFIELD_BIT( 5 );
                    uint32_t    m_QueryModeMismatch         : ML_BITFIELD_BIT( 6 );
                    uint32_t    m_Reserved                  : ML_BITFIELD_RANGE( 7, 31 );
                };
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Api report format.
            //////////////////////////////////////////////////////////////////////////
            struct ReportApi
            {
                uint64_t                               m_TotalTime;                     // Total query time in nanoseconds.
                uint64_t                               m_GpuTicks;
                uint64_t                               m_OaCounter[m_OaCountersCount];
                uint64_t                               m_NoaCounter[m_NoaCountersCount];
                uint64_t                               m_BeginTimestamp;
                uint64_t                               m_Reserved1;
                uint64_t                               m_Reserved2;
                TT::Layouts::OaBuffer::ReportReason    m_MiddleQueryEvents;             // All types of events occurring between query begin and query end.
                                                                                        // Single event values are defined in ReportReason struct.
                uint32_t                               m_OverrunOccured;
                uint64_t                               m_MarkerUser;
                uint64_t                               m_MarkerDriver;

                uint64_t                               m_SliceFrequency;
                uint64_t                               m_UnsliceFrequency;
                uint64_t                               m_PerformanceCounter1;           // ODLAT Performance monitor counter 1.
                uint64_t                               m_PerformanceCounter2;           // ODLAT Performance monitor counter 2.
                uint32_t                               m_SplitOccured;                  // Command buffer split has occurred.
                uint32_t                               m_CoreFrequencyChanged;          // Core frequency has changed.
                uint64_t                               m_CoreFrequency;                 // Core frequency during the query.
                uint32_t                               m_ReportId;
                uint32_t                               m_ReportsCount;

                uint64_t                               m_UserCounter[m_UserCountersCount];
                uint32_t                               m_UserCounterConfigurationId;
                ReportApiFlags                         m_Flags;                         // Report flags are defined in ReportApiFlags.
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Oa report collecting mode.
            //////////////////////////////////////////////////////////////////////////
            enum class ReportCollectingMode : uint32_t
            {
                ReportPerformanceCounters = 0,
                StoreRegisterMemoryOar,
                StoreRegisterMemoryOag,
                TriggerOag,
                TriggerOagExtended,
                // ...
                Default = ReportPerformanceCounters
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Query mode.
            //////////////////////////////////////////////////////////////////////////
            enum class Mode : uint32_t
            {
                Render         = 1,
                Compute        = 2,
                Global         = 3,
                GlobalExtended = 4,
                // ...
                Default = Render
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Get data mode.
            //////////////////////////////////////////////////////////////////////////
            enum class GetDataMode : uint64_t
            {
                Normal       = 0xFFFFFFFF, // GTDI_SAMPLING_TYPE_MAX
                Multisampled = 0,          // GTDI_QUERY_MULTI_SAMPLES_TYPE
                Extended     = 2           // GTDI_QUERY_EXTENDED_TYPE
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Base type for QueryReportGpu object.
            //////////////////////////////////////////////////////////////////////////
            struct ReportGpu
            {
                // Begin/end report.
                Report      m_Begin;
                Report      m_End;

                // Required only for WaDoNotUseMIReportPerfCount for BDW+.
                uint32_t    m_WaBegin[m_OaCounters40bitsCount];
                uint32_t    m_WaEnd[m_OaCounters40bitsCount];

                union
                {
                    struct
                    {
                        // Query end marker.
                        uint64_t    m_EndTag;

                        // Command buffer split indicators.
                        uint32_t    m_DmaFenceIdBegin;
                        uint32_t    m_DmaFenceIdEnd;

                        // Oa buffer data related.
                        TT::Layouts::OaBuffer::Register          m_OaBuffer;
                        TT::Layouts::OaBuffer::TailRegister      m_OaTailPreBegin;
                        TT::Layouts::OaBuffer::TailRegister      m_OaTailPostBegin;
                        TT::Layouts::OaBuffer::TailRegister      m_OaTailPreEnd;
                        TT::Layouts::OaBuffer::TailRegister      m_OaTailPostEnd;

                        // Unused.
                        uint32_t    m_Reserved;

                        // Frequency change indicators.
                        uint32_t    m_CoreFrequencyBegin;
                        uint32_t    m_CoreFrequencyEnd;

                        // Markers.
                        uint64_t    m_MarkerUser;
                        uint64_t    m_MarkerDriver;
                    };

                    uint8_t    m_Payload[m_ReportGpuAlignment];
                };
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Sanity check.
        //////////////////////////////////////////////////////////////////////////
        ML_STATIC_ASSERT( ( sizeof( typename Query::ReportGpu ) % m_ReportGpuAlignment ) == 0, "Wrong structure size" );
        ML_STATIC_ASSERT( ( sizeof( Report )                    % m_ReportGpuAlignment ) == 0, "Wrong structure size" );
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct HwCountersLayoutsTrait : BASE::HwCountersLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( HwCountersLayoutsTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct HwCountersLayoutsTrait : GEN9::HwCountersLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( HwCountersLayoutsTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct HwCountersLayoutsTrait : GEN11::HwCountersLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( HwCountersLayoutsTrait, GEN11 );
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct HwCountersLayoutsTrait : XE_LP::HwCountersLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( HwCountersLayoutsTrait, XE_LP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Inherited report objects.
        //////////////////////////////////////////////////////////////////////////
        using ReportUser = typename Base::ReportUser;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Constants.
        //////////////////////////////////////////////////////////////////////////
        static constexpr uint32_t m_OagCountersCount                 = 38;
        static constexpr uint32_t m_OagCountersCountFrom0To36        = 37;
        static constexpr uint32_t m_OagCounters40bitsCountFrom4To23  = 20;
        static constexpr uint32_t m_OagCounters40bitsCountFrom28To31 = 4;
        static constexpr uint32_t m_OagCounters40bitsCountFrom0To31  = 32;
        static constexpr uint32_t m_OagCounters40bitsCount           = m_OagCounters40bitsCountFrom4To23 + m_OagCounters40bitsCountFrom28To31;
        static constexpr uint32_t m_OarCounters40bitsCount           = m_OagCounters40bitsCountFrom0To31;
        static constexpr uint32_t m_OaCountersCount                  = m_OagCountersCount;
        static constexpr uint32_t m_OaCounter4                       = 4;
        static constexpr uint32_t m_OaCounter23                      = 23;
        static constexpr uint32_t m_OaCounter28                      = 28;
        static constexpr uint32_t m_OaCounter31                      = 31;
        static constexpr uint32_t m_OaCounter35                      = 35;
        static constexpr uint32_t m_OaCounter36                      = 36;
        static constexpr uint32_t m_OaCounter37                      = 37;
        static constexpr uint32_t m_ReportGpuInfoAlignment           = Base::m_ReportGpuAlignment * 2;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Command streamer constants.
        //////////////////////////////////////////////////////////////////////////
        static constexpr uint32_t m_CommandStreamerIdentificatorRender   = 0xFF000000;
        static constexpr uint32_t m_CommandStreamerIdentificatorCompute0 = 0xFFFF0000;
        static constexpr uint32_t m_CommandStreamerIdentificatorCompute1 = 0xFFFF0001;
        static constexpr uint32_t m_CommandStreamerIdentificatorCompute2 = 0xFFFF0002;
        static constexpr uint32_t m_CommandStreamerIdentificatorCompute3 = 0xFFFF0003;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @param  commandStreamerIdentificator identificator of command streamer.
        /// @param  context                      context.
        /// @return                              trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetCommandStreamerDescription(
            const uint32_t                      commandStreamerIdentificator,
            [[maybe_unused]] const TT::Context& context )
        {
            switch( commandStreamerIdentificator )
            {
                case m_CommandStreamerIdentificatorRender:
                    return "(RCS )";
                case m_CommandStreamerIdentificatorCompute0:
                    return "(CCS0)";
                case m_CommandStreamerIdentificatorCompute1:
                    return "(CCS1)";
                case m_CommandStreamerIdentificatorCompute2:
                    return "(CCS2)";
                case m_CommandStreamerIdentificatorCompute3:
                    return "(CCS3)";
                default:
                    ML_ASSERT_ALWAYS_ADAPTER( context.m_AdapterId );
                    return "";
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa report data.
        //////////////////////////////////////////////////////////////////////////
        struct ReportOaData
        {
            union
            {
                // Oa and oar and oag pre-XeHP.
                struct
                {
                    uint32_t    m_OaCounter[Base::m_OaCountersCount];                       // A0 - A35.
                    uint8_t     m_OaCounterHB[Base::m_OaCounters40bitsCount];               // High bytes of A0 - A31.
                };

                // For XeHP+ platforms oag report format has been changed.
                // High bytes of A0 - A3 and A24 - A27 counters have been replaced by A36 and A37 32 bits counters.
                // There is no need to have 40 bits of A0 - A3 and A24 - A27 counters, because their values and deltas
                // are small enough to have them only 32 bits for stream and query purpose.
                struct
                {
                    uint32_t    m_OaCounter_0_36[m_OagCountersCountFrom0To36];              // A0 - A36.
                    uint8_t     m_OaCounterHB_4_23[m_OagCounters40bitsCountFrom4To23];      // High bytes of A4 - A23.
                    uint32_t    m_OaCounter_37;                                             // A37.
                    uint8_t     m_OaCounterHB_28_31[m_OagCounters40bitsCountFrom28To31];    // High bytes of A28 - A31.
                };
            }; // OaCounters.

            uint32_t            m_NoaCounter[Base::m_NoaCountersCount];
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report id.
        //////////////////////////////////////////////////////////////////////////
        struct ReportId
        {
            union
            {
                uint32_t        m_Value;

                struct
                {
                    uint32_t    m_QueryId            : ML_BITFIELD_RANGE( 0,  11 );
                    uint32_t    m_ContextId          : ML_BITFIELD_RANGE( 12, 31 );
                }; // For MI_RPC.

                struct
                {
                    uint32_t    m_FrequencyUnslice   : ML_BITFIELD_RANGE(  0,  8 );
                    uint32_t    m_FrequencySliceHigh : ML_BITFIELD_RANGE(  9, 10 );
                    uint32_t    m_NoaEvents          : ML_BITFIELD_RANGE( 11, 15 );
                    uint32_t    m_ContextValid       : ML_BITFIELD_BIT  (     16 );
                    uint32_t    m_ThresholdEnable    : ML_BITFIELD_BIT  (     17 );
                    uint32_t    m_StartTriggerEvent  : ML_BITFIELD_BIT  (     18 );
                    uint32_t    m_ReportReason       : ML_BITFIELD_RANGE( 19, 25 );
                    uint32_t    m_SourceId           : ML_BITFIELD_RANGE( 26, 31 );
                }; // For time based sampling.
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report header.
        //////////////////////////////////////////////////////////////////////////
        struct ReportHeader
        {
            ReportId    m_ReportId;
            uint32_t    m_Timestamp;
            uint32_t    m_ContextId;
            uint32_t    m_GpuTicks;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa report.
        //////////////////////////////////////////////////////////////////////////
        struct ReportOa
        {
            ReportHeader    m_Header;
            ReportOaData    m_Data;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Unaligned report.
        //////////////////////////////////////////////////////////////////////////
        struct ReportUnaligned
        {
            ReportHeader    m_Header;
            ReportOaData    m_DataOa;
            ReportUser      m_DataUser; // 16 * 8 + 4 = 132 bytes.
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report alignment.
        //////////////////////////////////////////////////////////////////////////
        static constexpr uint32_t m_ReportAlignment = ( (sizeof ( ReportUnaligned ) % Base::m_ReportGpuAlignment )
            ? ( ( sizeof( ReportUnaligned ) / Base::m_ReportGpuAlignment + 1 ) * Base::m_ReportGpuAlignment )
            : sizeof( ReportUnaligned ) );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for Report object.
        //////////////////////////////////////////////////////////////////////////
        struct Report
        {
            union
            {
                struct
                {
                    ReportOa      m_Oa;   // Oa.
                    ReportUser    m_User; // 16 * 8 + 4 = 132 bytes.
                };

                // OA reports have to be aligned to multiple of 64 bytes.
                uint8_t m_Payload[m_ReportAlignment];
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Hw counters structures related to query.
        //////////////////////////////////////////////////////////////////////////
        struct Query : Base::Query
        {
            using ReportApiFlags = typename Base::Query::ReportApiFlags;

            struct ReportApi
            {
                uint64_t                               m_TotalTime;                     // Total query time in nanoseconds.
                uint64_t                               m_GpuTicks;
                uint64_t                               m_OaCounter[m_OagCountersCount];
                uint64_t                               m_NoaCounter[Base::m_NoaCountersCount];
                uint64_t                               m_BeginTimestamp;
                uint64_t                               m_Reserved1;
                uint64_t                               m_Reserved2;
                TT::Layouts::OaBuffer::ReportReason    m_MiddleQueryEvents;             // All types of events occurring between query begin and query end.
                                                                                        // Single event values are defined in ReportReason struct.
                uint32_t                               m_OverrunOccured;
                uint64_t                               m_MarkerUser;
                uint64_t                               m_MarkerDriver;

                uint64_t                               m_SliceFrequency;
                uint64_t                               m_UnsliceFrequency;
                uint64_t                               m_Reserved3;
                uint64_t                               m_Reserved4;
                uint32_t                               m_SplitOccured;                  // Command buffer split has occurred.
                uint32_t                               m_CoreFrequencyChanged;          // Core frequency has changed.
                uint64_t                               m_CoreFrequency;                 // Core frequency during the query.
                uint32_t                               m_ReportId;
                uint32_t                               m_ReportsCount;

                uint64_t                               m_UserCounter[Base::m_UserCountersCount];
                uint32_t                               m_UserCounterConfigurationId;
                ReportApiFlags                         m_Flags;                         // Report flags are defined in ReportApiFlags.
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Base type for QueryReportGpu object.
            //////////////////////////////////////////////////////////////////////////
            struct ReportGpu
            {
                // Begin/end report.
                Report      m_Begin;
                Report      m_End;

                union
                {
                    struct
                    {
                        // Query end marker.
                        uint64_t    m_EndTag;

                        // Command buffer split indicators.
                        uint32_t    m_DmaFenceIdBegin;
                        uint32_t    m_DmaFenceIdEnd;

                        // Oa buffer data related.
                        TT::Layouts::OaBuffer::Register          m_OaBuffer;
                        TT::Layouts::OaBuffer::TailRegister      m_OaTailPreBegin;
                        TT::Layouts::OaBuffer::TailRegister      m_OaTailPostBegin;
                        TT::Layouts::OaBuffer::TailRegister      m_OaTailPreEnd;
                        TT::Layouts::OaBuffer::TailRegister      m_OaTailPostEnd;

                        // Query id.
                        uint32_t    m_QueryIdBegin;
                        uint32_t    m_QueryIdEnd;

                        // Command streamer identificator.
                        uint32_t    m_CommandStreamerIdentificator;

                        // Frequency change indicators.
                        uint32_t    m_CoreFrequencyBegin;
                        uint32_t    m_CoreFrequencyEnd;

                        // Markers.
                        uint64_t    m_MarkerUser;
                        uint64_t    m_MarkerDriver;
                    };

                    uint8_t     m_Payload[m_ReportGpuInfoAlignment];
                };
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Sanity check.
        //////////////////////////////////////////////////////////////////////////
        ML_STATIC_ASSERT( ( sizeof( typename Query::ReportGpu ) % Base::m_ReportGpuAlignment ) == 0, "Wrong structure size" );
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct HwCountersLayoutsTrait : XE_HP::HwCountersLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( HwCountersLayoutsTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct HwCountersLayoutsTrait : XE_HPG::HwCountersLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( HwCountersLayoutsTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct HwCountersLayoutsTrait : XE_HPG::HwCountersLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( HwCountersLayoutsTrait, XE_HPG );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Inherited report objects.
        //////////////////////////////////////////////////////////////////////////
        using ReportUser = typename Base::ReportUser;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Constants.
        //////////////////////////////////////////////////////////////////////////
        static constexpr uint32_t m_PerformanceEventCountersCount = 64;
        static constexpr uint32_t m_VisaCountersCount             = 16;
        static constexpr uint32_t m_OaReportAlignment             = 576;
        static constexpr uint32_t m_OaVisaReportAlignment         = 640;
        static constexpr uint64_t m_MirpcResetValue               = 0xF1E2D3C4B5A69788;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Type of pec counter.
        //////////////////////////////////////////////////////////////////////////
        enum class PecType : uint32_t
        {
            None = 0x0,
            Oar  = ML_BIT( 0 ),
            Oac  = ML_BIT( 1 ),
            All  = 0xFFFFFFFF
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report id.
        //////////////////////////////////////////////////////////////////////////
        struct ReportId
        {
            union
            {
                uint64_t        m_Value;

                struct
                {
                    uint64_t    m_QueryId            : ML_BITFIELD_RANGE( 0,  11 );
                    uint64_t    m_ContextId          : ML_BITFIELD_RANGE( 12, 31 );
                    uint64_t    m_Reserved           : ML_BITFIELD_RANGE( 32, 63 );
                }; // For MI_RPC.

                struct
                {
                    uint64_t    m_FrequencyUnslice   : ML_BITFIELD_RANGE(  0,  8 );
                    uint64_t    m_Reserved1          : ML_BITFIELD_RANGE(  9, 10 );
                    uint64_t    m_NoaEvents          : ML_BITFIELD_RANGE( 11, 15 );
                    uint64_t    m_ContextValid       : ML_BITFIELD_BIT  (     16 );
                    uint64_t    m_ThresholdEnable    : ML_BITFIELD_BIT  (     17 );
                    uint64_t    m_StartTriggerEvent  : ML_BITFIELD_BIT  (     18 );
                    uint64_t    m_ReportReason       : ML_BITFIELD_RANGE( 19, 25 );
                    uint64_t    m_SourceId           : ML_BITFIELD_RANGE( 26, 31 );
                    uint64_t    m_TileId             : ML_BITFIELD_RANGE( 32, 33 );
                    uint64_t    m_Reserved2          : ML_BITFIELD_RANGE( 34, 35 );
                    uint64_t    m_CcsSelectedId      : ML_BITFIELD_RANGE( 36, 40 );
                    uint64_t    m_Reserved3          : ML_BITFIELD_RANGE( 41, 46 );
                    uint64_t    m_DelayedReport      : ML_BITFIELD_BIT  (     47 );
                    uint64_t    m_Reserved4          : ML_BITFIELD_RANGE( 48, 63 );
                }; // For time based sampling.
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report header.
        //////////////////////////////////////////////////////////////////////////
        struct ReportHeader
        {
            ReportId    m_ReportId;
            uint64_t    m_Timestamp;
            uint64_t    m_ContextId;
            uint64_t    m_GpuTicks;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Pec report data.
        //////////////////////////////////////////////////////////////////////////
        struct ReportPecData
        {
            uint64_t    m_PerformanceEventCounter[m_PerformanceEventCountersCount]; // PEC0 - PEC63.
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Pec visa report data.
        //////////////////////////////////////////////////////////////////////////
        struct ReportPecVisaData
        {
            uint64_t    m_PerformanceEventCounter[m_PerformanceEventCountersCount]; // PEC0 - PEC63.
            uint32_t    m_VisaCounter[m_VisaCountersCount];                         // VISA0 - VISA15.
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa report.
        //////////////////////////////////////////////////////////////////////////
        struct ReportOa
        {
            union
            {
                struct
                {
                    ReportHeader     m_Header;
                    ReportPecData    m_Data;
                };

                uint8_t m_Payload[m_OaReportAlignment];
            };

        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa visa report.
        //////////////////////////////////////////////////////////////////////////
        struct ReportOaVisa
        {
            union
            {
                struct
                {
                    ReportHeader         m_Header;
                    ReportPecVisaData    m_Data;
                };

                uint8_t m_Payload[m_OaVisaReportAlignment];
            };

        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Unaligned report.
        //////////////////////////////////////////////////////////////////////////
        struct ReportUnaligned
        {
            union
            {
                ReportOa        m_OaReport;     // 576 bytes.
                ReportOaVisa    m_OaVisaReport; // 640 bytes.
            };

            ReportUser    m_DataUser; // 16 * 8 + 4 = 132 bytes.
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Report alignment.
        //////////////////////////////////////////////////////////////////////////
        static constexpr uint32_t m_ReportAlignment = ( (sizeof ( ReportUnaligned ) % Base::m_ReportGpuAlignment )
            ? ( ( sizeof( ReportUnaligned ) / Base::m_ReportGpuAlignment + 1 ) * Base::m_ReportGpuAlignment )
            : sizeof( ReportUnaligned ) );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for Report object.
        //////////////////////////////////////////////////////////////////////////
        struct Report
        {
            union
            {
                struct
                {
                    union
                    {
                        ReportOa        m_Oa;     // 576 bytes.
                        ReportOaVisa    m_OaVisa; // 640 bytes.
                    };

                    ReportUser    m_User; // 16 * 8 + 4 = 132 bytes.
                };

                // Pec reports have to be aligned to multiple of 64 bytes.
                uint8_t m_Payload[m_ReportAlignment];
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Hw counters structures related to query.
        //////////////////////////////////////////////////////////////////////////
        struct Query : Base::Query
        {
            using ReportApiFlags = typename Base::Query::ReportApiFlags;

            struct ReportApi
            {
                uint64_t                               m_TotalTime;                     // Total query time in nanoseconds.
                uint64_t                               m_GpuTicks;
                uint64_t                               m_PerformanceEventCounter[m_PerformanceEventCountersCount];
                uint64_t                               m_VisaCounter[m_VisaCountersCount];
                uint64_t                               m_BeginTimestamp;
                uint64_t                               m_Reserved1;
                uint64_t                               m_Reserved2;
                TT::Layouts::OaBuffer::ReportReason    m_MiddleQueryEvents;             // All types of events occurring between query begin and query end.
                                                                                        // Single event values are defined in ReportReason struct.
                uint32_t                               m_OverrunOccured;
                uint64_t                               m_MarkerUser;
                uint64_t                               m_MarkerDriver;

                uint64_t                               m_SliceFrequency;
                uint64_t                               m_UnsliceFrequency;
                uint32_t                               m_SplitOccured;                  // Command buffer split has occurred.
                uint32_t                               m_CoreFrequencyChanged;          // Core frequency has changed.
                uint64_t                               m_CoreFrequency;                 // Core frequency during the query.
                uint32_t                               m_ReportId;
                uint32_t                               m_ReportsCount;

                uint64_t                               m_UserCounter[Base::m_UserCountersCount];
                uint32_t                               m_UserCounterConfigurationId;
                ReportApiFlags                         m_Flags;                         // Report flags are defined in ReportApiFlags.
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Base type for QueryReportGpu object.
            //////////////////////////////////////////////////////////////////////////
            struct ReportGpu
            {
                // Begin/end report.
                Report    m_Begin;
                Report    m_End;

                union
                {
                    struct
                    {
                        // Query end marker.
                        uint64_t    m_EndTag;

                        // Command buffer split indicators.
                        uint32_t    m_DmaFenceIdBegin;
                        uint32_t    m_DmaFenceIdEnd;

                        // Oa buffer data related.
                        TT::Layouts::OaBuffer::Register        m_OaBuffer;
                        TT::Layouts::OaBuffer::TailRegister    m_OaTailPreBegin;
                        TT::Layouts::OaBuffer::TailRegister    m_OaTailPostBegin;
                        TT::Layouts::OaBuffer::TailRegister    m_OaTailPreEnd;
                        TT::Layouts::OaBuffer::TailRegister    m_OaTailPostEnd;

                        // Query id.
                        uint32_t    m_QueryIdBegin;
                        uint32_t    m_QueryIdEnd;

                        // Command streamer identificator.
                        uint32_t    m_CommandStreamerIdentificator;

                        // Frequency change indicators.
                        uint32_t    m_CoreFrequencyBegin;
                        uint32_t    m_CoreFrequencyEnd;

                        // Markers.
                        uint64_t    m_MarkerUser;
                        uint64_t    m_MarkerDriver;
                    };

                    uint8_t    m_Payload[Base::m_ReportGpuInfoAlignment];
                };
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Sanity check.
        //////////////////////////////////////////////////////////////////////////
        ML_STATIC_ASSERT( ( sizeof( typename Query::ReportGpu ) % Base::m_ReportGpuAlignment ) == 0, "Wrong structure size" );
    };
} // namespace ML::XE2_HPG

ML_STRUCTURE_PACK_END();

// clang-format on
