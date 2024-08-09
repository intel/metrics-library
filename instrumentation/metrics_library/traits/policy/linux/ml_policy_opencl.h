/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

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
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "PolicyOpenCLTrait<Traits>";
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
                static constexpr uint32_t m_MaxSlotCount = 1;
                static constexpr bool     m_UserCounters = false;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Query begin policies.
            //////////////////////////////////////////////////////////////////////////
            struct Begin
            {
                static constexpr bool m_FlushCommandBuffer = true;
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
                static constexpr bool m_AllowEmptyContextId               = true;
                static constexpr bool m_ResetOaBufferState                = false;
                static constexpr bool m_AsyncCompute                      = false;
                static constexpr bool m_RecoverContextId                  = false;
                static constexpr bool m_IncludeRenderContextSwitchReports = true;
                static constexpr bool m_CheckConfigurationActivation      = false;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Query write policies.
            //////////////////////////////////////////////////////////////////////////
            struct Write
            {
                static constexpr bool m_MirpcOnOagTriggers = false;
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
    struct PolicyOpenCLTrait : BASE::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct PolicyOpenCLTrait : GEN9::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct PolicyOpenCLTrait : GEN11::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, GEN11 );

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
    struct PolicyOpenCLTrait : XE_LP::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, XE_LP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Query hw counters policies.
        //////////////////////////////////////////////////////////////////////////
        struct QueryHwCounters : Base::QueryHwCounters
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Query get data policies.
            //////////////////////////////////////////////////////////////////////////
            struct GetData : Base::QueryHwCounters::GetData
            {
                static constexpr bool m_IncludeRenderContextSwitchReports = false;
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Query write policies.
            //////////////////////////////////////////////////////////////////////////
            struct Write : Base::QueryHwCounters::Write
            {
                static constexpr bool m_MirpcOnOagTriggers = true;
            };
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Sub device policies.
        //////////////////////////////////////////////////////////////////////////
        struct SubDevice
        {
            static constexpr bool m_DriverClientDataRequired = false;
            static constexpr bool m_AllowImplicitScaling     = false;
        };
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct PolicyOpenCLTrait : XE_HP::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, XE_HP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Query hw counters policies.
        //////////////////////////////////////////////////////////////////////////
        struct QueryHwCounters : Base::QueryHwCounters
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Query get data policies.
            //////////////////////////////////////////////////////////////////////////
            struct GetData : Base::QueryHwCounters::GetData
            {
                static constexpr bool m_IncludeRenderContextSwitchReports = true;
            };
        };
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct PolicyOpenCLTrait : XE_HPG::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, XE_HPG );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Query hw counters policies.
        //////////////////////////////////////////////////////////////////////////
        struct QueryHwCounters : Base::QueryHwCounters
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Query get data policies.
            //////////////////////////////////////////////////////////////////////////
            struct GetData : Base::QueryHwCounters::GetData
            {
                static constexpr bool m_IncludeRenderContextSwitchReports = false;
            };
        };
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct PolicyOpenCLTrait : XE_HPG::PolicyOpenCLTrait<T>
    {
        ML_DECLARE_TRAIT( PolicyOpenCLTrait, XE_HPG );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Query hw counters policies.
        //////////////////////////////////////////////////////////////////////////
        struct QueryHwCounters : Base::QueryHwCounters
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Query get data policies.
            //////////////////////////////////////////////////////////////////////////
            struct GetData : Base::QueryHwCounters::GetData
            {
                static constexpr bool m_IncludeRenderContextSwitchReports = false; // MI_RPC is not executed on OAG query
            };
        };
    };
} // namespace ML::XE2_HPG
