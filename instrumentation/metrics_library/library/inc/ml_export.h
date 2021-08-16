/*========================== begin_copyright_notice ============================

Copyright © 2020-2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_export.h

@brief Contains a definition of function, which allows to obtain Traits table
       for a given os / api / gen.
*/

#pragma once

const ML::DdiFunctionTableBase* GetFunctionTable( const ClientType_1_0& clientType );
