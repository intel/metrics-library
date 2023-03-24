/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2023 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_example.h

@brief Example file for creating a new trait. Make sure to change DdiObject
       template arguments and update description.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for ExampleTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ExampleTrait : DdiObject<T, TT::Example, ExampleHandle, ObjectType::Example>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ExampleTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ExampleTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            ML_STATIC_ASSERT_ALWAYS( "Add trait's description" );
            return "ExampleTrait<Traits>";
        }
    };
} // namespace ML
