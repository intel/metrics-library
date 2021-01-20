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

@file ml_object.h

@brief Base type for all Metrics Library's objects.

\******************************************************************************/

#pragma once

namespace ML
{
    //////////////////////////////////////////////////////////////////////////
    /// @brief Base object type.
    //////////////////////////////////////////////////////////////////////////
    struct BaseObject
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( BaseObject );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( BaseObject );

        // Object validation data. Must be the first entry.
        const uint32_t m_MagicHeader;

        // Object / client type.
        const ObjectType     m_ObjectType;
        const ClientType_1_0 m_ClientType;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base object constructor.
        /// @param objectType   object type (query hw counters, configuration, ...).
        /// @param clientType   client api type (dx9, ocl, ...).
        //////////////////////////////////////////////////////////////////////////
        BaseObject(
            const ObjectType&     objectType,
            const ClientType_1_0& clientType )
            : m_MagicHeader( Constants::Object::m_MagicHeader )
            , m_ObjectType( objectType )
            , m_ClientType{ clientType }
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Base object destructor.
        //////////////////////////////////////////////////////////////////////////
        virtual ~BaseObject()
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates a given object type.
        /// @param  type    object type.
        /// @return         true if object is valid.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static bool IsValidType( const BaseObject& object )
        {
            const bool validHeader     = object.m_MagicHeader == Constants::Object::m_MagicHeader;
            const bool validObjectType = ( object.m_ObjectType > ObjectType::Unknown ) && ( object.m_ObjectType < ObjectType::Last );

            return validHeader && validObjectType;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns object type for a given handle.
        /// @param  object  handle.
        /// @return         object type.
        //////////////////////////////////////////////////////////////////////////
        template <typename Handle>
        ML_INLINE static ObjectType GetType( const Handle& handle )
        {
            const BaseObject* object = static_cast<BaseObject*>( handle.data );
            const bool        valid  = object && IsValidType( *object );

            return valid
                ? object->m_ObjectType
                : ObjectType::Unknown;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns client type for a given handle.
        /// @param  object  client handle.
        /// @return         client type.
        //////////////////////////////////////////////////////////////////////////
        template <typename Handle>
        ML_INLINE static ClientType_1_0 GetClientType( const Handle& handle )
        {
            const BaseObject* object  = static_cast<BaseObject*>( handle.data );
            const bool        valid   = object && IsValidType( *object );
            ClientType_1_0    unknown = { ClientApi::Unknown, ClientGen::Unknown };

            ML_ASSERT( valid );

            return valid
                ? object->m_ClientType
                : unknown;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    /// @brief Trait object type.
    /// @param T        traits.
    /// @param Object   derived object used to cast from base class to derived.
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename Object>
    struct TraitObject
    {
        ML_DELETE_DEFAULT_COPY_AND_MOVE( TraitObject );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Trait object constructor.
        //////////////////////////////////////////////////////////////////////////
        TraitObject()
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Trait object destructor.
        //////////////////////////////////////////////////////////////////////////
        virtual ~TraitObject()
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns derived object instance.
        /// @return derived object instance.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE Object& Derived()
        {
            Object* object = T::ToolsOs::template DynamicCast<Object>( this );
            ML_ASSERT( object );
            return *object;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    /// @brief Ddi object type.
    /// @param T        traits.
    /// @param Object   derived object used to cast from base class to derived.
    /// @param Handle   api handle used to convert object to handle and vice versa.
    /// @param Type     object type to validate object type.
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename Object, typename Handle, ObjectType Type>
    struct DdiObject : BaseObject, TraitObject<T, Object>
    {
        ML_DELETE_DEFAULT_CONSTRUCTOR( DdiObject );
        ML_DELETE_DEFAULT_COPY_AND_MOVE( DdiObject );

        //////////////////////////////////////////////////////////////////////////
        /// @brief Types.
        //////////////////////////////////////////////////////////////////////////
        using BaseTraitObject = TraitObject<T, Object>;
        using BaseTraitObject::Derived;

        //////////////////////////////////////////////////////////////////////////
        /// @brief Ddi object constructor.
        /// @param clientType client api/gen information.
        //////////////////////////////////////////////////////////////////////////
        DdiObject( const ClientType_1_0& clientType )
            : BaseObject( Type, clientType )
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Ddi object destructor.
        //////////////////////////////////////////////////////////////////////////
        virtual ~DdiObject()
        {
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Allocates and constructs object instance.
        /// @param  arguments   constructor arguments.
        /// @return             new object instance.
        //////////////////////////////////////////////////////////////////////////
        template <typename... Arguments>
        ML_INLINE static Object* Allocate( Arguments&&... arguments )
        {
            Object* object = new( std::nothrow ) Object( std::forward<Arguments>( arguments )... );
            ML_ASSERT( object );
            return object;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Destroys object instance.
        /// @param object   object to delete.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static void Delete( Object*& object )
        {
            ML_ASSERT( object );
            delete object;
            object = nullptr;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief Destroys object instance for a given handle.
        /// @param handle   object to delete.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static StatusCode Delete( const Handle& handle )
        {
            const bool valid = IsValid( handle );

            if( valid )
            {
                Object* object = &Object::FromHandle( handle );
                Delete( object );
            }

            return valid
                ? StatusCode::Success
                : StatusCode::IncorrectParameter;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns object's handle.
        /// @return         object's handle.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE Handle GetHandle()
        {
            Handle handle = {};
            handle.data   = T::ToolsOs::template DynamicCast<Object>( this );

            ML_ASSERT( handle.data );
            return handle;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns a reference to object for a given handle.
        /// @param  handle object handle.
        /// @return        object instance.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static Object& FromHandle( const Handle& handle )
        {
            BaseObject* base   = static_cast<BaseObject*>( handle.data );
            Object*     object = T::ToolsOs::template DynamicCast<Object>( base );

            ML_ASSERT( object );
            ML_ASSERT( IsValidType( *object ) );
            return *object;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Returns a pointer to object for a given handle.
        /// @param  handle object handle.
        /// @return        object instance.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static Object* FromHandleUnsafe( const Handle& handle )
        {
            BaseObject* base   = static_cast<BaseObject*>( handle.data );
            Object*     object = T::ToolsOs::template DynamicCast<Object>( base );
            const bool  valid  = object && IsValidType( *object );

            return valid
                ? object
                : nullptr;
        }

        //////////////////////////////////////////////////////////////////////////
        /// @brief  Validates a given handle.
        /// @return handle  handle to validate.
        //////////////////////////////////////////////////////////////////////////
        ML_INLINE static bool IsValid( const Handle& handle )
        {
            BaseObject* base   = static_cast<BaseObject*>( handle.data );
            Object*     object = T::ToolsOs::template DynamicCast<Object>( base );

            return object && IsValidType( *object );
        }
    };
} // namespace ML
