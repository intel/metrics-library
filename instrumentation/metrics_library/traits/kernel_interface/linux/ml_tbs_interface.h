/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2023 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_tbs_interface.h

@brief Allows to enable, disable and use time-based sampling.
*/

#pragma once

namespace ML::BASE
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
            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            const TT::KernelInterface& m_Kernel;
            void*                      m_CpuAddress;
            uint32_t                   m_Size;
            uint32_t                   m_ReportSize;
            int32_t                    m_Stream;
            bool                       m_Mapped;

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Oa buffer constructor.
            /// @param  kernel kernel interface.
            //////////////////////////////////////////////////////////////////////////
            OaBufferMapped( const TT::KernelInterface& kernel )
                : m_Kernel( kernel )
                , m_CpuAddress( nullptr )
                , m_Size( 0 )
                , m_ReportSize( sizeof( TT::Layouts::HwCounters::ReportOa ) )
                , m_Stream( T::ConstantsOs::Tbs::m_Invalid )
                , m_Mapped( false )
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Oa buffer destructor.
            //////////////////////////////////////////////////////////////////////////
            ~OaBufferMapped()
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

                if( m_Mapped )
                {
                    log.Warning( "Oa Buffer is still mapped." );
                }
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Initializes oa buffer.
            /// @param  stream  tbs stream id.
            /// @return         success if oa buffer has been initialized.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Initialize( const int32_t stream )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
                ML_FUNCTION_CHECK( stream != T::ConstantsOs::Tbs::m_Invalid );

                m_Stream = stream;

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Checks oa buffer mapping is supported.
            /// @return true if oa buffer mapping is supported.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsSupported() const
            {
                const uint32_t     revision  = static_cast<uint32_t>( m_Kernel.m_Revision );
                constexpr uint32_t expected  = static_cast<uint32_t>( T::ConstantsOs::Drm::Revision::OaBufferMapping );
                const bool         supported = revision >= expected;

                return supported;
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
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
                ML_FUNCTION_CHECK( m_Stream != T::ConstantsOs::Tbs::m_Invalid );

                // Obtain oa buffer properties.
                log.m_Result = m_Kernel.m_IoControl.MapOaBuffer( m_Stream, m_CpuAddress, m_Size );

                // Validate obtain data.
                ML_FUNCTION_CHECK( log.m_Result );
                ML_FUNCTION_CHECK( m_Size > 0 );
                ML_FUNCTION_CHECK( m_CpuAddress != nullptr );

                m_Mapped = m_Size && m_CpuAddress;

                // Log collected data.
                log.Info( "Mapped      ", m_Mapped );
                log.Info( "Address cpu ", FormatFlag::Hexadecimal, FormatFlag::ShowBase, m_CpuAddress );
                log.Info( "Size        ", m_Size );

                return log.m_Result = ML_STATUS( m_Mapped );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Unmaps oa buffer.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Unmap()
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

                if( m_CpuAddress )
                {
                    munmap( m_CpuAddress, m_Size );
                }

                m_CpuAddress = nullptr;
                m_Size       = 0;
                m_Mapped     = false;
                m_Stream     = T::ConstantsOs::Tbs::m_Invalid;

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
            TbsStream( TT::KernelInterface& kernel )
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
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

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
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
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
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
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
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

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
                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

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
                std::vector<uint64_t> properties;

                ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
                ML_FUNCTION_CHECK( IsEnabled() == false );
                ML_FUNCTION_CHECK( m_MetricSet != T::ConstantsOs::Tbs::m_Invalid );
                ML_FUNCTION_CHECK( m_Kernel.m_Tbs.GetStreamProperties( properties, m_MetricSet ) );

                // Enable stream.
                do
                {
                    log.m_Result = m_Kernel.m_IoControl.OpenTbs( properties, m_Id );
                    log.m_Result = ML_STATUS( ML_SUCCESS( log.m_Result ) && IsEnabled() );
                }
                while( ML_FAIL( log.m_Result ) && ML_SUCCESS( m_Kernel.m_Tbs.UpdateStreamProperties( properties ) ) );

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
        TT::KernelInterface& m_Kernel;
        TT::IoControl&       m_IoControl;
        TbsReportsCache      m_ReportsCache;
        TbsStream            m_Stream;

        //////////////////////////////////////////////////////////////////////////
        /// @brief TbsInterfaceTrait constructor.
        /// @param kernel reference to kernel interface object.
        //////////////////////////////////////////////////////////////////////////
        TbsInterfaceTrait( TT::KernelInterface& kernel )
            : m_Kernel( kernel )
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
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            return log.m_Result = m_Stream.Initialize();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns mapped oa buffer.
        /// @param  type    oa buffer type.
        /// @return         mapped oa buffer.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE OaBufferMapped& GetOaBufferMapped( const TT::Layouts::OaBuffer::Type type )
        {
            switch( type )
            {
                case T::Layouts::OaBuffer::Type::Oa:
                    return m_Stream.m_OaBufferMapped;

                default:
                    ML_ASSERT_ALWAYS_ADAPTER( m_Kernel.m_Context.m_AdapterId );
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

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns tbs properties.
        /// @param  metricSet   metric set associated with tbs stream.
        /// @return properties  tbs properties.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetStreamProperties(
            std::vector<uint64_t>& properties,
            const int32_t          metricSet ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            auto addProperty = [&]( const uint64_t key, const uint64_t value )
            {
                properties.push_back( key );
                properties.push_back( value );
            };

            // clang-format off
            addProperty( DRM_I915_PERF_PROP_SAMPLE_OA,      true );
            addProperty( DRM_I915_PERF_PROP_OA_METRICS_SET, static_cast<uint64_t>( metricSet ) );
            addProperty( DRM_I915_PERF_PROP_OA_FORMAT,      T::TbsInterface::GetOaReportType() );
            addProperty( DRM_I915_PERF_PROP_OA_EXPONENT,    GetTimerPeriodExponent( T::ConstantsOs::Tbs::m_TimerPeriod ) );
            // clang-format on

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Increments engine instance in properties.
        /// @return properties updated stream properties.
        /// @return            operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode UpdateStreamProperties( [[maybe_unused]] std::vector<uint64_t>& properties ) const
        {
            return StatusCode::NotSupported;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa reports.
        /// @return oaReports output oa reports.
        /// @return           operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetOaReports( OaReports& oaReports )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

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
            ML_FUNCTION_LOG( uint32_t{ 0 }, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK_ERROR( m_IoControl.IsTbsEnabled(), 0 );

            constexpr size_t reportSize = sizeof( TbsOaReport );
            const size_t     dataSize   = reportSize * count;

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
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
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
            ML_FUNCTION_LOG( uint64_t{ 0 }, &m_Kernel.m_Context );

            // Get minimum gpu timestamp period in ns.
            const uint64_t timestampPeriod = GetGpuTimestampPeriod();
            if( !timestampPeriod )
            {
                log.Error( "Invalid gpu timestamp period (0 ns)" );
                return log.m_Result = 0;
            }

            // Compute timer exponent.
            const uint64_t period = log2( timerPeriod / timestampPeriod ) - 1;

            ML_ASSERT( period );
            return log.m_Result = period;
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct TbsInterfaceTrait : BASE::TbsInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( TbsInterfaceTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct TbsInterfaceTrait : GEN9::TbsInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( TbsInterfaceTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct TbsInterfaceTrait : GEN11::TbsInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( TbsInterfaceTrait, GEN11 );
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct TbsInterfaceTrait : XE_LP::TbsInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( TbsInterfaceTrait, XE_LP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Base types.
        //////////////////////////////////////////////////////////////////////////
        using Base::m_Kernel;

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa report type.
        /// @return oa report type status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static drm_i915_oa_format GetOaReportType()
        {
            return static_cast<drm_i915_oa_format>( I915_OA_FORMAT_A24u40_A14u32_B8_C8 );
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns tbs properties.
        /// @param  metricSet   metric set associated with tbs stream.
        /// @return properties  tbs properties.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetStreamProperties(
            std::vector<uint64_t>& properties,
            const int32_t          metricSet ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            uint32_t engineClass    = 0;
            uint32_t engineInstance = 0;
            auto&    subDevice      = m_Kernel.m_Context.m_SubDevice;
            auto     addProperty    = [&]( const uint64_t key, const uint64_t value )
            {
                properties.push_back( key );
                properties.push_back( value );
            };

            // Use base settings.
            Base::GetStreamProperties( properties, metricSet );

            // Special path for sub devices.
            ML_FUNCTION_CHECK( subDevice.GetTbsEngine( engineClass, engineInstance ) );

            // clang-format off
            addProperty( PRELIM_DRM_I915_PERF_PROP_OA_ENGINE_CLASS,    engineClass );
            addProperty( PRELIM_DRM_I915_PERF_PROP_OA_ENGINE_INSTANCE, engineInstance );
            // clang-format on

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Increments engine instance in properties.
        /// @return properties updated stream properties.
        /// @return            operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode UpdateStreamProperties( std::vector<uint64_t>& properties ) const
        {
            auto&    subDevice   = m_Kernel.m_Context.m_SubDevice;
            uint32_t engineClass = 0;

            for( uint32_t i = 0; i < properties.size() / 2; ++i )
            {
                if( properties[i * 2] == PRELIM_DRM_I915_PERF_PROP_OA_ENGINE_CLASS )
                {
                    engineClass = properties[i * 2 + 1];
                }
                else if( properties[i * 2] == PRELIM_DRM_I915_PERF_PROP_OA_ENGINE_INSTANCE )
                {
                    if( !subDevice.IsEngineAvailable( engineClass, ++properties[i * 2 + 1] ) )
                    {
                        return StatusCode::NotSupported;
                    }
                }
            }

            return StatusCode::Success;
        }
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct TbsInterfaceTrait : XE_HP::TbsInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( TbsInterfaceTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct TbsInterfaceTrait : XE_HPG::TbsInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( TbsInterfaceTrait, XE_HPG );
    };
} // namespace ML::XE_HPC
