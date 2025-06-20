/*========================== begin_copyright_notice ============================

Copyright (C) 2021-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_drm_layouts.h

@brief Contains drm structures and enumerations that are not available in the
       official drm files from official linux repository.
*/

#pragma once

ML_STRUCTURE_PACK_BEGIN( 1 );

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for DrmLayoutsTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct DrmLayoutsTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( DrmLayoutsTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( DrmLayoutsTrait );
    };
} // namespace ML::BASE

namespace ML::XE_LP
{
    template <typename T>
    struct DrmLayoutsTrait : BASE::DrmLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( DrmLayoutsTrait, BASE );

        //////////////////////////////////////////////////////////////////////////
        /// @brief File constants.
        //////////////////////////////////////////////////////////////////////////
        struct File
        {
            static constexpr const char* m_Name = "i915";
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Stream constants.
        //////////////////////////////////////////////////////////////////////////
        struct Stream
        {
            static constexpr const char* m_ParanoidPath = "/proc/sys/dev/i915/perf_stream_paranoid";
        };
    };
} // namespace ML::XE_LP

namespace ML::XE_HPG
{
    template <typename T>
    struct DrmLayoutsTrait : XE_LP::DrmLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( DrmLayoutsTrait, XE_LP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct DrmLayoutsTrait : XE_HPG::DrmLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( DrmLayoutsTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct DrmLayoutsTrait : BASE::DrmLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( DrmLayoutsTrait, BASE );

        //////////////////////////////////////////////////////////////////////////
        /// @brief File constants.
        //////////////////////////////////////////////////////////////////////////
        struct File
        {
            static constexpr const char* m_Name = "xe";
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Stream constants.
        //////////////////////////////////////////////////////////////////////////
        struct Stream
        {
            static constexpr const char* m_ParanoidPath = "/proc/sys/dev/xe/observation_paranoid";
        };
    };
} // namespace ML::XE2_HPG

namespace ML::XE3
{
    template <typename T>
    struct DrmLayoutsTrait : XE2_HPG::DrmLayoutsTrait<T>
    {
        ML_DECLARE_TRAIT( DrmLayoutsTrait, XE2_HPG );
    };
} // namespace ML::XE3

ML_STRUCTURE_PACK_END();
