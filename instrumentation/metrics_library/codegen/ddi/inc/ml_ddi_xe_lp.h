/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_xe_lp.h

@brief Auto-generated file. Prepares a function table that contains Metrics Library
       entry points for a given os / gen / api.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_tables
Template:           Tools/MetricsLibraryGenerator/templates/function_table.h
*/

#pragma once

#if ML_ENABLE_XE_LP
#include "ml_ddi.h"

namespace ML
{
    namespace XE_LP
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Function signatures for XE_LP.
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
            /// @brief DDI function pointers for XE_LP.
            //////////////////////////////////////////////////////////////////////////
            template<typename T>
            struct DdiFunctionTable : DdiFunctionTableBase
            {
                //////////////////////////////////////////////////////////////////////////
                /// @brief Initializes all function pointers for XE_LP.
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
            /// @brief Function signatures for XE_LP.
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
            /// @brief DDI function pointers for XE_LP.
            //////////////////////////////////////////////////////////////////////////
            template<typename T>
            struct DdiFunctionTable : DdiFunctionTableBase
            {
                //////////////////////////////////////////////////////////////////////////
                /// @brief Initializes all function pointers for XE_LP.
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

    } // namespace XE_LP
} // namespace ML
#endif // ML_ENABLE_XE_LP