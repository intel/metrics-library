/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_configuration_gpu.h

@brief Auto-generated file. Allows to compile a given gpu platform.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_configuration_gen
Template:           Tools/MetricsLibraryGenerator/templates/configuration_gen.h
*/

// GEN9
#if !defined( ML_ENABLE_GEN9 )
    #if ML_LINUX
        #define ML_ENABLE_GEN9 1
    #else
        #define ML_ENABLE_GEN9 0
    #endif
#endif

// GEN11
#if !defined( ML_ENABLE_GEN11 )
    #if ML_LINUX
        #define ML_ENABLE_GEN11 1
    #else
        #define ML_ENABLE_GEN11 0
    #endif
#endif

// XE_LP
#if !defined( ML_ENABLE_XE_LP )
    #if ML_LINUX
        #define ML_ENABLE_XE_LP 1
    #else
        #define ML_ENABLE_XE_LP 0
    #endif
#endif

// XE_HP
#if !defined( ML_ENABLE_XE_HP )
    #if ML_LINUX
        #define ML_ENABLE_XE_HP 1
    #else
        #define ML_ENABLE_XE_HP 0
    #endif
#endif

// XE_HPG
#if !defined( ML_ENABLE_XE_HPG )
    #if ML_LINUX
        #define ML_ENABLE_XE_HPG 1
    #else
        #define ML_ENABLE_XE_HPG 0
    #endif
#endif

// XE_HPC
#if !defined( ML_ENABLE_XE_HPC )
    #if ML_LINUX
        #define ML_ENABLE_XE_HPC 1
    #else
        #define ML_ENABLE_XE_HPC 0
    #endif
#endif

// XE2_HPG
#if !defined( ML_ENABLE_XE2_HPG )
    #if ML_LINUX
        #define ML_ENABLE_XE2_HPG 1
    #else
        #define ML_ENABLE_XE2_HPG 0
    #endif
#endif

