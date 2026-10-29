#ifndef _FROGMAN_ENGINE_GC_PTR_
#define _FROGMAN_ENGINE_GC_PTR_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/gc_base.hpp>

#include <FE/thread_id.hxx>

#include <atomic>
#include <memory_resource>
#include <type_traits>
#include <utility>




BEGIN_NAMESPACE(FE)


class gc_metadata_base;

namespace internal::gc_ptr
{
#pragma warning(push)
#pragma warning(disable: 4324)
    struct alignas(FE::CPU_L1_cache_line::size) metadata
    {
        FE::gc_base* _data;
        std::atomic_bool _is_cyclic_garbage;
        std::atomic_bool _is_garbage;
		std::atomic_bool _is_under_concurrent_inspection;
        std::atomic_int64_t _reference_count;
        std::pmr::memory_resource* const _resource;
        FE::uint64 _length;
        FE::uint64 _sizeofT;
        FE::uint64 _alignofT;
    };
    static_assert(alignof(metadata) == FE::CPU_L1_cache_line::size);
#pragma warning(pop)

    _FE_FORCE_INLINE_ constexpr var::size align_up(FE::size size_in_bytes_p, FE::size alignment_in_bytes_p) noexcept
    {
        return ((size_in_bytes_p + alignment_in_bytes_p - 1) / alignment_in_bytes_p) * alignment_in_bytes_p;
    }
}

class gc_ptr_base
{
    friend class reference_table;
protected:
	virtual internal::gc_ptr::metadata* __get_control_block() const noexcept = 0;
	virtual void __release() noexcept = 0;
};


template <typename T>
class gc_ptr : public gc_ptr_base
{
    template <typename U>
    friend class gc_ptr;
    friend class gc_metadata_base;

    template <typename U, typename... Arguments>
    friend FE::gc_ptr<U> _FE_VECTOR_CALL_ gcnew(std::pmr::memory_resource* const resource_p, Arguments&&... arguments_p) noexcept;

    static_assert(std::is_array_v<T> == false, "Static assertion failed: gc_ptr<T> cannot hold a pointer to a bounded array. Use gc_ptr<T[]> instead.");
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: gc_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: gc_ptr cannot hold a pointer to a const type variable.");

    static_assert(std::is_base_of_v<FE::gc_base, T> == true, "Static assertion failed: T must be derived from FE::gc_base.");

    using control_block_type = internal::gc_ptr::metadata;

public:
    using element_type = T;
    using pointer = T*;
    using const_pointer = const T*;

private:
    std::atomic<control_block_type*> m_ptr;

    template <typename... Arguments>
    explicit gc_ptr(std::pmr::memory_resource* const resource_p, Arguments&&... arguments_p) noexcept
        :   m_ptr( __allocate_new_block(resource_p, std::forward<Arguments>(arguments_p)...) )
    {}

public:
    gc_ptr() noexcept
        :   m_ptr()
    {}

    gc_ptr(std::nullptr_t) noexcept
        :   m_ptr()
    {}

    ~gc_ptr() noexcept
    {
        __decrement_reference_count( m_ptr.load(std::memory_order_acquire) );
    }

    gc_ptr(const gc_ptr& other_p) noexcept
        :   m_ptr( __increment_reference_count(other_p.m_ptr.load(std::memory_order_acquire)) )
    {}

    template <class Child>
    gc_ptr(const gc_ptr<Child>& other_p) noexcept
        :   m_ptr( __increment_reference_count(other_p.m_ptr.load(std::memory_order_acquire)) )
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: Child must be derived from T.");
    }

    gc_ptr& operator=(const gc_ptr& other_p) noexcept
    {
        control_block_type* const l_control_block = __increment_reference_count( other_p.m_ptr.load(std::memory_order_acquire) );
        __decrement_reference_count( m_ptr.exchange(l_control_block, std::memory_order_acq_rel) );
        return *this;
    }

    template <class Child>
    gc_ptr& operator=(const gc_ptr<Child>& other_p) noexcept
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: Child must be derived from T.");

        control_block_type* const l_control_block = __increment_reference_count( other_p.m_ptr.load(std::memory_order_acquire) );
        __decrement_reference_count( m_ptr.exchange(l_control_block, std::memory_order_acq_rel) );
        return *this;
    }

    gc_ptr(gc_ptr&& other_p) noexcept
        :   m_ptr( other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel) )
    {}

    template <class Child>
    gc_ptr(gc_ptr<Child>&& other_p) noexcept
        :   m_ptr( other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel) )
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: Child must be derived from T.");
    }

    gc_ptr& operator=(gc_ptr&& other_p) noexcept
    {
        __decrement_reference_count( m_ptr.exchange(other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel), std::memory_order_acq_rel) );
        return *this;
    }

    template <class Child>
    gc_ptr& operator=(gc_ptr<Child>&& other_p) noexcept
    {
        static_assert(std::is_base_of_v<T, Child>, "Static assertion failed: Child must be derived from T.");

        __decrement_reference_count( m_ptr.exchange(other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel), std::memory_order_acq_rel) );
        return *this;
    }

    void reset() noexcept
    {
        __decrement_reference_count( m_ptr.exchange(nullptr, std::memory_order_acq_rel) );
    }

    void swap(gc_ptr& other_p) noexcept
    {
        other_p.m_ptr.store( m_ptr.exchange( other_p.m_ptr.load(std::memory_order_acquire), std::memory_order_acq_rel ), std::memory_order_release );
    }

    T* operator->() noexcept
    {
        return __get_data( m_ptr.load(std::memory_order_acquire) );
    }

    const T* operator->() const noexcept
    {
        return __get_data( m_ptr.load(std::memory_order_acquire) );
    }

    T& operator*() noexcept
    {
        return *( __get_data(m_ptr.load(std::memory_order_acquire)) );
    }

    const T& operator*() const noexcept
    {
        return *( __get_data(m_ptr.load(std::memory_order_acquire)) );
    }

    FE::boolean operator==(std::nullptr_t) const noexcept
    {
        return (m_ptr.load(std::memory_order_acquire) == nullptr);
    }

    FE::boolean operator!=(std::nullptr_t) const noexcept
    {
        return (m_ptr.load(std::memory_order_acquire) != nullptr);
    }

    FE::boolean operator==(const gc_ptr& other_p) const noexcept
    {
        return (m_ptr.load(std::memory_order_acquire) == other_p.m_ptr.load(std::memory_order_acquire));
    }

    FE::boolean operator!=(const gc_ptr& other_p) const noexcept
    {
        return (m_ptr.load(std::memory_order_acquire) != other_p.m_ptr.load(std::memory_order_acquire));
    }

    FE::uint64 reference_count() const noexcept
    {
        control_block_type* const l_control_block = m_ptr.load(std::memory_order_acquire);
        if (l_control_block == nullptr)
        {
            return 0;
        }
        return static_cast<var::uint64>( l_control_block->_reference_count.load(std::memory_order_acquire) );
    }

private:
	virtual control_block_type* __get_control_block() const noexcept override
	{
		return m_ptr.load(std::memory_order_acquire);
	}

	virtual void __release() noexcept override
	{
		reset();
	}

    template <typename... Arguments>
    static control_block_type* _FE_VECTOR_CALL_ __allocate_new_block(std::pmr::memory_resource* const resource_p, Arguments&&... arguments_p) noexcept
    {
        static_assert(std::is_base_of_v<FE::gc_base, T> == true, "Static assertion failed: T must be derived from FE::gc_base.");
        FE_ASSERT(resource_p != nullptr, "Assertion failed: cannot create a new gc_ptr data block with a null memory_resource.");

        constexpr FE::size l_metadata_offset_in_bytes = internal::gc_ptr::align_up(sizeof(T), alignof(control_block_type));
        constexpr FE::size l_total_size_in_bytes = l_metadata_offset_in_bytes + sizeof(control_block_type);
        constexpr FE::size l_alignment_in_bytes = (alignof(T) > alignof(control_block_type)) ? alignof(T) : alignof(control_block_type);

        void* const l_alloc_result = resource_p->allocate(l_total_size_in_bytes, l_alignment_in_bytes);
        FE_ASSERT(l_alloc_result != nullptr, "Assertion failed: gc_ptr allocation failed.");

        T* const l_data = ::new(l_alloc_result) T( std::forward<Arguments>(arguments_p)... );

        return ::new(static_cast<var::byte*>(l_alloc_result) + l_metadata_offset_in_bytes) 
            control_block_type
            (
                l_data,
                false,
                false,
                false,
                1,
                resource_p,
                1,
                sizeof(T),
                alignof(T)
            );
    }

    static control_block_type* __increment_reference_count(control_block_type* const control_block_p) noexcept
    {
        if (control_block_p == nullptr)
        {
            return nullptr;
        }

        _FE_MAYBE_UNUSED_ const auto l_previous_reference_count = control_block_p->_reference_count.fetch_add(1, std::memory_order_acq_rel);
        FE_ASSERT(l_previous_reference_count > 0, "Assertion failed: cannot reference a dead gc object.");
        return control_block_p;
    }

    static void __decrement_reference_count(control_block_type* const control_block_p) noexcept
    {
        if (control_block_p == nullptr)
        {
            return;
        }

        _FE_MAYBE_UNUSED_ const auto l_previous_reference_count = control_block_p->_reference_count.fetch_sub(1, std::memory_order_acq_rel);
        FE_ASSERT(l_previous_reference_count > 0, "Something went wrong with the Frogman Engine gc_ptr atomic counter.");
    }

    static T* __get_data(control_block_type* const control_block_p) noexcept
    {
        static_assert(std::is_base_of_v<FE::gc_base, T> == true, "Static assertion failed: T must be derived from FE::gc_base.");
        FE_ASSERT(control_block_p != nullptr, "Assertion failed: cannot dereference a null gc_ptr.");
        FE_ASSERT(control_block_p->_reference_count.load(std::memory_order_acquire) > 0, "Assertion failed: cannot access a dead gc object.");

        return static_cast<T*>(control_block_p->_data);
    }
};
static_assert(sizeof(FE::gc_ptr<FE::gc_base>) == 16, "Static assertion failed: FE::gc_ptr<T> must be an 16-byte smart pointer.");


template <typename T>
class gc_ptr<T[]> : public gc_ptr_base
{
    template <typename U, typename... Arguments>
    friend FE::gc_ptr<U> _FE_VECTOR_CALL_ gcnew(std::pmr::memory_resource* const resource_p, Arguments&&... arguments_p) noexcept;
    friend class gc_metadata_base;

    static_assert(std::is_array_v<T> == false, "Static assertion failed: gc_ptr cannot hold a pointer to a multidimensional array.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: gc_ptr cannot hold a pointer to a const type variable.");

    static_assert(std::is_base_of_v<FE::gc_base, T> == true, "Static assertion failed: T must be derived from FE::gc_base.");

    using control_block_type = internal::gc_ptr::metadata;

public:
    using element_type = T;
    using pointer = T*;
    using const_pointer = const T*;

private:
    std::atomic<control_block_type*> m_ptr;

    template <typename... Arguments>
    explicit gc_ptr(std::pmr::memory_resource* const resource_p, FE::size length_p, const Arguments&... arguments_p) noexcept
        :   m_ptr( __allocate_new_block(resource_p, length_p, arguments_p...) )
    {}

public:
    gc_ptr() noexcept
        :   m_ptr()
    {}

    gc_ptr(std::nullptr_t) noexcept
        :   m_ptr()
    {}

    ~gc_ptr() noexcept
    {
        __decrement_reference_count( m_ptr.load(std::memory_order_acquire) );
    }

    gc_ptr(const gc_ptr& other_p) noexcept
        :   m_ptr( __increment_reference_count(other_p.m_ptr.load(std::memory_order_acquire)) )
    {}

    gc_ptr& operator=(const gc_ptr& other_p) noexcept
    {
        control_block_type* const l_control_block = __increment_reference_count( other_p.m_ptr.load(std::memory_order_acquire) );
        __decrement_reference_count( m_ptr.exchange(l_control_block, std::memory_order_acq_rel) );
        return *this;
    }

    gc_ptr(gc_ptr&& other_p) noexcept
        :   m_ptr( other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel) )
    {}

    gc_ptr& operator=(gc_ptr&& other_p) noexcept
    {
        __decrement_reference_count( m_ptr.exchange(other_p.m_ptr.exchange(nullptr, std::memory_order_acq_rel), std::memory_order_acq_rel) );
        return *this;
    }

    void reset() noexcept
    {
        __decrement_reference_count( m_ptr.exchange(nullptr, std::memory_order_acq_rel) );
    }

    void swap(gc_ptr& other_p) noexcept
    {
        other_p.m_ptr.store( m_ptr.exchange( other_p.m_ptr.load(std::memory_order_acquire), std::memory_order_acq_rel ), std::memory_order_release );
    }

    T& operator[](FE::size index_p) noexcept
    {
        control_block_type* const l_control_block = m_ptr.load(std::memory_order_acquire);
        T* const l_data = __get_data(l_control_block);
        FE_ASSERT(index_p < l_control_block->_length, "Assertion failed: gc_ptr<T[]> index out of range.");
        return l_data[index_p];
    }

    const T& operator[](FE::size index_p) const noexcept
    {
        control_block_type* const l_control_block = m_ptr.load(std::memory_order_acquire);
        const T* const l_data = __get_data(l_control_block);
        FE_ASSERT(index_p < l_control_block->_length, "Assertion failed: gc_ptr<T[]> index out of range.");
        return l_data[index_p];
    }

    FE::size length() const noexcept
    {
        control_block_type* const l_control_block = m_ptr.load(std::memory_order_acquire);
        if (l_control_block == nullptr)
        {
            return 0;
        }
        return l_control_block->_length;
    }

    FE::boolean operator==(std::nullptr_t) const noexcept
    {
        return (m_ptr.load(std::memory_order_acquire) == nullptr);
    }

    FE::boolean operator!=(std::nullptr_t) const noexcept
    {
        return (m_ptr.load(std::memory_order_acquire) != nullptr);
    }

    FE::boolean operator==(const gc_ptr& other_p) const noexcept
    {
        return (m_ptr.load(std::memory_order_acquire) == other_p.m_ptr.load(std::memory_order_acquire));
    }

    FE::boolean operator!=(const gc_ptr& other_p) const noexcept
    {
        return (m_ptr.load(std::memory_order_acquire) != other_p.m_ptr.load(std::memory_order_acquire));
    }

    FE::uint64 reference_count() const noexcept
    {
        control_block_type* const l_control_block = m_ptr.load(std::memory_order_acquire);
        if (l_control_block == nullptr)
        {
            return 0;
        }
        return static_cast<var::uint64>( l_control_block->_reference_count.load(std::memory_order_acquire) );
    }

private:
	virtual control_block_type* __get_control_block() const noexcept override
    {
        return m_ptr.load(std::memory_order_acquire);
    }

	virtual void __release() noexcept override
	{
		reset();
	}

    template <typename... Arguments>
    static control_block_type* _FE_VECTOR_CALL_ __allocate_new_block(std::pmr::memory_resource* const resource_p, FE::size length_p, const Arguments&... arguments_p) noexcept
    {
        static_assert(std::is_base_of_v<FE::gc_base, T> == true, "Static assertion failed: T must be derived from FE::gc_base.");
        FE_ASSERT(resource_p != nullptr, "Assertion failed: cannot create a new gc_ptr data block with a null memory_resource.");
        FE_ASSERT(length_p > 0, "Assertion failed: cannot create a gc_ptr<T[]> with zero length.");
        FE_ASSERT(length_p <= FE::max_value<var::uint32>, "Assertion failed: gc_ptr<T[]> length is too large.");
        FE_ASSERT(length_p <= ((FE::max_value<var::size> - sizeof(control_block_type) - alignof(control_block_type)) / sizeof(T)), "Assertion failed: gc_ptr<T[]> length is too large.");

        FE::size l_metadata_offset_in_bytes = internal::gc_ptr::align_up(sizeof(T) * length_p, alignof(control_block_type));
        FE::size l_total_size_in_bytes = l_metadata_offset_in_bytes + sizeof(control_block_type);
        constexpr FE::size l_alignment_in_bytes = (alignof(T) > alignof(control_block_type)) ? alignof(T) : alignof(control_block_type);

        void* const l_alloc_result = resource_p->allocate(l_total_size_in_bytes, l_alignment_in_bytes);
        FE_ASSERT(l_alloc_result != nullptr, "Assertion failed: gc_ptr allocation failed.");

        T* const l_data = static_cast<T*>(l_alloc_result);
        for (var::size i = 0; i < length_p; ++i)
        {
            ::new(l_data + i) T( arguments_p... );
        }

        return ::new(static_cast<var::byte*>(l_alloc_result) + l_metadata_offset_in_bytes) 
            control_block_type
            (
                l_data, 
                false,
                false,
                false,
                1,
                resource_p, 
                length_p, 
                sizeof(T), 
                alignof(T)
            );
    }

    static control_block_type* __increment_reference_count(control_block_type* const control_block_p) noexcept
    {
        if (control_block_p == nullptr)
        {
            return nullptr;
        }

        _FE_MAYBE_UNUSED_ const auto l_previous_reference_count = control_block_p->_reference_count.fetch_add(1, std::memory_order_acq_rel);
        FE_ASSERT(l_previous_reference_count > 0, "Assertion failed: cannot reference a dead gc object.");
        return control_block_p;
    }

    static void __decrement_reference_count(control_block_type* const control_block_p) noexcept
    {
        if (control_block_p == nullptr)
        {
            return;
        }

        _FE_MAYBE_UNUSED_ const auto l_previous_reference_count = control_block_p->_reference_count.fetch_sub(1, std::memory_order_acq_rel);
        FE_ASSERT(l_previous_reference_count > 0, "Something went wrong with the Frogman Engine gc_ptr atomic counter.");
    }

    static T* __get_data(control_block_type* const control_block_p) noexcept
    {
        static_assert(std::is_base_of_v<FE::gc_base, T> == true, "Static assertion failed: T must be derived from FE::gc_base.");
        FE_ASSERT(control_block_p != nullptr, "Assertion failed: cannot dereference a null gc_ptr.");
        FE_ASSERT(control_block_p->_reference_count.load(std::memory_order_acquire) > 0, "Assertion failed: cannot access a dead gc object.");

        return static_cast<T*>(control_block_p->_data);
    }
};




template <typename T>
struct is_gc_ptr : std::false_type {};

template <typename T>
struct is_gc_ptr< FE::gc_ptr<T> > : std::true_type {};

template <typename T>
struct is_gc_ptr< FE::gc_ptr<T[]> > : std::true_type {};

template<typename T>
_FE_MAYBE_UNUSED_ constexpr inline bool is_gc_ptr_v = is_gc_ptr<T>::value;


namespace internal::gc_ptr
{
    // offsets from the FE::gc_base subobject to the gc_ptr properties that Host declares itself. filled by add_watch inside FE_PROPERTY's FE_DO_ONCE.
    template <class Host>
    inline std::pmr::vector<var::int64>& own_gc_ptr_offsets() noexcept
    {
        static std::pmr::vector<var::int64> s_offsets;
        return s_offsets;
    }
}

class gc_metadata_base
{
public:
    template <class Host, typename GCPtr>
    static void add_watch(Host* const host_p, GCPtr& watched_object_p) noexcept
    {
        if constexpr (FE::is_gc_ptr_v<GCPtr> == true)
        {
            internal::gc_ptr::own_gc_ptr_offsets<Host>().push_back( reinterpret_cast<var::byte*>(static_cast<FE::gc_ptr_base*>(&watched_object_p)) - reinterpret_cast<var::byte*>(static_cast<FE::gc_base*>(host_p)) );
        }
    }
};


END_NAMESPACE
#endif
