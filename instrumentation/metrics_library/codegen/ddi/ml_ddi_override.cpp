/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_override.cpp

@brief Auto-generated file. Implements Metrics Library entry points
       for a given os / gen / api configuration.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_ddi
Template:           Tools/MetricsLibraryGenerator/templates/ddi.h
*/

#include "ml_pch.h"
#include "ml_ddi_override.hpp"

#if ML_ENABLE_GEN9

#include "ml_traits_gen9.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiOverride.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN9
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideCreate_1_0(
                const OverrideCreateData_1_0* createData,
                OverrideHandle_1_0*           handle )
            {
                return DdiOverride<T>::OverrideCreate_1_0( createData, handle );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideDelete_1_0(
                const OverrideHandle_1_0 handle )
            {
                return DdiOverride<T>::OverrideDelete_1_0( handle );
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideCreate_1_0(
                const OverrideCreateData_1_0* createData,
                OverrideHandle_1_0*           handle )
            {
                return DdiOverride<T>::OverrideCreate_1_0( createData, handle );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideDelete_1_0(
                const OverrideHandle_1_0 handle )
            {
                return DdiOverride<T>::OverrideDelete_1_0( handle );
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
/// @brief Instantiates functions for DdiOverride.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN11
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideCreate_1_0(
                const OverrideCreateData_1_0* createData,
                OverrideHandle_1_0*           handle )
            {
                return DdiOverride<T>::OverrideCreate_1_0( createData, handle );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideDelete_1_0(
                const OverrideHandle_1_0 handle )
            {
                return DdiOverride<T>::OverrideDelete_1_0( handle );
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideCreate_1_0(
                const OverrideCreateData_1_0* createData,
                OverrideHandle_1_0*           handle )
            {
                return DdiOverride<T>::OverrideCreate_1_0( createData, handle );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideDelete_1_0(
                const OverrideHandle_1_0 handle )
            {
                return DdiOverride<T>::OverrideDelete_1_0( handle );
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace GEN11
} // namespace ML
#endif // ML_ENABLE_GEN11

#if ML_ENABLE_XE_LP

#include "ml_traits_xe_lp.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiOverride.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace XE_LP
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideCreate_1_0(
                const OverrideCreateData_1_0* createData,
                OverrideHandle_1_0*           handle )
            {
                return DdiOverride<T>::OverrideCreate_1_0( createData, handle );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideDelete_1_0(
                const OverrideHandle_1_0 handle )
            {
                return DdiOverride<T>::OverrideDelete_1_0( handle );
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideCreate_1_0(
                const OverrideCreateData_1_0* createData,
                OverrideHandle_1_0*           handle )
            {
                return DdiOverride<T>::OverrideCreate_1_0( createData, handle );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for OverrideDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL OverrideDelete_1_0(
                const OverrideHandle_1_0 handle )
            {
                return DdiOverride<T>::OverrideDelete_1_0( handle );
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace XE_LP
} // namespace ML
#endif // ML_ENABLE_XE_LP

