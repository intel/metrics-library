/*========================== begin_copyright_notice ============================

Copyright (C) 2023-2026 Intel Corporation

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
    template <typename T, bool isMert>
    struct TbsStreamTrait : TraitObject<T, std::conditional_t<isMert, TT::TbsStreamMert, TT::TbsStream>>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( TbsStreamTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( TbsStreamTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = TraitObject<T, std::conditional_t<isMert, TT::TbsStreamMert, TT::TbsStream>>;
        using Base::Derived;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::KernelInterface& m_Kernel;
        int32_t              m_Id;
        int32_t              m_MetricSet;
        int32_t              m_MetricSetInternal;
        uint64_t             m_MetricSetModificationTimestamp;
        uint64_t             m_MetricSetIndexNode;
        bool                 m_IsMetricSetUpdateRequired;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Tbs stream constructor.
        /// @param kernel   kernel interface.
        //////////////////////////////////////////////////////////////////////////
        TbsStreamTrait( TT::KernelInterface& kernel )
            : Base()
            , m_Kernel( kernel )
            , m_Id( T::ConstantsOs::Drm::m_Invalid )
            , m_MetricSet( T::ConstantsOs::Drm::m_Invalid )
            , m_MetricSetInternal( T::ConstantsOs::Drm::m_Invalid )
            , m_MetricSetModificationTimestamp( 0 )
            , m_MetricSetIndexNode( 0 )
            , m_IsMetricSetUpdateRequired( true )
        {
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
            if constexpr( isMert )
            {
                m_MetricSet = m_Kernel.m_IoControl.GetKernelMertMetricSet();
            }
            else
            {
                m_MetricSet = m_Kernel.m_IoControl.GetKernelMetricSet();
            }

            // Otherwise, create an internal metric set to enable tbs.
            if( m_MetricSet == T::ConstantsOs::Drm::m_Invalid )
            {
                m_MetricSetInternal = m_Kernel.m_IoControl.template CreateMetricSet<isMert>();
            }

            return log.m_Result = Derived().Enable();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns tbs state.
        /// @return true if tbs is enabled.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool IsEnabled() const
        {
            return m_Id != T::ConstantsOs::Drm::m_Invalid;
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

            log.Debug( "Used set", m_MetricSet );
            log.Debug( "New set", set );

            if( m_IsMetricSetUpdateRequired )
            {
                log.m_Result = m_Kernel.m_IoControl.SetTbsMetricSet( m_Id, set );
            }

            m_MetricSet = ML_SUCCESS( log.m_Result ) ? set : m_MetricSet;

            log.Debug( "Current set", m_MetricSet );

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
            ML_ASSERT( m_MetricSet == set || m_MetricSetInternal == set );

            if( m_MetricSet != set && m_MetricSetInternal != set )
            {
                return log.m_Result = StatusCode::Failed;
            }

            if( m_MetricSetInternal != T::ConstantsOs::Drm::m_Invalid )
            {
                m_Kernel.m_IoControl.RemoveMetricSet( m_MetricSetInternal );
                m_MetricSetInternal = T::ConstantsOs::Drm::m_Invalid;
            }

            m_MetricSet = T::ConstantsOs::Drm::m_Invalid;

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
                m_Id = T::ConstantsOs::Drm::m_Invalid;
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Updates metric set info and checks if tbs metric set needs to
        ///         be updated on next activate.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode UpdateMetricSetInfo()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            uint64_t modificationTimestamp = 0;
            uint64_t indexNode             = 0;

            log.Info( "Old modification timestamp", m_MetricSetModificationTimestamp );
            log.Info( "Old index node", m_MetricSetIndexNode );

            if constexpr( isMert )
            {
                ML_FUNCTION_CHECK( m_Kernel.m_IoControl.GetKernelMertMetricSetInfo( modificationTimestamp, indexNode ) );
            }
            else
            {
                ML_FUNCTION_CHECK( m_Kernel.m_IoControl.GetKernelMetricSetInfo( modificationTimestamp, indexNode ) );
            }

            // OA configuration changed if modification timestamp and index node have initial values or
            // modification timestamp or index node changed.
            m_IsMetricSetUpdateRequired =
                ( m_MetricSetModificationTimestamp == 0 && m_MetricSetIndexNode == 0 ) ||
                ( m_MetricSetModificationTimestamp != modificationTimestamp ) ||
                ( m_MetricSetIndexNode != indexNode );

            m_MetricSetModificationTimestamp = modificationTimestamp;
            m_MetricSetIndexNode             = indexNode;

            log.Info( "Metric set update required", m_IsMetricSetUpdateRequired );

            return log.m_Result;
        }
    };
} // namespace ML::BASE

namespace ML::XE_LP
{
    template <typename T, bool isMert>
    struct TbsStreamTrait : BASE::TbsStreamTrait<T, isMert>
    {
        ML_DECLARE_TRAIT_WITH_TEMPLATE_ARG( TbsStreamTrait, BASE, isMert );

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
            ML_FUNCTION_CHECK( m_MetricSet != T::ConstantsOs::Drm::m_Invalid || m_MetricSetInternal != T::ConstantsOs::Drm::m_Invalid );
            ML_FUNCTION_CHECK( m_Kernel.m_Tbs.GetStreamProperties( properties, m_MetricSet != T::ConstantsOs::Drm::m_Invalid ? m_MetricSet : m_MetricSetInternal ) );

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
            if( m_MetricSetInternal != T::ConstantsOs::Drm::m_Invalid )
            {
                ReleaseMetricSet( m_MetricSetInternal );
            }

            return log.m_Result;
        }
    };
} // namespace ML::XE_LP

namespace ML::XE_HPG
{
    template <typename T, bool isMert>
    struct TbsStreamTrait : XE_LP::TbsStreamTrait<T, isMert>
    {
        ML_DECLARE_TRAIT_WITH_TEMPLATE_ARG( TbsStreamTrait, XE_LP, isMert );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T, bool isMert>
    struct TbsStreamTrait : XE_HPG::TbsStreamTrait<T, isMert>
    {
        ML_DECLARE_TRAIT_WITH_TEMPLATE_ARG( TbsStreamTrait, XE_HPG, isMert );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T, bool isMert>
    struct TbsStreamTrait : BASE::TbsStreamTrait<T, isMert>
    {
        ML_DECLARE_TRAIT_WITH_TEMPLATE_ARG( TbsStreamTrait, BASE, isMert );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::IsEnabled;
        using Base::ReleaseMetricSet;
        using Base::m_Id;
        using Base::m_Kernel;
        using Base::m_MetricSet;
        using Base::m_MetricSetInternal;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Enables tbs stream for a given metric set.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Enable()
        {
            drm_xe_ext_set_property properties[DRM_XE_OA_PROPERTY_WAIT_NUM_REPORTS - DRM_XE_OA_EXTENSION_SET_PROPERTY] = {};

            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( IsEnabled() == false );
            ML_FUNCTION_CHECK( m_MetricSet != T::ConstantsOs::Drm::m_Invalid || m_MetricSetInternal != T::ConstantsOs::Drm::m_Invalid );
            ML_FUNCTION_CHECK( m_Kernel.m_Tbs.GetStreamProperties( properties, m_MetricSet != T::ConstantsOs::Drm::m_Invalid ? m_MetricSet : m_MetricSetInternal ) );

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
            if( m_MetricSetInternal != T::ConstantsOs::Drm::m_Invalid )
            {
                ReleaseMetricSet( m_MetricSetInternal );
            }

            return log.m_Result;
        }
    };
} // namespace ML::XE2_HPG

namespace ML::XE3
{
    template <typename T, bool isMert>
    struct TbsStreamTrait : XE2_HPG::TbsStreamTrait<T, isMert>
    {
        ML_DECLARE_TRAIT_WITH_TEMPLATE_ARG( TbsStreamTrait, XE2_HPG, isMert );
    };
} // namespace ML::XE3

namespace ML::XE3P
{
    template <typename T, bool isMert>
    struct TbsStreamTrait : XE3::TbsStreamTrait<T, isMert>
    {
        ML_DECLARE_TRAIT_WITH_TEMPLATE_ARG( TbsStreamTrait, XE3, isMert );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::IsEnabled;
        using Base::ReleaseMetricSet;
        using Base::m_Id;
        using Base::m_Kernel;
        using Base::m_MetricSet;
        using Base::m_MetricSetInternal;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Enables tbs stream for a given metric set.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Enable()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            if constexpr( !isMert )
            {
                return log.m_Result = Base::Enable();
            }
            else
            {
                drm_xe_ext_set_property properties[DRM_XE_OA_PROPERTY_WAIT_NUM_REPORTS - DRM_XE_OA_EXTENSION_SET_PROPERTY] = {};

                ML_FUNCTION_CHECK( IsEnabled() == false );
                ML_FUNCTION_CHECK( m_MetricSet != T::ConstantsOs::Drm::m_Invalid || m_MetricSetInternal != T::ConstantsOs::Drm::m_Invalid );
                ML_FUNCTION_CHECK( m_Kernel.m_Tbs.GetMertStreamProperties( properties, m_MetricSet != T::ConstantsOs::Drm::m_Invalid ? m_MetricSet : m_MetricSetInternal ) );

                // Enable mert stream.
                log.m_Result = m_Kernel.m_IoControl.OpenTbs( properties, m_Id );
                log.m_Result = ML_STATUS( ML_SUCCESS( log.m_Result ) && IsEnabled() );

                // Disable an internal metric set used to enable tbs.
                // It will allow metrics discovery to enable another metrics set.
                if( m_MetricSetInternal != T::ConstantsOs::Drm::m_Invalid )
                {
                    ReleaseMetricSet( m_MetricSetInternal );
                }

                return log.m_Result;
            }
        }
    };
} // namespace ML::XE3P
