/*========================== begin_copyright_notice ============================

Copyright (C) 2023-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_query_hw_counters_slot.h

@brief Implements a single hw counters slot.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for QueryHwCountersSlotTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct QueryHwCountersSlotTrait : TraitObject<T, TT::Queries::HwCountersSlot>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( QueryHwCountersSlotTrait );
        ML_DELETE_DEFAULT_COPY( QueryHwCountersSlotTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = TraitObject<T, TT::Queries::HwCountersSlot>;
        using Base::Derived;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Query state enumeration.
        //////////////////////////////////////////////////////////////////////////
        enum class State : uint32_t
        {
            Initial = 0,
            Begun,
            Ended,
            Resolved
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        GpuMemory_1_0                                        m_GpuMemory;                 // Gpu memory associated with query slot.
        uint64_t                                             m_EndTag;                    // End tag / completion status.
        uint32_t                                             m_ApiReportIndex;            // Current report id exposed by api.
        uint32_t                                             m_ApiReportsCount;           // Reports count exposed by api.
        uint8_t*                                             m_WorkloadBegin;             // Store a buffer pointer at the begin of query.
        uint8_t*                                             m_WorkloadEnd;               // Store a buffer pointer at the end of query.
        TT::Context&                                         m_Context;                   // Context.
        TT::Layouts::OaBuffer::State                         m_OaBufferState;             // Oa buffer state captured during slot execution.
        TT::Layouts::HwCounters::Query::ReportCollectingMode m_ReportCollectingMode;      // Report collecting mode (via MIRPC / SRMs / Oa triggers).
        State                                                m_State;                     // Tracks state for query slot.
        uint32_t                                             m_TriggeredReportGetAttempt; // Number of triggered report get attempts.

        //////////////////////////////////////////////////////////////////////////
        /// @brief Slot constructor.
        /// @param context  context.
        //////////////////////////////////////////////////////////////////////////
        QueryHwCountersSlotTrait( TT::Context& context )
            : m_GpuMemory{}
            , m_EndTag( 0 )
            , m_ApiReportIndex( 1 )
            , m_ApiReportsCount( 1 )
            , m_WorkloadBegin( nullptr )
            , m_WorkloadEnd( nullptr )
            , m_Context( context )
            , m_OaBufferState{}
            , m_ReportCollectingMode( T::Layouts::HwCounters::Query::ReportCollectingMode::ReportPerformanceCounters )
            , m_State( State::Initial )
            , m_TriggeredReportGetAttempt( 0 )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Slot move constructor.
        /// @param slot slot.
        //////////////////////////////////////////////////////////////////////////
        QueryHwCountersSlotTrait( QueryHwCountersSlotTrait&& slot )
            : m_GpuMemory( slot.m_GpuMemory )
            , m_EndTag( slot.m_EndTag )
            , m_ApiReportIndex( slot.m_ApiReportIndex )
            , m_ApiReportsCount( slot.m_ApiReportsCount )
            , m_WorkloadBegin( slot.m_WorkloadBegin )
            , m_WorkloadEnd( slot.m_WorkloadEnd )
            , m_Context( slot.m_Context )
            , m_OaBufferState( slot.m_OaBufferState )
            , m_ReportCollectingMode( slot.m_ReportCollectingMode )
            , m_State( slot.m_State )
            , m_TriggeredReportGetAttempt( 0 )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "QueryHwCountersSlotTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Resets slot's state.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void Reset()
        {
            m_ApiReportIndex  = 1;
            m_ApiReportsCount = 1;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns internal hw counters gpu report for a given slot.
        /// @return a reference to internal hw counters layout data.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::Query::ReportGpu& GetReportGpu() const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_ASSERT( m_GpuMemory.CpuAddress );

            return *static_cast<TT::Layouts::HwCounters::Query::ReportGpu*>( m_GpuMemory.CpuAddress );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Clears gpu memory for a given slot.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void ClearReportGpu()
        {
            if constexpr( T::Policy::QueryHwCounters::Begin::m_ClearGpuMemory )
            {
                auto reportGpu = static_cast<TT::Layouts::HwCounters::Query::ReportGpu*>( m_GpuMemory.CpuAddress );

                if( reportGpu != nullptr )
                {
                    *reportGpu = {};

                    // Reset report oa memory to check mirpc completeness.
                    if( m_ReportCollectingMode == T::Layouts::HwCounters::Query::ReportCollectingMode::ReportPerformanceCounters )
                    {
                        Derived().ResetOaReport( *reportGpu );
                    }
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Resets oa report in gpu memory.
        /// @param  reportGpu   report gpu.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void ResetOaReport( [[maybe_unused]] TT::Layouts::HwCounters::Query::ReportGpu& reportGpu ) const
        {
            // Do nothing on purpose.
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Updates query slot state.
        /// @param newState    new query slot state.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void UpdateState( const State newState )
        {
            m_State = newState;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks query slot sequence correctness.
        /// @param  newState    new query slot state.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode CheckStateConsistency( const State newState ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            switch( m_State )
            {
                case State::Initial:
                    log.m_Result = ( newState == State::Begun )
                        ? StatusCode::Success
                        : StatusCode::Failed;
                    break;

                case State::Begun:
                    log.m_Result = ( newState == State::Ended )
                        ? StatusCode::Success
                        : ( newState == State::Begun ) // Multiple begins are acceptable, but not expected.
                        ? StatusCode::IncorrectParameter
                        : StatusCode::Failed;
                    break;

                case State::Ended:
                    log.m_Result = ( newState == State::Resolved )
                        ? StatusCode::Success
                        : ( newState == State::Begun || newState == State::Ended ) // Multiple ends or begin after end are acceptable, but not expected.
                        ? StatusCode::IncorrectParameter
                        : StatusCode::Failed;
                    break;

                case State::Resolved:
                    log.m_Result = ( newState == State::Begun || newState == State::Resolved )
                        ? StatusCode::Success
                        : StatusCode::Failed;
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    break;
            }

            switch( log.m_Result )
            {
                case StatusCode::Success:
                    log.Debug( "Correct sequence of query calls:", m_State, "->", newState );
                    break;

                case StatusCode::IncorrectParameter:
                    log.Warning( "Incorrect sequence of query calls but acceptable:", m_State, "->", newState );
                    log.m_Result = StatusCode::Success;
                    break;

                default:
                    log.Error( "Incorrect sequence of query calls:", m_State, "->", newState );
                    break;
            }

            return log.m_Result;
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct QueryHwCountersSlotTrait : BASE::QueryHwCountersSlotTrait<T>
    {
        ML_DECLARE_TRAIT_WITH_COPY_AND_MOVE( QueryHwCountersSlotTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct QueryHwCountersSlotTrait : GEN9::QueryHwCountersSlotTrait<T>
    {
        ML_DECLARE_TRAIT_WITH_COPY_AND_MOVE( QueryHwCountersSlotTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct QueryHwCountersSlotTrait : GEN11::QueryHwCountersSlotTrait<T>
    {
        ML_DECLARE_TRAIT_WITH_COPY_AND_MOVE( QueryHwCountersSlotTrait, GEN11 );
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct QueryHwCountersSlotTrait : XE_LP::QueryHwCountersSlotTrait<T>
    {
        ML_DECLARE_TRAIT_WITH_COPY_AND_MOVE( QueryHwCountersSlotTrait, XE_LP );
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct QueryHwCountersSlotTrait : XE_HP::QueryHwCountersSlotTrait<T>
    {
        ML_DECLARE_TRAIT_WITH_COPY_AND_MOVE( QueryHwCountersSlotTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct QueryHwCountersSlotTrait : XE_HPG::QueryHwCountersSlotTrait<T>
    {
        ML_DECLARE_TRAIT_WITH_COPY_AND_MOVE( QueryHwCountersSlotTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct QueryHwCountersSlotTrait : XE_HPG::QueryHwCountersSlotTrait<T>
    {
        ML_DECLARE_TRAIT_WITH_COPY_AND_MOVE( QueryHwCountersSlotTrait, XE_HPG );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Resets oa report in gpu memory.
        /// @param  reportGpu   report gpu.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void ResetOaReport( TT::Layouts::HwCounters::Query::ReportGpu& reportGpu ) const
        {
            constexpr uint32_t lastPerformanceEventCounterIndex = T::Layouts::HwCounters::m_PerformanceEventCountersCount - 1;

            reportGpu.m_Begin.m_Oa.m_Data.m_PerformanceEventCounter[lastPerformanceEventCounterIndex]     = T::Layouts::HwCounters::m_MirpcResetValue;
            reportGpu.m_Begin.m_Oa.m_Data.m_PerformanceEventCounter[lastPerformanceEventCounterIndex - 1] = T::Layouts::HwCounters::m_MirpcResetValue;

            reportGpu.m_End.m_Oa.m_Data.m_PerformanceEventCounter[lastPerformanceEventCounterIndex]     = T::Layouts::HwCounters::m_MirpcResetValue;
            reportGpu.m_End.m_Oa.m_Data.m_PerformanceEventCounter[lastPerformanceEventCounterIndex - 1] = T::Layouts::HwCounters::m_MirpcResetValue;
        }
    };
} // namespace ML::XE2_HPG
