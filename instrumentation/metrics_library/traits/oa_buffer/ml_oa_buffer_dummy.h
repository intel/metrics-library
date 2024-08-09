/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_oa_buffer_dummy.h

@brief Dummy representation of oa buffer.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for OaBufferDummyTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct OaBufferDummyTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( OaBufferDummyTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( OaBufferDummyTrait );

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Members.
        //////////////////////////////////////////////////////////////////////////
        TT::Layouts::HwCounters::ReportOa m_OaReportDummy;
        TT::Context&                      m_Context;

    public:
        //////////////////////////////////////////////////////////////////////////
        /// @brief OaBufferDummyTrait constructor.
        /// @param kernel   kernel interface.
        //////////////////////////////////////////////////////////////////////////
        OaBufferDummyTrait( const TT::KernelInterface& kernel )
            : m_OaReportDummy{}
            , m_Context( kernel.m_Context )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "OaBufferDummyTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes oa buffer.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize() const
        {
            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Updates oa buffer state kept by hw counters query.
        /// @param  state       oa buffer state.
        /// @param  reportGpu   query report gpu.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode UpdateQuery(
            [[maybe_unused]] TT::Layouts::OaBuffer::State&              state,
            [[maybe_unused]] TT::Layouts::HwCounters::Query::ReportGpu& reportGpu ) const
        {
            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Releases a reference to oa buffer.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Release() const
        {
            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks oa buffer state.
        /// @return true if oa buffer has valid state.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsValid() const
        {
            return false;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa buffer size in bytes.
        /// @return oa buffer size.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t GetSize() const
        {
            return 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns report size in oa buffer in bytes.
        /// @return report size.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t GetReportSize() const
        {
            return 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Finds oa reports between query begin/end reports.
        /// @param  oaBufferState   oa buffer state.
        /// @return                 oa reports count between query begin/end.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t FindOaWindow( [[maybe_unused]] const TT::Layouts::OaBuffer::State& oaBufferState ) const
        {
            return 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa report from oa buffer.
        /// @param  offset  oa report offset within oa buffer.
        /// @return         reference to oa report.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::ReportOa& GetReport( [[maybe_unused]] const uint32_t offset )
        {
            return m_OaReportDummy;
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
            [[maybe_unused]] const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
            [[maybe_unused]] uint32_t&                                        offset ) const
        {
            ML_ASSERT_ALWAYS_ADAPTER( m_Context.m_AdapterId );
            return StatusCode::Failed;
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
            [[maybe_unused]] const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
            [[maybe_unused]] uint32_t&                                        offset ) const
        {
            ML_ASSERT_ALWAYS_ADAPTER( m_Context.m_AdapterId );
            return StatusCode::Failed;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Dumps oa buffer reports between query begin / query end.
        /// @param  reportGpu   gpu query report.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode DumpReports( [[maybe_unused]] const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu ) const
        {
            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Prints out requested report count.
        /// @param offset   oa report start offset.
        /// @param count    oa report count.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void PrintReports(
            [[maybe_unused]] const uint32_t offset,
            [[maybe_unused]] const uint32_t count ) const
        {
        }
    };
} // namespace ML
