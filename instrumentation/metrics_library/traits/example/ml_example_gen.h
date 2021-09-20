/*========================== begin_copyright_notice ============================

Copyright (C) 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_example_gen.h

@brief Example file for creating a new trait that is gen specific.
       Make sure to change DdiObject template arguments and update description.
*/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for ExampleGenTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct ExampleGenTrait : DdiObject<T, TT::ExampleGen, ExampleGenHandle, ObjectType::ExampleGen>
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( ExampleGenTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( ExampleGenTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                ML_STATIC_ASSERT_ALWAYS( "Add trait's description" );
                return "ExampleGenTrait<Traits>";
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct ExampleGenTrait : BASE::ExampleGenTrait<T>
        {
            ML_DECLARE_TRAIT( ExampleGenTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct ExampleGenTrait : GEN9::ExampleGenTrait<T>
        {
            ML_DECLARE_TRAIT( ExampleGenTrait, GEN9 );
        };
    } // namespace GEN11

    namespace XE_LP
    {
        template <typename T>
        struct ExampleGenTrait : GEN11::ExampleGenTrait<T>
        {
            ML_DECLARE_TRAIT( ExampleGenTrait, GEN11 );
        };
    } // namespace XE_LP
} // namespace ML
