/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_configuration_gen.h

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

// GEN12
#if ML_LINUX
    #define ML_ENABLE_GEN12 1
#else
    #define ML_ENABLE_GEN12 0
#endif

