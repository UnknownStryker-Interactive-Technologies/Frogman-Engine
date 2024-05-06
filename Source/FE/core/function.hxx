#ifndef _FE_CORE_FUNCTION_HXX_
#define _FE_CORE_FUNCTION_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <any>
#define PASS_RETURN_BUFFER(return_buffer) (&return_buffer)




BEGIN_NAMESPACE(FE)


using void_t = uint8;


enum struct FORWARD_DATA : boolean
{
    _AS_LVALUE_REF = true,
    _AS_RVALUE_REF = false
};




template<class C, typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode = FE::FORWARD_DATA::_AS_RVALUE_REF, typename ...ArgumentsBufferTypes>
struct cpp_style_task;

template<typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode = FE::FORWARD_DATA::_AS_RVALUE_REF, typename ...ArgumentsBufferTypes>
struct c_style_task;




template<typename F, FORWARD_DATA ArgumentsForwardingMode = FE::FORWARD_DATA::_AS_RVALUE_REF>
class function;

template<typename R, typename ...Arguments>
class function<R(Arguments...), FE::FORWARD_DATA::_AS_LVALUE_REF> final
{
    template<typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct c_style_task;

    R(*m_function_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = false;

    using function_type = decltype(m_function_pointer);
    using return_type = R;

    _CONSTEXPR20_ _FORCE_INLINE_ function() noexcept : m_function_pointer() {}
    _CONSTEXPR23_ _FORCE_INLINE_ ~function() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ function(function_type function_p) noexcept : m_function_pointer(function_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ function(const function& other_p) noexcept : m_function_pointer(other_p.m_function_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ function(function&& rvalue_p) noexcept : m_function_pointer(rvalue_p.m_function_pointer) { rvalue_p.m_function_pointer = nullptr; }

    _FORCE_INLINE_ R operator()(Arguments& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_function_pointer));
        return this->m_function_pointer(arguments_p...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(function_type function_p) noexcept
    {
        this->m_function_pointer = function_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(const function& other_p) noexcept
    {
        this->m_function_pointer = other_p.m_function_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(function&& rvalue_p) noexcept
    {
        this->m_function_pointer = rvalue_p.m_function_pointer;
        rvalue_p.m_function_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_function_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const function& other_p) const noexcept
    {
        return (this->m_function_pointer == other_p.m_function_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const function& other_p) const noexcept
    {
        return (this->m_function_pointer != other_p.m_function_pointer) ? true : false;
    }
};

template<typename ...Arguments>
class function<void(Arguments...), FE::FORWARD_DATA::_AS_LVALUE_REF> final
{
    template<typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct c_style_task;

    void(*m_function_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = true;

    using function_type = decltype(m_function_pointer);
    using return_type = void;

    _CONSTEXPR20_ _FORCE_INLINE_ function() noexcept : m_function_pointer() {}
    ~function() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ function(function_type function_p) noexcept : m_function_pointer(function_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ function(const function& other_p) noexcept : m_function_pointer(other_p.m_function_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ function(function&& rvalue_p) noexcept : m_function_pointer(rvalue_p.m_function_pointer) { rvalue_p.m_function_pointer = nullptr; }

    _FORCE_INLINE_ void operator()(Arguments& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_function_pointer));
        this->m_function_pointer(arguments_p...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(function_type function_p) noexcept
    {
        this->m_function_pointer = function_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(const function& other_p) noexcept
    {
        this->m_function_pointer = other_p.m_function_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(function&& rvalue_p) noexcept
    {
        this->m_function_pointer = rvalue_p.m_function_pointer;
        rvalue_p.m_function_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_function_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const function& other_p) const noexcept
    {
        return (this->m_function_pointer == other_p.m_function_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const function& other_p) const noexcept
    {
        return (this->m_function_pointer != other_p.m_function_pointer) ? true : false;
    }
};




template<typename R, typename ...Arguments>
class function<R(Arguments...), FE::FORWARD_DATA::_AS_RVALUE_REF> final
{
    template<typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct c_style_task;

    R(*m_function_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = false;

    using function_type = decltype(m_function_pointer);
    using return_type = R;

    _CONSTEXPR20_ _FORCE_INLINE_ function() noexcept : m_function_pointer() {}
    ~function() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ function(function_type function_ptr_p) noexcept : m_function_pointer(function_ptr_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ function(const function& other_cref_p) noexcept : m_function_pointer(other_cref_p.m_function_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ function(function&& rvalue_p) noexcept : m_function_pointer(rvalue_p.m_function_pointer) { rvalue_p.m_function_pointer = nullptr; }

    _FORCE_INLINE_ R operator()(Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_function_pointer));
        return this->m_function_pointer(std::forward<Arguments>(arguments_p)...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(function_type function_p) noexcept
    {
        this->m_function_pointer = function_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(const function& other_p) noexcept
    {
        this->m_function_pointer = other_p.m_function_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(function&& rvalue_p) noexcept
    {
        this->m_function_pointer = rvalue_p.m_function_pointer;
        rvalue_p.m_function_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_function_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const function& other_p) const noexcept
    {
        return (this->m_function_pointer == other_p.m_function_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const function& other_p) const noexcept
    {
        return (this->m_function_pointer != other_p.m_function_pointer) ? true : false;
    }
};

template<typename ...Arguments>
class function<void(Arguments...), FE::FORWARD_DATA::_AS_RVALUE_REF> final
{
    template<typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct c_style_task;

    void(*m_function_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = true;

    using function_type = decltype(m_function_pointer);
    using return_type = void;

    _CONSTEXPR20_ _FORCE_INLINE_ function() noexcept : m_function_pointer() {}
    _CONSTEXPR23_ _FORCE_INLINE_ ~function() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ function(function_type function_p) noexcept : m_function_pointer(function_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ function(const function& other_p) noexcept : m_function_pointer(other_p.m_function_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ function(function&& rvalue_p) noexcept : m_function_pointer(rvalue_p.m_function_pointer) { rvalue_p.m_function_pointer = nullptr; }

    _FORCE_INLINE_ void operator()(Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_function_pointer));
        this->m_function_pointer(std::forward<Arguments>(arguments_p)...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(function_type function_p) noexcept
    {
        this->m_function_pointer = function_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(const function& other_p) noexcept
    {
        this->m_function_pointer = other_p.m_function_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ function& operator=(function&& rvalue_p) noexcept
    {
        this->m_function_pointer = rvalue_p.m_function_pointer;
        rvalue_p.m_function_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_function_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const function& other_p) const noexcept
    {
        return (this->m_function_pointer == other_p.m_function_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const function& other_p) const noexcept
    {
        return (this->m_function_pointer != other_p.m_function_pointer) ? true : false;
    }
};




template<class C, typename F, FORWARD_DATA ArgumentsForwardingMode = FE::FORWARD_DATA::_AS_RVALUE_REF>
class method;

template<class C, typename R, typename ...Arguments>
class method<C, R(Arguments...) const, FE::FORWARD_DATA::_AS_LVALUE_REF> final
{
    template<class C, typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct cpp_style_task;

    R(C::* m_method_pointer)(Arguments...) const;

public:
    static constexpr inline boolean has_void_return_type = false;

    using method_type = decltype(m_method_pointer);
    using return_type = R;
    using class_type = C;

    _CONSTEXPR20_ _FORCE_INLINE_ method() noexcept : m_method_pointer() {}
    _CONSTEXPR23_ _FORCE_INLINE_ ~method() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(method&& rvalue_p) noexcept : m_method_pointer(rvalue_p.m_method_pointer) { rvalue_p.m_method_pointer = nullptr; }

    _FORCE_INLINE_ R operator()(class_type& instance_p, Arguments& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_method_pointer));
        return (instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};

template<class C, typename ...Arguments>
class method<C, void(Arguments...) const, FE::FORWARD_DATA::_AS_LVALUE_REF> final
{
    template<class C, typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct cpp_style_task;

    void(C::* m_method_pointer)(Arguments...) const;

public:
    static constexpr inline boolean has_void_return_type = true;

    using method_type = decltype(m_method_pointer);
    using return_type = void;
    using class_type = C;

    _CONSTEXPR20_ _FORCE_INLINE_ method() noexcept : m_method_pointer() {}
    _CONSTEXPR23_ _FORCE_INLINE_ ~method() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(method&& rvalue_p) noexcept : m_method_pointer(rvalue_p.m_method_pointer) { rvalue_p.m_method_pointer = nullptr; }

    _FORCE_INLINE_ void operator()(class_type& instance_p, Arguments& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_method_pointer));
        (instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};

template<class C, typename R, typename ...Arguments>
class method<C, R(Arguments...), FE::FORWARD_DATA::_AS_LVALUE_REF> final
{
    template<class C, typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct cpp_style_task;

    R(C::* m_method_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = false;

    using method_type = decltype(m_method_pointer);
    using return_type = R;
    using class_type = C;

    constexpr _FORCE_INLINE_ method() noexcept : m_method_pointer() {}
    _CONSTEXPR23_ _FORCE_INLINE_ ~method() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(method&& rvalue_p) noexcept : m_method_pointer(rvalue_p.m_method_pointer) { rvalue_p.m_method_pointer = nullptr; }

    _FORCE_INLINE_ R operator()(class_type& instance_p, Arguments& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_method_pointer));
        return (instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};

template<class C, typename ...Arguments>
class method<C, void(Arguments...), FE::FORWARD_DATA::_AS_LVALUE_REF> final
{
    template<class C, typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct cpp_style_task;

    void(C::* m_method_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = true;

    using method_type = decltype(m_method_pointer);
    using return_type = void;
    using class_type = C;

    _CONSTEXPR20_ _FORCE_INLINE_ method() noexcept : m_method_pointer() {}
    _CONSTEXPR23_ _FORCE_INLINE_ ~method() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(method&& rvalue_p) noexcept : m_method_pointer(rvalue_p.m_method_pointer) { rvalue_p.m_method_pointer = nullptr; }

    _FORCE_INLINE_ void operator()(class_type& instance_p, Arguments& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_method_pointer));
        (instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};


template<class C, typename R, typename ...Arguments>
class method<C, R(Arguments...) const, FE::FORWARD_DATA::_AS_RVALUE_REF> final
{
    template<class C, typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct cpp_style_task;

    R(C::* m_method_pointer)(Arguments...) const;

public:
    static constexpr inline boolean has_void_return_type = false;

    using method_type = decltype(m_method_pointer);
    using return_type = R;
    using class_type = C;

    _CONSTEXPR20_ _FORCE_INLINE_ method() noexcept : m_method_pointer() {}
    _CONSTEXPR23_ _FORCE_INLINE_ ~method() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(method&& rvalue_p) noexcept : m_method_pointer(rvalue_p.m_method_pointer) { rvalue_p.m_method_pointer = nullptr; }

    _FORCE_INLINE_ R operator()(class_type& instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_method_pointer));
        return (instance_p.*this->m_method_pointer)(std::forward<Arguments>(arguments_p)...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};

template<class C, typename ...Arguments>
class method<C, void(Arguments...) const, FE::FORWARD_DATA::_AS_RVALUE_REF> final
{
    template<class C, typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct cpp_style_task;

    void(C::* m_method_pointer)(Arguments...) const;

public:
    static constexpr inline boolean has_void_return_type = true;

    using method_type = decltype(m_method_pointer);
    using return_type = void;
    using class_type = C;

    _CONSTEXPR20_ _FORCE_INLINE_ method() noexcept : m_method_pointer() {}
    _CONSTEXPR23_ _FORCE_INLINE_ ~method() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(method&& rvalue_p) noexcept : m_method_pointer(rvalue_p.m_method_pointer) { rvalue_p.m_method_pointer = nullptr; }

    _FORCE_INLINE_ void operator()(class_type& instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_method_pointer));
        (instance_p.*this->m_method_pointer)(std::forward<Arguments>(arguments_p)...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};

template<class C, typename R, typename ...Arguments>
class method<C, R(Arguments...), FE::FORWARD_DATA::_AS_RVALUE_REF> final
{
    template<class C, typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct cpp_style_task;

    R(C::* m_method_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = false;

    using method_type = decltype(m_method_pointer);
    using return_type = R;
    using class_type = C;

    _CONSTEXPR20_ _FORCE_INLINE_ method() noexcept : m_method_pointer() {}
     _CONSTEXPR23_ _FORCE_INLINE_ ~method() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(method&& rvalue_p) noexcept : m_method_pointer(rvalue_p.m_method_pointer) { rvalue_p.m_method_pointer = nullptr; }

    _FORCE_INLINE_ R operator()(class_type& instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_method_pointer));
        return (instance_p.*this->m_method_pointer)(std::forward<Arguments>(arguments_p)...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ var::boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ var::boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};

template<class C, typename ...Arguments>
class method<C, void(Arguments...), FE::FORWARD_DATA::_AS_RVALUE_REF> final
{
    template<class C, typename TaskImpl, FORWARD_DATA ArgumentsForwardingMode, typename ...ArgumentsBufferTypes>
    friend struct cpp_style_task;

    void(C::* m_method_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = true;

    using method_type = decltype(m_method_pointer);
    using return_type = void;
    using class_type = C;

    _CONSTEXPR20_ _FORCE_INLINE_ method() noexcept : m_method_pointer() {}
    _CONSTEXPR23_ _FORCE_INLINE_ ~method() noexcept {}

    _CONSTEXPR20_ _FORCE_INLINE_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}
    _CONSTEXPR20_ _FORCE_INLINE_ method(method&& rvalue_p) noexcept : m_method_pointer(rvalue_p.m_method_pointer) { rvalue_p.m_method_pointer = nullptr; }

    _FORCE_INLINE_ void operator()(class_type& instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_method_pointer));
        (instance_p.*this->m_method_pointer)(std::forward<Arguments>(arguments_p)...);
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ var::boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ var::boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};




enum struct ARGUMENTS_COUNT
{
    _0 = 0,
    _1 = 1,
    _2 = 2,
    _3 =3,
    _4 = 4,
    _5 = 5,
    _6 = 6,
    _7 = 7,
    _8 = 8,
    _9 = 9,
    _10 = 10
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Eighth, typename Ninth, typename Tenth>
struct arguments;

template<>
struct arguments<void, void, void, void, void, void, void, void, void, void> 
{
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_0;
};

template<typename First>
struct arguments<First, void, void, void, void, void, void, void, void, void>
{
    using first_type = First;

    First _first;
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_1;
    
    _CONSTEXPR20_ _FORCE_INLINE_ arguments() noexcept = default;
    _CONSTEXPR23_ _FORCE_INLINE_ ~arguments() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments& arguments_p) noexcept : _first(arguments_p._first) {};
    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments&& arguments_p) noexcept : _first(std::move(arguments_p._first)) {};

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments& arguments_p) noexcept
    {
        this->_first = arguments_p._first;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments&& arguments_p) noexcept
    {
        this->_first = std::move(arguments_p._first);
        return *this;
    }
};

template<typename First, typename Second>
struct arguments<First, Second, void, void, void, void, void, void, void, void> : public arguments<First, void, void, void, void, void, void, void, void, void>
{
    using base_type = arguments<First, void, void, void, void, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = Second;

    second_type _second;
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_2;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments() noexcept : base_type(), _second() {};
    _CONSTEXPR23_ _FORCE_INLINE_ ~arguments() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments& arguments_p) noexcept : base_type(arguments_p), _second(arguments_p._second) {};
    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments&& arguments_p) noexcept : base_type(std::move(arguments_p)), _second(std::move(arguments_p._second)) {};

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_second = arguments_p._second;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments&& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_second = std::move(arguments_p._second);
        return *this;
    }
};

template<typename First, typename Second, typename Third>
struct arguments<First, Second, Third, void, void, void, void, void, void, void> : public arguments<First, Second, void, void, void, void, void, void, void, void>
{
    using base_type = arguments<First, Second, void, void, void, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::Second;
    using third_type = Third;

    third_type _third;
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_3;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments() noexcept : base_type(), _third() {};
    _CONSTEXPR23_ _FORCE_INLINE_ ~arguments() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments& arguments_p) noexcept : base_type(arguments_p), _third(arguments_p._third) {};
    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments&& arguments_p) noexcept : base_type(std::move(arguments_p)), _third(std::move(arguments_p._third)) {};

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_third = arguments_p._third;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments&& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_third = std::move(arguments_p._third);
        return *this;
    }
};

template<typename First, typename Second, typename Third, typename Fourth>
struct arguments<First, Second, Third, Fourth, void, void, void, void, void, void> : public arguments<First, Second, Third, void, void, void, void, void, void, void>
{
    using base_type = arguments<First, Second, Third, void, void, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = Fourth;

    fourth_type _fourth;
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_4;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments() noexcept : base_type(), _fourth() {};
    _CONSTEXPR23_ _FORCE_INLINE_ ~arguments() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments& arguments_p) noexcept : base_type(arguments_p), _fourth(arguments_p._fourth) {};
    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments&& arguments_p) noexcept : base_type(std::move(arguments_p)), _fourth(std::move(arguments_p._fourth)) {};

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_fourth = arguments_p._fourth;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments&& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_fourth = std::move(arguments_p._fourth);
        return *this;
    }
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
struct arguments<First, Second, Third, Fourth, Fifth, void, void, void, void, void> : public arguments<First, Second, Third, Fourth, void, void, void, void, void, void>
{
    using base_type = arguments<First, Second, Third, Fourth, void, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = typename base_type::fourth_type;
    using fifth_type = Fifth;

    fifth_type _fifth;
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_5;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments() noexcept : base_type(), _fifth() {};
    _CONSTEXPR23_ _FORCE_INLINE_ ~arguments() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments& arguments_p) noexcept : base_type(arguments_p), _fifth(arguments_p._fifth) {};
    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments&& arguments_p) noexcept : base_type(std::move(arguments_p)), _fifth(std::move(arguments_p._fifth)) {};

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_fifth = arguments_p._fifth;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments&& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_fifth = std::move(arguments_p._fifth);
        return *this;
    }
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth>
struct arguments<First, Second, Third, Fourth, Fifth, Sixth, void, void, void, void> : public arguments<First, Second, Third, Fourth, Fifth, void, void, void, void, void>
{
    using base_type = arguments<First, Second, Third, Fourth, Fifth, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = typename base_type::fourth_type;
    using fifth_type = typename base_type::fifth_type;
    using sixth_type = Sixth;

    sixth_type _sixth;
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_6;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments() noexcept : base_type(), _sixth() {};
    _CONSTEXPR23_ _FORCE_INLINE_ ~arguments() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments& arguments_p) noexcept : base_type(arguments_p), _sixth(arguments_p._sixth) {};
    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments&& arguments_p) noexcept : base_type(std::move(arguments_p)), _sixth(std::move(arguments_p._sixth)) {};

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_sixth = arguments_p._sixth;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments&& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_sixth = std::move(arguments_p._sixth);
        return *this;
    }
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh>
struct arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, void, void, void> : public arguments<First, Second, Third, Fourth, Fifth, Sixth, void, void, void, void>
{
    using base_type = arguments<First, Second, Third, Fourth, Fifth, Sixth, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = typename base_type::fourth_type;
    using fifth_type = typename base_type::fifth_type;
    using sixth_type = typename base_type::sixth_type;
    using seventh_type = Seventh;

    seventh_type _seventh;
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_7;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments() noexcept : base_type(), _seventh() {};
    _CONSTEXPR23_ _FORCE_INLINE_ ~arguments() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments& arguments_p) noexcept : base_type(arguments_p), _seventh(arguments_p._seventh) {};
    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments&& arguments_p) noexcept : base_type(std::move(arguments_p)), _seventh(std::move(arguments_p._seventh)) {};

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_seventh = arguments_p._seventh;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments&& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_seventh = std::move(arguments_p._seventh);
        return *this;
    }
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Eighth>
struct arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, void, void> : public arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, void, void, void>
{
    using base_type = arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = typename base_type::fourth_type;
    using fifth_type = typename base_type::fifth_type;
    using sixth_type = typename base_type::sixth_type;
    using seventh_type = typename base_type::seventh_type;
    using eighth_type = Eighth;

    eighth_type _eighth;
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_8;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments() noexcept : base_type(), _eighth() {};
    _FORCE_INLINE_ ~arguments() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments& arguments_p) noexcept : base_type(arguments_p), _eighth(arguments_p._eighth) {};
    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments&& arguments_p) noexcept : base_type(std::move(arguments_p)), _eighth(std::move(arguments_p._eighth)) {};

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_eighth = arguments_p._eighth;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments&& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_eighth = std::move(arguments_p._eighth);
        return *this;
    }
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Eighth, typename Ninth>
struct arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Ninth, void> : public  arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, void, void>
{
    using base_type = arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = typename base_type::fourth_type;
    using fifth_type = typename base_type::fifth_type;
    using sixth_type = typename base_type::sixth_type;
    using seventh_type = typename base_type::seventh_type;
    using eighth_type = typename base_type::eighth_type;
    using ninth_type = Ninth;

    ninth_type _ninth;
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_9;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments() noexcept : base_type(), _ninth() {};
    _FORCE_INLINE_ ~arguments() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments& arguments_p) noexcept : base_type(arguments_p), _ninth(arguments_p._ninth) {};
    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments&& arguments_p) noexcept : base_type(std::move(arguments_p)), _ninth(std::move(arguments_p._ninth)) {};

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_ninth = arguments_p._ninth;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments&& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_ninth = std::move(arguments_p._ninth);
        return *this;
    }
};


template<typename First = void, typename Second = void, typename Third = void, typename Fourth = void, typename Fifth = void, typename Sixth = void, typename Seventh = void, typename Eighth = void, typename Ninth = void, typename Tenth = void>
struct arguments : public arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Ninth, void>
{
    using base_type = arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Ninth, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = typename base_type::fourth_type;
    using fifth_type = typename base_type::fifth_type;
    using sixth_type = typename base_type::sixth_type;
    using seventh_type = typename base_type::seventh_type;
    using eighth_type = typename base_type::eighth_type;
    using ninth_type = typename base_type::ninth_type;
    using tenth_type = Tenth;

    tenth_type _tenth;
    static constexpr inline ARGUMENTS_COUNT arguments_count = ARGUMENTS_COUNT::_10;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments() noexcept : base_type(), _tenth() {};
    _FORCE_INLINE_ ~arguments() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments& arguments_p) noexcept : base_type(arguments_p), _tenth(arguments_p._tenth) {};
    _CONSTEXPR20_ _FORCE_INLINE_ arguments(arguments&& arguments_p) noexcept : base_type(std::move(arguments_p)), _tenth(std::move(arguments_p._tenth)) {};

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_tenth = arguments_p._tenth;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ arguments& operator=(arguments&& arguments_p) noexcept
    {
        base_type::operator=(arguments_p);
        this->_tenth = std::move(arguments_p._tenth);
        return *this;
    }
};




struct task_base
{
    virtual void operator()(void) noexcept = 0;
    virtual boolean is_null(void) const noexcept = 0;
    virtual std::any get_result() const noexcept = 0;
};




template<class C, typename TaskImpl, typename ...ArgumentsBufferTypes>
struct cpp_style_task<C, TaskImpl, FE::FORWARD_DATA::_AS_RVALUE_REF, ArgumentsBufferTypes...> : public task_base
{
    static_assert(std::is_class<C>::value == true, "C must be a class or a struct type.");

    using class_type = C;
    using task_impl_type = TaskImpl;
    using task_type = FE::method<class_type, task_impl_type, FE::FORWARD_DATA::_AS_RVALUE_REF>;
    using arguments_buffer_type = FE::arguments<ArgumentsBufferTypes...>;
    using return_type = typename FE::remove_const_reference<typename FE::method<class_type, task_impl_type, FE::FORWARD_DATA::_AS_RVALUE_REF>::return_type>::type;
    using task_result_buffer_type = typename FE::conditional_type<std::is_void<return_type>::value, void_t, return_type>::type;

    task_result_buffer_type _task_result;
    arguments_buffer_type _arguments_buffer;
    typename task_type::method_type _method;
    class_type* _instance_ptr;

    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task() noexcept : _task_result(), _arguments_buffer(), _method(), _instance_ptr() {}
    _FORCE_INLINE_ ~cpp_style_task() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task(typename task_type::method_type task_p) noexcept : _arguments_buffer(), _method(task_p), _instance_ptr(), _task_result() {}
    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task(cpp_style_task& other_p) noexcept : _arguments_buffer(other_p._arguments_buffer), _method(other_p._method), _instance_ptr(other_p._instance_ptr), _task_result(other_p._task_result) {}
    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task(cpp_style_task&& rvalue_p) noexcept : _arguments_buffer(std::move(rvalue_p._arguments_buffer)), _method(rvalue_p._method), _instance_ptr(rvalue_p._instance_ptr), _task_result(rvalue_p._task_result)
    {
        rvalue_p._method = nullptr;
        rvalue_p._instance_ptr = nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task& operator=(typename task_type::method_type task_p) noexcept
    {
        this->_method = task_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task& operator=(cpp_style_task& other_p) noexcept
    {
        this->_arguments_buffer = other_p._arguments_buffer;
        this->_method = other_p._method;
        this->_instance_ptr = other_p._instance_ptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task& operator=(cpp_style_task&& rvalue_p) noexcept
    {
        this->_arguments_buffer = std::move(rvalue_p._arguments_buffer);

        this->_method = rvalue_p._method;
        rvalue_p._method = nullptr;

        this->_instance_ptr = rvalue_p._instance_ptr;
        rvalue_p._instance_ptr = nullptr;
        return *this;
    }

    virtual void operator()(void) noexcept override
    {
        FE_ASSERT(this->_instance_ptr == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->_instance_ptr));

        if constexpr (task_type::has_void_return_type == true)
        {
            if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_0)
            {   
                (_instance_ptr->*_method)();
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_1)
            {
                (_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_2)
            {
                (_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_3)
            {
                (_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_4)
            {
                (_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_5)
            {
                (_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_6)
            {
                (_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_7)
            {
                (_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_8)
            {
                (_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_9)
            {
                (_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth), std::forward<typename arguments_buffer_type::_ninth>(this->_arguments_buffer._ninth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_10)
            {
                (_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth), std::forward<typename arguments_buffer_type::_ninth>(this->_arguments_buffer._ninth), std::forward<typename arguments_buffer_type::_tenth>(this->_arguments_buffer._tenth));
                return;
            }
        }
        else if constexpr (task_type::has_void_return_type == false)
        {
            if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_0)
            {
                this->_task_result = std::move((_instance_ptr->*_method)());
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_1)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_2)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_3)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_4)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_5)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_6)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_7)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_8)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_9)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth), std::forward<typename arguments_buffer_type::_ninth>(this->_arguments_buffer._ninth)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_10)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth), std::forward<typename arguments_buffer_type::_ninth>(this->_arguments_buffer._ninth), std::forward<typename arguments_buffer_type::_tenth>(this->_arguments_buffer._tenth)));
                return;
            }
        }
    }

    virtual boolean is_null(void) const noexcept override
    {
        if (!this->_method)
        {
            return true;
        }

        if (this->_instance_ptr == nullptr)
        {
            return true;
        }

        return false;
    }

    virtual std::any get_result() const noexcept override
    {
		return this->_task_result;
    }
};

template<class C, typename TaskImpl, typename ...ArgumentsBufferTypes>
struct cpp_style_task<C, TaskImpl, FE::FORWARD_DATA::_AS_LVALUE_REF, ArgumentsBufferTypes...> : public task_base
{
    static_assert(std::is_class<C>::value == true, "C must be a class or a struct type.");

    using class_type = C;
    using task_impl_type = TaskImpl;
    using task_type = FE::method<class_type, task_impl_type, FE::FORWARD_DATA::_AS_LVALUE_REF>;
    using arguments_buffer_type = FE::arguments<ArgumentsBufferTypes...>;
    using return_type = typename FE::remove_const_reference<typename FE::method<class_type, task_impl_type, FE::FORWARD_DATA::_AS_LVALUE_REF>::return_type>::type;
    using task_result_buffer_type = typename FE::conditional_type<std::is_void<return_type>::value, void_t, return_type>::type;

    task_result_buffer_type _task_result;
    arguments_buffer_type _arguments_buffer;
    typename task_type::method_type _method;
    class_type* _instance_ptr;

    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task() noexcept : _task_result(), _arguments_buffer(), _method(), _instance_ptr() {}
    _FORCE_INLINE_ ~cpp_style_task() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task(typename task_type::method_type task_p) noexcept : _arguments_buffer(), _method(task_p), _instance_ptr(), _task_result(){}
    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task(cpp_style_task& other_p) noexcept : _arguments_buffer(other_p._arguments_buffer), _method(other_p._method), _instance_ptr(other_p._instance_ptr), _task_result(other_p._task_result) {}
    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task(cpp_style_task&& rvalue_p) noexcept : _arguments_buffer(std::move(rvalue_p._arguments_buffer)), _method(rvalue_p._method), _instance_ptr(rvalue_p._instance_ptr), _task_result(rvalue_p._task_result)
    {
        rvalue_p._method = nullptr;
        rvalue_p._instance_ptr = nullptr;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task& operator=(typename task_type::method_type task_p) noexcept
    {
        this->_method = task_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task& operator=(cpp_style_task& other_p) noexcept
    {
        this->_arguments_buffer = other_p._arguments_buffer;
        this->_method = other_p._method;
        this->_instance_ptr = other_p._instance_ptr;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ cpp_style_task& operator=(cpp_style_task&& rvalue_p) noexcept
    {
        this->_arguments_buffer = std::move(rvalue_p._arguments_buffer);

        this->_method = rvalue_p._method;
        rvalue_p._method = nullptr;

        this->_instance_ptr = rvalue_p._instance_ptr;
        rvalue_p._instance_ptr = nullptr;
        return *this;
    }

    virtual void operator()(void) noexcept override
    {
        FE_ASSERT(this->_instance_ptr == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->_instance_ptr));

        if constexpr (task_type::has_void_return_type == true)
        {
            if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_0)
            {
                (_instance_ptr->*_method)();
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_1)
            {
                (_instance_ptr->*_method)(this->_arguments_buffer._first);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_2)
            {
                (_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_3)
            {
                (_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_4)
            {
                (_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_5)
            {
                (_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_6)
            {
                (_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_7)
            {
                (_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_8)
            {
                (_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_9)
            {
                (_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth, this->_arguments_buffer._ninth);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_10)
            {
                (_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth, this->_arguments_buffer._ninth, this->_arguments_buffer._tenth);
                return;
            }
        }
        else if constexpr (task_type::has_void_return_type == false)
        {
            if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_0)
            {
                this->_task_result = std::move((_instance_ptr->*_method)());
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_1)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(this->_arguments_buffer._first));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_2)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_3)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_4)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_5)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_6)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_7)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_8)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_9)
            {
                *this->_task_result = std::move((_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth, this->_arguments_buffer._ninth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_10)
            {
                this->_task_result = std::move((_instance_ptr->*_method)(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth, this->_arguments_buffer._ninth, this->_arguments_buffer._tenth));
                return;
            }
        }
    }

    virtual boolean is_null(void) const noexcept override
    {
        if (!this->_method)
        {
            return true;
        }

        if (this->_instance_ptr == nullptr)
        {
            return true;
        }

        return false;
    }

    virtual std::any get_result() const noexcept override
    {
        return this->_task_result;
    }
};








template<typename TaskImpl, typename ...ArgumentsBufferTypes>
struct c_style_task<TaskImpl, FE::FORWARD_DATA::_AS_RVALUE_REF, ArgumentsBufferTypes...> : public task_base
{
    using task_impl_type = TaskImpl;
    using task_type = FE::function<task_impl_type, FE::FORWARD_DATA::_AS_RVALUE_REF>;
    using arguments_buffer_type = FE::arguments<ArgumentsBufferTypes...>;
    using return_type = typename FE::remove_const_reference<typename FE::function<task_impl_type, FE::FORWARD_DATA::_AS_RVALUE_REF>::return_type>::type;
    using task_result_buffer_type = typename FE::conditional_type<std::is_void<return_type>::value, void_t, return_type>::type;

    task_result_buffer_type _task_result;
    arguments_buffer_type _arguments_buffer;
    typename task_type::function_type _function;
    
    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task() noexcept : _task_result(), _arguments_buffer(), _function() {}
    _FORCE_INLINE_ ~c_style_task() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task(typename task_type::function_type task_p) noexcept : _function(task_p), _task_result() {}
    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task(c_style_task& other_p) noexcept : _arguments_buffer(other_p._arguments_buffer), _function(other_p._function), _task_result(other_p._task_result) {}
    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task(c_style_task&& rvalue_p) noexcept : _arguments_buffer(std::move(rvalue_p._arguments_buffer)), _function(rvalue_p._function), _task_result(rvalue_p._task_result) { rvalue_p._function = nullptr; }

    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task& operator=(typename task_type::function_type task_p) noexcept
    {
        this->_function = task_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task& operator=(c_style_task& other_p) noexcept
    {
        this->_arguments_buffer = other_p._arguments_buffer;
        this->_function = other_p._function;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task& operator=(c_style_task&& rvalue_p) noexcept
    {
        this->_arguments_buffer = std::move(rvalue_p._arguments_buffer);

        this->_function = rvalue_p._function;
        rvalue_p._function = nullptr;
        return *this;
    }

    virtual void operator()(void) noexcept override
    {
        if constexpr (task_type::has_void_return_type == true)
        {
            if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_0)
            {
                this->_function();
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_1)
            {
                this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_2)
            {
                this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_3)
            {
                this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_4)
            {
                this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_5)
            {
                this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_6)
            {
                this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_7)
            {
                this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_8)
            {
                this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_9)
            {
                this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth), std::forward<typename arguments_buffer_type::_ninth>(this->_arguments_buffer._ninth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_10)
            {
                this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth), std::forward<typename arguments_buffer_type::_ninth>(this->_arguments_buffer._ninth), std::forward<typename arguments_buffer_type::_tenth>(this->_arguments_buffer._tenth));
                return;
            }
        }
        else if constexpr (task_type::has_void_return_type == false)
        {
            if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_0)
            {
                this->_task_result = std::move(this->_function());
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_1)
            {
                this->_task_result = std::move(this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_2)
            {
                this->_task_result = std::move(this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_3)
            {
                this->_task_result = std::move(this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_4)
            {
                this->_task_result = std::move(this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_5)
            {
                this->_task_result = std::move(this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_6)
            {
                this->_task_result = std::move(this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_7)
            {
                this->_task_result = std::move(this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_8)
            {
                this->_task_result = std::move(this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_9)
            {
                *this->_task_result = std::move(this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth), std::forward<typename arguments_buffer_type::_ninth>(this->_arguments_buffer._ninth)));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_10)
            {
                this->_task_result = std::move(this->_function(std::forward<typename arguments_buffer_type::first_type>(this->_arguments_buffer._first), std::forward<typename arguments_buffer_type::second_type>(this->_arguments_buffer._second), std::forward<typename arguments_buffer_type::third_type>(this->_arguments_buffer._third), std::forward<typename arguments_buffer_type::fourth_type>(this->_arguments_buffer._fourth), std::forward<typename arguments_buffer_type::fifth_type>(this->_arguments_buffer._fifth), std::forward<typename arguments_buffer_type::sixth_type>(this->_arguments_buffer._sixth), std::forward<typename arguments_buffer_type::seventh_type>(this->_arguments_buffer._seventh), std::forward<typename arguments_buffer_type::Eighth>(this->_arguments_buffer._eighth), std::forward<typename arguments_buffer_type::_ninth>(this->_arguments_buffer._ninth), std::forward<typename arguments_buffer_type::_tenth>(this->_arguments_buffer._tenth)));
                return;
            }
        }
    }

    virtual boolean is_null(void) const noexcept override
    {
        if (!this->_function)
        {
            return true;
        }

        return false;
    }

    virtual std::any get_result() const noexcept override
    {
        return this->_task_result;
    }
};

template<typename TaskImpl, typename ...ArgumentsBufferTypes>
struct c_style_task<TaskImpl, FE::FORWARD_DATA::_AS_LVALUE_REF, ArgumentsBufferTypes...> : public task_base
{
    using task_impl_type = TaskImpl;
    using task_type = FE::function<task_impl_type, FE::FORWARD_DATA::_AS_LVALUE_REF>;
    using arguments_buffer_type = FE::arguments<ArgumentsBufferTypes...>;
    using return_type = typename FE::remove_const_reference<typename FE::function<task_impl_type, FE::FORWARD_DATA::_AS_LVALUE_REF>::return_type>::type;
    using task_result_buffer_type = typename FE::conditional_type<std::is_void<return_type>::value, void_t, return_type>::type;
    
    task_result_buffer_type _task_result;
    arguments_buffer_type _arguments_buffer;
    typename task_type::function_type _function;

    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task() noexcept : _task_result(), _arguments_buffer(), _function() {}
    _FORCE_INLINE_ ~c_style_task() noexcept = default;

    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task(typename task_type::function_type task_p) noexcept : _function(task_p), _task_result() {}
    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task(c_style_task& other_p) noexcept : _arguments_buffer(other_p._arguments_buffer), _function(other_p._function), _task_result(other_p._task_result) {}
    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task(c_style_task&& rvalue_p) noexcept : _arguments_buffer(std::move(rvalue_p._arguments_buffer)), _function(rvalue_p._function), _task_result(rvalue_p._task_result) { rvalue_p._function = nullptr; }

    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task& operator=(typename task_type::function_type task_p) noexcept
    {
        this->_function = task_p;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task& operator=(c_style_task& other_p) noexcept
    {
        this->_arguments_buffer = other_p._arguments_buffer;
        this->_function = other_p._function;
        return *this;
    }

    _CONSTEXPR20_ _FORCE_INLINE_ c_style_task& operator=(c_style_task&& rvalue_p) noexcept
    {
        this->_arguments_buffer = std::move(rvalue_p._arguments_buffer);

        this->_function = rvalue_p._function;
        rvalue_p._function = nullptr;
        return *this;
    }

    virtual void operator()(void) noexcept override
    {
        if constexpr (task_type::has_void_return_type == true)
        {
            if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_0)
            {
                this->_function();
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_1)
            {
                this->_function(this->_arguments_buffer._first);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_2)
            {
                this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_3)
            {
                this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_4)
            {
                this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_5)
            {
                this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_6)
            {
                this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_7)
            {
                this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_8)
            {
                this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_9)
            {
                this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth, this->_arguments_buffer._ninth);
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_10)
            {
                this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth, this->_arguments_buffer._ninth, this->_arguments_buffer._tenth);
                return;
            }
        }
        else if constexpr (task_type::has_void_return_type == false)
        {
            if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_0)
            {
                this->_task_result = std::move(this->_function());
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_1)
            {
                this->_task_result = std::move(this->_function(this->_arguments_buffer._first));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_2)
            {
                this->_task_result = std::move(this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_3)
            {
                this->_task_result = std::move(this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_4)
            {
                this->_task_result = std::move(this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_5)
            {
                this->_task_result = std::move(this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_6)
            {
                this->_task_result = std::move(this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_7)
            {
                this->_task_result = std::move(this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_8)
            {
                this->_task_result = std::move(this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_9)
            {
                *this->_task_result = std::move(this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth, this->_arguments_buffer._ninth));
                return;
            }
            else if constexpr (this->_arguments_buffer.arguments_count == ARGUMENTS_COUNT::_10)
            {
                this->_task_result = std::move(this->_function(this->_arguments_buffer._first, this->_arguments_buffer._second, this->_arguments_buffer._third, this->_arguments_buffer._fourth, this->_arguments_buffer._fifth, this->_arguments_buffer._sixth, this->_arguments_buffer._seventh, this->_arguments_buffer._eighth, this->_arguments_buffer._ninth, this->_arguments_buffer._tenth));
                return;
            }
        }
    }

    virtual boolean is_null(void) const noexcept override
    {
        if (!this->_function)
        {
            return true;
        }

        return false;
    }

    virtual std::any get_result() const noexcept override
    {
        return this->_task_result;
    }
};

END_NAMESPACE
#endif