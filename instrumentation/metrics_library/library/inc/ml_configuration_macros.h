/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_configuration_macros.h

@brief Metrics Library configuration macros.
*/

#pragma once

//////////////////////////////////////////////////////////////////////////
/// @brief Configuration macros.
//////////////////////////////////////////////////////////////////////////
#if defined( _DEBUG )
    #define ML_DEBUG 1
#else
    #define ML_DEBUG 0
#endif

#if defined( _RELEASE_INTERNAL )
    #define ML_INTERNAL 1
#else
    #define ML_INTERNAL 0
#endif

#if !defined( _DEBUG ) && !defined( _RELEASE_INTERNAL )
    #define ML_RELEASE 1
#else
    #define ML_RELEASE 0
#endif
