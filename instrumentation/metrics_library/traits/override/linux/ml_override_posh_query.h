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

@file ml_override_posh_query.h

@brief Enables posh query override.

\******************************************************************************/

#pragma once

namespace ML
{
    namespace BASE
    {
        //////////////////////////////////////////////////////////////////////////
        /// @brief Base type for OverridePoshQueryTrait object.
        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        struct OverridePoshQueryTrait : DdiObject<T, TT::Overrides::PoshQuery, OverrideHandle_1_0, ObjectType::OverridePoshQuery>
        {
            ML_DELETE_DEFAULT_CONSTRUCTOR( OverridePoshQueryTrait );
            ML_DELETE_DEFAULT_COPY_AND_MOVE( OverridePoshQueryTrait );

            //////////////////////////////////////////////////////////////////////////
            /// @brief Types.
            //////////////////////////////////////////////////////////////////////////
            using Base = DdiObject<T, TT::Overrides::PoshQuery, OverrideHandle_1_0, ObjectType::OverridePoshQuery>;
            using Base::IsValid;
            using Base::FromHandle;
            using Base::Allocate;
            using Base::Delete;
            using Base::Derived;

            //////////////////////////////////////////////////////////////////////////
            /// @brief Members.
            //////////////////////////////////////////////////////////////////////////
            TT::Context& m_Context;

            //////////////////////////////////////////////////////////////////////////
            /// @brief Override posh query constructor.
            /// @param context library context.
            //////////////////////////////////////////////////////////////////////////
            OverridePoshQueryTrait( TT::Context& context )
                : Base( context.m_ClientType )
                , m_Context( context )
            {
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns description about itself.
            /// @return trait name used in library's code.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static const std::string GetDescription()
            {
                return "OverridePoshQueryTrait<Traits> (Windows)";
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Creates posh query override.
            /// @param  context library context.
            /// @return handle  override handle.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static StatusCode Create(
                TT::Context&        context,
                OverrideHandle_1_0& handle )
            {
                auto override = Allocate( context );

                if( override )
                {
                    handle = override->GetHandle();
                }

                return override
                    ? StatusCode::Success
                    : StatusCode::Failed;
            }

            //////////////////////////////////////////////////////////////////////////
            /// @brief  Returns state of user overrides.
            /// @return getData data requested by client.
            /// @return         operation status.
            //////////////////////////////////////////////////////////////////////////
            ML_INLINE static StatusCode GetData( GetReportOverride_1_0& getData )
            {
                ML_FUNCTION_LOG( StatusCode::Success );
                ML_FUNCTION_CHECK( getData.Data != nullptr );
                ML_FUNCTION_CHECK( getData.DataSize == sizeof( TT::Layouts::Override::PoshQuery ) );
                ML_FUNCTION_CHECK( IsValid( getData.Handle ) );

                auto& report     = *static_cast<TT::Layouts::Override::PoshQuery*>( getData.Data );
                report.m_Enabled = false;

                return log.m_Result;
            }
        };
    } // namespace BASE

    namespace GEN9
    {
        template <typename T>
        struct OverridePoshQueryTrait : BASE::OverridePoshQueryTrait<T>
        {
            ML_DECLARE_TRAIT( OverridePoshQueryTrait, BASE );
        };
    } // namespace GEN9

    namespace GEN11
    {
        template <typename T>
        struct OverridePoshQueryTrait : GEN9::OverridePoshQueryTrait<T>
        {
            ML_DECLARE_TRAIT( OverridePoshQueryTrait, GEN9 );
        };
    } // namespace GEN11

    namespace GEN12
    {
        template <typename T>
        struct OverridePoshQueryTrait : GEN11::OverridePoshQueryTrait<T>
        {
            ML_DECLARE_TRAIT( OverridePoshQueryTrait, GEN11 );
        };
    } // namespace GEN12
} // namespace ML
