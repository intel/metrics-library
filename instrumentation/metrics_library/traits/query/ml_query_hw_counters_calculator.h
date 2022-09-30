/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_query_hw_counters_calculator.h

@brief Gets and calculates hw counters from two reports.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for QueryHwCountersCalculatorTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct QueryHwCountersCalculatorTrait : TraitObject<T, TT::Queries::HwCountersCalculator>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( QueryHwCountersCalculatorTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( QueryHwCountersCalculatorTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = TraitObject<T, TT::Queries::HwCountersCalculator>;
        using Base::Derived;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::Queries::HwCounters&                       m_Query;
        TT::Context&                                   m_Context;
        const uint32_t                                 m_QuerySlotIndex;
        TT::Queries::HwCounters::Slot&                 m_QuerySlot;
        TT::Layouts::HwCounters::Query::ReportGpu&     m_ReportGpu;
        TT::Layouts::HwCounters::Report&               m_ReportBegin;
        TT::Layouts::HwCounters::Report&               m_ReportEnd;
        TT::Layouts::HwCounters::Query::ReportApi&     m_ReportApi;
        TT::Layouts::OaBuffer::State&                  m_OaBufferState;
        TT::OaBuffer&                                  m_OaBuffer;
        TT::KernelInterface&                           m_Kernel;
        const TT::Layouts::Configuration::HwContextIds m_HwContextIds;
        const ConfigurationHandle_1_0&                 m_UserConfiguration;
        const bool                                     m_NullBegin;
        TT::Layouts::Configuration::TimestampType      m_TimestampType;
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
            : Base()
            , m_Query( query )
            , m_Context( query.m_Context )
            , m_QuerySlotIndex( slot )
            , m_QuerySlot( query.GetSlotData( slot ) )
            , m_ReportGpu( m_QuerySlot.GetReportGpu() )
            , m_ReportBegin( m_ReportGpu.m_Begin )
            , m_ReportEnd( m_ReportGpu.m_End )
            , m_ReportApi( reportApi )
            , m_OaBufferState( GetOaBufferState() )
            , m_OaBuffer( m_Context.m_OaBuffer )
            , m_Kernel( m_Context.m_Kernel )
            , m_HwContextIds( m_Kernel.GetHwContextIds() )
            , m_UserConfiguration( query.m_UserConfiguration )
            , m_NullBegin( m_Kernel.IsNullBeginOverride() )
            , m_TimestampType( m_QuerySlot.m_ReportCollectingMode == T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOag ? T::Layouts::Configuration::TimestampType::Oa : T::Layouts::Configuration::TimestampType::Cs )
            , m_GpuTimestampFrequency( m_Kernel.GetGpuTimestampFrequency( m_TimestampType ) )
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
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // Clear output report.
            m_ReportApi             = {};
            auto& derivedCalculator = Derived();

            // Validate gpu report completeness.
            if( !IsReportGpuReady( log.m_Result ) )
            {
                return log.m_Result;
            }

            // Reset oa buffer state if required (dx12/vulkan).
            if constexpr( T::Policy::QueryHwCounters::GetData::m_ResetOaBufferState )
            {
                m_OaBufferState.Reset();
                m_QuerySlot.Reset();
            }

            // Handle each sampling mode:
            switch( m_Query.m_GetDataMode )
            {
                case T::Layouts::HwCounters::Query::GetDataMode::Multisampled:
                    log.m_Result = derivedCalculator.GetReportMultisampled( false, m_ReportApi );
                    break;

                default:
                    log.m_Result = derivedCalculator.GetReportExtended();
                    break;
            }

            // Marker data.
            m_ReportApi.m_MarkerDriver = m_ReportGpu.m_MarkerDriver;
            m_ReportApi.m_MarkerUser   = m_ReportGpu.m_MarkerUser;

            // Print output api report.
            log.Info( "Report api:", m_ReportApi );

            return log.m_Result;
        }

    protected:
        //////////////////////////////////////////////////////////////////////////
        /// @brief   Prepares hw counters internal report to evaluate counters values.
        /// @return  operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode PrepareReportGpu()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

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
                    ML_FUNCTION_CHECK_ERROR( m_ReportGpu.m_OaTailPreBegin.All.m_Tail != m_ReportGpu.m_OaTailPostEnd.All.m_Tail, StatusCode::ReportLost );
                    ML_FUNCTION_CHECK_ERROR( m_Query.GetTriggeredOaReports( m_QuerySlotIndex ), StatusCode::ReportLost );
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
            ML_FUNCTION_LOG( true, &m_Context );

            auto& derived = Derived();

            // Check if gpu report is ready.
            status = derived.ValidateReportGpu();

            // Show what has been collected.
            status = ML_SUCCESS( status ) ? derived.PrintReportGpu() : status;

            // Validate gpu report.
            status = ML_SUCCESS( status ) ? PrepareReportGpu() : status;
            status = ML_SUCCESS( status ) ? derived.ValidateReportGpuContexts() : status;
            status = ML_SUCCESS( status ) ? ValidateReportGpuWorkload() : status;

            // Validate gpu report status.
            log.m_Result = ValidateReportGpuStatus( status );

            // Print report gpu status.
            log.Debug( "Status", status );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validate gpu report status.
        /// @return status   gpu report validation status.
        /// @return          success if no issues occurred.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateReportGpuStatus( StatusCode& status )
        {
            ML_FUNCTION_LOG( true, &m_Context );

            // Api report flags.
            auto& flags = m_ReportApi.m_Flags;
            flags       = TT::Layouts::HwCounters::Query::ReportApiFlags{};

            flags.m_ReportContextSwitchLost |= !m_OaBuffer.IsValid();

            switch( status )
            {
                case StatusCode::Success:
                    // Gpu report is correct.
                    break;

                case StatusCode::ReportNotReady:
                    // Gpu report is not ready.
                    // Ask user to repeat request.
                    flags.m_ReportNotReady = 1;
                    log.m_Result           = false;
                    break;

                case StatusCode::ReportLost:
                    flags.m_ReportLost = 1;                   // Mark report as lost.
                    log.m_Result       = false;               // Do not process this gpu report.
                    status             = StatusCode::Success; // Returns success to avoid subsequent get data request.
                    break;

                case StatusCode::ReportInconsistent:
                    flags.m_ReportInconsistent = 1;                   // Mark report as inconsistent.
                    log.m_Result               = false;               // Do not process this gpu report.
                    status                     = StatusCode::Success; // Returns success to avoid subsequent get data request.
                    break;

                case StatusCode::ReportContextSwitchLost:
                    flags.m_ReportContextSwitchLost = 1;                   // Context switches between begin / end are lost.
                    log.m_Result                    = true;                // Process this gpu report.
                    status                          = StatusCode::Success; // Returns success to avoid subsequent get data request.
                    break;

                case StatusCode::ReportWithoutWorkload:
                    flags.m_ReportWithoutWorkload = 1;                   // Missing workload between query begin / end.
                    log.m_Result                  = true;                // Process this gpu report.
                    status                        = StatusCode::Success; // Returns success to avoid subsequent get data request.
                    break;

                case StatusCode::ContextMismatch:
                    flags.m_ContextMismatch = 1;                   // ContextIds in begin / end are different.
                    log.m_Result            = false;               // Do not process this gpu report.
                    status                  = StatusCode::Success; // Returns success to avoid subsequent get data request.
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    log.m_Result = false;
                    break;
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns if gpu report is ready to return api report to the user.
        /// @return success if api report can be exposed to the user.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode ValidateReportGpu()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const bool validTags = m_Query.m_EndTag == m_ReportGpu.m_EndTag;

            log.Debug( "Valid tags   ", validTags );
            log.Debug( "    obtained ", m_ReportGpu.m_EndTag );
            log.Debug( "    expected ", m_Query.m_EndTag );

            return log.m_Result = validTags
                ? StatusCode::Success
                : StatusCode::ReportNotReady;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates gpu report contexts.
        /// @return true if gpu report contexts is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode ValidateReportGpuContexts()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // Validate contexts.
            // Compute command streamer always returns context id equals to zero.
            // Also Linux may use zero context id.
            const bool useNullContext = Derived().AllowEmptyContextId();
            const bool isSrmOag       = m_QuerySlot.m_ReportCollectingMode == T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOag;
            const bool validBegin     = m_ReportGpu.m_Begin.m_Oa.m_Header.m_ContextId != 0;
            const bool validEnd       = m_ReportGpu.m_End.m_Oa.m_Header.m_ContextId != 0;
            const bool equalContexts  = m_ReportGpu.m_Begin.m_Oa.m_Header.m_ContextId == m_ReportGpu.m_End.m_Oa.m_Header.m_ContextId;
            const bool validContexts  = ( validBegin && validEnd ) || isSrmOag || useNullContext;

            if( !( validContexts && equalContexts ) )
            {
                log.Error(
                    "validContexts =",
                    validContexts,
                    ", equalContexts =",
                    equalContexts,
                    ", begin.contextId = ",
                    FormatFlag::Hexadecimal,
                    FormatFlag::ShowBase,
                    m_ReportGpu.m_Begin.m_Oa.m_Header.m_ContextId,
                    ", end.contextId =",
                    FormatFlag::Hexadecimal,
                    FormatFlag::ShowBase,
                    m_ReportGpu.m_End.m_Oa.m_Header.m_ContextId );

                log.m_Result = StatusCode::ContextMismatch;
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates gpu report workload.
        /// @return true if gpu report workload is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode ValidateReportGpuWorkload()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            log.m_Result = m_QuerySlot.m_WorkloadBegin == m_QuerySlot.m_WorkloadEnd
                ? StatusCode::ReportWithoutWorkload
                : StatusCode::Success;

            if( ML_FAIL( log.m_Result ) )
            {
                log.Warning( "No workload between query begin and query end" );
            }

            return log.m_Result;
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
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const TT::Layouts::HwCounters::ReportOa* oaBegin   = &m_ReportGpu.m_Begin.m_Oa;
            const TT::Layouts::HwCounters::ReportOa* oaEnd     = &m_ReportGpu.m_End.m_Oa;
            TT::Layouts::HwCounters::ReportId        frequency = {};
            bool                                     overrun   = false;
            TT::Layouts::OaBuffer::ReportReason      events    = {};
            auto&                                    derived   = Derived();

            // Clear api report data.
            reportApi = {};

            if( BrowseOaBuffer() )
            {
                // Find appropriate begin & end reports.
                do
                {
                    // Revert pOaEnd to MiRpc end.
                    oaEnd = &m_ReportGpu.m_End.m_Oa;

                    // Set appropriate begin & end reports.
                    log.m_Result = GetNextOaReport(
                        oaBegin,
                        oaEnd,
                        frequency,
                        overrun,
                        events );
                }
                while( ML_SUCCESS( log.m_Result ) && !derived.IsValidOaReport( *oaBegin ) && !overrun );
            }
            else
            {
                log.Debug( "Context switches are not available for the report in the query" );
                log.m_Result = StatusCode::ReportContextSwitchLost;

                // Validate context switches lost.
                ML_FUNCTION_CHECK( ValidateReportGpuStatus( log.m_Result ) );
            }

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

                        // Validate gpu report consistency.
                        ML_FUNCTION_CHECK( ValidateReportGpuStatus( log.m_Result ) );
                    }
                }
            }

            // Gpu frequencies.
            ML_FUNCTION_CHECK( GetFrequencyGpu( reportApi.m_CoreFrequencyChanged, reportApi.m_CoreFrequency ) );
            ML_FUNCTION_CHECK( GetFrequencySlice( frequency, reportApi ) );

            // Gpu counters.
            TT::Layouts::HwCounters::Report reportBegin = {};
            TT::Layouts::HwCounters::Report reportEnd   = {};

            // Copy oa reports.
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

                log.Info( "report begin (oa/gp):", reportBegin.m_Oa, reportBegin.m_Gp );
                log.Info( "report end   (oa/gp):", reportEnd.m_Oa, reportEnd.m_Gp );

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
            reportApi.m_BeginTimestamp    = T::Queries::HwCountersCalculator::GetBeginTimestamp( oaBegin->m_Header.m_Timestamp, m_GpuTimestampFrequency );
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
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            TT::Layouts::HwCounters::Query::ReportApi source            = {};
            auto&                                     calculatorDerived = Derived();

            do
            {
                log.m_Result = calculatorDerived.GetReportMultisampled( true, source );

                if( ML_SUCCESS( log.m_Result ) )
                {
                    if( m_NullBegin )
                    {
                        if( source.m_ReportId == source.m_ReportsCount )
                        {
                            // Use only last counters partial differentials.
                            log.Info( "Null begin override with query extended is used." );
                            calculatorDerived.AggregateCounters( source, m_ReportApi );
                        }
                    }
                    else
                    {
                        // Aggregate all counters partial differentials
                        calculatorDerived.AggregateCounters( source, m_ReportApi );
                    }

                    m_ReportApi.m_SplitOccured |= source.m_SplitOccured;
                    m_ReportApi.m_CoreFrequencyChanged |= source.m_CoreFrequencyChanged;
                    m_ReportApi.m_CoreFrequency = source.m_CoreFrequency;
                }
            }
            while( ML_SUCCESS( log.m_Result ) && ( source.m_ReportId < source.m_ReportsCount ) );

            if( ML_SUCCESS( log.m_Result ) )
            {
                // Extended query provides only a single report.
                m_ReportApi.m_ReportId     = 1;
                m_ReportApi.m_ReportsCount = 1;

                calculatorDerived.AdjustGpCounters( m_ReportGpu.m_Begin.m_Gp, m_ReportGpu.m_End.m_Gp, m_ReportApi );
                AdjustUserCounters( m_ReportGpu.m_Begin.m_User, m_ReportGpu.m_End.m_User, m_ReportApi );
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
        /// @brief  Computes slice frequency.
        /// @param  frequency   gpu frequency value.
        /// @return             slice frequency.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t ComputeSliceFrequency( const TT::Layouts::HwCounters::ReportId frequency ) const
        {
            return ( ( frequency.m_FrequencySliceHigh << 7 ) + frequency.m_FrequencySliceLow ) * 100 / 6;
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
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

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
            const TT::Layouts::HwCounters::ReportId    frequency,
            TT::Layouts::HwCounters::Query::ReportApi& reportApi )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            log.Debug( "Gpu frequency:", frequency.m_Value );

            // Ratio encoding in this field can be decoded using the ratio
            // encoding table that is part of the definition of the
            // RP_FREQ_NORMAL register. => (100 / 6)
            reportApi.m_UnsliceFrequency = frequency.m_FrequencyUnslice * 100 / 6;
            reportApi.m_SliceFrequency   = Derived().ComputeSliceFrequency( frequency );

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
            TT::Layouts::HwCounters::Query::ReportApi& reportApi )
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
                Derived().AdjustGpCounters( m_NullBegin ? dummy.m_Gp : begin.m_Gp, end.m_Gp, reportApi );
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
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const uint64_t timestampBegin = begin.m_Header.m_Timestamp;
            const uint64_t timestampEnd   = end.m_Header.m_Timestamp;

            // Total time in nanoseconds.
            reportApi.m_TotalTime += T::Tools::CountersDelta( timestampEnd, timestampBegin, 32 ) * m_Kernel.GetGpuTimestampTick( m_TimestampType );

            // Gpu ticks.
            reportApi.m_GpuTicks = T::Tools::CountersDelta( end.m_Header.m_GpuTicks, begin.m_Header.m_GpuTicks, 32 );

            // Oa counters.
            T::Queries::HwCountersCalculator::OaCountersDelta( begin, end, reportApi );

            // Noa counters.
            T::Queries::HwCountersCalculator::NoaCountersDelta( begin, end, reportApi );

            log.Debug( "Report oa begin:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogBeginOffset, FormatFlag::AdjustRight, begin );
            log.Debug( "Report oa end:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndOffset, FormatFlag::AdjustRight, end );

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
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

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
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

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
            TT::Layouts::HwCounters::Query::ReportApi&       reportApi )
        {
            reportApi.m_GpuTicks += source.m_GpuTicks;
            reportApi.m_TotalTime += source.m_TotalTime;
            reportApi.m_PerformanceCounter1 += source.m_PerformanceCounter1;
            reportApi.m_PerformanceCounter2 += source.m_PerformanceCounter2;
            reportApi.m_OverrunOccured |= source.m_OverrunOccured;

            Derived().AggregateOaCounters( source, reportApi );
            AggregateNoaCounters( source, reportApi );
            AggregateUserCounters( source, reportApi );

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
        /// @brief  Aggregates oa counters gathered using extended query.
        /// @param  source      query sub data in api format.
        /// @param  reportApi   aggregated query data in api format.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void AggregateOaCounters(
            const TT::Layouts::HwCounters::Query::ReportApi& source,
            TT::Layouts::HwCounters::Query::ReportApi&       reportApi ) const
        {
            for( uint32_t i = 0; i < T::Layouts::HwCounters::m_OaCountersCount; ++i )
            {
                reportApi.m_OaCounter[i] += source.m_OaCounter[i];
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Aggregates noa counters gathered using extended query.
        /// @param  source      query sub data in api format.
        /// @param  reportApi   aggregated query data in api format.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void AggregateNoaCounters(
            const TT::Layouts::HwCounters::Query::ReportApi& source,
            TT::Layouts::HwCounters::Query::ReportApi&       reportApi ) const
        {
            for( uint32_t i = 0; i < T::Layouts::HwCounters::m_NoaCountersCount; ++i )
            {
                reportApi.m_NoaCounter[i] += source.m_NoaCounter[i];
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Aggregates noa counters gathered using extended query.
        /// @param  source      query sub data in api format.
        /// @param  reportApi   aggregated query data in api format.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void AggregateUserCounters(
            const TT::Layouts::HwCounters::Query::ReportApi& source,
            TT::Layouts::HwCounters::Query::ReportApi&       reportApi ) const
        {
            for( uint32_t i = 0; i < T::Layouts::HwCounters::m_UserCountersCount; ++i )
            {
                reportApi.m_UserCounter[i] += source.m_UserCounter[i];
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
        /// @brief  Function used to sum noa counters between two reports.
        /// @param  begin       begin internal hw counters report.
        /// @param  end         end internal hw counters report.
        /// @return reportApi   api report.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void NoaCountersDelta(
            const TT::Layouts::HwCounters::ReportOa&   begin,
            const TT::Layouts::HwCounters::ReportOa&   end,
            TT::Layouts::HwCounters::Query::ReportApi& reportApi )
        {
            for( uint32_t i = 0; i < T::Layouts::HwCounters::m_NoaCountersCount; ++i )
            {
                reportApi.m_NoaCounter[i] = T::Tools::CountersDelta( end.m_Data.m_NoaCounter[i], begin.m_Data.m_NoaCounter[i], 32 );
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
            ML_FUNCTION_LOG( true, &m_Context );

            // The begin report is always valid, in other cases check that the buffer report meets all conditions.
            if( &reportOa != &m_ReportBegin.m_Oa )
            {
                const uint32_t reason = reportOa.m_Header.m_ReportId.m_ReportReason;

                // 1. Check allowed contexts.
                log.m_Result = reportOa.m_Header.m_ReportId.m_ContextValid && IsMeasuredContextId( reportOa.m_Header.m_ContextId );

                // 2. Now check the report reason:
                if( reason & static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::C6 ) )
                {
                    // In C6, some configuration registers are reset,
                    // so it is assumed that the report is always invalid.
                    log.Debug( "Rc6 report found, oa config is invalid" );
                    m_OaBufferState.m_ConfigurationValid = false;
                }
                else if( reason & static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::ConfigurationUpdate ) )
                {
                    // The configuring report is always valid.
                    log.Debug( "Configuring report found, oa config is valid" );
                    m_OaBufferState.m_ConfigurationValid = true;
                }
                // Other report reasons do not affect the state of the m_Valid.

                // The report is only valid if the context is compatible and the oa configuration is valid.
                log.m_Result = log.m_Result && m_OaBufferState.m_ConfigurationValid;
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks if given hw context id belongs to measured contexts.
        /// @param  contextId       hw context id to check.
        /// @return                 true if hw context should be taken into account.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsMeasuredContextId( const uint32_t contextId ) const
        {
            // Check matching with the contextId used in begin report.
            return contextId == static_cast<uint32_t>( m_ReportBegin.m_Oa.m_Header.m_ContextId );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Retrieves oa report from oa buffer for hw counters.
        /// @param  reportBegin oa report begin.
        /// @param  reportEnd   oa report end.
        /// @return frequency   gpu frequency value.
        /// @return overrun     oa buffer has overwritten reports between begin/end.
        /// @return events      all types of events occurring between query begin/end.
        /// @return             success if oa report has been found.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetNextOaReport(
            const TT::Layouts::HwCounters::ReportOa*& reportBegin,
            const TT::Layouts::HwCounters::ReportOa*& reportEnd,
            TT::Layouts::HwCounters::ReportId&        frequency,
            bool&                                     overrun,
            TT::Layouts::OaBuffer::ReportReason&      events )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // Validate api report index.
            if( m_QuerySlot.m_ApiReportIndex > m_QuerySlot.m_ApiReportsCount )
            {
                // Repeated call, start from the beginning.
                m_QuerySlot.m_ApiReportIndex = 1;
            }

            // First call to obtain Oa report.
            if( m_OaBufferState.m_CurrentOffset == Constants::OaBuffer::m_InvalidOffset )
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
        /// @param  reportBegin oa report begin.
        /// @param  reportEnd   oa report end.
        /// @return frequency   gpu frequency value.
        /// @return events      all types of events occurring between query begin/end.
        /// @return             success if oa report has been found.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetOaInit(
            const TT::Layouts::HwCounters::ReportOa*& reportBegin,
            const TT::Layouts::HwCounters::ReportOa*& reportEnd,
            TT::Layouts::HwCounters::ReportId&        frequency,
            TT::Layouts::OaBuffer::ReportReason&      events )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_ASSERT( m_OaBufferState.m_CurrentOffset == Constants::OaBuffer::m_InvalidOffset );

            // Reset oa buffer and query slot state.
            m_OaBufferState.Reset();
            m_QuerySlot.Reset();

            // Check if oa buffer contains reports.
            if( !m_OaBuffer.IsValid() )
            {
                log.Warning( "Oa buffer is not available." );
            }
            else
            {
                // Initialize oa buffer state.
                if( ML_FAIL( m_OaBuffer.UpdateQuery( m_QuerySlot ) ) )
                {
                    ML_ASSERT_ALWAYS();
                    return log.m_Result = StatusCode::NotInitialized;
                }

                // Oa reports.
                const uint32_t oaReportsCount = m_OaBuffer.FindOaWindow( m_OaBufferState );

                for( uint32_t i = 0; i < oaReportsCount; ++i )
                {
                    const uint32_t                           oaReportOffset    = ( m_OaBufferState.m_TailBeginOffset + i * sizeof( TT::Layouts::HwCounters::ReportOa ) ) % m_OaBuffer.GetSize();
                    const TT::Layouts::HwCounters::ReportOa& oaReport          = m_OaBuffer.GetReport( oaReportOffset );
                    const auto&                              derivedCalculator = Derived();

                    if( derivedCalculator.CompareTimestamps( oaReport.m_Header.m_Timestamp, reportBegin->m_Header.m_Timestamp ) <= 0 )
                    {
                        frequency = oaReport.m_Header.m_ReportId;
                        log.Info( "oaReport (skip begin): ", "(", FormatFlag::Decimal, oaReportOffset, ")", oaReport );
                    }
                    else if( derivedCalculator.CompareTimestamps( oaReport.m_Header.m_Timestamp, reportEnd->m_Header.m_Timestamp ) >= 0 )
                    {
                        // All next reports in the oa buffer are too new, so ignore subsequent reports.
                        frequency = oaReport.m_Header.m_ReportId;
                        log.Info( "oaReport (skip end):   ", "(", FormatFlag::Decimal, oaReportOffset, ")", oaReport );

                        const uint32_t                           oaReportPostEndOffset = ( m_OaBufferState.m_TailBeginOffset + ( i + 1 ) * sizeof( TT::Layouts::HwCounters::ReportOa ) ) % m_OaBuffer.GetSize();
                        const TT::Layouts::HwCounters::ReportOa& oaReportPostEnd       = m_OaBuffer.GetReport( oaReportPostEndOffset );
                        log.Debug( "oaReport (post end):   ", "(", FormatFlag::Decimal, oaReportPostEndOffset, ")", oaReportPostEnd );
                        break;
                    }
                    else
                    {
                        // Collect events related to report reason.
                        const uint32_t middleQueryEvents = static_cast<uint32_t>( events ) | static_cast<uint32_t>( oaReport.m_Header.m_ReportId.m_ReportReason );
                        events                           = static_cast<TT::Layouts::OaBuffer::ReportReason>( middleQueryEvents );

                        // Mark the first found report as the first report in the Oa window.
                        if( m_OaBufferState.m_FirstOffset == Constants::OaBuffer::m_InvalidOffset )
                        {
                            m_OaBufferState.m_FirstOffset = oaReportOffset;
                        }

                        // Calculate the next after the last report from the window.
                        m_OaBufferState.m_NextAfterLastOffset = ( oaReportOffset + sizeof( TT::Layouts::HwCounters::ReportOa ) ) % m_OaBuffer.GetSize();

                        // Increment api reports count only for valid reports.
                        if( Derived().IsValidOaReport( oaReport ) )
                        {
                            m_QuerySlot.m_ApiReportsCount++;
                            log.Info( "oaReport (valid):      ", "(", FormatFlag::Decimal, oaReportOffset, ")", oaReport );
                        }
                        else
                        {
                            log.Info( "oaReport (tm-ok):      ", "(", FormatFlag::Decimal, oaReportOffset, ")", oaReport );
                        }
                    }
                }

                // Set the first report.
                m_OaBufferState.m_CurrentOffset      = m_OaBufferState.m_FirstOffset;
                m_OaBufferState.m_ConfigurationValid = true;
                m_OaBufferState.m_ContextValid       = true;
            }

            log.Debug( "m_OaBufferState.m_FirstOffset:", FormatFlag::Decimal, FormatFlag::ShowBase, m_OaBufferState.m_FirstOffset );
            log.Debug( "m_OaBufferState.m_NextAfterLastOffset:", FormatFlag::Decimal, FormatFlag::ShowBase, m_OaBufferState.m_NextAfterLastOffset );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Retrieves begin oa report for hw counters.
        /// @return reportBegin  oa report begin.
        /// @return frequency    gpu frequency.
        /// @return              success if begin oa report has been found.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetOaBeginReport(
            const TT::Layouts::HwCounters::ReportOa*& reportBegin,
            TT::Layouts::HwCounters::ReportId&        frequency )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            if( m_OaBufferState.m_CurrentOffset == m_OaBufferState.m_FirstOffset )
            {
                // This is the first report, use data from begin report:
                reportBegin                      = &m_ReportBegin.m_Oa;
                m_OaBufferState.m_LogBeginOffset = 0;
                log.Debug( "Mirpc begin:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndOffset, FormatFlag::AdjustRight, reportBegin );
            }
            else
            {
                // Set to the previous end report.
                reportBegin = &m_OaBufferState.m_ReportCopy[m_OaBufferState.m_ReportCopyIndex];
                frequency   = reportBegin->m_Header.m_ReportId;

                m_OaBufferState.m_LogBeginOffset = m_OaBufferState.m_CurrentOffset != m_OaBufferState.m_FirstOffset
                    ? m_OaBufferState.m_CurrentOffset - 1
                    : m_OaBufferState.m_NextAfterLastOffset - 1;
                log.Debug( "Oa begin:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndOffset, FormatFlag::AdjustRight, reportBegin );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Retrieves end oa report for hw counters.
        /// @return reportEnd   oa end report.
        /// @return overrun     true if oa buffer has overwritten reports between begin/end.
        /// @return             success if end oa report has been found.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetOaEndReport(
            const TT::Layouts::HwCounters::ReportOa*& reportEnd,
            bool&                                     overrun )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const uint32_t oaBufferSize = m_OaBuffer.GetSize();

            if( !oaBufferSize )
            {
                log.Warning( "Empty oa buffer." );
                return log.m_Result;
            }

            if( m_OaBufferState.m_CurrentOffset != m_OaBufferState.m_NextAfterLastOffset )
            {
                const TT::Layouts::HwCounters::ReportOa& oaReport = m_OaBuffer.GetReport( m_OaBufferState.m_CurrentOffset % oaBufferSize );

                // Switch Oa report copy.
                m_OaBufferState.m_ReportCopyIndex = ( ++m_OaBufferState.m_ReportCopyIndex ) % 2;

                // Copy oa report from oa Buffer.
                m_OaBufferState.m_ReportCopy[m_OaBufferState.m_ReportCopyIndex] = oaReport;

                // Check overrun condition.
                overrun = Derived().IsOverrun( oaReport.m_Header.m_Timestamp );

                if( overrun )
                {
                    log.Warning( "Buffer overrun by get next." );
                    reportEnd                      = &m_ReportEnd.m_Oa;
                    m_OaBufferState.m_LogEndOffset = 0;
                    log.Debug( "Mirpc end:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndOffset, FormatFlag::AdjustRight, reportEnd );

                    m_OaBufferState.m_CurrentOffset = Constants::OaBuffer::m_InvalidOffset;
                    m_QuerySlot.m_ApiReportIndex    = m_QuerySlot.m_ApiReportsCount;
                }
                else
                {
                    reportEnd                      = &m_OaBufferState.m_ReportCopy[m_OaBufferState.m_ReportCopyIndex];
                    m_OaBufferState.m_LogEndOffset = m_OaBufferState.m_CurrentOffset;
                    log.Debug( "Oa end:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndOffset, FormatFlag::AdjustRight, reportEnd );
                }
            }
            else
            {
                // Otherwise use data from end report.
                // This is the last api report.
                reportEnd                      = &m_ReportEnd.m_Oa;
                m_OaBufferState.m_LogEndOffset = 0;
                log.Debug( "Mirpc end:", FormatFlag::SetWidth5, FormatFlag::AdjustLeft, m_OaBufferState.m_LogEndOffset, FormatFlag::AdjustRight, reportEnd );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Controls the next iteration over oa reports.
        /// @return function success.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetOaNext()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const uint32_t oaBufferSize = m_OaBuffer.GetSize();

            if( !oaBufferSize )
            {
                log.Warning( "Empty oa buffer." );
                return log.m_Result;
            }

            if( m_OaBufferState.m_CurrentOffset != Constants::OaBuffer::m_InvalidOffset )
            {
                // Move m_CurrentOffset pointer to the next report.
                if( m_OaBufferState.m_CurrentOffset == m_OaBufferState.m_NextAfterLastOffset )
                {
                    // Last report was already read, so jump to the first report.
                    m_OaBufferState.m_CurrentOffset      = m_OaBufferState.m_FirstOffset;
                    m_OaBufferState.m_ConfigurationValid = true;
                    m_OaBufferState.m_ContextValid       = true;
                }
                else
                {
                    m_OaBufferState.m_CurrentOffset = ( m_OaBufferState.m_CurrentOffset + sizeof( TT::Layouts::HwCounters::ReportOa ) ) % oaBufferSize;
                }
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Compares reports timestamps.
        /// @param  value1  first timestamp.
        /// @param  value2  second timestamp.
        /// @return         -1 if value1 is lesser than value2.
        ///                 1 if value1 is greater than value2.
        ///                 0 if value1 is equal to value2.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE int32_t CompareTimestamps(
            const uint32_t value1,
            const uint32_t value2 ) const
        {
            return T::Tools::Compare32( value1, value2 );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Check timestamp overrun condition.
        /// @param  timestampReport timestamp from current report.
        /// @return true if overrun occurred, false otherwise.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsOverrun( const uint32_t timestampReport ) const
        {
            ML_FUNCTION_LOG( false, &m_Context );

            // Get timestamp from the end report.
            const uint32_t timestampEnd  = m_ReportEnd.m_Oa.m_Header.m_Timestamp;
            const uint32_t timestampCopy = m_OaBufferState.m_ReportCopy[m_OaBufferState.m_ReportCopyIndex].m_Header.m_Timestamp;

            // Check overrun condition.
            return log.m_Result = ( T::Tools::Compare32( timestampReport, timestampEnd ) >= 0 ) || ( timestampReport != timestampCopy );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa buffer state for a given layout slot.
        /// @return reference to oa buffer state.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::OaBuffer::State& GetOaBufferState() const
        {
            return m_QuerySlot.m_OaBufferState;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns true if null context id is valid, false otherwise.
        /// @return true if null context id is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool AllowEmptyContextId() const
        {
            return T::Policy::QueryHwCounters::GetData::m_AllowEmptyContextId;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns command buffer type.
        /// @return render command buffer as long as other types are not used.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE GpuCommandBufferType GetCommandBufferType() const
        {
            return GpuCommandBufferType::Render;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns true if oa buffer should be browsed.
        /// @return true if oa buffer should be browsed to find context switches.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool BrowseOaBuffer()
        {
            const bool isCommandBufferCompute = Derived().GetCommandBufferType() == GpuCommandBufferType::Compute;

            // There is no need to browse oa buffer for compute command streamers.
            return !isCommandBufferCompute && T::Policy::QueryHwCounters::GetData::m_IncludeRenderContextSwitchReports;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Prints platform specific report gpu information.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode PrintReportGpu()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            log.Debug( "Gpu report              " );
            log.Debug( "    m_Begin.m_Oa        ", m_ReportGpu.m_Begin.m_Oa );
            log.Debug( "    m_End.m_Oa          ", m_ReportGpu.m_End.m_Oa );
            log.Debug( "    m_EndTag            ", m_ReportGpu.m_EndTag );
            log.Debug( "    m_DmaFenceIdBegin   ", m_ReportGpu.m_DmaFenceIdBegin );
            log.Debug( "    m_DmaFenceIdEnd     ", m_ReportGpu.m_DmaFenceIdEnd );
            log.Debug( "    m_OaBuffer          ", m_ReportGpu.m_OaBuffer.All.m_ReportBufferOffset );
            log.Debug( "    m_OaTailPreBegin    ", m_ReportGpu.m_OaTailPreBegin.All.m_Tail );
            log.Debug( "    m_OaTailPostBegin   ", m_ReportGpu.m_OaTailPostBegin.All.m_Tail );
            log.Debug( "    m_OaTailPreEnd      ", m_ReportGpu.m_OaTailPreEnd.All.m_Tail );
            log.Debug( "    m_OaTailPostEnd     ", m_ReportGpu.m_OaTailPostEnd.All.m_Tail );
            log.Debug( "    m_CoreFrequencyBegin", m_ReportGpu.m_CoreFrequencyBegin );
            log.Debug( "    m_CoreFrequencyEnd  ", m_ReportGpu.m_CoreFrequencyEnd );
            log.Debug( "    m_MarkerUser        ", m_ReportGpu.m_MarkerUser );
            log.Debug( "    m_MarkerDriver      ", m_ReportGpu.m_MarkerDriver );

            return log.m_Result;
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Gen9 type for QueryHwCountersCalculatorTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct QueryHwCountersCalculatorTrait : BASE::QueryHwCountersCalculatorTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersCalculatorTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct QueryHwCountersCalculatorTrait : GEN9::QueryHwCountersCalculatorTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersCalculatorTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct QueryHwCountersCalculatorTrait : GEN11::QueryHwCountersCalculatorTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersCalculatorTrait, GEN11 );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::Derived;
        using Base::AggregateOaCounters;
        using Base::AggregateNoaCounters;
        using Base::AggregateUserCounters;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Null context id is valid on trigger based query.
        /// @return true if null context id is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool AllowEmptyContextId() const
        {
            return true;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Sums general purpose counters deltas between two reports.
        /// @param  begin       begin gp data in hw report format.
        /// @param  end         end gp data in hw report format.
        /// @return reportApi   report api.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void AdjustGpCounters(
            [[maybe_unused]] const TT::Layouts::HwCounters::ReportGp&   begin,
            [[maybe_unused]] const TT::Layouts::HwCounters::ReportGp&   end,
            [[maybe_unused]] TT::Layouts::HwCounters::Query::ReportApi& reportApi ) const
        {
            // General purpose counters are deprecated on XE+
            return;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Aggregates all oa counters gathered using extended query.
        /// @param  source      query sub data in api format.
        /// @param  reportApi   aggregated query data in api format.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void AggregateCounters(
            const TT::Layouts::HwCounters::Query::ReportApi& source,
            TT::Layouts::HwCounters::Query::ReportApi&       reportApi )
        {
            reportApi.m_GpuTicks += source.m_GpuTicks;
            reportApi.m_TotalTime += source.m_TotalTime;
            reportApi.m_OverrunOccured |= source.m_OverrunOccured;

            AggregateOaCounters( source, reportApi );
            AggregateNoaCounters( source, reportApi );
            AggregateUserCounters( source, reportApi );

            // Update begin timestamp and slice/unslice frequencies.
            if( source.m_ReportId == 1 )
            {
                reportApi.m_BeginTimestamp    = source.m_BeginTimestamp;
                reportApi.m_MiddleQueryEvents = source.m_MiddleQueryEvents;
                reportApi.m_SliceFrequency    = source.m_SliceFrequency;
                reportApi.m_UnsliceFrequency  = source.m_UnsliceFrequency;
            }
        }
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct QueryHwCountersCalculatorTrait : XE_LP::QueryHwCountersCalculatorTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersCalculatorTrait, XE_LP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::Derived;
        using Base::IsMeasuredContextId;
        using Base::m_ReportGpu;
        using Base::m_ReportBegin;
        using Base::m_OaBuffer;
        using Base::m_OaBufferState;
        using Base::m_HwContextIds;
        using Base::m_Query;
        using Base::m_QuerySlot;
        using Base::m_Context;

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
            for( uint32_t i = 0; i < T::Layouts::HwCounters::m_OagCountersCount; ++i )
            {
                // A4 - A23 and A28 - A31 counters are only 40 bits counters.
                if( i >= T::Layouts::HwCounters::m_OaCounter4 && i <= T::Layouts::HwCounters::m_OaCounter23 )
                {
                    const uint32_t highBitsIndex = i - T::Layouts::HwCounters::m_OaCounter4;

                    const uint64_t counterBegin = static_cast<uint64_t>( begin.m_Data.m_OaCounter_0_36[i] ) + ( static_cast<uint64_t>( begin.m_Data.m_OaCounterHB_4_23[highBitsIndex] ) << 32 );
                    const uint64_t counterEnd   = static_cast<uint64_t>( end.m_Data.m_OaCounter_0_36[i] ) + ( static_cast<uint64_t>( end.m_Data.m_OaCounterHB_4_23[highBitsIndex] ) << 32 );

                    reportApi.m_OaCounter[i] = T::Tools::CountersDelta( counterEnd, counterBegin, 40 );
                }
                else if( i >= T::Layouts::HwCounters::m_OaCounter28 && i <= T::Layouts::HwCounters::m_OaCounter31 )
                {
                    const uint32_t highBitsIndex = i - T::Layouts::HwCounters::m_OaCounter28;

                    const uint64_t counterBegin = static_cast<uint64_t>( begin.m_Data.m_OaCounter_0_36[i] ) + ( static_cast<uint64_t>( begin.m_Data.m_OaCounterHB_28_31[highBitsIndex] ) << 32 );
                    const uint64_t counterEnd   = static_cast<uint64_t>( end.m_Data.m_OaCounter_0_36[i] ) + ( static_cast<uint64_t>( end.m_Data.m_OaCounterHB_28_31[highBitsIndex] ) << 32 );

                    reportApi.m_OaCounter[i] = T::Tools::CountersDelta( counterEnd, counterBegin, 40 );
                }
                else if( i <= T::Layouts::HwCounters::m_OaCounter36 )
                {
                    reportApi.m_OaCounter[i] = T::Tools::CountersDelta( end.m_Data.m_OaCounter_0_36[i], begin.m_Data.m_OaCounter_0_36[i], 32 );
                }
                else
                {
                    reportApi.m_OaCounter[i] = T::Tools::CountersDelta( end.m_Data.m_OaCounter_37, begin.m_Data.m_OaCounter_37, 32 );
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Computes slice frequency.
        /// @param  frequency   gpu frequency value.
        /// @return             slice frequency.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t ComputeSliceFrequency( const TT::Layouts::HwCounters::ReportId frequency ) const
        {
            // Slice frequency is the same as unslice frequency.
            return frequency.m_FrequencyUnslice * 100 / 6;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns command buffer type.
        /// @return command buffer type obtained from query report.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE GpuCommandBufferType GetCommandBufferType() const
        {
            ML_FUNCTION_LOG( GpuCommandBufferType::Render, &m_Context );

            switch( m_ReportGpu.m_CommandStreamerIdentificator )
            {
                case T::Layouts::HwCounters::m_CommandStreamerIdentificatorRender:
                    return log.m_Result = GpuCommandBufferType::Render;

                case T::Layouts::HwCounters::m_CommandStreamerIdentificatorCompute0:
                case T::Layouts::HwCounters::m_CommandStreamerIdentificatorCompute1:
                case T::Layouts::HwCounters::m_CommandStreamerIdentificatorCompute2:
                case T::Layouts::HwCounters::m_CommandStreamerIdentificatorCompute3:
                    return log.m_Result = GpuCommandBufferType::Compute;

                default:
                    ML_ASSERT_ALWAYS();
                    return log.m_Result = GpuCommandBufferType::Render;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates gpu report contexts.
        /// @return true if gpu report contexts is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode ValidateReportGpuContexts()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // Validate contexts (rcs only).
            if( m_ReportGpu.m_CommandStreamerIdentificator == T::Layouts::HwCounters::m_CommandStreamerIdentificatorRender )
            {
                const bool useNullContext = Derived().AllowEmptyContextId();
                const bool isSrmOag       = m_QuerySlot.m_ReportCollectingMode == T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOag;
                const bool validBegin     = m_ReportGpu.m_Begin.m_Oa.m_Header.m_ContextId != 0;
                const bool validEnd       = m_ReportGpu.m_End.m_Oa.m_Header.m_ContextId != 0;
                const bool equalContexts  = m_ReportGpu.m_Begin.m_Oa.m_Header.m_ContextId == m_ReportGpu.m_End.m_Oa.m_Header.m_ContextId;
                const bool validContexts  = ( validBegin && validEnd ) || isSrmOag || useNullContext;

                if( !( validContexts && equalContexts ) )
                {
                    log.Error(
                        "validContexts =",
                        validContexts,
                        ", equalContexts = ",
                        equalContexts,
                        ", begin.contextId = ",
                        FormatFlag::Hexadecimal,
                        FormatFlag::ShowBase,
                        m_ReportGpu.m_Begin.m_Oa.m_Header.m_ContextId,
                        ", end.contextId =",
                        FormatFlag::Hexadecimal,
                        FormatFlag::ShowBase,
                        m_ReportGpu.m_End.m_Oa.m_Header.m_ContextId );

                    log.m_Result = StatusCode::ContextMismatch;
                }
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates if oa report taken from oa buffer meets all
        ///         requirements to be taken into consideration.
        /// @param  oaReport        oa report from oa buffer to compare.
        /// @return                 true if oa report is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsValidOaReport( const TT::Layouts::HwCounters::ReportOa& reportOa )
        {
            ML_FUNCTION_LOG( true, &m_Context );

            // The begin report is always valid, in other cases check that the buffer report meets all conditions.
            if( &reportOa != &m_ReportBegin.m_Oa )
            {
                const uint32_t reason = reportOa.m_Header.m_ReportId.m_ReportReason;

                // 1. Check allowed contexts.
                // only reports with the ContextValid flag set are considered
                if( reason & static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::ContextSwitch ) )
                {
                    if( IsMeasuredContextId( static_cast<uint32_t>( reportOa.m_Header.m_ContextId ) ) )
                    {
                        if( reportOa.m_Header.m_ReportId.m_ContextValid )
                        {
                            log.Debug( "Valid context report found" );
                            m_OaBufferState.m_ContextValid = true;
                        }
                        else
                        {
                            log.Debug( "Invalid context report found" );
                            m_OaBufferState.m_ContextValid = false;
                        }
                    }
                    else
                    {
                        if( IsRcsContextId( static_cast<uint32_t>( reportOa.m_Header.m_ContextId ) ) )
                        {
                            ML_ASSERT( !m_OaBufferState.m_ContextValid );
                        }
                    }
                }

                // 2. Now check the report reason:
                if( reason & static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::C6 ) )
                {
                    // In C6, some configuration registers are reset,
                    // so it is assumed that the report is always invalid.
                    log.Debug( "Rc6 report found, oa config is invalid" );
                    m_OaBufferState.m_ConfigurationValid = false;
                }
                else if( reason & static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::ConfigurationUpdate ) )
                {
                    // The configuring report is always valid.
                    log.Debug( "Configuring report found, oa config is valid" );
                    m_OaBufferState.m_ConfigurationValid = true;
                }
                // Other report reasons do not affect the state of the m_ConfigurationValid.

                // The report is only valid if the context is compatible and the oa configuration is valid.
                log.m_Result = m_OaBufferState.m_ContextValid && m_OaBufferState.m_ConfigurationValid;
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks if given hw context id belongs to Render Command Streamer.
        /// @param  contextId       hw context id to check.
        /// @return                 true if hw context should be taken into account.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsRcsContextId( const uint32_t contextId ) const
        {
            bool rcsContextId = false;

            for( uint32_t i = 0; i < m_HwContextIds.m_Count; ++i )
            {
                if( ( contextId == m_HwContextIds.m_Context[i].m_Id ) &&
                    ( m_HwContextIds.m_Context[i].m_Node == T::Layouts::Configuration::NodeType::Rcs ) )
                {
                    rcsContextId = true;
                    break;
                }
            }

            return rcsContextId;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns if gpu report is ready to return api report to the user.
        /// @return success if api report can be exposed to the user.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode ValidateReportGpu()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const bool validTags            = m_Query.m_EndTag == m_ReportGpu.m_EndTag;
            const bool validCommandStreamer = m_ReportGpu.m_CommandStreamerIdentificator > 0;

            log.Debug( "Valid tags       ", validTags );
            log.Debug( "    obtained     ", m_ReportGpu.m_EndTag );
            log.Debug( "    expected     ", m_Query.m_EndTag );
            log.Debug( "Command streamer ", m_ReportGpu.m_CommandStreamerIdentificator );

            return log.m_Result = validTags && validCommandStreamer
                ? StatusCode::Success
                : StatusCode::ReportNotReady;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Prints platform specific report gpu information.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode PrintReportGpu()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            uint32_t oaTailPreBeginOffset  = 0;
            uint32_t oaTailPostBeginOffset = 0;
            uint32_t oaTailPreEndOffset    = 0;
            uint32_t oaTailPostEndOffset   = 0;

            ML_FUNCTION_CHECK( m_OaBuffer.GetPreReportOffset( m_ReportGpu, true, oaTailPreBeginOffset ) );
            ML_FUNCTION_CHECK( m_OaBuffer.GetPostReportOffset( m_ReportGpu, true, oaTailPostBeginOffset ) );
            ML_FUNCTION_CHECK( m_OaBuffer.GetPreReportOffset( m_ReportGpu, false, oaTailPreEndOffset ) );
            ML_FUNCTION_CHECK( m_OaBuffer.GetPostReportOffset( m_ReportGpu, false, oaTailPostEndOffset ) );

            const auto csDescription = T::Layouts::HwCounters::GetCommandStreamerDescription( m_ReportGpu.m_CommandStreamerIdentificator, m_Context );

            log.Info(
                "oaTails:",
                csDescription,
                FormatFlag::Decimal,
                "PreBegin =",
                oaTailPreBeginOffset,
                ", PostBegin =",
                oaTailPostBeginOffset,
                ", PreEnd =",
                oaTailPreEndOffset,
                ", PostEnd =",
                oaTailPostEndOffset );

            // Dump all OA reports generated during query
            log.Debug(
                "query window reportsOa count:",
                csDescription,
                FormatFlag::Decimal,
                ( oaTailPostEndOffset - oaTailPreBeginOffset ) / sizeof( TT::Layouts::HwCounters::ReportOa ) );

            if( oaTailPreBeginOffset < oaTailPostEndOffset )
            {
                for( uint32_t i = oaTailPreBeginOffset; i < oaTailPostEndOffset; i += sizeof( TT::Layouts::HwCounters::ReportOa ) )
                {
                    auto& reportOa = m_OaBuffer.GetReport( i );
                    log.Debug(
                        "reportOa: ",
                        csDescription,
                        "(",
                        FormatFlag::Decimal,
                        FormatFlag::SetWidth5,
                        i / sizeof( TT::Layouts::HwCounters::ReportOa ),
                        ")",
                        reportOa );
                }
            }

            log.Debug( "Command streamer id", m_ReportGpu.m_CommandStreamerIdentificator );
            log.Debug( "Gpu report                " );
            log.Info(
                "    m_Begin.m_Oa          ",
                csDescription,
                m_ReportGpu.m_Begin.m_Oa );
            log.Info(
                "    m_End.m_Oa            ",
                csDescription,
                m_ReportGpu.m_End.m_Oa );

            log.Debug( "    m_EndTag              ", m_ReportGpu.m_EndTag );
            log.Debug( "    m_DmaFenceIdBegin     ", m_ReportGpu.m_DmaFenceIdBegin );
            log.Debug( "    m_DmaFenceIdEnd       ", m_ReportGpu.m_DmaFenceIdEnd );
            log.Debug( "    m_OaBuffer            ", m_ReportGpu.m_OaBuffer.All.m_ReportBufferOffset );
            log.Debug( "    m_CoreFrequencyBegin  ", m_ReportGpu.m_CoreFrequencyBegin );
            log.Debug( "    m_CoreFrequencyEnd    ", m_ReportGpu.m_CoreFrequencyEnd );
            log.Debug( "    m_MarkerUser          ", m_ReportGpu.m_MarkerUser );
            log.Debug( "    m_MarkerDriver        ", m_ReportGpu.m_MarkerDriver );

            return log.m_Result;
        }
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct QueryHwCountersCalculatorTrait : XE_HP::QueryHwCountersCalculatorTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersCalculatorTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct QueryHwCountersCalculatorTrait : XE_HPG::QueryHwCountersCalculatorTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersCalculatorTrait, XE_HPG );
    };
} // namespace ML::XE_HPC
