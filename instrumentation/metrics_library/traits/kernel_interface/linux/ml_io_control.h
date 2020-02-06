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

@file ml_io_control.h

@brief Allows to send io controls.

\******************************************************************************/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for IoControl object.
    //////////////////////////////////////////////////////////////////////////
    template <typename InputOutputType, IoControlLayoutsTrait::IoControlParameter IoControlParameterCode>
    struct IoControlCode
    {
        ML_DELETE_DEFAULT_COPY_AND_MOVE( IoControlCode );

        typename InputOutputType::Input  m_Input;
        typename InputOutputType::Output m_Output;

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns input / output data size.
        /// @return input / output data size.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE uint32_t GetInputSize() const
        {
            return sizeof( IoControlCode<InputOutputType, IoControlParameterCode> );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns a pointer to input / output data.
        /// @return input / output data pointer.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE typename InputOutputType::Input GetInput()
        {
            return m_Input;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief IoControl constructor.
        //////////////////////////////////////////////////////////////////////////
        IoControlCode()
            : m_Input{}
            , m_Output{}
        {
            m_Input = IoControlParameterCode;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for IoControlCodes object contains all escape codes.
    //////////////////////////////////////////////////////////////////////////
    struct IoControlCodes
    {
    };

    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for IoCIoControlTraitontrol object that sends escape codes.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct IoControlTrait
    {
        ML_DELETE_DEFAULT_COPY_AND_MOVE( IoControlTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        int32_t m_DrmFileDescriptor; // Drm file descriptor, used mostly for sending io controls.
        int32_t m_DrmCardNumber;     // Used for SysFs reads / writes.

        //////////////////////////////////////////////////////////////////////////
        /// @brief IoControlTrait constructor.
        //////////////////////////////////////////////////////////////////////////
        IoControlTrait()
            : m_DrmFileDescriptor( T::ConstantsOs::Drm::m_Invalid )
            , m_DrmCardNumber( T::ConstantsOs::Drm::m_Invalid )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief IoControlTrait destructor.
        //////////////////////////////////////////////////////////////////////////
        ~IoControlTrait()
        {
            CloseDrm();
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
        /// @brief  Initializes driver escape codes.
        /// @param  clientData  initializing client data.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode Initialize( const ClientData_1_0& clientData )
        {
            ML_FUNCTION_LOG( StatusCode::Success );
            ML_ASSERT( m_DrmFileDescriptor == T::ConstantsOs::Drm::m_Invalid );
            ML_ASSERT( m_DrmCardNumber == T::ConstantsOs::Drm::m_Invalid );

            if( ML_FAIL( OpenDrm() ) )
            {
                log.Error( "Failed to open drm render node" );
                return log.m_Result = StatusCode::Failed;
            }

            if( ML_FAIL( GetDrmCardNumber() ) )
            {
                CloseDrm();
                log.Error( "Failed to get drm card information" );
                return log.m_Result = StatusCode::Failed;
            }

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns parameter from the kernel.
        /// @param  parameter   parameter type.
        /// @return value       output value.
        /// @return             operation status.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE StatusCode GetParameter(
            const uint32_t& parameter,
            int32_t&        value ) const
        {
            drm_i915_getparam_t parameters = {};
            parameters.param               = parameter;
            parameters.value               = &value;

            return Send( DRM_IOCTL_I915_GETPARAM, parameters ) > T::ConstantsOs::Drm::m_Invalid
                ? StatusCode::Success
                : StatusCode::Failed;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Sends io control escape code.
        /// @param  request request type.
        /// @return data    obtained data.
        /// @return         operation status.
        //////////////////////////////////////////////////////////////////////////
        template <typename Data>
        ML_INLINE int32_t Send(
            const uint32_t& request,
            Data&           data ) const
        {
            ML_FUNCTION_LOG( int32_t{ 0 } );
            ML_ASSERT( m_DrmFileDescriptor != T::ConstantsOs::Drm::m_Invalid );

            log.m_Result = drmIoctl( m_DrmFileDescriptor, request, &data );
            log.Debug( "Io control errno", errno );

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

            m_DrmFileDescriptor = drmOpenWithType( T::ConstantsOs::Drm::m_Name, NULL, DRM_NODE_RENDER );

            if( m_DrmFileDescriptor < 0 )
            {
                m_DrmFileDescriptor = drmOpenWithType( T::ConstantsOs::Drm::m_Name, NULL, DRM_NODE_PRIMARY );
            }

            if( m_DrmFileDescriptor < 0 )
            {
                log.Error( "Failed to get drm card info (errno)", errno );
                return log.m_Result = StatusCode::Failed;
            }

            log.Debug( "Drm device opened", m_DrmFileDescriptor );

            return log.m_Result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Closes drm interface.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void CloseDrm()
        {
            ML_FUNCTION_LOG( StatusCode::Success );

            if( m_DrmFileDescriptor >= 0 )
            {
                drmClose( m_DrmFileDescriptor );
                m_DrmFileDescriptor = T::ConstantsOs::Drm::m_Invalid;
            }

            m_DrmCardNumber = T::ConstantsOs::Drm::m_Invalid;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns intel drm directory path.
        /// @return directory path.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE std::string GetDrmDirectoryPath() const
        {
            ML_FUNCTION_LOG( StatusCode::Success );
            ML_ASSERT( m_DrmFileDescriptor != T::ConstantsOs::Drm::m_Invalid );

            struct stat             fileInfo = {};
            Constants::String::Path path     = {};

            if( fstat( m_DrmFileDescriptor, &fileInfo ) )
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
                    m_DrmCardNumber = strtoull( entry->d_name + 4, nullptr, 10 );
                    log.Debug( "DRM card number is ", m_DrmCardNumber );
                    closedir( drmDirectory );
                    return log.m_Result = StatusCode::Success;
                }
            }

            closedir( drmDirectory );
            return log.m_Result;
        }
    };
} // namespace ML
