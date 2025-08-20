#ifndef _FE_FRAMEWORK_SMART_PTR_HXX_
#define _FE_FRAMEWORK_SMART_PTR_HXX_

#include <FE/prerequisites.h>

#include <memory_resource>
#include <optional>
#include <type_traits>
#include <utility>




CLASS_FORWARD_DECLARATION(FE::framework, framework_base)


BEGIN_NAMESPACE(FE)


enum struct RefType
{
    _Owner,
    _Observer
};

namespace internal::smart_ptr
{
    template <typename T>
    class block final
    {
    public:
        std::optional<T> m_data;
        var::int32 m_observer_count;

    public:
        template <typename... Arguments>
        _FE_FORCE_INLINE_ block(Arguments&&... arguments_p) noexcept
            : m_data(std::forward<Arguments>(arguments_p)...), m_observer_count() {}

        _FE_FORCE_INLINE_ ~block() noexcept
        {
            if (this->m_data == std::nullopt)
            {
                return;
            }
            this->m_data = std::nullopt;
        }

        _FE_FORCE_INLINE_ void destruct() noexcept
        {
            this->m_data = std::nullopt;
        }

        _FE_FORCE_INLINE_ void increment_observer_count() noexcept
        {
            ++m_observer_count;
        }

        _FE_FORCE_INLINE_ void decrement_observer_count() noexcept
        {
            --m_observer_count;
        }

        _FE_FORCE_INLINE_ FE::boolean is_expired() const noexcept
        {
            return m_data == std::nullopt;
        }

        _FE_FORCE_INLINE_ const T& get() const noexcept
        {
            FE_ASSERT(m_data != std::nullopt);
            return *m_data;
        }

        _FE_FORCE_INLINE_ T& get() noexcept
        {
            FE_ASSERT(m_data != std::nullopt);
            return *m_data;
        }

        _FE_FORCE_INLINE_ FE::int32 count_observers() const noexcept
        {
            return m_observer_count;
        }
    };
}


template <typename T, RefType Type>
class smart_ptr;


template <typename T>
class smart_ptr<T, RefType::_Owner>
{
    friend class smart_ptr<T, RefType::_Observer>;

    static_assert(std::is_array_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to an array.");
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a const type variable.");
    
    using block_type = internal::smart_ptr::block<T>;

    std::pmr::memory_resource* m_resource;
    block_type* m_ptr;

public:
    _FE_FORCE_INLINE_ smart_ptr() noexcept
        : m_resource(), m_ptr() {}

    template <typename... Arguments>
    _FE_FORCE_INLINE_ smart_ptr(std::pmr::polymorphic_allocator<block_type> resource_p, Arguments&&... arguments_p) noexcept
        : m_resource(resource_p.resource()), m_ptr()
    {
        this->m_ptr = resource_p.allocate(1);

        FE_ASSERT(m_ptr != nullptr);
        new(m_ptr) block_type(std::forward<Arguments>(arguments_p)...);
    }

    _FE_FORCE_INLINE_~smart_ptr() noexcept
    {
        if (this->m_ptr == nullptr)
        {
            return;
        }

        this->m_ptr->destruct();
        if (this->m_ptr->count_observers() <= 0)
        {
            this->m_ptr->~block_type();
            std::pmr::polymorphic_allocator<block_type>{ m_resource }.deallocate(m_ptr, 1);
        }
    }

    smart_ptr(const smart_ptr&) = delete;
    smart_ptr& operator=(const smart_ptr&) = delete;

    _FE_FORCE_INLINE_ smart_ptr(smart_ptr&& other_p) noexcept
        : m_resource(other_p.m_resource), m_ptr(other_p.m_ptr)
    {
        other_p.m_resource = nullptr;
        other_p.m_ptr = nullptr;
    }

    _FE_FORCE_INLINE_ smart_ptr& operator=(smart_ptr&& other_p) noexcept
    {
        this->reset();
        this->m_resource = other_p.m_resource;
        this->m_ptr = other_p.m_ptr;
        other_p.m_resource = nullptr;
        other_p.m_ptr = nullptr;
        return *this;
    }

    _FE_FORCE_INLINE_ void reset() noexcept
    {
        if (this->m_ptr == nullptr)
        {
            return;
        }

        this->m_ptr->destruct();
        if (this->m_ptr->count_observers() <= 0)
        {
            this->m_ptr->~block_type();
            std::pmr::polymorphic_allocator<block_type>{ m_resource }.deallocate(m_ptr, 1);
        }
        this->m_resource = nullptr;
        this->m_ptr = nullptr;
    }

    _FE_FORCE_INLINE_ void swap(smart_ptr& other_p) noexcept
    {
        std::swap(this->m_resource, other_p.m_resource);
        std::swap(this->m_ptr, other_p.m_ptr);
    }

    _FE_FORCE_INLINE_ T* operator->() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return &(m_ptr->get());
    }

    _FE_FORCE_INLINE_ const T* operator->() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return &(m_ptr->get());
    }

    _FE_FORCE_INLINE_ T& operator*() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return m_ptr->get();
    }

    _FE_FORCE_INLINE_ const T& operator*() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return m_ptr->get();
    }
    
	_FE_FORCE_INLINE_ FE::boolean is_null() const noexcept
	{
		return (this->m_ptr == nullptr);
	}
};

template <typename T>
class smart_ptr<T, RefType::_Observer>
{
	static_assert(std::is_array_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to an array.");
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a const type variable.");

    using block_type = internal::smart_ptr::block<T>;

    std::pmr::memory_resource* m_resource;
    block_type* m_ptr;

public:
    _FE_FORCE_INLINE_ smart_ptr() noexcept
		: m_resource(), m_ptr() {}

    _FE_FORCE_INLINE_ smart_ptr(const smart_ptr<T, RefType::_Owner>& target_p) noexcept
        : m_resource(target_p.m_resource), m_ptr(target_p.m_ptr)
    {
        if (this->m_ptr == nullptr)
        {
            return;
        }

        this->m_ptr->increment_observer_count();
    }

    _FE_FORCE_INLINE_ smart_ptr& operator=(const smart_ptr<T, RefType::_Owner>& target_p) noexcept
    {
        this->m_resource = target_p.m_resource;
        this->m_ptr = target_p.m_ptr;

        if (this->m_ptr == nullptr)
        {
            return *this;
        }

        this->m_ptr->increment_observer_count();
        return *this;
    }

    _FE_FORCE_INLINE_ ~smart_ptr() noexcept
    {
        if (this->m_ptr == nullptr)
        {
            return;
        }

        this->m_ptr->decrement_observer_count();
        if ((m_ptr->count_observers() <= 0)
            && (this->m_ptr->is_expired() == true))
        {
            this->m_ptr->~block_type();
            std::pmr::polymorphic_allocator<block_type>{ m_resource }.deallocate(m_ptr, 1);
        }
    }

    _FE_FORCE_INLINE_ smart_ptr(const smart_ptr& other_p) noexcept
        : m_resource(other_p.m_resource), m_ptr(other_p.m_ptr)
    {
        if (this->m_ptr == nullptr)
        {
            return;
        }
        this->m_ptr->increment_observer_count();
    }

    _FE_FORCE_INLINE_ smart_ptr& operator=(const smart_ptr& other_p) noexcept
    {
        this->m_resource = other_p.m_resource;
        this->m_ptr = other_p.m_ptr;

        if (this->m_ptr == nullptr)
        {
            return *this;
        }

        this->m_ptr->increment_observer_count();
        return *this;
    }

    _FE_FORCE_INLINE_ smart_ptr(smart_ptr&& other_p) noexcept
        : m_resource(other_p.m_resource), m_ptr(other_p.m_ptr)
    {
        other_p.m_resource = nullptr;
        other_p.m_ptr = nullptr;
    }

    _FE_FORCE_INLINE_ smart_ptr& operator=(smart_ptr&& other_p) noexcept
    {
        this->m_resource = other_p.m_resource;
        this->m_ptr = other_p.m_ptr;
        other_p.m_resource = nullptr;
        other_p.m_ptr = nullptr;
        return *this;
    }

    _FE_FORCE_INLINE_ void reset() noexcept
    {
        if (this->m_ptr == nullptr)
        {
            return;
        }

        this->m_ptr->decrement_observer_count();
        if ((m_ptr->count_observers() <= 0)
            && (this->m_ptr->is_expired() == true))
        {
            this->m_ptr->~block_type();
            std::pmr::polymorphic_allocator<block_type>{ m_resource }.deallocate(m_ptr, 1);
        }
        this->m_resource = nullptr;
        this->m_ptr = nullptr;
    }

    _FE_FORCE_INLINE_ void swap(smart_ptr& other_p) noexcept
    {
        std::swap(this->m_resource, other_p.m_resource);
        std::swap(this->m_ptr, other_p.m_ptr);
    }

    _FE_FORCE_INLINE_ T* operator->() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return &(m_ptr->get());
    }

    _FE_FORCE_INLINE_ const T* operator->() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return &(m_ptr->get());
    }

    _FE_FORCE_INLINE_ T& operator*() noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return m_ptr->get();
    }

    _FE_FORCE_INLINE_ const T& operator*() const noexcept
    {
        FE_ASSERT(m_ptr != nullptr);
        return m_ptr->get();
    }

    _FE_FORCE_INLINE_ FE::boolean is_expired() const noexcept
    {
        return ((this->m_ptr == nullptr) || (this->m_ptr->is_expired() == true));
    }

    _FE_FORCE_INLINE_ FE::boolean is_null() const noexcept
    {
        return (this->m_ptr == nullptr);
    }
};

template <typename T, typename... Arguments>
_FE_FORCE_INLINE_ smart_ptr<std::remove_all_extents_t<T>, RefType::_Owner> gcnew(Arguments&&... arguments_p) noexcept
{
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: smart_ptr cannot hold a pointer to a const type variable.");
    return smart_ptr<std::remove_all_extents_t<T>, RefType::_Owner>(::FE::framework::framework_base::get_framework().get_memory_resource(), std::forward<Arguments>(arguments_p)...);
}


END_NAMESPACE
#endif