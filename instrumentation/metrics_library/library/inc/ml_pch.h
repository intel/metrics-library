/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_pch.h

@brief Contains precompiled headers.
*/

#pragma once

//////////////////////////////////////////////////////////////////////////
/// @brief Metrics Library common headers.
//////////////////////////////////////////////////////////////////////////
#include "ml_os.h"
#include "ml_macros_os.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Metrics library configuration.
//////////////////////////////////////////////////////////////////////////
#include "ml_configuration_macros.h"
#include "ml_configuration_api.h"
#include "ml_configuration_gpu.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Metrics library api header.
//////////////////////////////////////////////////////////////////////////
#include "metrics_library_api_1_0.h"
using namespace MetricsLibraryApi;

//////////////////////////////////////////////////////////////////////////
/// @brief STL support.
//////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <fstream>
#include <map>
#include <memory>
#include <mutex>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//////////////////////////////////////////////////////////////////////////
/// @brief Instrumentation utilities framework headers.
//////////////////////////////////////////////////////////////////////////
#include "iu_debug.h"
#include "iu_std.h"

//////////////////////////////////////////////////////////////////////////
/// @brief Metrics library main headers.
//////////////////////////////////////////////////////////////////////////
#include "ml_macros.h"
#include "ml_constants.h"
#include "ml_validation.h"
#include "ml_gpu_commands_include.h"
#include "ml_ddi.h"
#include "ml_export.h"
#include "ml_object.h"
