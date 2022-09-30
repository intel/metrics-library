/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2022 Intel Corporation

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
        /// @brief Base type for HwCounters slot object.
        //////////////////////////////////////////////////////////////////////////
        struct Slot
        {
            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            GpuMemory_1_0                                        m_GpuMemory;            // Gpu memory associated with query slot.
            uint32_t                                             m_ApiReportIndex;       // Current report id exposed by api.
            uint32_t                                             m_ApiReportsCount;      // Reports count exposed by api.
            uint8_t*                                             m_WorkloadBegin;        // Store a buffer pointer at the begin of query.
            uint8_t*                                             m_WorkloadEnd;          // Store a buffer pointer at the end of query.
            TT::Context&                                         m_Context;              // Context.
            TT::Layouts::OaBuffer::State                         m_OaBufferState;        // Oa buffer state captured during slot execution.
            TT::Layouts::HwCounters::Query::ReportCollectingMode m_ReportCollectingMode; // Report collecting mode (via MIRPC / SRMs / Oa triggers).

            //////////////////////////////////////////////////////////////////////////
            /// @brief Slot constructor.
            /// @param context context.
            //////////////////////////////////////////////////////////////////////////
            Slot( TT::Context& context )
                : m_GpuMemory{}
                , m_Context( context )
                , m_OaBufferState{}
                , m_ReportCollectingMode( T::Layouts::HwCounters::Query::ReportCollectingMode::ReportPerformanceCounters )
            {
                Reset();
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief Resets slot's state.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void Reset()
            {
                m_ApiReportIndex  = 1;
                m_ApiReportsCount = 1;
                m_WorkloadBegin   = nullptr;
                m_WorkloadEnd     = nullptr;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns internal hw counters gpu report for a given slot.
            /// @param  slot    layout slot.
            /// @return         reference to internal hw counters layout data.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE TT::Layouts::HwCounters::Query::ReportGpu& GetReportGpu() const
            {
                ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
                ML_ASSERT( m_GpuMemory.CpuAddress );

                return *static_cast<TT::Layouts::HwCounters::Query::ReportGpu*>( m_GpuMemory.CpuAddress );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Clears gpu memory for a given slot.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void ClearReportGpu()
            {
                if constexpr( T::Policy::QueryHwCounters::Begin::m_ClearGpuMemory )
                {
                    auto reportGpu = static_cast<TT::Layouts::HwCounters::Query::ReportGpu*>( m_GpuMemory.CpuAddress );

                    if( reportGpu != nullptr )
                    {
                        *reportGpu = {};
                    }
                }
            }
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        TT::Layouts::HwCounters::Query::GetDataMode m_GetDataMode;
        ConfigurationHandle_1_0                     m_UserConfiguration;
        std::vector<Slot>                           m_Slots;
        uint64_t                                    m_EndTag;

        //////////////////////////////////////////////////////////////////////////
        /// @brief QueryHwCountersTrait constructor.
        /// @param context a reference to context object.
        //////////////////////////////////////////////////////////////////////////
        QueryHwCountersTrait( TT::Context& context )
            : Base( context )
            , m_GetDataMode( context.m_Kernel.GetQueryHwCountersReportingMode() )
            , m_UserConfiguration{ nullptr }
            , m_Slots{}
            , m_EndTag( 0 )
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

            // Will be called during a real begin query call.
            // Not during gpu commands size calculation phase.
            if( !buffer.IsCalculateSizePhase() && data.Begin )
            {
                query.SetGpuMemory( data.Slot, gpuMemory ); // Set gpu memory data.
                query.ResetOaBufferState( data.Slot );      // Reset oa buffer state.
            }

            log.m_Result = data.Begin
                ? query.WriteBegin( buffer, data )
                : query.WriteEnd( buffer, data );

            return log.m_Result;
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
                queryTarget.SetGpuMemory( i + data.SlotTarget, data.AddressTarget );
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
                if( kernel.m_OaConfigurationReferenceCounter <= 0 )
                {
                    log.Error( "Empty configuration found" );
                    log.m_Result = StatusCode::ReportWithEmptyConfiguration;
                    return log.m_Result;
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
                const uint32_t                    slot       = getData.Slot + i;
                TT::Queries::HwCountersCalculator calculator = { slot, query, reportsApi[i] };

                // Calculate query reports.
                log.m_Result = calculator.GetReportApi();

                // Validate calculation status.
                if( ML_FAIL( log.m_Result ) )
                {
                    log.Warning( "Unable to obtain query api report", log.m_Result );
                    break;
                }
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns query slot data for a given index.
        /// @param  slot    slot index.
        /// @return         reference slot data.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE Slot& GetSlotData( const uint32_t slot )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_ASSERT( slot < m_Slots.size() );

            return m_Slots[slot];
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Recreates oar report from srm reports.
        /// @param  slot    slot index.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void UseSrmOarReport( const uint32_t slot )
        {
            auto& report = GetReportGpu( slot );

            for( uint32_t i = 0; i < T::Layouts::HwCounters::m_OaCounters40bitsCount; ++i )
            {
                report.m_Begin.m_Oa.m_Data.m_OaCounterHB[i] = static_cast<uint8_t>( report.m_WaBegin[i] );
                report.m_End.m_Oa.m_Data.m_OaCounterHB[i]   = static_cast<uint8_t>( report.m_WaEnd[i] );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Recreates oag report from srm reports.
        /// @param  slot    slot index.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void UseSrmOagReport( const uint32_t slot )
        {
            UseSrmOarReport( slot );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Recreates oa report from triggered report from oa buffer.
        /// @param  slot    slot index.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetTriggeredOaReports( const uint32_t slot )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_FUNCTION_CHECK( m_Context.m_OaBuffer.IsValid() );
            ML_FUNCTION_CHECK( m_Context.m_OaBuffer.DumpReports( GetReportGpu( slot ) ) );
            ML_FUNCTION_CHECK( FindTriggeredOaReport( slot, true ) );
            ML_FUNCTION_CHECK( FindTriggeredOaReport( slot, false ) );

            return log.m_Result;
        }

    protected:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes hw counters.
        /// @param  slots   hw counters slots to create.
        /// @return         success if hw counters has been initialized.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize( const uint32_t slots )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            if constexpr( T::Policy::QueryHwCounters::Create::m_UserCounters )
            {
                ML_FUNCTION_CHECK( T::Configurations::HwCountersUser::Create( m_Context, m_UserConfiguration ) );
            }

            m_Slots.reserve( slots );
            for( uint32_t i = 0; i < slots; ++i )
            {
                m_Slots.emplace_back( m_Context );
            }

            return log.m_Result = ( static_cast<uint32_t>( m_Slots.size() ) == slots )
                ? StatusCode::Success
                : StatusCode::IncorrectParameter;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes begin hw counters commands to command buffer.
        /// @param  buffer  target command buffer.
        /// @param  data    hw counters data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteBegin(
            CommandBuffer&                          buffer,
            const CommandBufferQueryHwCounters_1_0& data )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const uint64_t gpuAddress   = m_Slots[data.Slot].m_GpuMemory.GpuAddress;
            auto&          queryDerived = Derived();

            if constexpr( !T::Policy::QueryHwCounters::Create::m_UserCounters )
            {
                m_UserConfiguration = data.HandleUserConfiguration;
            }

            if( !buffer.IsCalculateSizePhase() )
            {
                m_Slots[data.Slot].ClearReportGpu();
            }

            // Override oa report collecting mode if needed.
            queryDerived.CheckReportCollectingMode( buffer, data.Slot );

            ML_FUNCTION_CHECK( FlushCommandStreamer( buffer, true ) );
            ML_FUNCTION_CHECK( WriteNopId( buffer, gpuAddress, true ) );
            ML_FUNCTION_CHECK( WriteCoreFrequency( buffer, gpuAddress, true ) );
            ML_FUNCTION_CHECK( queryDerived.WriteOaState( buffer, gpuAddress, data.Slot, true ) );
            ML_FUNCTION_CHECK( WriteUserCounters( buffer, gpuAddress, true ) );
            ML_FUNCTION_CHECK( queryDerived.WriteGpCounters( buffer, gpuAddress, true ) );
            ML_FUNCTION_CHECK( queryDerived.WriteHwCounters( buffer, gpuAddress, data.Slot, true ) );

            if( !buffer.IsCalculateSizePhase() )
            {
                m_Slots[data.Slot].m_WorkloadBegin = buffer.GetBuffer() + buffer.m_Usage;
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

            const uint64_t gpuAddress   = m_Slots[data.Slot].m_GpuMemory.GpuAddress;
            auto&          queryDerived = Derived();
            m_EndTag                    = data.EndTag;

            if( !buffer.IsCalculateSizePhase() )
            {
                m_Slots[data.Slot].m_WorkloadEnd = buffer.GetBuffer();
            }

            ML_FUNCTION_CHECK( FlushCommandStreamer( buffer, false ) );
            ML_FUNCTION_CHECK( queryDerived.WriteHwCounters( buffer, gpuAddress, data.Slot, false ) );
            ML_FUNCTION_CHECK( queryDerived.WriteGpCounters( buffer, gpuAddress, false ) );
            ML_FUNCTION_CHECK( WriteUserCounters( buffer, gpuAddress, false ) );
            ML_FUNCTION_CHECK( queryDerived.WriteOaState( buffer, gpuAddress, data.Slot, false ) );
            ML_FUNCTION_CHECK( WriteNopId( buffer, gpuAddress, false ) );
            ML_FUNCTION_CHECK( WriteCoreFrequency( buffer, gpuAddress, false ) );
            ML_FUNCTION_CHECK( WriteUserMarker( buffer, gpuAddress, data.MarkerUser ) );
            ML_FUNCTION_CHECK( WriteDriverMarker( buffer, gpuAddress, data.MarkerDriver ) );
            ML_FUNCTION_CHECK( WriteEndTag( buffer, gpuAddress, data.EndTag ) );

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
            const uint32_t slotCount )
        {
            const uint32_t reportGpuSize = sizeof( TT::Layouts::HwCounters::Query::ReportGpu );

            return T::GpuCommands::CopyData(
                buffer,
                addressSource + ( slotSource * reportGpuSize ),
                addressTarget + ( slotTarget * reportGpuSize ),
                slotCount * reportGpuSize );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns internal hw counters gpu report for a given slot.
        /// @param  slot    layout slot.
        /// @return         reference to internal hw counters layout data.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::Query::ReportGpu& GetReportGpu( const uint32_t slot ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_ASSERT( slot < m_Slots.size() );

            return m_Slots[slot].GetReportGpu();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns gpu timestamp collected during query begin/end.
        /// @param  slot    layout slot.
        /// @param  begin   query begin/end.
        /// @return         gpu begin/end timestamp.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE const uint32_t GetGpuTimestamp(
            const uint32_t slot,
            const bool     begin ) const
        {
            const auto& reportGpu = GetReportGpu( slot );

            return begin
                ? reportGpu.m_Begin.m_Oa.m_Header.m_Timestamp
                : reportGpu.m_End.m_Oa.m_Header.m_Timestamp;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Sets gpu memory for a given query slot.
        /// @param  slot    query slot.
        /// @param  memory  gpu memory data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode SetGpuMemory(
            const uint32_t       slot,
            const GpuMemory_1_0& memory )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            ML_FUNCTION_CHECK( memory.CpuAddress != nullptr );
            ML_FUNCTION_CHECK( memory.GpuAddress != 0 );
            ML_FUNCTION_CHECK( slot < m_Slots.size() );

            const uint64_t offset = sizeof( TT::Layouts::HwCounters::Query::ReportGpu ) * slot;

            m_Slots[slot].m_GpuMemory = memory;

            m_Slots[slot].m_GpuMemory.CpuAddress = static_cast<uint8_t*>( memory.CpuAddress ) + offset;
            m_Slots[slot].m_GpuMemory.GpuAddress += offset;

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes MI_PIPE_CONTROL command to complete current draw call.
        /// @param  buffer  target command buffer.
        /// @param  begin   begin/end query.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode FlushCommandStreamer(
            CommandBuffer& buffer,
            const bool     begin ) const
        {
            const bool nullBegin = buffer.m_Context.m_Kernel.IsNullBeginOverride();

            return begin && nullBegin
                ? StatusCode::Success
                : T::GpuCommands::FlushCommandStreamer( buffer );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes mmio user counters commands to command buffer.
        /// @param  buffer  target command buffer.
        /// @param  offset  memory offset.
        /// @param  begin   query begin.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteUserCounters(
            CommandBuffer& buffer,
            const uint64_t offset,
            const bool     begin )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            if( m_UserConfiguration.IsValid() )
            {
                ML_FUNCTION_CHECK( T::Configurations::HwCountersUser::IsValid( m_UserConfiguration ) );

                const auto&    userRegisters      = T::Configurations::HwCountersUser::FromHandle( m_UserConfiguration ).m_UserRegisters;
                const uint32_t userRegistersCount = userRegisters.m_RegistersCount;

                for( uint32_t i = 0; i < userRegistersCount; ++i )
                {
                    ML_FUNCTION_CHECK( WriteUserCounter(
                        buffer,
                        offset + i * sizeof( uint64_t ),
                        userRegisters.m_Registers[i].m_Offset,
                        begin ) );

                    if( userRegisters.m_Registers[i].m_Size > 32 )
                    {
                        ML_FUNCTION_CHECK( WriteUserCounter(
                            buffer,
                            offset + i * sizeof( uint64_t ) + sizeof( uint32_t ),
                            userRegisters.m_Registers[i].m_Offset + sizeof( uint32_t ),
                            begin ) );
                    }
                }
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes mmio user counter.
        /// @param  buffer          target command buffer.
        /// @param  memoryAddress   gpu memory address.
        /// @param  registerAddress register address.
        /// @param  begin           query begin.
        /// @return                 operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteUserCounter(
            CommandBuffer& buffer,
            const uint64_t memoryAddress,
            const uint32_t registerAddress,
            const bool     begin )
        {
            const uint32_t offset = begin
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
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @param  begin   query begin.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteNopId(
            CommandBuffer& buffer,
            const uint64_t address,
            const bool     begin )
        {
            const uint32_t offset = begin
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
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @param  begin   query begin.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteCoreFrequency(
            CommandBuffer& buffer,
            const uint64_t address,
            const bool     begin )
        {
            const uint32_t offset = begin
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
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @param  begin   query begin.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteGpCounters(
            CommandBuffer& buffer,
            const uint64_t address,
            const bool     begin )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const uint32_t count  = 4;
            const uint32_t offset = begin
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
        /// @param  buffer      target command buffer.
        /// @param  address     gpu memory address.
        /// @param  slot        query slot index.
        /// @param  begin       query begin.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteHwCounters(
            CommandBuffer& buffer,
            const uint64_t address,
            const uint32_t slot,
            const bool     begin )
        {
            const auto     collectingMode = m_Slots[slot].m_ReportCollectingMode;
            const uint32_t queryId        = static_cast<const uint32_t>( T::Tools::GetHash( reinterpret_cast<const uintptr_t>( buffer.GetBuffer() ) ) );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            std::atomic_uint& reportId = m_Context.m_State.m_Query.m_ReportId;

            if( !buffer.IsCalculateSizePhase() )
            {
                reportId = ( reportId + 1 ) % 1000;
            }

            return T::GpuCommands::StoreHwCounters(
                buffer,
                collectingMode,
                address,
                reportId,
                queryId,
                begin,
                flags );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes oa state.
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @param  slot    query slot index.
        /// @param  begin   query begin.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteOaState(
            CommandBuffer&                  buffer,
            const uint64_t                  address,
            [[maybe_unused]] const uint32_t slot,
            const bool                      begin )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            const uint32_t oaBufferOffset       = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaBuffer );
            const uint32_t oaTailPreBeginOffset = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaTailPreBegin );
            const uint32_t oaTailPostEndOffset  = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaTailPostEnd );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            if( begin )
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
            const uint64_t marker )
        {
            const uint32_t offset = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_MarkerUser );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            return T::GpuCommands::StoreDataToMemory64(
                buffer,
                marker,
                address + offset,
                flags );
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
            const uint64_t marker )
        {
            const uint32_t offset = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_MarkerDriver );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            return T::GpuCommands::StoreDataToMemory64(
                buffer,
                marker,
                address + offset,
                flags );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes query end tag.
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @param  endTag  query end tag.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteEndTag(
            CommandBuffer& buffer,
            const uint64_t address,
            const uint64_t endTag )
        {
            const uint32_t offset = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_EndTag );

            const auto flags = m_Context.m_ClientOptions.m_WorkloadPartitionEnabled
                ? T::GpuCommands::Flags::WorkloadPartition
                : T::GpuCommands::Flags::None;

            return T::GpuCommands::StoreDataToMemory64(
                buffer,
                endTag,
                address + offset,
                flags );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks oa report collecting mode.
        /// @param  buffer    target command buffer.
        /// @param  slot      query slot index.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE void CheckReportCollectingMode(
            [[maybe_unused]] CommandBuffer& buffer,
            const uint32_t                  slot )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // For gen 9 always use mirpc.
            auto& mode = m_Slots[slot].m_ReportCollectingMode;
            mode       = T::Layouts::HwCounters::Query::ReportCollectingMode::ReportPerformanceCounters;

            log.Debug( "Oa report collecting mode", mode );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Resets oa buffer state for a given pool slot.
        /// @param  slot    slot index.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void ResetOaBufferState( const uint32_t slot )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );
            ML_ASSERT( slot < m_Slots.size() );

            m_Slots[slot].m_OaBufferState.Reset();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates triggered oa report against its reason.
        /// @param  reportHeader    query report header.
        /// @return                 success if triggered oa report reason is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateReportReason( const TT::Layouts::HwCounters::ReportHeader& reportHeader ) const
        {
            return reportHeader.m_ReportId.m_ReportReason == static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::TriggerQuery );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates triggered oa report against query begin/end ticks.
        /// @param  reportQuery     query report.
        /// @param  reportTriggered triggered oa report to validate.
        /// @return                 true if triggered oa report is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateGpuTicks(
            const TT::Layouts::HwCounters::Query::ReportGpu& reportQuery,
            const TT::Layouts::HwCounters::ReportOa&         reportTriggered )
        {
            ML_FUNCTION_LOG( false, &m_Context );

            const uint32_t queryThreshold  = 4000;                                                                                   // Threshold in ticks. Used to check whether obtained triggered reports were generated
            const uint32_t queryBeginTicks = static_cast<uint32_t>( reportQuery.m_Begin.m_Oa.m_Header.m_GpuTicks ) - queryThreshold; // near the actual query begin/end execution. During tests, typical delta between ticks
            const uint32_t queryEndTicks   = static_cast<uint32_t>( reportQuery.m_End.m_Oa.m_Header.m_GpuTicks ) + queryThreshold;   // gathered around begin/end trigger reports and ticks from actual trigger reports was equal to 1 tick.
            const uint32_t triggerTicks    = static_cast<uint32_t>( reportTriggered.m_Header.m_GpuTicks );

            // 1st condition: normal case.
            // Ticks should be in range <begin, end>, example:
            // ------<qb----oa--------qe>-------

            // 2nd condition: overflow case.
            // Ticks should be smaller (a) or greater (b), for example:
            // (a) --oa--qe>--------------<qb-------
            // (b) ------qe>--------------<qb---oa--

            if( queryEndTicks >= queryBeginTicks )
            {
                const bool validBegin = queryBeginTicks <= triggerTicks;
                const bool validEnd   = queryEndTicks >= triggerTicks;

                log.m_Result = validBegin && validEnd;
            }
            else
            {
                const bool caseA = ( triggerTicks <= queryBeginTicks ) && ( triggerTicks <= queryEndTicks );
                const bool caseB = ( triggerTicks >= queryBeginTicks ) && ( triggerTicks >= queryEndTicks );

                log.m_Result = caseA || caseB;
            }

            if( !log.m_Result )
            {
                log.Warning( "Invalid triggered oa report, tic:", triggerTicks );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets query id from triggered report from oa buffer.
        /// @param  slot        slot index.
        /// @param  begin       begin/end indicator.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t GetQueryId(
            [[maybe_unused]] const uint32_t slot,
            [[maybe_unused]] const bool     begin ) const
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
        /// @param  slot        slot index.
        /// @param  begin       begin/end indicator.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode FindTriggeredOaReport(
            const uint32_t slot,
            const bool     begin )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // Query data.
            auto& queryReport   = GetReportGpu( slot );
            auto& queryReportOa = begin ? queryReport.m_Begin.m_Oa : queryReport.m_End.m_Oa;

            // OaBuffer data.
            uint32_t oaBufferSize       = m_Context.m_OaBuffer.GetSize();
            uint32_t reportOaOffset     = 0;
            uint32_t reportOaOffsetPost = 0;
            bool     reportOaValid      = false;

            // Validate triggered oa report.
            if( ML_SUCCESS( m_Context.m_OaBuffer.GetPreReportOffset( queryReport, begin, reportOaOffset ) ) &&
                ML_SUCCESS( m_Context.m_OaBuffer.GetPostReportOffset( queryReport, begin, reportOaOffsetPost ) ) )
            {
                auto& derived = Derived();

                while( !reportOaValid && ( reportOaOffset != reportOaOffsetPost ) )
                {
                    auto& reportOa = m_Context.m_OaBuffer.GetReport( reportOaOffset );

                    const uint32_t queryIdExpected = derived.GetQueryId( slot, begin );
                    reportOaValid                  = derived.ValidateQueryId( reportOa.m_Header, reportOaOffset, queryIdExpected );

                    if( !reportOaValid )
                    {
                        reportOaOffset = ( reportOaOffset < oaBufferSize ) ? reportOaOffset + sizeof( TT::Layouts::HwCounters::ReportOa ) : 0;
                    }
                }

                // Recreate query report from triggered oa report.
                if( reportOaValid )
                {
                    auto& reportOa = m_Context.m_OaBuffer.GetReport( reportOaOffset );
                    log.Info(
                        "Used reportOa: ",
                        "(",
                        FormatFlag::Decimal,
                        FormatFlag::SetWidth5,
                        reportOaOffset,
                        ")",
                        reportOa );

                    ML_ASSERT( derived.ValidateReportReason( reportOa.m_Header ) );
                    ML_ASSERT( ValidateGpuTicks( queryReport, reportOa ) );

                    // Copy triggered oa report into query oa report.
                    derived.CopyTriggeredOaReport( queryReportOa, reportOa );
                }
            }

            if( !reportOaValid )
            {
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
            TT::Layouts::HwCounters::ReportOa& reportTriggered )
        {
            // Copy report.
            queryReportOa = reportTriggered;
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
        using Base::m_Slots;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks oa report collecting mode.
        /// @param  buffer    target command buffer.
        /// @param  slot      query slot index.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE void CheckReportCollectingMode(
            [[maybe_unused]] CommandBuffer& buffer,
            const uint32_t                  slot )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // Assert if posh (ptbr) is disabled and command buffer type is posh (tile).
            ML_ASSERT( m_Context.m_ClientOptions.m_PoshEnabled || buffer.m_Type != GpuCommandBufferType::Posh );
            ML_ASSERT( m_Context.m_ClientOptions.m_PtbrEnabled || buffer.m_Type != GpuCommandBufferType::Tile );

            const bool useSrm            = buffer.m_Type == GpuCommandBufferType::Posh;
            auto&      useCollectingMode = m_Slots[slot].m_ReportCollectingMode;

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
        using Base::m_Slots;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Checks oa report collecting mode.
        /// @param  buffer    target command buffer.
        /// @param  slot      query slot index.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE void CheckReportCollectingMode(
            [[maybe_unused]] CommandBuffer& buffer,
            const uint32_t                  slot )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Context );

            // For XeLP+ always use triggered reports.
            auto& mode = m_Slots[slot].m_ReportCollectingMode;
            mode       = T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOag;

            log.Debug( "Oa report collecting mode", mode );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Writes general purpose counters.
        /// @param  buffer  target command buffer.
        /// @param  address gpu memory address.
        /// @param  begin   query begin.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename CommandBuffer>
        ML_INLINE StatusCode WriteGpCounters(
            [[maybe_unused]] CommandBuffer& buffer,
            [[maybe_unused]] const uint64_t address,
            [[maybe_unused]] const bool     begin )
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
        using Base::GetReportGpu;
        using Base::m_Context;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates triggered oa report against its reason.
        /// @param  reportHeader    query report header.
        /// @return                 success if triggered oa report reason is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE bool ValidateReportReason( const TT::Layouts::HwCounters::ReportHeader& reportHeader ) const
        {
            ML_FUNCTION_LOG( true, &m_Context );

            const bool validReportReason = reportHeader.m_ReportId.m_ReportReason == static_cast<uint32_t>( T::Layouts::OaBuffer::ReportReason::MmioTrigger );

            if( !validReportReason )
            {
                log.Error( "report reason", reportHeader.m_ReportId.m_ReportReason );
            }

            return log.m_Result = ( validReportReason );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets query id from triggered report from oa buffer.
        /// @param  slot        slot index.
        /// @param  begin       begin/end indicator.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t GetQueryId(
            const uint32_t slot,
            const bool     begin ) const
        {
            auto& report = GetReportGpu( slot );
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
                    "(",
                    FormatFlag::Decimal,
                    FormatFlag::SetWidth5,
                    offset,
                    ")",
                    "found =",
                    FormatFlag::Hexadecimal,
                    FormatFlag::ShowBase,
                    reportHeader.m_ContextId,
                    ", expected =",
                    FormatFlag::Hexadecimal,
                    FormatFlag::ShowBase,
                    queryIdExpected );
            }

            return log.m_Result = validQueryId;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Recreates oag report from srm reports.
        /// @param  slot    slot index.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void UseSrmOagReport( const uint32_t slot )
        {
            auto& report = GetReportGpu( slot );

            const uint32_t oagCounters40bitsOffset = T::Layouts::HwCounters::m_OagCounters40bitsCount - 4;

            // High bytes of A4 - A23 counters.
            for( uint32_t i = 0; i < oagCounters40bitsOffset; ++i )
            {
                report.m_Begin.m_Oa.m_Data.m_OaCounterHB_4_23[i] = static_cast<uint8_t>( report.m_WaBeginOag[i] );
                report.m_End.m_Oa.m_Data.m_OaCounterHB_4_23[i]   = static_cast<uint8_t>( report.m_WaEndOag[i] );
            }

            // High bytes of A28 - A31 counters.
            for( uint32_t i = 0; i < 4; ++i )
            {
                report.m_Begin.m_Oa.m_Data.m_OaCounterHB_28_31[i] = static_cast<uint8_t>( report.m_WaBeginOag[i + oagCounters40bitsOffset] );
                report.m_End.m_Oa.m_Data.m_OaCounterHB_28_31[i]   = static_cast<uint8_t>( report.m_WaEndOag[i + oagCounters40bitsOffset] );
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Copies triggered oa report into query oa report.
        /// @param  queryReportOa   query oa report
        /// @param  reportTriggered triggered oa report.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void CopyTriggeredOaReport(
            TT::Layouts::HwCounters::ReportOa& queryReportOa,
            TT::Layouts::HwCounters::ReportOa& reportTriggered )
        {
            ML_FUNCTION_LOG( true, &m_Context );

            // Store real context id.
            auto contextId = queryReportOa.m_Header.m_ContextId;
            // Copy report (with queryId instead of contextId).
            queryReportOa = reportTriggered;
            // Restore real context id.
            queryReportOa.m_Header.m_ContextId = contextId;

            if constexpr( T::Policy::QueryHwCounters::GetData::m_RecoverContextId )
            {
                log.Info( "trigger query id ", reportTriggered.m_Header.m_ContextId );
                log.Info( "query context id   ", contextId );

                // (intentionally disabled - only useful for debugging)
                // reportTriggered.m_Header.m_ContextId = contextId;
            }
        }
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct QueryHwCountersTrait : XE_HP::QueryHwCountersTrait<T>
    {
        ML_DECLARE_TRAIT( QueryHwCountersTrait, XE_HP );
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
