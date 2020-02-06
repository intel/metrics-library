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

@file ml_configuration_gen.h

@brief Auto-generated file. Allows to compile a given gen platform.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_configuration_gen
Template:           Tools/MetricsLibraryGenerator/templates/configuration_gen.h

\******************************************************************************/

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

