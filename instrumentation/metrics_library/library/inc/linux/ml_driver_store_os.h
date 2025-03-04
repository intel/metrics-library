/*========================== begin_copyright_notice ============================

Copyright (C) 2021-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_driver_store_os.h

@brief Implements ddi redirection.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Driver store os.
    //////////////////////////////////////////////////////////////////////////
    struct DriverStoreOs
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns an address of redirected ddi using function name.
        ///         Not supported on Linux.
        /// @return Null pointer.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void* GetRedirectedDdi( const char* )
        {
            return nullptr;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Free loaded library.
        ///        Not supported on Linux.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void UnloadLibrary()
        {
        }
    };
} // namespace ML
