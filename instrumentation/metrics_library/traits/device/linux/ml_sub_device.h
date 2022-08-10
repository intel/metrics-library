/*========================== begin_copyright_notice ============================

Copyright (C) 2021-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_sub_device.h

@brief Sub device support.
*/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for SubDeviceTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct SubDeviceTrait : SubDeviceCommonTrait<T>
        {
            ML_DELETE_DEFAULT_COPY_AND_MOVE( SubDeviceTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Types.
            //////////////////////////////////////////////////////////////////////////
            using Base = SubDeviceCommonTrait<T>;
            using Base::m_Context;

            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            TT::IoControl& m_IoControl;

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Device constructor.
            /// @param  context metrics library context.
            //////////////////////////////////////////////////////////////////////////
            SubDeviceTrait( TT::Context& context )
                : Base( context )
                , m_IoControl( m_Context.m_Kernel.m_IoControl )
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Initializes device data.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            StatusCode Initialize()
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "SubDeviceTrait<Traits>";
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Gen9 type for SubDeviceTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct SubDeviceTrait : BASE::SubDeviceTrait<T>
        {
            ML_DECLARE_TRAIT( SubDeviceTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct SubDeviceTrait : GEN9::SubDeviceTrait<T>
        {
            ML_DECLARE_TRAIT( SubDeviceTrait, GEN9 );
        };
    } // namespace GEN11

    namespace XE_LP
    {
        template <typename T>
        struct SubDeviceTrait : GEN11::SubDeviceTrait<T>
        {
            ML_DECLARE_TRAIT( SubDeviceTrait, GEN11 );
        };
    } // namespace XE_LP

    namespace XE_HP
    {
        template <typename T>
        struct SubDeviceTrait : XE_LP::SubDeviceTrait<T>
        {
            ML_DECLARE_TRAIT( SubDeviceTrait, XE_LP );

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Base types.
            //////////////////////////////////////////////////////////////////////////
            using Base::m_Context;
            using Base::m_IoControl;
            using Base::m_SubDeviceCount;
            using Base::m_SubDeviceIndex;
            using Base::m_IsSubDevice;
            using Base::m_Enabled;

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Members.
            //////////////////////////////////////////////////////////////////////////
            std::vector<i915_engine_class_instance> m_Engines;

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Device constructor.
            /// @param  context metrics library context.
            //////////////////////////////////////////////////////////////////////////
            SubDeviceTrait( TT::Context& context )
                : Base( context )
                , m_Engines{}
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Initializes device instance.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            StatusCode Initialize()
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

                auto     engines    = std::vector<drm_i915_engine_info>();
                auto     regions    = std::vector<prelim_drm_i915_memory_region_info>();
                auto     distances  = std::vector<prelim_drm_i915_query_distance_info>();
                uint32_t subDevices = 0;

                // Print client options related to sub devices.
                log.Debug( "Driver client data    " );
                log.Debug( "    data present      ", m_Context.m_ClientOptions.m_SubDeviceDataPresent );
                log.Debug( "    is sub device     ", m_IsSubDevice );
                log.Debug( "    sub devices count ", m_SubDeviceCount );
                log.Debug( "    sub device index  ", m_SubDeviceIndex );

                // Check client options obtained from the driver.
                if( T::Policy::SubDevice::m_DriverClientDataRequired )
                {
                    ML_FUNCTION_CHECK( m_Context.m_ClientOptions.m_SubDeviceDataPresent );
                }

                // Enumerate engines.
                ML_FUNCTION_CHECK( GetEngines( engines ) );

                // Enumerate memory regions.
                ML_FUNCTION_CHECK( GetMemoryRegions( regions ) );

                // Enumerate engine distances.
                ML_FUNCTION_CHECK( GetEngineDistances( engines, regions, distances ) );

                // Enumerate sub device engines.
                ML_FUNCTION_CHECK( GetSubDeviceEngines( distances, subDevices ) );

                // Helper data.
                const bool isRootDevice           = m_IsSubDevice == false;
                const bool isSubDevice            = m_IsSubDevice == true;
                const bool isFirstSubDevice       = m_IsSubDevice && ( m_SubDeviceIndex == 0 );
                const bool isKernelSupport        = IsSubDeviceSupported() && m_Engines.size() > 0;
                const bool isAllowImplicitScaling = T::Policy::SubDevice::m_AllowImplicitScaling;

                // Supported modes.
                const bool validRootDeviceOneSubDevice   = isRootDevice && ( m_SubDeviceCount == 1 );
                const bool validRootDeviceManySubDevices = isRootDevice && ( m_SubDeviceCount > 1 ) && isAllowImplicitScaling;
                const bool validSubDeviceFirst           = isSubDevice && isFirstSubDevice;
                const bool validSubDeviceRest            = isSubDevice && !isFirstSubDevice && isKernelSupport;
                const bool validNoClientData             = !m_Context.m_ClientOptions.m_SubDeviceDataPresent && !T::Policy::SubDevice::m_DriverClientDataRequired;

                m_Enabled =
                    validRootDeviceOneSubDevice ||   // Selected root device with one sub device.
                    validRootDeviceManySubDevices || // Selected root device with many sub devices.
                    validSubDeviceFirst ||           // Selected first sub device.
                    validSubDeviceRest ||            // Selected subsequent sub device.
                    validNoClientData;               // No client data (with sub device information), first tile will be used.

                if( m_Enabled )
                {
                    if( subDevices != m_SubDeviceCount )
                    {
                        log.Warning( "Sub devices count mismatch" );
                        log.Debug( "Driver sub devices      ", m_SubDeviceCount );
                    }
                    log.Debug( "Detected sub devices    ", subDevices );
                    log.Debug( "Detected engines        ", m_Engines.size() );
                    log.Debug( "Is root device          ", isRootDevice );
                    log.Debug( "Is sub device           ", isSubDevice );
                    log.Debug( "Is first sub device     ", isFirstSubDevice );
                    log.Debug( "Is kernel support       ", isKernelSupport );
                    log.Debug( "Device enabled          ", m_Enabled );
                }
                else
                {
                    log.Critical( "Device valid:                               ", m_Enabled );
                    log.Critical( "       Valid root device - one sub device   ", validRootDeviceOneSubDevice );
                    log.Critical( "       Valid root device - many sub devices ", validRootDeviceManySubDevices );
                    log.Critical( "       Valid first sub device               ", validSubDeviceFirst );
                    log.Critical( "       Valid next sub device                ", validSubDeviceRest );
                    log.Critical( "       Valid no client data                 ", validNoClientData );
                    log.Critical( "Detected:                                   " );
                    log.Critical( "       Kernel support                       ", isKernelSupport );
                    log.Critical( "       Allow implicit scaling               ", isAllowImplicitScaling );
                    log.Critical( "       Sub device count                     ", subDevices );
                    log.Critical( "       Engines count                        ", m_Engines.size() );
                    log.Critical( "       Is root device                       ", isRootDevice );
                    log.Critical( "       Is sub device                        ", isSubDevice );
                    log.Critical( "       Is first sub device                  ", isFirstSubDevice );
                    log.Critical( "Driver data                                 " );
                    log.Critical( "       Data has been passed from driver     ", m_Context.m_ClientOptions.m_SubDeviceDataPresent );
                    log.Critical( "       Is root device                       ", isRootDevice );
                    log.Critical( "       Is sub device                        ", m_IsSubDevice );
                    log.Critical( "       Sub devices count                    ", m_SubDeviceCount );
                    log.Critical( "       Sub device index                     ", m_SubDeviceIndex );
                }

                return log.m_Result = ML_STATUS( m_Enabled );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns render or compute engine that can be used by tbs.
            /// @return engineClass     engine class.
            /// @return engineInstance  engine instance.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            StatusCode GetTbsEngine(
                uint32_t& engineClass,
                uint32_t& engineInstance )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

                for( uint32_t i = 0; i < m_Engines.size(); ++i )
                {
                    const bool isEngineRender  = m_Engines[i].engine_class == I915_ENGINE_CLASS_RENDER;
                    const bool isEngineCompute = m_Engines[i].engine_class == PRELIM_I915_ENGINE_CLASS_COMPUTE;

                    if( isEngineCompute || isEngineRender )
                    {
                        engineClass    = m_Engines[i].engine_class;
                        engineInstance = m_Engines[i].engine_instance;

                        log.Debug( "class   ", engineClass );
                        log.Debug( "instance", engineInstance );

                        return log.m_Result;
                    }
                }

                return log.m_Result = StatusCode::Failed;
            }

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief  Checks if sub devices are supported.
            /// @return true if sub devices are supported.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsSubDeviceSupported()
            {
                const uint32_t revision  = static_cast<uint32_t>( m_Context.m_Kernel.m_Revision );
                const uint32_t expected  = static_cast<uint32_t>( T::ConstantsOs::Drm::Revision::SubDevices );
                bool           supported = revision >= expected;

                return supported;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Enumerates all available engines.
            /// @return engines device engines.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            StatusCode GetEngines( std::vector<drm_i915_engine_info>& engines )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

                auto       buffer      = std::vector<uint8_t>();
                const bool validQuery  = ML_SUCCESS( m_IoControl.Query( DRM_I915_QUERY_ENGINE_INFO, buffer ) );
                auto       enginesData = reinterpret_cast<drm_i915_query_engine_info*>( buffer.data() );

                // Query check.
                ML_FUNCTION_CHECK( validQuery );
                ML_FUNCTION_CHECK( enginesData != nullptr );
                ML_FUNCTION_CHECK( buffer.size() > 0 );

                // Copy engine data.
                for( uint32_t i = 0; i < enginesData->num_engines; ++i )
                {
                    engines.push_back( enginesData->engines[i] );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Enumerate available memory regions.
            /// @return regions    memory regions.
            /// @return            operation status.
            //////////////////////////////////////////////////////////////////////////
            StatusCode GetMemoryRegions( std::vector<prelim_drm_i915_memory_region_info>& regions )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

                // Obtain memory regions.
                auto       buffer     = std::vector<uint8_t>();
                const bool validQuery = ML_SUCCESS( m_IoControl.Query( PRELIM_DRM_I915_QUERY_MEMORY_REGIONS, buffer ) );

                // Query check.
                ML_FUNCTION_CHECK( validQuery );
                ML_FUNCTION_CHECK( buffer.size() > 0 );

                // Copy regions data.
                auto regionsData = reinterpret_cast<prelim_drm_i915_query_memory_regions*>( buffer.data() );

                for( uint32_t i = 0; i < regionsData->num_regions; ++i )
                {
                    regions.push_back( regionsData->regions[i] );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Enumerate engine distances.
            /// @param  engines    engines information.
            /// @param  regions    memory regions.
            /// @return distances  engine distances.
            /// @return            operation status.
            //////////////////////////////////////////////////////////////////////////
            StatusCode GetEngineDistances(
                const std::vector<drm_i915_engine_info>&               engines,
                const std::vector<prelim_drm_i915_memory_region_info>& regions,
                std::vector<prelim_drm_i915_query_distance_info>&      distances )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

                auto items = std::vector<drm_i915_query_item>();
                auto query = drm_i915_query{};

                // Prepare distance information.
                for( uint32_t i = 0; i < regions.size(); ++i )
                {
                    if( PRELIM_I915_MEMORY_CLASS_DEVICE == regions[i].region.memory_class )
                    {
                        auto distance                   = prelim_drm_i915_query_distance_info{};
                        distance.region.memory_class    = PRELIM_I915_MEMORY_CLASS_DEVICE;
                        distance.region.memory_instance = regions[i].region.memory_instance;

                        for( uint32_t j = 0; j < engines.size(); ++j )
                        {
                            auto engine = engines[j];

                            switch( engine.engine.engine_class )
                            {
                                case I915_ENGINE_CLASS_RENDER:
                                case I915_ENGINE_CLASS_COPY:
                                case PRELIM_I915_ENGINE_CLASS_COMPUTE:
                                    distance.engine = engine.engine;
                                    distances.push_back( distance );
                                default:
                                    break;
                            }
                        }
                    }
                }

                // Prepare query items.
                items.resize( distances.size(), {} );

                for( uint32_t i = 0; i < distances.size(); ++i )
                {
                    items[i]          = {};
                    items[i].query_id = PRELIM_DRM_I915_QUERY_DISTANCE_INFO;
                    items[i].length   = sizeof( prelim_drm_i915_query_distance_info );
                    items[i].data_ptr = reinterpret_cast<uint64_t>( &( distances[i] ) );
                }

                // Prepare query data.
                query.items_ptr = reinterpret_cast<uint64_t>( items.data() );
                query.num_items = static_cast<uint32_t>( items.size() );

                return log.m_Result = m_IoControl.Query( query );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Enumerate engine distances.
            /// @param  distances  engine distances.
            /// @return subDevices sub device count.
            /// @return            operation status.
            //////////////////////////////////////////////////////////////////////////
            StatusCode GetSubDeviceEngines(
                const std::vector<prelim_drm_i915_query_distance_info>& distances,
                uint32_t&                                               subDevices )
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

                for( uint32_t i = 0, subDevice = 0; i < distances.size(); ++i )
                {
                    auto       engine        = distances[i].engine;
                    const bool validDistance = distances[i].distance == 0;
                    const bool newDevice     = ( i > 0 ) && ( distances[i].region.memory_instance != distances[i - 1].region.memory_instance );
                    subDevice                = subDevice + ( newDevice ? 1 : 0 );
                    subDevices               = subDevice + 1;

                    if( validDistance && ( subDevice == m_SubDeviceIndex ) )
                    {
                        log.Debug(
                            "Sub device / engine class and instance",
                            subDevice,
                            engine.engine_class,
                            engine.engine_instance );

                        switch( engine.engine_class )
                        {
                            case I915_ENGINE_CLASS_RENDER:
                            case PRELIM_I915_ENGINE_CLASS_COMPUTE:
                                m_Engines.push_back( engine );
                                break;

                            case I915_ENGINE_CLASS_COPY:
                                break;

                            default:
                                ML_ASSERT_ALWAYS();
                                break;
                        }
                    }
                }

                return log.m_Result;
            }
        };
    } // namespace XE_HP

    namespace XE_HPG
    {
        template <typename T>
        struct SubDeviceTrait : XE_HP::SubDeviceTrait<T>
        {
            ML_DECLARE_TRAIT( SubDeviceTrait, XE_HP );
        };
    } // namespace XE_HPG

    namespace XE_HPC
    {
        template <typename T>
        struct SubDeviceTrait : XE_HPG::SubDeviceTrait<T>
        {
            ML_DECLARE_TRAIT( SubDeviceTrait, XE_HPG );
        };
    } // namespace XE_HPC
} // namespace ML
