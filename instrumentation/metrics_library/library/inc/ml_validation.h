/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_validation.h

@brief Validates conditions.
*/

#pragma once

namespace ML::Validation
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief DummyResult structure is required by gcc. Otherwise the generic
    ///        CheckResult (below) will be compiled and the static assert will be
    ///        triggered even if generic version was not used at all.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct DummyResult : std::false_type
    {
    };

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Common validation for all types other than bool/StatusCode.
    /// @param  result  result to check.
    /// @return         always fail, only bool/StatusCode conditions are supported.
    //////////////////////////////////////////////////////////////////////////
    template <typename ResultT>
    ML_INLINE StatusCode CheckResult( const ResultT result )
    {
        static_assert( DummyResult<ResultT>::value, "Incorrect condition type (bool/StatusCode are allowed)." );
        return StatusCode::Failed;
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Validates SatutsCode type condition.
    /// @param  status  condition status.
    /// @return         just pass input to output.
    //////////////////////////////////////////////////////////////////////////
    template <>
    ML_INLINE constexpr StatusCode CheckResult<StatusCode>( const StatusCode result )
    {
        return result;
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief  Validates bool type condition.
    /// @param  result      bool type result.
    /// @param  condition   condition code.
    /// @return             bool converted to StatusCode.
    //////////////////////////////////////////////////////////////////////////
    template <>
    ML_INLINE constexpr StatusCode CheckResult<bool>( const bool result )
    {
        return result
            ? StatusCode::Success
            : StatusCode::Failed;
    }
} // namespace ML::Validation
