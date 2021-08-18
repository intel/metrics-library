/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

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
#define __CODEGEN_NAMESPACE_CLOSE }; } }
#define __CODEGEN_PLATFORM "GEN9"
#define __CODEGEN_NAMESPACE_OPEN namespace ML { namespace GEN9 { template<typename T> struct GpuCommandsLayoutsTrait {
#define __CODEGEN_FILE_DIRECTIVES_OPEN ML_STRUCTURE_PACK_BEGIN( 1 )
#define __CODEGEN_FILE_DIRECTIVES_CLOSE ML_STRUCTURE_PACK_END()
#include "ml_gpu_commands_gen9.h"
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_PLATFORM
#endif // ML_ENABLE_GEN9

#if ML_ENABLE_GEN11
// GEN11 IGFX Commands
#define __CODEGEN_NAMESPACE_CLOSE }; } }
#define __CODEGEN_PLATFORM "GEN11"
#define __CODEGEN_NAMESPACE_OPEN namespace ML { namespace GEN11 { template<typename T> struct GpuCommandsLayoutsTrait {
#define __CODEGEN_FILE_DIRECTIVES_OPEN ML_STRUCTURE_PACK_BEGIN( 1 )
#define __CODEGEN_FILE_DIRECTIVES_CLOSE ML_STRUCTURE_PACK_END()
#include "ml_gpu_commands_gen11.h"
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_PLATFORM
#endif // ML_ENABLE_GEN11

#if ML_ENABLE_GEN12
// GEN12 IGFX Commands
#define __CODEGEN_NAMESPACE_CLOSE }; } }
#define __CODEGEN_PLATFORM "GEN12"
#define __CODEGEN_NAMESPACE_OPEN namespace ML { namespace GEN12 { template<typename T> struct GpuCommandsLayoutsTrait {
#define __CODEGEN_FILE_DIRECTIVES_OPEN ML_STRUCTURE_PACK_BEGIN( 1 )
#define __CODEGEN_FILE_DIRECTIVES_CLOSE ML_STRUCTURE_PACK_END()
#include "ml_gpu_commands_gen12.h"
#undef __CODEGEN_NAMESPACE_OPEN
#undef __CODEGEN_PLATFORM
#endif // ML_ENABLE_GEN12

