/******************************************************************************\

Copyright © 2020, Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

@file ml_query_hw_counters_policy_opencl.h

@brief Query policy for OpenCL.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for QueryHwCountersPolicyOpenCLTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct QueryHwCountersPolicyOpenCLTrait
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( QueryHwCountersPolicyOpenCLTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( QueryHwCountersPolicyOpenCLTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "QueryHwCountersPolicyOpenCLTrait<Traits>";
            }

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

            struct Create
            {
                static constexpr uint32_t m_SlotsCount = 1;
            };

            struct Begin
            {
                static constexpr bool m_FlushCommandBuffer = true;
            };

            struct GetData
            {
                static constexpr bool m_AllowEmptyContextId  = true;
                static constexpr bool m_ResetOaBufferState   = false;
                static constexpr bool m_IncludeAllHwContexts = false;
            };
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct QueryHwCountersPolicyOpenCLTrait : BASE::QueryHwCountersPolicyOpenCLTrait<T>
        {
            ML_DECLARE_TRAIT( QueryHwCountersPolicyOpenCLTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct QueryHwCountersPolicyOpenCLTrait : GEN9::QueryHwCountersPolicyOpenCLTrait<T>
        {
            ML_DECLARE_TRAIT( QueryHwCountersPolicyOpenCLTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct QueryHwCountersPolicyOpenCLTrait : GEN11::QueryHwCountersPolicyOpenCLTrait<T>
        {
            ML_DECLARE_TRAIT( QueryHwCountersPolicyOpenCLTrait, GEN11 );

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
    } // namespace GEN12
} // namespace ML
