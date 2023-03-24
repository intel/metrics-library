/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2023 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_override_layouts.h

@brief Layout for overrides.
*/

#pragma once

namespace ML::BASE
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
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct OverrideLayoutsTrait : BASE::OverrideLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideLayoutsTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct OverrideLayoutsTrait : GEN9::OverrideLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideLayoutsTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct OverrideLayoutsTrait : GEN11::OverrideLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideLayoutsTrait, GEN11 );
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct OverrideLayoutsTrait : XE_LP::OverrideLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideLayoutsTrait, XE_LP );
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct OverrideLayoutsTrait : XE_HP::OverrideLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideLayoutsTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct OverrideLayoutsTrait : XE_HPG::OverrideLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( OverrideLayoutsTrait, XE_HPG );
    };
} // namespace ML::XE_HPC
