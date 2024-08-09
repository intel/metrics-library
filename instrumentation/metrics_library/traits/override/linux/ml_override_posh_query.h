/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_override_posh_query.h

@brief Enables posh query override.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for OverridePoshQueryTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct OverridePoshQueryTrait : DdiObject<T, TT::Overrides::PoshQuery, OverrideHandle_1_0, ObjectType::OverridePoshQuery>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( OverridePoshQueryTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( OverridePoshQueryTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Overrides::PoshQuery, OverrideHandle_1_0, ObjectType::OverridePoshQuery>;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Override posh query constructor.
        /// @param context library context.
        //////////////////////////////////////////////////////////////////////////
        OverridePoshQueryTrait( TT::Context& context )
            : Base( context )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "OverridePoshQueryTrait<Traits> (Linux)";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Creates posh query override.
        /// @param  context library context.
        /// @return handle  override handle.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode Create(
            [[maybe_unused]] TT::Context&        context,
            [[maybe_unused]] OverrideHandle_1_0& handle )
        {
            return StatusCode::NotImplemented;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns state of user overrides.
        /// @return getData data requested by client.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode GetData( [[maybe_unused]] GetReportOverride_1_0& getData )
        {
            return StatusCode::NotImplemented;
        }
    };
} // namespace ML
