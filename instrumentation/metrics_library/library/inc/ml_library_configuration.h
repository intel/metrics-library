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

@file ml_library_configuration.h

@brief Initializes iu. Allows to override gen or api.

\******************************************************************************/

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
        static void IntializeLogger()
        {
            IuLogGetSettings( IU_ADAPTER_ID_DEFAULT );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief   Overrides api and gen settings provided by client.
        /// @return  clientType     overridden client type.
        //////////////////////////////////////////////////////////////////////////
        static void OverrideClientType( ClientType_1_0& clientType )
        {
            GetApiOverride( clientType.Api );
            GetGenOverride( clientType.Gen );
        }

    private:
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns overridden client api type.
        /// @return overridden client api type.
        //////////////////////////////////////////////////////////////////////////
        static void GetApiOverride( ClientApi& clientApi )
        {
            ClientApi api = ClientApi::Unknown;

            const bool validApiVariable = ML_SUCCESS( BASE::ToolsOsTrait<ML_TRAITS_DUMMY>::GetSystemVariable( Constants::Library::m_ApiOverride, api ) );
            const bool validApi         = api < ClientApi::Last;

            if( validApiVariable && validApi )
            {
                clientApi = api;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns overridden gen type.
        /// @return overridden gen type.
        //////////////////////////////////////////////////////////////////////////
        static void GetGenOverride( ClientGen& clientGen )
        {
            ClientGen gen = ClientGen::Unknown;

            const bool validGenVariable = ML_SUCCESS( BASE::ToolsOsTrait<ML_TRAITS_DUMMY>::GetSystemVariable( Constants::Library::m_GenOverride, gen ) );
            const bool validGen         = gen < ClientGen::Last;

            if( validGenVariable && validGen )
            {
                clientGen = gen;
            }
        }
    };
} // namespace ML
