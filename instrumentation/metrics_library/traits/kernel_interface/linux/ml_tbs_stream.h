/*========================== begin_copyright_notice ============================

Copyright (C) 2023-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_tbs_stream.h

@brief Allows to enable, disable and use time-based sampling.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Tbs stream data.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct TbsStreamTrait : TraitObject<T, TT::TbsStream>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( TbsStreamTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( TbsStreamTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = TraitObject<T, TT::TbsStream>;
        using Base::Derived;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::KernelInterface& m_Kernel;
        int32_t              m_Id;
        int32_t              m_MetricSet;
        bool                 m_MetricSetInternal;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Tbs stream constructor.
        /// @param kernel   kernel interface.
        //////////////////////////////////////////////////////////////////////////
        TbsStreamTrait( TT::KernelInterface& kernel )
            : m_Kernel( kernel )
            , m_Id( T::ConstantsOs::Tbs::m_Invalid )
            , m_MetricSet( T::ConstantsOs::Tbs::m_Invalid )
            , m_MetricSetInternal( false )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "TbsStreamTrait<Traits> (Linux)";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes tbs stream.
        /// @return initialization status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            if( m_Kernel.m_Context.m_ClientOptions.m_TbsEnabled )
            {
                // Do not initialize tbs if metrics library was opened to support
                // umd driver that also uses tbs.
                // For example L0 may open metric streamer (tbs) and gpu commands
                // may be needed for stream markers. For such case metrics library,
                // cannot open tbs internally. It would block metric streamer to use tbs.
                // Application should reinitialize metrics library to use query again.
                log.Debug( "Another application is using tbs, tbs will not be used." );
                return log.m_Result;
            }

            // Try to obtain metric set activated by metrics discovery.
            m_MetricSet = m_Kernel.m_IoControl.GetKernelMetricSet();

            // Otherwise, create an internal metric set to enable tbs.
            if( m_MetricSet == T::ConstantsOs::Tbs::m_Invalid )
            {
                m_MetricSet         = m_Kernel.m_IoControl.CreateMetricSet();
                m_MetricSetInternal = m_MetricSet != T::ConstantsOs::Tbs::m_Invalid;
            }

            return log.m_Result = Derived().Enable();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns tbs state.
        /// @return true if tbs is enabled.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsEnabled() const
        {
            return m_Id != T::ConstantsOs::Tbs::m_Invalid;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Allows to use a new metric set by gpu.
        /// @param  set metric set id.
        /// @return     operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode SetMetricSet( int32_t set )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( IsEnabled() );

            log.Debug( "Used set ", m_MetricSet );
            log.Debug( "New set  ", set );

            log.m_Result = m_Kernel.m_IoControl.SetTbsMetricSet( m_Id, set );
            m_MetricSet  = ML_SUCCESS( log.m_Result ) ? set : m_MetricSet;

            log.Debug( "Current set ", m_MetricSet );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Releases metric set usage.
        /// @param  set metric set id to release.
        /// @return     operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode ReleaseMetricSet( const int32_t set )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_ASSERT( m_MetricSet == set );

            if( m_MetricSet != set )
            {
                return log.m_Result = StatusCode::Failed;
            }

            if( m_MetricSetInternal )
            {
                m_Kernel.m_IoControl.RemoveMetricSet( m_MetricSet );
                m_MetricSetInternal = false;
            }

            m_MetricSet = T::ConstantsOs::Tbs::m_Invalid;

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Disables tbs stream.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Disable()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            if( !m_Kernel.m_Context.m_ClientOptions.m_TbsEnabled )
            {
                ML_FUNCTION_CHECK( IsEnabled() );

                // Release used metric set.
                ReleaseMetricSet( m_MetricSet );

                // Close stream.
                m_Kernel.m_IoControl.CloseTbs( m_Id );
                m_Id = T::ConstantsOs::Tbs::m_Invalid;
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Restarts tbs stream.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Restart()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            // Disable stream.
            Disable();

            // Try to obtain metric set activated by metrics discovery.
            m_MetricSet = m_Kernel.m_IoControl.GetKernelMetricSet();

            // Enable stream.
            return log.m_Result = Derived().Enable();
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct TbsStreamTrait : BASE::TbsStreamTrait<T>
    {
        ML_DECLARE_TRAIT( TbsStreamTrait, BASE );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::IsEnabled;
        using Base::ReleaseMetricSet;
        using Base::m_Id;
        using Base::m_Kernel;
        using Base::m_MetricSet;
        using Base::m_MetricSetInternal;

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Enables tbs stream for a given metric set.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Enable()
        {
            std::vector<uint64_t> properties;

            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( IsEnabled() == false );
            ML_FUNCTION_CHECK( m_MetricSet != T::ConstantsOs::Tbs::m_Invalid );
            ML_FUNCTION_CHECK( m_Kernel.m_Tbs.GetStreamProperties( properties, m_MetricSet ) );

            // Enable stream.
            do
            {
                log.m_Result = m_Kernel.m_IoControl.OpenTbs( properties, m_Id );
                log.m_Result = ML_STATUS( ML_SUCCESS( log.m_Result ) && IsEnabled() );
            }
            while( ML_FAIL( log.m_Result ) && ML_SUCCESS( m_Kernel.m_Tbs.UpdateStreamProperties( properties ) ) );

            // Initialize oa buffer mapping if supported.
            if( ML_SUCCESS( log.m_Result ) )
            {
                m_Kernel.m_Tbs.m_OaBufferMapped.Initialize( m_Id );
            }

            // Disable an internal metric set used to enable tbs.
            // It will allow metrics discovery to enable another metrics set.
            if( m_MetricSetInternal )
            {
                ReleaseMetricSet( m_MetricSet );
            }

            return log.m_Result;
        }
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct TbsStreamTrait : GEN9::TbsStreamTrait<T>
    {
        ML_DECLARE_TRAIT( TbsStreamTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct TbsStreamTrait : GEN11::TbsStreamTrait<T>
    {
        ML_DECLARE_TRAIT( TbsStreamTrait, GEN11 );
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct TbsStreamTrait : XE_LP::TbsStreamTrait<T>
    {
        ML_DECLARE_TRAIT( TbsStreamTrait, XE_LP );
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct TbsStreamTrait : XE_HP::TbsStreamTrait<T>
    {
        ML_DECLARE_TRAIT( TbsStreamTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct TbsStreamTrait : XE_HPG::TbsStreamTrait<T>
    {
        ML_DECLARE_TRAIT( TbsStreamTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct TbsStreamTrait : BASE::TbsStreamTrait<T>
    {
        ML_DECLARE_TRAIT( TbsStreamTrait, BASE );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::IsEnabled;
        using Base::ReleaseMetricSet;
        using Base::m_Id;
        using Base::m_Kernel;
        using Base::m_MetricSet;
        using Base::m_MetricSetInternal;

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Enables tbs stream for a given metric set.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Enable()
        {
            drm_xe_ext_set_property properties[DRM_XE_OA_PROPERTY_NO_PREEMPT - DRM_XE_OA_EXTENSION_SET_PROPERTY] = {};

            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( IsEnabled() == false );
            ML_FUNCTION_CHECK( m_MetricSet != T::ConstantsOs::Tbs::m_Invalid );
            ML_FUNCTION_CHECK( m_Kernel.m_Tbs.GetStreamProperties( properties, m_MetricSet ) );

            // Enable stream.
            log.m_Result = m_Kernel.m_IoControl.OpenTbs( properties, m_Id );
            log.m_Result = ML_STATUS( ML_SUCCESS( log.m_Result ) && IsEnabled() );

            // We want a non-blocking read.
            if( const int32_t oldFlags = fcntl( m_Id, F_GETFL, 0 );
                oldFlags != -1 )
            {
                if( fcntl( m_Id, F_SETFL, oldFlags | O_CLOEXEC | O_NONBLOCK ) == -1 )
                {
                    log.Error( "Cannot set a non-blocking read." );
                    log.m_Result = StatusCode::Failed;
                }
            }

            // Initialize oa buffer mapping if supported.
            if( ML_SUCCESS( log.m_Result ) )
            {
                m_Kernel.m_Tbs.m_OaBufferMapped.Initialize( m_Id );
            }

            // Disable an internal metric set used to enable tbs.
            // It will allow metrics discovery to enable another metrics set.
            if( m_MetricSetInternal )
            {
                ReleaseMetricSet( m_MetricSet );
            }

            return log.m_Result;
        }
    };
} // namespace ML::XE2_HPG
