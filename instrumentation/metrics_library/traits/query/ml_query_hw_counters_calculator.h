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

@file ml_query_hw_counters_calculator.h

@brief Gets and calculates hw counters from two reports.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for QueryHwCountersCalculatorTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct QueryHwCountersCalculatorTrait
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( QueryHwCountersCalculatorTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( QueryHwCountersCalculatorTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            TT::Queries::HwCounters&                       m_Query;
            const uint32_t                                 m_QuerySlotIndex;
            TT::Queries::HwCounters::Slot&                 m_QuerySlot;
            TT::Layouts::HwCounters::Query::ReportGpu&     m_ReportGpu;
            TT::Layouts::HwCounters::ReportOa&             m_ReportOaBegin;
            TT::Layouts::HwCounters::ReportOa&             m_ReportOaEnd;
            TT::Layouts::HwCounters::Query::ReportApi&     m_ReportApi;
            TT::Layouts::OaBuffer::State&                  m_OaBufferState;
            TT::OaBuffer&                                  m_OaBuffer;
            const TT::Layouts::Configuration::HwContextIds m_HwContextIds;
            const TT::KernelInterface&                     m_Kernel;
            const ConfigurationHandle_1_0&                 m_UserConfiguration;
            const bool                                     m_NullBegin;
            const uint64_t                                 m_GpuTimestampFrequency;

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Hw counters query report constructor.
            /// @param  slot        query pool slot index.
            /// @param  query       reference to hw counters query object.
            /// @param  reportApi   reference to output report api.
            //////////////////////////////////////////////////////////////////////////
            QueryHwCountersCalculatorTrait(
                const uint32_t                             slot,
                TT::Queries::HwCounters&                   query,
                TT::Layouts::HwCounters::Query::ReportApi& reportApi )
                : m_Query( query )
                , m_QuerySlotIndex( slot )
                , m_QuerySlot( query.GetSlotData( slot ) )
                , m_ReportGpu( GetReportGpu() )
                , m_ReportOaBegin( m_ReportGpu.m_Begin.m_Oa )
                , m_ReportOaEnd( m_ReportGpu.m_End.m_Oa )
                , m_ReportApi( reportApi )
                , m_OaBufferState( GetOaBufferState() )
                , m_OaBuffer( query.m_Context.m_OaBuffer )
                , m_HwContextIds( m_Kernel.GetHwContextIds() )
                , m_Kernel( query.m_Context.m_Kernel )
                , m_UserConfiguration( query.m_UserConfiguration )
                , m_NullBegin( m_Kernel.IsNullBeginOverride() )
                , m_GpuTimestampFrequency( m_Kernel.GetGpuTimestampFrequency() )
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "QueryHwCountersCalculatorTrait<Traits>";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns api report obtained from executed hw counters object.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetReportApi()
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Clear output report.
                m_ReportApi = {};

                // Validate gpu report completeness.
                if( !IsReportGpuReady( log.m_Result ) )
                {
                    return log.m_Result;
                }

                // Reset oa buffer state if required (dx12/vulkan).
                if( T::Queries::HwCountersPolicy::GetData::m_ResetOaBufferState )
                {
                    m_OaBufferState.Reset();
                    m_QuerySlot.Reset();
                }

                // Dump MiRpc begin/end reports.
                log.Info( "Mirpc begin (0):", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, 0, FormatFlag::AdjustRight, m_ReportGpu.m_Begin.m_Oa, m_ReportGpu.m_Begin.m_Gp );
                log.Info( "Mirpc end   (0):", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, 0, FormatFlag::AdjustRight, m_ReportGpu.m_End.m_Oa, m_ReportGpu.m_End.m_Gp );

                // Handle each sampling mode:
                switch( m_Query.m_GetDataMode )
                {
                    case T::Layouts::HwCounters::Query::GetDataMode::Multisampled:
                        log.m_Result = GetReportMultisampled( false, m_ReportApi );
                        break;

                    default:
                        log.m_Result = GetReportExtended();
                        break;
                }

                // Marker data.
                m_ReportApi.m_MarkerDriver = m_ReportGpu.m_MarkerDriver;
                m_ReportApi.m_MarkerUser   = m_ReportGpu.m_MarkerUser;

                // Validate gpu report consistency.
                ValidateReportGpuStatus( log.m_Result );

                return log.m_Result;
            }

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief   Prepares hw counters internal report to evaluate counters values.
            /// @return  operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode PrepareReportGpu()
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                switch( m_QuerySlot.m_ReportCollectingMode )
                {
                    case T::Layouts::HwCounters::Query::ReportCollectingMode::ReportPerformanceCounters:
                        break;

                    case T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOar:
                        m_Query.UseSrmOarReport( m_QuerySlotIndex );
                        break;

                    case T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOag:
                        m_Query.UseSrmOagReport( m_QuerySlotIndex );
                        break;

                    case T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOag:
                        ML_FUNCTION_CHECK_ERROR( m_ReportGpu.m_OaTailBegin.Dw0.All.m_Tail != m_ReportGpu.m_OaTailEnd.Dw0.All.m_Tail, StatusCode::ReportLost );
                        ML_FUNCTION_CHECK_ERROR( m_Query.UseTriggeredOaReport( m_QuerySlotIndex, true ), StatusCode::ReportLost );
                        ML_FUNCTION_CHECK_ERROR( m_Query.UseTriggeredOaReport( m_QuerySlotIndex, false ), StatusCode::ReportLost );
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        log.m_Result = StatusCode::IncorrectParameter;
                        break;
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Checks if gpu report is ready to create api report.
            /// @return status    gpu report validation status.
            /// @return           true if gpu report is ready to be processed.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsReportGpuReady( StatusCode& status )
            {
                ML_FUNCTION_LOG( true );

                // Print out gpu report to validate.
                log.Debug( "Begin oa     ", m_ReportGpu.m_Begin.m_Oa );
                log.Debug( "End   oa     ", m_ReportGpu.m_End.m_Oa );
                log.Debug( "Begin fence  ", m_ReportGpu.m_DmaFenceIdBegin );
                log.Debug( "End   fence  ", m_ReportGpu.m_DmaFenceIdEnd );
                log.Debug( "End   id     ", m_ReportGpu.m_EndId );
                log.Debug( "User  marker ", m_ReportGpu.m_MarkerUser );

                // Validate gpu report.
                status = ValidateReportGpu();
                status = ML_SUCCESS( status ) ? PrepareReportGpu() : status;
                status = ML_SUCCESS( status ) ? ValidateReportGpuContexts() : status;

                // Validate gpu report status.
                log.m_Result = ValidateReportGpuStatus( status );

                // Print debug information.
                log.Debug( "Gpu report status   ", status );
                log.Debug( "Gpu report is ready ", log.m_Result );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Validate gpu report status.
            /// @return status   gpu report validation status.
            /// @return          success if no issues occured.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool ValidateReportGpuStatus( StatusCode& status )
            {
                ML_FUNCTION_LOG( true );

                // Api report flags.
                auto reportApiFlags = TT::Layouts::HwCounters::Query::ReportApiFlags{};

                switch( status )
                {
                    case StatusCode::Success:
                        // Gpu report is correct.
                        break;

                    case StatusCode::ReportNotReady:
                        // Gpu report is not ready.
                        // Ask user to repeat request.
                        log.m_Result = false;
                        break;

                    case StatusCode::ReportLost:
                        reportApiFlags.m_ReportLost = 1;                   // Mark report as lost.
                        log.m_Result                = false;               // Do not process this gpu report.
                        status                      = StatusCode::Success; // Returns success to avoid subsequent get data request.
                        break;

                    case StatusCode::ReportInconsistent:
                        reportApiFlags.m_ReportInconsistent = 1;                   // Mark report as inconsistent.
                        log.m_Result                        = false;               // Do not process this gpu report.
                        status                              = StatusCode::Success; // Returns success to avoid subsequent get data request.
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        log.m_Result = false;
                        break;
                }

                // Assign api report flags.
                m_ReportApi.m_Flags = reportApiFlags;

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns if gpu report is ready to return api report to the user.
            /// @return success if api report can be exposed to the user.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode ValidateReportGpu()
            {
                ML_FUNCTION_LOG( StatusCode::Success )

                log.Debug( "Gpu report end tag:" );
                log.Debug( "    obtained", m_ReportGpu.m_EndId );
                log.Debug( "    expected", m_Query.m_EndTag );

                return log.m_Result = ( m_Query.m_EndTag == m_ReportGpu.m_EndId )
                    ? StatusCode::Success
                    : StatusCode::ReportNotReady;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Validates gpu report contexts.
            /// @return true if gpu report contexts is valid.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode ValidateReportGpuContexts() const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Validate contexts.
                const bool isSrmOag      = m_QuerySlot.m_ReportCollectingMode == T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOag;
                const bool validBegin    = m_ReportGpu.m_Begin.m_Oa.m_Header.m_ContextId != 0;
                const bool validEnd      = m_ReportGpu.m_End.m_Oa.m_Header.m_ContextId != 0;
                const bool equalContexts = m_ReportGpu.m_Begin.m_Oa.m_Header.m_ContextId == m_ReportGpu.m_End.m_Oa.m_Header.m_ContextId;
                const bool validContexts = ( validBegin && validEnd && equalContexts ) || isSrmOag; // Compute command streamer always returns context id equals to zero.

                log.Debug( "Valid contexts", validContexts );

                return log.m_Result = validContexts
                    ? StatusCode::Success
                    : StatusCode::ReportNotReady;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns multisampled hw counters report.
            /// @param  isExtendedQuery true if below function is used by extended query.
            /// @return reportApi       reference to report api.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetReportMultisampled(
                const bool                                 isExtendedQuery,
                TT::Layouts::HwCounters::Query::ReportApi& reportApi )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const TT::Layouts::HwCounters::ReportOa* oaBegin   = &m_ReportGpu.m_Begin.m_Oa;
                const TT::Layouts::HwCounters::ReportOa* oaEnd     = &m_ReportGpu.m_End.m_Oa;
                TT::Layouts::HwCounters::ReportId        frequency = {};
                bool                                     overrun   = false;
                TT::Layouts::OaBuffer::ReportReason      events    = {};

                // Clear api report data.
                reportApi = {};

                // Find appropriate begin & end reports.
                do
                {
                    // Revert pOaEnd to MiRpc end.
                    oaEnd = &m_ReportGpu.m_End.m_Oa;

                    // Set appropriate begin & end reports.
                    log.m_Result = GetNextOaReport(
                        oaBegin,
                        oaEnd,
                        frequency.m_Value,
                        overrun,
                        events );

                } while( ML_SUCCESS( log.m_Result ) && !IsValidOaReport( *oaBegin ) && !overrun );

                // Api report id and count and check of the report consistency.
                if( ML_SUCCESS( log.m_Result ) )
                {
                    reportApi.m_ReportsCount = m_QuerySlot.m_ApiReportsCount;
                    reportApi.m_ReportId     = overrun
                        ? m_QuerySlot.m_ApiReportsCount
                        : m_QuerySlot.m_ApiReportIndex++;

                    // Debug information.
                    log.Info( "Report api: id / count", FormatFlag::Decimal, reportApi.m_ReportId, "/", reportApi.m_ReportsCount );

                    // Look for the last query subsample.
                    if( reportApi.m_ReportId == reportApi.m_ReportsCount )
                    {
                        // Verify if there is the report corruption.
                        if( oaEnd != &m_ReportGpu.m_End.m_Oa )
                        {
                            log.Warning( "The report in the query is inconsistent" );
                            log.m_Result = StatusCode::ReportInconsistent;
                        }
                    }
                }

                // Sanity check.
                ML_FUNCTION_CHECK( log.m_Result );

                // Gpu frequencies.
                ML_FUNCTION_CHECK( GetFrequencyGpu( reportApi.m_CoreFrequencyChanged, reportApi.m_CoreFrequency ) );
                ML_FUNCTION_CHECK( GetFrequencySlice( frequency, reportApi ) );

                // Gpu counters.
                TT::Layouts::HwCounters::Report reportBegin = {};
                TT::Layouts::HwCounters::Report reportEnd   = {};

                // Copy Oa reports.
                reportBegin.m_Oa = *oaBegin;
                reportEnd.m_Oa   = *oaEnd;

                CalculateCounters(
                    reportBegin,
                    reportEnd,
                    true,
                    false,
                    reportApi );

                if( !isExtendedQuery )
                {
                    // Copy gp reports.
                    reportBegin.m_Gp = m_ReportGpu.m_Begin.m_Gp;
                    reportEnd.m_Gp   = m_ReportGpu.m_End.m_Gp;

                    log.Info( "report begin (oa/gp)", reportBegin.m_Oa, reportBegin.m_Gp );
                    log.Info( "report end   (oa/gp)", reportEnd.m_Oa, reportEnd.m_Gp );

                    // If there is more than one report in the query (tbs multi samples type only),
                    // gp counters need to be calculated separately using timestamps
                    // from snapshots as the time range.
                    // Only approximated (average) values are available, because gp counters
                    // are not collected in oa buffer
                    reportBegin.m_Oa.m_Header.m_Timestamp = m_ReportGpu.m_Begin.m_Oa.m_Header.m_Timestamp;
                    reportEnd.m_Oa.m_Header.m_Timestamp   = m_ReportGpu.m_End.m_Oa.m_Header.m_Timestamp;

                    CalculateCounters(
                        reportBegin,
                        reportEnd,
                        false,
                        true,
                        reportApi );
                }

                reportApi.m_MiddleQueryEvents = events;
                reportApi.m_OverrunOccured    = overrun;
                reportApi.m_BeginTimestamp    = GetBeginTimestamp( oaBegin->m_Header.m_Timestamp, m_GpuTimestampFrequency );
                reportApi.m_SplitOccured      = m_ReportGpu.m_DmaFenceIdBegin != m_ReportGpu.m_DmaFenceIdEnd;

                // Output logs.
                log.Info( "Report api:", reportApi );
                log.Debug( "Begin dma fence:", m_ReportGpu.m_DmaFenceIdBegin );
                log.Debug( "End dma fence:", m_ReportGpu.m_DmaFenceIdEnd );
                log.Debug( "Events:", events );
                log.Debug( "Overrun:", overrun );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns extended hw counters report.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetReportExtended()
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                TT::Layouts::HwCounters::Query::ReportApi source = {};

                do
                {
                    log.m_Result = GetReportMultisampled( true, source );

                    if( ML_SUCCESS( log.m_Result ) )
                    {
                        if( m_NullBegin )
                        {
                            if( source.m_ReportId == source.m_ReportsCount )
                            {
                                // Use only last counters partial differentials.
                                log.Info( "Null begin override with query extended is used." );
                                AggregateCounters( source, m_ReportApi );
                            }
                        }
                        else
                        {
                            // Aggregate all counters partial differentials
                            AggregateCounters( source, m_ReportApi );
                        }

                        m_ReportApi.m_SplitOccured |= source.m_SplitOccured;
                        m_ReportApi.m_CoreFrequencyChanged |= source.m_CoreFrequencyChanged;
                        m_ReportApi.m_CoreFrequency = source.m_CoreFrequency;
                    }
                } while( ML_SUCCESS( log.m_Result ) && ( source.m_ReportId < source.m_ReportsCount ) );

                if( ML_SUCCESS( log.m_Result ) )
                {
                    // Extended query provides only a single report.
                    m_ReportApi.m_ReportId     = 1;
                    m_ReportApi.m_ReportsCount = 1;

                    AdjustGpCounters( m_ReportGpu.m_Begin.m_Gp, m_ReportGpu.m_End.m_Gp, m_ReportApi );
                    AdjustUserCounters( m_ReportGpu.m_Begin.m_User, m_ReportGpu.m_End.m_User, m_ReportApi );

                    // Debug information.
                    log.Info( "Report api:", m_ReportApi );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns gpu core clock.
            /// @param  status  value obtained from RPSTAT register.
            /// @return         core clock.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetGpuClock( const uint32_t status ) const
            {
                return T::Tools::GetMaskedValue( status, T::GpuRegisters::m_RpstatFrequencyMask ) * 100 / 6;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns gpu frequency.
            /// @return frequencyChanged indicates gpu frequency change between begin/end.
            /// @return frequency        gpu frequency value.
            /// @return                  operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetFrequencyGpu(
                uint32_t& frequencyChanged,
                uint64_t& frequency ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                log.Debug( "Gpu frequency begin/end:", m_ReportGpu.m_CoreFrequencyBegin, m_ReportGpu.m_CoreFrequencyEnd );

                const uint32_t frequencyBegin = GetGpuClock( m_ReportGpu.m_CoreFrequencyBegin );
                const uint32_t frequencyEnd   = GetGpuClock( m_ReportGpu.m_CoreFrequencyEnd );

                frequencyChanged |= ( frequencyBegin != frequencyEnd );
                frequency = frequencyEnd * Constants::Time::m_Megahertz;

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns slice and unslice frequencies.
            /// @param  frequency   gpu frequency data.
            /// @return reportApi   hw counters api report.
            /// @return             success if frequency was evaluated successfully.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetFrequencySlice(
                const TT::Layouts::HwCounters::ReportId&   frequency,
                TT::Layouts::HwCounters::Query::ReportApi& reportApi ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                log.Debug( "Gpu frequency:", frequency.m_Value );

                // Ratio encoding in this field can be decoded using the ratio
                // encoding table that is part of the definition of the
                // RP_FREQ_NORMAL register. => (100 / 6)
                reportApi.m_UnsliceFrequency = frequency.m_FrequencyUnslice * 100 / 6;
                reportApi.m_SliceFrequency   = ( ( frequency.m_FrequencySliceHigh << 7 ) + frequency.m_FrequencySliceLow ) * 100 / 6;

                // Convert MHz to Hz.
                reportApi.m_UnsliceFrequency *= Constants::Time::m_Megahertz;
                reportApi.m_SliceFrequency *= Constants::Time::m_Megahertz;

                log.Debug( "Unslice frequency:", reportApi.m_UnsliceFrequency );
                log.Debug( "Slice frequency:", reportApi.m_SliceFrequency );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Calculates counters delta values between two reports.
            ///         Handles context switches which can occur between reports.
            /// @param  begin           begin hw counters report.
            /// @param  end             end hw counters report.
            /// @param  calculateOa     flag to indicate if oa counters need to be calculated.
            /// @param  calculateGp     flag to indicate if gp counters need to be calculated.
            /// @return reportApi       api report.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void CalculateCounters(
                const TT::Layouts::HwCounters::Report&     begin,
                const TT::Layouts::HwCounters::Report&     end,
                const bool                                 calculateOa,
                const bool                                 calculateGp,
                TT::Layouts::HwCounters::Query::ReportApi& reportApi ) const
            {
                static const TT::Layouts::HwCounters::Report dummy = {};

                if( m_NullBegin )
                {
                    reportApi.m_ReportsCount = 1;
                }

                if( calculateOa )
                {
                    AdjustOaCounters( m_NullBegin ? dummy.m_Oa : begin.m_Oa, end.m_Oa, reportApi );
                }

                if( calculateGp )
                {
                    AdjustGpCounters( m_NullBegin ? dummy.m_Gp : begin.m_Gp, end.m_Gp, reportApi );
                    AdjustUserCounters( m_NullBegin ? dummy.m_User : begin.m_User, end.m_User, reportApi );
                }
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Function used to sum oa and noa counters between two reports.
            /// @param  begin       begin internal hw counters report.
            /// @param  end         end internal hw counters report.
            /// @return reportApi   api report.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void AdjustOaCounters(
                const TT::Layouts::HwCounters::ReportOa&   begin,
                const TT::Layouts::HwCounters::ReportOa&   end,
                TT::Layouts::HwCounters::Query::ReportApi& reportApi ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                uint64_t timestampBegin = begin.m_Header.m_Timestamp;
                uint64_t timestampEnd   = end.m_Header.m_Timestamp;

                // Total time in nanoseconds.
                reportApi.m_TotalTime += T::Tools::CountersDelta( timestampEnd, timestampBegin, 32 ) * m_Kernel.GetGpuTimestampTick();

                // Gpu ticks.
                reportApi.m_GpuTicks = T::Tools::CountersDelta( end.m_Header.m_GpuTicks, begin.m_Header.m_GpuTicks, 32 );

                // Oa counters.
                T::Queries::HwCountersCalculator::OaCountersDelta( begin, end, reportApi );

                // Noa counters.
                for( uint32_t i = 0; i < T::Layouts::HwCounters::m_NoaCountersCount; ++i )
                {
                    reportApi.m_NoaCounter[i] = T::Tools::CountersDelta( end.m_Data.m_NoaCounter[i], begin.m_Data.m_NoaCounter[i], 32 );
                }

                log.Info( "Report oa begin:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogBeginIndex, FormatFlag::AdjustRight, begin );
                log.Info( "Report oa end:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndIndex, FormatFlag::AdjustRight, end );

#if 0 // Debug only.
                if( report.m_GpuTicks != report.m_NoaCounter[1] )
                {
                    log.Warning( "TEST_OA???    -> IF YES: COUNTERS MISMATCH !!!" );
                }

                if( report.m_GpuTicks != report.m_OaCounter[0] )
                {
                    log.Warning( "BUSY_vs_CC??? -> IF YES: COUNTERS MISMATCH !!!");
                }
#endif
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Sums general purpose counters deltas between two reports.
            /// @param  begin       begin gp data in hw report format.
            /// @param  end         end gp data in hw report format.
            /// @return reportApi   report api.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void AdjustGpCounters(
                const TT::Layouts::HwCounters::ReportGp&   begin,
                const TT::Layouts::HwCounters::ReportGp&   end,
                TT::Layouts::HwCounters::Query::ReportApi& reportApi ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                reportApi.m_PerformanceCounter1 = T::Tools::CountersDelta( end.m_Counter1, begin.m_Counter1, 44 ) / reportApi.m_ReportsCount;
                reportApi.m_PerformanceCounter2 = T::Tools::CountersDelta( end.m_Counter2, begin.m_Counter2, 44 ) / reportApi.m_ReportsCount;

                log.Info( "Gp begin:", begin );
                log.Info( "Gp end:", end );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Sums mmio user counters between two reports.
            /// @param  begin       begin mmio user data.
            /// @param  end         end mmio user data.
            /// @return reportApi   report api.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void AdjustUserCounters(
                const TT::Layouts::HwCounters::ReportUser& begin,
                const TT::Layouts::HwCounters::ReportUser& end,
                TT::Layouts::HwCounters::Query::ReportApi& reportApi ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                if( T::Configurations::HwCountersUser::IsValid( m_UserConfiguration ) )
                {
                    const auto&    registers = T::Configurations::HwCountersUser::FromHandle( m_UserConfiguration ).m_UserRegisters;
                    const uint32_t count     = registers.m_RegistersCount;

                    for( uint32_t i = 0; i < count; ++i )
                    {
                        log.Debug( "Begin mmio user counter", i, begin.m_Counter[i] );
                        log.Debug( "End mmio user counter", i, end.m_Counter[i] );

                        reportApi.m_UserCounter[i] = T::Tools::CountersDelta( end.m_Counter[i], begin.m_Counter[i], registers.m_Registers[i].m_Size ) / reportApi.m_ReportsCount;
                    }

                    reportApi.m_UserCounterConfigurationId = registers.m_Handle;
                }
                else
                {
                    reportApi.m_UserCounterConfigurationId = Constants::Configuration::m_InvalidHandle;
                }
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Aggregates all oa counters gathered using extended query.
            /// @param  source      query sub data in api format.
            /// @param  reportApi   aggregated query data in api format.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void AggregateCounters(
                const TT::Layouts::HwCounters::Query::ReportApi& source,
                TT::Layouts::HwCounters::Query::ReportApi&       reportApi ) const
            {
                reportApi.m_GpuTicks += source.m_GpuTicks;
                reportApi.m_TotalTime += source.m_TotalTime;
                reportApi.m_PerformanceCounter1 += source.m_PerformanceCounter1;
                reportApi.m_PerformanceCounter2 += source.m_PerformanceCounter2;
                reportApi.m_OverrunOccured |= source.m_OverrunOccured;

                for( uint32_t i = 0; i < T::Layouts::HwCounters::m_OaCountersCount; ++i )
                {
                    reportApi.m_OaCounter[i] += source.m_OaCounter[i];
                }

                for( uint32_t i = 0; i < T::Layouts::HwCounters::m_NoaCountersCount; ++i )
                {
                    reportApi.m_NoaCounter[i] += source.m_NoaCounter[i];
                }

                for( uint32_t i = 0; i < T::Layouts::HwCounters::m_UserCountersCount; ++i )
                {
                    reportApi.m_UserCounter[i] += source.m_UserCounter[i];
                }

                // Update begin timestamp and slice/unslice frequencies.
                if( source.m_ReportId == 1 )
                {
                    reportApi.m_BeginTimestamp    = source.m_BeginTimestamp;
                    reportApi.m_MiddleQueryEvents = source.m_MiddleQueryEvents;
                    reportApi.m_SliceFrequency    = source.m_SliceFrequency;
                    reportApi.m_UnsliceFrequency  = source.m_UnsliceFrequency;
                }
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Function used to sum oa counters between two reports.
            /// @param  begin       begin internal hw counters report.
            /// @param  end         end internal hw counters report.
            /// @return reportApi   api report.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static void OaCountersDelta(
                const TT::Layouts::HwCounters::ReportOa&   begin,
                const TT::Layouts::HwCounters::ReportOa&   end,
                TT::Layouts::HwCounters::Query::ReportApi& reportApi )
            {
                for( uint32_t i = 0; i < T::Layouts::HwCounters::m_OaCountersCount; ++i )
                {
                    if( i < T::Layouts::HwCounters::m_OaCounters40bitsCount )
                    {
                        const uint64_t counterBegin = static_cast<uint64_t>( begin.m_Data.m_OaCounter[i] ) + ( static_cast<uint64_t>( begin.m_Data.m_OaCounterHB[i] ) << 32 );
                        const uint64_t counterEnd   = static_cast<uint64_t>( end.m_Data.m_OaCounter[i] ) + ( static_cast<uint64_t>( end.m_Data.m_OaCounterHB[i] ) << 32 );

                        reportApi.m_OaCounter[i] = T::Tools::CountersDelta( counterEnd, counterBegin, 40 );
                    }
                    else
                    {
                        reportApi.m_OaCounter[i] = T::Tools::CountersDelta( end.m_Data.m_OaCounter[i], begin.m_Data.m_OaCounter[i], 32 );
                    }
                }
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Calculates gpu timestamp gathered by query begin.
            /// @param  timestamp               begin gpu timestamp.
            /// @param  gpuTimestampFrequency   gpu timestamp frequency in Hz.
            /// @return                         timestamp in nanoseconds.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static uint64_t GetBeginTimestamp(
                const uint32_t timestamp,
                const uint64_t gpuTimestampFrequency )
            {
                return static_cast<uint64_t>( timestamp ) * Constants::Time::m_SecondInNanoseconds / gpuTimestampFrequency;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Validates if oa report taken from oa buffer meets all
            ///         requirements to be taken into consideration.
            /// @param  oaReport        oa report from oa buffer to compare.
            /// @return                 true if oa report is valid.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsValidOaReport( const TT::Layouts::HwCounters::ReportOa& reportOa )
            {
                ML_FUNCTION_LOG( true );

                // The mirpc begin report is always valid, in other cases check that the buffer report meets all conditions.
                if( &reportOa != &m_ReportOaBegin )
                {
                    const auto reason = static_cast<uint32_t>( reportOa.m_Header.m_ReportId.m_ReportReason );

                    // 1. Check allowed contexts.
                    log.m_Result = reportOa.m_Header.m_ReportId.m_ContextValid && IsValidContextId( reportOa.m_Header.m_ContextId );

                    // 2. Now check the report reason:
                    if( reason & static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::C6 ) )
                    {
                        // In C6, some configuration registers are reset,
                        // so it is assumed that the report is always invalid.
                        log.Info( "Rc6 report found, oa config is invalid" );
                        m_OaBufferState.m_Valid = false;
                    }
                    else if( reason & static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::ConfigurationUpdate ) )
                    {
                        // The configuring report is always valid.
                        log.Info( "Configuring report found, oa config is valid" );
                        m_OaBufferState.m_Valid = true;
                    }
                    // Other report reasons do not affect the state of the m_Valid.

                    // The report is only valid if the context is compatible and the oa configuration is valid.
                    log.m_Result = log.m_Result && m_OaBufferState.m_Valid;
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Checks if given hw context id belongs to allowed contexts.
            /// @param  contextId       hw context id to check.
            /// @return                 true if hw context should be taken into account.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsValidContextId( const uint32_t contextId ) const
            {
                // Check matching with the contextId used in mirpc begin report.
                bool valid = contextId == m_ReportOaBegin.m_Header.m_ContextId;

                // Check alternate valid contextIds if needed.
                if( !valid )
                {
                    for( uint32_t i = 0; i < m_HwContextIds.m_Count; ++i )
                    {
                        // Used to get pure hw context id, only 20 lsb bits
                        // in context id are used for that purpose.
                        TT::Layouts::HwCounters::ReportId reportId = {};
                        reportId.m_ContextId                       = contextId;

                        if( reportId.m_ContextId == m_HwContextIds.m_Context[i].m_Id )
                        {
                            valid = true;
                            break;
                        }
                    }
                }

                return valid;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Retrieves oa report from oa buffer for hw counters.
            /// @param  reportBegin oa report begin, initially from mirpc.
            /// @param  reportEnd   oa report end, initially from mirpc.
            /// @return frequency   gpu frequency value.
            /// @return overrun     oa buffer has overwritten reports between begin/end.
            /// @return events      all types of events occurring between query begin/end.
            /// @return             success if oa report has been found.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetNextOaReport(
                const TT::Layouts::HwCounters::ReportOa*& reportBegin,
                const TT::Layouts::HwCounters::ReportOa*& reportEnd,
                uint32_t&                                 frequency,
                bool&                                     overrun,
                TT::Layouts::OaBuffer::ReportReason&      events )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Validate api report index.
                if( m_QuerySlot.m_ApiReportIndex > m_QuerySlot.m_ApiReportsCount )
                {
                    // Repeated call, start from the beginning.
                    m_QuerySlot.m_ApiReportIndex = 1;
                }

                // First call to obtain Oa report.
                if( m_OaBufferState.m_CurrentIndex == Constants::OaBuffer::m_InvalidIndex )
                {
                    ML_FUNCTION_CHECK( GetOaInit( reportBegin, reportEnd, frequency, events ) );
                }

                // Obtain begin/end Oa report.
                ML_FUNCTION_CHECK( GetOaBeginReport( reportBegin, frequency ) );
                ML_FUNCTION_CHECK( GetOaEndReport( reportEnd, overrun ) );
                ML_FUNCTION_CHECK( GetOaNext() );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Initialize oa buffer state that will be used to look for oa reports.
            /// @param  reportBegin oa report begin, initially from mirpc.
            /// @param  reportEnd   oa report end, initially from mirpc.
            /// @return frequency   gpu frequency value.
            /// @return events      all types of events occurring between query begin/end.
            /// @return             success if oa report has been found.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetOaInit(
                const TT::Layouts::HwCounters::ReportOa*& reportBegin,
                const TT::Layouts::HwCounters::ReportOa*& reportEnd,
                uint32_t&                                 frequency,
                TT::Layouts::OaBuffer::ReportReason&      events )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_ASSERT( m_OaBufferState.m_CurrentIndex == Constants::OaBuffer::m_InvalidIndex );

                // Reset oa buffer and query slot state.
                m_OaBufferState.Reset();
                m_QuerySlot.Reset();

                // Initialize oa buffer state.
                if( m_OaBuffer.InitializeState( m_ReportGpu, m_OaBufferState ) == false )
                {
                    ML_ASSERT_ALWAYS();
                    return log.m_Result = StatusCode::NotInitialized;
                }

                // Check if oa buffer contains reports.
                if( m_OaBuffer.IsEmpty() )
                {
                    log.Warning( "Oa buffer is empty." );
                }
                else
                {
                    // Query reports.
                    const uint32_t timestampBegin = reportBegin->m_Header.m_Timestamp;
                    const uint32_t timestampEnd   = reportEnd->m_Header.m_Timestamp;

                    // Oa reports.
                    const uint32_t oaReportsCount = m_OaBuffer.FindOaWindow( m_ReportGpu, m_OaBufferState );

                    for( uint32_t i = 0; i < oaReportsCount; ++i )
                    {
                        const uint32_t                           oaReportIndex = ( m_OaBufferState.m_TailBeginIndex + i ) % m_OaBuffer.GetReportsCount();
                        const TT::Layouts::HwCounters::ReportOa& oaReport      = m_OaBuffer.GetReport( oaReportIndex );

                        if( T::Tools::Compare32( oaReport.m_Header.m_Timestamp, timestampBegin ) <= 0 )
                        {
                            frequency = oaReport.m_Header.m_ReportId.m_Value;
                            log.Info( "oaReport (skip begin):", FormatFlag::Decimal, oaReportIndex, oaReport );
                        }
                        else if( T::Tools::Compare32( oaReport.m_Header.m_Timestamp, timestampEnd ) >= 0 )
                        {
                            // All next reports in the oa buffer are too new, so ignore subsequent reports.
                            frequency = oaReport.m_Header.m_ReportId.m_Value;
                            log.Info( "oaReport (skip end):", FormatFlag::Decimal, oaReportIndex, oaReport );
                            break;
                        }
                        else
                        {
                            // Collect events related to report reason.
                            const uint32_t middleQueryEvents = static_cast<uint32_t>( events ) | static_cast<uint32_t>( oaReport.m_Header.m_ReportId.m_ReportReason );
                            events                           = static_cast<TT::Layouts::OaBuffer::ReportReason>( middleQueryEvents );

                            // Mark the first found report as the first report in the Oa window.
                            if( m_OaBufferState.m_FirstIndex == Constants::OaBuffer::m_InvalidIndex )
                            {
                                m_OaBufferState.m_FirstIndex = oaReportIndex;
                            }

                            // Calculate the next after the last report from the window.
                            m_OaBufferState.m_NextAfterLastIndex = ( oaReportIndex + 1 ) % m_OaBuffer.GetReportsCount();

                            // Increment api reports count only for valid reports.
                            if( IsValidOaReport( oaReport ) )
                            {
                                m_QuerySlot.m_ApiReportsCount++;
                                log.Info( "oaReport (valid):", FormatFlag::Decimal, oaReportIndex, oaReport );
                            }
                            else
                            {
                                log.Info( "oaReport (tm-ok):", FormatFlag::Decimal, oaReportIndex, oaReport );
                            }
                        }
                    }

                    // Set the first report.
                    m_OaBufferState.m_CurrentIndex = m_OaBufferState.m_FirstIndex;
                    m_OaBufferState.m_Valid        = true;
                }

                log.Info( "m_OaBufferState.m_FirstIndex:", FormatFlag::Decimal, FormatFlag::ShowBase, m_OaBufferState.m_FirstIndex );
                log.Info( "m_OaBufferState.m_NextAfterLastIndex:", FormatFlag::Decimal, FormatFlag::ShowBase, m_OaBufferState.m_NextAfterLastIndex );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Retrieves begin oa report for hw counters.
            /// @return reportBegin  oa report begin, initially from mirpc.
            /// @return frequency    gpu frequency.
            /// @return              success if begin oa report has been found.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetOaBeginReport(
                const TT::Layouts::HwCounters::ReportOa*& reportBegin,
                uint32_t&                                 frequency )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                if( m_OaBufferState.m_CurrentIndex == m_OaBufferState.m_FirstIndex )
                {
                    // This is the first mirpc report, use data from mirpc report:
                    reportBegin                     = &m_ReportOaBegin;
                    m_OaBufferState.m_LogBeginIndex = 0;
                    log.Debug( "Mirpc begin:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndIndex, FormatFlag::AdjustRight, reportBegin );
                }
                else
                {
                    // Set to the previous end report.
                    reportBegin = &m_OaBufferState.m_ReportCopy[m_OaBufferState.m_ReportCopyIndex];
                    frequency   = reportBegin->m_Header.m_ReportId.m_Value;

                    m_OaBufferState.m_LogBeginIndex = m_OaBufferState.m_CurrentIndex != m_OaBufferState.m_FirstIndex
                        ? m_OaBufferState.m_CurrentIndex - 1
                        : m_OaBufferState.m_NextAfterLastIndex - 1;
                    log.Debug( "Oa begin:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndIndex, FormatFlag::AdjustRight, reportBegin );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Retrieves end oa report for hw counters.
            /// @return reportEnd   oa end report, initially from mirpc.
            /// @return overrun     true if oa buffer has overwritten reports between begin/end.
            /// @return             success if end oa report has been found.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetOaEndReport(
                const TT::Layouts::HwCounters::ReportOa*& reportEnd,
                bool&                                     overrun )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const uint32_t oaReportsCount = m_OaBuffer.GetReportsCount();

                if( !oaReportsCount )
                {
                    log.Warning( "Empty oa buffer." );
                    return log.m_Result;
                }

                if( m_OaBufferState.m_CurrentIndex != m_OaBufferState.m_NextAfterLastIndex )
                {
                    const TT::Layouts::HwCounters::ReportOa& oaReport = m_OaBuffer.GetReport( m_OaBufferState.m_CurrentIndex % oaReportsCount );

                    // Switch Oa report copy.
                    m_OaBufferState.m_ReportCopyIndex = ( ++m_OaBufferState.m_ReportCopyIndex ) % 2;

                    // Copy oa report from oa Buffer.
                    m_OaBufferState.m_ReportCopy[m_OaBufferState.m_ReportCopyIndex] = oaReport;

                    // Get timestamp from the mirpc end report.
                    const uint32_t timestampEnd    = m_ReportOaEnd.m_Header.m_Timestamp;
                    const uint32_t timestampReport = oaReport.m_Header.m_Timestamp;
                    const uint32_t timestampCopy   = m_OaBufferState.m_ReportCopy[m_OaBufferState.m_ReportCopyIndex].m_Header.m_Timestamp;

                    // Check overrun condition.
                    overrun = ( T::Tools::Compare32( timestampReport, timestampEnd ) >= 0 ) || ( timestampReport != timestampCopy );

                    if( overrun )
                    {
                        log.Warning( "Buffer overrun by get next." );
                        reportEnd                     = &m_ReportOaEnd;
                        m_OaBufferState.m_LogEndIndex = 0;
                        log.Debug( "Mirpc end:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndIndex, FormatFlag::AdjustRight, reportEnd );

                        m_OaBufferState.m_CurrentIndex = Constants::OaBuffer::m_InvalidIndex;
                        m_QuerySlot.m_ApiReportIndex   = m_QuerySlot.m_ApiReportsCount;
                    }
                    else
                    {
                        reportEnd                     = &m_OaBufferState.m_ReportCopy[m_OaBufferState.m_ReportCopyIndex];
                        m_OaBufferState.m_LogEndIndex = m_OaBufferState.m_CurrentIndex;
                        log.Debug( "Oa end:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndIndex, FormatFlag::AdjustRight, reportEnd );
                    }
                }
                else
                {
                    // Otherwise use data from mirpc report.
                    // This is the last api report.
                    reportEnd                     = &m_ReportOaEnd;
                    m_OaBufferState.m_LogEndIndex = 0;
                    log.Debug( "Mirpc end", FormatFlag::Decimal, m_OaBufferState.m_LogEndIndex, reportEnd );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Controls the next iteration over oa reports.
            /// @return function success.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetOaNext()
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const uint32_t oaReportsCount = m_OaBuffer.GetReportsCount();

                if( !oaReportsCount )
                {
                    log.Warning( "Empty oa buffer." );
                    return log.m_Result;
                }

                if( m_OaBufferState.m_CurrentIndex != Constants::OaBuffer::m_InvalidIndex )
                {
                    // Move m_CurrentIndex pointer to the next report.
                    if( m_OaBufferState.m_CurrentIndex == m_OaBufferState.m_NextAfterLastIndex )
                    {
                        // Last report was already read, so jump to the first report.
                        m_OaBufferState.m_CurrentIndex = m_OaBufferState.m_FirstIndex;
                        m_OaBufferState.m_Valid        = true;
                    }
                    else
                    {
                        m_OaBufferState.m_CurrentIndex = ( m_OaBufferState.m_CurrentIndex + 1 ) % oaReportsCount;
                    }
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns internal hw counters gpu report for a given slot.
            /// @return reference to internal hw counters layout data.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE TT::Layouts::HwCounters::Query::ReportGpu& GetReportGpu() const
            {
                ML_ASSERT( m_QuerySlot.m_GpuMemory.CpuAddress );

                return *static_cast<TT::Layouts::HwCounters::Query::ReportGpu*>( m_QuerySlot.m_GpuMemory.CpuAddress );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa buffer state for a given layout slot.
            /// @return reference to oa buffer state.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE TT::Layouts::OaBuffer::State& GetOaBufferState() const
            {
                return m_QuerySlot.m_OaBufferState;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Gen9 type for QueryHwCountersCalculatorTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct QueryHwCountersCalculatorTrait : BASE::QueryHwCountersCalculatorTrait<T>
        {
            ML_DECLARE_TRAIT( QueryHwCountersCalculatorTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct QueryHwCountersCalculatorTrait : GEN9::QueryHwCountersCalculatorTrait<T>
        {
            ML_DECLARE_TRAIT( QueryHwCountersCalculatorTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct QueryHwCountersCalculatorTrait : GEN11::QueryHwCountersCalculatorTrait<T>
        {
            ML_DECLARE_TRAIT( QueryHwCountersCalculatorTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
