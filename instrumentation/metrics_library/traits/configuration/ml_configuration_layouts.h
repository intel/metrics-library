/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2026 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_configuration_layouts.h

@brief Layout for gpu configuration.
*/

#pragma once

ML_STRUCTURE_PACK_BEGIN( 4 );

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for ConfigurationLayoutsTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ConfigurationLayoutsTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ConfigurationLayoutsTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ConfigurationLayoutsTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Timestamp type.
        //////////////////////////////////////////////////////////////////////////
        enum class TimestampType : uint32_t
        {
            Oa = ML_BIT( 0 ),
            Cs = ML_BIT( 1 )
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for Register object.
        //////////////////////////////////////////////////////////////////////////
        struct Register
        {
            uint32_t m_Offset;

            union
            {
                uint32_t m_Value32;
                uint64_t m_Value64;
            };

            uint32_t m_Size;
            uint32_t m_Flags;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for UserRegisters object.
        //////////////////////////////////////////////////////////////////////////
        struct UserRegisters
        {
            uint32_t m_Handle;                                                  // range(0x100-...) - for dynamic configurations id is incremented by each change.
                                                                                // range(1-0x10)    - for static configurations id is constant INSTR_PM_REGS_CFG_INVALID.
            Register m_Registers[Constants::Configuration::m_MaxUserRegisters]; //
            uint32_t m_RegistersCount;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for OaRegisters object.
        //////////////////////////////////////////////////////////////////////////
        struct OaRegisters
        {
            uint32_t m_Handle;                                              // range(0x100-...) - for dynamic configurations id is incremented by each change.
                                                                            // range(1-0x10)    - for static configurations id is constant INSTR_PM_REGS_CFG_INVALID.
            Register m_Registers[Constants::Configuration::m_MaxRegisters]; //
            uint32_t m_RegistersCount;
            uint32_t m_PendingRegistersCount;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for GpRegisters object.
        //////////////////////////////////////////////////////////////////////////
        struct GpRegisters
        {
            uint32_t m_Handle;                                              // range(0x100-...) - for dynamic configurations id is incremented by each change.
                                                                            // range(1-0x10)    - for static configurations id is constant INSTR_PM_REGS_CFG_INVALID.
            Register m_Registers[Constants::Configuration::m_MaxRegisters]; //
            uint32_t m_RegistersCount;
            uint32_t m_PendingRegistersCount;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for PerformanceMonitoringRegisters object.
        //////////////////////////////////////////////////////////////////////////
        struct PerformanceMonitoringRegisters
        {
            UserRegisters m_User;
            OaRegisters   m_Oa;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for OverridesState structure.
        //////////////////////////////////////////////////////////////////////////
        struct OverridesState
        {
            uint32_t m_Global[T::Layouts::Override::Type::Last];
            uint32_t m_Query[T::Layouts::Override::Type::Last];
        };
    };
} // namespace ML

ML_STRUCTURE_PACK_END();
