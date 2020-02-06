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
        int32_t                m_DeviceId;
        struct gen_device_info m_DeviceInformation;
        TT::IoControl          m_IoControl;
        TT::TbsInterface       m_TbsInterface;

        //////////////////////////////////////////////////////////////////////////
        /// @brief KernelInterfaceTrait constructor.
        //////////////////////////////////////////////////////////////////////////
        KernelInterfaceTrait()
            : m_DeviceId( T::ConstantsOs::Drm::m_Invalid )
            , m_DeviceInformation{}
            , m_IoControl{}
            , m_TbsInterface( *this )
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
        /// @brief  Returns kernel interface. We use only one kernel interface for
        ///         a whole library lifetime to keep a global oa buffer (a shared
        ///         resource for all library contexts. We need to keep kernel
        ///         communication even if all contexts has been destroyed.
        /// @return kernel interface object.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static TT::KernelInterface& GetInstance()
        {
            static TT::KernelInterface kernelInterface;

            return kernelInterface;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes kernel interface.
        /// @param  clientData  initializing client data.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize( const ClientData_1_0& clientData )
        {
            const bool validIoControl = ML_SUCCESS( m_IoControl.Initialize( clientData ) );
            const bool validDevice    = validIoControl && InitializeDevice() && m_TbsInterface.Initialize();

            return validDevice
                ? StatusCode::Success
                : StatusCode::UnknownGen;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks hw counters support from kernel side.
        /// @return success if hw counters are supported.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode CheckInstrumentationSupport() const
        {
            return m_DeviceId != T::ConstantsOs::Drm::m_Invalid
                ? StatusCode::Success
                : StatusCode::NotInitialized;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns gen platform identification enumerator.
        /// @return gen platform identification enumerator.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE ClientGen GetGenType() const
        {
            ML_FUNCTION_LOG( ClientGen::Unknown );

            ML_ASSERT( m_DeviceId != T::ConstantsOs::Drm::m_Invalid );

            return log.m_Result = T::ToolsOs::GetLibraryGenFormat( m_DeviceInformation );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets exact gpu timestamp frequency.
        /// @return gpu timestamp frequency.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t GetGpuTimestampFrequency() const
        {
            ML_ASSERT( m_DeviceInformation.timestamp_frequency );

            return m_DeviceInformation.timestamp_frequency;
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

            return log.m_Result = m_TbsInterface.IsEnabled();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief   Enables time based sampling.
        /// @return  operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode TbsEnable() const
        {
            ML_FUNCTION_LOG( StatusCode::TbsUnableToEnable );

            log.Error( "Tbs can be enabled only during oa set activation (for example query begin)." );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Disables tbs stream.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode TbsDisable()
        {
            ML_FUNCTION_LOG( StatusCode::Success );

            return log.m_Result = m_TbsInterface.Disable();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns tbs reports.
        /// @return oaReports   output oa reports.
        /// @return count       collected oa reports count.
        /// @return completed   true if all oa reports has been received.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode TbsGetReports(
            TT::Layouts::HwCounters::ReportOa* oaReports,
            uint32_t&                          count,
            bool&                              completed ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success );

            return log.m_Result = m_TbsInterface.GetOaReports( oaReports, count, completed );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Loads oa configuration to gpu through tbs interface.
        /// @param  oaConfiguration  oa configuration.
        /// @return                  operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode LoadOaConfigurationToGpu( const TT::Layouts::Configuration::PerformanceMonitoringRegisters& oaConfiguration )
        {
            ML_FUNCTION_LOG( StatusCode::Success );

            // On linux oa metric set can be activated only through tbs.
            return log.m_Result = m_TbsInterface.Enable( oaConfiguration.m_Id );
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

            // Releases metric set, expecting three cases:
            //  1. Current activated metric set -> decrease reference counter.
            //  2. Unknown metric set           -> error.
            //  3. Reference counter == 0       -> disable tbs.
            if( ML_FAIL( log.m_Result = m_TbsInterface.Release( oaConfiguration.m_Id ) ) )
            {
                log.Error( "Unable to release metric set", oaConfiguration.m_Id );
            }

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

            oaConfiguration.m_Id = m_TbsInterface.GetKernelOaSetId();

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
        ML_INLINE bool InitializeDevice()
        {
            ML_FUNCTION_LOG( true );

            // Chipset.
            if( ML_FAIL( m_IoControl.GetParameter( I915_PARAM_CHIPSET_ID, m_DeviceId ) ) )
            {
                log.Error( "Unable to obtain chipset id from the kernel" );
                return log.m_Result = false;
            }

            // Device.
            if( !gen_get_device_info( m_DeviceId, &m_DeviceInformation ) )
            {
                log.Error( "Unable to obtain device information" );
                m_DeviceId          = T::ConstantsOs::Drm::m_Invalid;
                return log.m_Result = false;
            }

            return log.m_Result;
        }
    };
} // namespace ML
