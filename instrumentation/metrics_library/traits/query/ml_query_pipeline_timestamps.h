/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_query_pipeline_timestamps.h

@brief Initializes query pipeline timestamps.
*/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for QueryPipelineTimestampsTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct QueryPipelineTimestampsTrait : DdiObject<T, TT::Queries::PipelineTimestamps, QueryHandle_1_0, ObjectType::QueryPipelineTimestamps>
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( QueryPipelineTimestampsTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( QueryPipelineTimestampsTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Types.
            //////////////////////////////////////////////////////////////////////////
            using Base = DdiObject<T, TT::Queries::PipelineTimestamps, QueryHandle_1_0, ObjectType::QueryPipelineTimestamps>;
            using Base::IsValid;
            using Base::FromHandle;
            using Base::Allocate;
            using Base::Delete;
            using Base::Derived;

            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            TT::Context&                                m_Context;
            TT::Layouts::PipelineTimestamps::ReportGpu* m_GpuReport;
            GpuMemory_1_0                               m_GpuMemory;
            uint64_t                                    m_EndTag;
            uint32_t                                    m_ReportIndex;

            //////////////////////////////////////////////////////////////////////////
            /// @brief QueryPipelineTimestampsTrait constructor.
            /// @param context a reference to library context object.
            //////////////////////////////////////////////////////////////////////////
            QueryPipelineTimestampsTrait( TT::Context& context )
                : Base( context.m_ClientType )
                , m_Context( context )
                , m_GpuReport( nullptr )
                , m_GpuMemory{}
                , m_EndTag( 0 )
                , m_ReportIndex( 1 )
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "QueryPipelineTimestampsTrait<Traits>";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Creates QueryPipelineTimestampsTrait instance.
            /// @param  createData   create information.
            /// @return handle       handle query instance.
            /// @return              operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static StatusCode Create(
                const QueryCreateData_1_0& createData,
                QueryHandle_1_0&           handle )
            {
                auto& context = T::Context::FromHandle( createData.HandleContext );
                auto  query   = Allocate( context );
                auto  result  = query ? StatusCode::Success : StatusCode::OutOfMemory;

                if( ML_SUCCESS( result ) )
                {
                    handle = query->GetHandle();
                }

                return result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes query commands to command buffer if it is not
            ///         dummy, otherwise returns size of these commands.
            /// @param  buffer      command buffer.
            /// @param  gpuMemory   gpu memory allocation.
            /// @param  data        query data.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode Write(
                CommandBuffer&               buffer,
                const GpuMemory_1_0&         gpuMemory,
                const CommandBufferData_1_0& data )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( IsValid( data.QueryPipelineTimestamps.Handle ) );

                auto&          queryData = data.QueryPipelineTimestamps;
                auto&          query     = FromHandle( queryData.Handle );
                const uint32_t offset    = offsetof( TT::Layouts::PipelineTimestamps::ReportGpu, m_TimestampsRender );

                if( queryData.Begin )
                {
                    query.SetGpuMemory( gpuMemory );
                }

                return log.m_Result = queryData.Begin
                    ? query.WriteBegin( buffer, offset )
                    : query.WriteEnd( buffer, offset, queryData );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns api reports.
            /// @return getData data requested by client.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static StatusCode GetData( GetReportQuery_1_0& getData )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( getData.Data != nullptr );
                ML_FUNCTION_CHECK( getData.DataSize % sizeof( TT::Layouts::PipelineTimestamps::ReportApi ) == 0 );
                ML_FUNCTION_CHECK( IsValid( getData.Handle ) );

                auto& query     = FromHandle( getData.Handle );
                auto& reportApi = *reinterpret_cast<TT::Layouts::PipelineTimestamps::ReportApi*>( getData.Data );
                auto& reportGpu = query.m_GpuReport->m_TimestampsRender;

                // Only render report.
                reportApi.m_ReportId     = 1;
                reportApi.m_ReportsCount = 1;

                // Fill api report based on gpu report.
                return log.m_Result = query.GetReportApi(
                           reportGpu,
                           reportApi );
            }

        protected:
            //////////////////////////////////////////////////////////////////////////
            /// @brief  Sets gpu memory for gpu report.
            /// @param  memory  gpu memory data.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode SetGpuMemory( const GpuMemory_1_0& memory )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( memory.CpuAddress != nullptr );
                ML_FUNCTION_CHECK( memory.GpuAddress != 0 );

                m_GpuMemory = memory;
                m_GpuReport = reinterpret_cast<TT::Layouts::PipelineTimestamps::ReportGpu*>( memory.CpuAddress );

                return log.m_Result;
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Writes begin gpu commands to command buffer.
            /// @param  buffer  target command buffer.
            /// @param  offset  memory offset.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE StatusCode WriteBegin(
                CommandBuffer& buffer,
                const uint64_t offset )
            {
                // Srm timestamp gathered on begin query.
                return T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_TimestampLow,
                    m_GpuMemory.GpuAddress + offset + offsetof( TT::Layouts::PipelineTimestamps::Timestamps, m_Begin ) );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes end gpu commands to command buffer.
            /// @param  buffer  target command buffer.
            /// @param  offset  memory offset.
            /// @param  data    command buffer data.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE StatusCode WriteEnd(
                CommandBuffer&                                  buffer,
                const uint64_t                                  offset,
                const CommandBufferQueryPipelineTimestamps_1_0& data )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                const uint64_t memoryAddressEndOfPipeline = m_GpuMemory.GpuAddress + offset + offsetof( TT::Layouts::PipelineTimestamps::Timestamps, m_EndOfPipeline );
                const uint64_t memoryAddressEndEnter      = m_GpuMemory.GpuAddress + offset + offsetof( TT::Layouts::PipelineTimestamps::Timestamps, m_EndEnter );
                const uint64_t memoryAddressEndExit       = m_GpuMemory.GpuAddress + offset + offsetof( TT::Layouts::PipelineTimestamps::Timestamps, m_EndExit );
                const uint64_t memoryAddressEndTag        = m_GpuMemory.GpuAddress + offset + offsetof( TT::Layouts::PipelineTimestamps::Timestamps, m_EndTag );

                // End timestamp enter.
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_TimestampLow,
                    memoryAddressEndEnter ) );

                // End of pipeline timestamp (64-bit).
                ML_FUNCTION_CHECK( T::GpuCommands::StorePipelineTimestamp(
                    buffer,
                    memoryAddressEndOfPipeline ) );

                // End timestamp exit.
                ML_FUNCTION_CHECK( T::GpuCommands::StoreRegisterToMemory32(
                    buffer,
                    T::GpuRegisters::m_TimestampLow,
                    memoryAddressEndExit ) );

                // End tag.
                ML_FUNCTION_CHECK( T::GpuCommands::StoreDataToMemory64(
                    buffer,
                    m_EndTag = data.EndTag,
                    memoryAddressEndTag ) );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns api report.
            /// @param  reportGpu   report obtained from gpu.
            /// @return reportApi   report to be sent to user.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetReportApi(
                const TT::Layouts::PipelineTimestamps::Timestamps& reportGpu,
                TT::Layouts::PipelineTimestamps::ReportApi&        reportApi )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( reportGpu.m_EndTag == m_EndTag );

                // All timestamps are 32 bit.
                reportApi.m_BeginTimestamp    = reportGpu.m_Begin;
                reportApi.m_EndTimestampEnter = reportGpu.m_EndEnter;
                reportApi.m_EndTimestampExit  = reportGpu.m_EndExit;
                reportApi.m_QueryInfo         = reportGpu.m_Info;

                // Convert to 32-bit to match timestamps from other instrumentation reports.
                reportApi.m_EopTimestamp = static_cast<uint32_t>( reportGpu.m_EndOfPipeline );

                return log.m_Result;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct QueryPipelineTimestampsTrait : BASE::QueryPipelineTimestampsTrait<T>
        {
            ML_DECLARE_TRAIT( QueryPipelineTimestampsTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct QueryPipelineTimestampsTrait : GEN9::QueryPipelineTimestampsTrait<T>
        {
            ML_DECLARE_TRAIT( QueryPipelineTimestampsTrait, GEN9 );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Types.
            //////////////////////////////////////////////////////////////////////////
            using Base::IsValid;
            using Base::FromHandle;

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Writes query commands to command buffer if it is not
            ///         dummy, otherwise returns size of these commands.
            /// @param  buffer      command buffer.
            /// @param  gpuMemory   gpu memory allocation.
            /// @param  data        query data.
            /// @return             operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename CommandBuffer>
            ML_INLINE static StatusCode Write(
                CommandBuffer&               buffer,
                const GpuMemory_1_0&         gpuMemory,
                const CommandBufferData_1_0& data )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( IsValid( data.QueryPipelineTimestamps.Handle ) );

                auto&    queryData = data.QueryPipelineTimestamps;
                auto&    query     = FromHandle( queryData.Handle );
                uint32_t offset    = 0;

                if( queryData.Begin )
                {
                    query.SetGpuMemory( gpuMemory );
                }

                switch( data.Type )
                {
                    case GpuCommandBufferType::Render:
                        offset = offsetof( TT::Layouts::PipelineTimestamps::ReportGpu, m_TimestampsRender );
                        break;

                    case GpuCommandBufferType::Posh:
                        offset = offsetof( TT::Layouts::PipelineTimestamps::ReportGpu, m_TimestampsPosh );
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        break;
                }

                return log.m_Result = queryData.Begin
                    ? query.WriteBegin( buffer, offset )
                    : query.WriteEnd( buffer, offset, queryData );
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns api reports.
            /// @return getData data requested by client.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static StatusCode GetData( GetReportQuery_1_0& getData )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( getData.Data != nullptr );
                ML_FUNCTION_CHECK( getData.DataSize % sizeof( TT::Layouts::PipelineTimestamps::ReportApi ) == 0 );
                ML_FUNCTION_CHECK( IsValid( getData.Handle ) );

                auto& query     = FromHandle( getData.Handle );
                auto& reportApi = *reinterpret_cast<TT::Layouts::PipelineTimestamps::ReportApi*>( getData.Data );

                // Report index / count.
                reportApi.m_ReportsCount = query.m_Context.m_ClientOptions.m_PoshEnabled ? 2 : 1;
                query.m_ReportIndex      = ( ++query.m_ReportIndex ) % reportApi.m_ReportsCount;

                // Return api report.
                switch( static_cast<GpuCommandBufferType>( query.m_ReportIndex ) )
                {
                    case GpuCommandBufferType::Render:
                        reportApi.m_ReportId = 1;
                        log.m_Result         = query.GetReportApi( query.m_GpuReport->m_TimestampsRender, reportApi );
                        break;

                    case GpuCommandBufferType::Posh:
                        reportApi.m_ReportId = 2;
                        log.m_Result         = query.GetReportApi( query.m_GpuReport->m_TimestampsPosh, reportApi );
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        log.m_Result = StatusCode::Failed;
                        break;
                }

                return log.m_Result;
            }
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct QueryPipelineTimestampsTrait : GEN11::QueryPipelineTimestampsTrait<T>
        {
            ML_DECLARE_TRAIT( QueryPipelineTimestampsTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
