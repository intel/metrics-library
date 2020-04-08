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

@file ml_tbs_interface.h

@brief Allows to enable, disable and use time-based sampling.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for tbs object to enable/disable/use time based
        ///        sampling on linux.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct TbsInterfaceTrait
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( TbsInterfaceTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( TbsInterfaceTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Base type for time based sampling report layout used by i915 perf.
            //////////////////////////////////////////////////////////////////////////
            struct TbsReport
            {
                TT::ConstantsOs::Tbs::ReportHeader m_ReportHeader;
                TT::Layouts::HwCounters::ReportOa  m_ReportOa;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Base type for kernel oa set used by metrics library.
            //////////////////////////////////////////////////////////////////////////
            struct KernelOaSet
            {
                int64_t     m_Id     = T::ConstantsOs::Tbs::m_Invalid; // Oa set id currently used by metrics library.
                std::string m_IdPath = "";                             // File location related to activated (by metrics discovery) oa set.
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Types.
            //////////////////////////////////////////////////////////////////////////
            using TbsReports = std::array<TbsReport, T::ConstantsOs::TbsContainer::m_RequestCount>;

            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
        private:
            const TT::KernelInterface& m_Kernel;               // Kernel interface used to communicate with the kernel.
            KernelOaSet                m_KernelOaSet;          // Kernel oa set used to enable tbs.
            TbsReports                 m_Reports;              // Temporary buffer for tbs reports.
            int32_t                    m_StreamFileDescriptor; // Tbs stream file descriptor.
            int32_t                    m_ReferenceCounter;     // Tbs reference counter. Each query that use the same
                                                               // oa set, increments reference counter. Another oa set can be
                                                               // used after get data. Therefore, if two queries use the same
                                                               // oa set and another query with a different oa set want use tbs,
                                                               // it have to wait for get data related to previous queries.

        public:
            //////////////////////////////////////////////////////////////////////////
            /// @brief TbsInterfaceTrait constructor.
            /// @param kernel   reference to kernel interface object.
            //////////////////////////////////////////////////////////////////////////
            TbsInterfaceTrait( const TT::KernelInterface& kernel )
                : m_Kernel( kernel )
                , m_KernelOaSet{}
                , m_Reports{}
                , m_StreamFileDescriptor( T::ConstantsOs::Tbs::m_Invalid )
                , m_ReferenceCounter( 0 )
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "TbsInterfaceTrait<Traits> (Linux)";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Initializes TbsInterfaceTrait data.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool Initialize()
            {
                Constants::String::Path path          = {};
                const uint32_t          drmCardNumber = m_Kernel.m_IoControl.m_DrmCardNumber;

                // File location that keeps activated (by metrics discovery) oa metrics set id.
                snprintf( path, sizeof( path ), T::ConstantsOs::Tbs::m_ActiveMetricSetPath, drmCardNumber, T::ConstantsOs::Tbs::m_ActiveMetricSetGuid );

                // Store path to activated (within kernel) metric set id.
                m_KernelOaSet.m_IdPath = path;

                return true;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns tbs state.
            /// @return true if tbs is enabled.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsEnabled() const
            {
                return m_StreamFileDescriptor != T::ConstantsOs::Tbs::m_Invalid;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief   Enables time based sampling.
            /// @param   oaMetricSetId  oa metric set id to use with tbs.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Enable( const uint32_t oaMetricSetId )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const bool validOaSet            = ML_SUCCESS( SetOaSet( oaMetricSetId ) ); // Validate provided oa set id.
                const bool validReferenceCounter = m_ReferenceCounter == 1;                 // Each valid oa set id increments reference counter.
                                                                                            // Tbs will be enabled just once for all valid (the same) oa sets.
                if( validOaSet && validReferenceCounter )
                {
                    // clang-format off
                    const uint64_t properties[] = { DRM_I915_PERF_PROP_SAMPLE_OA,       true,
                                                    DRM_I915_PERF_PROP_OA_METRICS_SET,  static_cast<uint64_t>( m_KernelOaSet.m_Id ),
                                                    DRM_I915_PERF_PROP_OA_FORMAT,       T::ConstantsOs::Tbs::m_ReportType,
                                                    DRM_I915_PERF_PROP_OA_EXPONENT,     GetTimerPeriodExponent( T::ConstantsOs::Tbs::m_TimerPeriod ) };
                    // clang-format on

                    drm_i915_perf_open_param parameters = {};
                    parameters.flags                    = I915_PERF_FLAG_FD_CLOEXEC | I915_PERF_FLAG_FD_NONBLOCK;
                    parameters.num_properties           = ML_ARRAY_SIZE( properties ) / 2;
                    parameters.properties_ptr           = (uintptr_t) properties;

                    m_StreamFileDescriptor = m_Kernel.m_IoControl.Send( DRM_IOCTL_I915_PERF_OPEN, parameters );
                    log.Debug( "Tbs stream file descriptor", m_StreamFileDescriptor );

                    return log.m_Result = ( m_StreamFileDescriptor != T::ConstantsOs::Tbs::m_Invalid )
                        ? StatusCode::Success
                        : StatusCode::TbsUnableToEnable;
                }
                else if( validOaSet && IsEnabled() )
                {
                    log.Debug( "Tbs is already enabled for the same oa set id (stream file descriptor / oa set id)", m_StreamFileDescriptor, m_KernelOaSet.m_Id );
                    log.m_Result = StatusCode::Success;
                }
                else
                {
                    log.Debug( "Unable to enable tbs (another oa set id use it).", m_KernelOaSet.m_Id );
                    log.m_Result = StatusCode::TbsUnableToEnable;
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Forces library to disable tbs. For normal usage scenario tbs
            ///         should be disabled when there is no query to use it.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Disable()
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                if( m_StreamFileDescriptor != T::ConstantsOs::Tbs::m_Invalid )
                {
                    close( m_StreamFileDescriptor );
                }

                m_KernelOaSet.m_Id     = T::ConstantsOs::Tbs::m_Invalid;
                m_StreamFileDescriptor = T::ConstantsOs::Tbs::m_Invalid;
                m_ReferenceCounter     = 0;

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Decreases reference counter related to current oa metric set id
            ///         used by tbs (usually after query get data). Tbs will be disabled
            ///         if there is no oa metric set to use it.
            /// @param  oaMetricSetId   oa metric set to release.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Release( const uint32_t oaMetricSetId )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const bool validOaSet      = ReleaseOaSet( oaMetricSetId );
                const bool validReleaseTbs = validOaSet && ( m_ReferenceCounter == 0 );

                if( validReleaseTbs )
                {
                    Disable();
                }

                return log.m_Result = validReleaseTbs
                    ? StatusCode::Success
                    : StatusCode::Failed;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns activated (by metrics discovery) oa metrics set
            ///         identifier from the kernel.
            /// @return oa metrics set id.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint64_t GetKernelOaSetId() const
            {
                ML_FUNCTION_LOG( uint64_t{ 0 } );

                return ML_SUCCESS( ReadFile( m_KernelOaSet.m_IdPath, log.m_Result ) )
                    ? log.m_Result
                    : T::ConstantsOs::Tbs::m_Invalid;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa reports.
            /// @return oaReports   output oa reports.
            /// @param  count       oa reports count to collect.
            /// @return completed   true if all oa reports has been received.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetOaReports(
                TT::Layouts::HwCounters::ReportOa* oaReports,
                uint32_t&                          count,
                bool&                              completed ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( oaReports != nullptr );

                // Read tbs data.
                const int32_t readBytes = ReadReports( count );

                if( readBytes < 0 )
                {
                    if( errno == EAGAIN )
                    {
                        count     = 0;
                        completed = true;
                        log.Debug( "i915 performance stream data not available yet" );
                        return log.m_Result = StatusCode::Success;
                    }

                    log.Error( "ERROR: Reading i915 stream failed", errno, strerror( errno ) );
                    return log.m_Result = StatusCode::TbsUnableToRead;
                }

                return log.m_Result = ProcessReports( m_Reports.data(), readBytes, oaReports, count, completed );
            }

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief  Sets a new oa metric set to use for tbs. Three cases are allowed:
            ///         1) library does not use any oa metric set.
            ///         2) new oa metric set is equal to used one.
            ///         3) new oa metric set is different than current one (error)
            /// @param  oaMetricSetId oa metric set to use.
            /// @return               operation status.
            ///
            ///        Query usage scenario on Linux:
            ///
            ///        First Oa MetricSet:
            ///             MD::OaMetricSet[ 0 ]::Activate
            ///             OGL::CreateQuery
            ///             OGL::BeginQuery    ->  enable tbs
            ///             OGL::EndQuery
            ///             OGL::GetData
            ///
            ///        The same Oa MetricSet:
            ///             MD::OaMetricSet[ 0 ]::Activate
            ///             OGL::CreateQuery
            ///             OGL::BeginQuery    -> reuse current tbs instance
            ///             OGL::EndQuery
            ///             OGL::GetData
            ///
            ///        Second OaMetricSet
            ///             MD::OaMetricSet[ 1 ]::Activate
            ///             OGL::CreateQuery
            ///             OGL::BeginQuery    -> reenable tbs to use new oa set id
            ///             OGL::EndQuery
            ///             OGL::GetData
            ///
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode SetOaSet( const uint32_t oaMetricSetId )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const bool isFirstOaSetToUse = m_KernelOaSet.m_Id == T::ConstantsOs::Tbs::m_Invalid;
                const bool isSameOaSetToUse  = m_KernelOaSet.m_Id == oaMetricSetId;

                if( isFirstOaSetToUse )
                {
                    // The first metric set used by metrics library.
                    m_ReferenceCounter++;
                    m_KernelOaSet.m_Id = oaMetricSetId;
                    log.Debug( "First oa metric set (id)", m_KernelOaSet.m_Id );
                }
                else if( isSameOaSetToUse )
                {
                    // The same metric set already in use in library.
                    m_ReferenceCounter++;
                    log.Debug( "Subsequent oa metric set (oa set id/reference counter)", m_KernelOaSet.m_Id, m_ReferenceCounter );
                }
                else
                {
                    log.m_Result = StatusCode::Failed;
                    log.Error( "Unable to use oa set id (another oa set is in use).", oaMetricSetId, m_KernelOaSet.m_Id );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Releases oa metric set. When reference counter becomes zero,
            ///         another oa metrics set can be set.
            /// @param  metricSetId oa metric set to release.
            /// @return             true if oa metric set has been released.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool ReleaseOaSet( const uint32_t oaMetricSetId )
            {
                ML_FUNCTION_LOG( m_KernelOaSet.m_Id == oaMetricSetId );

                if( log.m_Result )
                {
                    // Decrease reference counter.
                    m_ReferenceCounter = std::max( 0, --m_ReferenceCounter );

                    // Referance counter == 0 then release current oa metric set.
                    m_KernelOaSet.m_Id = m_ReferenceCounter
                        ? m_KernelOaSet.m_Id
                        : T::ConstantsOs::Tbs::m_Invalid;

                    log.Debug( "Currently used oa metric set has been released (id/reference counter).", oaMetricSetId, m_ReferenceCounter );
                }
                else
                {
                    log.Debug( "Provided oa metric set is not currently used by metrics library (id used/provided).", m_KernelOaSet.m_Id, oaMetricSetId );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief   Reads tbs reports from the stream file descriptor.
            /// @param   count  tbs reports count to read.
            /// @return         read data in bytes.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE int32_t ReadReports( const uint32_t count ) const
            {
                ML_FUNCTION_LOG( int32_t( 0 ) );

                const size_t reportSize = sizeof( TbsReport );                                        // drm_i915_perf_record_header + oa report.
                const size_t dataSize   = reportSize * count + sizeof( drm_i915_perf_record_header ); // Adding header for flag-only reports, e.g. for situations where
                                                                                                      // user requests 1 report, but first report from Perf is
                                                                                                      // REPORT_LOST flag (which consists of header only).

                ML_ASSERT( m_StreamFileDescriptor != T::ConstantsOs::Tbs::m_Invalid );
                ML_ASSERT( count <= m_Reports.size() );
                ML_ASSERT( reportSize );
                ML_ASSERT( dataSize );

                log.Debug( "Tbs stream id", m_StreamFileDescriptor );
                log.Debug( "Tbs report size", reportSize );
                log.Debug( "Tbs data size to read", dataSize );

                const int32_t readBytes = read( m_StreamFileDescriptor, m_Reports.data(), dataSize );
                log.Debug( "Read bytes / reports", readBytes, readBytes / reportSize );

                return log.m_Result = readBytes;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief   Reads data from file.
            /// @param   path   file path.
            /// @return  data   data to read.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename Data>
            ML_INLINE StatusCode ReadFile(
                const std::string& path,
                Data&              data ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                Constants::String::Buffer buffer    = {};
                int32_t                   file      = open( path.c_str(), O_RDONLY );
                int32_t                   readBytes = 0;

                if( file < 0 )
                {
                    log.Error( "Failed to open file", path, errno, strerror( errno ) );
                    return log.m_Result = StatusCode::Failed;
                }

                readBytes = read( file, buffer, sizeof( buffer ) - 1 );

                if( readBytes < 0 )
                {
                    close( file );
                    log.Error( "Read negative number of bytes, error %s", strerror( errno ) );
                    return log.m_Result = StatusCode::Failed;
                }

                buffer[readBytes] = '\0';
                data              = strtoull( buffer, 0, 0 );

                close( file );
                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Transforms linux kernel tbs data to the original oa buffer format.
            /// @param  tbsReports         tbs reports to process.
            /// @param  tbsReportsSize     tbs reports data size.
            /// @return oaReports          oa reports.
            /// @return oaReportsCount     a reports count.
            /// @return completed          true if all oa reports has been received.
            /// @return                    operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode ProcessReports(
                const TbsReport*                   tbsReports,
                const int32_t                      tbsReportsSize,
                TT::Layouts::HwCounters::ReportOa* oaReports,
                uint32_t&                          oaReportsCount,
                bool&                              completed ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_ASSERT( tbsReports );
                ML_ASSERT( tbsReportsSize );

                // Oa data.
                const uint32_t oaReportSize  = sizeof( TT::Layouts::HwCounters::ReportOa );
                uint32_t       oaReportIndex = 0;

                // Tbs data.
                const uint32_t tbsReportSize   = sizeof( TbsReport );
                uint32_t       tbsReportOffset = 0;

                while( tbsReportOffset < tbsReportsSize )
                {
                    const auto tbsReport = (const TbsReport*) ( (uint8_t*) m_Reports.data() + tbsReportOffset );
                    const auto tbsHeader = &tbsReport->m_ReportHeader;

                    // Validate tbs report header.
                    if( tbsHeader->size == 0 )
                    {
                        log.Error( "Empty tbs header" );
                        return log.m_Result = StatusCode::TbsUnableToRead;
                    }

                    // Process tbs header.
                    switch( tbsHeader->type )
                    {
                        case DRM_I915_PERF_RECORD_SAMPLE:
                            if( tbsHeader->size != tbsReportSize )
                            {
                                log.Error( "Invalid oa report size" );
                                return log.m_Result = StatusCode::TbsUnableToRead;
                            }

                            oaReports[oaReportIndex++] = tbsReport->m_ReportOa;
                            break;

                        case DRM_I915_PERF_RECORD_OA_REPORT_LOST:
                            log.Debug( "Oa report lost" );
                            break;

                        case DRM_I915_PERF_RECORD_OA_BUFFER_LOST:
                            log.Debug( "Oa buffer lost" );
                            break;

                        default:
                            log.Error( "Unknown header type", tbsHeader->type );
                            return log.m_Result = StatusCode::TbsUnableToRead;
                    }

                    // Advance data offset.
                    tbsReportOffset += tbsHeader->size;

                    ML_ASSERT( tbsReportOffset <= tbsReportsSize );
                    ML_ASSERT( oaReportIndex <= oaReportsCount );
                }

                completed      = oaReportIndex < oaReportsCount;
                oaReportsCount = oaReportIndex;

                log.Debug( "Tbs read data completed", completed );
                log.Debug( "Tbs read oa reports count", oaReportsCount );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns minimum gpu timestamp period is nanoseconds based on
            ///         gpu timestamp frequency.
            /// @return gpu timestamp period in ns.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint64_t GetGpuTimestampPeriod() const
            {
                return Constants::Time::m_SecondInNanoseconds / m_Kernel.GetGpuTimestampFrequency();
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Changes the given timer period in nanoseconds to the oa timer
            ///         period exponent. Periods are rounded down to the nearest exponent.
            ///         Based on documentation:
            ///         StrobePeriod = MinimumTimeStampPeriod * 2^( TimerExponent + 1 ).
            /// @param  timerPeriod timer period in nanoseconds.
            /// @return             timer period exponent
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint64_t GetTimerPeriodExponent( const uint64_t timerPeriod ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Get minimum gpu timestamp period in ns.
                const uint64_t timestampPeriod = GetGpuTimestampPeriod();
                if( !timestampPeriod )
                {
                    log.Error( "Invalid gpu timestamp period (0 ns)" );
                    return 0;
                }

                // Compute timer exponent.
                const uint64_t period = log2( timerPeriod / timestampPeriod ) - 1;
                log.Debug( "Tbs timer period exponent", period );

                ML_ASSERT( period );
                return period;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct TbsInterfaceTrait : BASE::TbsInterfaceTrait<T>
        {
            ML_DECLARE_TRAIT( TbsInterfaceTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct TbsInterfaceTrait : GEN9::TbsInterfaceTrait<T>
        {
            ML_DECLARE_TRAIT( TbsInterfaceTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct TbsInterfaceTrait : GEN11::TbsInterfaceTrait<T>
        {
            ML_DECLARE_TRAIT( TbsInterfaceTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
