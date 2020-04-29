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

@file ml_context.h

@brief Creates metrics library context, which is the main object of the library
       and contains all created objects in the library. Initializes kernel
       interface and oa buffer.

\******************************************************************************/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for ContextTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ContextTrait : DdiObject<T, TT::Context, ContextHandle_1_0, ObjectType::Context>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ContextTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ContextTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Context, ContextHandle_1_0, ObjectType::Context>;
        using Base::Allocate;
        using Base::Delete;
        using Base::GetHandle;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::KernelInterface       m_Kernel;
        TT::OaBuffer              m_OaBuffer;
        const ClientHandle_1_0    m_ClientHandle;
        const ClientCallbacks_1_0 m_ClientCallbacks;
        ClientData_1_0            m_ClientData;
        int64_t                   m_OaConfigurationId;
        bool                      m_PoshEnabled;
        bool                      m_PtbrEnabled;
        bool                      m_AsynchronousCompute;

        //////////////////////////////////////////////////////////////////////////
        /// @brief ContextTrait constructor.
        /// @param clientType   client type.
        /// @param createData   context create data.
        //////////////////////////////////////////////////////////////////////////
        ContextTrait(
            const ClientType_1_0&        clientType,
            const ContextCreateData_1_0& createData )
            : Base( clientType )
            , m_Kernel{}
            , m_OaBuffer( m_Kernel )
            , m_ClientHandle( createData.ClientData->Handle )
            , m_ClientCallbacks{ *createData.ClientCallbacks }
            , m_ClientData{ *createData.ClientData }
            , m_OaConfigurationId( 0 )
            , m_PoshEnabled( false )
            , m_PtbrEnabled( false )
            , m_AsynchronousCompute( false )
        {
            // Copy client options from the pointer provided by the user.
            if( m_ClientData.ClientOptions != nullptr )
            {
                for( uint32_t i = 0; i < m_ClientData.ClientOptionsCount; ++i )
                {
                    switch( m_ClientData.ClientOptions[i].Type )
                    {
                        case ClientOptionsType::Posh:
                            m_PoshEnabled = m_ClientData.ClientOptions[i].Posh.Enabled;
                            break;

                        case ClientOptionsType::Ptbr:
                            m_PtbrEnabled = m_ClientData.ClientOptions[i].Ptbr.Enabled;
                            break;

                        case ClientOptionsType::Compute:
                            m_AsynchronousCompute = m_ClientData.ClientOptions[i].Compute.Asynchronous;
                            break;

                        default:
                            ML_ASSERT_ALWAYS();
                            break;
                    }
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief ContextTrait destructor.
        //////////////////////////////////////////////////////////////////////////
        virtual ~ContextTrait()
        {
            // Release reference to oa buffer.
            m_OaBuffer.Release();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "ContextTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Creates context instance.
        /// @param  clientType  api / gen information provided by client.
        /// @param  createData  create context information.
        /// @return handle      handle to created context instance.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode Create(
            const ClientType_1_0&  clientType,
            ContextCreateData_1_0& createData,
            ContextHandle_1_0&     handle )
        {
            ML_FUNCTION_LOG( StatusCode::Success );
            ML_FUNCTION_CHECK( createData.Api != nullptr );
            ML_FUNCTION_CHECK( createData.ClientData != nullptr );
            ML_FUNCTION_CHECK( createData.ClientCallbacks != nullptr );

            auto       context        = Allocate( clientType, createData );
            auto       functionTable  = GetFunctionTable( clientType );
            const bool validContext   = context && ML_SUCCESS( context->Initialize() );
            const bool validInterface = functionTable != nullptr;

            // Validate context and library interface.
            log.m_Result = validContext && validInterface
                ? StatusCode::Success
                : StatusCode::Failed;

            // Return context handle and library api.
            if( ML_SUCCESS( log.m_Result ) )
            {
                functionTable->GetInterface( *createData.Api );
                handle = context->GetHandle();
            }
            else
            {
                Delete( context );
            }

            return log.m_Result;
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes library context.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize()
        {
            ML_FUNCTION_LOG( StatusCode::Success );
            ML_FUNCTION_CHECK( m_Kernel.Initialize( m_ClientData ) );
            ML_FUNCTION_CHECK( m_OaBuffer.Initialize() );

            return log.m_Result;
        }
    };
} // namespace ML
