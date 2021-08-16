/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi.h

@brief Auto-generated file. Prepares a function table that contains Metrics Library
       entry points for a given os / gen / api.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_tables
Template:           Tools/MetricsLibraryGenerator/templates/function_table_base.h
*/

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
