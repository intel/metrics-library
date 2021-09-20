/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_configuration_gpu.h

@brief Auto-generated file. Allows to compile a given gen platform.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_configuration_gen
Template:           Tools/MetricsLibraryGenerator/templates/configuration_gen.h
*/

// GEN9
#if ML_LINUX
    #define ML_ENABLE_GEN9 1
#else
    #define ML_ENABLE_GEN9 0
#endif

// GEN11
#if ML_LINUX
    #define ML_ENABLE_GEN11 1
#else
    #define ML_ENABLE_GEN11 0
#endif

// XE_LP
#if ML_LINUX
    #define ML_ENABLE_XE_LP 1
#else
    #define ML_ENABLE_XE_LP 0
#endif
