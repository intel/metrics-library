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

@file ml_gpu_command_buffer.h

@brief Writes gpu commands to a command buffer.

\******************************************************************************/

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
        const TT::Context&         m_Context;

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
            const TT::Context&         context )
            : m_Buffer( buffer )
            , m_Size( size )
            , m_Usage( 0 )
            , m_MemoryPatchesCount( 0 )
            , m_Type( type )
            , m_GpuMemory( memory )
            , m_Context( context )
        {
            ML_ASSERT( buffer && size );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "GpuCommandBufferTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks if command buffer calculates gpu commands size only.
        /// @return true if command buffer calculates gpu commands size.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE const bool IsCalculateSizePhase() const
        {
            return false;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns command buffer requirements like needed memory space
        ///         and allocation handles to patch.
        /// @return requirements    command buffer size information.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetSizeRequirements( CommandBufferSize_1_0& requirements ) const
        {
            requirements                       = {};
            requirements.GpuMemorySize         = m_Usage;
            requirements.GpuMemoryPatchesCount = m_MemoryPatchesCount;

            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes gpu command to command buffer.
        /// @param  command         gpu command to write.
        /// @param  patchMemory     flag that indicates that command should be patched.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename GpuCommand>
        ML_INLINE StatusCode Write(
            const GpuCommand& command,
            const bool        patchMemory )
        {
            ML_FUNCTION_LOG( StatusCode::Success );

            const uint32_t commandSize = sizeof( command );
            const bool     validSpace  = m_Usage + commandSize <= m_Size;
            const bool     validBuffer = m_Buffer && validSpace;

            if( validBuffer )
            {
                T::Tools::MemoryCopy( (uint8_t*) m_Buffer + m_Usage, m_Size, &command, commandSize );

                m_Usage += commandSize;
                m_MemoryPatchesCount += patchMemory ? 1 : 0;

                if( patchMemory )
                {
                    const ClientMemoryHandle_1_0 memoryHandle  = m_GpuMemory.HandleMemory;
                    const uint64_t               memoryAddress = GetMemoryAddress( command );
                    const uint64_t               commandOffset = reinterpret_cast<uint64_t>( m_Buffer ) + m_Usage;

                    if( memoryAddress > 0 )
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
        ML_INLINE static uint64_t GetMemoryAddress( const TT::Layouts::GpuCommands::MI_LOAD_REGISTER_IMM& /*command*/ )
        {
            return 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns gpu memory address from gpu command.
        /// @param  command gpu command.
        /// @return         gpu memory address.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static uint64_t GetMemoryAddress( const TT::Layouts::GpuCommands::MI_STORE_DATA_IMM& /*command*/ )
        {
            return 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns gpu memory address from gpu command.
        /// @param  command gpu command.
        /// @return         gpu memory address.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static uint64_t GetMemoryAddress( const TT::Layouts::GpuCommands::PIPE_CONTROL& /*command*/ )
        {
            return 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns gpu memory address from gpu command.
        /// @param  command gpu command.
        /// @return         gpu memory address.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static uint64_t GetMemoryAddress( const TT::Layouts::GpuCommands::MI_COPY_MEM_MEM& /*command*/ )
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
            const bool  validPatchFunction = m_Context.m_ClientCallbacks.GpuMemoryPatch != nullptr;
            const bool  validPatchPolicy   = T::Policy::QueryHwCounters::Common::m_PatchGpuMemory;
            const auto& callback           = m_Context.m_ClientCallbacks.GpuMemoryPatch;

            return validPatchFunction && validPatchPolicy
                ? callback( m_Context.m_ClientHandle, handle, memoryOffset, commandOffset )
                : StatusCode::Success;
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
        const TT::Context&         m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief GpuCommandBufferCalculatorTrait constructor.
        /// @param type     internal buffer type.
        /// @param context  library context.
        //////////////////////////////////////////////////////////////////////////
        GpuCommandBufferCalculatorTrait(
            const GpuCommandBufferType type,
            const TT::Context&         context )
            : m_Usage( 0 )
            , m_MemoryPatchesCount( 0 )
            , m_Type( type )
            , m_Context( context )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "GpuCommandBufferCalculatorTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks if command buffer calculates gpu commands size only.
        /// @return true if command buffer calculates gpu commands size.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE const bool IsCalculateSizePhase() const
        {
            return true;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns command buffer requirements like needed memory space
        ///         and allocation handles to patch.
        /// @return requirements    command buffer requirements information.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetSizeRequirements( CommandBufferSize_1_0& requirements ) const
        {
            requirements                       = {};
            requirements.GpuMemorySize         = m_Usage;
            requirements.GpuMemoryPatchesCount = m_MemoryPatchesCount;

            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes gpu command to command buffer.
        /// @param  command         gpu command to write.
        /// @param  patchMemory     flag that indicates that command should be patched.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename GpuCommand>
        ML_INLINE StatusCode Write(
            const GpuCommand& command,
            const bool        patchMemory )
        {
            m_Usage += sizeof( command );
            m_MemoryPatchesCount += patchMemory ? 1 : 0;

            return StatusCode::Success;
        }
    };
} // namespace ML
