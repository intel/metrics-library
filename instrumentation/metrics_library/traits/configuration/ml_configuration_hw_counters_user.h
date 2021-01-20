/******************************************************************************\

Copyright © 2021, Intel Corporation

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

@file ml_configuration_hw_counters_user.h

@brief Creates mmio user configuration.

\******************************************************************************/

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
            : Base( context.m_ClientType )
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
