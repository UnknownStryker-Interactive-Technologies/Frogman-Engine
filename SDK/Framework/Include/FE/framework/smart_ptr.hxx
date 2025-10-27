#ifndef _FE_FRAMEWORK_SMART_PTR_HXX_
#define _FE_FRAMEWORK_SMART_PTR_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>

#include <memory_resource>
#include <type_traits>
#include <utility>




BEGIN_NAMESPACE(FE)


namespace internal::smart_ptr
{
    struct metadata
    {
        std::pmr::memory_resource* const _resource;
		FE::uint32 _sizeofT;
        std::atomic_int16_t _observer_count;
        std::atomic_bool _is_expired;
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
    friend class smart_ptr;
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
        new(m_metadata) internal::smart_ptr::metadata(resource_p, sizeof(T), 0, false);

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
        FE_ASSERT(m_metadata->_observer_count.load(std::memory_order_acquire) >= 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);
        FE_ASSERT(m_metadata->_is_expired.load(std::memory_order_acquire) == false);

        m_metadata->_is_expired.store(true, std::memory_order_release); // Mark the object as expired.

		// Call the destructor of Archetype.
        m_ptr->~T();
        m_metadata->_resource->deallocate(m_ptr, m_metadata->_sizeofT); // Deallocate the entity.

		// Nobody is observing this object, so we can safely deallocate it.
        if (m_metadata->_observer_count.load(std::memory_order_acquire) == 0)
        {
            // Deallocate the metadata instance.
            m_metadata->_resource->deallocate( m_metadata, sizeof(internal::smart_ptr::metadata) );
        }
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
        :   m_ptr(other_p.m_ptr ),
            m_metadata(other_p.m_metadata )
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: Child must be derived from T.");
        other_p.m_ptr = nullptr;
		other_p.m_metadata = nullptr;
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
        m_ptr = other_p.m_ptr;
        m_metadata = other_p.m_metadata;
		m_ptr = nullptr;
		m_metadata = nullptr;
        return *this;
    }

    void reset() noexcept
    {
        if (m_ptr == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count.load(std::memory_order_acquire) >= 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);
        FE_ASSERT(m_metadata->_is_expired.load(std::memory_order_acquire) == false);

        m_metadata->_is_expired.store(true, std::memory_order_release); // Mark the object as expired.

        // Call the destructor of Archetype.
        m_ptr->~T();
        m_metadata->_resource->deallocate(m_ptr, m_metadata->_sizeofT); // Deallocate the Archetype instance.
        m_ptr = nullptr;

        // Nobody is observing this object, so we can safely deallocate it.
        if (m_metadata->_observer_count.load(std::memory_order_acquire) == 0)
        {
            m_metadata->_resource->deallocate(m_metadata, sizeof(internal::smart_ptr::metadata));
        }
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
    
	_FE_FORCE_INLINE_ FE::boolean operator==(std::nullptr_t) const noexcept
	{
		return (m_ptr == nullptr);
	}

	_FE_FORCE_INLINE_ FE::boolean operator!=(std::nullptr_t) const noexcept
	{
		return (m_ptr != nullptr);
	}

    _FE_FORCE_INLINE_ FE::uint64 observer_count() const noexcept
    {
        if (m_metadata == nullptr)
        {
            return 0;
        }
        return m_metadata->_observer_count.load(std::memory_order_acquire);
    }

    _FE_FORCE_INLINE_ FE::boolean is_unreachable() const noexcept
    {
        FE_ASSERT(m_metadata != nullptr);
        return m_metadata->_is_unreachable.load(std::memory_order_acquire);
	}
};

class archetype_base;

template <typename T>
class smart_ptr<T, RefType::_Observer>
{
	static_assert(std::is_array_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to an array.");
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a const type variable.");
   
    friend class archetype_base;
    friend class smart_ptr;

public:
    using element_type = T;
    using pointer = T*;
    using const_pointer = const T*;

private:
    std::atomic<pointer> m_ptr;
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
        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count.load(std::memory_order_acquire) >= 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);
        FE_ASSERT(m_metadata->_is_expired.load(std::memory_order_acquire) == false);

        m_metadata->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
    }

    template<class Child>
    smart_ptr(const smart_ptr<Child, RefType::_Owner>& target_p) noexcept
        :   m_ptr(reinterpret_cast<const smart_ptr<T, RefType::_Owner>&>(target_p).m_ptr),
		    m_metadata(reinterpret_cast<const smart_ptr<T, RefType::_Owner>&>(target_p).m_metadata)
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count.load(std::memory_order_acquire) >= 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);
        FE_ASSERT(m_metadata->_is_expired.load(std::memory_order_acquire) == false);

        m_metadata->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
    }

    smart_ptr& operator=(const smart_ptr<T, RefType::_Owner>& target_p) noexcept
    {
        if (target_p.m_ptr == nullptr)
        {
            return *this;
        }
        FE_ASSERT(target_p.m_metadata != nullptr);
        FE_ASSERT(target_p.m_metadata->_observer_count.load(std::memory_order_acquire) >= 0);
        FE_ASSERT(target_p.m_metadata->_resource != nullptr);
        FE_ASSERT(target_p.m_metadata->_sizeofT >= 0);
        FE_ASSERT(target_p.m_metadata->_is_expired.load(std::memory_order_acquire) == false);

        reset();
        m_ptr.store(target_p.m_ptr, std::memory_order_release);
		m_metadata = target_p.m_metadata;
        
        m_metadata->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
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
        FE_ASSERT(l_target.m_metadata->_observer_count.load(std::memory_order_acquire) >= 0);
        FE_ASSERT(l_target.m_metadata->_resource != nullptr);
        FE_ASSERT(l_target.m_metadata->_sizeofT >= 0);
        FE_ASSERT(l_target.m_metadata->_is_expired.load(std::memory_order_acquire) == false);

        reset();
        m_ptr.store(l_target.m_ptr, std::memory_order_release);
        m_metadata = l_target.m_metadata;

        m_metadata->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
        return *this;
    }

    ~smart_ptr() noexcept
    {
        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count.load(std::memory_order_acquire) > 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);

        m_metadata->_observer_count.fetch_sub(1, std::memory_order_acq_rel); // Decrement the observer count

        if (m_metadata->_is_expired.load(std::memory_order_acquire) == true)
        {
            // Nobody is observing this object, so we can safely deallocate it
            if (m_metadata->_observer_count.load(std::memory_order_acquire) == 0)
            {
                m_metadata->_resource->deallocate(m_metadata, sizeof(internal::smart_ptr::metadata));
            }
        }
    }

    smart_ptr(const smart_ptr& other_p) noexcept
        :   m_ptr(other_p.m_ptr.load(std::memory_order_acquire)),
		    m_metadata(other_p.m_metadata)
    {
        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count.load(std::memory_order_acquire) > 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);

        m_metadata->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count
    }

    template<class Child>
    smart_ptr(const smart_ptr<Child, FE::RefType::_Observer>& other_p) noexcept
        :   m_ptr(other_p.m_ptr.load(std::memory_order_acquire)),
            m_metadata(other_p.m_metadata)
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count.load(std::memory_order_acquire) > 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);

        m_metadata->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count
    }

    smart_ptr& operator=(const smart_ptr& other_p) noexcept
    {
        if ((other_p.m_ptr.load(std::memory_order_acquire) == nullptr) || (other_p.m_ptr.load(std::memory_order_acquire) == m_ptr.load(std::memory_order_acquire)))
        {
            return *this;
        }
        FE_ASSERT(other_p.m_metadata != nullptr);
        FE_ASSERT(other_p.m_metadata->_observer_count.load(std::memory_order_acquire) > 0);
        FE_ASSERT(other_p.m_metadata->_resource != nullptr);
        FE_ASSERT(other_p.m_metadata->_sizeofT >= 0);

        reset();
        m_ptr.store(other_p.m_ptr.load(std::memory_order_acquire), std::memory_order_release);
        m_metadata = other_p.m_metadata;

        m_metadata->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
        return *this;
    }

    template<class Child>
    smart_ptr& operator=(const smart_ptr<Child, FE::RefType::_Observer>& other_p) noexcept
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        if ((other_p.m_ptr.load(std::memory_order_acquire) == nullptr) || (other_p.m_ptr.load(std::memory_order_acquire) == m_ptr.load(std::memory_order_acquire)))
        {
            return *this;
        }
        FE_ASSERT(other_p.m_metadata != nullptr);
        FE_ASSERT(other_p.m_metadata->_observer_count.load(std::memory_order_acquire) > 0);
        FE_ASSERT(other_p.m_metadata->_resource != nullptr);
        FE_ASSERT(other_p.m_metadata->_sizeofT >= 0);

        reset();
        m_ptr.store(other_p.m_ptr, std::memory_order_release);
        m_metadata = other_p.m_metadata;

        m_metadata->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
        return *this;
    }

    smart_ptr(smart_ptr&& other_p) noexcept
        :   m_ptr(other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel)),
            m_metadata(other_p.m_metadata)
    {
		other_p.m_metadata = nullptr;
    }

    template<class Child>
    smart_ptr(smart_ptr<Child, FE::RefType::_Observer>&& other_p) noexcept
        :   m_ptr(other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel)),
            m_metadata(other_p.m_metadata)
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");
        other_p.m_metadata = nullptr;
    }

    smart_ptr& operator=(smart_ptr&& other_p) noexcept
    {
        m_ptr = other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel);
		m_metadata = other_p.m_metadata;
		other_p.m_metadata = nullptr;
        return *this;
    }

    template<class Child>
    smart_ptr& operator=(smart_ptr<Child, FE::RefType::_Observer>&& other_p) noexcept
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        m_ptr = other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel);
        m_metadata = other_p.m_metadata;
        other_p.m_metadata = nullptr;
        return *this;
    }

    void reset() noexcept
    {
        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_metadata != nullptr);
        FE_ASSERT(m_metadata->_observer_count.load(std::memory_order_acquire) > 0);
        FE_ASSERT(m_metadata->_resource != nullptr);
        FE_ASSERT(m_metadata->_sizeofT >= 0);

        m_metadata->_observer_count.fetch_sub(1, std::memory_order_acq_rel); // Decrement the observer count

        if (m_metadata->_is_expired.load(std::memory_order_acquire) == true)
        {
            // Nobody is observing this object, so we can safely deallocate it
            if (m_metadata->_observer_count.load(std::memory_order_acquire) == 0)
            {
                m_metadata->_resource->deallocate(m_metadata, sizeof(internal::smart_ptr::metadata));
            }
        }
        m_ptr.store(nullptr, std::memory_order_release);
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
        FE_ASSERT(m_metadata->_is_expired.load(std::memory_order_acquire) == false);
        return m_ptr;
    }

    _FE_FORCE_INLINE_ const T* operator->() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_metadata->_is_expired.load(std::memory_order_acquire) == false);
        return m_ptr;
    }

    _FE_FORCE_INLINE_ T& operator*() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_metadata->_is_expired.load(std::memory_order_acquire) == false);
        return *m_ptr;
    }

    _FE_FORCE_INLINE_ const T& operator*() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_metadata->_is_expired.load(std::memory_order_acquire) == false);
        return *m_ptr;
    }

    _FE_FORCE_INLINE_ FE::boolean is_valid() const noexcept
    {
        return ((m_ptr.load(std::memory_order_acquire) != nullptr) && (m_metadata->_is_expired.load(std::memory_order_acquire) == false));
    }

    _FE_FORCE_INLINE_ FE::uint64 observer_count() const noexcept
    {
        if (m_metadata == nullptr)
        {
            return 0;
        }
        return m_metadata->_observer_count.load(std::memory_order_acquire);
	}

    _FE_FORCE_INLINE_ void set_unreachable() noexcept
    {
        FE_ASSERT(m_metadata != nullptr);
        return m_metadata->_is_unreachable.store(true, std::memory_order_release);
	}
};

template <typename T, typename... Arguments>
_FE_FORCE_INLINE_ smart_ptr<std::remove_all_extents_t<T>, RefType::_Owner> make_owner(std::pmr::memory_resource* resource_p = std::pmr::get_default_resource(), Arguments&&... arguments_p) noexcept
{
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a const type variable.");
    return smart_ptr<std::remove_all_extents_t<T>, RefType::_Owner>(resource_p, std::forward<Arguments>(arguments_p)...);
}

template <class Child, class Parent>
_FE_FORCE_INLINE_ FE::smart_ptr<Child, FE::RefType::_Observer> downcast_owner_to_observer(FE::smart_ptr<Parent, FE::RefType::_Owner>&& other_p) noexcept
{
    static_assert(std::is_base_of_v<Parent, Child>, "Static assertion failed: Parent must be the base class of Child.");
    
    FE::smart_ptr<Child, FE::RefType::_Observer> l_result = reinterpret_cast< FE::smart_ptr<Child, FE::RefType::_Owner>&& >(other_p);
    return l_result;
}

template <class Child, class Parent>
_FE_FORCE_INLINE_ FE::smart_ptr<Child, FE::RefType::_Observer> downcast_owner_to_observer(FE::smart_ptr<Parent, FE::RefType::_Owner>& other_p) noexcept
{
    static_assert(std::is_base_of_v<Parent, Child>, "Static assertion failed: Parent must be the base class of Child.");

    FE::smart_ptr<Child, FE::RefType::_Observer> l_result = reinterpret_cast<FE::smart_ptr<Child, FE::RefType::_Owner>&>(other_p);
    return l_result;
}

template <class Child, class Parent>
_FE_FORCE_INLINE_ FE::smart_ptr<Child, FE::RefType::_Observer> downcast_observer(const FE::smart_ptr<Parent, FE::RefType::_Observer>& other_p) noexcept
{
    static_assert(std::is_base_of_v<Parent, Child>, "Static assertion failed: Parent must be the base class of Child.");
    FE::smart_ptr<Child, FE::RefType::_Observer> l_result = reinterpret_cast<const FE::smart_ptr<Child, FE::RefType::_Observer>&>(other_p);
    return l_result;
}

template <class Parent, class Child>
_FE_FORCE_INLINE_ FE::smart_ptr<Parent, FE::RefType::_Observer> upcast_observer(const FE::smart_ptr<Child, FE::RefType::_Observer>& other_p) noexcept
{
    static_assert(std::is_base_of_v<Parent, Child>, "Static assertion failed: Parent must be the base class of Child.");
    FE::smart_ptr<Parent, FE::RefType::_Observer> l_result = reinterpret_cast<const FE::smart_ptr<Parent, FE::RefType::_Observer>&>(other_p);
    return l_result;
}


template <typename T>
struct is_observer_smart_ptr : std::false_type {};

template <typename T>
struct is_observer_smart_ptr< FE::smart_ptr<T, FE::RefType::_Observer> > : std::true_type {};

template<typename T>
_FE_MAYBE_UNUSED_ constexpr inline bool is_observer_smart_ptr_v = is_observer_smart_ptr<T>::value;


template <typename T>
struct is_owner_smart_ptr : std::false_type {};

template <typename T>
struct is_owner_smart_ptr< FE::smart_ptr<T, FE::RefType::_Owner> > : std::true_type {};

template<typename T>
_FE_MAYBE_UNUSED_ constexpr inline bool is_owner_smart_ptr_v = is_owner_smart_ptr<T>::value;


template <typename T>
struct is_smart_ptr
{
    _FE_MAYBE_UNUSED_ constexpr inline static bool value = is_owner_smart_ptr_v<T> || is_observer_smart_ptr_v<T>;
};

template<typename T>
_FE_MAYBE_UNUSED_ constexpr inline bool is_smart_ptr_v = is_smart_ptr<T>::value;


END_NAMESPACE
#endif
