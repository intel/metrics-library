/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2023 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_configuration_api.h

@brief Auto-generated file. Allows to compile a given api.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_configuration_api
Template:           Tools/MetricsLibraryGenerator/templates/configuration_api.h
*/

// OpenCL
#if ML_LINUX
    #define ML_ENABLE_OPENCL 1
#else
    #define ML_ENABLE_OPENCL 0
#endif

// OneApi
#if ML_LINUX
    #define ML_ENABLE_ONEAPI 1
#else
    #define ML_ENABLE_ONEAPI 0
#endif

