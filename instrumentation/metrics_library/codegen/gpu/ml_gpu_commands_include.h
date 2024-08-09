/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_gpu_commands_include.h

@brief Auto-generated file. Enables gpu commands structures for a given platform.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_gfx.py
Script function:    generate_gfx
Template:           Tools/MetricsLibraryGenerator/templates/gfx.h
*/

#pragma once


#if ML_ENABLE_GEN9
// GEN9 IGFX Commands
#define __CODEGEN_NAMESPACE_CLOSE }; }
#define __CODEGEN_PLATFORM "GEN9"
#define __CODEGEN_NAMESPACE_OPEN namespace ML::GEN9 { template <typename T> struct GpuCommandsLayoutsTrait {
#define __CODEGEN_FILE_DIRECTIVES_OPEN ML_STRUCTURE_PACK_BEGIN( 1 )
#define __CODEGEN_FILE_DIRECTIVES_CLOSE ML_STRUCTURE_PACK_END()
#include "ml_gpu_commands_gen9.h"
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_PLATFORM
#endif // ML_ENABLE_GEN9

#if ML_ENABLE_GEN11
// GEN11 IGFX Commands
#define __CODEGEN_NAMESPACE_CLOSE }; }
#define __CODEGEN_PLATFORM "GEN11"
#define __CODEGEN_NAMESPACE_OPEN namespace ML::GEN11 { template <typename T> struct GpuCommandsLayoutsTrait {
#define __CODEGEN_FILE_DIRECTIVES_OPEN ML_STRUCTURE_PACK_BEGIN( 1 )
#define __CODEGEN_FILE_DIRECTIVES_CLOSE ML_STRUCTURE_PACK_END()
#include "ml_gpu_commands_gen11.h"
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_PLATFORM
#endif // ML_ENABLE_GEN11

#if ML_ENABLE_XE_LP
// XE_LP IGFX Commands
#define __CODEGEN_NAMESPACE_CLOSE }; }
#define __CODEGEN_PLATFORM "XE_LP"
#define __CODEGEN_NAMESPACE_OPEN namespace ML::XE_LP { template <typename T> struct GpuCommandsLayoutsTrait {
#define __CODEGEN_FILE_DIRECTIVES_OPEN ML_STRUCTURE_PACK_BEGIN( 1 )
#define __CODEGEN_FILE_DIRECTIVES_CLOSE ML_STRUCTURE_PACK_END()
#include "ml_gpu_commands_xe_lp.h"
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_PLATFORM
#endif // ML_ENABLE_XE_LP

#if ML_ENABLE_XE_HP
// XE_HP IGFX Commands
#define __CODEGEN_NAMESPACE_CLOSE }; }
#define __CODEGEN_PLATFORM "XE_HP"
#define __CODEGEN_NAMESPACE_OPEN namespace ML::XE_HP { template <typename T> struct GpuCommandsLayoutsTrait {
#define __CODEGEN_FILE_DIRECTIVES_OPEN ML_STRUCTURE_PACK_BEGIN( 1 )
#define __CODEGEN_FILE_DIRECTIVES_CLOSE ML_STRUCTURE_PACK_END()
#include "ml_gpu_commands_xe_hp.h"
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_PLATFORM
#endif // ML_ENABLE_XE_HP

#if ML_ENABLE_XE_HPG
// XE_HPG IGFX Commands
#define __CODEGEN_NAMESPACE_CLOSE }; }
#define __CODEGEN_PLATFORM "XE_HPG"
#define __CODEGEN_NAMESPACE_OPEN namespace ML::XE_HPG { template <typename T> struct GpuCommandsLayoutsTrait {
#define __CODEGEN_FILE_DIRECTIVES_OPEN ML_STRUCTURE_PACK_BEGIN( 1 )
#define __CODEGEN_FILE_DIRECTIVES_CLOSE ML_STRUCTURE_PACK_END()
#include "ml_gpu_commands_xe_hpg.h"
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_PLATFORM
#endif // ML_ENABLE_XE_HPG

#if ML_ENABLE_XE_HPC
// XE_HPC IGFX Commands
#define __CODEGEN_NAMESPACE_CLOSE }; }
#define __CODEGEN_PLATFORM "XE_HPC"
#define __CODEGEN_NAMESPACE_OPEN namespace ML::XE_HPC { template <typename T> struct GpuCommandsLayoutsTrait {
#define __CODEGEN_FILE_DIRECTIVES_OPEN ML_STRUCTURE_PACK_BEGIN( 1 )
#define __CODEGEN_FILE_DIRECTIVES_CLOSE ML_STRUCTURE_PACK_END()
#include "ml_gpu_commands_xe_hpc.h"
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_PLATFORM
#endif // ML_ENABLE_XE_HPC

#if ML_ENABLE_XE2_HPG
// XE2_HPG IGFX Commands
#define __CODEGEN_NAMESPACE_CLOSE }; }
#define __CODEGEN_PLATFORM "XE2_HPG"
#define __CODEGEN_NAMESPACE_OPEN namespace ML::XE2_HPG { template <typename T> struct GpuCommandsLayoutsTrait {
#define __CODEGEN_FILE_DIRECTIVES_OPEN ML_STRUCTURE_PACK_BEGIN( 1 )
#define __CODEGEN_FILE_DIRECTIVES_CLOSE ML_STRUCTURE_PACK_END()
#include "ml_gpu_commands_xe2_hpg.h"
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_PLATFORM
#endif // ML_ENABLE_XE2_HPG

