#ifndef _FE_FRAMEWORK_SMART_PTR_HXX_
#define _FE_FRAMEWORK_SMART_PTR_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>

#include <memory_resource>
#include <type_traits>
#include <utility>




BEGIN_NAMESPACE(FE)


namespace internal::smart_ptr
{
    struct metadata
    {
        var::uint64 _observer_count;
        std::pmr::memory_resource* const _resource;
		FE::uint32 _sizeofT;
        var::boolean _is_expired;
    };
}


enum struct RefType
{
    _Owner,
    _Observer
};


template <typename T, RefType Type>
class smart_ptr;


template <typename T>
class smart_ptr<T, RefType::_Owner>
{
    friend class smart_ptr<T, RefType::_Observer>;

    static_assert(std::is_array_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to an array.");
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a const type variable.");
public:
    using element_type = T;
    using pointer = T*;
    using const_pointer = const T*;

private:
    T* m_ptr;
	internal::smart_ptr::metadata* m_metadata;

public:
    smart_ptr() noexcept
        :   m_ptr(), 
            m_metadata() 
    {}

    template <typename... Arguments>
    smart_ptr(std::pmr::memory_resource* resource_p, Arguments&&... arguments_p) noexcept
        :   m_ptr(), 
            m_metadata()
    {
        m_metadata = (internal::smart_ptr::metadata*)resource_p->allocate( sizeof(internal::smart_ptr::metadata) );
        new(m_metadata) internal::smart_ptr::metadata( 0, resource_p, sizeof(T), false );

        m_ptr = (pointer)m_metadata->_resource->allocate( m_metadata->_sizeofT );
        new(m_ptr) T( std::forward<Arguments&&>(arguments_p)... );
    }

    ~smart_ptr() noexcept
    {
        if (m_ptr == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count >= 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);
        FE_ASSERT(m_metadata->_is_expired == false);

		// Call the destructor of T.
        m_ptr->~T();

		// Nobody is observing this object, so we can safely deallocate it.
        if (m_metadata->_observer_count == 0)
        {
			// Deallocate the T instance.
            m_metadata->_resource->deallocate( m_ptr, m_metadata->_sizeofT );
            std::pmr::memory_resource* l_tmp = m_metadata->_resource;
            l_tmp->deallocate( m_metadata, sizeof(internal::smart_ptr::metadata) );
            return;
        }
        m_metadata->_is_expired = true; // Mark the object as expired, so that it can be deallocated later.
    }

    smart_ptr(const smart_ptr&) = delete;
    smart_ptr& operator=(const smart_ptr&) = delete;

    smart_ptr(smart_ptr&& other_p) noexcept
        :   m_ptr(other_p.m_ptr), 
            m_metadata(other_p.m_metadata)
    {
        other_p.m_ptr = nullptr;
		other_p.m_metadata = nullptr;
    }

    template <class Child>
    smart_ptr(smart_ptr<Child, FE::RefType::_Owner>&& other_p) noexcept
        :   m_ptr( reinterpret_cast< std::remove_pointer_t<decltype(this)>&& >(other_p).m_ptr ),
            m_metadata( reinterpret_cast< std::remove_pointer_t<decltype(this)>&& >(other_p).m_metadata )
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: Child must be derived from T.");

        std::memset(&other_p, 0, sizeof(other_p));
    }

    smart_ptr& operator=(smart_ptr&& other_p) noexcept
    {
        reset();
        m_ptr = other_p.m_ptr;
		m_metadata = other_p.m_metadata;
        other_p.m_ptr = nullptr;
		other_p.m_metadata = nullptr;
        return *this;
    }

    template <class Child>
    smart_ptr& operator=(smart_ptr<Child, FE::RefType::_Owner>&& other_p) noexcept
    {
		static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: Child must be derived from T.");

        reset();
        m_ptr = reinterpret_cast< std::remove_pointer_t<decltype(this)>&& >(other_p).m_ptr;
        m_metadata = reinterpret_cast< std::remove_pointer_t<decltype(this)>&& >(other_p).m_metadata;
        std::memset(&other_p, 0, sizeof(other_p));
        return *this;
    }

    void reset() noexcept
    {
        if (m_ptr == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count >= 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);
        FE_ASSERT(m_metadata->_is_expired == false);

        // Call the destructor of T.
        m_ptr->~T();

        // Nobody is observing this object, so we can safely deallocate it.
        if (m_metadata->_observer_count == 0)
        {
            // Deallocate the T instance.
            m_metadata->_resource->deallocate(m_ptr, m_metadata->_sizeofT);
            std::pmr::memory_resource* l_tmp = m_metadata->_resource;
            l_tmp->deallocate(m_metadata, sizeof(internal::smart_ptr::metadata));
            m_ptr = nullptr;
            m_metadata = nullptr;
            return;
        }
        m_metadata->_is_expired = true; // Mark the object as expired, so that it can be deallocated later.
        m_ptr = nullptr;
		m_metadata = nullptr;
    }

    _FE_FORCE_INLINE_ void swap(smart_ptr& other_p) noexcept
    {
        std::swap(m_ptr, other_p.m_ptr);
		std::swap(m_metadata, other_p.m_metadata);
    }

    _FE_FORCE_INLINE_ T* operator->() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return m_ptr;
    }

    _FE_FORCE_INLINE_ const T* operator->() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return m_ptr;
    }

    _FE_FORCE_INLINE_ T& operator*() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return *m_ptr;
    }

    _FE_FORCE_INLINE_ const T& operator*() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return *m_ptr;
    }
    
	_FE_FORCE_INLINE_ FE::boolean is_null() const noexcept
	{
		return (m_ptr == nullptr);
	}

    _FE_FORCE_INLINE_ FE::uint64 observer_count() const noexcept
    {
        if (m_metadata == nullptr)
        {
            return 0;
        }
        return m_metadata->_observer_count;
    }
};

template <typename T>
class smart_ptr<T, RefType::_Observer>
{
	static_assert(std::is_array_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to an array.");
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a const type variable.");
public:
    using element_type = T;
    using pointer = T*;
    using const_pointer = const T*;

private:
    T* m_ptr;
	internal::smart_ptr::metadata* m_metadata;

public:
    smart_ptr() noexcept
		:   m_ptr(), 
            m_metadata() 
    {}

    smart_ptr(const smart_ptr<T, RefType::_Owner>& target_p) noexcept
        :   m_ptr(target_p.m_ptr),
            m_metadata(target_p.m_metadata)
    {
        if (m_ptr == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count >= 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);
        FE_ASSERT(m_metadata->_is_expired == false);

		++(m_metadata->_observer_count); // Increment the observer count.
    }

    template<class Child>
    smart_ptr(const smart_ptr<Child, RefType::_Owner>& target_p) noexcept
        :   m_ptr(reinterpret_cast<const smart_ptr<T, RefType::_Owner>&>(target_p).m_ptr),
		    m_metadata(reinterpret_cast<const smart_ptr<T, RefType::_Owner>&>(target_p).m_metadata)
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");
        
        if (m_ptr == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count >= 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);
        FE_ASSERT(m_metadata->_is_expired == false);

        ++(m_metadata->_observer_count); // Increment the observer count.
    }

    smart_ptr& operator=(const smart_ptr<T, RefType::_Owner>& target_p) noexcept
    {
        if (target_p.m_ptr == nullptr)
        {
            return *this;
        }
        FE_ASSERT(target_p.m_metadata != nullptr);
        FE_ASSERT(target_p.m_metadata->_observer_count >= 0);
        FE_ASSERT(target_p.m_metadata->_resource != nullptr);
        FE_ASSERT(target_p.m_metadata->_sizeofT >= 0);
        FE_ASSERT(target_p.m_metadata->_is_expired == false);

        reset();
        m_ptr = target_p.m_ptr;
		m_metadata = target_p.m_metadata;
        
        ++(m_metadata->_observer_count); // Increment the observer count.
        return *this;
    }

    template<class Child>
    smart_ptr& operator=(const smart_ptr<Child, RefType::_Owner>& target_p) noexcept
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        const smart_ptr<T, RefType::_Owner>& l_target = reinterpret_cast<const smart_ptr<T, RefType::_Owner>&>(target_p);
        if (l_target.m_ptr == nullptr)
        {
            return *this;
        }
        FE_ASSERT(l_target.m_metadata != nullptr);
        FE_ASSERT(l_target.m_metadata->_observer_count >= 0);
        FE_ASSERT(l_target.m_metadata->_resource != nullptr);
        FE_ASSERT(l_target.m_metadata->_sizeofT >= 0);
        FE_ASSERT(l_target.m_metadata->_is_expired == false);

        reset();
        m_ptr = l_target.m_ptr;
        m_metadata = l_target.m_metadata;

        ++(m_metadata->_observer_count); // Increment the observer count.
        return *this;
    }

    ~smart_ptr() noexcept
    {
        if (m_ptr == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count > 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);

        --(m_metadata->_observer_count); // Decrement the observer count

        if (m_metadata->_is_expired == true)
        {
            // Nobody is observing this object, so we can safely deallocate it
            if (m_metadata->_observer_count == 0)
            {
                // Call the destructor of T
                m_ptr->~T();

                // Deallocate the T instance.
                m_metadata->_resource->deallocate(m_ptr, m_metadata->_sizeofT);
                std::pmr::memory_resource* l_tmp = m_metadata->_resource;
                l_tmp->deallocate(m_metadata, sizeof(internal::smart_ptr::metadata));
            }
        }
    }

    smart_ptr(const smart_ptr& other_p) noexcept
        :   m_ptr(other_p.m_ptr),
		    m_metadata(other_p.m_metadata)
    {
        if (m_ptr == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count > 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);

        ++(m_metadata->_observer_count); // Increment the observer count
    }

    template<class Child>
    smart_ptr(const smart_ptr<Child, FE::RefType::_Observer>& other_p) noexcept
        :   m_ptr(reinterpret_cast<const smart_ptr<T, RefType::_Observer>&>(other_p).m_ptr),
            m_metadata(reinterpret_cast<const smart_ptr<T, RefType::_Observer>&>(other_p).m_metadata)
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        if (m_ptr == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count > 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);

        ++(m_metadata->_observer_count); // Increment the observer count
    }

    smart_ptr& operator=(const smart_ptr& other_p) noexcept
    {
        if (other_p.m_ptr == nullptr)
        {
            return *this;
        }
        FE_ASSERT(other_p.m_metadata != nullptr);
        FE_ASSERT(other_p.m_metadata->_observer_count > 0);
        FE_ASSERT(other_p.m_metadata->_resource != nullptr);
        FE_ASSERT(other_p.m_metadata->_sizeofT >= 0);

        reset();
        m_ptr = other_p.m_ptr;
        m_metadata = other_p.m_metadata;

        ++(m_metadata->_observer_count); // Increment the observer count.
        return *this;
    }

    template<class Child>
    smart_ptr& operator=(const smart_ptr<Child, FE::RefType::_Observer>& other_p) noexcept
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        const smart_ptr<T, RefType::_Observer>& l_other = reinterpret_cast<const smart_ptr<T, RefType::_Observer>&>(other_p);
        if (l_other.m_ptr == nullptr)
        {
            return *this;
        }
        FE_ASSERT(l_other.m_metadata != nullptr);
        FE_ASSERT(l_other.m_metadata->_observer_count > 0);
        FE_ASSERT(l_other.m_metadata->_resource != nullptr);
        FE_ASSERT(l_other.m_metadata->_sizeofT >= 0);

        reset();
        m_ptr = l_other.m_ptr;
        m_metadata = l_other.m_metadata;

        ++(m_metadata->_observer_count); // Increment the observer count.
        return *this;
    }

    smart_ptr(smart_ptr&& other_p) noexcept
        :   m_ptr(other_p.m_ptr),
            m_metadata(other_p.m_metadata)
    {
		other_p.m_ptr = nullptr;
		other_p.m_metadata = nullptr;
    }

    template<class Child>
    smart_ptr(smart_ptr<Child, FE::RefType::_Observer>&& other_p) noexcept
        :   m_ptr(reinterpret_cast<smart_ptr<T, RefType::_Observer>&&>(other_p).m_ptr),
            m_metadata(reinterpret_cast<smart_ptr<T, RefType::_Observer>&&>(other_p).m_metadata)
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        smart_ptr<T, RefType::_Observer>&& l_other = reinterpret_cast<smart_ptr<T, RefType::_Observer>&&>(other_p);
        l_other.m_ptr = nullptr;
        l_other.m_metadata = nullptr;
    }

    smart_ptr& operator=(smart_ptr&& other_p) noexcept
    {
        m_ptr = other_p.m_ptr;
		m_metadata = other_p.m_metadata;
        other_p.m_ptr = nullptr;
		other_p.m_metadata = nullptr;
        return *this;
    }

    template<class Child>
    smart_ptr& operator=(smart_ptr<Child, FE::RefType::_Observer>&& other_p) noexcept
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        smart_ptr<T, RefType::_Observer>&& l_other = reinterpret_cast<smart_ptr<T, RefType::_Observer>&&>(other_p);
        m_ptr = l_other.m_ptr;
        m_metadata = l_other.m_metadata;
        l_other.m_ptr = nullptr;
        l_other.m_metadata = nullptr;
        return *this;
    }

    void reset() noexcept
    {
        if (m_ptr == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count > 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);

        --(m_metadata->_observer_count); // Decrement the observer count

        if (m_metadata->_is_expired == true)
        {
            // Nobody is observing this object, so we can safely deallocate it
            if (m_metadata->_observer_count == 0)
            {
                // Call the destructor of T
                m_ptr->~T();

                // Deallocate the T instance.
                m_metadata->_resource->deallocate(m_ptr, m_metadata->_sizeofT);
                std::pmr::memory_resource* l_tmp = m_metadata->_resource;
                l_tmp->deallocate(m_metadata, sizeof(internal::smart_ptr::metadata));
            }
        }
        m_ptr = nullptr;
		m_metadata = nullptr;
    }

    _FE_FORCE_INLINE_ void swap(smart_ptr& other_p) noexcept
    {
        std::swap(m_ptr, other_p.m_ptr);
		std::swap(m_metadata, other_p.m_metadata);
    }

    _FE_FORCE_INLINE_ T* operator->() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_metadata->_is_expired == false);
        return m_ptr;
    }

    _FE_FORCE_INLINE_ const T* operator->() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_metadata->_is_expired == false);
        return m_ptr;
    }

    _FE_FORCE_INLINE_ T& operator*() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_metadata->_is_expired == false);
        return *m_ptr;
    }

    _FE_FORCE_INLINE_ const T& operator*() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_metadata->_is_expired == false);
        return *m_ptr;
    }

    _FE_FORCE_INLINE_ FE::boolean is_valid() const noexcept
    {
        return ((m_ptr != nullptr) && (m_metadata->_is_expired == false));
    }

    _FE_FORCE_INLINE_ FE::uint64 observer_count() const noexcept
    {
        if (m_metadata == nullptr)
        {
            return 0;
        }
        return m_metadata->_observer_count;
	}
};

template <typename T, typename... Arguments>
_FE_FORCE_INLINE_ smart_ptr<std::remove_all_extents_t<T>, RefType::_Owner> gcnew(std::pmr::memory_resource* resource_p = std::pmr::get_default_resource(), Arguments&&... arguments_p) noexcept
{
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a const type variable.");
    return smart_ptr<std::remove_all_extents_t<T>, RefType::_Owner>(resource_p, std::forward<Arguments>(arguments_p)...);
}


END_NAMESPACE
#endif
