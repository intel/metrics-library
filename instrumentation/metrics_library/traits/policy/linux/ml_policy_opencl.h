/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2026 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_policy_opencl.h

@brief Policies for OpenCL.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for PolicyOpenCLTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct PolicyOpenCLTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( PolicyOpenCLTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( PolicyOpenCLTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Query hw counters policies.
        //////////////////////////////////////////////////////////////////////////
        struct QueryHwCounters
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Query creation policies.
            //////////////////////////////////////////////////////////////////////////
            struct Create
            {
                static constexpr uint32_t m_MaxSlotCount = 1;
                static constexpr bool     m_UserCounters = false;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Query begin policies.
            //////////////////////////////////////////////////////////////////////////
            struct Begin
            {
                static constexpr bool m_FlushCommandBuffer = false;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Query get data policies.
            //////////////////////////////////////////////////////////////////////////
            struct GetData
            {
                static constexpr bool m_CheckConfigurationActivation = false;
            };
        };
    };
} // namespace ML::BASE

namespace ML::XE_LP
{
    template <typename T>
    struct PolicyOpenCLTrait : BASE::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, BASE );
    };
} // namespace ML::XE_LP

namespace ML::XE_HPG
{
    template <typename T>
    struct PolicyOpenCLTrait : XE_LP::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, XE_LP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Sub device policies.
        //////////////////////////////////////////////////////////////////////////
        struct SubDevice
        {
            static constexpr bool m_AllowImplicitScaling = false;
        };
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct PolicyOpenCLTrait : XE_HPG::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct PolicyOpenCLTrait : XE_HPG::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, XE_HPG );
    };
} // namespace ML::XE2_HPG

namespace ML::XE3
{
    template <typename T>
    struct PolicyOpenCLTrait : XE2_HPG::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, XE2_HPG );
    };
} // namespace ML::XE3

namespace ML::XE3P
{
    template <typename T>
    struct PolicyOpenCLTrait : XE3::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, XE3 );
    };
} // namespace ML::XE3P
