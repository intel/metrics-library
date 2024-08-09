/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2024 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_library_configuration.h

@brief Initializes iu. Allows to override gen or api.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Metrics Library configuration.
    //////////////////////////////////////////////////////////////////////////
    struct LibraryConfiguration
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Initializes log settings.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void InitializeLogger()
        {
            IuLogGetSettings( nullptr );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Overrides api and gen settings provided by client.
        /// @return clientType     overridden client type.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void OverrideClientType( ClientType_1_0& clientType )
        {
            GetApiOverride( clientType.Api );
            GetGpuOverride( clientType.Gen );
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns overridden client api type.
        /// @return overridden client api type.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void GetApiOverride( ClientApi& clientApi )
        {
            ClientApi api = ClientApi::Unknown;

            const bool validApiVariable = ML_SUCCESS( ToolsOsTrait<ML_TRAITS_DUMMY>::GetSystemVariable( Constants::Library::m_ApiOverride, api ) );
            const bool validApi         = ( api > ClientApi::Unknown && api < ClientApi::Last );

            if( validApiVariable && validApi )
            {
                clientApi = api;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns overridden gpu type.
        /// @return overridden gen type.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void GetGpuOverride( ClientGen& clientGpu )
        {
            ClientGen gpu = ClientGen::Unknown;

            const bool validGpuVariable = ML_SUCCESS( ToolsOsTrait<ML_TRAITS_DUMMY>::GetSystemVariable( Constants::Library::m_GpuOverride, gpu ) );
            const bool validGpu         = ( gpu > ClientGen::Unknown && gpu < ClientGen::Last );

            if( validGpuVariable && validGpu )
            {
                clientGpu = gpu;
            }
        }
    };
} // namespace ML
