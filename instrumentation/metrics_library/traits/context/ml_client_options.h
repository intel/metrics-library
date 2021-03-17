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

@file ml_client_options.h

@brief Stores all client's options that have been passed to the library by them.

\******************************************************************************/

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
        bool     m_IsSubDevice;
        bool     m_SubDeviceDataPresent;
        uint32_t m_SubDeviceCount;
        uint32_t m_SubDeviceIndex;

        //////////////////////////////////////////////////////////////////////////
        /// @brief ClientOptionsTrait constructor.
        /// @param clientData   client's data.
        //////////////////////////////////////////////////////////////////////////
        ClientOptionsTrait( const ClientData_1_0& clientData )
            : m_PoshEnabled( false )
            , m_PtbrEnabled( false )
            , m_TbsEnabled( false )
            , m_AsynchronousCompute( false )
            , m_IsSubDevice( false )
            , m_SubDeviceDataPresent( false )
            , m_SubDeviceCount( 0 )
            , m_SubDeviceIndex( 0 )
        {
            ML_FUNCTION_LOG( StatusCode::Success );

            // Copy client's options from the pointer provided by the user.
            if( clientData.ClientOptions != nullptr )
            {
                for( uint32_t i = 0; i < clientData.ClientOptionsCount; ++i )
                {
                    auto& options = clientData.ClientOptions[i];

                    switch( clientData.ClientOptions[i].Type )
                    {
                        case ClientOptionsType::Posh:
                            m_PoshEnabled = options.Posh.Enabled;
                            break;

                        case ClientOptionsType::Ptbr:
                            m_PtbrEnabled = options.Ptbr.Enabled;
                            break;

                        case ClientOptionsType::Compute:
                            m_AsynchronousCompute = options.Compute.Asynchronous;
                            break;

                        case ClientOptionsType::Tbs:
                            m_TbsEnabled = options.Tbs.Enabled;
                            break;

                        case ClientOptionsType::SubDevice:
                            m_IsSubDevice          = options.SubDevice.Enabled;
                            m_SubDeviceDataPresent = true;
                            break;

                        case ClientOptionsType::SubDeviceIndex:
                            m_SubDeviceIndex = options.SubDeviceIndex.Index;
                            break;

                        case ClientOptionsType::SubDeviceCount:
                            m_SubDeviceCount = options.SubDeviceCount.Count;
                            break;

                        default:
                            log.Warning( "Unknown client option", clientData.ClientOptions[i].Type );
                            break;
                    }
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "ClientOptionsTrait<Traits>";
        }
    };
} // namespace ML
