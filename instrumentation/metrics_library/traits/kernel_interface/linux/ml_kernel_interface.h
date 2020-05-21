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

@file ml_kernel_interface.h

@brief Allows to communicate with os kernel.

\******************************************************************************/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for KernelInterfaceTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct KernelInterfaceTrait
    {
        ML_DELETE_DEFAULT_COPY_AND_MOVE( KernelInterfaceTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::Context&     m_Context;
        int32_t          m_DeviceId;
        TT::IoControl    m_IoControl;
        TT::TbsInterface m_Tbs;

        //////////////////////////////////////////////////////////////////////////
        /// @brief KernelInterfaceTrait constructor.
        /// @param context  metrics library context.
        //////////////////////////////////////////////////////////////////////////
        KernelInterfaceTrait( TT::Context& context )
            : m_Context( context )
            , m_DeviceId( T::ConstantsOs::Drm::m_Invalid )
            , m_IoControl( *this )
            , m_Tbs( *this )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "KernelInterfaceTrait<Traits> (Linux)";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes kernel interface.
        /// @param  clientData  initializing client data.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize( const ClientData_1_0& clientData )
        {
            ML_FUNCTION_LOG( StatusCode::Success );
            ML_FUNCTION_CHECK( m_IoControl.Initialize( clientData ) );
            ML_FUNCTION_CHECK( InitializeDevice() );
            ML_FUNCTION_CHECK( m_Tbs.Initialize() );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets exact gpu timestamp frequency.
        /// @return gpu timestamp frequency.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t GetGpuTimestampFrequency() const
        {
            ML_FUNCTION_LOG( StatusCode::Success );

            static uint64_t frequency = 0;

            if( frequency == 0 )
            {
                frequency = m_IoControl.GetGpuTimestampFrequency();

                if( frequency == 0 )
                {
                    ML_ASSERT_ALWAYS();
                    frequency = 12000000; // Default, one tick per 83.333ns.
                    log.Warning( "Predefined default gpu timestamp frequency used", frequency );
                }
            }

            return frequency;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets gpu timestamp tick value.
        /// @return gpu timestamp tick value.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t GetGpuTimestampTick() const
        {
            static uint64_t gpuTimestampTickValue = 0;

            if( gpuTimestampTickValue == 0 )
            {
                gpuTimestampTickValue = Constants::Time::m_SecondInNanoseconds / GetGpuTimestampFrequency();
            }

            return gpuTimestampTickValue;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns hw context ids for current process.
        /// @return allowed hw contexts ids and their count.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::Configuration::HwContextIds GetHwContextIds() const
        {
            static TT::Layouts::Configuration::HwContextIds contexts = {};

            return contexts;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns tbs state.
        /// @return true if tbs is enabled.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool TbsIsEnabled() const
        {
            ML_FUNCTION_LOG( false );

            return log.m_Result = m_Tbs.IsEnabled();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Loads oa configuration to gpu through tbs interface.
        /// @param  oaConfiguration  oa configuration.
        /// @return                  operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode LoadOaConfigurationToGpu( const TT::Layouts::Configuration::PerformanceMonitoringRegisters& oaConfiguration )
        {
            ML_FUNCTION_LOG( StatusCode::Success );
            ML_FUNCTION_CHECK( m_Tbs.m_Stream.SetMetricSet( oaConfiguration.m_Id ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Releases oa metric set. If metric set has been used previously
        ///         then reference counter will be decreased. If reference counter
        ///         will become zero then another metric set can be used.
        /// @param  oaConfiguration  oa configuration.
        /// @return                  operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode UnloadOaConfigurationFromGpu( const TT::Layouts::Configuration::PerformanceMonitoringRegisters& oaConfiguration )
        {
            ML_FUNCTION_LOG( StatusCode::Success );
            ML_FUNCTION_CHECK( m_Tbs.m_Stream.ReleaseMetricSet( oaConfiguration.m_Id ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns activated by metrics discovery oa configuration
        ///         from the kernel.
        /// @param  oaConfiguration     activated oa configuration.
        /// @return                     operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetOaConfiguration( TT::Layouts::Configuration::PerformanceMonitoringRegisters& oaConfiguration ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success );

            oaConfiguration.m_Id = m_IoControl.GetKernelMetricSet();

            ML_FUNCTION_CHECK( oaConfiguration.m_Id != T::ConstantsOs::Tbs::m_Invalid );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns activated by metrics discovery mmio user configuration
        ///         from the kernel.
        ///         Not supported on linux.
        /// @param  userConfiguration   activated mmio user configuration.
        /// @return                     operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetUserConfiguration( TT::Layouts::Configuration::UserRegisters& /*userConfiguration*/ ) const
        {
            ML_FUNCTION_LOG( StatusCode::NotSupported );

            ML_ASSERT_ALWAYS();

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns auto sampling mode for hw counters.
        /// @return hw counters auto sampling mode.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::Query::GetDataMode GetQueryHwCountersReportingMode() const
        {
            return T::Layouts::HwCounters::Query::GetDataMode::Normal;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns null begin override state.
        /// @return true if null begin override has been enabled.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsNullBeginOverride() const
        {
            ML_FUNCTION_LOG( false );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns null query override state.
        /// @return true if null query override is enabled.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsNullQueryOverride() const
        {
            ML_FUNCTION_LOG( false );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns posh query override state.
        /// @return true if posh query override has been enabled.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsPoshQueryOverride() const
        {
            ML_FUNCTION_LOG( false );

            return log.m_Result;
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes device.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode InitializeDevice()
        {
            ML_FUNCTION_LOG( StatusCode::Success );

            // Chipset.
            if( ML_FAIL( m_IoControl.GetChipsetId( m_DeviceId ) ) )
            {
                log.Error( "Unable to obtain chipset id from the kernel" );
                return log.m_Result = StatusCode::NotInitialized;
            }

            return log.m_Result = ML_STATUS( m_DeviceId != T::ConstantsOs::Drm::m_Invalid );
        }
    };
} // namespace ML
