/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_traits_include.h

@brief Auto-generated file. Creates a single header with all traits header files.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_traits_include
Template:           Tools/MetricsLibraryGenerator/templates/traits_include.h
*/

//////////////////////////////////////////////////////////////////////////
/// @brief Common.
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @brief Constants:
//////////////////////////////////////////////////////////////////////////
#if ML_LINUX
    #include "linux/ml_constants_os.h"
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Tools:
//////////////////////////////////////////////////////////////////////////
#include "ml_tools.h"
#if ML_LINUX
    #include "linux/ml_tools_os.h"
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Logs:
//////////////////////////////////////////////////////////////////////////
#if ML_DEBUG || ML_INTERNAL
    #include "ml_function_log.h"
#endif
#if ML_RELEASE
    #include "ml_function_log_release.h"
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Library context:
//////////////////////////////////////////////////////////////////////////
#include "ml_context.h"
#include "ml_client_options.h"
#include "ml_ddi_objects.h"
#include "ml_state.h"

//////////////////////////////////////////////////////////////////////////
/// @brief SubDevice:
//////////////////////////////////////////////////////////////////////////
#include "ml_sub_device_common.h"
#if ML_LINUX
    #include "linux/ml_sub_device.h"
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Gpu registers commands / command buffer:
//////////////////////////////////////////////////////////////////////////
#include "ml_gpu_command_buffer.h"
#include "ml_gpu_commands.h"
#include "ml_gpu_registers.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Layouts:
//////////////////////////////////////////////////////////////////////////
#include "ml_configuration_layouts.h"
#include "ml_oa_buffer_layouts.h"
#include "ml_hw_counters_layouts.h"
#if ML_LINUX
    #include "linux/ml_io_control_layouts.h"
#endif
#include "ml_gpu_commands_include.h"
#if ML_LINUX
    #include "linux/ml_override_layouts.h"
#endif
#include "ml_pipeline_timestamps_layouts.h"
#include "ml_gpu_registers_layouts.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Debug helper:
//////////////////////////////////////////////////////////////////////////
#if ML_LINUX
    #include "linux/ml_debug_helper.h"
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Markers:
//////////////////////////////////////////////////////////////////////////
#include "ml_marker_stream_user.h"
#if ML_LINUX
    #include "linux/ml_marker_stream_user_extended.h"
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Linux kernel interface traits:
//////////////////////////////////////////////////////////////////////////
#if ML_LINUX
    #include "linux/ml_tbs_interface.h"
#endif
#if ML_LINUX
    #include "linux/ml_kernel_interface.h"
#endif
#if ML_LINUX
    #include "linux/ml_io_control.h"
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Configurations:
//////////////////////////////////////////////////////////////////////////
#if ML_LINUX
    #include "linux/ml_configuration_hw_counters_oa.h"
#endif
#include "ml_configuration_hw_counters_user.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Oa buffer support:
//////////////////////////////////////////////////////////////////////////
#if 0
    #include "ml_oa_buffer_dummy.h"
#endif
#include "ml_oa_buffer_mapped.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Overrides:
//////////////////////////////////////////////////////////////////////////
#if ML_LINUX
    #include "linux/ml_override_user.h"
#endif
#include "ml_override_null_hardware.h"
#include "ml_override_flush_caches.h"
#if ML_LINUX
    #include "linux/ml_override_posh_query.h"
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief Queries:
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// @brief Hw Counters:
//////////////////////////////////////////////////////////////////////////
#include "ml_query_hw_counters.h"
#include "ml_query_hw_counters_calculator.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Pipeline timestamps:
//////////////////////////////////////////////////////////////////////////
#include "ml_query_pipeline_timestamps.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Internal features:
//////////////////////////////////////////////////////////////////////////
#if ML_LINUX || ML_RELEASE
    #include "ml_internal_feature.h"
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief OpenCL.
//////////////////////////////////////////////////////////////////////////
#if ML_ENABLE_OPENCL
    #if ML_LINUX
        #include "linux/ml_policy_opencl.h"
    #endif
#endif

//////////////////////////////////////////////////////////////////////////
/// @brief OneApi.
//////////////////////////////////////////////////////////////////////////
#if ML_ENABLE_ONEAPI
    #if ML_LINUX
        #include "linux/ml_policy_oneapi.h"
    #endif
#endif

