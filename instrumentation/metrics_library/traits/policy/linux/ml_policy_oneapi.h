/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_policy_openapi.h

@brief Policies for OneApi.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for PolicyOneApiTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct PolicyOneApiTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( PolicyOneApiTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( PolicyOneApiTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "PolicyOneApiTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Query hw counters policies.
        //////////////////////////////////////////////////////////////////////////
        struct QueryHwCounters
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Common query policies.
            //////////////////////////////////////////////////////////////////////////
            struct Common
            {
                static constexpr bool m_PatchGpuMemory = false;
            };

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
                static constexpr bool m_ClearGpuMemory     = true;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Query end policies.
            //////////////////////////////////////////////////////////////////////////
            struct End
            {
                static constexpr bool m_UseEndTagAsCompletionStatus = true;
                static constexpr bool m_UseDoubleTriggers           = false;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Query get data policies.
            //////////////////////////////////////////////////////////////////////////
            struct GetData
            {
                static constexpr bool m_AllowEmptyContextId          = true;
                static constexpr bool m_ResetOaBufferState           = true;
                static constexpr bool m_CheckConfigurationActivation = true;
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Configuration oa policies.
        //////////////////////////////////////////////////////////////////////////
        struct ConfigurationOa
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Configuration activation policies.
            //////////////////////////////////////////////////////////////////////////
            struct Activate
            {
                static constexpr bool m_RestartTbs = true;
            };
        };
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct PolicyOneApiTrait : BASE::PolicyOneApiTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOneApiTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct PolicyOneApiTrait : GEN9::PolicyOneApiTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOneApiTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct PolicyOneApiTrait : GEN11::PolicyOneApiTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOneApiTrait, GEN11 );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Configuration oa policies.
        //////////////////////////////////////////////////////////////////////////
        struct ConfigurationOa : Base::ConfigurationOa
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Configuration activation policies.
            //////////////////////////////////////////////////////////////////////////
            struct Activate
            {
                static constexpr bool m_RestartTbs = false;
            };
        };
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct PolicyOneApiTrait : XE_LP::PolicyOneApiTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOneApiTrait, XE_LP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Sub device policies.
        //////////////////////////////////////////////////////////////////////////
        struct SubDevice
        {
            static constexpr bool m_DriverClientDataRequired = false;
            static constexpr bool m_AllowImplicitScaling     = true;
        };
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct PolicyOneApiTrait : XE_HP::PolicyOneApiTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOneApiTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct PolicyOneApiTrait : XE_HPG::PolicyOneApiTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOneApiTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct PolicyOneApiTrait : XE_HPG::PolicyOneApiTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOneApiTrait, XE_HPG );
    };
} // namespace ML::XE2_HPG
