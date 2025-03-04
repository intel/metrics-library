/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_client_options.h

@brief Stores all client's options that have been passed to the library by them.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for ClientOptionsTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ClientOptionsTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ClientOptionsTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ClientOptionsTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        bool     m_PoshEnabled;
        bool     m_PtbrEnabled;
        bool     m_TbsEnabled;
        bool     m_AsynchronousCompute;
        bool     m_WorkloadPartitionEnabled;
        bool     m_IsSubDevice;
        bool     m_SubDeviceDataPresent;
        uint32_t m_SubDeviceCount;
        uint32_t m_SubDeviceIndex;

        //////////////////////////////////////////////////////////////////////////
        /// @brief ClientOptionsTrait constructor.
        /// @param clientData   client's data.
        /// @param context      context.
        //////////////////////////////////////////////////////////////////////////
        ClientOptionsTrait( const ClientData_1_0& clientData, TT::Context& context )
            : m_PoshEnabled( false )
            , m_PtbrEnabled( false )
            , m_TbsEnabled( false )
            , m_AsynchronousCompute( false )
            , m_WorkloadPartitionEnabled( false )
            , m_IsSubDevice( false )
            , m_SubDeviceDataPresent( false )
            , m_SubDeviceCount( 0 )
            , m_SubDeviceIndex( 0 )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &context );

            // Copy client's options from the pointer provided by the user.
            if( clientData.ClientOptions != nullptr )
            {
                for( uint32_t i = 0; i < clientData.ClientOptionsCount; ++i )
                {
                    switch( const auto& options = clientData.ClientOptions[i];
                            options.Type )
                    {
                        case ClientOptionsType::Posh:
                            m_PoshEnabled = options.Posh.Enabled;
                            log.Info( "Posh enabled", m_PoshEnabled );
                            break;

                        case ClientOptionsType::Ptbr:
                            m_PtbrEnabled = options.Ptbr.Enabled;
                            log.Info( "Ptbr enabled", m_PtbrEnabled );
                            break;

                        case ClientOptionsType::Compute:
                            m_AsynchronousCompute = options.Compute.Asynchronous;
                            log.Info( "Asynchronous compute", m_AsynchronousCompute );
                            break;

                        case ClientOptionsType::Tbs:
                            m_TbsEnabled = options.Tbs.Enabled;
                            log.Info( "Tbs enabled", m_TbsEnabled );
                            break;

                        case ClientOptionsType::SubDevice:
                            m_IsSubDevice          = options.SubDevice.Enabled;
                            m_SubDeviceDataPresent = true;
                            log.Info( "Subdevice enabled", m_IsSubDevice );
                            break;

                        case ClientOptionsType::SubDeviceIndex:
                            m_SubDeviceIndex = options.SubDeviceIndex.Index;
                            log.Info( "Subdevice index", m_SubDeviceIndex );
                            break;

                        case ClientOptionsType::SubDeviceCount:
                            m_SubDeviceCount = options.SubDeviceCount.Count;
                            log.Info( "Subdevice count", m_SubDeviceCount );
                            break;

                        case ClientOptionsType::WorkloadPartition:
                            m_WorkloadPartitionEnabled = options.WorkloadPartition.Enabled;
                            log.Info( "Workload partition enabled", m_WorkloadPartitionEnabled );
                            break;

                        default:
                            log.Warning( "Unknown client option", clientData.ClientOptions[i].Type );
                            break;
                    }
                }
            }
        }
    };
} // namespace ML
