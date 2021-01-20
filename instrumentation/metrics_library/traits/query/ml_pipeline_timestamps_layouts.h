/******************************************************************************\

Copyright © 2021, Intel Corporation

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

@file ml_pipeline_timestamps_layouts.h

@brief Layout for pipeline timestamps.

\******************************************************************************/

#pragma once

ML_STRUCTURE_PACK_BEGIN( 1 );

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for PipelineTimestampsLayoutsTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct PipelineTimestampsLayoutsTrait
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( PipelineTimestampsLayoutsTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( PipelineTimestampsLayoutsTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "PipelineTimestampsLayoutsTrait<Traits>";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Api report format.
            //////////////////////////////////////////////////////////////////////////
            struct ReportApi
            {
                uint64_t m_BeginTimestamp;    // Store register memory timestamp gathered on query begin.
                uint64_t m_EndTimestampEnter; // Store register memory timestamp gathered on query end enter.
                uint64_t m_EopTimestamp;      // End of pipe timestamp.
                uint64_t m_EndTimestampExit;  // Store register memory timestamp gathered on query end exit.

                uint64_t m_QueryInfo;         // Query execution information flags.
                uint32_t m_ReportId;
                uint32_t m_ReportsCount;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Pipeline timestamps.
            //////////////////////////////////////////////////////////////////////////
            struct Timestamps
            {
                uint64_t m_Begin;         // Store register memory timestamp gathered on query begin.
                uint64_t m_EndEnter;      // Store register memory timestamp gathered on query end enter.
                uint64_t m_EndOfPipeline; // End of pipe timestamp.
                uint64_t m_EndExit;       // Store register memory timestamp gathered on query end exit.
                uint64_t m_Info;          // Query execution information flags.
                uint64_t m_EndTag;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Gpu report format.
            //////////////////////////////////////////////////////////////////////////
            struct ReportGpu
            {
                Timestamps m_TimestampsRender;
            };
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct PipelineTimestampsLayoutsTrait : BASE::PipelineTimestampsLayoutsTrait<T>
        {
            ML_DECLARE_TRAIT( PipelineTimestampsLayoutsTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct PipelineTimestampsLayoutsTrait : GEN9::PipelineTimestampsLayoutsTrait<T>
        {
            ML_DECLARE_TRAIT( PipelineTimestampsLayoutsTrait, GEN9 );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Gpu report format.
            //////////////////////////////////////////////////////////////////////////
            struct ReportGpu
            {
                typename Base::Timestamps m_TimestampsRender;
                typename Base::Timestamps m_TimestampsPosh;
            };
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct PipelineTimestampsLayoutsTrait : GEN11::PipelineTimestampsLayoutsTrait<T>
        {
            ML_DECLARE_TRAIT( PipelineTimestampsLayoutsTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML

ML_STRUCTURE_PACK_END();
