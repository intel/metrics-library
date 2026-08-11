/*========================== begin_copyright_notice ============================

Copyright (C) 2026 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_policy_vulkan.h

@brief Policies for Vulkan.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for PolicyVulkanTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct PolicyVulkanTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( PolicyVulkanTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( PolicyVulkanTrait );

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
                static constexpr uint32_t m_MaxSlotCount = 0xFFFFFFFF;
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
    struct PolicyVulkanTrait : BASE::PolicyVulkanTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyVulkanTrait, BASE );
    };
} // namespace ML::XE_LP

namespace ML::XE_HPG
{
    template <typename T>
    struct PolicyVulkanTrait : XE_LP::PolicyVulkanTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyVulkanTrait, XE_LP );

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
    struct PolicyVulkanTrait : XE_HPG::PolicyVulkanTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyVulkanTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct PolicyVulkanTrait : XE_HPG::PolicyVulkanTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyVulkanTrait, XE_HPG );
    };
} // namespace ML::XE2_HPG

namespace ML::XE3
{
    template <typename T>
    struct PolicyVulkanTrait : XE2_HPG::PolicyVulkanTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyVulkanTrait, XE2_HPG );
    };
} // namespace ML::XE3

namespace ML::XE3P
{
    template <typename T>
    struct PolicyVulkanTrait : XE3::PolicyVulkanTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyVulkanTrait, XE3 );
    };
} // namespace ML::XE3P
