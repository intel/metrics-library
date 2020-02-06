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

@file ml_configuration_hw_counters_oa.h

@brief Creates, activates and deactivates oa configuration.

\******************************************************************************/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for ConfigurationHwCountersOaTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ConfigurationHwCountersOaTrait : DdiObject<T, TT::Configurations::HwCountersOa, ConfigurationHandle_1_0, ObjectType::ConfigurationHwCountersOa>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ConfigurationHwCountersOaTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ConfigurationHwCountersOaTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Configurations::HwCountersOa, ConfigurationHandle_1_0, ObjectType::ConfigurationHwCountersOa>;
        using Base::Allocate;
        using Base::Delete;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::KernelInterface&                                       m_Kernel;
        TT::Context&                                               m_Context;
        TT::Layouts::Configuration::PerformanceMonitoringRegisters m_OaRegisters;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Configuration hw counters oa constructor.
        /// @param context  library context.
        //////////////////////////////////////////////////////////////////////////
        ConfigurationHwCountersOaTrait( TT::Context& context )
            : Base( context.m_ClientType )
            , m_Kernel{ context.m_Kernel }
            , m_Context( context )
            , m_OaRegisters{}
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "ConfigurationHwCountersOaTrait<Traits> (Linux)";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Creates oa configuration.
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

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Activates oa configuration.
        /// @param  activateData  oa counter activate data.
        /// @return               operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Activate( const ConfigurationActivateData_1_0& activateData ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success );
            ML_FUNCTION_CHECK( activateData.Type == GpuConfigurationActivationType::Tbs );

            return log.m_Result = m_Kernel.LoadOaConfigurationToGpu( m_OaRegisters );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Deactivates oa configuration.
        ///         If oa metric set is in use, reference counter will be decreased.
        ///         if reference counter will become zero, another metric set will be
        ///         allowed to be used.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Deactivate() const
        {
            ML_FUNCTION_LOG( StatusCode::Success );
            return log.m_Result = m_Kernel.UnloadOaConfigurationFromGpu( m_OaRegisters );
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes activated oa configuration.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize()
        {
            ML_FUNCTION_LOG( StatusCode::Success );
            return log.m_Result = m_Kernel.GetOaConfiguration( m_OaRegisters );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Calls flush command buffer callback.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode FlushCommandBuffer() const
        {
            const bool  validFlushCallback = m_Context.m_ClientCallbacks.CommandBufferFlush != nullptr;
            const bool  validFlushPolicy   = T::Queries::HwCountersPolicy::Begin::FlushCommandBuffer;
            const auto& callback           = m_Context.m_ClientCallbacks.CommandBufferFlush;

            return validFlushCallback && validFlushPolicy
                ? callback( m_Context.m_ClientHandle )
                : StatusCode::Success;
        }
    };
} // namespace ML
