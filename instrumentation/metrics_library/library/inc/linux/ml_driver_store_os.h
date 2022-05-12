/*========================== begin_copyright_notice ============================

Copyright (C) 2021-2022 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_driver_store_os.h

@brief Implements ddi redirecting.
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
        /// @brief   Returns an address of redirected ddi using function name.
        /// @return  Null pointer.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void* GetRedirectedDdi( const char* functionName )
        {
            return nullptr;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief   Free loaded library.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void UnloadLibrary()
        {
        }
    };
} // namespace ML
