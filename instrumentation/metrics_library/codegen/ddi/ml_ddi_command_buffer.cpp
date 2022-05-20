/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_command_buffer.cpp

@brief Auto-generated file. Implements Metrics Library entry points
       for a given os / gen / api configuration.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_ddi
Template:           Tools/MetricsLibraryGenerator/templates/ddi.h
*/

#include "ml_pch.h"
#include "ml_ddi_command_buffer.hpp"

#if ML_ENABLE_GEN9

#include "ml_traits_gen9.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiCommandBuffer.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN9
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
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
/// @brief Instantiates functions for DdiCommandBuffer.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN11
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
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
/// @brief Instantiates functions for DdiCommandBuffer.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace XE_LP
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace XE_LP
} // namespace ML
#endif // ML_ENABLE_XE_LP

#if ML_ENABLE_XE_HP

#include "ml_traits_xe_hp.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiCommandBuffer.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace XE_HP
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace XE_HP
} // namespace ML
#endif // ML_ENABLE_XE_HP

#if ML_ENABLE_XE_HPG

#include "ml_traits_xe_hpg.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiCommandBuffer.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace XE_HPG
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace XE_HPG
} // namespace ML
#endif // ML_ENABLE_XE_HPG

#if ML_ENABLE_XE_HPC

#include "ml_traits_xe_hpc.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiCommandBuffer.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace XE_HPC
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGet_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGet_1_0(
                const CommandBufferData_1_0* data )
            {
                return DdiCommandBuffer<T>::CommandBufferGet_1_0( data );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for CommandBufferGetSize_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL CommandBufferGetSize_1_0(
                const CommandBufferData_1_0* data,
                CommandBufferSize_1_0*       size )
            {
                return DdiCommandBuffer<T>::CommandBufferGetSize_1_0( data, size );
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace XE_HPC
} // namespace ML
#endif // ML_ENABLE_XE_HPC

