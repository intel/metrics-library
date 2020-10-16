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

@file ml_traits_gen11.h

@brief Auto-generated file. Creates Traits structures for a given gen / os / api.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_traits
Template:           Tools/MetricsLibraryGenerator/templates/traits.h

\******************************************************************************/

#pragma once

#if ML_ENABLE_GEN11
#define ML_GEN11 1

#include "ml_ddi_gen11.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates ML traits for GEN11.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN11
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
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
                template <typename ResultT> using FunctionLogCommon = FunctionLogCommonTrait<Traits,  ResultT>; //  ml_function_log_common.h
                                            using FunctionLogStatic = FunctionLog<StatusCode>;

                //////////////////////////////////////////////////////////////////////////
                /// @brief Library context.
                //////////////////////////////////////////////////////////////////////////
                using Context                       = ContextTrait<Traits>;                         //  ml_context.h
                using ClientOptions                 = ClientOptionsTrait<Traits>;                   //  ml_client_options.h

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
                #if ML_LINUX
                    using IoControl                 = IoControlLayoutsTrait;                        //  linux/ml_io_control_layouts.h
                #endif
                    using GpuCommands               = GpuCommandsLayoutsTrait<Traits>;              //  ml_gpu_commands_include.h
                #if ML_LINUX
                    using Override                  = OverrideLayoutsTrait<Traits>;                 //  linux/ml_override_layouts.h
                #endif
                    using PipelineTimestamps        = PipelineTimestampsLayoutsTrait<Traits>;       //  ml_pipeline_timestamps_layouts.h
                    using GpuRegisters              = GpuRegistersLayoutsTrait<Traits>;             //  ml_gpu_registers_layouts.h
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
                using TbsInterface                  = TbsInterfaceTrait<Traits>;                    //  linux/ml_tbs_interface.h
                #endif
                #if ML_LINUX
                using KernelInterface               = KernelInterfaceTrait<Traits>;                 //  linux/ml_kernel_interface.h
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
                    using HwCounters                  = QueryHwCountersTrait<Traits>;                   //  ml_query_hw_counters.h
                #if 1
                    using HwCountersCalculatorMain    = QueryHwCountersCalculatorTrait<Traits>;         //  ml_query_hw_counters_calculator.h
                #endif
                #if 1
                    using HwCountersCalculator        = HwCountersCalculatorMain;
                #endif

                //////////////////////////////////////////////////////////////////////////
                /// @brief Pipeline timestamps.
                //////////////////////////////////////////////////////////////////////////
                    using PipelineTimestamps          = QueryPipelineTimestampsTrait<Traits>;           //  ml_query_pipeline_timestamps.h
                };

                static void PrintBuildNumber()
                {
                #if ML_DEBUG || ML_INTERNAL
                    FunctionLogStatic::Traits( "Major number", METRICS_LIBRARY_MAJOR_NUMBER );
                    FunctionLogStatic::Traits( "Minor number", METRICS_LIBRARY_MINOR_NUMBER );
                    FunctionLogStatic::Traits( "Build number", METRICS_LIBRARY_BUILD_NUMBER );
                #endif
                }
                static void PrintClient()
                {
                #if ML_DEBUG || ML_INTERNAL
                    FunctionLogStatic::Traits( "Client", "OpenCL" );
                #endif
                }
                static void PrintGen()
                {
                #if ML_DEBUG || ML_INTERNAL
                    FunctionLogStatic::Traits( "Gen", "GEN11" );
                #endif
                }
                static void PrintTraits()
                {
                #if ML_DEBUG || ML_INTERNAL
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using ConstantsOs", ConstantsOs::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Tools", Tools::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using ToolsOs", ToolsOs::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using FunctionLogStatic", FunctionLogStatic::GetDescription() );
                    FunctionLogStatic::Traits( "using Context", Context::GetDescription() );
                    FunctionLogStatic::Traits( "using ClientOptions", ClientOptions::GetDescription() );
                    FunctionLogStatic::Traits( "using GpuCommandBuffer", GpuCommandBuffer::GetDescription() );
                    FunctionLogStatic::Traits( "using GpuCommandBufferCalculator", GpuCommandBufferCalculator::GetDescription() );
                    FunctionLogStatic::Traits( "using GpuCommands", GpuCommands::GetDescription() );
                    FunctionLogStatic::Traits( "using GpuRegisters", GpuRegisters::GetDescription() );
                    FunctionLogStatic::Traits( "using Layouts::Configuration", Layouts::Configuration::GetDescription() );
                    FunctionLogStatic::Traits( "using Layouts::OaBuffer", Layouts::OaBuffer::GetDescription() );
                    FunctionLogStatic::Traits( "using Layouts::HwCounters", Layouts::HwCounters::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Layouts::IoControl", Layouts::IoControl::GetDescription() );
                #endif
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Layouts::Override", Layouts::Override::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Layouts::PipelineTimestamps", Layouts::PipelineTimestamps::GetDescription() );
                    FunctionLogStatic::Traits( "using Layouts::GpuRegisters", Layouts::GpuRegisters::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Debug", Debug::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Markers::StreamUser", Markers::StreamUser::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Markers::StreamUserExtended", Markers::StreamUserExtended::GetDescription() );
                #endif
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using TbsInterface", TbsInterface::GetDescription() );
                #endif
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using KernelInterface", KernelInterface::GetDescription() );
                #endif
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using IoControl", IoControl::GetDescription() );
                #endif
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Configurations::HwCountersOa", Configurations::HwCountersOa::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Configurations::HwCountersUser", Configurations::HwCountersUser::GetDescription() );
                #if 0
                    FunctionLogStatic::Traits( "using OaBuffer", OaBuffer::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using OaBuffer", OaBuffer::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Overrides::User", Overrides::User::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Overrides::NullHardware", Overrides::NullHardware::GetDescription() );
                    FunctionLogStatic::Traits( "using Overrides::FlushCaches", Overrides::FlushCaches::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Overrides::PoshQuery", Overrides::PoshQuery::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Queries::HwCounters", Queries::HwCounters::GetDescription() );
                #if 1
                    FunctionLogStatic::Traits( "using Queries::HwCountersCalculatorMain", Queries::HwCountersCalculatorMain::GetDescription() );
                #endif
                #if 1
                    FunctionLogStatic::Traits( "using Queries::HwCountersCalculator", Queries::HwCountersCalculator::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Queries::PipelineTimestamps", Queries::PipelineTimestamps::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Policy", Policy::GetDescription() );
                #endif
                #endif
                }
            };
        } // namespace OpenCL
        #endif //ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
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
                template <typename ResultT> using FunctionLogCommon = FunctionLogCommonTrait<Traits,  ResultT>; //  ml_function_log_common.h
                                            using FunctionLogStatic = FunctionLog<StatusCode>;

                //////////////////////////////////////////////////////////////////////////
                /// @brief Library context.
                //////////////////////////////////////////////////////////////////////////
                using Context                       = ContextTrait<Traits>;                         //  ml_context.h
                using ClientOptions                 = ClientOptionsTrait<Traits>;                   //  ml_client_options.h

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
                #if ML_LINUX
                    using IoControl                 = IoControlLayoutsTrait;                        //  linux/ml_io_control_layouts.h
                #endif
                    using GpuCommands               = GpuCommandsLayoutsTrait<Traits>;              //  ml_gpu_commands_include.h
                #if ML_LINUX
                    using Override                  = OverrideLayoutsTrait<Traits>;                 //  linux/ml_override_layouts.h
                #endif
                    using PipelineTimestamps        = PipelineTimestampsLayoutsTrait<Traits>;       //  ml_pipeline_timestamps_layouts.h
                    using GpuRegisters              = GpuRegistersLayoutsTrait<Traits>;             //  ml_gpu_registers_layouts.h
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
                using TbsInterface                  = TbsInterfaceTrait<Traits>;                    //  linux/ml_tbs_interface.h
                #endif
                #if ML_LINUX
                using KernelInterface               = KernelInterfaceTrait<Traits>;                 //  linux/ml_kernel_interface.h
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
                    using HwCounters                  = QueryHwCountersTrait<Traits>;                   //  ml_query_hw_counters.h
                #if 1
                    using HwCountersCalculatorMain    = QueryHwCountersCalculatorTrait<Traits>;         //  ml_query_hw_counters_calculator.h
                #endif
                #if 1
                    using HwCountersCalculator        = HwCountersCalculatorMain;
                #endif

                //////////////////////////////////////////////////////////////////////////
                /// @brief Pipeline timestamps.
                //////////////////////////////////////////////////////////////////////////
                    using PipelineTimestamps          = QueryPipelineTimestampsTrait<Traits>;           //  ml_query_pipeline_timestamps.h
                };

                static void PrintBuildNumber()
                {
                #if ML_DEBUG || ML_INTERNAL
                    FunctionLogStatic::Traits( "Major number", METRICS_LIBRARY_MAJOR_NUMBER );
                    FunctionLogStatic::Traits( "Minor number", METRICS_LIBRARY_MINOR_NUMBER );
                    FunctionLogStatic::Traits( "Build number", METRICS_LIBRARY_BUILD_NUMBER );
                #endif
                }
                static void PrintClient()
                {
                #if ML_DEBUG || ML_INTERNAL
                    FunctionLogStatic::Traits( "Client", "OneApi" );
                #endif
                }
                static void PrintGen()
                {
                #if ML_DEBUG || ML_INTERNAL
                    FunctionLogStatic::Traits( "Gen", "GEN11" );
                #endif
                }
                static void PrintTraits()
                {
                #if ML_DEBUG || ML_INTERNAL
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using ConstantsOs", ConstantsOs::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Tools", Tools::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using ToolsOs", ToolsOs::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using FunctionLogStatic", FunctionLogStatic::GetDescription() );
                    FunctionLogStatic::Traits( "using Context", Context::GetDescription() );
                    FunctionLogStatic::Traits( "using ClientOptions", ClientOptions::GetDescription() );
                    FunctionLogStatic::Traits( "using GpuCommandBuffer", GpuCommandBuffer::GetDescription() );
                    FunctionLogStatic::Traits( "using GpuCommandBufferCalculator", GpuCommandBufferCalculator::GetDescription() );
                    FunctionLogStatic::Traits( "using GpuCommands", GpuCommands::GetDescription() );
                    FunctionLogStatic::Traits( "using GpuRegisters", GpuRegisters::GetDescription() );
                    FunctionLogStatic::Traits( "using Layouts::Configuration", Layouts::Configuration::GetDescription() );
                    FunctionLogStatic::Traits( "using Layouts::OaBuffer", Layouts::OaBuffer::GetDescription() );
                    FunctionLogStatic::Traits( "using Layouts::HwCounters", Layouts::HwCounters::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Layouts::IoControl", Layouts::IoControl::GetDescription() );
                #endif
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Layouts::Override", Layouts::Override::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Layouts::PipelineTimestamps", Layouts::PipelineTimestamps::GetDescription() );
                    FunctionLogStatic::Traits( "using Layouts::GpuRegisters", Layouts::GpuRegisters::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Debug", Debug::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Markers::StreamUser", Markers::StreamUser::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Markers::StreamUserExtended", Markers::StreamUserExtended::GetDescription() );
                #endif
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using TbsInterface", TbsInterface::GetDescription() );
                #endif
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using KernelInterface", KernelInterface::GetDescription() );
                #endif
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using IoControl", IoControl::GetDescription() );
                #endif
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Configurations::HwCountersOa", Configurations::HwCountersOa::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Configurations::HwCountersUser", Configurations::HwCountersUser::GetDescription() );
                #if 0
                    FunctionLogStatic::Traits( "using OaBuffer", OaBuffer::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using OaBuffer", OaBuffer::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Overrides::User", Overrides::User::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Overrides::NullHardware", Overrides::NullHardware::GetDescription() );
                    FunctionLogStatic::Traits( "using Overrides::FlushCaches", Overrides::FlushCaches::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Overrides::PoshQuery", Overrides::PoshQuery::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Queries::HwCounters", Queries::HwCounters::GetDescription() );
                #if 1
                    FunctionLogStatic::Traits( "using Queries::HwCountersCalculatorMain", Queries::HwCountersCalculatorMain::GetDescription() );
                #endif
                #if 1
                    FunctionLogStatic::Traits( "using Queries::HwCountersCalculator", Queries::HwCountersCalculator::GetDescription() );
                #endif
                    FunctionLogStatic::Traits( "using Queries::PipelineTimestamps", Queries::PipelineTimestamps::GetDescription() );
                #if ML_LINUX
                    FunctionLogStatic::Traits( "using Policy", Policy::GetDescription() );
                #endif
                #endif
                }
            };
        } // namespace OneApi
        #endif //ML_ENABLE_ONEAPI

    } // namespace GEN11
} // namespace ML
#undef ML_GEN11
#endif // ML_ENABLE_GEN11
