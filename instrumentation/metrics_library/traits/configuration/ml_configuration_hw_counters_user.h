/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_configuration_hw_counters_user.h

@brief Creates mmio user configuration.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for ConfigurationHwCountersUserTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ConfigurationHwCountersUserTrait : DdiObject<T, TT::Configurations::HwCountersUser, ConfigurationHandle_1_0, ObjectType::ConfigurationHwCountersUser>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ConfigurationHwCountersUserTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ConfigurationHwCountersUserTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Configurations::HwCountersUser, ConfigurationHandle_1_0, ObjectType::ConfigurationHwCountersUser>;
        using Base::Allocate;
        using Base::Delete;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        const TT::KernelInterface&                m_Kernel;
        TT::Layouts::Configuration::UserRegisters m_UserRegisters;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Configuration hw counters mmio user constructor.
        /// @param context  library context.
        //////////////////////////////////////////////////////////////////////////
        ConfigurationHwCountersUserTrait( TT::Context& context )
            : Base( context )
            , m_Kernel( context.m_Kernel )
            , m_UserRegisters{}
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "ConfigurationHwCountersUserTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Creates mmio user configuration.
        /// @param  context library context.
        /// @return handle  configuration handle.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode Create(
            TT::Context&             context,
            ConfigurationHandle_1_0& handle )
        {
            auto       configuration = Allocate( context );
            const bool valid         = configuration && ML_SUCCESS( configuration->Initialize() );

            if( valid )
            {
                handle = configuration->GetHandle();
            }
            else
            {
                Delete( configuration );
            }

            return valid
                ? StatusCode::Success
                : StatusCode::Failed;
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes mmio user configuration.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize()
        {
            return m_Kernel.GetUserConfiguration( m_UserRegisters );
        }
    };
} // namespace ML
