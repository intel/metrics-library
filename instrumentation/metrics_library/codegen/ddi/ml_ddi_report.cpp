/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_report.cpp

@brief Auto-generated file. Implements Metrics Library entry points
       for a given os / gen / api configuration.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_ddi
Template:           Tools/MetricsLibraryGenerator/templates/ddi.h
*/

#include "ml_pch.h"
#include "ml_ddi_report.hpp"

#if ML_ENABLE_GEN9

#include "ml_traits_gen9.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiReport.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN9
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetData_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetData_1_0(
                GetReportData_1_0* data )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( data );

                // Call entry point implementation.
                log.m_Result = DdiReport<T>::GetData_1_0(
                    data );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success || log.m_Result == StatusCode::ReportNotReady );

                return log.m_Result;
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetData_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetData_1_0(
                GetReportData_1_0* data )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( data );

                // Call entry point implementation.
                log.m_Result = DdiReport<T>::GetData_1_0(
                    data );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success || log.m_Result == StatusCode::ReportNotReady );

                return log.m_Result;
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace GEN9
} // namespace ML
#endif // ML_ENABLE_GEN9

#if ML_ENABLE_GEN11

#include "ml_traits_gen11.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiReport.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN11
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetData_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetData_1_0(
                GetReportData_1_0* data )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( data );

                // Call entry point implementation.
                log.m_Result = DdiReport<T>::GetData_1_0(
                    data );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success || log.m_Result == StatusCode::ReportNotReady );

                return log.m_Result;
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetData_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetData_1_0(
                GetReportData_1_0* data )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( data );

                // Call entry point implementation.
                log.m_Result = DdiReport<T>::GetData_1_0(
                    data );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success || log.m_Result == StatusCode::ReportNotReady );

                return log.m_Result;
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace GEN11
} // namespace ML
#endif // ML_ENABLE_GEN11

#if ML_ENABLE_GEN12

#include "ml_traits_gen12.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiReport.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN12
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetData_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetData_1_0(
                GetReportData_1_0* data )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( data );

                // Call entry point implementation.
                log.m_Result = DdiReport<T>::GetData_1_0(
                    data );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success || log.m_Result == StatusCode::ReportNotReady );

                return log.m_Result;
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for GetData_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL GetData_1_0(
                GetReportData_1_0* data )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( data );

                // Call entry point implementation.
                log.m_Result = DdiReport<T>::GetData_1_0(
                    data );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success || log.m_Result == StatusCode::ReportNotReady );

                return log.m_Result;
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace GEN12
} // namespace ML
#endif // ML_ENABLE_GEN12

