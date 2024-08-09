/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_override_layouts.h

@brief Layout for overrides.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for OverrideLayoutsTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct OverrideLayoutsTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( OverrideLayoutsTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( OverrideLayoutsTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Override posh query.
        //////////////////////////////////////////////////////////////////////////
        struct PoshQuery
        {
            bool m_Enabled;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "OverrideLayoutsTrait<Traits>";
        }
    };
} // namespace ML
