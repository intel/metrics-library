/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_gpu_command_buffer.h

@brief Writes gpu commands to a command buffer.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief GpuCommandBuffer object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct GpuCommandBufferTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( GpuCommandBufferTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( GpuCommandBufferTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        void*                      m_Buffer;
        uint32_t                   m_Size;
        uint32_t                   m_Usage;
        uint32_t                   m_MemoryPatchesCount;
        const GpuCommandBufferType m_Type;
        const GpuMemory_1_0&       m_GpuMemory;
        TT::Context&               m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief GpuCommandBufferTrait constructor.
        /// @param buffer   internal buffer contains gpu commands.
        /// @param size     internal buffer size.
        /// @param type     internal buffer type.
        /// @param memory   structure that contains pointers to gpu address
        ///                 and command buffer.
        /// @param context  library context.
        //////////////////////////////////////////////////////////////////////////
        GpuCommandBufferTrait(
            void*                      buffer,
            const uint32_t             size,
            const GpuCommandBufferType type,
            const GpuMemory_1_0&       memory,
            TT::Context&               context )
            : m_Buffer( buffer )
            , m_Size( size )
            , m_Usage( 0 )
            , m_MemoryPatchesCount( 0 )
            , m_Type( type )
            , m_GpuMemory( memory )
            , m_Context( context )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &context );
            ML_ASSERT( buffer && size );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns command buffer requirements like needed memory space
        ///         and allocation handles to patch.
        /// @return requirements    command buffer size information.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void GetSizeRequirements( CommandBufferSize_1_0& requirements ) const
        {
            requirements                       = {};
            requirements.GpuMemorySize         = m_Usage;
            requirements.GpuMemoryPatchesCount = m_MemoryPatchesCount;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes gpu command to command buffer.
        /// @param  patchMemory flag that indicates that command should be patched.
        /// @param  command     gpu command to write.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool patchMemory, typename GpuCommand>
        ML_INLINE StatusCode Write( const GpuCommand& command )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            constexpr uint32_t commandSize = sizeof( command );
            const bool         validSpace  = m_Usage + commandSize <= m_Size;
            const bool         validBuffer = m_Buffer && validSpace;

            if( validBuffer )
            {
                T::Tools::MemoryCopy( static_cast<uint8_t*>( m_Buffer ) + m_Usage, m_Size, &command, commandSize );

                m_Usage += commandSize;

                if constexpr( patchMemory && T::Policy::QueryHwCounters::Common::m_PatchGpuMemory )
                {
                    m_MemoryPatchesCount++;

                    const ClientMemoryHandle_1_0 memoryHandle  = m_GpuMemory.HandleMemory;
                    const uint64_t               memoryAddress = GetMemoryAddress( command );
                    const uint64_t               commandOffset = reinterpret_cast<uint64_t>( m_Buffer ) + m_Usage;

                    if( memoryAddress != 0 )
                    {
                        ML_FUNCTION_CHECK( PatchGpuMemory( memoryHandle, memoryAddress, commandOffset ) );
                    }
                }
            }

            ML_ASSERT( validBuffer );

            return log.m_Result = validBuffer
                ? StatusCode::Success
                : StatusCode::InsufficientSpace;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns pointer to the command buffer.
        /// @return pointer to the command buffer.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint8_t* GetBuffer()
        {
            return static_cast<uint8_t*>( m_Buffer );
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns gpu memory address from gpu command.
        /// @param  command gpu command.
        /// @return         gpu memory address.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static uint64_t GetMemoryAddress( const TT::Layouts::GpuCommands::MI_REPORT_PERF_COUNT& command )
        {
            return command.GetMemoryAddress();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns gpu memory address from gpu command.
        /// @param  command gpu command.
        /// @return         gpu memory address.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static uint64_t GetMemoryAddress( const TT::Layouts::GpuCommands::MI_STORE_REGISTER_MEM& command )
        {
            return command.GetMemoryAddress();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns gpu memory address from gpu command.
        /// @param  command gpu command.
        /// @return         gpu memory address.
        //////////////////////////////////////////////////////////////////////////
        template <typename GpuCommand>
        ML_INLINE static uint64_t GetMemoryAddress( const GpuCommand& )
        {
            return 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Calls gpu memory patch callback.
        /// @param  handle          allocation info.
        /// @param  memoryOffset    gpu memory offset.
        /// @param  commandOffset   command buffer memory offset.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode PatchGpuMemory(
            const ClientMemoryHandle_1_0 handle,
            const uint64_t               memoryOffset,
            const uint64_t               commandOffset ) const
        {
            if constexpr( T::Policy::QueryHwCounters::Common::m_PatchGpuMemory )
            {
                if( const auto callback = m_Context.m_ClientCallbacks.GpuMemoryPatch;
                    callback != nullptr )
                {
                    return callback( m_Context.m_ClientHandle, handle, memoryOffset, commandOffset );
                }
            }

            return StatusCode::Success;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    /// @brief GpuCommandBufferCalculatorTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct GpuCommandBufferCalculatorTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( GpuCommandBufferCalculatorTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( GpuCommandBufferCalculatorTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        uint32_t                   m_Usage;
        uint32_t                   m_MemoryPatchesCount;
        const GpuCommandBufferType m_Type;
        TT::Context&               m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief GpuCommandBufferCalculatorTrait constructor.
        /// @param type     internal buffer type.
        /// @param context  library context.
        //////////////////////////////////////////////////////////////////////////
        GpuCommandBufferCalculatorTrait(
            const GpuCommandBufferType type,
            TT::Context&               context )
            : m_Usage( 0 )
            , m_MemoryPatchesCount( 0 )
            , m_Type( type )
            , m_Context( context )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns command buffer requirements like needed memory space
        ///         and allocation handles to patch.
        /// @return requirements    command buffer requirements information.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void GetSizeRequirements( CommandBufferSize_1_0& requirements ) const
        {
            requirements                       = {};
            requirements.GpuMemorySize         = m_Usage;
            requirements.GpuMemoryPatchesCount = m_MemoryPatchesCount;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes gpu command to command buffer.
        /// @param  patchMemory flag that indicates that command should be patched.
        /// @param  command     gpu command to write.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool patchMemory, typename GpuCommand>
        ML_INLINE StatusCode Write( const GpuCommand& command )
        {
            m_Usage += sizeof( command );

            if constexpr( patchMemory && T::Policy::QueryHwCounters::Common::m_PatchGpuMemory )
            {
                m_MemoryPatchesCount++;
            }

            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns null pointer due to calculator command buffer.
        /// @return null pointer.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint8_t* GetBuffer()
        {
            return nullptr;
        }
    };
} // namespace ML
