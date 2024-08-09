/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

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

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base functions for DdiMarker.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct DdiMarker
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for MarkerCreate_1_0
        /// @param createData
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL MarkerCreate_1_0(
            const MarkerCreateData_1_0* createData,
            MarkerHandle_1_0*           handle )
        {
            ML_FUNCTION_CHECK_STATIC( handle != nullptr );
            ML_FUNCTION_CHECK_STATIC( createData != nullptr );
            ML_FUNCTION_CHECK_STATIC( T::Context::IsValid( createData->HandleContext ) );

            auto& context = T::Context::FromHandle( createData->HandleContext );
            ML_FUNCTION_LOG( StatusCode::Success, &context )

            // Print input values.
            log.Input( *createData );

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

            ML_ASSERT( log.m_Result == StatusCode::Success );

            // Print output values.
            log.Output( *handle );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Generic implementation for MarkerDelete_1_0
        /// @param handle
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ML_STDCALL MarkerDelete_1_0(
            const MarkerHandle_1_0 handle )
        {
            ML_FUNCTION_LOG_STATIC( StatusCode::Success );

            // Print input values.
            log.Input( handle );

            switch( BaseObject::GetType( handle ) )
            {
                case ObjectType::MarkerStreamUser:
                    log.m_Result = StatusCode::NotImplemented;
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    log.m_Result = StatusCode::IncorrectObject;
                    break;
            }

            ML_ASSERT( log.m_Result == StatusCode::Success );

            return log.m_Result;
        }
    };
} // namespace ML::BASE

#pragma region Platform Specific Functions for DdiMarker
namespace ML::GEN9
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN9-specific functions for DdiMarker.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiMarker = BASE::DdiMarker<T>;
} // namespace ML::GEN9

namespace ML::GEN11
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GEN11-specific functions for DdiMarker.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiMarker = GEN9::DdiMarker<T>;
} // namespace ML::GEN11

namespace ML::XE_LP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_LP-specific functions for DdiMarker.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiMarker = GEN11::DdiMarker<T>;
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HP-specific functions for DdiMarker.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiMarker = XE_LP::DdiMarker<T>;
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPG-specific functions for DdiMarker.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiMarker = XE_HP::DdiMarker<T>;
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE_HPC-specific functions for DdiMarker.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiMarker = XE_HPG::DdiMarker<T>;
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief XE2_HPG-specific functions for DdiMarker.
    ///        These should ONLY be used for MAJOR algorithm changes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    using DdiMarker = XE_HPG::DdiMarker<T>;
} // namespace ML::XE2_HPG

#pragma endregion
