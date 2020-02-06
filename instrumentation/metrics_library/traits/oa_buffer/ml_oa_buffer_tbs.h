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

@file ml_oa_buffer_tbs.h

@brief Time based sampling representation of oa buffer. Allows to initialize
       oa buffer and its state and collects reports from it.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for OaBufferTbsTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct OaBufferTbsTrait
        {
            ML_DELETE_DEFAULT_COPY_AND_MOVE( OaBufferTbsTrait );

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            TT::KernelInterface& m_Kernel;
            TT::TbsContainer*    m_TbsContainer;
            int32_t              m_ReferenceCounter;

        public:
            //////////////////////////////////////////////////////////////////////////
            /// @brief OaBufferTbsTrait constructor.
            //////////////////////////////////////////////////////////////////////////
            OaBufferTbsTrait()
                : m_Kernel( T::KernelInterface::GetInstance() )
                , m_TbsContainer( nullptr )
                , m_ReferenceCounter( 0 )
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "OaBufferTbsTrait<Traits>";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns one common oa buffer instance.
            /// @return reference to oa buffer.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static TT::OaBuffer& GetInstance()
            {
                static TT::OaBuffer oaBuffer;
                return oaBuffer;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Initializes oa buffer.
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Initialize()
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Initialize oa buffer once for all contexts.
                if( m_ReferenceCounter == 0 )
                {
                    // Enable tbs.
                    if( T::ConstantsOs::Tbs::m_EnableOnCreateContext )
                    {
                        log.m_Result = EnableTbs();
                        log.Debug( "Enabling tbs during create context has:", log.m_Result );
                    }

                    // Enable tbs container.
                    if( ML_SUCCESS( log.m_Result ) )
                    {
                        log.m_Result = EnableTbsContainer();
                        log.Debug( "Enabling tbs container has:", log.m_Result );
                    }

                    // Sanity check.
                    if( ML_FAIL( log.m_Result ) )
                    {
                        log.Error( "Unable to enable tbs." );
                        DisableTbs();
                        return log.m_Result;
                    }
                }

                // Increase reference counter.
                m_ReferenceCounter++;

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Initializes oa buffer state kept by hw counters query.
            /// @param  reportGpu       query gpu report.
            /// @param  oaBufferState   oa buffer state to initialize.
            /// @return                 true if success.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool InitializeState(
                const TT::Layouts::HwCounters::Query::ReportGpu& /*reportGpu*/,
                TT::Layouts::OaBuffer::State& /*oaBufferState*/ ) const
            {
                return true;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief   Releases oa buffer.
            /// @return  operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Release()
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( IsValid() );

                // Decrement reference counter.
                m_ReferenceCounter = std::max<int32_t>( --m_ReferenceCounter, 0 );

                // Release oa buffer mapping if the last context has been destroyed.
                if( m_ReferenceCounter == 0 )
                {
                    // Disable tbs.
                    if( T::ConstantsOs::Tbs::m_EnableOnCreateContext )
                    {
                        DisableTbs();
                    }

                    // Disable tbs container.
                    DisableTbsContainer();
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Checks oa buffer state.
            /// @return true if oa buffer has valid state.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsValid() const
            {
                // Tbs will be always valid, because:
                //  1. It can be enabled.
                //  2. It can be disabled, which should not affect an application.
                //  3. It can be already enabled by other application.
                return m_TbsContainer != nullptr;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Finds oa reports between query begin/end reports.
            /// @param  reportGpu       query gpu report.
            /// @param  oaBufferState   oa buffer state.
            /// @return                 oa reports count between query begin/end.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t FindOaWindow(
                const TT::Layouts::HwCounters::Query::ReportGpu& reportGpu,
                TT::Layouts::OaBuffer::State&                    oaBufferState ) const
            {
                ML_ASSERT( m_TbsContainer );

                return m_TbsContainer->FindOaWindow( reportGpu, oaBufferState );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa report from oa buffer.
            /// @param  index   oa report index within oa buffer.
            /// @return         reference to oa report.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE const TT::Layouts::HwCounters::ReportOa& GetReport( const uint32_t index )
            {
                ML_ASSERT( m_Kernel.TbsIsEnabled() );
                ML_ASSERT( m_TbsContainer );
                return m_TbsContainer->GetReport( index );
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa report index collected by oa head registers.
            /// @param  address   address collected by oa head registers.
            /// @return           oa report index.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetReportIndex( const TT::Layouts::OaBuffer::HeadRegister address ) const
            {
                ML_ASSERT_ALWAYS();
                return 0;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns oa report index collected by oa tail registers.
            /// @param  address   address collected by oa tail registers.
            /// @return           oa report index.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetReportIndex( const TT::Layouts::OaBuffer::TailRegister address ) const
            {
                ML_ASSERT_ALWAYS();
                return 0;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Check whether oa buffer contains oa reports.
            /// @return true if oa buffer is empty.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE bool IsEmpty() const
            {
                return m_Kernel.TbsIsEnabled()
                    ? m_TbsContainer->IsEmpty()
                    : true;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief   Returns oa reports count within oa buffer.
            /// @return  oa reports count within oa buffer.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint32_t GetReportsCount() const
            {
                return m_Kernel.TbsIsEnabled()
                    ? m_TbsContainer->GetCapacity()
                    : 0;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Collects oa reports from tbs.
            /// @param  endTimestamp    collect reports older than endTimestamp.
            /// @return                 success if oa reports has been collected.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode CollectReports( const uint32_t endTimestamp )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                log.m_Result = m_Kernel.TbsIsEnabled()
                    ? m_TbsContainer->Collect( m_Kernel, endTimestamp )
                    : StatusCode::Success;

                return log.m_Result;
            }

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief  Enables tbs support.
            /// @return success if tbs has been enabled.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode EnableTbs() const
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                return log.m_Result = m_Kernel.TbsEnable();
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Enables container for tbs reports.
            /// return success if tbs container has been enabled.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode EnableTbsContainer()
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                m_TbsContainer = new( std::nothrow ) TT::TbsContainer();

                return log.m_Result = m_TbsContainer
                    ? StatusCode::Success
                    : StatusCode::OutOfMemory;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Disables tbs support.
            /// @return success if tbs has been disabled.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode DisableTbs() const
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                return m_Kernel.TbsDisable();
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Disables tbs container.
            /// return success if tbs container has been disabled.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode DisableTbsContainer()
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                delete m_TbsContainer;
                m_TbsContainer = nullptr;
                return log.m_Result;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct OaBufferTbsTrait : BASE::OaBufferTbsTrait<T>
        {
            ML_DECLARE_TRAIT( OaBufferTbsTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct OaBufferTbsTrait : GEN9::OaBufferTbsTrait<T>
        {
            ML_DECLARE_TRAIT( OaBufferTbsTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct OaBufferTbsTrait : GEN11::OaBufferTbsTrait<T>
        {
            ML_DECLARE_TRAIT( OaBufferTbsTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
