/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_hw_counters_layouts.h

@brief Layout for hw counters.
*/

// clang-format off

#pragma once

ML_STRUCTURE_PACK_BEGIN( 1 );

namespace ML
{
    namespace BASE
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
            static constexpr uint32_t m_OaCountersCount       = 36;
            static constexpr uint32_t m_OaCounters40bitsCount = 32;
            static constexpr uint32_t m_NoaCountersCount      = 16;
            static constexpr uint32_t m_UserCountersCount     = 16;
            static constexpr uint32_t m_ReportGpuAlignment    = 64;

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
                        uint32_t    m_ReportLost                : 1;
                        uint32_t    m_ReportInconsistent        : 1;
                        uint32_t    m_ReportNotReady            : 1;
                        uint32_t    m_ReportContextSwitchLost   : 1;
                        uint32_t    m_ReportWithoutWorkload     : 1;
                        uint32_t    m_Reserved                  : 27;
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
                    ReportApiFlags                         m_Flags;                         // Report flags are defined in ReportApiFlags
                };

                //////////////////////////////////////////////////////////////////////////
                /// @brief Oa report collecting mode.
                //////////////////////////////////////////////////////////////////////////
                enum class ReportCollectingMode : uint32_t
                {
                    ReportPerformanceCounters = 0,
                    StoreRegisterMemoryOar,
                    StoreRegisterMemoryOag,
                    TriggerOag
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
                            TT::Layouts::OaBuffer::Register                 m_OaBuffer;
                            TT::Layouts::OaBuffer::TailRegister             m_OaTailBegin;          // Oa window begin & triggered mode.
                            TT::Layouts::OaBuffer::TailRegister             m_OaTailTriggerEnd;     // Triggered mode only.
                            TT::Layouts::OaBuffer::TailRegister             m_OaTailEnd;            // Oa window end.

                            // Temporary data.
                            uint64_t    m_Scratch;

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
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct HwCountersLayoutsTrait : BASE::HwCountersLayoutsTrait<T>
        {
            ML_DECLARE_TRAIT( HwCountersLayoutsTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct HwCountersLayoutsTrait : GEN9::HwCountersLayoutsTrait<T>
        {
            ML_DECLARE_TRAIT( HwCountersLayoutsTrait, GEN9 );
        };
    } // namespace GEN11

    namespace XE_LP
    {
        template <typename T>
        struct HwCountersLayoutsTrait : GEN11::HwCountersLayoutsTrait<T>
        {
            ML_DECLARE_TRAIT( HwCountersLayoutsTrait, GEN11 );
        };
    } // namespace XE_LP
} // namespace ML

ML_STRUCTURE_PACK_END();

// clang-format on
