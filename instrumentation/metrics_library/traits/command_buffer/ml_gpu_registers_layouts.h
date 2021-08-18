/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_gpu_registers_layouts.h

@brief Layout for gpu registers.
*/

// clang-format off

#pragma once

ML_STRUCTURE_PACK_BEGIN( 1 );

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for GpuRegistersLayoutsTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct GpuRegistersLayoutsTrait
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( GpuRegistersLayoutsTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( GpuRegistersLayoutsTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "GpuRegistersLayoutsTrait<Traits>";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Oa report trigger.
            //////////////////////////////////////////////////////////////////////////
            struct OaReportTrigger
            {
                union
                {
                    uint32_t        m_Value;

                    struct
                    {
                        uint32_t    m_Reserved1           : ML_BITFIELD_RANGE(  0, 20 );
                        uint32_t    m_InvertCEnable1      : ML_BITFIELD_BIT  (     21 );
                        uint32_t    m_InvertDEnable0      : ML_BITFIELD_BIT  (     22 );
                        uint32_t    m_Reserved2           : ML_BITFIELD_RANGE( 23, 30 );
                        uint32_t    m_ReportTriggerEnable : ML_BITFIELD_BIT  (     31 );
                    };
                };

                OaReportTrigger( const bool fallingEdge )
                    : m_Reserved1( 0 )
                    , m_InvertCEnable1( true )
                    , m_InvertDEnable0( fallingEdge )
                    , m_Reserved2( 0 )
                    , m_ReportTriggerEnable( true )
                {
                }
            };
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct GpuRegistersLayoutsTrait : BASE::GpuRegistersLayoutsTrait<T>
        {
            ML_DECLARE_TRAIT( GpuRegistersLayoutsTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct GpuRegistersLayoutsTrait : GEN9::GpuRegistersLayoutsTrait<T>
        {
            ML_DECLARE_TRAIT( GpuRegistersLayoutsTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct GpuRegistersLayoutsTrait : GEN11::GpuRegistersLayoutsTrait<T>
        {
            ML_DECLARE_TRAIT( GpuRegistersLayoutsTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML

ML_STRUCTURE_PACK_END();

// clang-format on
