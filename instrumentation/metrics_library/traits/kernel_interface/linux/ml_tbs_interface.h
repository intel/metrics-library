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
            /// @brief Tbs oa report layout used by linux kernel.
            //////////////////////////////////////////////////////////////////////////
            struct TbsOaReport
            {
                drm_i915_perf_record_header       m_Header;
                TT::Layouts::HwCounters::ReportOa m_Report;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Base type for mapped oa buffer object.
            //////////////////////////////////////////////////////////////////////////
            struct OaBufferMapped
            {
                const TT::KernelInterface&         m_Kernel;
                TT::Layouts::HwCounters::ReportOa* m_Reports;
                void*                              m_CpuAddress;
                uint32_t                           m_ReportsCount;
                uint32_t                           m_ReportSize;
                uint32_t                           m_Size;
                int32_t                            m_Stream;
                bool                               m_Mapped;

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Oa buffer constructor.
                /// @param  kernel kernel interface.
                //////////////////////////////////////////////////////////////////////////
                OaBufferMapped( const TT::KernelInterface& kernel )
                    : m_Kernel( kernel )
                    , m_Reports( nullptr )
                    , m_CpuAddress( nullptr )
                    , m_ReportsCount( 0 )
                    , m_ReportSize( sizeof( TT::Layouts::HwCounters::ReportOa ) )
                    , m_Size( 0 )
                    , m_Stream( T::ConstantsOs::Tbs::m_Invalid )
                    , m_Mapped( false )
                {
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Oa buffer destructor.
                //////////////////////////////////////////////////////////////////////////
                ~OaBufferMapped()
                {
                    ML_ASSERT( m_Mapped == false );
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Initializes oa buffer.
                /// @param  stream  tbs stream id.
                /// @return         success if oa buffer has been initialized.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE StatusCode Initialize( const int32_t stream )
                {
                    ML_FUNCTION_LOG( StatusCode::Success );
                    ML_FUNCTION_CHECK( stream != T::ConstantsOs::Tbs::m_Invalid );

                    m_Stream = stream;

                    return log.m_Result;
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Checks if oa buffer is mapped.
                /// @return true if oa buffer is mapped.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE bool IsMapped() const
                {
                    return m_Mapped;
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Obtains cpu/gpu addresses of oa buffer.
                /// @return operation status.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE StatusCode Map()
                {
                    ML_FUNCTION_LOG( StatusCode::Success );
                    ML_FUNCTION_CHECK( m_Stream != T::ConstantsOs::Tbs::m_Invalid );

                    // Obtain oa buffer properties.
                    log.m_Result = m_Kernel.m_IoControl.MapOaBuffer( m_Stream, m_CpuAddress, m_Size );

                    // Validate obtain data.
                    ML_FUNCTION_CHECK( log.m_Result );
                    ML_FUNCTION_CHECK( m_Size > 0 );
                    ML_FUNCTION_CHECK( m_CpuAddress != nullptr );

                    m_Reports      = reinterpret_cast<TT::Layouts::HwCounters::ReportOa*>( m_CpuAddress );
                    m_ReportsCount = m_Size / m_ReportSize;
                    m_Mapped       = m_Size && m_CpuAddress;

                    // Log collected data.
                    log.Info( "Mapped        ", m_Mapped );
                    log.Info( "Address cpu   ", FormatFlag::Hexadecimal, FormatFlag::ShowBase, m_CpuAddress );
                    log.Info( "Size          ", m_Size );
                    log.Info( "Reports count ", m_ReportsCount );

                    return log.m_Result = ML_STATUS( m_Mapped );
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Unmaps oa buffer.
                /// @return operation status.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE StatusCode Unmap()
                {
                    ML_FUNCTION_LOG( StatusCode::Success );
                    ML_FUNCTION_CHECK( m_Mapped );

                    if( m_CpuAddress )
                    {
                        munmap( m_CpuAddress, m_Size );
                    }

                    m_CpuAddress   = nullptr;
                    m_Reports      = nullptr;
                    m_ReportsCount = 0;
                    m_Size         = 0;
                    m_Mapped       = false;
                    m_Stream       = T::ConstantsOs::Tbs::m_Invalid;

                    return log.m_Result;
                }
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Tbs stream data.
            //////////////////////////////////////////////////////////////////////////
            struct TbsStream
            {
                TT::KernelInterface& m_Kernel;
                int32_t              m_Id;
                int32_t              m_MetricSet;
                bool                 m_MetricSetInternal;
                OaBufferMapped       m_OaBufferMapped;

                //////////////////////////////////////////////////////////////////////////
                /// @brief Tbs stream constructor.
                /// @param kernel kernel interface.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE TbsStream( TT::KernelInterface& kernel )
                    : m_Kernel( kernel )
                    , m_Id( T::ConstantsOs::Tbs::m_Invalid )
                    , m_MetricSet( T::ConstantsOs::Tbs::m_Invalid )
                    , m_MetricSetInternal( false )
                    , m_OaBufferMapped( kernel )
                {
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Initializes tbs stream.
                /// @return initialization status.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE StatusCode Initialize()
                {
                    ML_FUNCTION_LOG( StatusCode::Success );

                    if( m_Kernel.m_Context.m_ClientOptions.m_TbsEnabled )
                    {
                        // Do not initialize tbs if metrics library was opened to support
                        // umd driver that also uses tbs.
                        // For example L0 may open metric tracer (tbs) and gpu commands
                        // may be needed for stream markers. For such case metrics library,
                        // cannot open tbs internally. It would block metric tracer to use tbs.
                        // Application should reinitialize metrics library to use query again.
                        log.Debug( "Another application is using tbs, tbs will not be used." );
                        return log.m_Result;
                    }

                    // Try to obtain metric set activated by metrics discovery.
                    m_MetricSet = m_Kernel.m_IoControl.GetKernelMetricSet();

                    // Otherwise, create an internal metric set to enable tbs.
                    if( m_MetricSet == T::ConstantsOs::Tbs::m_Invalid )
                    {
                        m_MetricSet         = m_Kernel.m_IoControl.CreateMetricSet();
                        m_MetricSetInternal = m_MetricSet != T::ConstantsOs::Tbs::m_Invalid;
                    }

                    return log.m_Result = Enable();
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Returns tbs state.
                /// @return true if tbs is enabled.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE bool IsEnabled() const
                {
                    return m_Id != T::ConstantsOs::Tbs::m_Invalid;
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Allows to use a new metric set by gpu.
                /// @param  set metric set id.
                /// @return     operation status.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE StatusCode SetMetricSet( int32_t set )
                {
                    ML_FUNCTION_LOG( StatusCode::Success );
                    ML_FUNCTION_CHECK( IsEnabled() );

                    log.Debug( "Used set ", m_MetricSet );
                    log.Debug( "New set  ", set );

                    log.m_Result = m_Kernel.m_IoControl.SetTbsMetricSet( m_Id, set );
                    m_MetricSet  = ML_SUCCESS( log.m_Result ) ? set : m_MetricSet;

                    log.Debug( "Current set ", m_MetricSet );

                    return log.m_Result;
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Releases metric set usage.
                /// @param  set metric set id to release.
                /// @return     operation status.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE StatusCode ReleaseMetricSet( const int32_t set )
                {
                    ML_FUNCTION_LOG( StatusCode::Success );
                    ML_ASSERT( m_MetricSet == set );

                    if( m_MetricSetInternal )
                    {
                        m_Kernel.m_IoControl.RemoveMetricSet( m_MetricSet );
                        m_MetricSetInternal = false;
                    }

                    m_MetricSet = T::ConstantsOs::Tbs::m_Invalid;

                    return log.m_Result;
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Disables tbs stream.
                /// @return operation status.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE StatusCode Disable()
                {
                    ML_FUNCTION_LOG( StatusCode::Success );

                    if( !m_Kernel.m_Context.m_ClientOptions.m_TbsEnabled )
                    {
                        ML_FUNCTION_CHECK( IsEnabled() );

                        // Release used metric set.
                        ReleaseMetricSet( m_MetricSet );

                        // Close stream.
                        m_Kernel.m_IoControl.CloseTbs( m_Id );
                        m_Id = T::ConstantsOs::Tbs::m_Invalid;
                    }

                    return log.m_Result;
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Restarts tbs stream.
                /// @return operation status.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE StatusCode Restart()
                {
                    ML_FUNCTION_LOG( StatusCode::Success );

                    // Disable stream.
                    Disable();

                    // Try to obtain metric set activated by metrics discovery.
                    m_MetricSet = m_Kernel.m_IoControl.GetKernelMetricSet();

                    // Enable stream.
                    return log.m_Result = Enable();
                }

            private:
                /////////////////////////////////////////////////////////////////////////
                /// @brief  Enables tbs stream for a given metric set.
                /// @return operation status.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE StatusCode Enable()
                {
                    ML_FUNCTION_LOG( StatusCode::Success );
                    ML_FUNCTION_CHECK( IsEnabled() == false );
                    ML_FUNCTION_CHECK( m_MetricSet != T::ConstantsOs::Tbs::m_Invalid );

                    // Fill stream properties.
                    std::vector<uint64_t> properties;
                    GetProperties( properties );

                    // Enable stream.
                    log.m_Result = m_Kernel.m_IoControl.OpenTbs( properties, m_Id );
                    log.m_Result = ML_STATUS( ML_SUCCESS( log.m_Result ) && IsEnabled() );

                    // Initialize oa buffer mapping if supported.
                    if( ML_SUCCESS( log.m_Result ) )
                    {
                        m_OaBufferMapped.Initialize( m_Id );
                    }

                    // Disable an internal metric set used to enable tbs.
                    // It will allow metrics discovery to enable another metrics set.
                    if( m_MetricSetInternal )
                    {
                        ReleaseMetricSet( m_MetricSet );
                    }

                    return log.m_Result;
                }

                /////////////////////////////////////////////////////////////////////////
                /// @brief  Returns tbs properties.
                /// @return properties tbs properties.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE void GetProperties( std::vector<uint64_t>& properties ) const
                {
                    auto addProperty = [&]( const uint64_t key, const uint64_t value ) {
                        properties.push_back( key );
                        properties.push_back( value );
                    };

                    // clang-format off
                    addProperty( DRM_I915_PERF_PROP_SAMPLE_OA,      true );
                    addProperty( DRM_I915_PERF_PROP_OA_METRICS_SET, static_cast<uint64_t>( m_MetricSet ) );
                    addProperty( DRM_I915_PERF_PROP_OA_FORMAT,      T::TbsInterface::GetOaReportType() );
                    addProperty( DRM_I915_PERF_PROP_OA_EXPONENT,    m_Kernel.m_Tbs.GetTimerPeriodExponent( T::ConstantsOs::Tbs::m_TimerPeriod ) );
                    // clang-format on
                }
            };

        public:
            //////////////////////////////////////////////////////////////////////////
            /// @brief Tbs/oa reports containers.
            //////////////////////////////////////////////////////////////////////////
            using OaReports       = std::vector<TT::Layouts::HwCounters::ReportOa>;
            using TbsReportsCache = std::array<TbsOaReport, T::ConstantsOs::Tbs::m_CacheCapacity>;

            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            TT::KernelInterface&           m_Kernel;
            TT::ConstantsOs::Tbs::Revision m_Revision;
            TT::IoControl&                 m_IoControl;
            TbsReportsCache                m_ReportsCache;
            TbsStream                      m_Stream;

            //////////////////////////////////////////////////////////////////////////
            /// @brief TbsInterfaceTrait constructor.
            /// @param kernel reference to kernel interface object.
            //////////////////////////////////////////////////////////////////////////
            TbsInterfaceTrait( TT::KernelInterface& kernel )
                : m_Kernel( kernel )
                , m_Revision( T::ConstantsOs::Tbs::Revision::Unsupported )
                , m_IoControl( kernel.m_IoControl )
                , m_ReportsCache{}
                , m_Stream( kernel )
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief TbsInterfaceTrait destructor.
            //////////////////////////////////////////////////////////////////////////
            ~TbsInterfaceTrait()
            {
                m_Stream.Disable();
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
            /// @brief  Initializes tbs interface.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Initialize()
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Obtain kernel performance interface revision.
                m_IoControl.GetTbsRevision( m_Revision );

                // Enable tbs stream.
                return log.m_Result = m_Stream.Initialize();
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns mapped oa buffer.
            /// @param  type    oa buffer type.
            /// @return         mapped oa buffer.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE OaBufferMapped& GetOaBufferMapped(
                const TT::Layouts::OaBuffer::Type type )
            {
                switch( type )
                {
                    case T::Layouts::OaBuffer::Type::Oa:
                        return m_Stream.m_OaBufferMapped;

                    default:
                        ML_ASSERT_ALWAYS();
                        break;
                }

                return m_Stream.m_OaBufferMapped;
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa report type.
            /// @return oa report type status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static drm_i915_oa_format GetOaReportType()
            {
                return I915_OA_FORMAT_A32u40_A4u32_B8_C8;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa reports.
            /// @return oaReports output oa reports.
            /// @return           operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetOaReports( OaReports& oaReports )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const int32_t readBytes = ReadReports( oaReports.capacity() );

                if( readBytes < 0 )
                {
                    if( errno == EAGAIN )
                    {
                        log.Debug( "i915 performance stream data not available yet" );
                        return log.m_Result = StatusCode::Success;
                    }

                    log.Error( "Reading i915 stream failed", errno, strerror( errno ) );
                    return log.m_Result = StatusCode::TbsUnableToRead;
                }

                return log.m_Result = ProcessReports( readBytes, oaReports );
            }

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief  Reads tbs reports from the stream file descriptor.
            /// @param  count tbs reports count to read.
            /// @return       read data in bytes.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t ReadReports( const uint32_t count )
            {
                ML_FUNCTION_LOG( uint32_t{ 0 } );
                ML_FUNCTION_CHECK_ERROR( m_IoControl.IsTbsEnabled(), 0 );

                const size_t reportSize = sizeof( TbsOaReport );
                const size_t dataSize   = reportSize * count;

                ML_ASSERT( count <= m_ReportsCache.size() );
                ML_ASSERT( reportSize );
                ML_ASSERT( dataSize );

                log.Debug( "Tbs report size  ", reportSize );
                log.Debug( "Tbs size to read ", dataSize );

                return log.m_Result = m_IoControl.ReadTbs( m_ReportsCache, dataSize );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Transforms linux kernel tbs data to the original oa buffer format.
            /// @param  tbsDataSize tbs reports data size in bytes.
            /// @return oaReports   oa reports.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode ProcessReports(
                const uint32_t tbsDataSize,
                OaReports&     oaReports )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_ASSERT( tbsDataSize > 0 );

                uint32_t tbsDataOffset = 0;

                while( tbsDataOffset < tbsDataSize )
                {
                    const uint8_t* tbsData     = reinterpret_cast<const uint8_t*>( m_ReportsCache.data() );
                    const auto     tbsHeader   = reinterpret_cast<const drm_i915_perf_record_header*>( tbsData + tbsDataOffset );
                    const auto     tbsOaReport = reinterpret_cast<const TbsOaReport*>( tbsData + tbsDataOffset );

                    // Validate header.
                    ML_FUNCTION_CHECK( tbsHeader->size != 0 );

                    // Process each tbs report type.
                    switch( tbsHeader->type )
                    {
                        case DRM_I915_PERF_RECORD_SAMPLE:
                            ML_FUNCTION_CHECK( tbsHeader->size == sizeof( TbsOaReport ) );
                            ML_ASSERT( oaReports.size() < oaReports.capacity() );
                            oaReports.push_back( tbsOaReport->m_Report );
                            break;

                        case DRM_I915_PERF_RECORD_OA_REPORT_LOST:
                            log.Error( "Oa report lost" );
                            break;

                        case DRM_I915_PERF_RECORD_OA_BUFFER_LOST:
                            log.Error( "Oa buffer lost" );
                            break;

                        default:
                            log.Warning( "Unknown header type", tbsHeader->type );
                            break;
                    }

                    // Advance data offset.
                    tbsDataOffset += tbsHeader->size;
                    ML_ASSERT( tbsDataOffset <= tbsDataSize );
                }

                log.Debug( "Oa reports count", oaReports.size() );
                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns minimum gpu timestamp period is nanoseconds based on
            ///         gpu timestamp frequency.
            /// @return gpu timestamp period in ns.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint64_t GetGpuTimestampPeriod() const
            {
                return Constants::Time::m_SecondInNanoseconds / m_Kernel.GetGpuTimestampFrequency( T::Layouts::Configuration::TimestampType::Oa );
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
