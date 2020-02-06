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

@file ml_ddi_marker.cpp

@brief Auto-generated file. Implements Metrics Library entry points
       for a given os / gen / api configuration.

DO NOT EDIT

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_ddi
Template:           Tools/MetricsLibraryGenerator/templates/ddi.h

\******************************************************************************/

#include "ml_pch.h"
#include "ml_ddi_marker.hpp"

#if ML_ENABLE_GEN9

#include "ml_traits_gen9.h"

//////////////////////////////////////////////////////////////////////////
/// @brief T is not available in this file.
//////////////////////////////////////////////////////////////////////////
#define T Traits

//////////////////////////////////////////////////////////////////////////
/// @brief Instantiates functions for DdiMarker.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN9
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerCreate_1_0(
                const MarkerCreateData_1_0* createData,
                MarkerHandle_1_0*           handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerCreate_1_0(
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerDelete_1_0(
                const MarkerHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerCreate_1_0(
                const MarkerCreateData_1_0* createData,
                MarkerHandle_1_0*           handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerCreate_1_0(
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerDelete_1_0(
                const MarkerHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

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
/// @brief Instantiates functions for DdiMarker.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN11
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerCreate_1_0(
                const MarkerCreateData_1_0* createData,
                MarkerHandle_1_0*           handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerCreate_1_0(
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerDelete_1_0(
                const MarkerHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerCreate_1_0(
                const MarkerCreateData_1_0* createData,
                MarkerHandle_1_0*           handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerCreate_1_0(
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerDelete_1_0(
                const MarkerHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

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
/// @brief Instantiates functions for DdiMarker.
//////////////////////////////////////////////////////////////////////////
namespace ML
{
    namespace GEN12
    {
        #if ML_ENABLE_OPENCL
        namespace OpenCL
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerCreate_1_0(
                const MarkerCreateData_1_0* createData,
                MarkerHandle_1_0*           handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerCreate_1_0(
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerDelete_1_0(
                const MarkerHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

        } // namespace OpenCL
        #endif // ML_ENABLE_OPENCL

        #if ML_ENABLE_ONEAPI
        namespace OneApi
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerCreate_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerCreate_1_0(
                const MarkerCreateData_1_0* createData,
                MarkerHandle_1_0*           handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( createData );
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerCreate_1_0(
                    createData,
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Instantiates template function for MarkerDelete_1_0.
            //////////////////////////////////////////////////////////////////////////
            StatusCode ML_STDCALL MarkerDelete_1_0(
                const MarkerHandle_1_0 handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Print input values.
                log.Input( handle );

                // Call entry point implementation.
                log.m_Result = DdiMarker<T>::MarkerDelete_1_0(
                    handle );

                // Validate function result.
                ML_ASSERT( log.m_Result == StatusCode::Success );

                return log.m_Result;
            }

        } // namespace OneApi
        #endif // ML_ENABLE_ONEAPI

    } // namespace GEN12
} // namespace ML
#endif // ML_ENABLE_GEN12

