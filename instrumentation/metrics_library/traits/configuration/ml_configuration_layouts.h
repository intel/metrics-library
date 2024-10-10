/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_configuration_layouts.h

@brief Layout for gpu configuration.
*/

#pragma once

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
        ML_STRUCTURE_PACK_BEGIN( 4 );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "ConfigurationLayoutsTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Configuration type enumeration.
        //////////////////////////////////////////////////////////////////////////
        enum class Type : uint32_t
        {
            Dynamic = 0, // GTDI_CONFIGURATION_SET_DYNAMIC
        };

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
            uint32_t m_StoreRegisterMemoryCount;
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
        /// @brief Base type for OverrideRegisters object.
        //////////////////////////////////////////////////////////////////////////
        struct OverrideRegisters
        {
            uint32_t m_Handle; // INSTR_PM_REGS_CFG_INVALID or any positive number.

            struct
            {
                Register m_Registers[Constants::Configuration::m_MaxOverrideRegisters];
                uint32_t m_RegistersCount;
            } m_Enable;

            struct
            {
                Register m_Registers[Constants::Configuration::m_MaxOverrideRegisters];
                uint32_t m_RegistersCount;
            } m_Disable;
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for RegisterSettings object.
        //////////////////////////////////////////////////////////////////////////
        struct RegisterSettings
        {
            uint32_t m_Offset;          // Read-only.
            uint32_t m_EnableValue;     // Read-only.
            uint32_t m_DisableValue;    // Read-only.
            bool     m_UpperHalfIsMask; // Read-only.
            uint32_t m_WriteMask;       // Read-only.
            uint32_t m_InitialValue;    // Read-write - used to revert back original CG settings.
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for Initialize object.
        //////////////////////////////////////////////////////////////////////////
        struct InitializeRegisters
        {
            uint32_t         m_Pending;
            uint32_t         m_Count;
            RegisterSettings m_Registers[Constants::Configuration::m_MaxInitRegisters];
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for PerformanceMonitoringRegisters object.
        //////////////////////////////////////////////////////////////////////////
        struct PerformanceMonitoringRegisters
        {
            UserRegisters     m_User;
            OaRegisters       m_Oa;
            GpRegisters       m_Gp;
            OverrideRegisters m_Overrides;
            union
            {
                InitializeRegisters m_Initialize; // Not used by Metrics Library, so let's use it as configuration id.
                int64_t             m_Id;         // Additional parameter calculated from oa/gp.
            };                                    // Not compatible with instrumentation,
        };                                        // but should not effect query execution since.
                                                  // This is a last element within the structure.

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for OverridesState structure.
        //////////////////////////////////////////////////////////////////////////
        struct OverridesState
        {
            uint32_t m_Global[T::Layouts::Override::Type::Last];
            uint32_t m_Query[T::Layouts::Override::Type::Last];
        };

        ML_STRUCTURE_PACK_END();
    };
} // namespace ML
