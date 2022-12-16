/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_gpu_command_buffer_functions.h

@brief Functions that fill gpu commands with a proper data.
*/

#pragma once

namespace ML::BASE
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
        /// @brief Gpu command flags.
        //////////////////////////////////////////////////////////////////////////
        enum class Flags : uint32_t
        {
            None              = 0,
            EnableMmioRemap   = ML_BIT( 0 ),
            EnableStall       = ML_BIT( 1 ),
            EnablePostSync    = ML_BIT( 2 ),
            WorkloadPartition = ML_BIT( 3 )
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "GpuCommandsTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes PIPE_CONTROL to gpu command buffer to complete
        ///         current draw call on render pipeline.
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
        /// @brief  Writes PIPE_CONTROL to gpu command buffer to flush gpu caches.
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
        /// @brief  Writes MI_STORE_REGISTER_MEMORY command to gpu command buffer
        ///         to store register 32 bit data in memory.
        /// @param  buffer          target command buffer.
        /// @param  registerAddress register address.
        /// @param  memoryAddress   memory address.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreRegisterToMemory32(
            CommandBuffer&               buffer,
            const uint32_t               registerAddress,
            const uint64_t               memoryAddress,
            [[maybe_unused]] const Flags flags )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            log.Input( "registerAddress", registerAddress );
            log.Input( "memoryAddress", memoryAddress );

            TT::Layouts::GpuCommands::MI_STORE_REGISTER_MEM command = {};

            command.Init();
            command.SetUseGlobalGTT( false );
            command.SetRegisterAddress( registerAddress );
            command.SetMemoryAddress( memoryAddress );

            return log.m_Result = buffer.Write( command, true );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_STORE_REGISTER_MEMORY command to gpu command buffer
        ///         to store register 64 bit data in memory.
        /// @param  buffer          target command buffer.
        /// @param  registerAddress register address.
        /// @param  memoryAddress   memory address.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreRegisterToMemory64(
            CommandBuffer& buffer,
            const uint32_t registerAddress,
            const uint64_t memoryAddress,
            const Flags    flags )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                registerAddress,
                memoryAddress,
                flags ) );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                registerAddress + sizeof( uint32_t ),
                memoryAddress + sizeof( uint32_t ),
                flags ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_STORE_DATA_IMMEDIATE command to gpu command buffer
        ///         to store a 32 bit constant in memory.
        /// @param  buffer  target command buffer.
        /// @param  data    data to write.
        /// @param  address memory offset.
        /// @param  flags   gpu command flags.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreDataToMemory32(
            CommandBuffer&               buffer,
            const uint32_t               data,
            const uint64_t               address,
            [[maybe_unused]] const Flags flags )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            log.Input( "data", data );
            log.Input( "address", address );

            TT::Layouts::GpuCommands::MI_STORE_DATA_IMM command = {};

            command.Init();
            command.SetUseGlobalGTT( false );
            command.SetStoreQword( false );
            command.SetDataDWord0( data );
            command.SetAddress( address );

            return log.m_Result = buffer.Write( command, true );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_STORE_DATA_IMMEDIATE or PIPE_CONTROL command to gpu
        ///         command buffer to store a 64 bit constant in memory.
        /// @param  data    data to write.
        /// @param  address memory offset.
        /// @param  flags   gpu command flags.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreDataToMemory64(
            CommandBuffer& buffer,
            const uint64_t data,
            const uint64_t address,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( CheckFlags( flags, Flags::EnablePostSync ) );

            log.Input( "data", data );
            log.Input( "address", address );
            log.Input( "flags", flags );

            const bool isPostSyncEnabled = static_cast<uint32_t>( flags & Flags::EnablePostSync ) != 0;

            if( isPostSyncEnabled )
            {
                const uint32_t addressLow  = static_cast<uint32_t>( address & 0xFFFFFFFF );
                const uint32_t addressHigh = static_cast<uint32_t>( address >> 32 );

                TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

                command.Init();
                command.SetPostSyncOperation( T::Layouts::GpuCommands::PIPE_CONTROL::POST_SYNC_OPERATION_WRITE_IMMEDIATE_DATA );
                command.SetAddress( addressLow );
                command.SetAddressHigh( addressHigh );
                command.SetImmediateData( data );

                return log.m_Result = buffer.Write( command, true );
            }
            else
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

                return log.m_Result = buffer.Write( command, true );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_LOAD_REGISTER_IMM or PIPE_CONTROL command to gpu
        ///         command buffer to load a register with a 32 bit constant.
        /// @param  buffer          target command buffer.
        /// @param  registerAddress register address.
        /// @param  data            data to load.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode LoadRegisterImmediate32(
            CommandBuffer& buffer,
            const uint32_t registerAddress,
            const uint32_t data,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( CheckFlags( flags, Flags::EnablePostSync, Flags::EnableStall ) );

            log.Input( "registerAddress", registerAddress );
            log.Input( "data", data );
            log.Input( "flags", flags );

            const bool isStallEnabled    = static_cast<uint32_t>( flags & Flags::EnableStall ) != 0;
            const bool isPostSyncEnabled = static_cast<uint32_t>( flags & Flags::EnablePostSync ) != 0;

            if( isPostSyncEnabled )
            {
                TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

                command.Init();
                command.SetLRIPostSyncOperation( T::Layouts::GpuCommands::PIPE_CONTROL::LRI_POST_SYNC_OPERATION_MMIO_WRITE_IMMEDIATE_DATA );
                command.SetAddress( registerAddress );
                command.SetImmediateData( data );

                if( isStallEnabled )
                {
                    command.SetStallAtPixelScoreboard( true );
                    command.SetCommandStreamerStallEnable( true );
                }

                return log.m_Result = buffer.Write( command, false );
            }
            else
            {
                TT::Layouts::GpuCommands::MI_LOAD_REGISTER_IMM command = {};

                command.Init();
                command.SetRegisterOffset( registerAddress );
                command.SetDataDWord( data );

                return log.m_Result = buffer.Write( command, false );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_LOAD_REGISTER_REG command to gpu command buffer to
        ///         load a register with another register data.
        /// @param  buffer                      target command buffer.
        /// @param  sourceRegisterAddress       source register address.
        /// @param  destinationRegisterAddress  destination register address.
        /// @param  flags                       gpu command flags.
        /// @return                             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode LoadRegisterToRegister32(
            CommandBuffer&               buffer,
            const uint32_t               sourceRegisterAddress,
            const uint32_t               destinationRegisterAddress,
            [[maybe_unused]] const Flags flags )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            log.Input( "sourceRegisterAddress", sourceRegisterAddress );
            log.Input( "destinationRegisterAddress", destinationRegisterAddress );

            TT::Layouts::GpuCommands::MI_LOAD_REGISTER_REG command = {};

            command.Init();
            command.SetSourceRegisterAddress( sourceRegisterAddress );
            command.SetDestinationRegisterAddress( destinationRegisterAddress );

            return log.m_Result = buffer.Write( command, false );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes commands to gpu command buffer to store hw counters.
        /// @param  buffer          target command buffer.
        /// @param  collectingMode  oa report collecting mode.
        /// @param  address         memory offset.
        /// @param  reportId        report id.
        /// @param  queryId         query id.
        /// @param  begin           begin query.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreHwCounters(
            CommandBuffer&                                             buffer,
            const TT::Layouts::HwCounters::Query::ReportCollectingMode collectingMode,
            const uint64_t                                             address,
            const uint32_t                                             reportId,
            const uint32_t                                             queryId,
            const bool                                                 begin,
            const Flags                                                flags = Flags::None )
        {
            switch( collectingMode )
            {
                case T::Layouts::HwCounters::Query::ReportCollectingMode::ReportPerformanceCounters:
                    return T::GpuCommands::StoreHwCountersViaMirpc(
                        buffer,
                        address,
                        reportId,
                        begin );

                case T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOar:
                    return T::GpuCommands::StoreHwCountersViaSrmOar(
                        buffer,
                        address,
                        reportId,
                        begin,
                        flags );

                case T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOag:
                case T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOagExtended:
                    return T::GpuCommands::StoreHwCountersViaOagTriggers(
                        buffer,
                        address,
                        reportId,
                        queryId,
                        begin,
                        flags );

                default:
                    ML_ASSERT_ALWAYS_ADAPTER( buffer.m_Context.m_AdapterId );
                    break;
            }

            return StatusCode::NotSupported;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_REPORT_PERF_COUNT command to gpu command buffer to
        ///         store hw counters.
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
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            log.Input( "address", address );
            log.Input( "reportId", reportId );
            log.Input( "begin", begin );

            TT::Layouts::GpuCommands::MI_REPORT_PERF_COUNT command = {};

            const uint32_t oaReportOffset = begin
                ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_Begin.m_Oa )
                : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_End.m_Oa );

            command.Init();
            command.SetUseGlobalGTT( false );
            command.SetReportID( reportId );
            command.SetMemoryAddress( address + oaReportOffset );

            return log.m_Result = buffer.Write( command, true );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes STORE_REGISTER_MEM commands to gpu command buffer to
        ///         store hw counters.
        /// @param  buffer      target command buffer.
        /// @param  address     memory offset.
        /// @param  reportId    report id.
        /// @param  begin       begin query.
        /// @param  flags       gpu command flags.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreHwCountersViaSrmOar(
            CommandBuffer& buffer,
            const uint64_t address,
            const uint32_t reportId,
            const bool     begin,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

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
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    oaCountersLow[i],
                    countersOffset + ( ( oaCountersLow[i] - T::GpuRegisters::m_OaPerfA0 ) / 2 ),
                    flags ) );
            }

            // Higher byte of oa counters.
            countersOffset = begin
                ? address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_WaBegin )
                : address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_WaEnd );

            for( size_t i = 0; i < oaCountersHigh.size(); ++i )
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    oaCountersHigh[i],
                    countersOffset + ( ( oaCountersHigh[i] - T::GpuRegisters::m_OaPerfA0Upper ) / 2 ),
                    flags ) );
            }

            // Noa counters (available only 32 bits).
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Data.m_NoaCounter[0] );

            for( size_t i = 0; i < noaCounters.size(); ++i )
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    noaCounters[i],
                    countersOffset + ( noaCounters[i] - T::GpuRegisters::m_OaPerfB0 ),
                    flags ) );
            }

            // Report id.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ReportId );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreDataToMemory32(
                buffer,
                reportId,
                countersOffset,
                flags ) );

            // Context id.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ContextId );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_ContextId,
                countersOffset,
                flags ) );

            // Gpu ticks.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_GpuTicks );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_GpuTicks,
                countersOffset,
                flags ) );

            // Timestamp.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_Timestamp );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_TimestampLow,
                countersOffset,
                flags ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes gpu commands to trigger oag report and store hw
        ///         counters.
        /// @param  buffer      target command buffer.
        /// @param  address     report memory offset.
        /// @param  reportId    report id associated with executed query.
        /// @param  queryId     query id associated with context id field within
        ///                     a generated report.
        /// @param  begin       begin/end query indicator.
        /// @param  flags       gpu command flags.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreHwCountersViaOagTriggers(
            CommandBuffer& buffer,
            const uint64_t address,
            const uint32_t reportId,
            const uint32_t queryId,
            const bool     begin,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            const uint32_t oaTailPostBeginOffset = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaTailPostBegin );
            const uint32_t oaTailPreEndOffset    = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaTailPreEnd );
            const uint32_t oaReportOffset        = begin
                       ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_Begin.m_Oa )
                       : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_End.m_Oa );

            // Ticks before triggered report.
            if( begin )
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreTicks(
                    buffer,
                    address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_GpuTicks ),
                    flags ) );

                ML_FUNCTION_CHECK( T::GpuCommands::StoreDataToMemory32(
                    buffer,
                    reportId,
                    address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ReportId ),
                    flags ) );
            }
            else
            {
                // OaTail before triggered report.
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_OaTail,
                    address + oaTailPreEndOffset,
                    flags ) );
            }

            // Trigger report in oa buffer.
            ML_FUNCTION_CHECK( T::GpuCommands::TriggerQueryReport(
                buffer,
                queryId ) );

            if( begin )
            {
                // OaTail after triggered report.
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_OaTail,
                    address + oaTailPostBeginOffset,
                    flags ) );
            }
            else
            {
                // Ticks after triggered report.
                ML_FUNCTION_CHECK( T::GpuCommands::StoreTicks(
                    buffer,
                    address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_GpuTicks ),
                    flags ) );

                ML_FUNCTION_CHECK( T::GpuCommands::StoreDataToMemory32(
                    buffer,
                    reportId,
                    address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ReportId ),
                    flags ) );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes LOAD_REGISTER_IMM commands to trigger oa report with
        ///         report reason 2.
        /// @param  buffer              target command buffer.
        /// @param  queryId             query id associated with context id field
        ///                             within a generated report.
        /// @return                     operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode TriggerQueryReport(
            CommandBuffer&                  buffer,
            [[maybe_unused]] const uint32_t queryId )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            auto risingEdge  = TT::Layouts::GpuRegisters::OaReportTrigger( false );
            auto fallingEdge = TT::Layouts::GpuRegisters::OaReportTrigger( true );

            // Write rising edge.
            ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                buffer,
                T::GpuRegisters::m_OaTrigger2,
                risingEdge.m_Value ) );

            // Write falling edge.
            ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                buffer,
                T::GpuRegisters::m_OaTrigger2,
                fallingEdge.m_Value ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes LOAD_REGISTER_IMM commands to trigger oa report with
        ///         report reason 4.
        /// @param  buffer  target command buffer.
        /// @param  marker  marker value.
        /// @param  flags   gpu command flags.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode TriggerStreamReport(
            CommandBuffer&                  buffer,
            [[maybe_unused]] const uint32_t marker,
            const Flags                     flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            auto risingEdge  = TT::Layouts::GpuRegisters::OaReportTrigger( false );
            auto fallingEdge = TT::Layouts::GpuRegisters::OaReportTrigger( true );

            // Write rising edge.
            ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                buffer,
                T::GpuRegisters::m_OaTrigger6,
                risingEdge.m_Value,
                flags ) );

            // Write falling edge.
            ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                buffer,
                T::GpuRegisters::m_OaTrigger6,
                fallingEdge.m_Value,
                flags ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes timestamp command to gpu command buffer to store
        ///         32 bit timestamp in memory.
        /// @param  buffer          target command buffer.
        /// @param  memoryAddress   memory address where timestamp should be stored.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreTimestamp(
            CommandBuffer& buffer,
            const uint64_t memoryAddress,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_TimestampLow,
                memoryAddress,
                flags ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes gpu ticks command to gpu command buffer to store
        ///         32 bit gpu ticks in memory.
        /// @param  buffer          target command buffer.
        /// @param  memoryAddress   memory address where gpu ticks should be stored.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreTicks(
            CommandBuffer& buffer,
            const uint64_t memoryAddress,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_GpuTicks,
                memoryAddress,
                flags ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes pipeline timestamp command to gpu command buffer to
        ///         store 64 bit timestamp in memory.
        /// @param  buffer          target command buffer.
        /// @param  memoryAddress   memory address where timestamp should be stored.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StorePipelineTimestamp(
            CommandBuffer&               buffer,
            const uint64_t               memoryAddress,
            [[maybe_unused]] const Flags flags )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            log.Input( "memoryAddress", memoryAddress );

            TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

            const uint32_t addressLow  = static_cast<uint32_t>( memoryAddress );
            const uint32_t addressHigh = static_cast<uint32_t>( memoryAddress >> 32 );

            command.Init();
            command.SetPostSyncOperation( T::Layouts::GpuCommands::PIPE_CONTROL::POST_SYNC_OPERATION_WRITE_TIMESTAMP );
            command.SetAddress( addressLow );
            command.SetAddressHigh( addressHigh );

            return log.m_Result = buffer.Write( command, true );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_COPY_MEM_MEM commands to gpu command buffer to copy
        ///         a given data from one gpu memory address into another.
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
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_ASSERT( ( size % sizeof( uint32_t ) ) == 0 );

            log.Input( "addressSource", addressSource );
            log.Input( "addressTarget", addressTarget );
            log.Input( "size", size );

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

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks if given flags are supported by a gpu command.
        /// @param  givenFlags      flags to check.
        /// @param  supportedFlags  supported flags.
        /// @return                 true if given flags are included in supported
        ///                         ones.
        //////////////////////////////////////////////////////////////////////////
        template <typename... SupportedFlags>
        ML_INLINE static bool CheckFlags(
            [[maybe_unused]] const Flags givenFlags,
            [[maybe_unused]] const SupportedFlags... supportedFlags )
        {
#if ML_RELEASE
            // The flags are not validated in Release.
            return true;
#else // ML_RELEASE
            if( ( givenFlags == ( Flags::EnableMmioRemap | Flags::EnablePostSync ) ) ||
                ( givenFlags == ( Flags::EnableMmioRemap | Flags::EnableStall ) ) )
            {
                // Post sync or stall operation is not supported if mmio remap is enabled.
                return false;
            }

            return static_cast<uint32_t>( ( givenFlags ^ ( supportedFlags | ... ) ) & givenFlags ) == 0;

#endif // ML_RELEASE
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct GpuCommandsTrait : BASE::GpuCommandsTrait<T>
    {
        ML_DECLARE_TRAIT( GpuCommandsTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct GpuCommandsTrait : GEN9::GpuCommandsTrait<T>
    {
        ML_DECLARE_TRAIT( GpuCommandsTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct GpuCommandsTrait : GEN11::GpuCommandsTrait<T>
    {
        ML_DECLARE_TRAIT( GpuCommandsTrait, GEN11 );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::CheckFlags;
        using Flags = typename Base::Flags;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_STORE_REGISTER_MEMORY command to gpu command buffer
        ///         with mmio remap flag to store register 32 bit data in memory.
        /// @param  buffer          target command buffer.
        /// @param  registerAddress register address.
        /// @param  memoryAddress   memory address.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreRegisterToMemory32(
            CommandBuffer& buffer,
            const uint32_t registerAddress,
            const uint64_t memoryAddress,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( CheckFlags( flags, Flags::EnableMmioRemap ) );

            log.Input( "registerAddress", registerAddress );
            log.Input( "memoryAddress", memoryAddress );
            log.Input( "flags", flags );

            TT::Layouts::GpuCommands::MI_STORE_REGISTER_MEM command = {};

            command.Init();
            command.SetUseGlobalGTT( false );
            command.SetRegisterAddress( registerAddress );
            command.SetMemoryAddress( memoryAddress );

            const bool isMmioRemapEnabled = static_cast<uint32_t>( flags & Flags::EnableMmioRemap ) != 0;

            if( isMmioRemapEnabled &&
                registerAddress >= T::GpuRegisters::m_RenderMmioRangeBegin &&
                registerAddress <= T::GpuRegisters::m_RenderMmioRangeEnd )
            {
                command.SetMMIORemapEnable( true );
            }

            return log.m_Result = buffer.Write( command, true );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_LOAD_REGISTER_IMM or PIPE_CONTROL command to gpu
        ///         command buffer to load a register with a 32 bit constant.
        /// @param  buffer          target command buffer.
        /// @param  registerAddress register address.
        /// @param  data            data to load.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode LoadRegisterImmediate32(
            CommandBuffer& buffer,
            const uint32_t registerAddress,
            const uint32_t data,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( CheckFlags( flags, Flags::EnablePostSync, Flags::EnableStall, Flags::EnableMmioRemap ) );

            log.Input( "registerAddress", registerAddress );
            log.Input( "data", data );
            log.Input( "flags", flags );

            TT::Layouts::GpuCommands::MI_LOAD_REGISTER_IMM command = {};

            command.Init();
            command.SetRegisterOffset( registerAddress );
            command.SetDataDWord( data );

            const bool isMmioRemapEnabled = static_cast<uint32_t>( flags & Flags::EnableMmioRemap ) != 0;

            if( isMmioRemapEnabled &&
                registerAddress >= T::GpuRegisters::m_RenderMmioRangeBegin &&
                registerAddress <= T::GpuRegisters::m_RenderMmioRangeEnd )
            {
                command.SetMMIORemapEnable( true );
            }

            return log.m_Result = buffer.Write( command, false );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes gpu ticks command to gpu command buffer to store
        ///         32 bit gpu ticks in memory.
        /// @param  buffer          target command buffer.
        /// @param  memoryAddress   memory address where gpu ticks should be stored.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreTicks(
            CommandBuffer& buffer,
            const uint64_t memoryAddress,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_OagGpuTicks,
                memoryAddress,
                flags ) );

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
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreHwCounters(
            CommandBuffer&                                             buffer,
            const TT::Layouts::HwCounters::Query::ReportCollectingMode collectingMode,
            const uint64_t                                             address,
            const uint32_t                                             reportId,
            const uint32_t                                             queryId,
            const bool                                                 begin,
            const Flags                                                flags = Flags::None )
        {
            switch( collectingMode )
            {
                case T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOag:
                    return T::GpuCommands::StoreHwCountersViaSrmOag(
                        buffer,
                        address,
                        reportId,
                        begin,
                        flags );

                default:
                    return Base::StoreHwCounters(
                        buffer,
                        collectingMode,
                        address,
                        reportId,
                        queryId,
                        begin,
                        flags );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes STORE_REGISTER_MEM commands to gpu command buffer to
        ///         store hw counters.
        /// @param  buffer      target command buffer.
        /// @param  address     memory offset.
        /// @param  reportId    report id.
        /// @param  begin       begin query.
        /// @param  flags       gpu command flags.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreHwCountersViaSrmOag(
            CommandBuffer& buffer,
            const uint64_t address,
            const uint32_t reportId,
            const bool     begin,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

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
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    oagCountersLow[i],
                    countersOffset + ( ( oagCountersLow[i] - T::GpuRegisters::m_OagPerfA0 ) / 2 ),
                    flags ) );
            }

            // Higher byte of oa counters.
            countersOffset = begin
                ? address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_WaBegin )
                : address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_WaEnd );

            for( size_t i = 0; i < oagCountersHigh.size(); ++i )
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    oagCountersHigh[i],
                    countersOffset + ( ( oagCountersHigh[i] - T::GpuRegisters::m_OagPerfA0Upper ) / 2 ),
                    flags ) );
            }

            // Noa counters (available only 32 bits).
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Data.m_NoaCounter[0] );

            for( size_t i = 0; i < noagCounters.size(); ++i )
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    noagCounters[i],
                    countersOffset + ( noagCounters[i] - T::GpuRegisters::m_OagPerfB0 ),
                    flags ) );
            }

            // Report id.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ReportId );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreDataToMemory32(
                buffer,
                reportId,
                countersOffset,
                flags ) );

            // Context id.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ContextId );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_ContextId,
                countersOffset,
                flags ) );

            // Gpu ticks.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_GpuTicks );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_OagGpuTicks,
                countersOffset,
                flags ) );

            // Timestamp.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_Timestamp );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_TimestampLow,
                countersOffset,
                flags ) );

            return log.m_Result;
        }
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct GpuCommandsTrait : XE_LP::GpuCommandsTrait<T>
    {
        ML_DECLARE_TRAIT( GpuCommandsTrait, XE_LP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::CheckFlags;
        using Flags = typename Base::Flags;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_STORE_REGISTER_MEMORY command to gpu command buffer
        ///         with mmio remap flag to store register 32 bit data in memory.
        /// @param  buffer          target command buffer.
        /// @param  registerAddress register address.
        /// @param  memoryAddress   memory address.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreRegisterToMemory32(
            CommandBuffer& buffer,
            const uint32_t registerAddress,
            const uint64_t memoryAddress,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( CheckFlags( flags, Flags::EnableMmioRemap, Flags::WorkloadPartition ) );

            log.Input( "registerAddress", registerAddress );
            log.Input( "memoryAddress", memoryAddress );
            log.Input( "flags", flags );

            TT::Layouts::GpuCommands::MI_STORE_REGISTER_MEM command = {};

            command.Init();
            command.SetUseGlobalGTT( false );
            command.SetRegisterAddress( registerAddress );
            command.SetMemoryAddress( memoryAddress );

            const bool isMmioRemapEnabled         = static_cast<uint32_t>( flags & Flags::EnableMmioRemap ) != 0;
            const bool isWorkloadPartitionEnabled = static_cast<uint32_t>( flags & Flags::WorkloadPartition ) != 0;

            if( isMmioRemapEnabled &&
                registerAddress >= T::GpuRegisters::m_RenderMmioRangeBegin &&
                registerAddress <= T::GpuRegisters::m_RenderMmioRangeEnd )
            {
                command.SetMMIORemapEnable( true );
            }

            if( isWorkloadPartitionEnabled )
            {
                command.SetWorkloadPartitionIDOffsetEnable( true );
            }

            return log.m_Result = buffer.Write( command, true );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_STORE_DATA_IMMEDIATE command to gpu command buffer
        ///         to store a 32 bit constant in memory.
        /// @param  buffer  target command buffer.
        /// @param  data    data to write.
        /// @param  address memory offset.
        /// @param  flags   gpu command flags.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreDataToMemory32(
            CommandBuffer& buffer,
            const uint32_t data,
            const uint64_t address,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( CheckFlags( flags, Flags::WorkloadPartition ) );

            log.Input( "data", data );
            log.Input( "address", address );
            log.Input( "flags", flags );

            TT::Layouts::GpuCommands::MI_STORE_DATA_IMM command = {};

            command.Init();
            command.SetUseGlobalGTT( false );
            command.SetStoreQword( false );
            command.SetDataDWord0( data );
            command.SetAddress( address );

            const bool isWorkloadPartitionEnabled = static_cast<uint32_t>( flags & Flags::WorkloadPartition ) != 0;

            if( isWorkloadPartitionEnabled )
            {
                command.SetWorkloadPartitionIDOffsetEnable( true );
            }

            return log.m_Result = buffer.Write( command, true );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_STORE_DATA_IMMEDIATE or PIPE_CONTROL command to gpu
        ///         command buffer to store a 64 bit constant in memory.
        /// @param  data    data to write.
        /// @param  address memory offset.
        /// @param  flags   gpu command flags.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreDataToMemory64(
            CommandBuffer& buffer,
            const uint64_t data,
            const uint64_t address,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( CheckFlags( flags, Flags::EnablePostSync, Flags::WorkloadPartition ) );

            log.Input( "data", data );
            log.Input( "address", address );
            log.Input( "flags", flags );

            const bool isPostSyncEnabled          = static_cast<uint32_t>( flags & Flags::EnablePostSync ) != 0;
            const bool isWorkloadPartitionEnabled = static_cast<uint32_t>( flags & Flags::WorkloadPartition ) != 0;

            if( isPostSyncEnabled )
            {
                const uint32_t addressLow  = static_cast<uint32_t>( address & 0xFFFFFFFF );
                const uint32_t addressHigh = static_cast<uint32_t>( address >> 32 );

                TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

                command.Init();
                command.SetPostSyncOperation( T::Layouts::GpuCommands::PIPE_CONTROL::POST_SYNC_OPERATION_WRITE_IMMEDIATE_DATA );
                command.SetAddress( addressLow );
                command.SetAddressHigh( addressHigh );
                command.SetImmediateData( data );

                if( isWorkloadPartitionEnabled )
                {
                    command.SetWorkloadPartitionIDOffsetEnable( true );
                }

                return log.m_Result = buffer.Write( command, true );
            }
            else
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

                if( isWorkloadPartitionEnabled )
                {
                    command.SetWorkloadPartitionIDOffsetEnable( true );
                }

                return log.m_Result = buffer.Write( command, true );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes pipeline timestamp command to gpu command buffer to
        ///         store 64 bit timestamp in memory.
        /// @param  buffer          target command buffer.
        /// @param  memoryAddress   memory address where timestamp should be stored.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StorePipelineTimestamp(
            CommandBuffer& buffer,
            const uint64_t memoryAddress,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( CheckFlags( flags, Flags::WorkloadPartition ) );

            log.Input( "memoryAddress", memoryAddress );
            log.Input( "flags", flags );

            TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

            const uint32_t addressLow  = static_cast<uint32_t>( memoryAddress );
            const uint32_t addressHigh = static_cast<uint32_t>( memoryAddress >> 32 );

            command.Init();
            command.SetPostSyncOperation( T::Layouts::GpuCommands::PIPE_CONTROL::POST_SYNC_OPERATION_WRITE_TIMESTAMP );
            command.SetAddress( addressLow );
            command.SetAddressHigh( addressHigh );

            const bool isWorkloadPartitionEnabled = static_cast<uint32_t>( flags & Flags::WorkloadPartition ) != 0;

            if( isWorkloadPartitionEnabled )
            {
                command.SetWorkloadPartitionIDOffsetEnable( true );
            }

            return log.m_Result = buffer.Write( command, true );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes commands to gpu command buffer to store hw counters.
        /// @param  buffer          target command buffer.
        /// @param  collectingMode  oa report collecting mode.
        /// @param  address         memory offset.
        /// @param  reportId        report id.
        /// @param  queryId         query id.
        /// @param  begin           begin query.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreHwCounters(
            CommandBuffer&                                             buffer,
            const TT::Layouts::HwCounters::Query::ReportCollectingMode collectingMode,
            const uint64_t                                             address,
            const uint32_t                                             reportId,
            const uint32_t                                             queryId,
            const bool                                                 begin,
            const Flags                                                flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( T::GpuCommands::StoreCommandStreamerIdentificator(
                buffer,
                address,
                begin,
                flags ) );

            switch( collectingMode )
            {
                case T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOag:
                    return log.m_Result = T::GpuCommands::StoreHwCountersViaSrmOag(
                               buffer,
                               address,
                               reportId,
                               begin,
                               flags );

                case T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOag:
                case T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOagExtended:
                    ML_FUNCTION_CHECK( T::GpuCommands::StoreQueryId(
                        buffer,
                        address,
                        queryId,
                        begin,
                        flags ) );
                    [[fallthrough]];

                default:
                    return log.m_Result = Base::StoreHwCounters(
                               buffer,
                               collectingMode,
                               address,
                               reportId,
                               queryId,
                               begin,
                               flags );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes STORE_REGISTER_MEM commands to gpu command buffer to
        ///         store hw counters.
        /// @param  buffer      target command buffer.
        /// @param  address     memory offset.
        /// @param  reportId    report id.
        /// @param  begin       begin query.
        /// @param  flags       gpu command flags.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreHwCountersViaSrmOag(
            CommandBuffer& buffer,
            const uint64_t address,
            const uint32_t reportId,
            const bool     begin,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            uint64_t       countersOffset  = 0;
            const auto&    oagCountersLow  = T::GpuRegisters::GetOagCountersLow();
            const auto&    oagCountersHigh = T::GpuRegisters::GetOagCountersHigh();
            const auto&    noagCounters    = T::GpuRegisters::GetNoagCounters();
            const uint32_t oaReportOffset  = begin
                 ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_Begin.m_Oa )
                 : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_End.m_Oa );

            // Lower 32 bits of oag counters.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Data.m_OaCounter_0_36[0] );

            // Store A0 - A35 counters.
            for( size_t i = 0; i < T::Layouts::HwCounters::m_OaCounter35; ++i )
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    oagCountersLow[i],
                    countersOffset + ( ( oagCountersLow[i] - T::GpuRegisters::m_OagPerfA0 ) / 2 ),
                    flags ) );
            }

            // Store A36 counter.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Data.m_OaCounter_0_36[T::Layouts::HwCounters::m_OaCounter36] );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                oagCountersLow[T::Layouts::HwCounters::m_OaCounter36],
                countersOffset,
                flags ) );

            // Store A37 counter.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Data.m_OaCounter_37 );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                oagCountersLow[T::Layouts::HwCounters::m_OaCounter37],
                countersOffset,
                flags ) );

            // Higher byte of oag counters.
            countersOffset = begin
                ? address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_WaBeginOag )
                : address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_WaEndOag );

            // Store A4 - A23 counters.
            for( size_t i = T::Layouts::HwCounters::m_OaCounter4; i < T::Layouts::HwCounters::m_OaCounter23; ++i )
            {
                // It starts from A4 counter.
                const uint32_t baseRegister = T::GpuRegisters::m_OagPerfA4Upper;

                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    oagCountersHigh[i],
                    countersOffset + ( ( oagCountersHigh[i] - baseRegister ) / 2 ),
                    flags ) );
            }

            // Store A28 - A31 counters.
            for( size_t i = T::Layouts::HwCounters::m_OaCounter28; i < T::Layouts::HwCounters::m_OaCounter31; ++i )
            {
                // It starts from A28 counter, but the offset must be moved by 20 registers,
                // because it is a continuation of storing A4 - A23 counters previously.
                const uint32_t baseRegister = T::GpuRegisters::m_OagPerfA8Upper;

                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    oagCountersHigh[i],
                    countersOffset + ( ( oagCountersHigh[i] - baseRegister ) / 2 ),
                    flags ) );
            }

            // Noag counters (available only 32 bits).
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Data.m_NoaCounter[0] );

            for( size_t i = 0; i < noagCounters.size(); ++i )
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    noagCounters[i],
                    countersOffset + ( noagCounters[i] - T::GpuRegisters::m_OagPerfB0 ),
                    flags ) );
            }

            // Report id.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ReportId );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreDataToMemory32(
                buffer,
                reportId,
                countersOffset,
                flags ) );

            // Context id.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_ContextId );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_ContextId,
                countersOffset,
                flags ) );

            // Gpu ticks.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_GpuTicks );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_OagGpuTicks,
                countersOffset,
                flags ) );

            // Timestamp.
            countersOffset = address + oaReportOffset + offsetof( TT::Layouts::HwCounters::ReportOa, m_Header.m_Timestamp );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_TimestampLow,
                countersOffset,
                flags ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes gpu commands to trigger oag report and store hw
        ///         counters.
        /// @param  buffer      target command buffer.
        /// @param  address     report memory offset.
        /// @param  reportId    report id associated with executed query.
        /// @param  queryId     query id associated with context id field within
        ///                     a generated report.
        /// @param  begin       begin/end query indicator.
        /// @param  flags       gpu command flags.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreHwCountersViaOagTriggers(
            CommandBuffer& buffer,
            const uint64_t address,
            const uint32_t reportId,
            const uint32_t queryId,
            const bool     begin,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            // StoreHwCounters via mi rpc to report gpu - to get hw context id and additionally oar/oac data (not used yet).
            // Mi rpc is needed only when oa buffer is browsed.
            if constexpr( T::Policy::QueryHwCounters::GetData::m_IncludeRenderContextSwitchReports )
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreHwCountersViaMirpc(
                    buffer,
                    address,
                    reportId,
                    begin ) );
            }

            // StoreHwCounters via trigger to oa buffer and overwrite gpu ticks in report gpu with (x)cs.tick.
            // Note: oac/oar.tick do not correlate with oag.tick.
            ML_FUNCTION_CHECK( Base::StoreHwCountersViaOagTriggers(
                buffer,
                address,
                reportId,
                queryId,
                begin,
                flags ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes gpu commands to store command streamer identificator.
        /// @param  buffer  target command buffer.
        /// @param  address report memory offset.
        /// @param  begin   begin/end query indicator.
        /// @param  flags   gpu command flags.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreCommandStreamerIdentificator(
            CommandBuffer& buffer,
            const uint64_t address,
            const bool     begin,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            if( !begin )
            {
                // Write render command streamer identificator to general purpose register without mmio remap.
                ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_IdentityRCS,
                    T::Layouts::HwCounters::m_CommandStreamerIdentificatorRender ) );

                // Write compute command streamer 0 identificator to general purpose register without mmio remap.
                ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_IdentityCCS0,
                    T::Layouts::HwCounters::m_CommandStreamerIdentificatorCompute0 ) );

                // Write compute command streamer 1 identificator to general purpose register without mmio remap.
                ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_IdentityCCS1,
                    T::Layouts::HwCounters::m_CommandStreamerIdentificatorCompute1 ) );

                // Write compute command streamer 2 identificator to general purpose register without mmio remap.
                ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_IdentityCCS2,
                    T::Layouts::HwCounters::m_CommandStreamerIdentificatorCompute2 ) );

                // Write compute command streamer 3 identificator to general purpose register without mmio remap.
                ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_IdentityCCS3,
                    T::Layouts::HwCounters::m_CommandStreamerIdentificatorCompute3 ) );

                // Store command streamer identificator to memory with mmio remap enabled.
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_GeneralPurposeRegisterRender,
                    address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_CommandStreamerIdentificator ),
                    flags | Flags::EnableMmioRemap ) );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes gpu commands to store command streamer identificator.
        /// @param  buffer  target command buffer.
        /// @param  address report memory offset.
        /// @param  queryId query id.
        /// @param  begin   begin/end query indicator.
        /// @param  flags   gpu command flags.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreQueryId(
            CommandBuffer& buffer,
            const uint64_t address,
            const uint32_t queryId,
            const bool     begin,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            const uint64_t queryIdOffset = begin
                ? address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_QueryIdBegin )
                : address + offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_QueryIdEnd );

            ML_FUNCTION_CHECK( T::GpuCommands::StoreDataToMemory32(
                buffer,
                queryId,
                queryIdOffset,
                flags ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes LOAD_REGISTER_IMM commands to trigger oag mmio report
        ///         for queries with a given query id.
        /// @param  buffer              target command buffer.
        /// @param  queryId             query id associated with context id field
        ///                             within a generated report.
        /// @return                     operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode TriggerQueryReport(
            CommandBuffer& buffer,
            const uint32_t queryId )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );

            ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                buffer,
                T::GpuRegisters::m_OagTrigger,
                queryId ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes LOAD_REGISTER_IMM command to trigger oag mmio report
        ///         with a given marker value.
        /// @param  buffer  target command buffer.
        /// @param  marker  marker value.
        /// @param  flags   gpu command flags.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode TriggerStreamReport(
            CommandBuffer& buffer,
            const uint32_t marker,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( CheckFlags( flags, Flags::EnablePostSync ) );

            const bool isPostSyncEnabled = static_cast<uint32_t>( flags & Flags::EnablePostSync ) != 0;

            if( isPostSyncEnabled )
            {
                log.Input( "marker", marker );
                log.Input( "flags", flags );

                TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

                command.Init();
                command.SetLRIPostSyncOperation( T::Layouts::GpuCommands::PIPE_CONTROL::LRI_POST_SYNC_OPERATION_MMIO_WRITE_IMMEDIATE_DATA );
                command.SetAddress( T::GpuRegisters::m_OagTrigger );
                command.SetImmediateData( marker );

                return log.m_Result = buffer.Write( command, false );
            }
            else
            {
                ML_FUNCTION_CHECK( T::GpuCommands::LoadRegisterImmediate32(
                    buffer,
                    T::GpuRegisters::m_OagTrigger,
                    marker ) );

                return log.m_Result;
            }
        }
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct GpuCommandsTrait : XE_HP::GpuCommandsTrait<T>
    {
        ML_DECLARE_TRAIT( GpuCommandsTrait, XE_HP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct GpuCommandsTrait : XE_HPG::GpuCommandsTrait<T>
    {
        ML_DECLARE_TRAIT( GpuCommandsTrait, XE_HPG );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types
        //////////////////////////////////////////////////////////////////////////
        using Base::CheckFlags;
        using Flags = typename Base::Flags;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes PIPE_CONTROL to gpu command buffer to complete
        ///         current dispatch on compute pipeline.
        /// @param  buffer  target command buffer.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode FlushCommandStreamer( CommandBuffer& buffer )
        {
            TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

            command.Init();
            command.SetCommandStreamerStallEnable( true );

            return buffer.Write( command, false );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes PIPE_CONTROL to gpu command buffer to flush gpu caches.
        /// @param  buffer  target command buffer.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode FlushCaches( CommandBuffer& buffer )
        {
            TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

            command.Init();
            command.SetDCFlushEnable( true );
            command.SetStateCacheInvalidationEnable( true );
            command.SetTextureCacheInvalidationEnable( true );
            command.SetConstantCacheInvalidationEnable( true );
            command.SetInstructionCacheInvalidateEnable( true );
            command.SetCommandStreamerStallEnable( true );

            return buffer.Write( command, false );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes STORE_REGISTER_MEM commands to gpu command buffer to
        ///         store hw counters.
        /// @param  buffer      target command buffer.
        /// @param  address     memory offset.
        /// @param  reportId    report id.
        /// @param  begin       begin query.
        /// @param  flags       gpu command flags.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreHwCountersViaSrmOar(
            [[maybe_unused]] CommandBuffer& buffer,
            [[maybe_unused]] const uint64_t address,
            [[maybe_unused]] const uint32_t reportId,
            [[maybe_unused]] const bool     begin,
            [[maybe_unused]] const Flags    flags )
        {
            ML_ASSERT_ALWAYS_ADAPTER( buffer.m_Context.m_AdapterId );
            return StatusCode::NotSupported;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes STORE_REGISTER_MEM commands to gpu command buffer to
        ///         store hw counters.
        /// @param  buffer      target command buffer.
        /// @param  address     memory offset.
        /// @param  reportId    report id.
        /// @param  begin       begin query.
        /// @param  flags       gpu command flags.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode StoreHwCountersViaSrmOag(
            [[maybe_unused]] CommandBuffer& buffer,
            [[maybe_unused]] const uint64_t address,
            [[maybe_unused]] const uint32_t reportId,
            [[maybe_unused]] const bool     begin,
            [[maybe_unused]] const Flags    flags )
        {
            ML_ASSERT_ALWAYS_ADAPTER( buffer.m_Context.m_AdapterId );
            return StatusCode::NotSupported;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_LOAD_REGISTER_IMM or PIPE_CONTROL command to gpu
        ///         command buffer to load a register with a 32 bit constant.
        /// @param  buffer          target command buffer.
        /// @param  registerAddress register address.
        /// @param  data            data to load.
        /// @param  flags           gpu command flags.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode LoadRegisterImmediate32(
            CommandBuffer& buffer,
            const uint32_t registerAddress,
            const uint32_t data,
            const Flags    flags = Flags::None )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( CheckFlags( flags, Flags::EnablePostSync, Flags::EnableStall, Flags::EnableMmioRemap ) );

            log.Input( "registerAddress", registerAddress );
            log.Input( "data", data );
            log.Input( "flags", flags );

            const bool isStallEnabled     = static_cast<uint32_t>( flags & Flags::EnableStall ) != 0;
            const bool isPostSyncEnabled  = static_cast<uint32_t>( flags & Flags::EnablePostSync ) != 0;
            const bool isMmioRemapEnabled = static_cast<uint32_t>( flags & Flags::EnableMmioRemap ) != 0;

            if( isPostSyncEnabled )
            {
                TT::Layouts::GpuCommands::PIPE_CONTROL command = {};

                command.Init();
                command.SetLRIPostSyncOperation( T::Layouts::GpuCommands::PIPE_CONTROL::LRI_POST_SYNC_OPERATION_MMIO_WRITE_IMMEDIATE_DATA );
                command.SetAddress( registerAddress );
                command.SetImmediateData( data );

                if( isStallEnabled )
                {
                    // Stall at pixel scoreboard is not available on XeHPC.
                    command.SetCommandStreamerStallEnable( true );
                }

                return log.m_Result = buffer.Write( command, false );
            }
            else
            {
                TT::Layouts::GpuCommands::MI_LOAD_REGISTER_IMM command = {};

                command.Init();
                command.SetRegisterOffset( registerAddress );
                command.SetDataDWord( data );

                if( isMmioRemapEnabled &&
                    registerAddress >= T::GpuRegisters::m_RenderMmioRangeBegin &&
                    registerAddress <= T::GpuRegisters::m_RenderMmioRangeEnd )
                {
                    command.SetMMIORemapEnable( true );
                }

                return log.m_Result = buffer.Write( command, false );
            }
        }
    };
} // namespace ML::XE_HPC
