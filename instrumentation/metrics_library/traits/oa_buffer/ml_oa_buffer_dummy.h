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

@file ml_oa_buffer_dummy.h

@brief Dummy representation of oa buffer.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for OaBufferDummyTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct OaBufferDummyTrait
        {
            ML_DELETE_DEFAULT_COPY_AND_MOVE( OaBufferDummyTrait );

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief  Members.
            //////////////////////////////////////////////////////////////////////////
            const TT::Layouts::HwCounters::ReportOa m_OaReportDummy;

        public:
            //////////////////////////////////////////////////////////////////////////
            /// @brief OaBufferDummyTrait constructor.
            /// @param kernel   kernel interface.
            //////////////////////////////////////////////////////////////////////////
            OaBufferDummyTrait( const TT::KernelInterface& /*kernel*/ )
                : m_OaReportDummy{}
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
                ML_FUNCTION_LOG( StatusCode::Success );
                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Initializes oa buffer state kept by hw counters query.
            /// @param  reportGpu       query gpu report.
            /// @param  oaBufferState   oa buffer state to initialize.
            /// @return                 true if success.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool InitializeState(
                const TT::Layouts::HwCounters::Query::ReportGpu& /*reportGpu*/,
                TT::Layouts::OaBuffer::State& /*oaBufferState*/ ) const
            {
                return true;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Releases a reference to oa buffer.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Release() const
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Checks oa buffer state.
            /// @return true if oa buffer has valid state.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsValid() const
            {
                ML_FUNCTION_LOG( true );
                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Finds oa reports from oa buffer between query begin/end reports.
            /// @param  reportGpu       query gpu report.
            /// @param  oaBufferState   oa buffer state.
            /// @return                 oa reports count between query begin/end.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t FindOaWindow(
                const TT::Layouts::HwCounters::Query::ReportGpu& /*reportGpu*/,
                TT::Layouts::OaBuffer::State& /*oaBufferState*/ ) const
            {
                return 0;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa report from oa buffer.
            /// @param  index   oa report index within oa buffer.
            /// @return         reference to oa report.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE const TT::Layouts::HwCounters::ReportOa& GetReport( const uint32_t /*index*/ ) const
            {
                return m_OaReportDummy;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa report index collected by oa head registers.
            /// @param  address   address collected by oa head registers.
            /// @return           oa report index.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetReportIndex( const TT::Layouts::OaBuffer::HeadRegister /*address*/ ) const
            {
                return 0;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa report index collected by oa tail registers.
            /// @param  address   address collected by oa tail registers.
            /// @return           oa report index.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetReportIndex( const TT::Layouts::OaBuffer::TailRegister /*address*/ ) const
            {
                return 0;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Check whether oa buffer contains reports.
            /// @return true if oa buffer is empty.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsEmpty() const
            {
                ML_FUNCTION_LOG( true );
                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa reports count within oa buffer.
            /// @return oa reports count within oa buffer.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetReportsCount() const
            {
                ML_FUNCTION_LOG( uint32_t{ 0 } );
                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Collects oa reports from oa buffer.
            /// @param  endTimestamp    collected reports older than endTimestamp
            /// @return                 success if oa reports have been collected.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode CollectReports( const uint32_t /*endTimestamp*/ ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                return log.m_Result;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct OaBufferDummyTrait : BASE::OaBufferDummyTrait<T>
        {
            ML_DECLARE_TRAIT( OaBufferDummyTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct OaBufferDummyTrait : GEN9::OaBufferDummyTrait<T>
        {
            ML_DECLARE_TRAIT( OaBufferDummyTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct OaBufferDummyTrait : GEN11::OaBufferDummyTrait<T>
        {
            ML_DECLARE_TRAIT( OaBufferDummyTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
