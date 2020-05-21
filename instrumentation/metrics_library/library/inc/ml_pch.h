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

@file ml_pch.h

@brief Contains precompiled headers.

\******************************************************************************/
#pragma once

//////////////////////////////////////////////////////////////////////////
/// @brief Metrics library api header.
//////////////////////////////////////////////////////////////////////////
#include "metrics_library_api_1_0.h"
using namespace MetricsLibraryApi;

//////////////////////////////////////////////////////////////////////////
/// @brief Metrics Library common headers.
//////////////////////////////////////////////////////////////////////////
#include "ml_os.h"
#include "ml_macros_os.h"

//////////////////////////////////////////////////////////////////////////
/// @brief STL support.
//////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
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
#include "ml_configuration_api.h"
#include "ml_configuration_gen.h"
#include "ml_constants.h"
#include "ml_validation.h"
#include "ml_gpu_commands_include.h"
#include "ml_ddi.h"
#include "ml_export.h"
#include "ml_object.h"
#include "ml_traits_include.h"
#include "ml_library_configuration.h"
