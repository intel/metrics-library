/******************************************************************************\

Copyright © 2021, Intel Corporation

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

@file ml_ddi.h

@brief Auto-generated file. Prepares a function table that contains Metrics Library
       entry points for a given os / gen / api.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_tables
Template:           Tools/MetricsLibraryGenerator/templates/function_table_base.h

\******************************************************************************/

#pragma once

namespace ML
{
    struct DdiFunctionTableBase;
    const DdiFunctionTableBase* GetDdiFunctionTable( const ClientType_1_0& clientType );
    const DdiFunctionTableBase* GetDdiFunctionTable( const ContextHandle_1_0& handle );

    //////////////////////////////////////////////////////////////////////////
    // @brief DDI function pointers.
    //////////////////////////////////////////////////////////////////////////
    struct DdiFunctionTableBase
    {
        ContextCreateFunction_1_0                     pfnContextCreate_1_0;
        ContextDeleteFunction_1_0                     pfnContextDelete_1_0;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Initializes all function pointers with null functions.
        //////////////////////////////////////////////////////////////////////////
        DdiFunctionTableBase()
        {
            pfnContextCreate_1_0                      = nullptr;
            pfnContextDelete_1_0                      = nullptr;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Virtual GetInterface function.
        //////////////////////////////////////////////////////////////////////////
        virtual void GetInterface( Interface_1_0& interface ) const = 0;
    };
} // namespace ML
