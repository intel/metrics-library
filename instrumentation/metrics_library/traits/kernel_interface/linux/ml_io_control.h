/******************************************************************************\

Copyright © 2021, Intel Corporation

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

@file ml_io_control.h

@brief Allows to send io controls.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for IoControlTrait object that sends escape codes.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct IoControlTrait
        {
            ML_DELETE_DEFAULT_COPY_AND_MOVE( IoControlTrait );

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            const TT::KernelInterface& m_Kernel;
            std::string                m_KernelMetricSet;
            int32_t                    m_DrmFile;
            int32_t                    m_DrmCard;
            bool                       m_DrmOpenedByUmd;

        public:
            //////////////////////////////////////////////////////////////////////////
            /// @brief IoControlTrait constructor.
            /// @param kernel
            //////////////////////////////////////////////////////////////////////////
            IoControlTrait( const TT::KernelInterface& kernel )
                : m_Kernel( kernel )
                , m_KernelMetricSet( "" )
                , m_DrmFile( T::ConstantsOs::Drm::m_Invalid )
                , m_DrmCard( T::ConstantsOs::Drm::m_Invalid )
                , m_DrmOpenedByUmd( false )
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
            /// @return initialization status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode Initialize( const ClientData_1_0& clientData )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
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

                // Obtain path that contains metric set id
                // activated by metrics discovery.
                TT::ConstantsOs::String::Path path = "";

                snprintf( path, sizeof( path ),
                          T::ConstantsOs::Tbs::m_ActiveMetricSetPath,   // Activated metric file location.
                          m_DrmCard,                                    // Drm card index.
                          T::ConstantsOs::Tbs::m_ActiveMetricSetGuid ); // Activated metric set guid.

                m_KernelMetricSet = path;

                return log.m_Result;
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Creates dummy metric set configuration.
            /// @return dummy metric set id.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE int32_t CreateMetricSet() const
            {
                ML_FUNCTION_LOG( int32_t{ T::ConstantsOs::Tbs::m_Invalid } );

                const char*             guid                  = T::ConstantsOs::Tbs::m_ActiveMetricSetGuid;
                uint32_t                guidLength            = strlen( T::ConstantsOs::Tbs::m_ActiveMetricSetGuid );
                drm_i915_perf_oa_config configuration         = {};
                uint32_t                configurationDummy[2] = { T::GpuRegisters::m_OaTrigger2, 0 };

                // Copy guid without ending '\0' (size 36).
                T::Tools::MemoryCopy( configuration.uuid, sizeof( configuration.uuid ), guid, guidLength );

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
                ML_FUNCTION_LOG( StatusCode::Success );
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
                int32_t&               stream )
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                auto parameters           = drm_i915_perf_open_param{};
                parameters.flags          = I915_PERF_FLAG_FD_CLOEXEC | I915_PERF_FLAG_FD_NONBLOCK;
                parameters.num_properties = properties.size() / 2;
                parameters.properties_ptr = reinterpret_cast<uintptr_t>( properties.data() );

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
                int32_t       set )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( set != T::ConstantsOs::Tbs::m_Invalid );
                ML_FUNCTION_CHECK( stream != T::ConstantsOs::Tbs::m_Invalid );

                const int32_t error = drmIoctl( stream, I915_PERF_IOCTL_CONFIG, reinterpret_cast<void*>( set ) );
                log.m_Result        = ML_STATUS( error != T::ConstantsOs::Tbs::m_Invalid );

                if( ML_FAIL( log.m_Result ) )
                {
                    log.Debug( "Error id          ", errno );
                    log.Debug( "Error description ", (std::string) strerror( errno ) );
                }

                return log.m_Result;
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Returns metric set id activated by metrics discovery.
            /// @return metric set id.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE int32_t GetKernelMetricSet() const
            {
                ML_FUNCTION_LOG( int32_t{ T::ConstantsOs::Tbs::m_Invalid } );

                return ML_SUCCESS( ReadFile( m_KernelMetricSet, log.m_Result ) )
                    ? log.m_Result
                    : T::ConstantsOs::Tbs::m_Invalid;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief   Reads data from file.
            /// @param   path  file path.
            /// @return  data  data to read.
            /// @return        operation status.
            //////////////////////////////////////////////////////////////////////////
            template <typename Data>
            ML_INLINE StatusCode ReadFile(
                const std::string& path,
                Data&              data ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

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
                    log.Warning( "Read negative number of bytes, error %s", strerror( errno ) );
                    return log.m_Result = StatusCode::Failed;
                }

                buffer[readBytes] = '\0';
                data              = strtoull( buffer, 0, 0 );

                close( file );
                return log.m_Result;
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Reads tbs stream
            /// @param  stream  tbs stream id.
            /// @param  size    tbs data size to read.
            /// @return buffer  buffer with tbs reports.
            /// @return         read reports in bytes.
            //////////////////////////////////////////////////////////////////////////
            template <typename Buffer>
            ML_INLINE uint32_t ReadTbs(
                const int32_t  stream,
                const uint32_t size,
                Buffer&        buffer ) const
            {
                ML_FUNCTION_LOG( uint32_t{ 0 } );
                ML_FUNCTION_CHECK_ERROR( stream != T::ConstantsOs::Tbs::m_Invalid, 0 );

                return log.m_Result = read( stream, buffer.data(), size );
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Closes tbs stream.
            /// @param  stream tbs stream id.
            /// @return        operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode CloseTbs( const int32_t stream )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( stream != T::ConstantsOs::Tbs::m_Invalid );

                close( stream );

                return log.m_Result;
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Maps buffer to obtain cpu address.
            /// @param  handle   buffer handle.
            /// @param  size     buffer size to map.
            /// @return address  cpu address.
            /// @return          operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode MapBuffer(
                const uint32_t handle,
                const uint32_t size,
                uint8_t*&      address ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                auto map   = drm_i915_gem_mmap{};
                map.handle = handle;
                map.size   = size;
                address    = nullptr;

                log.m_Result = SendDrm( DRM_IOCTL_I915_GEM_MMAP, map );

                if( ML_SUCCESS( log.m_Result ) )
                {
                    address = reinterpret_cast<uint8_t*>( static_cast<uintptr_t>( map.addr_ptr ) );
                }

                ML_ASSERT( address != nullptr );
                return log.m_Result;
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Unmaps buffer to release cpu access.
            /// @param  handle buffer handle.
            /// @return        operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode UnmapBuffer( const uint32_t handle ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                auto unmap   = drm_i915_gem_sw_finish{};
                unmap.handle = handle;

                return log.m_Result = SendDrm( DRM_IOCTL_I915_GEM_SW_FINISH, unmap );
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Returns chipset id.
            /// @return id  chipset id.
            /// @return     operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode GetChipsetId( int32_t& id ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

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
                ML_FUNCTION_LOG( StatusCode::TbsUnableToRead );

                // Obtain oa buffer size / offset.
                auto properties = drm_i915_perf_oa_buffer_info{};
                log.m_Result    = GetStreamParameter( stream, I915_PERF_IOCTL_GET_OA_BUFFER_INFO, properties );

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
            ML_INLINE StatusCode GetTbsRevision( TT::ConstantsOs::Tbs::Revision& revision ) const
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                int32_t output = static_cast<int32_t>( T::ConstantsOs::Tbs::Revision::Unsupported );
                log.m_Result   = GetDrmParameter( I915_PARAM_PERF_REVISION, output );
                revision       = static_cast<TT::ConstantsOs::Tbs::Revision>( output );

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Gets exact gpu timestamp frequency.
            /// @param  timestampType  select timestamp domain - oa or cs.
            /// @return                gpu timestamp frequency.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE uint64_t GetGpuTimestampFrequency( const TT::Layouts::Configuration::TimestampType /*timestampType*/ ) const
            {
                ML_FUNCTION_LOG( uint64_t{ 0 } );

                int32_t frequency = 0;

                if( ML_SUCCESS( GetDrmParameter( I915_PARAM_CS_TIMESTAMP_FREQUENCY, frequency ) ) )
                {
                    log.Debug( "Gpu timestamp frequency:", frequency );
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
                ML_FUNCTION_LOG( StatusCode::Success );
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
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        return log.m_Result = StatusCode::NotSupported;
                }

                log.m_Result = SendDrm( DRM_IOCTL_I915_GETPARAM, parameters );
                result       = static_cast<Result>( output );

                return log.m_Result;
            }

        private:
            //////////////////////////////////////////////////////////////////////////
            /// @brief  Opens intel drm interface
            /// @return operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE StatusCode OpenDrm()
            {
                ML_FUNCTION_LOG( StatusCode::Success );

                m_DrmFile = drmOpenWithType( T::ConstantsOs::Drm::m_Name, NULL, DRM_NODE_RENDER );

                if( m_DrmFile < 0 )
                {
                    m_DrmFile = drmOpenWithType( T::ConstantsOs::Drm::m_Name, NULL, DRM_NODE_PRIMARY );
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
                ML_FUNCTION_LOG( StatusCode::Success );

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
                ML_FUNCTION_LOG( StatusCode::Success );
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
                ML_FUNCTION_LOG( StatusCode::Failed );

                const std::string drmDirectoryPath = GetDrmDirectoryPath();
                DIR*              drmDirectory     = opendir( drmDirectoryPath.c_str() );
                dirent*           entry            = nullptr;

                if( drmDirectory == nullptr )
                {
                    return StatusCode::Failed;
                }

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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( stream != T::ConstantsOs::Tbs::m_Invalid );

                // Check parameter availability.
                switch( request )
                {
                    case I915_PERF_IOCTL_GET_OA_BUFFER_INFO:
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        return log.m_Result = StatusCode::NotSupported;
                }

                const int32_t error = drmIoctl( stream, request, &result );

                if( error == T::ConstantsOs::Tbs::m_Invalid )
                {
                    log.Debug( "Error id          ", errno );
                    log.Debug( "Error description ", (std::string) strerror( errno ) );

                    return log.m_Result = StatusCode::Failed;
                }

                return log.m_Result = StatusCode::Success;
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
                ML_FUNCTION_LOG( StatusCode::Success );
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
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( m_DrmFile != T::ConstantsOs::Drm::m_Invalid );

                result       = drmIoctl( m_DrmFile, request, &parameters );
                log.m_Result = ML_STATUS( result != T::ConstantsOs::Drm::m_Invalid );

                if( ML_FAIL( log.m_Result ) )
                {
                    log.Debug( "Error id          ", errno );
                    log.Debug( "Error description ", (std::string) strerror( errno ) );
                }

                return log.m_Result;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns register value for a given mmio offset.
            /// @param  offset  register offset.
            /// @return         register value.
            //////////////////////////////////////////////////////////////////////////
            template <typename Type>
            ML_INLINE Type ReadRegister32( const uint32_t offset ) const
            {
                ML_STATIC_ASSERT( sizeof( Type ) == sizeof( uint32_t ), "Incorrect input size, expected 4 bytes." );

                auto output       = Type{};
                auto parameters   = drm_i915_reg_read{};
                parameters.offset = offset;

                // Availability check.
                switch( offset )
                {
                    case T::GpuRegisters::m_TimestampLow:
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        break;
                }

                // Read register with io control.
                if( ML_SUCCESS( SendDrm( DRM_IOCTL_I915_REG_READ, parameters ) ) )
                {
                    *reinterpret_cast<uint32_t*>( &output ) = static_cast<uint32_t>( parameters.val );
                }

                return output;
            }

            /////////////////////////////////////////////////////////////////////////
            /// @brief  Returns register value for a given mmio offset.
            /// @param  offset  register offset.
            /// @return         register value.
            //////////////////////////////////////////////////////////////////////////
            template <typename Type>
            ML_INLINE Type ReadRegister64( const uint32_t offset ) const
            {
                ML_STATIC_ASSERT( sizeof( Type ) == sizeof( uint64_t ), "Incorrect input size, expected 8 bytes." );

                auto output       = Type{};
                auto parameters   = drm_i915_reg_read{};
                parameters.offset = offset;

                // Availability check.
                switch( offset )
                {
                    case T::GpuRegisters::m_TimestampLow:
                        break;

                    default:
                        ML_ASSERT_ALWAYS();
                        break;
                }

                // Read register with io control.
                if( ML_SUCCESS( SendDrm( DRM_IOCTL_I915_REG_READ, parameters ) ) )
                {
                    *reinterpret_cast<uint64_t*>( &output ) = parameters.val;
                }

                return output;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct IoControlTrait : BASE::IoControlTrait<T>
        {
            ML_DECLARE_TRAIT( IoControlTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct IoControlTrait : GEN9::IoControlTrait<T>
        {
            ML_DECLARE_TRAIT( IoControlTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct IoControlTrait : GEN11::IoControlTrait<T>
        {
            ML_DECLARE_TRAIT( IoControlTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
