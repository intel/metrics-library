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

@file ml_gpu_commands_include.h

@brief Auto-generated file. Enables gpu commands structures for a given platform.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_gfx.py
Script function:    generate_gfx
Template:           Tools/MetricsLibraryGenerator/templates/gfx.h

\******************************************************************************/

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

