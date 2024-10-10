/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_query_hw_counters.h

@brief Initializes hw counters. Writes hw counters commands.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for QueryHwCountersTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct QueryHwCountersTrait : DdiObject<T, TT::Queries::HwCounters, QueryHandle_1_0, ObjectType::QueryHwCounters>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( QueryHwCountersTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( QueryHwCountersTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = DdiObject<T, TT::Queries::HwCounters, QueryHandle_1_0, ObjectType::QueryHwCounters>;
        using Base::IsValid;
        using Base::FromHandle;
        using Base::Allocate;
        using Base::Delete;
        using Base::Derived;
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::Layouts::HwCounters::Query::GetDataMode m_GetDataMode;
        ConfigurationHandle_1_0                     m_UserConfiguration;
        std::vector<TT::Queries::HwCountersSlot>    m_Slots;

        //////////////////////////////////////////////////////////////////////////
        /// @brief QueryHwCountersTrait constructor.
        /// @param context a reference to context object.
        //////////////////////////////////////////////////////////////////////////
        QueryHwCountersTrait( TT::Context& context )
            : Base( context )
            , m_GetDataMode( context.m_Kernel.GetQueryHwCountersReportingMode() )
            , m_UserConfiguration{ nullptr }
            , m_Slots{}
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief QueryHwCountersTrait destructor.
        //////////////////////////////////////////////////////////////////////////
        ~QueryHwCountersTrait()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            if constexpr( T::Policy::QueryHwCounters::Create::m_UserCounters )
            {
                log.m_Result = m_UserConfiguration.IsValid()
                    ? T::Configurations::HwCountersUser::Delete( m_UserConfiguration )
                    : StatusCode::Success;

                ML_ASSERT( ML_SUCCESS( log.m_Result ) );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "QueryHwCountersTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Creates QueryHwCountersTrait instance.
        /// @param  createData  create information.
        /// @return handle      handle to hw counters instance.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode Create(
            const QueryCreateData_1_0& createData,
            QueryHandle_1_0&           handle )
        {
            auto& context = T::Context::FromHandle( createData.HandleContext );
            auto  query   = Allocate( context );
            auto  result  = query ? query->Initialize( createData.Slots ) : StatusCode::OutOfMemory;

            if( ML_SUCCESS( result ) )
            {
                handle = query->GetHandle();
            }
            else
            {
                Delete( query );
            }

            return result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes hw counters commands to command buffer if it is not
        ///         dummy, otherwise returns size of these commands.
        /// @param  buffer      command buffer.
        /// @param  gpuMemory   gpu memory allocation.
        /// @param  data        hw counters data.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode Write(
            CommandBuffer&                          buffer,
            const GpuMemory_1_0&                    gpuMemory,
            const CommandBufferQueryHwCounters_1_0& data )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( IsValid( data.Handle ) );

            auto& query = FromHandle( data.Handle );

            return log.m_Result = data.Begin
                ? query.WriteBegin( buffer, data, gpuMemory )
                : query.WriteEnd( buffer, data );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Copies hw query gpu reports into another hw query.
        ///         Useful to make a copy before reusing a given query.
        /// @param  buffer      command buffer.
        /// @param  data        hw counters copy data.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE static StatusCode CopyReports(
            CommandBuffer&                                     buffer,
            const CommandBufferQueryHwCountersCopyReports_1_0& data )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &buffer.m_Context );
            ML_FUNCTION_CHECK( IsValid( data.HandleSource ) );
            ML_FUNCTION_CHECK( IsValid( data.HandleTarget ) );

            // Obtain query objects.
            auto& querySource = FromHandle( data.HandleSource );
            auto& queryTarget = FromHandle( data.HandleTarget );

            // Validate requested slot ranges.
            ML_FUNCTION_CHECK( data.SlotSource + data.SlotCount <= querySource.m_Slots.size() );
            ML_FUNCTION_CHECK( data.SlotTarget + data.SlotCount <= queryTarget.m_Slots.size() );

            // Copy user configuration.
            queryTarget.m_UserConfiguration = querySource.m_UserConfiguration;

            // Initialize gpu memory for each target query heap slot.
            // Normally it happens during query begin. But target query
            // was never used.
            for( uint32_t i = 0; i < data.SlotCount; ++i )
            {
                const uint32_t slotIndex = i + data.SlotTarget;
                auto&          slot      = queryTarget.GetSlot( slotIndex );

                queryTarget.SetGpuMemory( slotIndex, data.AddressTarget, slot );
            }

            // Copy query and return a status.
            return log.m_Result = queryTarget.Copy(
                       buffer,
                       data.AddressSource.GpuAddress,
                       data.AddressTarget.GpuAddress,
                       data.SlotSource,
                       data.SlotTarget,
                       data.SlotCount );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns query reports.
        /// @return getData data requested by client.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode GetData( GetReportQuery_1_0& getData )
        {
            ML_FUNCTION_CHECK_STATIC( getData.Data != nullptr );
            ML_FUNCTION_CHECK_STATIC( getData.DataSize % sizeof( TT::Layouts::HwCounters::Query::ReportApi ) == 0 );
            ML_FUNCTION_CHECK_STATIC( IsValid( getData.Handle ) );

            auto& query = FromHandle( getData.Handle );
            ML_FUNCTION_LOG( StatusCode::Success, &query.m_Context );

            auto& kernel     = query.m_Context.m_Kernel;
            auto  reportsApi = static_cast<TT::Layouts::HwCounters::Query::ReportApi*>( getData.Data );

            // Check if a configuration was activated before executing query commands.
            if constexpr( T::Policy::QueryHwCounters::GetData::m_CheckConfigurationActivation )
            {
                if( kernel.m_ConfigurationManager.m_OaConfigurationReferenceCounter <= 0 )
                {
                    log.Error( "Empty configuration found" );
                    return log.m_Result = StatusCode::ReportWithEmptyConfiguration;
                }
            }

            // Skip report calculation for null query.
            if( kernel.IsNullQueryOverride() )
            {
                return log.m_Result;
            }

            // Calculate all api reports.
            for( uint32_t i = 0; i < getData.SlotsCount; ++i )
            {
                // Initialize calculator.
                TT::Queries::HwCountersCalculator calculator = { getData.Slot + i, query, reportsApi[i] };

                // Validate query calls sequence correctness.
                if( ML_FAIL( calculator.m_QuerySlot.CheckStateConsistency( T::Queries::HwCountersSlot::State::Resolved ) ) )
                {
                    log.m_Result = StatusCode::Success;
                    break;
                }

                // Calculate query reports.
                const StatusCode calculationResult = calculator.GetReportApi();

                // Validate calculation status.
                if( ML_FAIL( calculationResult ) )
                {
                    log.Warning( "Unable to obtain query api report for slot index", i, calculationResult );
                }

                // Process calculation status.
                log.m_Result = ProcessCalculationResult( log.m_Result, calculationResult );

                if( log.m_Result == StatusCode::Failed )
                {
                    break;
                }
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns query slot for a given index.
        /// @param  slotIndex   slot index.
        /// @return             reference slot.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Queries::HwCountersSlot& GetSlot( const uint32_t slotIndex )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_ASSERT( slotIndex < m_Slots.size() );

            return m_Slots[slotIndex];
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Recreates oar report from srm reports.
        /// @param  report  report gpu.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void UseSrmOarReport( TT::Layouts::HwCounters::Query::ReportGpu& report ) const
        {
            for( uint32_t i = 0; i < T::Layouts::HwCounters::m_OaCounters40bitsCount; ++i )
            {
                report.m_Begin.m_Oa.m_Data.m_OaCounterHB[i] = static_cast<uint8_t>( report.m_WaBegin[i] );
                report.m_End.m_Oa.m_Data.m_OaCounterHB[i]   = static_cast<uint8_t>( report.m_WaEnd[i] );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Recreates oa report from triggered report from oa buffer.
        /// @param  slot    query slot.
        /// @param  report  report gpu.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetTriggeredOaReports(
            TT::Queries::HwCountersSlot&               slot,
            TT::Layouts::HwCounters::Query::ReportGpu& report )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_FUNCTION_CHECK( m_Context.m_OaBuffer.IsValid() );

            log.m_Result = FindTriggeredOaReport<true>( slot, report );
            log.m_Result = ML_SUCCESS( log.m_Result ) ? FindTriggeredOaReport<false>( slot, report ) : log.m_Result;
            log.m_Result = ML_SUCCESS( log.m_Result ) ? m_Context.m_OaBuffer.DumpReports( report ) : log.m_Result;

            return log.m_Result;
        }

    protected:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes hw counters.
        /// @param  slotCount   hw counters slots to create.
        /// @return             success if hw counters has been initialized.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize( const uint32_t slotCount )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_FUNCTION_CHECK( slotCount <= T::Policy::QueryHwCounters::Create::m_MaxSlotCount );

            if constexpr( T::Policy::QueryHwCounters::Create::m_UserCounters )
            {
                ML_FUNCTION_CHECK( T::Configurations::HwCountersUser::Create( m_Context, m_UserConfiguration ) );
            }

            m_Slots.reserve( slotCount );
            for( uint32_t i = 0; i < slotCount; ++i )
            {
                m_Slots.emplace_back( m_Context );
            }

            return log.m_Result = ( static_cast<uint32_t>( m_Slots.size() ) == slotCount )
                ? StatusCode::Success
                : StatusCode::IncorrectParameter;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes begin hw counters commands to command buffer.
        /// @param  buffer      target command buffer.
        /// @param  data        hw counters data.
        /// @param  gpuMemory   gpu memory allocation.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteBegin(
            CommandBuffer&                          buffer,
            const CommandBufferQueryHwCounters_1_0& data,
            const GpuMemory_1_0&                    gpuMemory )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            if constexpr( !T::Policy::QueryHwCounters::Create::m_UserCounters )
            {
                m_UserConfiguration = data.HandleUserConfiguration;
            }

            const uint32_t slotIndex = data.Slot;
            auto&          slot      = GetSlot( slotIndex );
            auto&          derived   = Derived();

            // Override oa report collecting mode if needed.
            derived.CheckReportCollectingMode( buffer, slot );

            // Will be called during a real begin query call.
            // Not during gpu commands size calculation phase.
            if constexpr( std::is_same<CommandBuffer, TT::GpuCommandBuffer>() )
            {
                ML_FUNCTION_CHECK( slot.CheckStateConsistency( T::Queries::HwCountersSlot::State::Begun ) ); // Validate query calls sequence correctness.
                ML_FUNCTION_CHECK( SetGpuMemory( slotIndex, gpuMemory, slot ) );                             // Set gpu memory data.

                slot.m_TriggeredReportGetAttempt = 0; // Clear triggered report attempt counter.
                slot.ClearReportGpu();                // Clear gpu memory.
                ResetOaBufferState( slot );           // Reset oa buffer state.
            }

            const uint64_t gpuAddress = slot.m_GpuMemory.GpuAddress;

            ML_FUNCTION_CHECK( FlushCommandStreamer<true>( buffer ) );
            ML_FUNCTION_CHECK( WriteNopId<true>( buffer, gpuAddress ) );
            ML_FUNCTION_CHECK( WriteCoreFrequency<true>( buffer, gpuAddress ) );
            ML_FUNCTION_CHECK( derived.template WriteOaState<true>( buffer, gpuAddress, slot ) );
            ML_FUNCTION_CHECK( WriteUserCounters<true>( buffer, gpuAddress ) );
            ML_FUNCTION_CHECK( derived.template WriteGpCounters<true>( buffer, gpuAddress ) );
            ML_FUNCTION_CHECK( WriteHwCounters<true>( buffer, gpuAddress, slot ) );

            if constexpr( std::is_same<CommandBuffer, TT::GpuCommandBuffer>() )
            {
                slot.m_WorkloadBegin = buffer.GetBuffer() + buffer.m_Usage;

                slot.UpdateState( T::Queries::HwCountersSlot::State::Begun );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes end hw counters commands to command buffer.
        /// @param  buffer  target command buffer.
        /// @param  data    hw counters data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteEnd(
            CommandBuffer&                          buffer,
            const CommandBufferQueryHwCounters_1_0& data )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const auto&    derived    = Derived();
            auto&          slot       = GetSlot( data.Slot );
            const uint64_t gpuAddress = slot.m_GpuMemory.GpuAddress;

            if constexpr( T::Policy::QueryHwCounters::End::m_UseEndTagAsCompletionStatus )
            {
                slot.m_EndTag = 1;
            }
            else
            {
                slot.m_EndTag = data.EndTag;
            }

            if constexpr( std::is_same<CommandBuffer, TT::GpuCommandBuffer>() )
            {
                ML_FUNCTION_CHECK( slot.CheckStateConsistency( T::Queries::HwCountersSlot::State::Ended ) ); // Validate query calls sequence correctness.

                slot.m_WorkloadEnd = buffer.GetBuffer();
            }

            ML_FUNCTION_CHECK( FlushCommandStreamer<false>( buffer ) );
            ML_FUNCTION_CHECK( WriteHwCounters<false>( buffer, gpuAddress, slot ) );
            ML_FUNCTION_CHECK( derived.template WriteGpCounters<false>( buffer, gpuAddress ) );
            ML_FUNCTION_CHECK( WriteUserCounters<false>( buffer, gpuAddress ) );
            ML_FUNCTION_CHECK( derived.template WriteOaState<false>( buffer, gpuAddress, slot ) );
            ML_FUNCTION_CHECK( WriteNopId<false>( buffer, gpuAddress ) );
            ML_FUNCTION_CHECK( WriteCoreFrequency<false>( buffer, gpuAddress ) );
            ML_FUNCTION_CHECK( WriteUserMarker( buffer, gpuAddress, data.MarkerUser ) );
            ML_FUNCTION_CHECK( WriteDriverMarker( buffer, gpuAddress, data.MarkerDriver ) );
            ML_FUNCTION_CHECK( WriteEndTag( buffer, gpuAddress, slot ) );

            if constexpr( std::is_same<CommandBuffer, TT::GpuCommandBuffer>() )
            {
                slot.UpdateState( T::Queries::HwCountersSlot::State::Ended );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Copies gpu reports into another query.
        /// @param  buffer         command buffer.
        /// @param  source         source query.
        /// @param  addressSource  source start memory address to copy from.
        /// @param  addressTarget  target start memory address to copy into.
        /// @param  slotSource     source query slot index.
        /// @param  slotTarget     target query slot index.
        /// @param  slotCount      slot count to copy.
        /// @return                operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode Copy(
            CommandBuffer& buffer,
            const uint64_t addressSource,
            const uint64_t addressTarget,
            const uint32_t slotSource,
            const uint32_t slotTarget,
            const uint32_t slotCount ) const
        {
            constexpr uint32_t reportGpuSize = sizeof( TT::Layouts::HwCounters::Query::ReportGpu );

            return T::GpuCommands::CopyData(
                buffer,
                addressSource + ( slotSource * reportGpuSize ),
                addressTarget + ( slotTarget * reportGpuSize ),
                slotCount * reportGpuSize );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Sets gpu memory for a given query slot.
        /// @param  slotIndex   query slot index.
        /// @param  memory      gpu memory data.
        /// @return slot        modified query slot data.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode SetGpuMemory(
            const uint32_t               slotIndex,
            const GpuMemory_1_0&         memory,
            TT::Queries::HwCountersSlot& slot ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            ML_FUNCTION_CHECK( memory.CpuAddress != nullptr );
            ML_FUNCTION_CHECK( memory.GpuAddress != 0 );

            const uint64_t offset = sizeof( TT::Layouts::HwCounters::Query::ReportGpu ) * slotIndex;

            slot.m_GpuMemory = memory;

            // Offset for a given slot.
            slot.m_GpuMemory.CpuAddress  = static_cast<uint8_t*>( memory.CpuAddress ) + offset;
            slot.m_GpuMemory.GpuAddress += offset;

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_PIPE_CONTROL command to complete current draw call.
        /// @param  begin   begin/end query.
        /// @param  buffer  target command buffer.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin, typename CommandBuffer>
        ML_INLINE StatusCode FlushCommandStreamer( CommandBuffer& buffer ) const
        {
            const bool nullBegin = buffer.m_Context.m_Kernel.IsNullBeginOverride();

            return begin && nullBegin
                ? StatusCode::Success
                : T::GpuCommands::FlushCommandStreamer( buffer );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes mmio user counters commands to command buffer.
        /// @param  begin   begin/end query.
        /// @param  buffer  target command buffer.
        /// @param  offset  memory offset.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin, typename CommandBuffer>
        ML_INLINE StatusCode WriteUserCounters(
            CommandBuffer& buffer,
            const uint64_t offset ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            if( m_UserConfiguration.IsValid() )
            {
                ML_FUNCTION_CHECK( T::Configurations::HwCountersUser::IsValid( m_UserConfiguration ) );

                const auto&    userRegisters      = T::Configurations::HwCountersUser::FromHandle( m_UserConfiguration ).m_UserRegisters;
                const uint32_t userRegistersCount = userRegisters.m_RegistersCount;

                for( uint32_t i = 0; i < userRegistersCount; ++i )
                {
                    ML_FUNCTION_CHECK( WriteUserCounter<begin>(
                        buffer,
                        offset + i * sizeof( uint64_t ),
                        userRegisters.m_Registers[i].m_Offset ) );

                    if( userRegisters.m_Registers[i].m_Size > 32 )
                    {
                        ML_FUNCTION_CHECK( WriteUserCounter<begin>(
                            buffer,
                            offset + i * sizeof( uint64_t ) + sizeof( uint32_t ),
                            userRegisters.m_Registers[i].m_Offset + sizeof( uint32_t ) ) );
                    }
                }
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes mmio user counter.
        /// @param  begin           begin/end query.
        /// @param  buffer          target command buffer.
        /// @param  memoryAddress   gpu memory address.
        /// @param  registerAddress register address.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin, typename CommandBuffer>
        ML_INLINE StatusCode WriteUserCounter(
            CommandBuffer& buffer,
            const uint64_t memoryAddress,
            const uint32_t registerAddress ) const
        {
            constexpr uint32_t offset = begin
                ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_Begin.m_User )
                : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_End.m_User );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            return T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                registerAddress,
                memoryAddress + offset,
                flags );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes noop id register.
        /// @param  begin   begin/end query.
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin, typename CommandBuffer>
        ML_INLINE StatusCode WriteNopId(
            CommandBuffer& buffer,
            const uint64_t address ) const
        {
            constexpr uint32_t offset = begin
                ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_DmaFenceIdBegin )
                : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_DmaFenceIdEnd );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            return T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_NopId,
                address + offset,
                flags );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes core frequency.
        /// @param  begin   begin/end query.
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin, typename CommandBuffer>
        ML_INLINE StatusCode WriteCoreFrequency(
            CommandBuffer& buffer,
            const uint64_t address ) const
        {
            constexpr uint32_t offset = begin
                ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_CoreFrequencyBegin )
                : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_CoreFrequencyEnd );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            return T::GpuCommands::StoreRegisterToMemory32(
                buffer,
                T::GpuRegisters::m_CoreFrequency,
                address + offset,
                flags );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes general purpose counters.
        /// @param  begin   begin/end query.
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin, typename CommandBuffer>
        ML_INLINE StatusCode WriteGpCounters(
            CommandBuffer& buffer,
            const uint64_t address ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            constexpr uint32_t count  = 4;
            constexpr uint32_t offset = begin
                ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_Begin.m_Gp )
                : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_End.m_Gp );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            for( uint32_t i = 0; i < count; ++i )
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_GpCounter + ( i * sizeof( uint32_t ) ),
                    address + offset + ( i * sizeof( uint32_t ) ),
                    flags ) );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes Hw counters.
        /// @param  begin       begin/end query.
        /// @param  buffer      target command buffer.
        /// @param  address     gpu memory address.
        /// @param  slot        query slot data.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin, typename CommandBuffer>
        ML_INLINE StatusCode WriteHwCounters(
            CommandBuffer&                     buffer,
            const uint64_t                     address,
            const TT::Queries::HwCountersSlot& slot )
        {
            const auto     collectingMode = slot.m_ReportCollectingMode;
            const uint32_t queryId        = static_cast<const uint32_t>( T::Tools::GetHash( reinterpret_cast<const uintptr_t>( buffer.GetBuffer() ) ) );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            std::atomic_uint& reportId = m_Context.m_State.m_Query.m_ReportId;

            if constexpr( std::is_same<CommandBuffer, TT::GpuCommandBuffer>() )
            {
                reportId = ( reportId % 1000 ) + 1;
            }

            return T::GpuCommands::template StoreHwCounters<begin>(
                buffer,
                collectingMode,
                address,
                reportId,
                queryId,
                flags );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes oa state.
        /// @param  begin       begin/end query.
        /// @param  buffer      target command buffer.
        /// @param  address     gpu memory address.
        /// @param  slot        query slot data.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin, typename CommandBuffer>
        ML_INLINE StatusCode WriteOaState(
            CommandBuffer&                                      buffer,
            const uint64_t                                      address,
            [[maybe_unused]] const TT::Queries::HwCountersSlot& slot ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            constexpr uint32_t oaBufferOffset       = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaBuffer );
            constexpr uint32_t oaTailPreBeginOffset = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaTailPreBegin );
            constexpr uint32_t oaTailPostEndOffset  = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaTailPostEnd );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            if constexpr( begin )
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_OaTail,
                    address + oaTailPreBeginOffset,
                    flags ) );
            }
            else
            {
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_OaTail,
                    address + oaTailPostEndOffset,
                    flags ) );

                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_OaBuffer,
                    address + oaBufferOffset,
                    flags ) );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes user's marker.
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @param  marker  user's marker.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteUserMarker(
            CommandBuffer& buffer,
            const uint64_t address,
            const uint64_t marker ) const
        {
            if( marker != 0 )
            {
                constexpr uint32_t offset = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_MarkerUser );

                const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                    ? T::GpuCommands::Flags::WorkloadPartition
                    : T::GpuCommands::Flags::None;

                return T::GpuCommands::StoreDataToMemory64(
                    buffer,
                    marker,
                    address + offset,
                    flags );
            }

            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes driver's marker.
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @param  marker  driver's marker.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteDriverMarker(
            CommandBuffer& buffer,
            const uint64_t address,
            const uint64_t marker ) const
        {
            if( marker != 0 )
            {
                constexpr uint32_t offset = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_MarkerDriver );

                const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                    ? T::GpuCommands::Flags::WorkloadPartition
                    : T::GpuCommands::Flags::None;

                return T::GpuCommands::StoreDataToMemory64(
                    buffer,
                    marker,
                    address + offset,
                    flags );
            }

            return StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes query end tag.
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @param  slot    query slot data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteEndTag(
            CommandBuffer&                     buffer,
            const uint64_t                     address,
            const TT::Queries::HwCountersSlot& slot ) const
        {
            constexpr uint32_t offset = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_EndTag );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            return T::GpuCommands::StoreDataToMemory64(
                buffer,
                slot.m_EndTag,
                address + offset,
                flags );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks oa report collecting mode.
        /// @param  buffer  target command buffer.
        /// @param  slot    query slot data.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE void CheckReportCollectingMode(
            [[maybe_unused]] CommandBuffer& buffer,
            TT::Queries::HwCountersSlot&    slot )
        {
            // For gen 9 always use mirpc.
            auto& mode = slot.m_ReportCollectingMode;
            mode       = T::Layouts::HwCounters::Query::ReportCollectingMode::ReportPerformanceCounters;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Resets oa buffer state for a given pool slot.
        /// @param  slot    slot data.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void ResetOaBufferState( TT::Queries::HwCountersSlot& slot ) const
        {
            slot.m_OaBufferState.Reset();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates triggered oa report against its reason.
        /// @param  reportHeader    query report header.
        /// @return                 success if triggered oa report reason is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateReportReason( const TT::Layouts::HwCounters::ReportHeader& reportHeader ) const
        {
            return ( reportHeader.m_ReportId.m_ReportReason & static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::TriggerQuery ) ) != 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates triggered oa report against query begin/end timestamps.
        /// @param  reportQuery     query report.
        /// @param  reportTriggered triggered oa report to validate.
        /// @return                 true if triggered oa report is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateGpuTimestamps(
            const TT::Layouts::HwCounters::Query::ReportGpu& reportQuery,
            const TT::Layouts::HwCounters::ReportOa&         reportTriggered ) const
        {
            ML_FUNCTION_LOG( false, &m_Context );

            constexpr uint64_t queryThreshold      = 4000;                                                                                    // Threshold in ticks. Used to check whether obtained triggered reports were generated
            const uint32_t     queryBeginTimestamp = static_cast<uint32_t>( reportQuery.m_Begin.m_Oa.m_Header.m_Timestamp - queryThreshold ); // near the actual query begin/end execution. During tests, typical delta between ticks
            const uint32_t     queryEndTimestamp   = static_cast<uint32_t>( reportQuery.m_End.m_Oa.m_Header.m_Timestamp + queryThreshold );   // gathered around begin/end trigger reports and ticks from actual trigger reports was equal to 1 tick.
            const uint32_t     triggerTimestamp    = static_cast<uint32_t>( reportTriggered.m_Header.m_Timestamp );

            // 1st condition: normal case.
            // Timestamp should be in range <begin, end>, example:
            // ------<qb----oa--------qe>-------

            // 2nd condition: overflow case.
            // Timestamp should be smaller (a) or greater (b), for example:
            // (a) --oa--qe>--------------<qb-------
            // (b) ------qe>--------------<qb---oa--

            if( queryEndTimestamp >= queryBeginTimestamp )
            {
                const bool validBegin = queryBeginTimestamp <= triggerTimestamp;
                const bool validEnd   = queryEndTimestamp >= triggerTimestamp;

                log.m_Result = validBegin && validEnd;
            }
            else
            {
                const bool caseA = ( triggerTimestamp <= queryBeginTimestamp ) && ( triggerTimestamp <= queryEndTimestamp );
                const bool caseB = ( triggerTimestamp >= queryBeginTimestamp ) && ( triggerTimestamp >= queryEndTimestamp );

                log.m_Result = caseA || caseB;
            }

            if( !log.m_Result )
            {
                log.Warning( "Invalid triggered oa report, timestamp:", triggerTimestamp );
                log.Warning( "queryBeginTimestamp:", queryBeginTimestamp );
                log.Warning( "queryEndTimestamp:", queryEndTimestamp );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets query id from triggered report from oa buffer.
        /// @param  begin   begin/end indicator.
        /// @param  report  query report gpu.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin>
        ML_INLINE uint32_t GetQueryId( [[maybe_unused]] const TT::Layouts::HwCounters::Query::ReportGpu& report ) const
        {
            // queryId is not used before XeHP.
            return 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates triggered oa report against queryId (put in contextId field)
        /// @param  reportHeader    query report header.
        /// @param  offset          oa report offset.
        /// @param  queryIdExpected expected query id.
        /// @return                 success if triggered oa report reason is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateQueryId(
            [[maybe_unused]] const TT::Layouts::HwCounters::ReportHeader& reportHeader,
            [[maybe_unused]] const uint32_t                               offset,
            [[maybe_unused]] const uint32_t                               queryIdExpected ) const
        {
            // queryId validation is not used before XeHP.
            return true;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Recreates oa report from triggered report from oa buffer.
        /// @param  begin       begin/end indicator.
        /// @param  slot        query slot.
        /// @param  queryReport query report gpu.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin>
        ML_INLINE StatusCode FindTriggeredOaReport(
            TT::Queries::HwCountersSlot&               slot,
            TT::Layouts::HwCounters::Query::ReportGpu& queryReport )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // Query data.
            auto& queryReportOa = begin ? queryReport.m_Begin.m_Oa : queryReport.m_End.m_Oa;

            // OaBuffer data.
            const uint32_t     oaBufferSize        = m_Context.m_OaBuffer.GetSize();
            const uint32_t     reportSize          = m_Context.m_OaBuffer.GetReportSize();
            uint32_t           reportOaOffset      = 0;
            uint32_t           reportOaOffsetPost  = 0;
            bool               reportOaValid       = false;
            uint32_t           foundTriggers       = 0;
            uint32_t           foundReportOaOffset = 0;
            constexpr uint32_t expectedTriggers    = ( !begin && T::Policy::QueryHwCounters::End::m_UseDoubleTriggers )
                   ? 2
                   : 1;

            // Validate triggered oa report.
            if( ML_SUCCESS( m_Context.m_OaBuffer.template GetPreReportOffset<begin>( queryReport, reportOaOffset ) ) &&
                ML_SUCCESS( m_Context.m_OaBuffer.template GetPostReportOffset<begin>( queryReport, reportOaOffsetPost ) ) )
            {
                const auto& derived = Derived();

                // Round down offset before a triggered oa report.
                reportOaOffset -= reportOaOffset % reportSize;

                // Round up offset after a triggered oa report.
                const uint32_t remainder = reportOaOffsetPost % reportSize;
                if( remainder )
                {
                    reportOaOffsetPost = ( reportOaOffsetPost + reportSize - remainder ) % oaBufferSize;
                }

                // Number of tries to validate oa report.
                uint32_t remainingTries = 100;

                while( !reportOaValid && ( reportOaOffset != reportOaOffsetPost ) )
                {
                    auto& reportOa = m_Context.m_OaBuffer.GetReport( reportOaOffset );

                    const uint32_t queryIdExpected = derived.template GetQueryId<begin>( queryReport );

                    reportOaValid =
                        derived.ValidateQueryId( reportOa.m_Header, reportOaOffset, queryIdExpected ) &&
                        derived.ValidateReportReason( reportOa.m_Header ) &&
                        derived.ValidateGpuTimestamps( queryReport, reportOa );

                    if( reportOaValid )
                    {
                        if( ++foundTriggers == 1 )
                        {
                            // Keep the offset of the first found oa report.
                            foundReportOaOffset = reportOaOffset;
                        }

                        if( foundTriggers < expectedTriggers )
                        {
                            // Set report to not valid and advance offset if more triggers are expected.
                            reportOaValid  = false;
                            reportOaOffset = ( reportOaOffset + reportSize ) % oaBufferSize;
                        }
                    }
                    else
                    {
                        reportOaOffset = ( reportOaOffset + reportSize ) % oaBufferSize;

                        if( --remainingTries == 0 )
                        {
                            log.Critical( "Exhausted maximum number of retries" );
                            break;
                        }
                    }
                }

                // Recreate query report from triggered oa report.
                if( reportOaValid )
                {
                    auto& reportOa = m_Context.m_OaBuffer.GetReport( foundReportOaOffset );
                    log.Info(
                        "Used reportOa: ",
                        "(", FormatFlag::Decimal, FormatFlag::SetWidth5, foundReportOaOffset, ")",
                        reportOa );

                    // Copy triggered oa report into query oa report.
                    derived.CopyTriggeredOaReport( queryReportOa, reportOa );

                    // Reset attempts.
                    if constexpr( expectedTriggers > 1 )
                    {
                        slot.m_TriggeredReportGetAttempt = 0;
                    }
                }
            }

            if( !reportOaValid )
            {
                if constexpr( expectedTriggers > 1 )
                {
                    if( ++slot.m_TriggeredReportGetAttempt < T::Layouts::HwCounters::m_TriggeredReportGetAttempts )
                    {
                        log.Debug( "Triggered report is not ready yet, attempt number:", slot.m_TriggeredReportGetAttempt );
                        return log.m_Result = StatusCode::ReportNotReady;
                    }

                    slot.m_TriggeredReportGetAttempt = 0;
                }

                queryReport.m_Begin.m_Oa.m_Data = {};
                queryReport.m_End.m_Oa.m_Data   = {};
                log.Critical( "Unable to recreate report from triggered oa report" );
                log.m_Result = StatusCode::ReportLost;
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Copies triggered oa report into query oa report.
        /// @param  queryReportOa   query oa report
        /// @param  reportTriggered triggered oa report.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void CopyTriggeredOaReport(
            TT::Layouts::HwCounters::ReportOa& queryReportOa,
            TT::Layouts::HwCounters::ReportOa& reportTriggered ) const
        {
            // Copy report.
            queryReportOa = reportTriggered;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Process result returned by GetReportApi method.
        /// @param  previous previous status code.
        /// @param  next     next status code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode ProcessCalculationResult( const StatusCode previous, const StatusCode next )
        {
            switch( previous )
            {
                case StatusCode::Success:
                    // Accept any next result
                    return next;

                case StatusCode::ReportNotReady:
                    switch( next )
                    {
                        case StatusCode::Success:
                        case StatusCode::ReportNotReady:
                            return StatusCode::ReportNotReady;

                        default:
                            break;
                    }
                    [[fallthrough]];

                case StatusCode::Failed:
                default:
                    return StatusCode::Failed;
            }
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct QueryHwCountersTrait : BASE::QueryHwCountersTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersTrait, BASE );
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct QueryHwCountersTrait : GEN9::QueryHwCountersTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersTrait, GEN9 );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks oa report collecting mode.
        /// @param  buffer  target command buffer.
        /// @param  slot    query slot data.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE void CheckReportCollectingMode(
            [[maybe_unused]] CommandBuffer& buffer,
            TT::Queries::HwCountersSlot&    slot )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // Assert if posh (ptbr) is disabled and command buffer type is posh (tile).
            ML_ASSERT( m_Context.m_ClientOptions.m_PoshEnabled || buffer.m_Type != GpuCommandBufferType::Posh );
            ML_ASSERT( m_Context.m_ClientOptions.m_PtbrEnabled || buffer.m_Type != GpuCommandBufferType::Tile );

            const bool useSrm            = buffer.m_Type == GpuCommandBufferType::Posh;
            auto&      useCollectingMode = slot.m_ReportCollectingMode;

            // Use srm for a posh command streamer and store chosen collecting mode
            // in the query slot to properly calculate results.
            useCollectingMode = useSrm
                ? T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOar
                : useCollectingMode;

            log.Debug( "Oa report collecting mode", useCollectingMode );
        }
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct QueryHwCountersTrait : GEN11::QueryHwCountersTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersTrait, GEN11 );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks oa report collecting mode.
        /// @param  buffer  target command buffer.
        /// @param  slot    query slot data.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE void CheckReportCollectingMode(
            [[maybe_unused]] CommandBuffer& buffer,
            TT::Queries::HwCountersSlot&    slot )
        {
            // For XeLP+ always use triggered reports.
            auto& mode = slot.m_ReportCollectingMode;
            mode       = T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOag;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes general purpose counters.
        /// @param  begin   begin/end query.
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin, typename CommandBuffer>
        ML_INLINE StatusCode WriteGpCounters(
            [[maybe_unused]] CommandBuffer& buffer,
            [[maybe_unused]] const uint64_t address ) const
        {
            // General purpose counters are deprecated on XE+.
            return StatusCode::Success;
        }
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct QueryHwCountersTrait : XE_LP::QueryHwCountersTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersTrait, XE_LP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::Derived;
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates triggered oa report against its reason.
        /// @param  reportHeader    query report header.
        /// @return                 success if triggered oa report reason is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateReportReason( [[maybe_unused]] const TT::Layouts::HwCounters::ReportHeader& reportHeader ) const
        {
            // Accept any report reason due to report reason == 0 for mmio triggers on XeHP.
            return true;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates triggered oa report against query begin/end timestamps.
        /// @param  reportQuery     query report.
        /// @param  reportTriggered triggered oa report to validate.
        /// @return                 true if triggered oa report is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateGpuTimestamps(
            [[maybe_unused]] const TT::Layouts::HwCounters::Query::ReportGpu& reportQuery,
            [[maybe_unused]] const TT::Layouts::HwCounters::ReportOa&         reportTriggered ) const
        {
            // Do not validate gpu timestamps because query id in mmio trigger is enough unique.
            return true;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets query id from triggered report from oa buffer.
        /// @param  begin   begin/end indicator.
        /// @param  report  query report gpu.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin>
        ML_INLINE uint32_t GetQueryId( const TT::Layouts::HwCounters::Query::ReportGpu& report ) const
        {
            return begin ? report.m_QueryIdBegin : report.m_QueryIdEnd;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates triggered oa report against query id (put in contextId field)
        /// @param  reportHeader    query report header.
        /// @param  offset          oa report offset.
        /// @param  queryIdExpected expected query id.
        /// @return                 success if triggered oa report reason is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateQueryId(
            const TT::Layouts::HwCounters::ReportHeader& reportHeader,
            const uint32_t                               offset,
            const uint32_t                               queryIdExpected ) const
        {
            ML_FUNCTION_LOG( true, &m_Context );

            // Only context switch and mmio trigger reports have non-zero context id on XeHP+ platforms.
            const bool validQueryId = reportHeader.m_ContextId == queryIdExpected;

            if( !validQueryId )
            {
                log.Warning(
                    "query id",
                    "(", FormatFlag::Decimal, FormatFlag::SetWidth5, offset, ")",
                    "found =", FormatFlag::Hexadecimal, FormatFlag::ShowBase, reportHeader.m_ContextId,
                    ", expected =", FormatFlag::Hexadecimal, FormatFlag::ShowBase, queryIdExpected );
            }

            return log.m_Result = validQueryId;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Copies triggered oa report into query oa report.
        /// @param  queryReportOa   query oa report
        /// @param  reportTriggered triggered oa report.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void CopyTriggeredOaReport(
            TT::Layouts::HwCounters::ReportOa& queryReportOa,
            TT::Layouts::HwCounters::ReportOa& reportTriggered ) const
        {
            // Copy report.
            queryReportOa = reportTriggered;

            // Set dummy context id.
            queryReportOa.m_Header.m_ContextId = Constants::Query::m_DummyContextId;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Recreates oar report from srm reports.
        /// @param  report  report gpu.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void UseSrmOarReport( [[maybe_unused]] TT::Layouts::HwCounters::Query::ReportGpu& report ) const
        {
            // Not supported.
            ML_ASSERT_ALWAYS_ADAPTER( m_Context.m_AdapterId );
        }
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct QueryHwCountersTrait : XE_HP::QueryHwCountersTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersTrait, XE_HP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates triggered oa report against its reason.
        /// @param  reportHeader    query report header.
        /// @return                 success if triggered oa report reason is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateReportReason( const TT::Layouts::HwCounters::ReportHeader& reportHeader ) const
        {
            ML_FUNCTION_LOG( false, &m_Context );

            const bool validReportReason = ( reportHeader.m_ReportId.m_ReportReason & static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::MmioTrigger ) ) != 0;

            if( !validReportReason )
            {
                log.Error( "Invalid report reason", reportHeader.m_ReportId.m_ReportReason );
            }

            return log.m_Result = validReportReason;
        }
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct QueryHwCountersTrait : XE_HPG::QueryHwCountersTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct QueryHwCountersTrait : XE_HPG::QueryHwCountersTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersTrait, XE_HPG );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes oa state.
        /// @param  begin       begin/end query.
        /// @param  buffer      target command buffer.
        /// @param  address     gpu memory address.
        /// @param  slot        query slot data.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <bool begin, typename CommandBuffer>
        ML_INLINE StatusCode WriteOaState(
            CommandBuffer&                     buffer,
            const uint64_t                     address,
            const TT::Queries::HwCountersSlot& slot ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const auto mode = slot.m_ReportCollectingMode;

            // Oa buffer is only used in oag mode.
            if( mode == T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOag ||
                mode == T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOagExtended )
            {
                Base::template WriteOaState<begin>(
                    buffer,
                    address,
                    slot );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks oa report collecting mode.
        /// @param  buffer  target command buffer.
        /// @param  slot    query slot data.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE void CheckReportCollectingMode(
            [[maybe_unused]] CommandBuffer& buffer,
            TT::Queries::HwCountersSlot&    slot )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            auto& mode = slot.m_ReportCollectingMode;
            mode       = m_Context.m_Kernel.GetReportCollectingModeOverride();

            log.Debug( "Oa report collecting mode", mode );
        }
    };
} // namespace ML::XE2_HPG
