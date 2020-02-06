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

@file ml_validation.h

@brief Validates conditions.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace Validation
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief   DummyResult structure is required by gcc. Otherwise the generic
        ///          CheckResult (below) will be compiled and the static assert will be
        ///          triggered even if generic version was not used at all.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct DummyResult : std::false_type
        {
        };

        //////////////////////////////////////////////////////////////////////////
        /// @brief   Common validation for all types other than bool/StatusCode.
        /// @param   result      result to check.
        /// @return              always fail, only bool/StatusCode conditions are supported.
        //////////////////////////////////////////////////////////////////////////
        template <typename ResultT>
        ML_INLINE StatusCode CheckResult(
            const ResultT result )
        {
            static_assert( DummyResult<ResultT>::value, "Incorrect condition type (bool/StatusCode are allowed)." );
            return StatusCode::Failed;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief   Validates SatutsCode type condition.
        /// @param   status    condition status.
        /// @return            just pass input to output.
        //////////////////////////////////////////////////////////////////////////
        template <>
        ML_INLINE constexpr StatusCode CheckResult<StatusCode>(
            const StatusCode result )
        {
            return result;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief   Validates bool type condition.
        /// @param   result    bool type result.
        /// @param   condition condition code.
        /// @return            bool converted to StatusCode.
        //////////////////////////////////////////////////////////////////////////
        template <>
        ML_INLINE constexpr StatusCode CheckResult<bool>(
            const bool result )
        {
            return result
                ? StatusCode::Success
                : StatusCode::Failed;
        }
    } // namespace Validation
} // namespace ML
