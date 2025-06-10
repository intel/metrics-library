/*========================== begin_copyright_notice ============================

Copyright (C) 2021-2025 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_example_gpu.h

@brief Example file for creating a new trait that is gpu specific.
       Make sure to change DdiObject template arguments and update description.
*/

#pragma once

namespace ML::BASE
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for ExampleGpuTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct ExampleGpuTrait : DdiObject<T, TT::ExampleGpu, ExampleGpuHandle, ObjectType::ExampleGpu>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( ExampleGpuTrait );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( ExampleGpuTrait );
    };
} // namespace ML::BASE

namespace ML::XE_LP
{
    template <typename T>
    struct ExampleGpuTrait : BASE::ExampleGpuTrait<T>
    {
        ML_DECLARE_TRAIT( ExampleGpuTrait, BASE );
    };
} // namespace ML::XE_LP

namespace ML::XE_HPG
{
    template <typename T>
    struct ExampleGpuTrait : XE_LP::ExampleGpuTrait<T>
    {
        ML_DECLARE_TRAIT( ExampleGpuTrait, XE_LP );
    };
} // namespace ML::XE_HPG

namespace ML::XE_HPC
{
    template <typename T>
    struct ExampleGpuTrait : XE_HPG::ExampleGpuTrait<T>
    {
        ML_DECLARE_TRAIT( ExampleGpuTrait, XE_HPG );
    };
} // namespace ML::XE_HPC

namespace ML::XE2_HPG
{
    template <typename T>
    struct ExampleGpuTrait : XE_HPG::ExampleGpuTrait<T>
    {
        ML_DECLARE_TRAIT( ExampleGpuTrait, XE_HPG );
    };
} // namespace ML::XE2_HPG

namespace ML::XE3
{
    template <typename T>
    struct ExampleGpuTrait : XE2_HPG::ExampleGpuTrait<T>
    {
        ML_DECLARE_TRAIT( ExampleGpuTrait, XE2_HPG );
    };
} // namespace ML::XE3
