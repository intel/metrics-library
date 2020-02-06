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

@file ml_ddi_gen11.h

@brief Auto-generated file. Prepares a function table that contains Metrics Library
       entry points for a given os / gen / api.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_tables
Template:           Tools/MetricsLibraryGenerator/templates/function_table.h

\******************************************************************************/

#pragma once

#if ML_ENABLE_GEN11
#include "ml_ddi.h"

namespace ML
{
    namespace GEN11
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Function signatures for GEN11.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextCreate_1_0( ClientType_1_0, ContextCreateData_1_0*, ContextHandle_1_0* );
            StatusCode ML_STDCALL ContextDelete_1_0( const ContextHandle_1_0 );
            StatusCode ML_STDCALL GetParameter_1_0( const ParameterType, ValueType*, TypedValue_1_0* );
            StatusCode ML_STDCALL CommandBufferGet_1_0( const CommandBufferData_1_0* );
            StatusCode ML_STDCALL CommandBufferGetSize_1_0( const CommandBufferData_1_0*, CommandBufferSize_1_0* );
            StatusCode ML_STDCALL QueryCreate_1_0( const QueryCreateData_1_0*, QueryHandle_1_0* );
            StatusCode ML_STDCALL QueryDelete_1_0( const QueryHandle_1_0 );
            StatusCode ML_STDCALL OverrideCreate_1_0( const OverrideCreateData_1_0*, OverrideHandle_1_0* );
            StatusCode ML_STDCALL OverrideDelete_1_0( const OverrideHandle_1_0 );
            StatusCode ML_STDCALL ConfigurationCreate_1_0( const ConfigurationCreateData_1_0*, ConfigurationHandle_1_0* );
            StatusCode ML_STDCALL ConfigurationActivate_1_0( const ConfigurationHandle_1_0, const ConfigurationActivateData_1_0* );
            StatusCode ML_STDCALL ConfigurationDeactivate_1_0( const ConfigurationHandle_1_0 );
            StatusCode ML_STDCALL ConfigurationDelete_1_0( const ConfigurationHandle_1_0 );
            StatusCode ML_STDCALL GetData_1_0( GetReportData_1_0* );
            StatusCode ML_STDCALL MarkerCreate_1_0( const MarkerCreateData_1_0*, MarkerHandle_1_0* );
            StatusCode ML_STDCALL MarkerDelete_1_0( const MarkerHandle_1_0 );

            //////////////////////////////////////////////////////////////////////////
            /// @brief DDI function pointers for GEN11.
            //////////////////////////////////////////////////////////////////////////
            template<typename T>
            struct DdiFunctionTable : DdiFunctionTableBase
            {
                //////////////////////////////////////////////////////////////////////////
                /// @brief Initializes all function pointers for GEN11.
                //////////////////////////////////////////////////////////////////////////
                DdiFunctionTable()
                {
                    pfnContextCreate_1_0                          = ContextCreate_1_0;
                    pfnContextDelete_1_0                          = ContextDelete_1_0;
                }

                void GetInterface( Interface_1_0& interface ) const
                {
                    interface = {};
                    interface.GetParameter                        = GetParameter_1_0;
                    interface.CommandBufferGet                    = CommandBufferGet_1_0;
                    interface.CommandBufferGetSize                = CommandBufferGetSize_1_0;
                    interface.QueryCreate                         = QueryCreate_1_0;
                    interface.QueryDelete                         = QueryDelete_1_0;
                    interface.OverrideCreate                      = OverrideCreate_1_0;
                    interface.OverrideDelete                      = OverrideDelete_1_0;
                    interface.ConfigurationCreate                 = ConfigurationCreate_1_0;
                    interface.ConfigurationActivate               = ConfigurationActivate_1_0;
                    interface.ConfigurationDeactivate             = ConfigurationDeactivate_1_0;
                    interface.ConfigurationDelete                 = ConfigurationDelete_1_0;
                    interface.GetData                             = GetData_1_0;
                    interface.MarkerCreate                        = MarkerCreate_1_0;
                    interface.MarkerDelete                        = MarkerDelete_1_0;
                }

                //////////////////////////////////////////////////////////////////////////
                /// @return singleton DdiFunctionTable object.
                //////////////////////////////////////////////////////////////////////////
                static const DdiFunctionTable<T>& GetInstance()
                {
                    static const DdiFunctionTable<T> instance;
                    return instance;
                }
            };
        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Function signatures for GEN11.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL ContextCreate_1_0( ClientType_1_0, ContextCreateData_1_0*, ContextHandle_1_0* );
            StatusCode ML_STDCALL ContextDelete_1_0( const ContextHandle_1_0 );
            StatusCode ML_STDCALL GetParameter_1_0( const ParameterType, ValueType*, TypedValue_1_0* );
            StatusCode ML_STDCALL CommandBufferGet_1_0( const CommandBufferData_1_0* );
            StatusCode ML_STDCALL CommandBufferGetSize_1_0( const CommandBufferData_1_0*, CommandBufferSize_1_0* );
            StatusCode ML_STDCALL QueryCreate_1_0( const QueryCreateData_1_0*, QueryHandle_1_0* );
            StatusCode ML_STDCALL QueryDelete_1_0( const QueryHandle_1_0 );
            StatusCode ML_STDCALL OverrideCreate_1_0( const OverrideCreateData_1_0*, OverrideHandle_1_0* );
            StatusCode ML_STDCALL OverrideDelete_1_0( const OverrideHandle_1_0 );
            StatusCode ML_STDCALL ConfigurationCreate_1_0( const ConfigurationCreateData_1_0*, ConfigurationHandle_1_0* );
            StatusCode ML_STDCALL ConfigurationActivate_1_0( const ConfigurationHandle_1_0, const ConfigurationActivateData_1_0* );
            StatusCode ML_STDCALL ConfigurationDeactivate_1_0( const ConfigurationHandle_1_0 );
            StatusCode ML_STDCALL ConfigurationDelete_1_0( const ConfigurationHandle_1_0 );
            StatusCode ML_STDCALL GetData_1_0( GetReportData_1_0* );
            StatusCode ML_STDCALL MarkerCreate_1_0( const MarkerCreateData_1_0*, MarkerHandle_1_0* );
            StatusCode ML_STDCALL MarkerDelete_1_0( const MarkerHandle_1_0 );

            //////////////////////////////////////////////////////////////////////////
            /// @brief DDI function pointers for GEN11.
            //////////////////////////////////////////////////////////////////////////
            template<typename T>
            struct DdiFunctionTable : DdiFunctionTableBase
            {
                //////////////////////////////////////////////////////////////////////////
                /// @brief Initializes all function pointers for GEN11.
                //////////////////////////////////////////////////////////////////////////
                DdiFunctionTable()
                {
                    pfnContextCreate_1_0                          = ContextCreate_1_0;
                    pfnContextDelete_1_0                          = ContextDelete_1_0;
                }

                void GetInterface( Interface_1_0& interface ) const
                {
                    interface = {};
                    interface.GetParameter                        = GetParameter_1_0;
                    interface.CommandBufferGet                    = CommandBufferGet_1_0;
                    interface.CommandBufferGetSize                = CommandBufferGetSize_1_0;
                    interface.QueryCreate                         = QueryCreate_1_0;
                    interface.QueryDelete                         = QueryDelete_1_0;
                    interface.OverrideCreate                      = OverrideCreate_1_0;
                    interface.OverrideDelete                      = OverrideDelete_1_0;
                    interface.ConfigurationCreate                 = ConfigurationCreate_1_0;
                    interface.ConfigurationActivate               = ConfigurationActivate_1_0;
                    interface.ConfigurationDeactivate             = ConfigurationDeactivate_1_0;
                    interface.ConfigurationDelete                 = ConfigurationDelete_1_0;
                    interface.GetData                             = GetData_1_0;
                    interface.MarkerCreate                        = MarkerCreate_1_0;
                    interface.MarkerDelete                        = MarkerDelete_1_0;
                }

                //////////////////////////////////////////////////////////////////////////
                /// @return singleton DdiFunctionTable object.
                //////////////////////////////////////////////////////////////////////////
                static const DdiFunctionTable<T>& GetInstance()
                {
                    static const DdiFunctionTable<T> instance;
                    return instance;
                }
            };
        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace GEN11
} // namespace ML
#endif // ML_ENABLE_GEN11
