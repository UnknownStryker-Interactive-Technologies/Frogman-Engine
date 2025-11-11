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
	template<typename T>
    struct metadata
    {
        T* _data;
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
    template <typename T, RefType Type>
    friend class smart_ptr;

    static_assert(std::is_array_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to an array.");
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a const type variable.");

	using control_block_type = internal::smart_ptr::metadata<T>;

public:
    using element_type = T;
    using pointer = T*;
    using const_pointer = const T*;

private:
    std::atomic<control_block_type*> m_ptr;

public:
    smart_ptr() noexcept
        :   m_ptr() 
    {}

    template <typename... Arguments>
    smart_ptr(std::pmr::memory_resource* resource_p, Arguments&&... arguments_p) noexcept
        :   m_ptr( __allocate_new_block(resource_p, std::forward<Arguments&&>(arguments_p)...) )
    {
	    FE_ASSERT(m_ptr != nullptr, "Assertion failed: smart_ptr allocation failed.");
    }

    ~smart_ptr() noexcept
    {
        if (m_ptr == nullptr)
        {
            return;
        }
        
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0);
        m_ptr.load(std::memory_order_acquire)->_is_expired.store(true, std::memory_order_release); // Mark the object as expired.

		// Nobody is observing this object, so we can safely deallocate it.
        if (m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) == 0)
        {
			__destruct_and_deallocate_all(m_ptr);
        }
    }

    smart_ptr(const smart_ptr&) = delete;
    smart_ptr& operator=(const smart_ptr&) = delete;

    smart_ptr(smart_ptr&& other_p) noexcept
        :   m_ptr(other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel))
    {}

    template <class Child>
    smart_ptr(smart_ptr<Child, FE::RefType::_Owner>&& other_p) noexcept
        :   m_ptr( reinterpret_cast<control_block_type*>(other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel)) )
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: Child must be derived from T.");
    }

    smart_ptr& operator=(smart_ptr&& other_p) noexcept
    {
        reset();
        m_ptr = other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel);
        return *this;
    }

    template <class Child>
    smart_ptr& operator=(smart_ptr<Child, FE::RefType::_Owner>&& other_p) noexcept
    {
		static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: Child must be derived from T.");

        reset();
        m_ptr = reinterpret_cast<control_block_type*>(other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel));
        return *this;
    }

    void reset() noexcept
    {
        if (m_ptr == nullptr)
        {
            return;
        }
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");

        m_ptr.load(std::memory_order_acquire)->_is_expired.store(true, std::memory_order_release); // Mark the object as expired.
		__destruct_T(m_ptr); // We got watchers, so destruct T only.

        // Nobody is observing this object, so we can safely deallocate it.
        if (m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) == 0)
        {
            __destruct_and_deallocate_all(m_ptr);
        }
		m_ptr.store(nullptr, std::memory_order_release);
    }

    _FE_FORCE_INLINE_ void swap(smart_ptr& other_p) noexcept
    {
        other_p.m_ptr.store( m_ptr.exchange( other_p.m_ptr.load(std::memory_order_acquire), std::memory_order_acq_rel ), std::memory_order_release ); // this op is not atomic, but helps other threads to view on time.
    }

    _FE_FORCE_INLINE_ T* operator->() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return m_ptr.load(std::memory_order_acquire)->_data;
    }

    _FE_FORCE_INLINE_ const T* operator->() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return m_ptr.load(std::memory_order_acquire)->_data;
    }

    _FE_FORCE_INLINE_ T& operator*() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return *(m_ptr.load(std::memory_order_acquire)->_data);
    }

    _FE_FORCE_INLINE_ const T& operator*() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return *(m_ptr.load(std::memory_order_acquire)->_data);
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
        if (m_ptr == nullptr)
        {
            return 0;
        }
        return m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire);
    }

private:
    template <typename... Arguments>
    control_block_type* _FE_VECTOR_CALL_ __allocate_new_block(std::pmr::memory_resource* const resource_p, Arguments&&... arguments_p) noexcept
    {
		FE_ASSERT(resource_p != nullptr, "Assertion failed: cannot create a new smart_ptr data block with a null memory_resource.");
		FE::size l_total_size_in_bytes = sizeof(T) + sizeof(control_block_type);
		 void* const l_alloc_result = resource_p->allocate(l_total_size_in_bytes);

		::new(l_alloc_result) T( std::forward<Arguments>(arguments_p)... );
        control_block_type* const l_control_block = reinterpret_cast<control_block_type*>( (var::byte*)l_alloc_result + sizeof(T) );

		::new(l_control_block) control_block_type(static_cast<T* const>(l_alloc_result), // _data
                                                resource_p, // _resource
                                                sizeof(T), // _sizeofT
                                                0, // _observer_count
                                                false // _is_expired
        );
		return l_control_block;
    }

    void __destruct_and_deallocate_all(control_block_type* const control_block_p) noexcept
    {
        if (control_block_p == nullptr)
        {
            return;
        }

        std::pmr::memory_resource* const l_resource = control_block_p->_resource;
		FE::size l_total_size_in_bytes = control_block_p->_sizeofT + sizeof(control_block_type);
        if (control_block_p->_is_expired.load(std::memory_order_acquire) == false)
        {
            control_block_p->_data->~T();
        }
        control_block_p->~control_block_type();
		l_resource->deallocate(control_block_p->_data, l_total_size_in_bytes);
    }

    void __destruct_T(control_block_type* const control_block_p) noexcept
    {
        if (control_block_p == nullptr)
        {
            return;
        }
		control_block_p->_data->~T();
        control_block_p->_is_expired.store(true, std::memory_order_release);
    }
};


template <typename T>
class smart_ptr<T, RefType::_Observer>
{
    template <typename T, RefType Type>
    friend class smart_ptr;

    friend class FE::archetype_base;

	static_assert(std::is_array_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to an array.");
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a const type variable.");
   
    using control_block_type = internal::smart_ptr::metadata<T>;

public:
    using element_type = T;
    using pointer = T*;
    using const_pointer = const T*;

private:
    std::atomic<control_block_type*> m_ptr;

public:
    smart_ptr() noexcept
		:   m_ptr()
    {}

    smart_ptr(const smart_ptr<T, RefType::_Owner>& target_p) noexcept
        :   m_ptr(target_p.m_ptr.load(std::memory_order_acquire))
    {
        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);

        m_ptr.load(std::memory_order_acquire)->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
    }

    template<class Child>
    smart_ptr(const smart_ptr<Child, RefType::_Owner>& target_p) noexcept
        :   m_ptr(reinterpret_cast<control_block_type*>(target_p.m_ptr.load(std::memory_order_acquire)))
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);

        m_ptr.load(std::memory_order_acquire)->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
    }

    smart_ptr& operator=(const smart_ptr<T, RefType::_Owner>& target_p) noexcept
    {
        if (target_p.m_ptr == nullptr)
        {
            return *this;
        }
        FE_ASSERT(target_p.m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");
        FE_ASSERT(target_p.m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);

        reset();
        m_ptr.store(target_p.m_ptr, std::memory_order_release);
        m_ptr.load(std::memory_order_acquire)->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
        return *this;
    }

    template<class Child>
    smart_ptr& operator=(const smart_ptr<Child, RefType::_Owner>& target_p) noexcept
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");
        if (target_p.m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return *this;
        }
        FE_ASSERT(target_p.m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");
        FE_ASSERT(target_p.m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);

        reset();
        m_ptr.store(reinterpret_cast<control_block_type*>(target_p.m_ptr.load(std::memory_order_acquire)), std::memory_order_release);
        m_ptr.load(std::memory_order_acquire)->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
        return *this;
    }

    ~smart_ptr() noexcept
    {
        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");

        m_ptr.load(std::memory_order_acquire)->_observer_count.fetch_sub(1, std::memory_order_acq_rel); // Decrement the observer count

        if ((m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == true) &&
            (m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) == 0))
        {
			__destruct_and_deallocate_all(m_ptr);
        }
    }

    smart_ptr(const smart_ptr& other_p) noexcept
        :   m_ptr(other_p.m_ptr.load(std::memory_order_acquire))
    {
        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);

        m_ptr.load(std::memory_order_acquire)->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count
    }

    template<class Child>
    smart_ptr(const smart_ptr<Child, FE::RefType::_Observer>& other_p) noexcept
        :   m_ptr(reinterpret_cast<control_block_type*>(other_p.m_ptr.load(std::memory_order_acquire)))
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);

        m_ptr.load(std::memory_order_acquire)->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count
    }

    smart_ptr& operator=(const smart_ptr& other_p) noexcept
    {
        if ((other_p.m_ptr.load(std::memory_order_acquire) == nullptr) || (other_p.m_ptr.load(std::memory_order_acquire) == m_ptr.load(std::memory_order_acquire)))
        {
            return *this;
        }
        FE_ASSERT(other_p.m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");
        FE_ASSERT(other_p.m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);

        reset();
        m_ptr.store(other_p.m_ptr.load(std::memory_order_acquire), std::memory_order_release);
        m_ptr.load(std::memory_order_acquire)->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
        return *this;
    }

    template<class Child>
    smart_ptr& operator=(const smart_ptr<Child, FE::RefType::_Observer>& other_p) noexcept
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");

        if ((other_p.m_ptr.load(std::memory_order_acquire) == nullptr) || (reinterpret_cast<control_block_type*>(other_p.m_ptr.load(std::memory_order_acquire)) == m_ptr.load(std::memory_order_acquire)))
        {
            return *this;
        }
        FE_ASSERT(other_p.m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");
        FE_ASSERT(other_p.m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);

        reset();
        m_ptr.store(reinterpret_cast<control_block_type*>(other_p.m_ptr.load()), std::memory_order_release);
        m_ptr.load(std::memory_order_acquire)->_observer_count.fetch_add(1, std::memory_order_acq_rel); // Increment the observer count.
        return *this;
    }

    smart_ptr(smart_ptr&& other_p) noexcept
        :   m_ptr( other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel) )
    {}

    template<class Child>
    smart_ptr(smart_ptr<Child, FE::RefType::_Observer>&& other_p) noexcept
        :   m_ptr(reinterpret_cast<control_block_type*>(other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel)) )
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");
    }

    smart_ptr& operator=(smart_ptr&& other_p) noexcept
    {
        m_ptr = other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel);
        return *this;
    }

    template<class Child>
    smart_ptr& operator=(smart_ptr<Child, FE::RefType::_Observer>&& other_p) noexcept
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: the template argument Child is not polymorphic.");
        m_ptr = reinterpret_cast<control_block_type*>(other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel));
        return *this;
    }

    void reset() noexcept
    {
        if (m_ptr.load(std::memory_order_acquire) == nullptr)
        {
            return;
        }
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) >= 0, "Something went wrong with the Frogman Engine smart_ptr atomic counter.");

        m_ptr.load(std::memory_order_acquire)->_observer_count.fetch_sub(1, std::memory_order_acq_rel); // Decrement the observer count

        if ((m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == true) &&
            (m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire) == 0))
        {
            __destruct_and_deallocate_all(m_ptr);
        }
        m_ptr.store(nullptr, std::memory_order_release);
    }

    _FE_FORCE_INLINE_ void swap(smart_ptr& other_p) noexcept
    {
        other_p.m_ptr.store(m_ptr.exchange(other_p.m_ptr.load(std::memory_order_acquire), std::memory_order_acq_rel), std::memory_order_release); // this op is not atomic, but helps other threads to view on time.
    }

    _FE_FORCE_INLINE_ T* operator->() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);
        return m_ptr.load(std::memory_order_acquire)->_data;
    }

    _FE_FORCE_INLINE_ const T* operator->() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);
        return m_ptr.load(std::memory_order_acquire)->_data;
    }

    _FE_FORCE_INLINE_ T& operator*() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);
        return *(m_ptr.load(std::memory_order_acquire)->_data);
    }

    _FE_FORCE_INLINE_ const T& operator*() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        FE_ASSERT(m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false);
        return *(m_ptr.load(std::memory_order_acquire)->_data);
    }

    _FE_FORCE_INLINE_ FE::boolean is_valid() const noexcept
    {
        return ((m_ptr.load(std::memory_order_acquire) != nullptr) && (m_ptr.load(std::memory_order_acquire)->_is_expired.load(std::memory_order_acquire) == false));
    }

    _FE_FORCE_INLINE_ FE::uint64 observer_count() const noexcept
    {
        if (m_ptr == nullptr)
        {
            return 0;
        }
        return m_ptr.load(std::memory_order_acquire)->_observer_count.load(std::memory_order_acquire);
	}

private:
    void __destruct_and_deallocate_all(control_block_type* const control_block_p) noexcept
    {
        if (control_block_p == nullptr)
        {
            return;
        }

        std::pmr::memory_resource* const l_resource = control_block_p->_resource;
        FE::size l_total_size_in_bytes = control_block_p->_sizeofT + sizeof(control_block_type);
        if (control_block_p->_is_expired.load(std::memory_order_acquire) == false)
        {
			control_block_p->_data->~T(); // control_block_p->_data points to the beginning of the allocated block.
        }
        control_block_p->~control_block_type();
        l_resource->deallocate(control_block_p->_data, l_total_size_in_bytes);
    }

    void __destruct_T(control_block_type* const control_block_p) noexcept
    {
        if (control_block_p == nullptr)
        {
            return;
        }
        control_block_p->_data->~T();
        control_block_p->_is_expired .store(true, std::memory_order_release);
    }
};

template <typename T, typename... Arguments>
_FE_FORCE_INLINE_ smart_ptr<std::remove_all_extents_t<T>, RefType::_Owner> _FE_VECTOR_CALL_ make_owner(std::pmr::memory_resource* resource_p = std::pmr::get_default_resource(), Arguments&&... arguments_p) noexcept
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
