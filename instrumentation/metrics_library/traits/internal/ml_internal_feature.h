/*========================== begin_copyright_notice ============================

Copyright (C) 2022-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_internal_feature.h

@brief A file describing a trait that's intended for internal use (for example debugging) only.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for InternalFeatureTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct InternalFeatureTrait
    {
        ML_DELETE_DEFAULT_COPY_AND_MOVE( InternalFeatureTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief InternalFeatureTrait constructor.
        //////////////////////////////////////////////////////////////////////////
        InternalFeatureTrait()
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "InternalFeatureTrait<Traits>";
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct InternalFeatureTrait : BASE::InternalFeatureTrait<T>
    {
        ML_DECLARE_TRAIT( InternalFeatureTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct InternalFeatureTrait : GEN9::InternalFeatureTrait<T>
    {
        ML_DECLARE_TRAIT( InternalFeatureTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct InternalFeatureTrait : GEN11::InternalFeatureTrait<T>
    {
        ML_DECLARE_TRAIT( InternalFeatureTrait, GEN11 );
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct InternalFeatureTrait : XE_LP::InternalFeatureTrait<T>
    {
        ML_DECLARE_TRAIT( InternalFeatureTrait, XE_LP );
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct InternalFeatureTrait : XE_HP::InternalFeatureTrait<T>
    {
        ML_DECLARE_TRAIT( InternalFeatureTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct InternalFeatureTrait : XE_HPG::InternalFeatureTrait<T>
    {
        ML_DECLARE_TRAIT( InternalFeatureTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct InternalFeatureTrait : XE_HPG::InternalFeatureTrait<T>
    {
        ML_DECLARE_TRAIT( InternalFeatureTrait, XE_HPG );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Uses provided buffer to override masked pec counters with provided value.
        /// @param  buffer command buffer.
        /// @param  type of pec registers to override.
        /// @param  mask 64-bit mask that dictates what counters will be overwritten
        ///         lower bytes represent counters at lower offset.
        /// @param  value value that will be written to the counters.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static void OverridePecCounters(
            [[maybe_unused]] CommandBuffer&                         buffer,
            [[maybe_unused]] const TT::Layouts::HwCounters::PecType type,
            [[maybe_unused]] const uint64_t                         mask,
            [[maybe_unused]] const uint64_t                         value )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Uses values in the register to override pec counters
        /// @param  buffer command buffer.
        /// @param  type of pec registers to override.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE void OverridePecCounters(
            [[maybe_unused]] CommandBuffer&                         buffer,
            [[maybe_unused]] const TT::Layouts::HwCounters::PecType type ) const
        {
        }
    };
} // namespace ML::XE2_HPG
