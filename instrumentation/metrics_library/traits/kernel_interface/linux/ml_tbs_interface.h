/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

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
    struct TbsInterfaceTrait : TraitObject<T, TT::TbsInterface>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( TbsInterfaceTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( TbsInterfaceTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = TraitObject<T, TT::TbsInterface>;
        using Base::Derived;

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
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::KernelInterface& m_Kernel;
        TT::IoControl&       m_IoControl;
        OaBufferMapped       m_OaBufferMapped;
        TT::TbsStream        m_Stream;

        //////////////////////////////////////////////////////////////////////////
        /// @brief TbsInterfaceTrait constructor.
        /// @param kernel reference to kernel interface object.
        //////////////////////////////////////////////////////////////////////////
        TbsInterfaceTrait( TT::KernelInterface& kernel )
            : m_Kernel( kernel )
            , m_IoControl( kernel.m_IoControl )
            , m_OaBufferMapped( kernel )
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
            if( type != T::Layouts::OaBuffer::Type::Oa )
            {
                ML_ASSERT_ALWAYS_ADAPTER( m_Kernel.m_Context.m_AdapterId );
            }

            return m_OaBufferMapped;
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

    protected:
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

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns minimum gpu timestamp period is nanoseconds based on
        ///         gpu timestamp frequency.
        /// @return gpu timestamp period in ns.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t GetGpuTimestampPeriod() const
        {
            return Constants::Time::m_SecondInNanoseconds / m_Kernel.GetGpuTimestampFrequency( T::Layouts::Configuration::TimestampType::Oa );
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct TbsInterfaceTrait : BASE::TbsInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( TbsInterfaceTrait, BASE );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::Derived;
        using Base::GetTimerPeriodExponent;
        using Base::m_Kernel;

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns tbs properties.
        /// @param  metricSet   metric set associated with tbs stream.
        /// @return properties  tbs properties.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetStreamProperties(
            std::vector<uint64_t>& properties,
            const int32_t          metricSet )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            auto addProperty = [&]( const uint64_t key, const uint64_t value )
            {
                properties.push_back( key );
                properties.push_back( value );
            };

            addProperty( DRM_I915_PERF_PROP_SAMPLE_OA, true );
            addProperty( DRM_I915_PERF_PROP_OA_METRICS_SET, static_cast<uint64_t>( metricSet ) );
            addProperty( DRM_I915_PERF_PROP_OA_FORMAT, Derived().GetOaReportType() );
            addProperty( DRM_I915_PERF_PROP_OA_EXPONENT, GetTimerPeriodExponent( T::ConstantsOs::Tbs::m_TimerPeriod ) );

            return log.m_Result;
        }

    protected:
        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa report type.
        /// @return oa report type status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE drm_i915_oa_format GetOaReportType() const
        {
            return I915_OA_FORMAT_A32u40_A4u32_B8_C8;
        }
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
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::m_Kernel;

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa report type.
        /// @return oa report type status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE drm_i915_oa_format GetOaReportType() const
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
            const int32_t          metricSet )
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

            addProperty( PRELIM_DRM_I915_PERF_PROP_OA_BUFFER_SIZE, GetMaxOaBufferSize() );
            addProperty( PRELIM_DRM_I915_PERF_PROP_OA_ENGINE_CLASS, engineClass );
            addProperty( PRELIM_DRM_I915_PERF_PROP_OA_ENGINE_INSTANCE, engineInstance );

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

    protected:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets maximum supported oa buffer size on a given platform.
        /// @return maximum supported oa buffer size.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t GetMaxOaBufferSize() const
        {
            return ( 128 * Constants::Data::m_Megabyte );
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

namespace ML::XE2_HPG
{
    template <typename T>
    struct TbsInterfaceTrait : BASE::TbsInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( TbsInterfaceTrait, BASE );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::GetTimerPeriodExponent;
        using Base::m_Kernel;

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa report type.
        /// @return oa report type status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t GetOaReportType() const
        {
            ML_FUNCTION_LOG( uint32_t{ 0 }, &m_Kernel.m_Context );

            switch( m_Kernel.GetQueryModeOverride() )
            {
                case T::Layouts::HwCounters::Query::Mode::Render:
                case T::Layouts::HwCounters::Query::Mode::Compute:
                case T::Layouts::HwCounters::Query::Mode::Global:
                    return log.m_Result = ( DRM_XE_OA_FMT_TYPE_PEC | ( 1 << 8 ) | ( 1 << 16 ) | ( 0 << 24 ) ); // counter select = 1, counter size = 1, bc report = 0

                case T::Layouts::HwCounters::Query::Mode::GlobalExtended:
                    return log.m_Result = ( DRM_XE_OA_FMT_TYPE_PEC | ( 1 << 8 ) | ( 1 << 16 ) | ( 1 << 24 ) ); // counter select = 1, counter size = 1, bc report = 1

                default:
                    ML_ASSERT_ALWAYS();
                    return log.m_Result;
            }
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns tbs properties.
        /// @param  metricSet   metric set associated with tbs stream.
        /// @return properties  tbs properties.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetStreamProperties(
            drm_xe_ext_set_property properties[],
            const int32_t           metricSet ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            uint32_t oaUnit       = 0;
            uint32_t currentIndex = 0;
            auto&    subDevice    = m_Kernel.m_Context.m_SubDevice;

            auto addProperty = [&]( const uint64_t key, const uint64_t value )
            {
                drm_xe_ext_set_property property = {};
                property.base.name               = DRM_XE_OA_EXTENSION_SET_PROPERTY;
                property.property                = key;
                property.value                   = value;

                properties[currentIndex] = property;

                if( currentIndex > 0 )
                {
                    properties[currentIndex - 1].base.next_extension = reinterpret_cast<uint64_t>( &properties[currentIndex] );
                }

                ++currentIndex;
            };

            ML_FUNCTION_CHECK( subDevice.GetTbsOaUnit( oaUnit ) );

            addProperty( DRM_XE_OA_PROPERTY_OA_UNIT_ID, oaUnit );
            addProperty( DRM_XE_OA_PROPERTY_SAMPLE_OA, true );
            addProperty( DRM_XE_OA_PROPERTY_OA_METRIC_SET, static_cast<uint32_t>( metricSet ) );
            addProperty( DRM_XE_OA_PROPERTY_OA_FORMAT, GetOaReportType() );
            addProperty( DRM_XE_OA_PROPERTY_OA_PERIOD_EXPONENT, GetTimerPeriodExponent( T::ConstantsOs::Tbs::m_TimerPeriod ) );

            return log.m_Result;
        }

    protected:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets maximum supported oa buffer size on a given platform.
        /// @return maximum supported oa buffer size.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t GetMaxOaBufferSize() const
        {
            return ( 16 * Constants::Data::m_Megabyte );
        }
    };
} // namespace ML::XE2_HPG
