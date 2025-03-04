/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_traits_gen9.h

@brief Auto-generated file. Creates Traits structures for a given gen / os / api.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_traits
Template:           Tools/MetricsLibraryGenerator/templates/traits.h
*/

#pragma once

#if ML_ENABLE_GEN9
#define ML_GEN9 1

#include "ml_ddi_gen9.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates ML traits for GEN9.
//////////////////////////////////////////////////////////////////////////
#if ML_ENABLE_OPENCL
namespace ML::GEN9::OpenCL
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Traits definitions.
    //////////////////////////////////////////////////////////////////////////
    struct Traits
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Traits information.
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        /// @brief OpenCL specific traits.
        //////////////////////////////////////////////////////////////////////////
        #if ML_LINUX
        using Policy                    = PolicyOpenCLTrait<Traits>;                        //  linux/ml_policy_opencl.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Constants.
        //////////////////////////////////////////////////////////////////////////
        #if ML_LINUX
        using ConstantsOs                    = ConstantsOsTrait<Traits>;                    //  linux/ml_constants_os.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Tools.
        //////////////////////////////////////////////////////////////////////////
        using Tools                         = ToolsTrait<Traits>;                           //  ml_tools.h
        #if ML_LINUX
        using ToolsOs                       = ToolsOsTrait<Traits>;                         //  linux/ml_tools_os.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs.
        //////////////////////////////////////////////////////////////////////////
        #if ML_DEBUG || ML_INTERNAL
        template <typename ResultT> using FunctionLog       = FunctionLogTrait<Traits, ResultT>;        //  ml_function_log.h
        #endif
        #if ML_RELEASE
        template <typename ResultT> using FunctionLog       = FunctionLogReleaseTrait<Traits, ResultT>; //  ml_function_log_release.h
        #endif
                                    using FunctionLogStatic = FunctionLog<StatusCode>;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Library context.
        //////////////////////////////////////////////////////////////////////////
        using Context                       = ContextTrait<Traits>;                         //  ml_context.h
        using ClientOptions                 = ClientOptionsTrait<Traits>;                   //  ml_client_options.h
        using DdiObjects                    = DdiObjectsTrait<Traits>;                      //  ml_ddi_objects.h
        using State                         = StateTrait<Traits>;                           //  ml_state.h

        //////////////////////////////////////////////////////////////////////////
        /// @brief SubDevice.
        //////////////////////////////////////////////////////////////////////////
        using SubDeviceCommon               = SubDeviceCommonTrait<Traits>;                 //  ml_sub_device_common.h
        #if ML_LINUX
        using SubDevice                     = SubDeviceTrait<Traits>;                       //  linux/ml_sub_device.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Gpu registers commands / command buffer.
        //////////////////////////////////////////////////////////////////////////
        using GpuCommandBuffer              = GpuCommandBufferTrait<Traits>;                //  ml_gpu_command_buffer.h
        using GpuCommandBufferCalculator    = GpuCommandBufferCalculatorTrait<Traits>;      //  ml_gpu_command_buffer.h
        using GpuCommands                   = GpuCommandsTrait<Traits>;                     //  ml_gpu_commands.h
        using GpuRegisters                  = GpuRegistersTrait<Traits>;                    //  ml_gpu_registers.h

        //////////////////////////////////////////////////////////////////////////
        /// @brief Layouts.
        //////////////////////////////////////////////////////////////////////////
        struct Layouts
        {
            using Configuration             = ConfigurationLayoutsTrait<Traits>;            //  ml_configuration_layouts.h
            using OaBuffer                  = OaBufferLayoutsTrait<Traits>;                 //  ml_oa_buffer_layouts.h
            using HwCounters                = HwCountersLayoutsTrait<Traits>;               //  ml_hw_counters_layouts.h
            using GpuCommands               = GpuCommandsLayoutsTrait<Traits>;              //  ml_gpu_commands_include.h
        #if ML_LINUX
            using Override                  = OverrideLayoutsTrait<Traits>;                 //  linux/ml_override_layouts.h
        #endif
            using GpuRegisters              = GpuRegistersLayoutsTrait<Traits>;             //  ml_gpu_registers_layouts.h
        #if ML_LINUX
            using Drm                       = DrmLayoutsTrait<Traits>;                      //  linux/ml_drm_layouts.h
        #endif
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Debug helper.
        //////////////////////////////////////////////////////////////////////////
        #if ML_LINUX
        using Debug                         = DebugTrait<Traits>;                           //  linux/ml_debug_helper.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Markers.
        //////////////////////////////////////////////////////////////////////////
        struct Markers
        {
            using StreamUser                = MarkerStreamUserTrait<Traits>;                //  ml_marker_stream_user.h
        #if ML_LINUX
            using StreamUserExtended        = MarkerStreamUserExtendedTrait<Traits>;        //  linux/ml_marker_stream_user_extended.h
        #endif
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Linux kernel interface traits.
        //////////////////////////////////////////////////////////////////////////
        #if ML_LINUX
        using KernelInterface               = KernelInterfaceTrait<Traits>;                 //  linux/ml_kernel_interface.h
        #endif
        #if ML_LINUX
        using TbsInterface                  = TbsInterfaceTrait<Traits>;                    //  linux/ml_tbs_interface.h
        #endif
        #if ML_LINUX
        using TbsStream                     = TbsStreamTrait<Traits>;                       //  linux/ml_tbs_stream.h
        #endif
        #if ML_LINUX
        using IoControl                     = IoControlTrait<Traits>;                       //  linux/ml_io_control.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Configurations.
        //////////////////////////////////////////////////////////////////////////
        struct Configurations
        {
        #if ML_LINUX
            using HwCountersOa              = ConfigurationHwCountersOaTrait<Traits>;       //  linux/ml_configuration_hw_counters_oa.h
        #endif
            using HwCountersUser            = ConfigurationHwCountersUserTrait<Traits>;     //  ml_configuration_hw_counters_user.h
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa buffer support.
        //////////////////////////////////////////////////////////////////////////
        #if 0
        using OaBuffer                      = OaBufferDummyTrait<Traits>;                   //  ml_oa_buffer_dummy.h
        #endif
        using OaBuffer                      = OaBufferMappedTrait<Traits>;                  //  ml_oa_buffer_mapped.h

        //////////////////////////////////////////////////////////////////////////
        /// @brief Overrides.
        //////////////////////////////////////////////////////////////////////////
        struct Overrides
        {
        #if ML_LINUX
            using User                      = OverrideUserTrait<Traits>;                    //  linux/ml_override_user.h
        #endif
            using NullHardware              = OverrideNullHardwareTrait<Traits>;            //  ml_override_null_hardware.h
            using FlushCaches               = OverrideFlushCachesTrait<Traits>;             //  ml_override_flush_caches.h
        #if ML_LINUX
            using PoshQuery                 = OverridePoshQueryTrait<Traits>;               //  linux/ml_override_posh_query.h
        #endif
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Queries.
        //////////////////////////////////////////////////////////////////////////
        struct Queries
        {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Hw Counters.
        //////////////////////////////////////////////////////////////////////////
            using HwCountersSlot              = QueryHwCountersSlotTrait<Traits>;               //  ml_query_hw_counters_slot.h
            using HwCounters                  = QueryHwCountersTrait<Traits>;                   //  ml_query_hw_counters.h
            using HwCountersCalculator        = QueryHwCountersCalculatorTrait<Traits>;         //  ml_query_hw_counters_calculator.h
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Internal features.
        //////////////////////////////////////////////////////////////////////////
        #if ML_LINUX || ML_RELEASE
        using InternalFeature           = InternalFeatureTrait<Traits>;                     //  ml_internal_feature.h
        #endif
    };
} // namespace ML::GEN9::OpenCL
#endif //ML_ENABLE_OPENCL

#if ML_ENABLE_ONEAPI
namespace ML::GEN9::OneApi
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Traits definitions.
    //////////////////////////////////////////////////////////////////////////
    struct Traits
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Traits information.
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        /// @brief One Api specific traits.
        //////////////////////////////////////////////////////////////////////////
        #if ML_LINUX
        using Policy                    = PolicyOneApiTrait<Traits>;                        //  linux/ml_policy_oneapi.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Constants.
        //////////////////////////////////////////////////////////////////////////
        #if ML_LINUX
        using ConstantsOs                    = ConstantsOsTrait<Traits>;                    //  linux/ml_constants_os.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Tools.
        //////////////////////////////////////////////////////////////////////////
        using Tools                         = ToolsTrait<Traits>;                           //  ml_tools.h
        #if ML_LINUX
        using ToolsOs                       = ToolsOsTrait<Traits>;                         //  linux/ml_tools_os.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Logs.
        //////////////////////////////////////////////////////////////////////////
        #if ML_DEBUG || ML_INTERNAL
        template <typename ResultT> using FunctionLog       = FunctionLogTrait<Traits, ResultT>;        //  ml_function_log.h
        #endif
        #if ML_RELEASE
        template <typename ResultT> using FunctionLog       = FunctionLogReleaseTrait<Traits, ResultT>; //  ml_function_log_release.h
        #endif
                                    using FunctionLogStatic = FunctionLog<StatusCode>;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Library context.
        //////////////////////////////////////////////////////////////////////////
        using Context                       = ContextTrait<Traits>;                         //  ml_context.h
        using ClientOptions                 = ClientOptionsTrait<Traits>;                   //  ml_client_options.h
        using DdiObjects                    = DdiObjectsTrait<Traits>;                      //  ml_ddi_objects.h
        using State                         = StateTrait<Traits>;                           //  ml_state.h

        //////////////////////////////////////////////////////////////////////////
        /// @brief SubDevice.
        //////////////////////////////////////////////////////////////////////////
        using SubDeviceCommon               = SubDeviceCommonTrait<Traits>;                 //  ml_sub_device_common.h
        #if ML_LINUX
        using SubDevice                     = SubDeviceTrait<Traits>;                       //  linux/ml_sub_device.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Gpu registers commands / command buffer.
        //////////////////////////////////////////////////////////////////////////
        using GpuCommandBuffer              = GpuCommandBufferTrait<Traits>;                //  ml_gpu_command_buffer.h
        using GpuCommandBufferCalculator    = GpuCommandBufferCalculatorTrait<Traits>;      //  ml_gpu_command_buffer.h
        using GpuCommands                   = GpuCommandsTrait<Traits>;                     //  ml_gpu_commands.h
        using GpuRegisters                  = GpuRegistersTrait<Traits>;                    //  ml_gpu_registers.h

        //////////////////////////////////////////////////////////////////////////
        /// @brief Layouts.
        //////////////////////////////////////////////////////////////////////////
        struct Layouts
        {
            using Configuration             = ConfigurationLayoutsTrait<Traits>;            //  ml_configuration_layouts.h
            using OaBuffer                  = OaBufferLayoutsTrait<Traits>;                 //  ml_oa_buffer_layouts.h
            using HwCounters                = HwCountersLayoutsTrait<Traits>;               //  ml_hw_counters_layouts.h
            using GpuCommands               = GpuCommandsLayoutsTrait<Traits>;              //  ml_gpu_commands_include.h
        #if ML_LINUX
            using Override                  = OverrideLayoutsTrait<Traits>;                 //  linux/ml_override_layouts.h
        #endif
            using GpuRegisters              = GpuRegistersLayoutsTrait<Traits>;             //  ml_gpu_registers_layouts.h
        #if ML_LINUX
            using Drm                       = DrmLayoutsTrait<Traits>;                      //  linux/ml_drm_layouts.h
        #endif
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Debug helper.
        //////////////////////////////////////////////////////////////////////////
        #if ML_LINUX
        using Debug                         = DebugTrait<Traits>;                           //  linux/ml_debug_helper.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Markers.
        //////////////////////////////////////////////////////////////////////////
        struct Markers
        {
            using StreamUser                = MarkerStreamUserTrait<Traits>;                //  ml_marker_stream_user.h
        #if ML_LINUX
            using StreamUserExtended        = MarkerStreamUserExtendedTrait<Traits>;        //  linux/ml_marker_stream_user_extended.h
        #endif
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Linux kernel interface traits.
        //////////////////////////////////////////////////////////////////////////
        #if ML_LINUX
        using KernelInterface               = KernelInterfaceTrait<Traits>;                 //  linux/ml_kernel_interface.h
        #endif
        #if ML_LINUX
        using TbsInterface                  = TbsInterfaceTrait<Traits>;                    //  linux/ml_tbs_interface.h
        #endif
        #if ML_LINUX
        using TbsStream                     = TbsStreamTrait<Traits>;                       //  linux/ml_tbs_stream.h
        #endif
        #if ML_LINUX
        using IoControl                     = IoControlTrait<Traits>;                       //  linux/ml_io_control.h
        #endif

        //////////////////////////////////////////////////////////////////////////
        /// @brief Configurations.
        //////////////////////////////////////////////////////////////////////////
        struct Configurations
        {
        #if ML_LINUX
            using HwCountersOa              = ConfigurationHwCountersOaTrait<Traits>;       //  linux/ml_configuration_hw_counters_oa.h
        #endif
            using HwCountersUser            = ConfigurationHwCountersUserTrait<Traits>;     //  ml_configuration_hw_counters_user.h
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Oa buffer support.
        //////////////////////////////////////////////////////////////////////////
        #if 0
        using OaBuffer                      = OaBufferDummyTrait<Traits>;                   //  ml_oa_buffer_dummy.h
        #endif
        using OaBuffer                      = OaBufferMappedTrait<Traits>;                  //  ml_oa_buffer_mapped.h

        //////////////////////////////////////////////////////////////////////////
        /// @brief Overrides.
        //////////////////////////////////////////////////////////////////////////
        struct Overrides
        {
        #if ML_LINUX
            using User                      = OverrideUserTrait<Traits>;                    //  linux/ml_override_user.h
        #endif
            using NullHardware              = OverrideNullHardwareTrait<Traits>;            //  ml_override_null_hardware.h
            using FlushCaches               = OverrideFlushCachesTrait<Traits>;             //  ml_override_flush_caches.h
        #if ML_LINUX
            using PoshQuery                 = OverridePoshQueryTrait<Traits>;               //  linux/ml_override_posh_query.h
        #endif
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Queries.
        //////////////////////////////////////////////////////////////////////////
        struct Queries
        {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Hw Counters.
        //////////////////////////////////////////////////////////////////////////
            using HwCountersSlot              = QueryHwCountersSlotTrait<Traits>;               //  ml_query_hw_counters_slot.h
            using HwCounters                  = QueryHwCountersTrait<Traits>;                   //  ml_query_hw_counters.h
            using HwCountersCalculator        = QueryHwCountersCalculatorTrait<Traits>;         //  ml_query_hw_counters_calculator.h
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Internal features.
        //////////////////////////////////////////////////////////////////////////
        #if ML_LINUX || ML_RELEASE
        using InternalFeature           = InternalFeatureTrait<Traits>;                     //  ml_internal_feature.h
        #endif
    };
} // namespace ML::GEN9::OneApi
#endif //ML_ENABLE_ONEAPI

#undef ML_GEN9
#endif // ML_ENABLE_GEN9
