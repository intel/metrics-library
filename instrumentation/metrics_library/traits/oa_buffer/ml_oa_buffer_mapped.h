/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_oa_buffer_mapped.h

@brief Mapped representation of oa buffer. Allows to initialize oa buffer and
       its state and collects reports from it.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for OaBufferMappedTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct OaBufferMappedTrait : TraitObject<T, TT::OaBuffer>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( OaBufferMappedTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( OaBufferMappedTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = TraitObject<T, TT::OaBuffer>;
        using Base::DerivedConst;

    protected:
        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::KernelInterface&              m_Kernel;
        const TT::Layouts::OaBuffer::Type m_OaBufferType;
        TT::TbsInterface::OaBufferMapped& m_OaBuffer;
        TT::Layouts::HwCounters::ReportOa m_ReportSplitted;

    public:
        //////////////////////////////////////////////////////////////////////////
        /// @brief OaBufferMappedTrait constructor.
        /// @param kernel   kernel interface.
        //////////////////////////////////////////////////////////////////////////
        OaBufferMappedTrait( TT::KernelInterface& kernel )
            : Base()
            , m_Kernel( kernel )
            , m_OaBufferType( T::Layouts::OaBuffer::Type::Oa )
            , m_OaBuffer( kernel.m_Tbs.GetOaBufferMapped( m_OaBufferType ) )
            , m_ReportSplitted{}
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes oa buffer.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            // Some cases does not require mapped oa buffer.
            // For example when tbs is used by metrics discovery.
            log.m_Result = m_Kernel.m_Context.m_ClientOptions.m_TbsEnabled
                ? StatusCode::Success
                : m_OaBuffer.Map();

            // Mapped oa buffer may not be required for some cases.
            // For example linux without oa buffer mapping patch
            // will use a standard mirpc (without context switch handling).
            log.m_Result = T::ConstantsOs::Tbs::m_MappingRequired
                ? log.m_Result
                : StatusCode::Success;

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Updates oa buffer state kept by hw counters query.
        /// @param  state       oa buffer state.
        /// @param  reportGpu   query report gpu.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode UpdateQuery(
            TT::Layouts::OaBuffer::State&              state,
            TT::Layouts::HwCounters::Query::ReportGpu& reportGpu ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_ASSERT( IsValid() );
            ML_FUNCTION_CHECK( GetPreReportOffset<true>( reportGpu, state.m_TailPreBeginOffset ) );
            ML_FUNCTION_CHECK( GetPostReportOffset<true>( reportGpu, state.m_TailPostBeginOffset ) );
            ML_FUNCTION_CHECK( GetPreReportOffset<false>( reportGpu, state.m_TailPreEndOffset ) );
            ML_FUNCTION_CHECK( GetPostReportOffset<false>( reportGpu, state.m_TailPostEndOffset ) );

            const uint32_t base       = reportGpu.m_OaBuffer.GetAllocationOffset();
            const bool     validBegin = ( state.m_TailPreBeginOffset >= 0 ) && ( state.m_TailPreBeginOffset < m_OaBuffer.m_Size );
            const bool     validEnd   = ( state.m_TailPostEndOffset >= 0 ) && ( state.m_TailPostEndOffset < m_OaBuffer.m_Size );

            log.m_Result = ML_STATUS( validBegin && validEnd );

            log.Debug( "Base address  ", base );
            log.Debug( "Tail pre begin", state.m_TailPreBeginOffset + base, state.m_TailPreBeginOffset );
            log.Debug( "Tail post end ", state.m_TailPostEndOffset + base, state.m_TailPostEndOffset );
            log.Debug( "Valid         ", log.m_Result );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Releases a reference to oa buffer.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Release()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            return log.m_Result = m_Kernel.m_Context.m_ClientOptions.m_TbsEnabled
                ? StatusCode::Success
                : m_OaBuffer.Unmap();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks oa buffer correctness.
        /// @return true if oa buffer has valid state.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsValid() const
        {
            ML_FUNCTION_LOG( false, &m_Kernel.m_Context );

            return log.m_Result = m_OaBuffer.IsMapped();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa buffer size in bytes.
        /// @return oa buffer size.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t GetSize() const
        {
            return m_OaBuffer.m_Size;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns report size in oa buffer in bytes.
        /// @return report size.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t GetReportSize() const
        {
            return m_OaBuffer.m_ReportSize;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Finds oa reports between query begin/end reports.
        /// @param  oaBufferState   oa buffer state.
        /// @return                 oa reports count between query begin/end.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t FindOaWindow( const TT::Layouts::OaBuffer::State& oaBufferState ) const
        {
            ML_FUNCTION_LOG( uint32_t{ 0 }, &m_Kernel.m_Context );
            ML_ASSERT( m_OaBuffer.IsMapped() );

            const uint32_t reportSize     = m_OaBuffer.m_ReportSize;
            const uint32_t oaWindow       = ( oaBufferState.m_TailPostEndOffset + m_OaBuffer.m_Size - oaBufferState.m_TailPreBeginOffset ) % m_OaBuffer.m_Size;
            const uint32_t oaReportsCount = oaWindow / reportSize;

            log.Info( "Oa tail begin offset", FormatFlag::Decimal, oaBufferState.m_TailPreBeginOffset );
            log.Info( "Oa tail end offset  ", FormatFlag::Decimal, oaBufferState.m_TailPostEndOffset );
            log.Info( "Oa reports count    ", FormatFlag::Decimal, oaReportsCount );

            return log.m_Result = oaReportsCount;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa report from oa buffer.
        /// @param  offset  oa report offset within oa buffer.
        /// @return         a reference to oa report.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::ReportOa& GetReport( const uint32_t offset )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_ASSERT( m_OaBuffer.IsMapped() )

            const bool oaReportSplit = IsSplitted( offset );

            auto& oaReport = oaReportSplit
                ? GetSplittedReport( offset )
                : *reinterpret_cast<TT::Layouts::HwCounters::ReportOa*>( static_cast<uint8_t*>( m_OaBuffer.m_CpuAddress ) + offset );

            log.Debug( "Oa report offset", FormatFlag::Decimal, offset );
            log.Debug( "Oa report split", oaReportSplit );
            log.Debug( "Oa report", oaReport );

            return oaReport;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns first oa report associated with query begin/end report.
        /// @param  begin       query begin/end.
        /// @param  reportGpu   gpu report collected by query.
        /// @return offset      oa tail offset.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin>
        ML_INLINE StatusCode GetPreReportOffset(
            const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
            uint32_t&                                        offset ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( m_OaBuffer.IsMapped() );

            const auto&    derived = DerivedConst();
            const auto&    oaTail  = begin ? reportGpu.m_OaTailPreBegin : reportGpu.m_OaTailPreEnd;
            const uint32_t base    = reportGpu.m_OaBuffer.GetAllocationOffset();
            const uint32_t size    = m_OaBuffer.m_Size;
            offset                 = oaTail.GetOffset() - base;

            // Roll back offset before begin report to know the measured context on query begin.
            if constexpr( begin )
            {
                derived.RollBackOffset( offset );
            }

            // Round down offset before a triggered oa report if incomplete report.
            derived.RoundDownReportOffset( offset );

            log.Debug( "Offset", offset );

            return log.m_Result = ML_STATUS( offset < size );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns last oa report associated with query begin/end report.
        /// @param  begin       query begin/end.
        /// @param  reportGpu   gpu report collected by query.
        /// @return offset      oa tail offset.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin>
        ML_INLINE StatusCode GetPostReportOffset(
            const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
            uint32_t&                                        offset ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( m_OaBuffer.IsMapped() );

            const auto&    oaTail = begin ? reportGpu.m_OaTailPostBegin : reportGpu.m_OaTailPostEnd;
            const uint32_t base   = reportGpu.m_OaBuffer.GetAllocationOffset();
            const uint32_t size   = m_OaBuffer.m_Size;
            offset                = oaTail.GetOffset() - base;

            // Round up offset after a triggered oa report if incomplete report.
            DerivedConst().RoundUpReportOffset( offset );

            log.Debug( "Offset", offset );

            return log.m_Result = ML_STATUS( offset < size );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Dumps oa buffer reports between query begin / query end.
        /// @param  state   oa buffer state.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode DumpReports( const TT::Layouts::OaBuffer::State& state )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            if( T::Tools::CheckLogLevel( LogType::Csv ) )
            {
                const auto     emptyReportOa = TT::Layouts::HwCounters::ReportOa{};
                const uint32_t reportSize    = m_OaBuffer.m_ReportSize;
                const uint32_t beginOffset   = state.m_TailPreBeginOffset;
                const uint32_t endOffset     = state.m_TailPostEndOffset;

                // Print empty report first to distinguish oa buffer reports for different queries.
                log.Csv( emptyReportOa );

                if( beginOffset != endOffset )
                {
                    const uint32_t oaBufferSize = m_OaBuffer.m_Size;
                    uint32_t       offset       = beginOffset;

                    ML_ASSERT( ( beginOffset < oaBufferSize ) && ( endOffset < oaBufferSize ) );

                    do
                    {
                        log.Csv( GetReport( offset ) );
                        offset = ( offset + reportSize ) % oaBufferSize;
                    }
                    while( offset != endOffset );
                }
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Prints out requested report count.
        /// @param offset   oa report start offset.
        /// @param count    oa report count.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void PrintReports(
            const uint32_t offset,
            const uint32_t count )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            const uint32_t reportSize = m_OaBuffer.m_ReportSize;
            ML_ASSERT( offset + ( count * reportSize ) < m_OaBuffer.m_Size );

            for( uint32_t i = 0; i < count; ++i )
            {
                const auto& oaReport = GetReport( offset + ( i * reportSize ) );
                log.Debug( "Oa report", i, oaReport );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Rounds down report offset to the nearest report beginning.
        /// @return offset  report offset to update.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void RoundDownReportOffset( uint32_t& offset ) const
        {
            offset -= offset % m_OaBuffer.m_ReportSize;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Rounds up report offset to the nearest report end.
        /// @return offset  report offset to update.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void RoundUpReportOffset( uint32_t& offset ) const
        {
            const uint32_t remainder = offset % m_OaBuffer.m_ReportSize;
            if( remainder )
            {
                offset = ( offset + m_OaBuffer.m_ReportSize - remainder ) % m_OaBuffer.m_Size;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Rolls back report offset to the nearest context switch before
        ///         query begin report.
        /// @return offset  report offset to update.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void RollBackOffset( [[maybe_unused]] uint32_t& offset ) const
        {
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks whether oa report is splitted.
        /// @param  offset  oa report offset.
        /// @return         true if oa report is splitted.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsSplitted( const uint32_t offset ) const
        {
            return ( offset + m_OaBuffer.m_ReportSize ) > m_OaBuffer.m_Size;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Reconstructs splitted oa report.
        /// @param  offset  splitted oa report offset.
        /// @return         a reference to reconstructed oa report.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::ReportOa& GetSplittedReport( const uint32_t offset )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            uint8_t*       report      = reinterpret_cast<uint8_t*>( &m_ReportSplitted );
            uint8_t*       cpuAddress  = reinterpret_cast<uint8_t*>( m_OaBuffer.m_CpuAddress );
            const uint32_t reportSize  = m_OaBuffer.m_ReportSize;
            const uint32_t part1Length = m_OaBuffer.m_Size - offset;
            const uint32_t part2Length = reportSize - part1Length;

            T::Tools::MemoryCopy( report, reportSize, cpuAddress + offset, part1Length );
            T::Tools::MemoryCopy( report + part1Length, reportSize - part1Length, cpuAddress, part2Length );

            ML_ASSERT( ( offset + reportSize ) > m_OaBuffer.m_Size );

            return m_ReportSplitted;
        }
    };
} // namespace ML::BASE

namespace ML::XE_LP
{
    template <typename T>
    struct OaBufferMappedTrait : BASE::OaBufferMappedTrait<T>
    {
        ML_DECLARE_TRAIT( OaBufferMappedTrait, BASE );
    };
} // namespace ML::XE_LP

namespace ML::XE_HPG
{
    template <typename T>
    struct OaBufferMappedTrait : XE_LP::OaBufferMappedTrait<T>
    {
        ML_DECLARE_TRAIT( OaBufferMappedTrait, XE_LP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::m_OaBuffer;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Rolls back report offset to the nearest context switch before
        ///         query begin report.
        /// @return offset  report offset to update.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void RollBackOffset( uint32_t& offset ) const
        {
            if( offset < m_OaBuffer.m_ReportSize * Constants::OaBuffer::m_ReportsBeforeQueryBegin )
            {
                // If offset is less than the size of required reports, roll it back to reports at the end of oa buffer.
                offset = m_OaBuffer.m_Size - ( m_OaBuffer.m_ReportSize * Constants::OaBuffer::m_ReportsBeforeQueryBegin - offset );
            }
            else
            {
                offset -= m_OaBuffer.m_ReportSize * Constants::OaBuffer::m_ReportsBeforeQueryBegin;
            }
        }
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct OaBufferMappedTrait : XE_HPG::OaBufferMappedTrait<T>
    {
        ML_DECLARE_TRAIT( OaBufferMappedTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct OaBufferMappedTrait : XE_HPG::OaBufferMappedTrait<T>
    {
        ML_DECLARE_TRAIT( OaBufferMappedTrait, XE_HPG );
    };
} // namespace ML::XE2_HPG

namespace ML::XE3
{
    template <typename T>
    struct OaBufferMappedTrait : XE2_HPG::OaBufferMappedTrait<T>
    {
        ML_DECLARE_TRAIT( OaBufferMappedTrait, XE2_HPG );
    };
} // namespace ML::XE3
