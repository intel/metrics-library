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

@file ml_gpu_command_buffer_functions.h

@brief Functions that fill gpu commands with a proper data.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for GpuCommandsTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct GpuCommandsTrait
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( GpuCommandsTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( GpuCommandsTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "GpuCommandsTrait<Traits>";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes MI_PIPE_CONTROL to complete current draw call on
            ///         render pipeline.
            /// @param  buffer  target command buffer.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode FlushCommandStreamer( CommandBuffer& buffer )
            {
                TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

                command.Init();
                command.SetStallAtPixelScoreboard( true );
                command.SetCommandStreamerStallEnable( true );

                return buffer.Write( command, false );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes MI_PIPE_CONTROL to flush gpu caches.
            /// @param  buffer  target command buffer.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode FlushCaches( CommandBuffer& buffer )
            {
                TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

                command.Init();
                command.SetRenderTargetCacheFlushEnable( true );
                command.SetDepthCacheFlushEnable( true );
                command.SetDCFlushEnable( true );
                command.SetStallAtPixelScoreboard( true );
                command.SetStateCacheInvalidationEnable( true );
                command.SetTextureCacheInvalidationEnable( true );
                command.SetConstantCacheInvalidationEnable( true );
                command.SetInstructionCacheInvalidateEnable( true );
                command.SetCommandStreamerStallEnable( true );
                command.SetVFCacheInvalidationEnable( true );

                return buffer.Write( command, false );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes MI_STORE_REGISTER_MEMORY command to gpu command buffer.
            /// @param  buffer          target command buffer.
            /// @param  registerAddress register address.
            /// @param  memoryAddress   memory address.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreRegisterToMemory32(
                CommandBuffer& buffer,
                const uint32_t registerAddress,
                const uint64_t memoryAddress )
            {
                TT::Layouts::GpuCommands::MI_STORE_REGISTER_MEM command = {};

                command.Init();
                command.SetUseGlobalGTT( false );
                command.SetRegisterAddress( registerAddress );
                command.SetMemoryAddress( memoryAddress );

                return buffer.Write( command, true );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes MI_STORE_REGISTER_MEMORY command to gpu command buffer.
            /// @param  buffer          target command buffer.
            /// @param  registerAddress register address.
            /// @param  memoryAddress   memory address.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreRegisterToMemory64(
                CommandBuffer& buffer,
                const uint32_t registerAddress,
                const uint64_t memoryAddress )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                    buffer,
                    registerAddress,
                    memoryAddress ) );

                ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                    buffer,
                    registerAddress + sizeof( uint32_t ),
                    memoryAddress + sizeof( uint32_t ) ) );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes MI_STORE_DATA_IMMEDIATE command to gpu command buffer.
            /// @param  buffer  target command buffer.
            /// @param  data    data to write.
            /// @param  address memory offset.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreDataToMemory32(
                CommandBuffer& buffer,
                const uint32_t data,
                const uint64_t address )
            {
                TT::Layouts::GpuCommands::MI_STORE_DATA_IMM command = {};

                command.Init();
                command.SetUseGlobalGTT( false );
                command.SetStoreQword( false );
                command.SetDataDWord0( data );
                command.SetAddress( address );

                return buffer.Write( command, true );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes MI_STORE_DATA_IMMEDIATE command to gpu command buffer.
            /// @param  buffer  target command buffer.
            /// @param  data    data to write.
            /// @param  address memory offset.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreDataToMemory64(
                CommandBuffer& buffer,
                const uint64_t data,
                const uint64_t address )
            {
                const uint32_t dword0 = static_cast<uint32_t>( data & 0xFFFFFFFF );
                const uint32_t dword1 = static_cast<uint32_t>( data >> 32 );

                TT::Layouts::GpuCommands::MI_STORE_DATA_IMM command = {};

                command.Init();
                command.SetUseGlobalGTT( false );
                command.SetDWordLength( T::Layouts::GpuCommands::MI_STORE_DATA_IMM::DWORD_LENGTH_STORE_QWORD );
                command.SetStoreQword( true );
                command.SetDataDWord0( dword0 );
                command.SetDataDWord1( dword1 );
                command.SetAddress( address );

                return buffer.Write( command, true );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes MI_LOAD_REGISTER_IMM command to gpu command buffer.
            /// @param  buffer          target command buffer.
            /// @param  registerAddress register address.
            /// @param  data            data to load.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode LoadRegisterImmediate32(
                CommandBuffer& buffer,
                const uint32_t registerAddress,
                const uint32_t data )
            {
                TT::Layouts::GpuCommands::MI_LOAD_REGISTER_IMM command = {};

                command.Init();
                command.SetRegisterOffset( registerAddress );
                command.SetDataDWord( data );

                return buffer.Write( command, false );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes MI_PIPE_CONTROL to load register immediate with post
            ///         sync operation.
            /// @param  buffer          target command buffer.
            /// @param  registerAddress register address.
            /// @param  data            data to load.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode LoadRegisterImmediatePostSync32(
                CommandBuffer& buffer,
                const uint32_t registerAddress,
                const uint32_t data )
            {
                TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

                command.Init();
                command.SetLRIPostSyncOperation( T::Layouts::GpuCommands::PIPE_CONTROL::LRI_POST_SYNC_OPERATION_MMIO_WRITE_IMMEDIATE_DATA );
                command.SetAddress( registerAddress );
                command.SetImmediateData( data );

                return buffer.Write( command, false );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes MI_PIPE_CONTROL to load register immediate with post
            ///         sync operation and stall command streamer.
            /// @param  buffer          target command buffer.
            /// @param  registerAddress register address.
            /// @param  data            data to load.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode LoadRegisterImmediatePostSyncWithStall32(
                CommandBuffer& buffer,
                const uint32_t registerAddress,
                const uint32_t data )
            {
                TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

                command.Init();
                command.SetLRIPostSyncOperation( T::Layouts::GpuCommands::PIPE_CONTROL::LRI_POST_SYNC_OPERATION_MMIO_WRITE_IMMEDIATE_DATA );
                command.SetAddress( registerAddress );
                command.SetImmediateData( data );
                command.SetStallAtPixelScoreboard( true );
                command.SetCommandStreamerStallEnable( true );

                return buffer.Write( command, false );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes commands to gpu command buffer to store hw counters.
            /// @param  buffer          target command buffer.
            /// @param  collectingMode  oa report collecting mode.
            /// @param  address         memory offset.
            /// @param  reportId        report id.
            /// @param  queryId         query id.
            /// @param  begin           begin query.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreHwCounters(
                CommandBuffer&                                             buffer,
                const TT::Layouts::HwCounters::Query::ReportCollectingMode reportingMode,
                const uint64_t                                             address,
                const uint32_t                                             reportId,
                const uint32_t                                             queryId,
                const bool                                                 begin )
            {
                switch( reportingMode )
                {
                    case T::Layouts::HwCounters::Query::ReportCollectingMode::ReportPerformanceCounters:
                        return T::GpuCommands::StoreHwCountersViaMirpc( buffer, address, reportId, begin );

                    case T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOar:
                        return T::GpuCommands::StoreHwCountersViaSrmOar( buffer, address, reportId, begin );

                    case T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOag:
                        return T::GpuCommands::StoreHwCountersViaOagTriggers( buffer, address, reportId, queryId, begin );

                    default:
                        ML_ASSERT_ALWAYS();
                        break;
                }

                return StatusCode::NotSupported;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes MI_REPORT_PERF_COUNT command to gpu command buffer.
            /// @param  buffer      target command buffer.
            /// @param  address     memory offset.
            /// @param  reportId    report id.
            /// @param  begin       begin query.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreHwCountersViaMirpc(
                CommandBuffer& buffer,
                const uint64_t address,
                const uint32_t reportId,
                const bool     begin )
            {
                TT::Layouts::GpuCommands::MI_REPORT_PERF_COUNT command = {};

                const uint32_t oaReportOffset = begin
                    ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_Begin.m_Oa )
                    : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_End.m_Oa );

                command.Init();
                command.SetUseGlobalGTT( false );
                command.SetReportID( reportId );
                command.SetMemoryAddress( address + oaReportOffset );

                return buffer.Write( command, true );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes STORE_REGISTER_MEM commands to gpu command buffer.
            /// @param  buffer      target command buffer.
            /// @param  address     memory offset.
            /// @param  reportId    report id.
            /// @param  begin       begin query.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreHwCountersViaSrmOar(
                CommandBuffer& buffer,
                const uint64_t address,
                const uint32_t reportId,
                const bool     begin )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                uint64_t       countersOffset = 0;
                const auto&    oaCountersLow  = T::GpuRegisters::GetOaCountersLow();
                const auto&    oaCountersHigh = T::GpuRegisters::GetOaCountersHigh();
                const auto&    noaCounters    = T::GpuRegisters::GetNoaCounters();
                const uint32_t oaReportOffset = begin
                    ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_Begin.m_Oa )
                    : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_End.m_Oa );

                // Lower 32 bits of oa counters.
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Data.m_OaCounter[0] );

                for( size_t i = 0; i < oaCountersLow.size(); ++i )
                {
                    ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                        buffer,
                        oaCountersLow[i],
                        countersOffset + ( ( oaCountersLow[i] - T::GpuRegisters::m_OaPerfA0 ) / 2 ) ) );
                }

                // Higher byte of oa counters.
                countersOffset = begin
                    ? address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_WaBegin )
                    : address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_WaEnd );

                for( size_t i = 0; i < oaCountersHigh.size(); ++i )
                {
                    ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                        buffer,
                        oaCountersHigh[i],
                        countersOffset + ( ( oaCountersHigh[i] - T::GpuRegisters::m_OaPerfA0Upper ) / 2 ) ) );
                }

                // Noa counters (available only 32 bits).
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Data.m_NoaCounter[0] );

                for( size_t i = 0; i < noaCounters.size(); ++i )
                {
                    ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                        buffer,
                        noaCounters[i],
                        countersOffset + ( noaCounters[i] - T::GpuRegisters::m_OaPerfB0 ) ) );
                }

                // Report id.
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ReportId );

                ML_FUNCTION_CHECK( StoreDataToMemory32(
                    buffer,
                    reportId,
                    countersOffset ) );

                // Context id.
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ContextId );

                ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_ContextId,
                    countersOffset ) );

                // Gpu ticks.
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_GpuTicks );

                ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_GpuTicks,
                    countersOffset ) );

                // Timestamp.
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_Timestamp );

                ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_TimestampLow,
                    countersOffset ) );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes gpu commands to trigger oag report.
            /// @param  buffer      target command buffer.
            /// @param  address     report memory offset.
            /// @param  reportId    report id associated with executed query.
            /// @param  queryId     query id associated with context id field within
            ///                     a generated report.
            /// @param  begin       begin/end query indicator.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreHwCountersViaOagTriggers(
                CommandBuffer& buffer,
                const uint64_t address,
                const uint32_t reportId,
                const uint32_t queryId,
                const bool     begin )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const uint32_t oaReportOffset = begin
                    ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_Begin.m_Oa )
                    : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_End.m_Oa );

                // Timestamp before triggered report.
                if( begin )
                {
                    ML_FUNCTION_CHECK( T::GpuCommands::StoreTimestamp(
                        buffer,
                        address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_Timestamp ) ) );

                    ML_FUNCTION_CHECK( StoreDataToMemory32(
                        buffer,
                        reportId,
                        address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ReportId ) ) );
                }
                // OaTail before triggered report.
                else
                {
                    ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                        buffer,
                        T::GpuRegisters::m_OaTail,
                        address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaTailTriggerEnd ) ) );
                }

                // Trigger report in oa buffer.
                ML_FUNCTION_CHECK( T::GpuCommands::TriggerQueryReport(
                    buffer,
                    queryId ) );

                // Timestamp after triggered report.
                if( !begin )
                {
                    ML_FUNCTION_CHECK( T::GpuCommands::StoreTimestamp(
                        buffer,
                        address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_Timestamp ) ) );

                    ML_FUNCTION_CHECK( StoreDataToMemory32(
                        buffer,
                        reportId,
                        address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ReportId ) ) );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes LOAD_REGISTER_IMM commands to trigger oa report with
            ///         report reason 2.
            /// @param  buffer  target command buffer.
            /// @param  queryId query id associated with context id field within a
            ///                 generated report.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode TriggerQueryReport(
                CommandBuffer& buffer,
                const uint32_t /*queryId*/ )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                auto risingEdge  = TT::Layouts::GpuRegisters::OaReportTrigger( false );
                auto fallingEdge = TT::Layouts::GpuRegisters::OaReportTrigger( true );

                // Write rising edge.
                ML_FUNCTION_CHECK( LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_OaTrigger2,
                    risingEdge.m_Value ) );

                // Write falling edge.
                ML_FUNCTION_CHECK( LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_OaTrigger2,
                    fallingEdge.m_Value ) );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes LOAD_REGISTER_IMM commands to trigger oa report with
            ///         report reason 4.
            /// @param  buffer  target command buffer.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode TriggerStreamReport( CommandBuffer& buffer )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                auto risingEdge  = TT::Layouts::GpuRegisters::OaReportTrigger( false );
                auto fallingEdge = TT::Layouts::GpuRegisters::OaReportTrigger( true );

                // Write rising edge.
                ML_FUNCTION_CHECK( LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_OaTrigger6,
                    risingEdge.m_Value ) );

                // Write falling edge.
                ML_FUNCTION_CHECK( LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_OaTrigger6,
                    fallingEdge.m_Value ) );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes PIPE_CONTROL commands to trigger oa report with post
            ///         sync operation.
            /// @param  buffer  target command buffer.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode TriggerStreamReportPostSync( CommandBuffer& buffer )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                auto risingEdge  = TT::Layouts::GpuRegisters::OaReportTrigger( false );
                auto fallingEdge = TT::Layouts::GpuRegisters::OaReportTrigger( true );

                ML_FUNCTION_CHECK( LoadRegisterImmediatePostSync32(
                    buffer,
                    T::GpuRegisters::m_OaTrigger6,
                    risingEdge.m_Value ) );

                ML_FUNCTION_CHECK( LoadRegisterImmediatePostSync32(
                    buffer,
                    T::GpuRegisters::m_OaTrigger6,
                    fallingEdge.m_Value ) );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes timestamp command to gpu command buffer.
            /// @param  buffer          target command buffer.
            /// @param  memoryAddress   memory address where timestamp should be stored.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreTimestamp(
                CommandBuffer& buffer,
                const uint64_t memoryAddress )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_TimestampLow,
                    memoryAddress ) );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes pipeline timestamp command to gpu command buffer.
            /// @param  buffer          target command buffer.
            /// @param  memoryAddress   memory address where timestamp should be stored.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StorePipelineTimestamp(
                CommandBuffer& buffer,
                const uint64_t memoryAddress )
            {
                TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

                const uint32_t addressLow  = static_cast<uint32_t>( memoryAddress );
                const uint32_t addressHigh = static_cast<uint32_t>( memoryAddress >> 32 );

                command.Init();
                command.SetPostSyncOperation( T::Layouts::GpuCommands::PIPE_CONTROL::POST_SYNC_OPERATION_WRITE_TIMESTAMP );
                command.SetAddress( addressLow );
                command.SetAddressHigh( addressHigh );

                return buffer.Write( command, true );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Copies a given data from one gpu memory address into another.
            /// @param  buffer          target command buffer.
            /// @param  addressSource   source memory address.
            /// @param  addressTarget   target memory address.
            /// @param  size            data size in bytes.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode CopyData(
                CommandBuffer& buffer,
                const uint64_t addressSource,
                const uint64_t addressTarget,
                const uint32_t size )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_ASSERT( ( size % sizeof( uint32_t ) ) == 0 );

                TT::Layouts::GpuCommands::MI_COPY_MEM_MEM command = {};

                for( uint32_t i = 0; i < size; i += sizeof( uint32_t ) )
                {
                    command.Init();
                    command.SetSourceMemoryAddress( addressSource + i );
                    command.SetDestinationMemoryAddress( addressTarget + i );

                    ML_FUNCTION_CHECK( buffer.Write( command, true ) );
                }

                return log.m_Result;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct GpuCommandsTrait : BASE::GpuCommandsTrait<T>
        {
            ML_DECLARE_TRAIT( GpuCommandsTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct GpuCommandsTrait : GEN9::GpuCommandsTrait<T>
        {
            ML_DECLARE_TRAIT( GpuCommandsTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct GpuCommandsTrait : GEN11::GpuCommandsTrait<T>
        {
            ML_DECLARE_TRAIT( GpuCommandsTrait, GEN11 );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Types.
            //////////////////////////////////////////////////////////////////////////
            using Base::StoreDataToMemory32;
            using Base::StoreRegisterToMemory32;

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes timestamp command to gpu command buffer.
            /// @param  buffer          target command buffer.
            /// @param  memoryAddress   memory address where timestamp should be stored.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreTimestamp(
                CommandBuffer& buffer,
                const uint64_t memoryAddress )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const uint32_t registerAddress = buffer.m_Context.m_ClientOptions.m_AsynchronousCompute && buffer.m_Type == GpuCommandBufferType::Compute
                    ? T::GpuRegisters::m_CcsTimestampLow
                    : T::GpuRegisters::m_TimestampLow;

                ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                    buffer,
                    registerAddress,
                    memoryAddress ) );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes commands to gpu command buffer to store hw counters.
            /// @param  buffer          target command buffer.
            /// @param  collectingMode  oa report collecting mode.
            /// @param  address         memory offset.
            /// @param  reportId        report id.
            /// @param  queryId         query id.
            /// @param  begin           begin query.
            /// @return                 operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreHwCounters(
                CommandBuffer&                                             buffer,
                const TT::Layouts::HwCounters::Query::ReportCollectingMode reportingMode,
                const uint64_t                                             address,
                const uint32_t                                             reportId,
                const uint32_t                                             queryId,
                const bool                                                 begin )
            {
                switch( reportingMode )
                {
                    case T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOag:
                        return T::GpuCommands::StoreHwCountersViaSrmOag( buffer, address, reportId, begin );

                    default:
                        return Base::StoreHwCounters(
                            buffer,
                            reportingMode,
                            address,
                            reportId,
                            queryId,
                            begin );
                }
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes STORE_REGISTER_MEM commands to gpu command buffer.
            /// @param  buffer      target command buffer.
            /// @param  address     memory offset.
            /// @param  reportId    report id.
            /// @param  begin       begin query.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode StoreHwCountersViaSrmOag(
                CommandBuffer& buffer,
                const uint64_t address,
                const uint32_t reportId,
                const bool     begin )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                uint64_t       countersOffset  = 0;
                const auto&    oagCountersLow  = T::GpuRegisters::GetOagCountersLow();
                const auto&    oagCountersHigh = T::GpuRegisters::GetOagCountersHigh();
                const auto&    noagCounters    = T::GpuRegisters::GetNoagCounters();
                const uint32_t oaReportOffset  = begin
                    ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_Begin.m_Oa )
                    : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_End.m_Oa );

                // Lower 32 bits of oa counters.
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Data.m_OaCounter[0] );

                for( size_t i = 0; i < oagCountersLow.size(); ++i )
                {
                    ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                        buffer,
                        oagCountersLow[i],
                        countersOffset + ( ( oagCountersLow[i] - T::GpuRegisters::m_OagPerfA0 ) / 2 ) ) );
                }

                // Higher byte of oa counters.
                countersOffset = begin
                    ? address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_WaBegin )
                    : address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_WaEnd );

                for( size_t i = 0; i < oagCountersHigh.size(); ++i )
                {
                    ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                        buffer,
                        oagCountersHigh[i],
                        countersOffset + ( ( oagCountersHigh[i] - T::GpuRegisters::m_OagPerfA0Upper ) / 2 ) ) );
                }

                // Noa counters (available only 32 bits).
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Data.m_NoaCounter[0] );

                for( size_t i = 0; i < noagCounters.size(); ++i )
                {
                    ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                        buffer,
                        noagCounters[i],
                        countersOffset + ( noagCounters[i] - T::GpuRegisters::m_OagPerfB0 ) ) );
                }

                // Report id.
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ReportId );

                ML_FUNCTION_CHECK( StoreDataToMemory32(
                    buffer,
                    reportId,
                    countersOffset ) );

                // Context id.
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ContextId );

                ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_ContextId,
                    countersOffset ) );

                // Gpu ticks.
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_GpuTicks );

                ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_OagGpuTicks,
                    countersOffset ) );

                // Timestamp.
                countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_Timestamp );

                const uint32_t registerAddress = buffer.m_Context.m_ClientOptions.m_AsynchronousCompute && buffer.m_Type == GpuCommandBufferType::Compute
                    ? T::GpuRegisters::m_CcsTimestampLow
                    : T::GpuRegisters::m_TimestampLow;

                ML_FUNCTION_CHECK( StoreRegisterToMemory32(
                    buffer,
                    registerAddress,
                    countersOffset ) );

                return log.m_Result;
            }
        };
    } // namespace GEN12
} // namespace ML
