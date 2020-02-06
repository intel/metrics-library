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

@file ml_ddi_marker.hpp

@brief Partially auto-generated file. Provides implementation for Metrics Library
       entry points.

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_ddi
Template:           Tools/MetricsLibraryGenerator/templates/hpp.h

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base functions for DdiMarker.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct DdiMarker
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Generic implementation for MarkerCreate_1_0
            /// @param markerData
            /// @param handle
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static StatusCode ML_STDCALL MarkerCreate_1_0(
                const MarkerCreateData_1_0* createData,
                MarkerHandle_1_0*           handle )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( handle != nullptr );
                ML_FUNCTION_CHECK( createData != nullptr );
                ML_FUNCTION_CHECK( T::Context::IsValid( createData->HandleContext ) );

                switch( createData->Type )
                {
                    case ObjectType::MarkerStreamUser:
                        log.m_Result = StatusCode::NotImplemented;
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        log.m_Result = StatusCode::IncorrectParameter;
                        break;
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Generic implementation for MarkerDelete_1_0
            /// @param handle
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static StatusCode ML_STDCALL MarkerDelete_1_0(
                const MarkerHandle_1_0 handle )
            {
                switch( BaseObject::GetType( handle ) )
                {
                    case ObjectType::MarkerStreamUser:
                        return StatusCode::NotImplemented;

                    default:
                        ML_ASSERT_ALWAYS();
                        return StatusCode::IncorrectObject;
                }
            }
        };
    } // namespace BASE

#pragma region Platform Specific Functions for DdiMarker
    namespace GEN9
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN9-specific functions for DdiMarker.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiMarker = BASE::DdiMarker<T>;
    } // namespace GEN9

    namespace GEN11
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN11-specific functions for DdiMarker.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiMarker = GEN9::DdiMarker<T>;
    } // namespace GEN11

    namespace GEN12
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief GEN12-specific functions for DdiMarker.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiMarker = GEN11::DdiMarker<T>;
    } // namespace GEN12

#pragma endregion
} // namespace ML
