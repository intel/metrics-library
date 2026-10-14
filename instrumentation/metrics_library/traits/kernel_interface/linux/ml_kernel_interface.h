/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2026 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_kernel_interface.h

@brief Allows to communicate with os kernel.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for KernelInterfaceTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct KernelInterfaceTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( KernelInterfaceTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( KernelInterfaceTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::Context&                                           m_Context;
        TT::ConstantsOs::Drm::Revision                         m_Revision;
        TT::IoControl                                          m_IoControl;
        TT::TbsInterface                                       m_Tbs;
        TT::Configurations::HwCountersOa::ConfigurationManager m_ConfigurationManager;
        uint64_t                                               m_OaFrequency;
        uint64_t                                               m_CsFrequency;
        uint64_t                                               m_GpuTimestampTickValue;
        int32_t                                                m_DeviceId;

        //////////////////////////////////////////////////////////////////////////
        /// @brief KernelInterfaceTrait constructor.
        /// @param context  metrics library context.
        //////////////////////////////////////////////////////////////////////////
        KernelInterfaceTrait( TT::Context& context )
            : m_Context( context )
            , m_Revision( T::ConstantsOs::Drm::Revision::Unsupported )
            , m_IoControl( static_cast<TT::KernelInterface&>( *this ) )
            , m_Tbs( static_cast<TT::KernelInterface&>( *this ) )
            , m_ConfigurationManager{}
            , m_OaFrequency( 0 )
            , m_CsFrequency( 0 )
            , m_GpuTimestampTickValue( 0 )
            , m_DeviceId( T::ConstantsOs::Drm::m_Invalid )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Destroys all cached oa configurations.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void DeleteConfigurations()
        {
            std::lock_guard<std::mutex> guard( m_ConfigurationManager.m_Mutex );

            for( auto& configuration : m_ConfigurationManager.m_ActiveConfigurations )
            {
                T::Configurations::HwCountersOa::Base::Delete( configuration.second );
            }

            m_ConfigurationManager.m_ActiveConfigurations.clear();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes kernel interface.
        /// @param  clientData  initializing client data.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize( const ClientData_1_0& clientData )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_FUNCTION_CHECK( CheckParanoidMode() );
            ML_FUNCTION_CHECK( m_IoControl.Initialize( clientData, m_Revision ) );
            ML_FUNCTION_CHECK( InitializeDevice() );
            ML_FUNCTION_CHECK( InitializeSubDevice() );
            ML_FUNCTION_CHECK( m_Tbs.Initialize() );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets adapter id value.
        /// @return adapterId  adapter id.
        /// @return            operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetAdapterId( uint32_t& adapterId ) const
        {
            ML_FUNCTION_LOG_STATIC( StatusCode::Success );

            log.m_Result = m_IoControl.m_DrmCard != T::ConstantsOs::Drm::m_Invalid
                ? StatusCode::Success
                : StatusCode::Failed;

            adapterId = ML_SUCCESS( log.m_Result )
                ? static_cast<uint32_t>( m_IoControl.m_DrmCard )
                : IU_ADAPTER_ID_UNKNOWN;

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns gpu oa timestamp frequency.
        /// @return gpu oa timestamp frequency.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t GetGpuOaTimestampFrequency()
        {
            ML_FUNCTION_LOG( uint64_t{ 0 }, &m_Context );

            if( m_OaFrequency == 0 )
            {
                m_OaFrequency = m_IoControl.GetGpuTimestampFrequency( T::Layouts::Configuration::TimestampType::Oa );

                if( m_OaFrequency == 0 )
                {
                    ML_ASSERT_ALWAYS();
                    m_OaFrequency = Constants::ComboTimestamp::m_DefaultGpuFrequency;
                    log.Warning( "Predefined default gpu oa timestamp frequency used", m_CsFrequency );
                }
            }

            return log.m_Result = m_OaFrequency;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns gpu cs timestamp frequency.
        /// @return gpu cs timestamp frequency.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t GetGpuCsTimestampFrequency()
        {
            ML_FUNCTION_LOG( uint64_t{ 0 }, &m_Context );

            if( m_CsFrequency == 0 )
            {
                m_CsFrequency = m_IoControl.GetGpuTimestampFrequency( T::Layouts::Configuration::TimestampType::Cs );

                if( m_CsFrequency == 0 )
                {
                    ML_ASSERT_ALWAYS();
                    m_CsFrequency = Constants::ComboTimestamp::m_DefaultGpuFrequency;
                    log.Warning( "Predefined default gpu cs timestamp frequency used", m_CsFrequency );
                }
            }

            return log.m_Result = m_CsFrequency;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets exact gpu timestamp frequency.
        /// @param  timestampType  select timestamp domain - oa or cs.
        /// @return                gpu timestamp frequency.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t GetGpuTimestampFrequency( const TT::Layouts::Configuration::TimestampType timestampType )
        {
            return timestampType == T::Layouts::Configuration::TimestampType::Oa
                ? GetGpuOaTimestampFrequency()
                : GetGpuCsTimestampFrequency();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns tbs state.
        /// @return true if tbs is enabled.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool TbsIsEnabled() const
        {
            ML_FUNCTION_LOG( false, &m_Context );

            return log.m_Result = m_Tbs.IsEnabled();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Loads oa configuration to gpu through tbs interface.
        /// @param  oaConfigurationId   oa configuration id.
        /// @param  indexNode           index node of the kernel metric set directory.
        /// @param  reportType          report type.
        /// @return                     operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode LoadOaConfigurationToGpu(
            const int32_t                   oaConfigurationId,
            const uint64_t                  indexNode,
            [[maybe_unused]] const uint32_t reportType )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_FUNCTION_CHECK( m_Tbs.m_Stream.SetMetricSet( oaConfigurationId, indexNode ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Releases oa metric set. If metric set has been used previously
        ///         then reference counter will be decreased. If reference counter
        ///         will become zero then another metric set can be used.
        /// @param  oaConfigurationId  oa configuration id.
        /// @return                    operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode UnloadOaConfigurationFromGpu( const int32_t oaConfigurationId )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_FUNCTION_CHECK( m_Tbs.m_Stream.ReleaseMetricSet( oaConfigurationId ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns data of activated by metrics discovery
        ///         oa configuration from the kernel.
        /// @return oa      oa configuration data.
        /// @return mert    mert configuration data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetOaConfigurationData(
            TT::Configurations::HwCountersOa::ConfigurationData& oa,
            TT::Configurations::HwCountersOa::ConfigurationData& mert )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            ML_FUNCTION_CHECK( m_IoControl.GetKernelMetricSetData( oa, mert ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns activated by metrics discovery oa configuration
        ///         from the kernel.
        /// @return oa  oa configuration data.
        /// @return     operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetOaConfiguration( TT::Configurations::HwCountersOa::ConfigurationData& oa )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            oa.m_KernelMetricSet = m_IoControl.GetKernelMetricSetFile( oa.m_Name );

            ML_FUNCTION_CHECK( oa.m_KernelMetricSet != T::ConstantsOs::Drm::m_Invalid );

            oa.m_Id = m_IoControl.GetKernelMetricSet( oa.m_KernelMetricSet );

            if( oa.m_Id == T::ConstantsOs::Drm::m_Invalid )
            {
                close( oa.m_KernelMetricSet );
                oa.m_KernelMetricSet = T::ConstantsOs::Drm::m_Invalid;
                return log.m_Result  = StatusCode::Failed;
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets cached configuration handle by configuration hash.
        /// @param  configurationHash   oa configuration hash.
        /// @return handle              oa configuration handle.
        /// @return                     true if configuration found, false otherwise.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsOaConfigurationHandle(
            const uint64_t           configurationHash,
            ConfigurationHandle_1_0& handle )
        {
            std::lock_guard<std::mutex> guard( m_ConfigurationManager.m_Mutex );

            if( auto iterator = m_ConfigurationManager.m_ActiveConfigurations.find( configurationHash );
                iterator != m_ConfigurationManager.m_ActiveConfigurations.end() )
            {
                auto& savedConfiguration = T::Configurations::HwCountersOa::FromHandle( iterator->second );
                savedConfiguration.m_ReferenceCounter++;

                handle = iterator->second;

                return true;
            }

            return false;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns activated by metrics discovery mmio user configuration
        ///         from the kernel.
        ///         Not supported on Linux.
        /// @param  userConfiguration   activated mmio user configuration.
        /// @return                     operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetUserConfiguration( [[maybe_unused]] TT::Layouts::Configuration::UserRegisters& userConfiguration ) const
        {
            ML_FUNCTION_LOG( StatusCode::NotSupported, &m_Context );

            ML_ASSERT_ALWAYS();

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Loads oa mert configuration to gpu through tbs interface.
        /// @param  oaMertConfigurationId   oa mert configuration id.
        /// @param  indexNode               index node of the kernel metric set directory.
        /// @param  reportType              report type.
        /// @return                         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode LoadOaMertConfigurationToGpu(
            [[maybe_unused]] const int32_t  oaMertConfigurationId,
            [[maybe_unused]] const uint64_t indexNode,
            [[maybe_unused]] const uint32_t reportType )
        {
            // Not supported.
            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Releases oa mert metric set. If metric set has been used previously
        ///         then reference counter will be decreased. If reference counter
        ///         will become zero then another metric set can be used.
        /// @param  oaMertConfigurationId   oa mert configuration id.
        /// @return                         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode UnloadOaMertConfigurationFromGpu( [[maybe_unused]] const int32_t oaMertConfigurationId )
        {
            // Not supported.
            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns activated by metrics discovery oa mert configuration
        ///         from the kernel.
        /// @return mert    oa mert configuration data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetOaMertConfiguration( [[maybe_unused]] TT::Configurations::HwCountersOa::ConfigurationData& mert )
        {
            // Not supported.
            mert.m_Id              = T::ConstantsOs::Drm::m_Invalid;
            mert.m_KernelMetricSet = T::ConstantsOs::Drm::m_Invalid;
            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns auto sampling mode for hw counters.
        /// @return hw counters auto sampling mode.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::Query::GetDataMode GetQueryHwCountersReportingMode() const
        {
            return T::Layouts::HwCounters::Query::GetDataMode::Normal;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns posh query override state.
        /// @return true if posh query override has been enabled.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsPoshQueryOverride() const
        {
            ML_FUNCTION_LOG( false, &m_Context );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns query mode override state.
        /// @return query mode.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::Query::Mode GetQueryModeOverride() const
        {
            return m_IoControl.GetQueryModeOverride();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns report collecting mode override state.
        /// @return report collecting mode.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::Query::ReportCollectingMode GetReportCollectingModeOverride() const
        {
            return m_IoControl.GetReportCollectingModeOverride();
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes device.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode InitializeDevice()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // Chipset.
            ML_FUNCTION_CHECK( m_IoControl.GetChipsetId( m_DeviceId ) );
            ML_FUNCTION_CHECK( m_DeviceId != T::ConstantsOs::Drm::m_Invalid );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes sub device.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode InitializeSubDevice()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            return log.m_Result = m_Context.m_SubDevice.Initialize();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks i915 paranoid mode required by query to work.
        ///         Logs a warning if check fails.
        /// @return always success.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode CheckParanoidMode() const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            auto file      = fopen( T::Layouts::Drm::Stream::m_ParanoidPath, "r" );
            char flag[256] = "";

            const bool validFile = file != nullptr;
            const bool validRead = validFile && ( fread( flag, 1, sizeof( flag ), file ) > 0 );
            const bool validFlag = validRead && ( atoi( flag ) == 0 );

            // Close file.
            if( file )
            {
                fclose( file );
            }

            if( !validFile )
            {
                log.Warning( "Paranoid mode check failed: Unable to open the file" );
            }
            else if( !validRead )
            {
                log.Warning( "Paranoid mode check failed: Unable to read the file" );
            }
            else if( !validFlag )
            {
                log.Warning( "Paranoid mode check failed: Paranoid mode is not available" );
            }

            // Always return success.
            return log.m_Result = StatusCode::Success;
        }
    };
} // namespace ML::BASE

namespace ML::XE_LP
{
    template <typename T>
    struct KernelInterfaceTrait : BASE::KernelInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( KernelInterfaceTrait, BASE );
    };
} // namespace ML::XE_LP

namespace ML::XE_HPG
{
    template <typename T>
    struct KernelInterfaceTrait : XE_LP::KernelInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( KernelInterfaceTrait, XE_LP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct KernelInterfaceTrait : XE_HPG::KernelInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( KernelInterfaceTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct KernelInterfaceTrait : XE_HPG::KernelInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( KernelInterfaceTrait, XE_HPG );
    };
} // namespace ML::XE2_HPG

namespace ML::XE3
{
    template <typename T>
    struct KernelInterfaceTrait : XE2_HPG::KernelInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( KernelInterfaceTrait, XE2_HPG );
    };
} // namespace ML::XE3

namespace ML::XE3P
{
    template <typename T>
    struct KernelInterfaceTrait : XE3::KernelInterfaceTrait<T>
    {
        ML_DECLARE_TRAIT( KernelInterfaceTrait, XE3 );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::m_Context;
        using Base::m_IoControl;
        using Base::m_Tbs;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        bool m_IsOaMertSupported;

        //////////////////////////////////////////////////////////////////////////
        /// @brief KernelInterfaceTrait constructor.
        /// @param context  metrics library context.
        //////////////////////////////////////////////////////////////////////////
        KernelInterfaceTrait( TT::Context& context )
            : Base( context )
            , m_IsOaMertSupported( false )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Loads oa mert configuration to gpu through tbs interface.
        /// @param  oaMertConfigurationId   oa mert configuration id.
        /// @param  indexNode               index node of the kernel metric set directory.
        /// @param  reportType              report type.
        /// @return                         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode LoadOaMertConfigurationToGpu(
            const int32_t                   oaMertConfigurationId,
            const uint64_t                  indexNode,
            [[maybe_unused]] const uint32_t reportType )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            if( m_IsOaMertSupported && oaMertConfigurationId != T::ConstantsOs::Drm::m_Invalid )
            {
                ML_FUNCTION_CHECK( m_Tbs.m_StreamMert.SetMetricSet( oaMertConfigurationId, indexNode ) );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Releases oa mert metric set. If metric set has been used previously
        ///         then reference counter will be decreased. If reference counter
        ///         will become zero then another metric set can be used.
        /// @param  oaMertConfigurationId   oa mert configuration id.
        /// @return                         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode UnloadOaMertConfigurationFromGpu( const int32_t oaMertConfigurationId )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            if( m_IsOaMertSupported && oaMertConfigurationId != T::ConstantsOs::Drm::m_Invalid )
            {
                ML_FUNCTION_CHECK( m_Tbs.m_StreamMert.ReleaseMetricSet( oaMertConfigurationId ) );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns activated by metrics discovery oa mert configuration
        ///         from the kernel.
        /// @return mert    oa mert configuration data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetOaMertConfiguration( TT::Configurations::HwCountersOa::ConfigurationData& mert )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            if( m_IsOaMertSupported )
            {
                mert.m_KernelMetricSet = m_IoControl.GetKernelMetricSetFile( mert.m_Name );
                mert.m_Id              = m_IoControl.GetKernelMetricSet( mert.m_KernelMetricSet );

                if( mert.m_Id == T::ConstantsOs::Drm::m_Invalid )
                {
                    log.Info( "Oa mert is not activated" );
                }
            }
            else
            {
                mert.m_Id              = T::ConstantsOs::Drm::m_Invalid;
                mert.m_KernelMetricSet = T::ConstantsOs::Drm::m_Invalid;
                log.Info( "Oa mert is not supported" );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa mert support state.
        /// @return true if oa mert is supported.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsOaMertSupported() const
        {
            return m_IsOaMertSupported;
        }
    };
} // namespace ML::XE3P
