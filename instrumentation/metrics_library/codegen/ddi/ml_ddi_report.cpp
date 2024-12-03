/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

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
#include "ml_traits_include.h"
#include "ml_ddi_report.hpp"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

#if ML_ENABLE_GEN9

#include "ml_traits_gen9.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiReport.
//////////////////////////////////////////////////////////////////////////
#if ML_ENABLE_OPENCL
namespace ML::GEN9::OpenCL
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::GEN9::OpenCL
#endif // ML_ENABLE_OPENCL

#if ML_ENABLE_ONEAPI
namespace ML::GEN9::OneApi
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::GEN9::OneApi
#endif // ML_ENABLE_ONEAPI

#endif // ML_ENABLE_GEN9

#if ML_ENABLE_GEN11

#include "ml_traits_gen11.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiReport.
//////////////////////////////////////////////////////////////////////////
#if ML_ENABLE_OPENCL
namespace ML::GEN11::OpenCL
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::GEN11::OpenCL
#endif // ML_ENABLE_OPENCL

#if ML_ENABLE_ONEAPI
namespace ML::GEN11::OneApi
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::GEN11::OneApi
#endif // ML_ENABLE_ONEAPI

#endif // ML_ENABLE_GEN11

#if ML_ENABLE_XE_LP

#include "ml_traits_xe_lp.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiReport.
//////////////////////////////////////////////////////////////////////////
#if ML_ENABLE_OPENCL
namespace ML::XE_LP::OpenCL
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::XE_LP::OpenCL
#endif // ML_ENABLE_OPENCL

#if ML_ENABLE_ONEAPI
namespace ML::XE_LP::OneApi
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::XE_LP::OneApi
#endif // ML_ENABLE_ONEAPI

#endif // ML_ENABLE_XE_LP

#if ML_ENABLE_XE_HPG

#include "ml_traits_xe_hpg.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiReport.
//////////////////////////////////////////////////////////////////////////
#if ML_ENABLE_OPENCL
namespace ML::XE_HPG::OpenCL
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::XE_HPG::OpenCL
#endif // ML_ENABLE_OPENCL

#if ML_ENABLE_ONEAPI
namespace ML::XE_HPG::OneApi
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::XE_HPG::OneApi
#endif // ML_ENABLE_ONEAPI

#endif // ML_ENABLE_XE_HPG

#if ML_ENABLE_XE_HPC

#include "ml_traits_xe_hpc.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiReport.
//////////////////////////////////////////////////////////////////////////
#if ML_ENABLE_OPENCL
namespace ML::XE_HPC::OpenCL
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::XE_HPC::OpenCL
#endif // ML_ENABLE_OPENCL

#if ML_ENABLE_ONEAPI
namespace ML::XE_HPC::OneApi
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::XE_HPC::OneApi
#endif // ML_ENABLE_ONEAPI

#endif // ML_ENABLE_XE_HPC

#if ML_ENABLE_XE2_HPG

#include "ml_traits_xe2_hpg.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiReport.
//////////////////////////////////////////////////////////////////////////
#if ML_ENABLE_OPENCL
namespace ML::XE2_HPG::OpenCL
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::XE2_HPG::OpenCL
#endif // ML_ENABLE_OPENCL

#if ML_ENABLE_ONEAPI
namespace ML::XE2_HPG::OneApi
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Instantiates template function for GetData_1_0.
    //////////////////////////////////////////////////////////////////////////
    StatusCode ML_STDCALL GetData_1_0(
        GetReportData_1_0* data )
    {
        return DdiReport<T>::GetData_1_0( data );
    }

} // namespace ML::XE2_HPG::OneApi
#endif // ML_ENABLE_ONEAPI

#endif // ML_ENABLE_XE2_HPG

