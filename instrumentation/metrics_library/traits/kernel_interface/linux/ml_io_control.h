/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_io_control.h

@brief Allows to send io controls.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for IoControlTrait object that sends escape codes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct IoControlTrait : TraitObject<T, TT::IoControl>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( IoControlTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( IoControlTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base = TraitObject<T, TT::IoControl>;
        using Base::Derived;

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        std::string m_KernelMetricSet;
        bool        m_DrmOpenedByUmd;

    protected:
        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        const TT::KernelInterface& m_Kernel;
        int32_t                    m_DrmFile;

    public:
        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        int32_t m_DrmCard;

    public:
        //////////////////////////////////////////////////////////////////////////
        /// @brief IoControlTrait constructor.
        /// @param kernel
        //////////////////////////////////////////////////////////////////////////
        IoControlTrait( const TT::KernelInterface& kernel )
            : m_KernelMetricSet( "" )
            , m_DrmOpenedByUmd( false )
            , m_Kernel( kernel )
            , m_DrmFile( T::ConstantsOs::Drm::m_Invalid )
            , m_DrmCard( T::ConstantsOs::Drm::m_Invalid )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief IoControlTrait destructor.
        //////////////////////////////////////////////////////////////////////////
        ~IoControlTrait()
        {
            if( !m_DrmOpenedByUmd )
            {
                CloseDrm();
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "IoControlTrait<Traits> (Linux)";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes io control interface.
        /// @param  clientData  initializing client data.
        /// @return revision    drm performance revision.
        /// @return initialization status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize(
            const ClientData_1_0&           clientData,
            TT::ConstantsOs::Drm::Revision& revision )
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_ASSERT( m_DrmFile == T::ConstantsOs::Drm::m_Invalid );
            ML_ASSERT( m_DrmCard == T::ConstantsOs::Drm::m_Invalid );

            if( clientData.Linux.Adapter != nullptr )
            {
                const ClientDataLinuxAdapter_1_0& adapter = *clientData.Linux.Adapter;

                switch( adapter.Type )
                {
                    case LinuxAdapterType::DrmFileDescriptor:
                        m_DrmFile        = adapter.DrmFileDescriptor;
                        m_DrmOpenedByUmd = true;
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        return log.m_Result = StatusCode::IncorrectParameter;
                }
            }
            else if( ML_FAIL( OpenDrm() ) )
            {
                log.Error( "Failed to open drm render node" );
                return log.m_Result = StatusCode::Failed;
            }

            if( ML_FAIL( GetDrmCardNumber() ) )
            {
                if( !m_DrmOpenedByUmd )
                {
                    CloseDrm();
                }

                log.Error( "Failed to get drm card information" );
                return log.m_Result = StatusCode::Failed;
            }

            // Obtain path that contains metric set id activated by metrics discovery.
            TT::ConstantsOs::String::Path path           = "";
            const uint32_t                subDeviceIndex = m_Kernel.m_Context.m_ClientOptions.m_IsSubDevice ? m_Kernel.m_Context.m_ClientOptions.m_SubDeviceIndex : 0;
            const std::string             guid           = GenerateQueryGuid( subDeviceIndex );
            ML_FUNCTION_CHECK( guid != "" );

            snprintf( path, sizeof( path ),
                      T::ConstantsOs::Tbs::m_ActiveMetricSetPath, // Activated metric file location.
                      m_DrmCard,                                  // Drm card index.
                      guid.c_str() );                             // Activated metric set guid for given sub device.

            m_KernelMetricSet = path;

            // Obtain drm performance revision.
            // No check for fail, since old kernels do not support this information.
            Derived().GetDrmRevision( revision );

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns metric set id activated by metrics discovery.
        /// @return metric set id.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE int32_t GetKernelMetricSet() const
        {
            ML_FUNCTION_LOG( int32_t{ T::ConstantsOs::Tbs::m_Invalid }, &m_Kernel.m_Context );

            if( ML_FAIL( ReadFile( m_KernelMetricSet, log.m_Result ) ) )
            {
                log.Warning( "Cannot get kernel metric set" );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Reads data from file.
        /// @param  path    file path.
        /// @return data    data to read.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename Data>
        ML_INLINE StatusCode ReadFile(
            const std::string& path,
            Data&              data ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            Constants::String::Buffer buffer    = {};
            int32_t                   file      = open( path.c_str(), O_RDONLY );
            int32_t                   readBytes = 0;

            if( file < 0 )
            {
                return log.m_Result = StatusCode::Failed;
            }

            readBytes = read( file, buffer, sizeof( buffer ) - 1 );

            if( readBytes < 0 )
            {
                close( file );
                log.Warning( "Read negative number of bytes", errno, strerror( errno ) );
                return log.m_Result = StatusCode::Failed;
            }

            buffer[readBytes] = '\0';
            data              = strtoull( buffer, 0, 0 );

            close( file );
            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Closes tbs stream.
        /// @param  stream tbs stream id.
        /// @return        operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode CloseTbs( const int32_t stream ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( stream != T::ConstantsOs::Tbs::m_Invalid );

            close( stream );

            return log.m_Result;
        }

    protected:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Opens intel drm interface
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode OpenDrm()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            m_DrmFile = drmOpenWithType( T::Layouts::Drm::File::m_Name, nullptr, DRM_NODE_RENDER );

            if( m_DrmFile < 0 )
            {
                m_DrmFile = drmOpenWithType( T::Layouts::Drm::File::m_Name, nullptr, DRM_NODE_PRIMARY );
            }

            if( m_DrmFile < 0 )
            {
                log.Error( "Failed to get drm card info", errno );
                return log.m_Result = StatusCode::Failed;
            }

            log.Debug( "Drm device opened", m_DrmFile );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Closes drm interface.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void CloseDrm()
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            if( m_DrmFile >= 0 )
            {
                drmClose( m_DrmFile );
                m_DrmFile = T::ConstantsOs::Drm::m_Invalid;
            }

            m_DrmCard = T::ConstantsOs::Drm::m_Invalid;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns intel drm directory path.
        /// @return directory path.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE std::string GetDrmDirectoryPath() const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_ASSERT( m_DrmFile != T::ConstantsOs::Drm::m_Invalid );

            struct stat             fileInfo = {};
            Constants::String::Path path     = {};

            if( fstat( m_DrmFile, &fileInfo ) )
            {
                log.Error( "Failed to stat drm file descriptor" );
                return nullptr;
            }

            const int32_t majorNumber = major( fileInfo.st_rdev );
            const int32_t minorNumber = minor( fileInfo.st_rdev );

            snprintf( path, sizeof( path ), T::ConstantsOs::Drm::m_Path, majorNumber, minorNumber );
            log.Debug( "Drm path", path );

            return path;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns intel drm card number.
        /// @return operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetDrmCardNumber()
        {
            ML_FUNCTION_LOG( StatusCode::Failed, &m_Kernel.m_Context );

            const std::string drmDirectoryPath = GetDrmDirectoryPath();
            DIR*              drmDirectory     = opendir( drmDirectoryPath.c_str() );
            dirent*           entry            = nullptr;

            if( drmDirectory != nullptr )
            {
                // Look for a directory named 'card.*'.
                while( ( entry = readdir( drmDirectory ) ) != nullptr )
                {
                    const bool validDirectory = entry->d_type == DT_DIR;
                    const bool validCard      = validDirectory && ( strncmp( entry->d_name, "card", 4 ) == 0 );

                    if( validCard )
                    {
                        m_DrmCard = strtoull( entry->d_name + 4, nullptr, 10 );
                        log.Debug( "DRM card number is ", m_DrmCard );
                        closedir( drmDirectory );
                        return log.m_Result = StatusCode::Success;
                    }
                }

                closedir( drmDirectory );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Sends io control to drm.
        /// @param  request    drm request type.
        /// @return parameters obtained data.
        /// @return            operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename Data>
        ML_INLINE StatusCode SendDrm(
            const uint32_t& request,
            Data&           parameters ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( m_DrmFile != T::ConstantsOs::Drm::m_Invalid );

            int32_t result = T::ConstantsOs::Drm::m_Invalid;
            log.m_Result   = SendDrm( request, parameters, result );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Sends io control to drm.
        /// @param  request    drm request type.
        /// @return parameter  parameter to send/receive.
        /// @return result     io control result.
        /// @return            operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename Data>
        ML_INLINE StatusCode SendDrm(
            const uint32_t request,
            Data&          parameters,
            int32_t&       result ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( m_DrmFile != T::ConstantsOs::Drm::m_Invalid );

            result       = drmIoctl( m_DrmFile, request, &parameters );
            log.m_Result = ML_STATUS( result != T::ConstantsOs::Drm::m_Invalid );

            if( ML_FAIL( log.m_Result ) )
            {
                log.Debug( "Error id          ", errno );
                log.Debug( "Error description ", strerror( errno ) );
            }

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Generates guid for query for given sub device index.
        /// @param  subDeviceIndex  sub device index.
        /// @return                 generated guid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE std::string GenerateQueryGuid( const uint32_t subDeviceIndex ) const
        {
            ML_FUNCTION_LOG( std::string(), &m_Kernel.m_Context );

            const std::string defaultGuid = T::ConstantsOs::Tbs::m_ActiveMetricSetGuid;

            if( subDeviceIndex == 0 )
            {
                return log.m_Result = defaultGuid;
            }

            const std::string valueToReplace    = "42a7";
            const uint32_t    maxSubDeviceIndex = std::pow( 2, valueToReplace.length() * 4 ) - 1;

            if( subDeviceIndex > maxSubDeviceIndex )
            {
                log.Error( "Invalid sub device index" );
                return log.m_Result = "";
            }

            std::stringstream stream;
            stream << std::setfill( '0' ) << std::setw( valueToReplace.length() ) << std::hex << subDeviceIndex;

            return log.m_Result = std::regex_replace( defaultGuid, std::regex( valueToReplace ), stream.str() );
        }
    };
} // namespace ML::BASE

namespace ML::GEN9
{
    template <typename T>
    struct IoControlTrait : BASE::IoControlTrait<T>
    {
        ML_DECLARE_TRAIT( IoControlTrait, BASE );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::GenerateQueryGuid;
        using Base::SendDrm;
        using Base::m_Kernel;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Queries i915 for specific information length.
        /// @param  id      query id.
        /// @return         data length.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t QueryLength( const uint32_t id ) const
        {
            ML_FUNCTION_LOG( uint32_t{ 0 }, &m_Kernel.m_Context );

            auto query = drm_i915_query{};
            auto item  = drm_i915_query_item{};

            // Query length.
            item.query_id   = id;
            item.length     = 0;
            query.items_ptr = reinterpret_cast<uint64_t>( &item );
            query.num_items = 1;

            const bool validCall   = ML_SUCCESS( Query( query ) );
            const bool validLength = item.length > 0;

            return log.m_Result = ( validCall && validLength )
                ? item.length
                : 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Queries i915 for specific information.
        /// @param  query   query structure.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Query( drm_i915_query& query ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            return log.m_Result = SendDrm( DRM_IOCTL_I915_QUERY, query );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Queries i915 for specific information.
        /// @param  id      query id.
        /// @return data    returned data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Query(
            const uint32_t        id,
            std::vector<uint8_t>& data ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            auto query = drm_i915_query{};
            auto item  = drm_i915_query_item{};

            // Prepare space for query data.
            data.resize( QueryLength( id ) );

            // Prepare query item.
            item.query_id   = id;
            item.length     = data.size();
            item.data_ptr   = reinterpret_cast<uint64_t>( data.data() );
            query.items_ptr = reinterpret_cast<uint64_t>( &item );
            query.num_items = 1;

            // Input check.
            ML_FUNCTION_CHECK( item.length > 0 );

            // Send io control.
            ML_FUNCTION_CHECK( Query( query ) );

            // Output check.
            ML_FUNCTION_CHECK( data.size() == static_cast<uint32_t>( item.length ) );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Queries i915 for specific information.
        /// @param  id      query id.
        /// @return data    output data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename Data>
        ML_INLINE StatusCode Query(
            const uint32_t id,
            Data&          data ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            auto query = drm_i915_query{};
            auto item  = drm_i915_query_item{};

            item.query_id   = id;
            item.length     = QueryLength( id );
            item.data_ptr   = reinterpret_cast<uint64_t>( &data );
            query.items_ptr = reinterpret_cast<uint64_t>( &item );
            query.num_items = 1;

            // Input check.
            ML_FUNCTION_CHECK( item.length > 0 );
            ML_FUNCTION_CHECK( item.length == sizeof( data ) );

            // Send io control.
            ML_FUNCTION_CHECK( Query( query ) );

            // Output check.
            ML_FUNCTION_CHECK( item.length > 0 );
            ML_FUNCTION_CHECK( item.length == sizeof( data ) );

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Creates dummy metric set configuration.
        /// @return dummy metric set id.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE int32_t CreateMetricSet() const
        {
            ML_FUNCTION_LOG( int32_t{ T::ConstantsOs::Tbs::m_Invalid }, &m_Kernel.m_Context );

            const uint32_t    subDeviceIndex = m_Kernel.m_Context.m_ClientOptions.m_IsSubDevice ? m_Kernel.m_Context.m_ClientOptions.m_SubDeviceIndex : 0;
            const std::string guid           = GenerateQueryGuid( subDeviceIndex );
            ML_FUNCTION_CHECK_ERROR( guid != "", T::ConstantsOs::Tbs::m_Invalid );

            drm_i915_perf_oa_config configuration         = {};
            uint32_t                configurationDummy[2] = { T::GpuRegisters::m_OaTrigger2, 0 };

            // Copy guid without ending '\0' (size 36).
            T::Tools::MemoryCopy( configuration.uuid, sizeof( configuration.uuid ), guid.c_str(), guid.size() );

            // Dummy configuration parameters.
            configuration.boolean_regs_ptr = reinterpret_cast<uint64_t>( configurationDummy );
            configuration.mux_regs_ptr     = 0;
            configuration.flex_regs_ptr    = 0;

            configuration.n_boolean_regs = 1;
            configuration.n_mux_regs     = 0;
            configuration.n_flex_regs    = 0;

            // Send configuration to linux kernel.
            SendDrm( DRM_IOCTL_I915_PERF_ADD_CONFIG, configuration, log.m_Result );

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Removes metric set configuration from the kernel.
        ///         Do not change int64_t to int32_t. Drm uses int32_t to
        ///         identify metric set. But int64_t is needed to remove
        ///         metric set configuration.
        /// @param  set  metric set to remove.
        /// @return      operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode RemoveMetricSet( int64_t set ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( set != T::ConstantsOs::Tbs::m_Invalid );

            return log.m_Result = SendDrm( DRM_IOCTL_I915_PERF_REMOVE_CONFIG, set );
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Opens tbs stream.
        /// @param  properties   tbs stream properties.
        /// @return stream       opened tbs stream id.
        /// @return              operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode OpenTbs(
            std::vector<uint64_t>& properties,
            int32_t&               stream ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            auto parameters           = drm_i915_perf_open_param{};
            parameters.flags          = I915_PERF_FLAG_FD_CLOEXEC | I915_PERF_FLAG_FD_NONBLOCK;
            parameters.num_properties = properties.size() / 2;
            parameters.properties_ptr = reinterpret_cast<uintptr_t>( properties.data() );

            // Print properties.
            for( uint32_t i = 0; i < properties.size() / 2; ++i )
            {
                log.Info( "Tbs properties: ", properties[i * 2], properties[i * 2 + 1] );
            }

            return log.m_Result = SendDrm( DRM_IOCTL_I915_PERF_OPEN, parameters, stream );
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Sets new tbs metric set.
        /// @param  stream  tbs stream.
        /// @param  set     metric set to use with tbs stream.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode SetTbsMetricSet(
            const int32_t stream,
            int32_t       set ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( set != T::ConstantsOs::Tbs::m_Invalid );
            ML_FUNCTION_CHECK( stream != T::ConstantsOs::Tbs::m_Invalid );

            const int32_t error = drmIoctl( stream, I915_PERF_IOCTL_CONFIG, reinterpret_cast<void*>( set ) );
            log.m_Result        = ML_STATUS( error != T::ConstantsOs::Tbs::m_Invalid );

            if( ML_FAIL( log.m_Result ) )
            {
                log.Debug( "Error id          ", errno );
                log.Debug( "Error description ", strerror( errno ) );
            }

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns chipset id.
        /// @return id  chipset id.
        /// @return     operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetChipsetId( int32_t& id ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            return log.m_Result = GetDrmParameter( I915_PARAM_CHIPSET_ID, id );
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa buffer cpu address.
        /// @param  stream     tbs stream id.
        /// @return addressCpu oa buffer mapped cpu address.
        /// @return size       oa buffer size.
        /// @return            operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode MapOaBuffer(
            const int32_t stream,
            void*&        addressCpu,
            uint32_t&     size ) const
        {
            ML_FUNCTION_LOG( StatusCode::TbsUnableToRead, &m_Kernel.m_Context );

            // Obtain oa buffer size / offset.
            auto properties = prelim_drm_i915_perf_oa_buffer_info{};
            log.m_Result    = GetStreamParameter( stream, PRELIM_I915_PERF_IOCTL_GET_OA_BUFFER_INFO, properties );

            if( ML_FAIL( log.m_Result ) )
            {
                return log.m_Result;
            }

            // Obtain oa buffer cpu address.
            size         = properties.size;
            addressCpu   = mmap( 0, properties.size, PROT_READ, MAP_PRIVATE, stream, properties.offset );
            log.m_Result = ML_STATUS( ( addressCpu != nullptr ) && ( addressCpu != reinterpret_cast<void*>( -1 ) ) );

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns  performance module revision.
        /// @return revision performance module revision.
        /// @return          operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetDrmRevision( TT::ConstantsOs::Drm::Revision& revision ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            int32_t output = static_cast<int32_t>( T::ConstantsOs::Drm::Revision::Unsupported );
            log.m_Result   = GetDrmParameter( I915_PARAM_PERF_REVISION, output );
            revision       = static_cast<TT::ConstantsOs::Drm::Revision>( output );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets exact gpu timestamp frequency.
        /// @param  timestampType  select timestamp domain - oa or cs.
        /// @return                gpu timestamp frequency.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t GetGpuTimestampFrequency( [[maybe_unused]] const TT::Layouts::Configuration::TimestampType timestampType ) const
        {
            ML_FUNCTION_LOG( uint64_t{ 0 }, &m_Kernel.m_Context );

            if( int32_t frequency = 0;
                ML_SUCCESS( GetDrmParameter( I915_PARAM_CS_TIMESTAMP_FREQUENCY, frequency ) ) )
            {
                log.m_Result = static_cast<uint64_t>( frequency );
            }
            else
            {
                log.Debug( "Unable to obtain gpu timestamp frequency." );
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns parameter from the drm.
        /// @param  parameter   drm parameter type.
        /// @return result      result value.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename Result>
        ML_INLINE StatusCode GetDrmParameter(
            const uint32_t parameter,
            Result&        result ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_STATIC_ASSERT( sizeof( Result ) == sizeof( int32_t ), "Incorrect input size, expected 4 bytes." );

            int32_t output     = 0;
            auto    parameters = drm_i915_getparam_t{};
            parameters.param   = parameter;
            parameters.value   = &output;

            // Check parameter availability.
            switch( parameter )
            {
                case I915_PARAM_PERF_REVISION:
                case I915_PARAM_CHIPSET_ID:
                case I915_PARAM_CS_TIMESTAMP_FREQUENCY:
                case I915_PARAM_OA_TIMESTAMP_FREQUENCY:
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    return log.m_Result = StatusCode::NotSupported;
            }

            log.m_Result = SendDrm( DRM_IOCTL_I915_GETPARAM, parameters );
            result       = static_cast<Result>( output );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns parameter from the tbs stream.
        /// @param  stream   tbs stream id.
        /// @param  request  request type to send.
        /// @return result   result value.
        /// @return          operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename Result>
        ML_INLINE StatusCode GetStreamParameter(
            const int32_t  stream,
            const uint32_t request,
            Result&        result ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( stream != T::ConstantsOs::Tbs::m_Invalid );

            // Check parameter availability.
            switch( request )
            {
                case PRELIM_I915_PERF_IOCTL_GET_OA_BUFFER_INFO:
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    return log.m_Result = StatusCode::NotSupported;
            }

            const int32_t error = drmIoctl( stream, request, &result );

            if( error == T::ConstantsOs::Tbs::m_Invalid )
            {
                log.Debug( "Error id          ", errno );
                log.Debug( "Error description ", strerror( errno ) );

                return log.m_Result = StatusCode::Failed;
            }

            return log.m_Result = StatusCode::Success;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns query mode override state.
        /// @return query mode.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::Query::Mode GetQueryModeOverride() const
        {
            ML_FUNCTION_LOG( T::Layouts::HwCounters::Query::Mode::Default, &m_Kernel.m_Context );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns report collecting mode override state.
        /// @return report collecting mode.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::Query::ReportCollectingMode GetReportCollectingModeOverride() const
        {
            ML_FUNCTION_LOG( T::Layouts::HwCounters::Query::ReportCollectingMode::Default, &m_Kernel.m_Context );

            return log.m_Result;
        }
    };
} // namespace ML::GEN9

namespace ML::GEN11
{
    template <typename T>
    struct IoControlTrait : GEN9::IoControlTrait<T>
    {
        ML_DECLARE_TRAIT( IoControlTrait, GEN9 );
    };
} // namespace ML::GEN11

namespace ML::XE_LP
{
    template <typename T>
    struct IoControlTrait : GEN11::IoControlTrait<T>
    {
        ML_DECLARE_TRAIT( IoControlTrait, GEN11 );
    };
} // namespace ML::XE_LP

namespace ML::XE_HP
{
    template <typename T>
    struct IoControlTrait : XE_LP::IoControlTrait<T>
    {
        ML_DECLARE_TRAIT( IoControlTrait, XE_LP );
    };
} // namespace ML::XE_HP

namespace ML::XE_HPG
{
    template <typename T>
    struct IoControlTrait : XE_HP::IoControlTrait<T>
    {
        ML_DECLARE_TRAIT( IoControlTrait, XE_HP );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::GetDrmParameter;
        using Base::m_Kernel;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets exact gpu timestamp frequency.
        /// @param  timestampType  select timestamp domain - oa or cs.
        /// @return                gpu timestamp frequency.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t GetGpuTimestampFrequency( const TT::Layouts::Configuration::TimestampType timestampType ) const
        {
            ML_FUNCTION_LOG( uint64_t{ 0 }, &m_Kernel.m_Context );

            int32_t frequency = 0;
            int32_t parameter = timestampType == T::Layouts::Configuration::TimestampType::Oa
                ? I915_PARAM_OA_TIMESTAMP_FREQUENCY
                : I915_PARAM_CS_TIMESTAMP_FREQUENCY;

            if( ML_SUCCESS( GetDrmParameter( parameter, frequency ) ) )
            {
                log.m_Result = static_cast<uint64_t>( frequency );
            }
            else
            {
                if( parameter == I915_PARAM_OA_TIMESTAMP_FREQUENCY )
                {
                    if( ML_SUCCESS( GetDrmParameter( I915_PARAM_CS_TIMESTAMP_FREQUENCY, frequency ) ) )
                    {
                        frequency <<= 1;
                        log.Debug( "Oa timestamp frequency is not available, Cs timestamp frequency is used as a workaround." );
                        log.m_Result = static_cast<uint64_t>( frequency );
                    }
                }
                else
                {
                    log.Debug( "Unable to obtain gpu timestamp frequency." );
                }
            }

            return log.m_Result;
        }
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct IoControlTrait : XE_HPG::IoControlTrait<T>
    {
        ML_DECLARE_TRAIT( IoControlTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct IoControlTrait : BASE::IoControlTrait<T>
    {
        ML_DECLARE_TRAIT( IoControlTrait, BASE );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using Base::GenerateQueryGuid;
        using Base::SendDrm;
        using Base::m_DrmFile;
        using Base::m_Kernel;

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Opens tbs stream.
        /// @param  properties   tbs stream properties.
        /// @return stream       opened tbs stream id.
        /// @return              operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode OpenTbs(
            drm_xe_ext_set_property properties[],
            int32_t&                stream ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            // Print properties.
            for( uint32_t i = 0; i < DRM_XE_OA_PROPERTY_NO_PREEMPT - DRM_XE_OA_EXTENSION_SET_PROPERTY; ++i )
            {
                std::string propertyName = "";

                switch( properties[i].property )
                {
                    case DRM_XE_OA_PROPERTY_OA_UNIT_ID:
                        propertyName = "Oa unit:";
                        break;

                    case DRM_XE_OA_PROPERTY_SAMPLE_OA:
                        propertyName = "Sample oa:";
                        break;

                    case DRM_XE_OA_PROPERTY_OA_METRIC_SET:
                        propertyName = "Metric set:";
                        break;

                    case DRM_XE_OA_PROPERTY_OA_FORMAT:
                        propertyName = "Oa format:";
                        break;

                    case DRM_XE_OA_PROPERTY_OA_PERIOD_EXPONENT:
                        propertyName = "Period exponent:";
                        break;

                    case DRM_XE_OA_PROPERTY_OA_DISABLED:
                        propertyName = "Oa disabled:";
                        break;

                    case DRM_XE_OA_PROPERTY_EXEC_QUEUE_ID:
                        propertyName = "Exec queue id:";
                        break;

                    case DRM_XE_OA_PROPERTY_OA_ENGINE_INSTANCE:
                        propertyName = "Engine instance:";
                        break;

                    case DRM_XE_OA_PROPERTY_NO_PREEMPT:
                        propertyName = "No preempt:";
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        break;
                }

                log.Info( propertyName, static_cast<uint32_t>( properties[i].value ) );

                if( properties[i].base.next_extension == 0 )
                {
                    break;
                }
            }

            return log.m_Result = SendXeObservation( DRM_XE_OBSERVATION_OP_STREAM_OPEN, *properties, stream );
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Sets new tbs metric set.
        /// @param  stream  tbs stream.
        /// @param  set     metric set to use with tbs stream.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode SetTbsMetricSet(
            const int32_t stream,
            int32_t       set ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( set != T::ConstantsOs::Tbs::m_Invalid );
            ML_FUNCTION_CHECK( stream != T::ConstantsOs::Tbs::m_Invalid );

            drm_xe_ext_set_property property = {};
            property.base.name               = DRM_XE_OA_EXTENSION_SET_PROPERTY;
            property.property                = DRM_XE_OA_PROPERTY_OA_METRIC_SET;
            property.value                   = set;

            const int32_t error = drmIoctl( stream, DRM_XE_OBSERVATION_IOCTL_CONFIG, reinterpret_cast<void*>( &property ) );
            log.m_Result        = ML_STATUS( error != T::ConstantsOs::Tbs::m_Invalid );

            if( ML_FAIL( log.m_Result ) )
            {
                log.Debug( "Error id          ", errno );
                log.Debug( "Error description ", strerror( errno ) );
            }

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Removes metric set configuration from the kernel.
        ///         Do not change int64_t to int32_t. Drm uses int32_t to
        ///         identify metric set. But int64_t is needed to remove
        ///         metric set configuration.
        /// @param  set  metric set to remove.
        /// @return      operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode RemoveMetricSet( int64_t set ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( set != T::ConstantsOs::Tbs::m_Invalid );

            int32_t result = T::ConstantsOs::Drm::m_Invalid;

            return log.m_Result = SendXeObservation( DRM_XE_OBSERVATION_OP_REMOVE_CONFIG, set, result );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Gets exact gpu timestamp frequency.
        /// @param  timestampType  select timestamp domain - oa or cs.
        /// @return                gpu timestamp frequency.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint64_t GetGpuTimestampFrequency( const TT::Layouts::Configuration::TimestampType timestampType ) const
        {
            ML_FUNCTION_LOG( uint64_t{ 0 }, &m_Kernel.m_Context );

            std::vector<uint8_t> buffer = {};

            if( timestampType == T::Layouts::Configuration::TimestampType::Oa )
            {
                // Query check.
                const bool validQuery  = ML_SUCCESS( Query( DRM_XE_DEVICE_QUERY_OA_UNITS, buffer ) );
                const bool validBuffer = buffer.size() > 0;
                const auto oaData      = reinterpret_cast<drm_xe_query_oa_units*>( buffer.data() );
                const bool validData   = ( oaData != nullptr ) && ( oaData->num_oa_units > 0 );

                if( validQuery && validBuffer && validData )
                {
                    const auto oaUnit = reinterpret_cast<drm_xe_oa_unit*>( oaData->oa_units );

                    return log.m_Result = oaUnit->oa_timestamp_freq;
                }
            }
            else
            {
                // Query check.
                const bool validQuery  = ML_SUCCESS( Query( DRM_XE_DEVICE_QUERY_GT_LIST, buffer ) );
                const bool validBuffer = buffer.size() > 0;

                auto gtsData = reinterpret_cast<drm_xe_query_gt_list*>( buffer.data() );

                const bool validData = ( gtsData != nullptr ) && ( gtsData->num_gt > 0 );

                if( validQuery && validBuffer && validData )
                {
                    return log.m_Result = gtsData->gt_list[0].reference_clock;
                }
            }

            log.Error( "Failed to get timestamp" );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Queries XE for specific information.
        /// @param  query   query structure.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Query( drm_xe_device_query& query ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            return log.m_Result = SendDrm( DRM_IOCTL_XE_DEVICE_QUERY, query );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Queries XE for specific information length.
        /// @param  id      query id.
        /// @return         data length.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t QueryLength( const uint32_t id ) const
        {
            ML_FUNCTION_LOG( uint32_t{ 0 }, &m_Kernel.m_Context );

            auto query = drm_xe_device_query{};

            // Query length.
            query.query = id;

            const bool validCall   = ML_SUCCESS( Query( query ) );
            const bool validLength = query.size > 0;

            return log.m_Result = ( validCall && validLength )
                ? query.size
                : 0;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Queries XE for specific information.
        /// @param  id      query id.
        /// @return data    returned data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Query(
            const uint32_t        id,
            std::vector<uint8_t>& data ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            auto query = drm_xe_device_query{};

            // Prepare space for query data.
            data.resize( QueryLength( id ) );

            // Prepare query item.
            query.query = id;
            query.size  = data.size();
            query.data  = reinterpret_cast<uint64_t>( data.data() );

            // Input check.
            ML_FUNCTION_CHECK( query.size > 0 );

            // Send io control.
            ML_FUNCTION_CHECK( Query( query ) );

            // Output check.
            ML_FUNCTION_CHECK( data.size() == static_cast<uint32_t>( query.size ) );

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns  observation module revision.
        /// @return revision observation module revision.
        /// @return          operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetDrmRevision( [[maybe_unused]] TT::ConstantsOs::Drm::Revision& revision ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            // XE OA does not support revisions.

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns chipset id.
        /// @return id  chipset id.
        /// @return     operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetChipsetId( int32_t& id ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            std::vector<uint8_t> buffer = {};

            ML_FUNCTION_CHECK( Query( DRM_XE_QUERY_CONFIG_REV_AND_DEVICE_ID, buffer ) );
            ML_FUNCTION_CHECK( buffer.size() > 0 );

            auto config = reinterpret_cast<drm_xe_query_config*>( buffer.data() );

            ML_FUNCTION_CHECK( config != nullptr );

            id = config->info[DRM_XE_QUERY_CONFIG_REV_AND_DEVICE_ID] & 0xffff;

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Returns oa buffer cpu address.
        /// @param  stream     tbs stream id.
        /// @return addressCpu oa buffer mapped cpu address.
        /// @return size       oa buffer size.
        /// @return            operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode MapOaBuffer(
            const int32_t stream,
            void*&        addressCpu,
            uint32_t&     size ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );

            // Obtain oa buffer size / offset.
            auto properties = drm_xe_oa_stream_info{};
            log.m_Result    = GetStreamParameter( stream, DRM_XE_OBSERVATION_IOCTL_INFO, properties );

            if( ML_FAIL( log.m_Result ) )
            {
                return log.m_Result;
            }

            // Obtain oa buffer cpu address.
            size         = properties.oa_buf_size;
            addressCpu   = mmap( 0, size, PROT_READ, MAP_PRIVATE, stream, 0 );
            log.m_Result = ML_STATUS( ( addressCpu != nullptr ) && ( addressCpu != reinterpret_cast<void*>( -1 ) ) );

            return log.m_Result;
        }

        /////////////////////////////////////////////////////////////////////////
        /// @brief  Creates dummy metric set configuration.
        /// @return dummy metric set id.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE int32_t CreateMetricSet() const
        {
            ML_FUNCTION_LOG( int32_t{ T::ConstantsOs::Tbs::m_Invalid }, &m_Kernel.m_Context );

            const uint32_t    subDeviceIndex = m_Kernel.m_Context.m_ClientOptions.m_IsSubDevice ? m_Kernel.m_Context.m_ClientOptions.m_SubDeviceIndex : 0;
            const std::string guid           = GenerateQueryGuid( subDeviceIndex );
            ML_FUNCTION_CHECK_ERROR( guid != "", T::ConstantsOs::Tbs::m_Invalid );

            drm_xe_oa_config configuration         = {};
            uint32_t         configurationDummy[2] = { T::GpuRegisters::m_OaTrigger2, 0 };

            // Copy guid without ending '\0' (size 36).
            T::Tools::MemoryCopy( configuration.uuid, sizeof( configuration.uuid ), guid.c_str(), guid.size() );

            // Dummy configuration parameters.
            configuration.regs_ptr = reinterpret_cast<uint64_t>( configurationDummy );
            configuration.n_regs   = 1;

            // Send configuration to linux kernel.
            SendXeObservation( DRM_XE_OBSERVATION_OP_ADD_CONFIG, configuration, log.m_Result );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns query mode override state.
        /// @return query mode.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::Query::Mode GetQueryModeOverride() const
        {
            ML_FUNCTION_LOG( T::Layouts::HwCounters::Query::Mode::Global, &m_Kernel.m_Context );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns report collecting mode override state.
        /// @return report collecting mode.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE TT::Layouts::HwCounters::Query::ReportCollectingMode GetReportCollectingModeOverride() const
        {
            ML_FUNCTION_LOG( T::Layouts::HwCounters::Query::ReportCollectingMode::TriggerOag, &m_Kernel.m_Context );

            return log.m_Result;
        }

    protected:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Sends xe observation io control to drm.
        /// @param  operation   xe observation operation type.
        /// @return parameters  parameters to send.
        /// @return result      xe observation io control result.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename Data>
        ML_INLINE StatusCode SendXeObservation(
            const uint32_t operation,
            Data&          parameters,
            int32_t&       result ) const
        {
            auto xeObservation = drm_xe_observation_param{};

            xeObservation.observation_type = DRM_XE_OBSERVATION_TYPE_OA;
            xeObservation.observation_op   = operation;
            xeObservation.param            = reinterpret_cast<uint64_t>( &parameters );

            return SendDrm( DRM_IOCTL_XE_OBSERVATION, xeObservation, result );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns parameter from the tbs stream.
        /// @param  stream   tbs stream id.
        /// @param  request  request type to send.
        /// @return result   result value.
        /// @return          operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename Result>
        ML_INLINE StatusCode GetStreamParameter(
            const int32_t  stream,
            const uint32_t request,
            Result&        result ) const
        {
            ML_FUNCTION_LOG( StatusCode::Success, &m_Kernel.m_Context );
            ML_FUNCTION_CHECK( stream != T::ConstantsOs::Tbs::m_Invalid );

            // Check parameter availability.
            switch( request )
            {
                case DRM_XE_OBSERVATION_IOCTL_INFO:
                    break;

                default:
                    ML_ASSERT_ALWAYS();
                    return log.m_Result = StatusCode::NotSupported;
            }

            const int32_t error = drmIoctl( stream, request, &result );

            if( error == T::ConstantsOs::Tbs::m_Invalid )
            {
                log.Debug( "Error id          ", errno );
                log.Debug( "Error description ", strerror( errno ) );

                return log.m_Result = StatusCode::Failed;
            }

            return log.m_Result = StatusCode::Success;
        }
    };
} // namespace ML::XE2_HPG
