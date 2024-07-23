#ifndef _FE_CORE_FUNCTION_HXX_
#define _FE_CORE_FUNCTION_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/type_traits.hxx>

// std
#include <any>




BEGIN_NAMESPACE(FE)


class void_t{};

enum class ARGUMENTS_COUNT : uint8
{
    _0 = 0,
    _1 = 1,
    _2 = 2,
    _3 = 3,
    _4 = 4,
    _5 = 5,
    _6 = 6,
    _7 = 7,
    _8 = 8,
    _9 = 9,
    _10 = 10
};

class argument_base {};

template<typename First = void, typename Second = void, typename Third = void, typename Fourth = void, typename Fifth = void, typename Sixth = void, typename Seventh = void, typename Eighth = void, typename Ninth = void, typename Tenth = void>
class arguments;

template<>
class arguments<void, void, void, void, void, void, void, void, void, void> : public argument_base
{
public:
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_0;
};

template<typename First>
class arguments<First, void, void, void, void, void, void, void, void, void> : public argument_base
{
public:
    using first_type = First;

    First _first;
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_1;

    _FORCE_INLINE_ arguments() noexcept 
        : _first() {}

    _FORCE_INLINE_ arguments(const First& first_p) noexcept
        : _first(first_p) {}
};

template<typename First, typename Second>
class arguments<First, Second, void, void, void, void, void, void, void, void> : public arguments<First, void, void, void, void, void, void, void, void, void>
{
public:
    using base_type = arguments<First, void, void, void, void, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = Second;

    second_type _second;
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_2;

    _FORCE_INLINE_ arguments() noexcept 
        : base_type(), _second() {}
    
    _FORCE_INLINE_ arguments(const First& first_p, const Second& second_p) noexcept
        : base_type(first_p), _second(second_p) {}
};

template<typename First, typename Second, typename Third>
class arguments<First, Second, Third, void, void, void, void, void, void, void> : public arguments<First, Second, void, void, void, void, void, void, void, void>
{
public:
    using base_type = arguments<First, Second, void, void, void, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::Second;
    using third_type = Third;

    third_type _third;
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_3;

    _FORCE_INLINE_ arguments() noexcept 
        : base_type(), _third() {}
    
    _FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p) noexcept
        : base_type(first_p, second_p), _third(third_p) {}
};

template<typename First, typename Second, typename Third, typename Fourth>
class arguments<First, Second, Third, Fourth, void, void, void, void, void, void> : public arguments<First, Second, Third, void, void, void, void, void, void, void>
{
public:
    using base_type = arguments<First, Second, Third, void, void, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = Fourth;

    fourth_type _fourth;
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_4;

    _FORCE_INLINE_ arguments() noexcept
        : base_type(), _fourth() {}

    _FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p) noexcept
        : base_type(first_p, second_p, third_p), _fourth(fourth_p) {}
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth>
class arguments<First, Second, Third, Fourth, Fifth, void, void, void, void, void> : public arguments<First, Second, Third, Fourth, void, void, void, void, void, void>
{
public:
    using base_type = arguments<First, Second, Third, Fourth, void, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = typename base_type::fourth_type;
    using fifth_type = Fifth;

    fifth_type _fifth;
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_5;

    _FORCE_INLINE_ arguments() noexcept 
        : base_type(), _fifth() {}

    _FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p, 
                             const Fifth& fifth_p) noexcept
        : base_type(first_p, second_p, third_p, fourth_p), _fifth(fifth_p) {}
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth>
class arguments<First, Second, Third, Fourth, Fifth, Sixth, void, void, void, void> : public arguments<First, Second, Third, Fourth, Fifth, void, void, void, void, void>
{
public:
    using base_type = arguments<First, Second, Third, Fourth, Fifth, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = typename base_type::fourth_type;
    using fifth_type = typename base_type::fifth_type;
    using sixth_type = Sixth;

    sixth_type _sixth;
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_6;

    _FORCE_INLINE_ arguments() noexcept 
        : base_type(), _sixth() {}

    _FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p,
                             const Fifth& fifth_p, const Sixth& sixth_p) noexcept
        : base_type(first_p, second_p, third_p, fourth_p, fifth_p), _sixth(sixth_p) {}
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh>
class arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, void, void, void> : public arguments<First, Second, Third, Fourth, Fifth, Sixth, void, void, void, void>
{
public:
    using base_type = arguments<First, Second, Third, Fourth, Fifth, Sixth, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = typename base_type::third_type;
    using fourth_type = typename base_type::fourth_type;
    using fifth_type = typename base_type::fifth_type;
    using sixth_type = typename base_type::sixth_type;
    using seventh_type = Seventh;

    seventh_type _seventh;
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_7;


    _FORCE_INLINE_ arguments() noexcept
        : base_type(), _seventh() {}

    _FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p,
                             const Fifth& fifth_p, const Sixth& sixth_p, const Seventh& seventh_p) noexcept
        : base_type(first_p, second_p, third_p, fourth_p, fifth_p, sixth_p), _seventh(seventh_p) {}
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Eighth>
class arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, void, void> : public arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, void, void, void>
{
public:
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
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_8;

    _FORCE_INLINE_ arguments() noexcept
        : base_type(), _eighth() {}

    _FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p,
                             const Fifth& fifth_p, const Sixth& sixth_p, const Seventh& seventh_p, const Eighth& eighth_p)
        : base_type(first_p, second_p, third_p, fourth_p, fifth_p, sixth_p, seventh_p), _eighth(eighth_p) {}
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Eighth, typename Ninth>
class arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Ninth, void> : public  arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, void, void>
{
public:
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
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_9;

    _FORCE_INLINE_ arguments() noexcept 
        : base_type(), _ninth() {}

    _FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p,
                             const Fifth& fifth_p, const Sixth& sixth_p, const Seventh& seventh_p, const Eighth& eighth_p,
                             const Ninth& ninth_p) noexcept
        : base_type(first_p, second_p, third_p, fourth_p, fifth_p, sixth_p, seventh_p, eighth_p), _ninth(ninth_p) {}
};

template<typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Eighth, typename Ninth, typename Tenth>
class arguments : public arguments<First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Ninth, void>
{
public:
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
    static constexpr inline ARGUMENTS_COUNT count = ARGUMENTS_COUNT::_10;


    _FORCE_INLINE_ arguments() noexcept 
        : base_type(), _tenth() {}
        
    _FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p,
                             const Fifth& fifth_p, const Sixth& sixth_p, const Seventh& seventh_p, const Eighth& eighth_p,
                             const Ninth& ninth_p, const Tenth& tenth_p) noexcept
        : base_type(first_p, second_p, third_p, fourth_p, fifth_p, sixth_p, seventh_p, eighth_p, ninth_p), _tenth(tenth_p) {}
};


template<class C, typename TaskImpl, class ArgumentsBufferType>
class cpp_style_task;

template<typename TaskImpl, class ArgumentsBufferType>
class c_style_task;


template<typename F>
class function;

template<typename R, typename ...Arguments>
class function<R(Arguments...)> final
{
    template<typename task_impl, class arguments_buffer_type>
    friend class c_style_task;

    R(*m_function_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = false;

    using function_type = decltype(m_function_pointer);
    using return_type = R;
    using arguments_type = FE::arguments<std::remove_reference_t<Arguments>...>;
    using parameters_type = FE::arguments<Arguments...>;

    _FORCE_INLINE_ _CONSTEXPR20_ function() noexcept : m_function_pointer() {}
    _FORCE_INLINE_ _CONSTEXPR23_ ~function() noexcept = default;

    _FORCE_INLINE_ _CONSTEXPR20_ function(function_type function_p) noexcept : m_function_pointer(function_p) {}
    _FORCE_INLINE_ _CONSTEXPR20_ function(const function& other_p) noexcept : m_function_pointer(other_p.m_function_pointer) {}

    _FORCE_INLINE_ R operator()(Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_function_pointer));
        return this->m_function_pointer(arguments_p...);
    }

    _FORCE_INLINE_ const R operator()(Arguments&& ...arguments_p) const noexcept
    {
        FE_ASSERT(this->m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_function_pointer));
        return this->m_function_pointer(arguments_p...);
    }

    _FORCE_INLINE_ _CONSTEXPR20_ function& operator=(function_type function_p) noexcept
    {
        this->m_function_pointer = function_p;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ function& operator=(const function& other_p) noexcept
    {
        this->m_function_pointer = other_p.m_function_pointer;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ function& operator=(function&& rvalue_p) noexcept
    {
        this->m_function_pointer = rvalue_p.m_function_pointer;
        rvalue_p.m_function_pointer = nullptr;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ operator bool() const noexcept
    {
        return this->m_function_pointer != nullptr;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(const function& other_p) const noexcept
    {
        return (this->m_function_pointer == other_p.m_function_pointer) ? true : false;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(const function& other_p) const noexcept
    {
        return (this->m_function_pointer != other_p.m_function_pointer) ? true : false;
    }
};

template<typename ...Arguments>
class function<void(Arguments...)> final
{
    template<typename task_impl, class arguments_buffer_type>
    friend class c_style_task;

    void(*m_function_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = true;

    using function_type = decltype(m_function_pointer);
    using return_type = void;
    using arguments_type = FE::arguments<std::remove_reference_t<Arguments>...>;
    using parameters_type = FE::arguments<Arguments...>;

    _FORCE_INLINE_ _CONSTEXPR20_ function() noexcept : m_function_pointer() {}
    ~function() noexcept = default;

    _FORCE_INLINE_ _CONSTEXPR20_ function(function_type function_p) noexcept : m_function_pointer(function_p) {}
    _FORCE_INLINE_ _CONSTEXPR20_ function(const function& other_p) noexcept : m_function_pointer(other_p.m_function_pointer) {}

    _FORCE_INLINE_ void operator()(Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_function_pointer));
        this->m_function_pointer(arguments_p...);
    }

    _FORCE_INLINE_ void operator()(Arguments&& ...arguments_p) const noexcept
    {
        FE_ASSERT(this->m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_function_pointer));
        this->m_function_pointer(arguments_p...);
    }

    _FORCE_INLINE_ _CONSTEXPR20_ function& operator=(function_type function_p) noexcept
    {
        this->m_function_pointer = function_p;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ function& operator=(const function& other_p) noexcept
    {
        this->m_function_pointer = other_p.m_function_pointer;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ function& operator=(function&& rvalue_p) noexcept
    {
        this->m_function_pointer = rvalue_p.m_function_pointer;
        rvalue_p.m_function_pointer = nullptr;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ operator bool() const noexcept
    {
        return this->m_function_pointer != nullptr;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(const function& other_p) const noexcept
    {
        return (this->m_function_pointer == other_p.m_function_pointer) ? true : false;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(const function& other_p) const noexcept
    {
        return (this->m_function_pointer != other_p.m_function_pointer) ? true : false;
    }
};


template<class C, typename F>
class method;

template<class C, typename R, typename ...Arguments>
class method<C, R(Arguments...) const> final
{
    template<class c, typename task_impl, class arguments_buffer_type>
    friend class cpp_style_task;

    R(C::* m_method_pointer)(Arguments...) const;

public:
    static constexpr inline boolean has_void_return_type = false;

    using method_type = decltype(m_method_pointer);
    using return_type = R;
    using arguments_type = FE::arguments<std::remove_reference_t<Arguments>...>;
    using parameters_type = FE::arguments<Arguments...>;
    using class_type = C;

    _FORCE_INLINE_ _CONSTEXPR20_ method() noexcept : m_method_pointer() {}
    _FORCE_INLINE_ _CONSTEXPR23_ ~method() noexcept {}

    _FORCE_INLINE_ _CONSTEXPR20_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _FORCE_INLINE_ _CONSTEXPR20_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}

    _FORCE_INLINE_ R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        return (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _FORCE_INLINE_ const R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) const noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        return (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};

template<class C, typename ...Arguments>
class method<C, void(Arguments...) const> final
{
    template<class c, typename task_impl, class arguments_buffer_type>
    friend class cpp_style_task;

    void(C::* m_method_pointer)(Arguments...) const;

public:
    static constexpr inline boolean has_void_return_type = true;

    using method_type = decltype(m_method_pointer);
    using return_type = void;
    using arguments_type = FE::arguments<std::remove_reference_t<Arguments>...>;
    using parameters_type = FE::arguments<Arguments...>;
    using class_type = C;

    _FORCE_INLINE_ _CONSTEXPR20_ method() noexcept : m_method_pointer() {}
    _FORCE_INLINE_ _CONSTEXPR23_ ~method() noexcept {}

    _FORCE_INLINE_ _CONSTEXPR20_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _FORCE_INLINE_ _CONSTEXPR20_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}

    _FORCE_INLINE_ void operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _FORCE_INLINE_ void operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) const noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};

template<class C, typename R, typename ...Arguments>
class method<C, R(Arguments...)> final
{
    template<class c, typename task_impl, class arguments_buffer_type>
    friend class cpp_style_task;

    R(C::* m_method_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = false;

    using method_type = decltype(m_method_pointer);
    using return_type = R;
    using arguments_type = FE::arguments<std::remove_reference_t<Arguments>...>;
    using parameters_type = FE::arguments<Arguments...>;
    using class_type = C;

    constexpr _FORCE_INLINE_ method() noexcept : m_method_pointer() {}
    _FORCE_INLINE_ _CONSTEXPR23_ ~method() noexcept {}

    _FORCE_INLINE_ _CONSTEXPR20_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _FORCE_INLINE_ _CONSTEXPR20_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}

    _FORCE_INLINE_ R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        return (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _FORCE_INLINE_ const R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) const noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        return (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};

template<class C, typename ...Arguments>
class method<C, void(Arguments...)> final
{
    template<class c, typename task_impl, class arguments_buffer_type>
    friend class cpp_style_task;

    void(C::* m_method_pointer)(Arguments...);

public:
    static constexpr inline boolean has_void_return_type = true;

    using method_type = decltype(m_method_pointer);
    using return_type = void;
    using arguments_type = FE::arguments<std::remove_reference_t<Arguments>...>;
    using parameters_type = FE::arguments<Arguments...>;
    using class_type = C;

    _FORCE_INLINE_ _CONSTEXPR20_ method() noexcept : m_method_pointer() {}
    _FORCE_INLINE_ _CONSTEXPR23_ ~method() noexcept {}

    _FORCE_INLINE_ _CONSTEXPR20_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _FORCE_INLINE_ _CONSTEXPR20_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}

    _FORCE_INLINE_ void operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _FORCE_INLINE_ void operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) const noexcept
    {
        FE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _FORCE_INLINE_ _CONSTEXPR20_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};




class task_base
{
public:
    task_base() noexcept = default;
    virtual ~task_base() noexcept = default;

    virtual std::any operator()(argument_base* const arguments_p) noexcept = 0;
    virtual std::any operator()(std::any& arguments_p) noexcept = 0;

    // Caller must ensure the class type correctness. It has no effect for c_style_tasks.
	virtual void set_instance(const std::any& target_instance_p) noexcept = 0;

	// Checks if the function pointer is nullptr.
    virtual boolean is_null(void) const noexcept = 0;
};


template<class C, typename TaskImpl, class ArgumentsBufferType = typename FE::method<C, TaskImpl>::arguments_type>
class cpp_style_task : public task_base
{
    static_assert(std::is_class<C>::value == true, "C must be a class type.");

public:
    using class_type = C;
    using task_impl_type = TaskImpl;
    using task_type = FE::method<class_type, task_impl_type>;
    using arguments_buffer_type = ArgumentsBufferType;
    using parameters_type = typename task_type::parameters_type;
    using return_type = typename FE::remove_const_reference<typename FE::method<class_type, task_impl_type>::return_type>::type;
    using task_result_buffer_type = typename FE::conditional_type<std::is_void<return_type>::value, void_t, return_type>::type;

    //static_assert(std::is_same<typename task_type::arguments_type, arguments_buffer_type>::value == true, "Static assertion failed: The task arugments type and the task arugments buffer type mismatch.");

    typename task_type::method_type _method;

private:
    class_type* m_instance;

public:
    _FORCE_INLINE_ _CONSTEXPR20_ cpp_style_task() noexcept : _method(), m_instance() {}
    virtual ~cpp_style_task() noexcept = default;

    _FORCE_INLINE_ _CONSTEXPR20_ cpp_style_task(typename task_type::method_type task_p) noexcept : _method(task_p), m_instance() {}

    virtual std::any operator()(argument_base* const arguments_p) noexcept override
    {
        FE_ASSERT(this->m_instance == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_instance));

        if constexpr (task_type::has_void_return_type == true)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {   
                (m_instance->*_method)();
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_seventh);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_ninth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_ninth, static_cast<arguments_buffer_type* const>(arguments_p)->_tenth);
                return void_t();
            }
        }
        else if constexpr (task_type::has_void_return_type == false)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {
                return (m_instance->*_method)();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_third);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_fifth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_seventh);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_ninth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return (m_instance->*_method)(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth, 
                                              static_cast<arguments_buffer_type* const>(arguments_p)->_ninth, static_cast<arguments_buffer_type* const>(arguments_p)->_tenth);
            }
        }
    }

    virtual std::any operator()(std::any& arguments_p) noexcept override
    {
        FE_ASSERT(this->m_instance == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_instance));

        if constexpr (task_type::has_void_return_type == true)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {   
                (m_instance->*_method)();
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                (m_instance->*_method)(std::any_cast<arguments_buffer_type&>(arguments_p)._first);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)->_second);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._seventh);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._ninth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._ninth, std::any_cast<arguments_buffer_type>(arguments_p)._tenth);
                return void_t();
            }
        }
        else if constexpr (task_type::has_void_return_type == false)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {
                return (m_instance->*_method)();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                return (m_instance->*_method)(std::any_cast<arguments_buffer_type&>(arguments_p)._first);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                return (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)->_second);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                return (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._third);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                return (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                return (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                              std::any_cast<arguments_buffer_type>(arguments_p)._fifth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                return (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                              std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                return (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                              std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._seventh);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                return (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                              std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                return (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                              std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._ninth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                return (m_instance->*_method)(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                              std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth, 
                                              std::any_cast<arguments_buffer_type>(arguments_p)._ninth, std::any_cast<arguments_buffer_type>(arguments_p)._tenth);
            }
        }
    }

    virtual void set_instance(const std::any& host_class_instance_p) noexcept override
    {
		this->m_instance = std::any_cast<class_type*>(host_class_instance_p);
    }

    virtual boolean is_null(void) const noexcept override
    {
        if (!this->_method)
        {
            return true;
        }

        if (this->m_instance == nullptr)
        {
            return true;
        }

        return false;
    }
};


template<typename TaskImpl, class ArgumentsBufferType = typename FE::function<TaskImpl>::arguments_type>
class c_style_task : public task_base
{
public:
    using task_impl_type = TaskImpl;
    using task_type = FE::function<task_impl_type>;
    using arguments_buffer_type = ArgumentsBufferType;
    using parameters_type = typename task_type::parameters_type;
    using return_type = typename FE::remove_const_reference<typename FE::function<task_impl_type>::return_type>::type;
    using task_result_buffer_type = typename FE::conditional_type<std::is_void<return_type>::value, void_t, return_type>::type;

    //static_assert(std::is_same<typename task_type::arguments_type, arguments_buffer_type>::value == true, "Static assertion failed: The task arugments type and the task arugments buffer type mismatch.");

    typename task_type::function_type _function;

public:
    _FORCE_INLINE_ _CONSTEXPR20_ c_style_task() noexcept : _function() {}
    virtual ~c_style_task() noexcept = default;

    _FORCE_INLINE_ _CONSTEXPR20_ c_style_task(typename task_type::function_type task_p) noexcept : _function(task_p) {}

    virtual std::any operator()(argument_base* const arguments_p) noexcept override
    {
        if constexpr (task_type::has_void_return_type == true)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {
                this->_function();
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_third);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_fifth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_seventh);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_ninth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth, 
                                static_cast<arguments_buffer_type* const>(arguments_p)->_ninth, static_cast<arguments_buffer_type* const>(arguments_p)->_tenth);
                return void_t();
            }
        }
        else if constexpr (task_type::has_void_return_type == false)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_seventh);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_ninth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                FE_ASSERT(arguments_p == nullptr, "Incorrect argument value detected: nullptr has been passed to a task() with non-void parameters.");
                return this->_function(static_cast<arguments_buffer_type* const>(arguments_p)->_first, static_cast<arguments_buffer_type* const>(arguments_p)->_second, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_third, static_cast<arguments_buffer_type* const>(arguments_p)->_fourth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_fifth, static_cast<arguments_buffer_type* const>(arguments_p)->_sixth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_seventh, static_cast<arguments_buffer_type* const>(arguments_p)->_eighth, 
                                       static_cast<arguments_buffer_type* const>(arguments_p)->_ninth, static_cast<arguments_buffer_type* const>(arguments_p)->_tenth);
            }
        }
    }

    virtual std::any operator()(std::any& arguments_p) noexcept override
    {
        if constexpr (task_type::has_void_return_type == true)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {
                this->_function();
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._third);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._fifth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._seventh);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second,
                                std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._ninth);
                return void_t();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth, 
                                std::any_cast<arguments_buffer_type>(arguments_p)._ninth, std::any_cast<arguments_buffer_type>(arguments_p)._tenth);
                return void_t();
            }
        }
        else if constexpr (task_type::has_void_return_type == false)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {
                return this->_function();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                return this->_function(std::any_cast<arguments_buffer_type&>(arguments_p)._first);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                return this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                return this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                return this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second,
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                return this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                return this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                return this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._seventh);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                return this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                return this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._ninth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                return this->_function(std::any_cast<arguments_buffer_type>(arguments_p)._first, std::any_cast<arguments_buffer_type>(arguments_p)._second, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._third, std::any_cast<arguments_buffer_type>(arguments_p)._fourth,
                                       std::any_cast<arguments_buffer_type>(arguments_p)._fifth, std::any_cast<arguments_buffer_type>(arguments_p)._sixth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._seventh, std::any_cast<arguments_buffer_type>(arguments_p)._eighth, 
                                       std::any_cast<arguments_buffer_type>(arguments_p)._ninth, std::any_cast<arguments_buffer_type>(arguments_p)._tenth);
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

    virtual void set_instance(const std::any&) noexcept override {};
};

END_NAMESPACE
#endif