/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_override_flush_caches.h

@brief Writes gpu commands to flush caches.
*/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for OverrideFlushCachesTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct OverrideFlushCachesTrait : DdiObject<T, TT::Overrides::FlushCaches, OverrideHandle_1_0, ObjectType::OverrideFlushCaches>
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( OverrideFlushCachesTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( OverrideFlushCachesTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Types.
            //////////////////////////////////////////////////////////////////////////
            using Base = DdiObject<T, TT::Overrides::FlushCaches, OverrideHandle_1_0, ObjectType::OverrideFlushCaches>;

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "OverrideFlushCachesTrait<Traits>";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes flush gpu caches override command to command buffer.
            /// @param  buffer  command buffer.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode Write( CommandBuffer& buffer )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( T::GpuCommands::FlushCaches( buffer ) );

                return log.m_Result;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct OverrideFlushCachesTrait : BASE::OverrideFlushCachesTrait<T>
        {
            ML_DECLARE_TRAIT( OverrideFlushCachesTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct OverrideFlushCachesTrait : GEN9::OverrideFlushCachesTrait<T>
        {
            ML_DECLARE_TRAIT( OverrideFlushCachesTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct OverrideFlushCachesTrait : GEN11::OverrideFlushCachesTrait<T>
        {
            ML_DECLARE_TRAIT( OverrideFlushCachesTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
