/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_query_hw_counters.h

@brief Initializes hw counters. Writes hw counters commands.
*/

#pragma once

namespace ML
{
    namespace BASE
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
                TT::Layouts::OaBuffer::State                         m_OaBufferState;        // Oa buffer state captured during slot execution.
                TT::Layouts::HwCounters::Query::ReportCollectingMode m_ReportCollectingMode; // Report collecting mode (via MIRPC / SRMs / Oa triggers).

                //////////////////////////////////////////////////////////////////////////
                /// @brief Slot constructor.
                //////////////////////////////////////////////////////////////////////////
                Slot()
                    : m_GpuMemory{}
                    , m_OaBufferState()
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
                }

                //////////////////////////////////////////////////////////////////////////
                /// @brief  Returns internal hw counters gpu report for a given slot.
                /// @param  slot    layout slot.
                /// @return         reference to internal hw counters layout data.
                //////////////////////////////////////////////////////////////////////////
                ML_INLINE TT::Layouts::HwCounters::Query::ReportGpu& GetReportGpu() const
                {
                    ML_ASSERT( m_GpuMemory.CpuAddress );

                    return *static_cast<TT::Layouts::HwCounters::Query::ReportGpu*>( m_GpuMemory.CpuAddress );
                }
            };

            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            TT::Context&                                m_Context;
            TT::Layouts::HwCounters::Query::GetDataMode m_GetDataMode;
            ConfigurationHandle_1_0                     m_UserConfiguration;
            std::vector<Slot>                           m_Slots;
            uint64_t                                    m_EndTag;

            //////////////////////////////////////////////////////////////////////////
            /// @brief QueryHwCountersTrait constructor.
            /// @param context a reference to context object.
            //////////////////////////////////////////////////////////////////////////
            QueryHwCountersTrait( TT::Context& context )
                : Base( context.m_ClientType )
                , m_Context( context )
                , m_GetDataMode( context.m_Kernel.GetQueryHwCountersReportingMode() )
                , m_UserConfiguration{ nullptr }
                , m_Slots()
                , m_EndTag( 0 )
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief QueryHwCountersTrait destructor.
            //////////////////////////////////////////////////////////////////////////
            ~QueryHwCountersTrait()
            {
                if( T::Policy::QueryHwCounters::Create::m_UserCounters )
                {
                    const StatusCode result = m_UserConfiguration.IsValid()
                        ? T::Configurations::HwCountersUser::Delete( m_UserConfiguration )
                        : StatusCode::Success;

                    (void) result;
                    ML_ASSERT( ML_SUCCESS( result ) );
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
                ML_FUNCTION_LOG( StatusCode::Success );
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
                ML_FUNCTION_LOG( StatusCode::Success );
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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( getData.Data != nullptr );
                ML_FUNCTION_CHECK( getData.DataSize % sizeof( TT::Layouts::HwCounters::Query::ReportApi ) == 0 );
                ML_FUNCTION_CHECK( IsValid( getData.Handle ) );

                auto& query      = FromHandle( getData.Handle );
                auto& kernel     = query.m_Context.m_Kernel;
                auto  reportsApi = static_cast<TT::Layouts::HwCounters::Query::ReportApi*>( getData.Data );

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
            ML_INLINE StatusCode UseTriggeredOaReport( const uint32_t slot )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( m_Context.m_OaBuffer.IsValid() );

                uint32_t beginTailIndex = 0;
                uint32_t endTailIndex   = 0;

                ML_FUNCTION_CHECK( UseTriggeredOaReport( slot, true, beginTailIndex ) );
                ML_FUNCTION_CHECK( UseTriggeredOaReport( slot, false, endTailIndex ) );
                ML_FUNCTION_CHECK( m_Context.m_OaBuffer.DumpReports( beginTailIndex, endTailIndex ) );

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
                ML_FUNCTION_LOG( StatusCode::Success );

                if( T::Policy::QueryHwCounters::Create::m_UserCounters )
                {
                    ML_FUNCTION_CHECK( T::Configurations::HwCountersUser::Create( m_Context, m_UserConfiguration ) );
                }

                m_Slots.resize( slots );

                return log.m_Result = m_Slots.size()
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
                ML_FUNCTION_LOG( StatusCode::Success );

                if( !T::Policy::QueryHwCounters::Create::m_UserCounters )
                {
                    m_UserConfiguration = data.HandleUserConfiguration;
                }

                const uint64_t gpuAddress = m_Slots[data.Slot].m_GpuMemory.GpuAddress;

                ML_FUNCTION_CHECK( FlushCommandStreamer( buffer, true ) );
                ML_FUNCTION_CHECK( WriteNopId( buffer, gpuAddress, true ) );
                ML_FUNCTION_CHECK( WriteCoreFrequency( buffer, gpuAddress, true ) );
                ML_FUNCTION_CHECK( WriteOaState( buffer, gpuAddress, true ) );
                ML_FUNCTION_CHECK( WriteUserCounters( buffer, gpuAddress, true ) );
                ML_FUNCTION_CHECK( WriteGpCounters( buffer, gpuAddress, true ) );
                ML_FUNCTION_CHECK( Derived().WriteHwCounters( buffer, gpuAddress, data.Slot, true ) );

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
                ML_FUNCTION_LOG( StatusCode::Success );

                m_EndTag = data.EndTag;

                const uint64_t gpuAddress = m_Slots[data.Slot].m_GpuMemory.GpuAddress;

                ML_FUNCTION_CHECK( FlushCommandStreamer( buffer, false ) );
                ML_FUNCTION_CHECK( Derived().WriteHwCounters( buffer, gpuAddress, data.Slot, false ) );
                ML_FUNCTION_CHECK( WriteGpCounters( buffer, gpuAddress, false ) );
                ML_FUNCTION_CHECK( WriteUserCounters( buffer, gpuAddress, false ) );
                ML_FUNCTION_CHECK( WriteOaState( buffer, gpuAddress, false ) );
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
            /// @param  addressTarget  target start memory adderess to copy into.
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
                ML_FUNCTION_LOG( StatusCode::Success );

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
                ML_FUNCTION_LOG( StatusCode::Success );

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

                return T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    registerAddress,
                    memoryAddress + offset );
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

                return T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_NopId,
                    address + offset );
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

                return T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_CoreFrequency,
                    address + offset );
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
                ML_FUNCTION_LOG( StatusCode::Success );

                const uint32_t count  = 4;
                const uint32_t offset = begin
                    ? offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_Begin.m_Gp )
                    : offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_End.m_Gp );

                for( uint32_t i = 0; i < count; ++i )
                {
                    ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                        buffer,
                        T::GpuRegisters::m_GpCounter + ( i * sizeof( uint32_t ) ),
                        address + offset + ( i * sizeof( uint32_t ) ) ) );
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
                static std::atomic_uint reportId( 0 );
                const auto&             collectingMode = m_Slots[slot].m_ReportCollectingMode;
                const uint32_t          queryId        = static_cast<const uint32_t>( T::Tools::GetHash( reinterpret_cast<const uintptr_t>( this ) ) );

                return T::GpuCommands::StoreHwCounters(
                    buffer,
                    collectingMode,
                    address,
                    ( ++reportId ) % 0x1000,
                    queryId,
                    begin );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes oa state.
            /// @param  buffer  target command buffer.
            /// @param  address gpu memory address.
            /// @param  begin   query begin.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE StatusCode WriteOaState(
                CommandBuffer& buffer,
                const uint64_t address,
                const bool     begin )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const uint32_t oaBufferOffset    = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaBuffer );
                const uint32_t oaTailBeginOffset = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaTailBegin );
                const uint32_t oaTailEndOffset   = offsetof( TT::Layouts::HwCounters::Query::ReportGpu, m_OaTailEnd );

                if( begin )
                {
                    ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                        buffer,
                        T::GpuRegisters::m_OaTail,
                        address + oaTailBeginOffset ) );
                }
                else
                {
                    ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                        buffer,
                        T::GpuRegisters::m_OaTail,
                        address + oaTailEndOffset ) );

                    ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                        buffer,
                        T::GpuRegisters::m_OaBuffer,
                        address + oaBufferOffset ) );
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

                return T::GpuCommands::StoreDataToMemory64(
                    buffer,
                    marker,
                    address + offset );
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

                return T::GpuCommands::StoreDataToMemory64(
                    buffer,
                    marker,
                    address + offset );
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

                return T::GpuCommands::StoreDataToMemory64(
                    buffer,
                    endTag,
                    address + offset );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Resets oa buffer state for a given pool slot.
            /// @param  slot    slot index.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void ResetOaBufferState( const uint32_t slot )
            {
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
            ML_INLINE bool ValidateTriggeredOaReport(
                const TT::Layouts::HwCounters::Query::ReportGpu& reportQuery,
                const TT::Layouts::HwCounters::ReportOa&         reportTriggered )
            {
                ML_FUNCTION_LOG( false );
                ML_FUNCTION_CHECK_ERROR( Derived().ValidateReportReason( reportTriggered.m_Header ), false );

                const uint32_t queryThreshold  = 4000;                                                          // Threshold in ticks. Used to check whether obtained triggered reports were generated
                const uint32_t queryBeginTicks = reportQuery.m_Begin.m_Oa.m_Header.m_GpuTicks - queryThreshold; // near the actual query begin/end execution. During tests, typical delta between ticks
                const uint32_t queryEndTicks   = reportQuery.m_End.m_Oa.m_Header.m_GpuTicks + queryThreshold;   // gathered around begin/end trigger reports and ticks from actual trigger reports was equal to 1 tick.
                const uint32_t triggerTicks    = reportTriggered.m_Header.m_GpuTicks;

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

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Recreates oa report from triggered report from oa buffer.
            /// @param  slot        slot index.
            /// @param  begin       begin/end indicator.
            /// @return tailIndex   triggered report index.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode UseTriggeredOaReport(
                const uint32_t slot,
                const bool     begin,
                uint32_t&      tailIndex )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                // Query data.
                auto& queryReport   = GetReportGpu( slot );
                auto& queryReportOa = begin ? queryReport.m_Begin.m_Oa : queryReport.m_End.m_Oa;

                // Tail data.
                bool  tailIndexValid = ML_SUCCESS( m_Context.m_OaBuffer.GetTriggeredReportIndex( queryReport, begin, tailIndex ) );
                auto  dummyReportOa  = TT::Layouts::HwCounters::ReportOa{};
                auto& tailReportOa   = tailIndexValid ? m_Context.m_OaBuffer.GetReport( tailIndex ) : dummyReportOa;
                bool  tailValid      = tailIndexValid && ValidateTriggeredOaReport( queryReport, tailReportOa );

                // Set the contextId.
                Derived().SetContextId( queryReportOa.m_Header.m_ContextId, tailReportOa );

                // Triggered report debug information.
                log.Debug( "Query report     " );
                log.Debug( "  begin          ", begin );
                log.Debug( "  ticks begin    ", queryReport.m_Begin.m_Oa.m_Header.m_GpuTicks );
                log.Debug( "  ticks end      ", queryReport.m_End.m_Oa.m_Header.m_GpuTicks );
                log.Debug( "  contextId      ", queryReportOa.m_Header.m_ContextId );
                log.Debug( "Tail report:     " );
                log.Debug( "  index          ", tailIndex );
                log.Debug( "  tailIndexValid ", tailIndexValid );
                log.Debug( "  valid          ", tailValid );
                log.Debug( "  tailReportOa   ", tailReportOa );

                // Recreate query report from triggered oa report or make it empty.
                if( tailValid )
                {
                    queryReportOa = tailReportOa;
                }
                else
                {
                    queryReport.m_Begin.m_Oa.m_Data = {};
                    queryReport.m_End.m_Oa.m_Data   = {};
                    log.Critical( "Unable to recreate report from triggered oa report" );
                    log.m_Result = StatusCode::ReportLost;
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Copies context id from cache into triggered report.
            /// @param  contextId       hw context id from the cache.
            /// @return reportTriggered triggered oa report.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE void SetContextId(
                const uint32_t /*contextId*/,
                TT::Layouts::HwCounters::ReportOa& /*reportTriggered*/)
            {
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct QueryHwCountersTrait : BASE::QueryHwCountersTrait<T>
        {
            ML_DECLARE_TRAIT( QueryHwCountersTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
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
                CommandBuffer& buffer,
                const uint32_t slot )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const bool useSrm            = buffer.m_Type == GpuCommandBufferType::Posh;
                auto&      useCollectingMode = m_Slots[slot].m_ReportCollectingMode;

                // Use srm for a posh command streamer and store chosen collecting mode
                // in the query slot to properly calculate results.
                useCollectingMode = useSrm
                    ? T::Layouts::HwCounters::Query::ReportCollectingMode::StoreRegisterMemoryOar
                    : useCollectingMode;

                log.Debug( "Oa report collecting mode", useCollectingMode );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes hw counters.
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
                // Assert if posh (ptbr) is disabled and command buffer type is posh (tile).
                ML_ASSERT( m_Context.m_ClientOptions.m_PoshEnabled || buffer.m_Type != GpuCommandBufferType::Posh );
                ML_ASSERT( m_Context.m_ClientOptions.m_PtbrEnabled || buffer.m_Type != GpuCommandBufferType::Tile );

                // Override oa report collecting mode if needed.
                CheckReportCollectingMode( buffer, slot );

                // Collect report.
                return Base::WriteHwCounters(
                    buffer,
                    address,
                    slot,
                    begin );
            }
        };
    } // namespace GEN11

    namespace GEN12
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
                CommandBuffer& /*buffer*/,
                const uint32_t slot )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_ASSERT( T::Policy::QueryHwCounters::Common::UseTriggeredOaReport( m_Context.m_Kernel ) );

                // For gen 12+ always use triggered reports.
                auto& mode = m_Slots[slot].m_ReportCollectingMode;
                mode       = T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOag;

                log.Debug( "Oa report collecting mode", mode );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes hw counters.
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
                // Assert if posh (ptbr) is disabled and command buffer type is posh (tile).
                ML_ASSERT( m_Context.m_ClientOptions.m_PoshEnabled || buffer.m_Type != GpuCommandBufferType::Posh );
                ML_ASSERT( m_Context.m_ClientOptions.m_PtbrEnabled || buffer.m_Type != GpuCommandBufferType::Tile );

                // Override oa report collecting mode if needed.
                CheckReportCollectingMode( buffer, slot );

                // Collect report.
                return Base::WriteHwCounters(
                    buffer,
                    address,
                    slot,
                    begin );
            }
        };
    } // namespace GEN12
} // namespace ML
