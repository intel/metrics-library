/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_context.h

@brief Creates metrics library context, which is the main object of the library
       and contains all created objects in the library. Initializes kernel
       interface and oa buffer.
*/

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
        uint32_t                  m_AdapterId;
        TT::Debug                 m_Debug;
        TT::KernelInterface       m_Kernel;
        TT::OaBuffer              m_OaBuffer;
        TT::ClientOptions         m_ClientOptions;
        const ClientHandle_1_0    m_ClientHandle;
        const ClientCallbacks_1_0 m_ClientCallbacks;
        ClientData_1_0            m_ClientData;
        TT::SubDevice             m_SubDevice;
        TT::DdiObjects            m_DdiObjects;
        TT::State                 m_State;
        TT::InternalFeature       m_Internal;

        //////////////////////////////////////////////////////////////////////////
        /// @brief ContextTrait constructor.
        /// @param clientType   client type.
        /// @param createData   context create data.
        //////////////////////////////////////////////////////////////////////////
        ContextTrait(
            const ClientType_1_0&        clientType,
            const ContextCreateData_1_0& createData )
            : Base( clientType, *this )
            , m_AdapterId( IU_ADAPTER_ID_UNKNOWN )
            , m_Debug{}
            , m_Kernel( *this )
            , m_OaBuffer( m_Kernel )
            , m_ClientOptions( *createData.ClientData, *this )
            , m_ClientHandle( createData.ClientData->Handle )
            , m_ClientCallbacks{ *createData.ClientCallbacks }
            , m_ClientData{ *createData.ClientData }
            , m_SubDevice( *this )
            , m_DdiObjects( *this )
            , m_State{}
            , m_Internal{}
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief ContextTrait destructor.
        //////////////////////////////////////////////////////////////////////////
        ~ContextTrait()
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
            ML_FUNCTION_CHECK_STATIC( createData.Api != nullptr );
            ML_FUNCTION_CHECK_STATIC( createData.ClientData != nullptr );
            ML_FUNCTION_CHECK_STATIC( createData.ClientCallbacks != nullptr );

            auto       context      = Allocate( clientType, createData );
            const bool validContext = context && ML_SUCCESS( context->Initialize() );
            ML_FUNCTION_LOG( StatusCode::Success, validContext ? context : nullptr );

            auto       functionTable  = GetFunctionTable( clientType );
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
            ML_FUNCTION_LOG( StatusCode::Success, this );

            ML_FUNCTION_CHECK( m_Kernel.Initialize( m_ClientData ) );
            ML_FUNCTION_CHECK( m_Kernel.GetAdapterId( m_AdapterId ) );
            ML_FUNCTION_CHECK( m_OaBuffer.Initialize() );

            return log.m_Result;
        }
    };
} // namespace ML
