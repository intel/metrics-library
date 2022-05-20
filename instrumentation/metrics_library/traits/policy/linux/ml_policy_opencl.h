/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_policy_opencl.h

@brief Policies for OpenCL.
*/

#pragma once

namespace ML
{
    namespace BASE
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
                    static constexpr uint32_t m_SlotsCount   = 1;
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
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Stream marker policies.
            //////////////////////////////////////////////////////////////////////////
            struct StreamMarker
            {
                static constexpr bool m_Use32bitValue = false;
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
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct PolicyOpenCLTrait : BASE::PolicyOpenCLTrait<T>
        {
            ML_DECLARE_TRAIT( PolicyOpenCLTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct PolicyOpenCLTrait : GEN9::PolicyOpenCLTrait<T>
        {
            ML_DECLARE_TRAIT( PolicyOpenCLTrait, GEN9 );
        };
    } // namespace GEN11

    namespace XE_LP
    {
        template <typename T>
        struct PolicyOpenCLTrait : GEN11::PolicyOpenCLTrait<T>
        {
            ML_DECLARE_TRAIT( PolicyOpenCLTrait, GEN11 );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Query hw counters policies.
            //////////////////////////////////////////////////////////////////////////
            struct QueryHwCounters : Base::QueryHwCounters
            {
                //////////////////////////////////////////////////////////////////////////
                /// @brief Common query policies.
                //////////////////////////////////////////////////////////////////////////
                struct Common
                {
                    static constexpr bool m_PatchGpuMemory = false;
                };
            };

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
    } // namespace XE_LP

    namespace XE_HP
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
    } // namespace XE_HP

    namespace XE_HPG
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
    } // namespace XE_HPG

    namespace XE_HPC
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
    } // namespace XE_HPC
} // namespace ML
