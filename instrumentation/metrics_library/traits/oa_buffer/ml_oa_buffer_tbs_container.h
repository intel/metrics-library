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

@file ml_oa_buffer_tbs_container.h

@brief Allows to collect reports from tbs and print it out.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for TbsContainerTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct TbsContainerTrait
        {
            ML_DELETE_DEFAULT_COPY_AND_MOVE( TbsContainerTrait );

        private:
            using OaReports = std::array<TT::Layouts::HwCounters::ReportOa, T::ConstantsOs::TbsContainer::m_ReportsCount>;

            bool           m_Overrun;     // Buffer overrun flag.
            uint32_t       m_RecentIndex; // Recently used report index.
            const uint32_t m_Capacity;    // Buffer capacity.
            OaReports      m_OaReports;   // Buffer reports.

        public:
            //////////////////////////////////////////////////////////////////////////
            /// @brief  Tbs container constructor.
            //////////////////////////////////////////////////////////////////////////
            TbsContainerTrait()
                : m_Overrun( false )
                , m_RecentIndex( T::ConstantsOs::OaBuffer::m_InvalidIndex )
                , m_Capacity( T::ConstantsOs::TbsContainer::m_ReportsCount )
                , m_OaReports{}
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "OaBufferTbsContainer<Traits>";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Collects oa reports from tbs.
            /// @param  kernel          kernel interface.
            /// @param  endTimestamp    collect oa reports older than endTimestamp
            /// @return                 success if oa reports has been collected.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Collect(
                const TT::KernelInterface& kernel,
                const uint32_t             endTimestamp )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                bool complete = GetLastTimestamp() > endTimestamp;

                // Collect samples.
                while( ML_SUCCESS( log.m_Result ) && !complete )
                {
                    log.m_Result = CollectReports( kernel, complete );
                    complete |= GetLastTimestamp() > endTimestamp;
                }

                // Print statistics.
                T::ConstantsOs::TbsContainer::m_PrintAll
                    ? PrintAll()
                    : PrintState();

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Finds oa reports between query begin/end reports.
            /// @param  reportGpu       query gpu report.
            /// @param  oaBufferState   oa buffer state.
            /// @return                 oa reports count between query begin/end.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t FindOaWindow(
                const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
                TT::Layouts::OaBuffer::State&                    oaBufferState )
            {
                ML_FUNCTION_LOG( uint32_t{ 0 } );

                const bool validOaTailBegin = FindOaTailBegin( reportGpu, oaBufferState );
                const bool validOaTailEnd   = validOaTailBegin && FindOaTailEnd( reportGpu, oaBufferState );
                const bool valid            = validOaTailEnd;

                log.m_Result = valid
                    ? GetReportsCount( oaBufferState )
                    : 0;

                PrintOaWindow( reportGpu, oaBufferState );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Check whether oa buffer contains reports.
            /// @return true if oa buffer is empty.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsEmpty() const
            {
                return m_RecentIndex == T::ConstantsOs::OaBuffer::m_InvalidIndex;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief   Returns maximum oa reports count within container.
            /// @return  maximum oa reports count within container.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetCapacity() const
            {
                return m_Capacity;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns collected oa report.
            /// @param  index   oa report index
            /// @return         reference to oa report.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE const TT::Layouts::HwCounters::ReportOa& GetReport( const uint32_t index ) const
            {
                ML_ASSERT( index >= 0 );
                ML_ASSERT( index < m_Capacity );

                return m_OaReports[index];
            }

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa reports count between oa tail begin/end.
            /// @param  oaBufferState  oa buffer state captured by query.
            /// @return                oa reports count between oa tail begin/end.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetReportsCount( const TT::Layouts::OaBuffer::State& oaBufferState )
            {
                const auto& tailBegin = oaBufferState.m_TailBeginIndex;
                const auto& tailEnd   = oaBufferState.m_TailEndIndex;

                if( !m_Overrun )
                {
                    ML_ASSERT( tailEnd >= tailBegin );
                    return ( tailEnd - tailBegin );
                }
                else
                {
                    return tailEnd > tailBegin
                        ? ( tailEnd - tailBegin )
                        : ( m_Capacity - tailEnd + tailBegin );
                }
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Increments slot index.
            /// @param  index   index to increment.
            /// @return         incremented slot index.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetNextSlot( const uint32_t index ) const
            {
                return ( index + 1 ) % m_Capacity;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Decrements slot index.
            /// @param  index   index to decrement.
            /// @return         decremented slot index.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetPreviousSlot( const uint32_t index ) const
            {
                if( !m_Overrun )
                {
                    return index
                        ? index - 1
                        : m_RecentIndex;
                }
                else
                {
                    return index
                        ? index - 1
                        : m_Capacity - 1;
                }
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Collects oa reports from tbs.
            /// @param  kernel      interface to kernel driver.
            /// @return completed   true if oa reports has been collected
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode CollectReports(
                const TT::KernelInterface& kernel,
                bool&                      completed )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Slots to obtain.
                const uint32_t           slotNext       = GetNextSlot( m_RecentIndex );
                const uint32_t           slotsFree      = m_Capacity - slotNext;
                const constexpr uint32_t slotsRequested = T::ConstantsOs::TbsContainer::m_RequestCount;
                uint32_t                 slots          = std::min<uint32_t>( slotsFree, slotsRequested );

                // Tbs result;
                const bool validTbsResult  = ML_SUCCESS( log.m_Result = kernel.TbsGetReports( &( m_OaReports[slotNext] ), slots, completed ) );
                const bool validTbsSamples = slots > 0;

                if( validTbsResult && validTbsSamples )
                {
                    m_RecentIndex += slots;
                    m_Overrun |= m_RecentIndex >= m_Capacity;
                    m_RecentIndex %= m_Capacity;
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Finds oa tail begin report just after query begin report.
            /// @param  reportGpu       query gpu report.
            /// @param  oaBufferState   oa buffer state.
            /// @return                 true if oa begin report has been found.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool FindOaTailBegin(
                const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
                TT::Layouts::OaBuffer::State&                    oaBufferState ) const
            {
                ML_ASSERT( m_RecentIndex != T::ConstantsOs::OaBuffer::m_InvalidIndex );

                // clang-format off
                auto oaBegin     = m_OaReports.begin();
                auto oaEnd       = m_OaReports.end();
                auto oaLast      = m_OaReports.begin() + m_RecentIndex;
                auto oaTailBegin = m_OaReports.end();
                auto oaTailFind  = []( const TT::Layouts::HwCounters::ReportOa& report0,
                                       const TT::Layouts::HwCounters::ReportOa& report1 )
                                   {
                                       return report0.m_Header.m_Timestamp < report1.m_Header.m_Timestamp;
                                   };
                // clang-format on

                if( !m_Overrun )
                {
                    // 1. Search between oaBegin ... oaLast.
                    oaTailBegin = std::lower_bound( oaBegin, oaLast, reportGpu.m_Begin.m_Oa, oaTailFind );
                }
                else
                {
                    // 1. Search between newest reports (oaBegin ... oaLast).
                    // 2. Search between old reports (oaLast ... oaEnd).
                    oaTailBegin = std::lower_bound( oaBegin, oaLast, reportGpu.m_Begin.m_Oa, oaTailFind );
                    oaTailBegin = oaTailBegin == oaEnd
                        ? std::lower_bound( oaLast, oaEnd, reportGpu.m_Begin.m_Oa, oaTailFind )
                        : oaTailBegin;
                }

                // Validate found oa tail begin.
                const bool oaTailFound          = oaTailBegin != oaEnd;
                const bool oaTailBeforeQueryEnd = oaTailFound && ( oaTailBegin->m_Header.m_Timestamp <= reportGpu.m_End.m_Oa.m_Header.m_Timestamp );
                const bool oaTailValid          = oaTailFound && oaTailBeforeQueryEnd;

                // Store oa tail position.
                oaBufferState.m_TailBeginIndex = oaTailValid
                    ? static_cast<uint32_t>( std::distance( oaBegin, oaTailBegin ) )
                    : 0;

                return oaTailValid;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Finds oa tail end report before query end report.
            /// @param  reportGpu     query gpu report.
            /// @param  oaBufferState oa buffer state captured by query.
            /// @return               true if oa tail end report has been found.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool FindOaTailEnd(
                const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
                TT::Layouts::OaBuffer::State&                    oaBufferState ) const
            {
                ML_ASSERT( m_RecentIndex != T::ConstantsOs::OaBuffer::m_InvalidIndex );

                // clang-format off
                auto oaBegin     = m_OaReports.begin();
                auto oaEnd       = m_OaReports.end();
                auto oaLast      = m_OaReports.begin() + m_RecentIndex;
                auto oaTailBegin = m_OaReports.begin() + oaBufferState.m_TailBeginIndex;
                auto oaTailEnd   = m_OaReports.end();
                auto oaTailFind  = []( const TT::Layouts::HwCounters::ReportOa& report0,
                                       const TT::Layouts::HwCounters::ReportOa& report1 )
                                   {
                                       return report0.m_Header.m_Timestamp < report1.m_Header.m_Timestamp;
                                   };
                // clang-format on

                if( !m_Overrun )
                {
                    // 1. Search between oaBegin ... oaLast.
                    oaTailEnd = std::lower_bound( oaTailBegin, oaLast, reportGpu.m_End.m_Oa, oaTailFind );
                }
                else
                {
                    // 1. Search between newest reports (oaBegin ... oaLast).
                    // 2. Search between old reports (oaLast ... oaEnd).
                    oaTailEnd = std::lower_bound( oaBegin, oaLast, reportGpu.m_End.m_Oa, oaTailFind );
                    oaTailEnd = oaTailEnd == oaEnd
                        ? std::lower_bound( oaLast, oaEnd, reportGpu.m_End.m_Oa, oaTailFind )
                        : oaTailEnd;
                }

                // We have found oaTail > mirpcEnd we need --oaTail to have oaTail < mirpcEnd
                if( oaTailEnd != oaEnd )
                {
                    oaBufferState.m_TailEndIndex = static_cast<uint32_t>( std::distance( oaBegin, oaTailEnd ) );
                    oaBufferState.m_TailEndIndex = GetPreviousSlot( oaBufferState.m_TailEndIndex );

                    const bool validBegin = m_OaReports[oaBufferState.m_TailEndIndex].m_Header.m_Timestamp > reportGpu.m_Begin.m_Oa.m_Header.m_Timestamp;
                    const bool validEnd   = m_OaReports[oaBufferState.m_TailEndIndex].m_Header.m_Timestamp < reportGpu.m_End.m_Oa.m_Header.m_Timestamp;

                    return validBegin && validEnd;
                }

                return false;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns a timestamp from the last collected oa reports.
            /// @return last timestamp.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE const uint32_t GetLastTimestamp() const
            {
                return m_RecentIndex != T::ConstantsOs::OaBuffer::m_InvalidIndex
                    ? m_OaReports[m_RecentIndex].m_Header.m_Timestamp
                    : 0;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Prints out all collected reports.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void PrintAll() const
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_ASSERT( m_RecentIndex != T::ConstantsOs::OaBuffer::m_InvalidIndex );

                const uint32_t count = m_Overrun
                    ? m_Capacity
                    : m_RecentIndex;

                PrintState();

                for( uint32_t i = 0; i < count; ++i )
                {
                    log.Debug( "Oa report", i, m_OaReports[i] );
                }
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Prints out a simple statistics about collected samples.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void PrintState() const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                log.Debug( "Oa recently used index", m_RecentIndex );
                log.Debug( "Oa buffer capacity", m_Capacity );
                log.Debug( "Oa buffer overrun", m_Overrun );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Prints out found oa reports collected between query begin/end.
            /// @param  reportGpu       query gpu report.
            /// @param  oaBufferState   oa buffer state.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void PrintOaWindow(
                const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
                const TT::Layouts::OaBuffer::State&              oaBufferState )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const uint32_t oaReportsCount = GetReportsCount( oaBufferState );

                if( oaReportsCount )
                {
                    const uint32_t oaTailBeginPrevious = GetPreviousSlot( oaBufferState.m_TailBeginIndex );
                    const uint32_t oaTailEndNext       = GetNextSlot( oaBufferState.m_TailEndIndex );
                    const auto&    tailBeginPrevious   = m_OaReports[oaTailBeginPrevious];
                    const auto&    tailBegin           = m_OaReports[oaBufferState.m_TailBeginIndex];
                    const auto&    tailEnd             = m_OaReports[oaBufferState.m_TailEndIndex];
                    const auto&    tailEndNext         = m_OaReports[oaTailEndNext];

                    log.Debug( "Oa    reports    ", oaReportsCount );
                    log.Debug( "Oa    prev       ", tailBeginPrevious.m_Header.m_Timestamp, oaTailBeginPrevious );
                    log.Debug( "Query begin      ", reportGpu.m_Begin.m_Oa.m_Header.m_Timestamp );
                    log.Debug( "Oa    tail begin ", tailBegin.m_Header.m_Timestamp );
                    log.Debug( "Oa    tail end   ", tailEnd.m_Header.m_Timestamp );
                    log.Debug( "Query end        ", reportGpu.m_End.m_Oa.m_Header.m_Timestamp );
                    log.Debug( "Oa    next       ", tailEndNext.m_Header.m_Timestamp, oaTailEndNext );

                    // clang-format off
                    ML_ASSERT( reportGpu.m_Begin.m_Oa.m_Header.m_Timestamp <= tailBegin.m_Header.m_Timestamp );             // query begin <= tail begin.
                    ML_ASSERT( tailBegin.m_Header.m_Timestamp              <= tailEnd.m_Header.m_Timestamp );               // tail begin  <= tail end.
                    ML_ASSERT( tailEnd.m_Header.m_Timestamp                <= reportGpu.m_End.m_Oa.m_Header.m_Timestamp );  // tail end    <= query end.
                    // clang-format on
                }
                else
                {
                    log.Debug( "Unable to find oa reports between query begin/end" );
                    log.Debug( "Mirpc begin", reportGpu.m_Begin.m_Oa.m_Header.m_Timestamp );
                    log.Debug( "Mirpc end  ", reportGpu.m_End.m_Oa.m_Header.m_Timestamp );
                    log.Debug( "Recent oa  ", m_OaReports[m_RecentIndex].m_Header.m_Timestamp );
                }
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct TbsContainerTrait : BASE::TbsContainerTrait<T>
        {
            ML_DECLARE_TRAIT( TbsContainerTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct TbsContainerTrait : GEN9::TbsContainerTrait<T>
        {
            ML_DECLARE_TRAIT( TbsContainerTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct TbsContainerTrait : GEN11::TbsContainerTrait<T>
        {
            ML_DECLARE_TRAIT( TbsContainerTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
