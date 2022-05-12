/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_oa_buffer_mapped.h

@brief Mapped representation of oa buffer. Allows to initialize oa buffer and
       its state and collects reports from it.
*/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for OaBufferMappedTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct OaBufferMappedTrait
        {
            ML_DELETE_DEFAULT_COPY_AND_MOVE( OaBufferMappedTrait );

        private:
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
                : m_Kernel( kernel )
                , m_OaBufferType( T::Layouts::OaBuffer::Type::Oa )
                , m_OaBuffer( kernel.m_Tbs.GetOaBufferMapped( m_OaBufferType ) )
                , m_ReportSplitted{}
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "OaBufferMappedTrait<Traits>";
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
            /// @param  query query instance.
            /// @return       operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode UpdateQuery( TT::Queries::HwCounters::Slot& query ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
                ML_ASSERT( IsValid() );

                auto& state     = query.m_OaBufferState;
                auto& reportGpu = query.GetReportGpu();

                const uint32_t base      = reportGpu.m_OaBuffer.GetAllocationOffset();
                const uint32_t tailBegin = reportGpu.m_OaTailPreBegin.GetOffset();
                const uint32_t tailEnd   = reportGpu.m_OaTailPostEnd.GetOffset();

                const bool validBegin = tailBegin >= base;
                const bool validEnd   = tailEnd >= base;

                state.m_TailBeginIndex = reportGpu.m_OaTailPreBegin.GetIndex( reportGpu.m_OaBuffer, m_Kernel.m_Context );
                state.m_TailEndIndex   = reportGpu.m_OaTailPostEnd.GetIndex( reportGpu.m_OaBuffer, m_Kernel.m_Context );
                log.m_Result           = ML_STATUS( validBegin && validEnd );

                log.Debug( "Base address   ", base );
                log.Debug( "Tail pre begin ", tailBegin, state.m_TailBeginIndex );
                log.Debug( "Tail post end  ", tailEnd, state.m_TailEndIndex );
                log.Debug( "Valid          ", log.m_Result );

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
            /// @brief  Finds oa reports between query begin/end reports.
            /// @param  oaBufferState  oa buffer state.
            /// @return                oa reports count between query begin/end.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t FindOaWindow( const TT::Layouts::OaBuffer::State& oaBufferState ) const
            {
                ML_FUNCTION_LOG( uint32_t{ 0 }, &m_Kernel.m_Context );
                ML_ASSERT( m_OaBuffer.IsMapped() );

                const uint32_t oaWindow       = oaBufferState.m_TailEndIndex - oaBufferState.m_TailBeginIndex;
                const uint32_t oaReportsCount = ( ( oaBufferState.m_TailEndIndex < oaBufferState.m_TailBeginIndex ) ? m_OaBuffer.m_ReportsCount : 0 ) + oaWindow;

                log.Info( "Oa tail begin index", FormatFlag::Decimal, oaBufferState.m_TailBeginIndex );
                log.Info( "Oa tail end index  ", FormatFlag::Decimal, oaBufferState.m_TailEndIndex );
                log.Info( "Oa reports count   ", FormatFlag::Decimal, oaReportsCount );

                return log.m_Result = oaReportsCount;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa report from oa buffer.
            /// @param  index   oa report index within oa buffer.
            /// @return         reference to oa report.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE TT::Layouts::HwCounters::ReportOa& GetReport( const uint32_t index )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
                ML_ASSERT( m_OaBuffer.IsMapped() )

                const bool oaReportSplit = IsSplitted( index );

                auto& oaReport = oaReportSplit
                    ? GetSplittedReport( index )
                    : m_OaBuffer.m_Reports[index];

                log.Debug( "Oa report index", FormatFlag::Decimal, index );
                log.Debug( "Oa report split", oaReportSplit );
                log.Debug( "Oa report", oaReport );

                return oaReport;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns first oa report associated with query begin/end report.
            /// @param  reportGpu   gpu report collected by query.
            /// @param  begin       query begin/end.
            /// @return index       oa tail index.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetPreReportIndex(
                const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
                const bool                                       begin,
                uint32_t&                                        index )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
                ML_FUNCTION_CHECK( m_OaBuffer.IsMapped() );

                const auto&    oaTail = begin ? reportGpu.m_OaTailPreBegin : reportGpu.m_OaTailPreEnd;
                const uint32_t count  = GetReportsCount();
                index                 = oaTail.GetIndex( reportGpu.m_OaBuffer, m_Kernel.m_Context );

                return log.m_Result = ML_STATUS( index < count );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns last oa report associated with query begin/end report.
            /// @param  query   gpu report collected by query.
            /// @param  begin   query begin/end.
            /// @return index   oa tail index.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetPostReportIndex(
                const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
                const bool                                       begin,
                uint32_t&                                        index )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
                ML_FUNCTION_CHECK( m_OaBuffer.IsMapped() );

                const auto&    oaTail = begin ? reportGpu.m_OaTailPostBegin : reportGpu.m_OaTailPostEnd;
                const uint32_t count  = GetReportsCount();
                index                 = oaTail.GetIndex( reportGpu.m_OaBuffer, m_Kernel.m_Context );

                return log.m_Result = ML_STATUS( index < count );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa reports count within oa buffer.
            /// @return oa reports count within oa buffer.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetReportsCount() const
            {
                return m_OaBuffer.m_ReportsCount;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Dumps oa buffer reports between query begin / query end.
            /// @param  reportGpu   gpu query report.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode DumpReports( const TT::Layouts::HwCounters::Query::ReportGpu reportGpu )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

                if( T::Tools::CheckLogLevel( LogType::Csv ) )
                {
                    const auto emptyReportOa = TT::Layouts::HwCounters::ReportOa{};
                    uint32_t   beginIndex    = reportGpu.m_OaTailPreBegin.GetIndex( reportGpu.m_OaBuffer, m_Kernel.m_Context );
                    uint32_t   endIndex      = reportGpu.m_OaTailPostEnd.GetIndex( reportGpu.m_OaBuffer, m_Kernel.m_Context );

                    // Print empty report first to distinguish oa buffer reports for different queries.
                    log.Csv( &m_Kernel.m_Context, emptyReportOa );

                    if( beginIndex != endIndex )
                    {
                        uint32_t reportsCount = GetReportsCount();
                        uint32_t index        = beginIndex;

                        ML_ASSERT( ( beginIndex < reportsCount ) && ( endIndex < reportsCount ) );

                        do
                        {
                            log.Csv( &m_Kernel.m_Context, GetReport( index ) );
                            index = ( index < reportsCount ) ? ++index : 0;
                        }
                        while( index != endIndex );
                    }
                }

                return log.m_Result;
            }

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief Prints out requested report count.
            /// @param index  oa report start index.
            /// @param count  oa report count.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void PrintReports(
                const uint32_t index,
                const uint32_t count ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

                for( uint32_t i = 0; i < count; ++i )
                {
                    log.Debug( "Oa report", index + i, m_OaBuffer.m_Reports[index + i] );
                }
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns byte offset for a given oa report index.
            /// @param  index   oa report index.
            /// @return         byte offset.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetOffset( const uint32_t index ) const
            {
                return ( index * m_OaBuffer.m_ReportSize ) % m_OaBuffer.m_Size;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Checks whether oa report is splitted.
            /// @param  index   oa report index.
            /// @return         true if oa report is splitted.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsSplitted( const uint32_t index ) const
            {
                const uint32_t offset = GetOffset( index );

                return ( offset + m_OaBuffer.m_ReportSize ) > m_OaBuffer.m_Size;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Reconstructs splitted oa report.
            /// @param  index   splitted oa report index.
            /// @return         a reference to reconstructed oa report.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE TT::Layouts::HwCounters::ReportOa& GetSplittedReport( const uint32_t index )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

                const uint32_t offset      = GetOffset( index );
                const uint32_t reportSize  = m_OaBuffer.m_ReportSize;
                uint8_t*       report      = reinterpret_cast<uint8_t*>( &m_ReportSplitted );
                uint8_t*       cpuAddress  = reinterpret_cast<uint8_t*>( m_OaBuffer.m_CpuAddress );
                const uint32_t part1Length = m_OaBuffer.m_Size - offset;
                const uint32_t part2Length = reportSize - part1Length;

                T::Tools::MemoryCopy( report, reportSize, cpuAddress + offset, part1Length );
                T::Tools::MemoryCopy( report + part1Length, reportSize - part1Length, cpuAddress, part2Length );

                ML_ASSERT( ( offset + reportSize ) > m_OaBuffer.m_Size );

                return m_ReportSplitted;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct OaBufferMappedTrait : BASE::OaBufferMappedTrait<T>
        {
            ML_DECLARE_TRAIT( OaBufferMappedTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct OaBufferMappedTrait : GEN9::OaBufferMappedTrait<T>
        {
            ML_DECLARE_TRAIT( OaBufferMappedTrait, GEN9 );
        };
    } // namespace GEN11

    namespace XE_LP
    {
        template <typename T>
        struct OaBufferMappedTrait : GEN11::OaBufferMappedTrait<T>
        {
            ML_DECLARE_TRAIT( OaBufferMappedTrait, GEN11 );
        };
    } // namespace XE_LP
} // namespace ML
