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

@file ml_io_control_layouts.h

@brief Layout for io controls.

\******************************************************************************/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for IoControlLayoutsTrait object.
    //////////////////////////////////////////////////////////////////////////
    struct IoControlLayoutsTrait
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( IoControlLayoutsTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( IoControlLayoutsTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief IoControlParameter enumeration.
        //////////////////////////////////////////////////////////////////////////
        enum class IoControlParameter : uint32_t
        {
            PlatformIndex = 12, // GTDI_DEVICE_PARAM_PLATFORM_INDEX
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "IoControlLayoutsTrait (Linux)";
        }
    };
} // namespace ML
