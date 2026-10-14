/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2026 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_configuration_hw_counters_oa.h

@brief Creates, activates and deactivates oa configuration.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for ConfigurationHwCountersOaTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ConfigurationHwCountersOaTrait : DdiObject<T, TT::Configurations::HwCountersOa, ConfigurationHandle_1_0, ObjectType::ConfigurationHwCountersOa>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ConfigurationHwCountersOaTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ConfigurationHwCountersOaTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Configurations::HwCountersOa, ConfigurationHandle_1_0, ObjectType::ConfigurationHwCountersOa>;
        using Base::Allocate;
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for HwCounters configuration data object.
        //////////////////////////////////////////////////////////////////////////
        struct ConfigurationData
        {
            std::string m_Name;
            int32_t     m_Id;
            int32_t     m_KernelMetricSet;
            uint64_t    m_IndexNode;
            uint32_t    m_ReportType;

            //////////////////////////////////////////////////////////////////////////
            /// @brief ConfigurationData constructor.
            //////////////////////////////////////////////////////////////////////////
            ConfigurationData()
                : m_Name()
                , m_Id( T::ConstantsOs::Drm::m_Invalid )
                , m_KernelMetricSet( T::ConstantsOs::Drm::m_Invalid )
                , m_IndexNode( 0 )
                , m_ReportType( 0 )
            {
            }
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for HwCounters configuration manager object.
        //////////////////////////////////////////////////////////////////////////
        struct ConfigurationManager
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            int32_t                                     m_OaConfigurationReferenceCounter;
            std::mutex                                  m_Mutex;
            std::map<uint64_t, ConfigurationHandle_1_0> m_ActiveConfigurations;

            //////////////////////////////////////////////////////////////////////////
            /// @brief ConfigurationManager constructor.
            //////////////////////////////////////////////////////////////////////////
            ConfigurationManager()
                : m_OaConfigurationReferenceCounter( 0 )
                , m_Mutex{}
                , m_ActiveConfigurations{}
            {
            }
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::KernelInterface& m_Kernel;
        ConfigurationData    m_Oa;
        ConfigurationData    m_Mert;
        uint64_t             m_Hash;
        int32_t              m_ReferenceCounter;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Configuration hw counters oa constructor.
        /// @param context  library context.
        //////////////////////////////////////////////////////////////////////////
        ConfigurationHwCountersOaTrait( TT::Context& context )
            : Base( context )
            , m_Kernel{ context.m_Kernel }
            , m_Oa()
            , m_Mert()
            , m_Hash( 0 )
            , m_ReferenceCounter( 1 )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Configuration hw counters oa destructor.
        //////////////////////////////////////////////////////////////////////////
        ~ConfigurationHwCountersOaTrait()
        {
            if( m_Oa.m_KernelMetricSet != T::ConstantsOs::Drm::m_Invalid )
            {
                close( m_Oa.m_KernelMetricSet );
            }

            if( m_Mert.m_KernelMetricSet != T::ConstantsOs::Drm::m_Invalid )
            {
                close( m_Mert.m_KernelMetricSet );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Creates oa configuration.
        /// @param  context library context.
        /// @return handle  configuration handle.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode Create(
            TT::Context&             context,
            ConfigurationHandle_1_0& handle )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &context );

            auto& kernel = context.m_Kernel;

            std::array<ConfigurationData, 2> data = {};

            ConfigurationData& oa   = data[0];
            ConfigurationData& mert = data[1];

            // Gets name, index node and report.
            ML_FUNCTION_CHECK( kernel.GetOaConfigurationData( oa, mert ) );

            const uint64_t hash = GetConfigurationHash( oa.m_IndexNode, mert.m_IndexNode );

            if( kernel.IsOaConfigurationHandle( hash, handle ) )
            {
                return log.m_Result;
            }

            if( auto configuration = Allocate( context );
                configuration != nullptr )
            {
                if( ML_FAIL( log.m_Result = kernel.GetOaConfiguration( oa ) ) ||
                    ML_FAIL( log.m_Result = kernel.GetOaMertConfiguration( mert ) ) )
                {
                    Base::Delete( configuration );
                    return log.m_Result;
                }

                configuration->m_Oa   = oa;
                configuration->m_Mert = mert;
                configuration->m_Hash = hash;
                handle                = configuration->GetHandle();

                auto& configurationManager = kernel.m_ConfigurationManager;

                std::lock_guard<std::mutex> guard( configurationManager.m_Mutex );

                configurationManager.m_ActiveConfigurations.emplace( hash, handle );

                return log.m_Result;
            }

            return log.m_Result = StatusCode::Failed;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Decreases oa configuration reference counter.
        ///         If reference counter is equal to zero, deletes configuration object.
        /// @return handle  configuration handle.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode Delete( const ConfigurationHandle_1_0& handle )
        {
            if constexpr( !T::Policy::Configuration::m_DeferDelete )
            {
                auto& configuration        = T::Configurations::HwCountersOa::FromHandle( handle );
                auto& configurationManager = configuration.m_Context.m_Kernel.m_ConfigurationManager;

                ML_FUNCTION_LOG( StatusCode::Success, &configuration.m_Context );

                std::lock_guard<std::mutex> guard( configurationManager.m_Mutex );

                if( --( configuration.m_ReferenceCounter ) == 0 )
                {
                    configurationManager.m_ActiveConfigurations.erase( configuration.m_Hash );

                    Base::Delete( &configuration );
                }
            }

            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Activates oa configuration.
        /// @param  activateData  oa counter activate data.
        /// @return               operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Activate( const ConfigurationActivateData_1_0& activateData ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_FUNCTION_CHECK( activateData.Type == GpuConfigurationActivationType::Tbs );
            ML_FUNCTION_CHECK( m_Kernel.LoadOaConfigurationToGpu( m_Oa.m_Id, m_Oa.m_IndexNode, m_Oa.m_ReportType ) );

            if( ML_SUCCESS( m_Kernel.LoadOaMertConfigurationToGpu( m_Mert.m_Id, m_Mert.m_IndexNode, m_Mert.m_ReportType ) ) )
            {
                m_Kernel.m_ConfigurationManager.m_OaConfigurationReferenceCounter++;
            }
            else
            {
                ML_FUNCTION_CHECK( m_Kernel.UnloadOaConfigurationFromGpu( m_Oa.m_Id ) );
                log.m_Result = StatusCode::Failed;
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Deactivates oa configuration.
        ///         If oa metric set is in use, reference counter will be decreased.
        ///         if reference counter will become zero, another metric set will be
        ///         allowed to be used.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Deactivate() const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_FUNCTION_CHECK( m_Kernel.UnloadOaConfigurationFromGpu( m_Oa.m_Id ) );

            log.m_Result = m_Kernel.UnloadOaMertConfigurationFromGpu( m_Mert.m_Id );

            m_Kernel.m_ConfigurationManager.m_OaConfigurationReferenceCounter--;

            return log.m_Result;
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns a hash of oa and mert configuration index nodes.
        /// @param  indexNodeOa     oa configuration index node.
        /// @param  indexNodeMert   mert configuration index node.
        /// @return                 configuration hash.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static uint64_t GetConfigurationHash(
            const uint64_t indexNodeOa,
            const uint64_t indexNodeMert )
        {
            constexpr uint64_t prime = 0x9e3779b97f4a7c15ull;
            uint64_t           hash  = 0;

            hash ^= T::Tools::GetHash( static_cast<uint64_t>( indexNodeOa ) ) + prime + ( hash << 6 ) + ( hash >> 2 );
            hash ^= T::Tools::GetHash( static_cast<uint64_t>( indexNodeMert ) ) + prime + ( hash << 12 ) + ( hash >> 4 );

            return hash;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Calls flush command buffer callback.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode FlushCommandBuffer() const
        {
            if constexpr( T::Policy::QueryHwCounters::Begin::m_FlushCommandBuffer )
            {
                if( const auto callback = m_Context.m_ClientCallbacks.CommandBufferFlush;
                    callback != nullptr )
                {
                    return callback( m_Context.m_ClientHandle );
                }
            }

            return StatusCode::Success;
        }
    };
} // namespace ML
