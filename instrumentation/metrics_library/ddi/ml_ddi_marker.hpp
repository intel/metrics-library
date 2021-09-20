/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_marker.hpp

@brief Partially auto-generated file. Provides implementation for Metrics Library
       entry points.

Script file:        Tools/MetricsLibraryGenerator/generators/generate_umd.py
Script function:    generate_umd_ddi
Template:           Tools/MetricsLibraryGenerator/templates/hpp.h
*/

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

    namespace XE_LP
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief XE_LP-specific functions for DdiMarker.
        ///        These should ONLY be used for MAJOR algorithm changes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        using DdiMarker = GEN11::DdiMarker<T>;
    } // namespace XE_LP

#pragma endregion
} // namespace ML
