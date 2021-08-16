/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

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

                    //////////////////////////////////////////////////////////////////////////
                    /// @brief  Checks oa triggered reports usage.
                    /// @param  kernel kernel interface.
                    /// @return        true if triggered oa reports can be used.
                    //////////////////////////////////////////////////////////////////////////
                    ML_INLINE static bool UseTriggeredOaReport( const TT::KernelInterface& /*kernel*/ )
                    {
                        return false;
                    }
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
                };

                //////////////////////////////////////////////////////////////////////////
                /// @brief Query get data policies.
                //////////////////////////////////////////////////////////////////////////
                struct GetData
                {
                    static constexpr bool m_AllowEmptyContextId               = true;
                    static constexpr bool m_ResetOaBufferState                = false;
                    static constexpr bool m_IncludeAllHwContexts              = false;
                    static constexpr bool m_RecoverContextId                  = false;
                    static constexpr bool m_IncludeRenderContextSwitchReports = true;
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

    namespace GEN12
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

                    //////////////////////////////////////////////////////////////////////////
                    /// @brief  Checks oa triggered reports usage.
                    /// @param  kernel kernel interface.
                    /// @return        true if triggered oa reports can be used.
                    //////////////////////////////////////////////////////////////////////////
                    ML_INLINE static bool UseTriggeredOaReport( const TT::KernelInterface& kernel )
                    {
                        bool supported = kernel.m_Tbs.m_Stream.m_OaBufferMapped.IsMapped();
                        return supported;
                    }
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
    } // namespace GEN12
} // namespace ML
