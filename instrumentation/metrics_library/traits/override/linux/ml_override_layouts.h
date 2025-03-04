/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

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
    };
} // namespace ML
