/*========================== begin_copyright_notice ============================

Copyright (C) 2021 Intel Corporation

SPDX-License-Identifier: MIT

============================= end_copyright_notice ===========================*/

/*
@file ml_ddi_objects.h

@brief Stores created ddi objects.
       Warns if there are remaining ddi objects on context deletion.
*/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base type for DdiObjectsTrait object.
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct DdiObjectsTrait
    {
        ML_DELETE_DEFAULT_COPY_AND_MOVE( DdiObjectsTrait );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Members.
        //////////////////////////////////////////////////////////////////////////
        std::unordered_set<BaseObject*> m_Objects;
        std::mutex                      m_Mutex;

        //////////////////////////////////////////////////////////////////////////
        /// @brief DdiObjectsTrait constructor.
        //////////////////////////////////////////////////////////////////////////
        DdiObjectsTrait()
            : m_Objects{}
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief DdiObjectsTrait destructor.
        //////////////////////////////////////////////////////////////////////////
        ~DdiObjectsTrait()
        {
            ValidateState();
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns description about itself.
        /// @return trait name used in library's code.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static const std::string GetDescription()
        {
            return "DdiObjectsTrait<Traits>";
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Adds ddi object to unordered set.
        /// @param object an object to be added.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void AddObject( BaseObject* object )
        {
            std::lock_guard<std::mutex> guard( m_Mutex );

            m_Objects.insert( object );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Removes ddi object from unordered set.
        /// @param object an object to be removed.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void RemoveObject( BaseObject* object )
        {
            std::lock_guard<std::mutex> guard( m_Mutex );

            m_Objects.erase( object );
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Validates undeleted objects.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE void ValidateState()
        {
            ML_FUNCTION_LOG( true );

            if( !m_Objects.empty() )
            {
                log.Warning( "Some ddi objects should be deleted, count", m_Objects.size() );
            }
        }
    };
} // namespace ML
