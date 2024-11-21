#ifndef _FE_CORE_FUNCTION_HXX_
#define _FE_CORE_FUNCTION_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/type_traits.hxx>

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


class argument_base
{
public:
    virtual ~argument_base() noexcept = default;
};


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

    _FE_FORCE_INLINE_ arguments() noexcept 
        : _first() {}

    _FE_FORCE_INLINE_ arguments(const First& first_p) noexcept
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

    _FE_FORCE_INLINE_ arguments() noexcept 
        : base_type(), _second() {}
    
    _FE_FORCE_INLINE_ arguments(const First& first_p, const Second& second_p) noexcept
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

    _FE_FORCE_INLINE_ arguments() noexcept 
        : base_type(), _third() {}
    
    _FE_FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p) noexcept
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

    _FE_FORCE_INLINE_ arguments() noexcept
        : base_type(), _fourth() {}

    _FE_FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p) noexcept
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

    _FE_FORCE_INLINE_ arguments() noexcept 
        : base_type(), _fifth() {}

    _FE_FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p, 
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

    _FE_FORCE_INLINE_ arguments() noexcept 
        : base_type(), _sixth() {}

    _FE_FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p,
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


    _FE_FORCE_INLINE_ arguments() noexcept
        : base_type(), _seventh() {}

    _FE_FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p,
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

    _FE_FORCE_INLINE_ arguments() noexcept
        : base_type(), _eighth() {}

    _FE_FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p,
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

    _FE_FORCE_INLINE_ arguments() noexcept 
        : base_type(), _ninth() {}

    _FE_FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p,
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


    _FE_FORCE_INLINE_ arguments() noexcept 
        : base_type(), _tenth() {}
        
    _FE_FORCE_INLINE_ arguments(const First& first_p, const Second& second_p, const Third& third_p, const Fourth& fourth_p,
                             const Fifth& fifth_p, const Sixth& sixth_p, const Seventh& seventh_p, const Eighth& eighth_p,
                             const Ninth& ninth_p, const Tenth& tenth_p) noexcept
        : base_type(first_p, second_p, third_p, fourth_p, fifth_p, sixth_p, seventh_p, eighth_p, ninth_p), _tenth(tenth_p) {}
};


template<class C, typename TaskImpl, class ArgumentsBufferType>
class cpp_style_task;

template<typename TaskImpl, class ArgumentsBufferType>
class c_style_task;


template<typename R, typename ...Arguments>
class function;

template<typename R, typename ...Arguments>
class function<R(Arguments...)> final
{
    template<typename task_impl, class arguments_buffer_type>
    friend class c_style_task;

    R(*m_function_pointer)(Arguments...);

public:
    using function_type = decltype(m_function_pointer);
    using return_type = R;
    using arguments_type = FE::arguments<std::remove_reference_t<Arguments>...>;

    function() noexcept = default;
    ~function() noexcept = default;

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ function(function_type function_p) noexcept : m_function_pointer(function_p) {}
    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ function(const function& other_p) noexcept : m_function_pointer(other_p.m_function_pointer) {}

    _FE_FORCE_INLINE_ R operator()(Arguments&& ...arguments_p) noexcept
    {
        FE_NEGATIVE_ASSERT(this->m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_function_pointer));
		if constexpr (std::is_same<R, void>::value == true)
        {
			this->m_function_pointer(arguments_p...);
		}
        else if constexpr (std::is_same<R, void>::value == false)
        {
			return this->m_function_pointer(arguments_p...);
		}
    }

    _FE_FORCE_INLINE_ const R operator()(Arguments&& ...arguments_p) const noexcept
    {
        FE_NEGATIVE_ASSERT(this->m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_function_pointer));
        if constexpr (std::is_same<R, void>::value == true)
        {
            this->m_function_pointer(arguments_p...);
        }
        else if constexpr (std::is_same<R, void>::value == false)
        {
            return this->m_function_pointer(arguments_p...);
        }
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ function& operator=(function_type function_p) noexcept
    {
        this->m_function_pointer = function_p;
        return *this;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ function& operator=(const function& other_p) noexcept
    {
        this->m_function_pointer = other_p.m_function_pointer;
        return *this;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ function& operator=(function&& rvalue_p) noexcept
    {
        this->m_function_pointer = rvalue_p.m_function_pointer;
        rvalue_p.m_function_pointer = nullptr;
        return *this;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ operator bool() const noexcept
    {
        return this->m_function_pointer != nullptr;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator==(const function& other_p) const noexcept
    {
        return (this->m_function_pointer == other_p.m_function_pointer) ? true : false;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator!=(const function& other_p) const noexcept
    {
        return (this->m_function_pointer != other_p.m_function_pointer) ? true : false;
    }
};


template<class C, typename R, typename ...Arguments>
class method;

template<class C, typename R, typename ...Arguments>
class method<C, R(Arguments...) const> final
{
    template<class c, typename task_impl, class arguments_buffer_type>
    friend class cpp_style_task;

    R(C::* m_method_pointer)(Arguments...) const;

public:
    using method_type = decltype(m_method_pointer);
    using return_type = R;
    using arguments_type = FE::arguments<std::remove_reference_t<Arguments>...>;
    using class_type = C;

    method() noexcept = default;
    ~method() noexcept = default;

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}

    _FE_FORCE_INLINE_ R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_NEGATIVE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        if constexpr (std::is_same<R, void>::value == true)
        {
            (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
        }
        else if constexpr (std::is_same<R, void>::value == false)
        {
            return (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
        }
    }

    _FE_FORCE_INLINE_ const R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) const noexcept
    {
        FE_NEGATIVE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        if constexpr (std::is_same<R, void>::value == true)
        {
            (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
        }
        else if constexpr (std::is_same<R, void>::value == false)
        {
            return (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
        }
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator!=(const method& other_p) const noexcept
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
    using method_type = decltype(m_method_pointer);
    using return_type = R;
    using arguments_type = FE::arguments<std::remove_reference_t<Arguments>...>;
    using class_type = C;

    method() noexcept = default;
    ~method() noexcept = default;

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}

    _FE_FORCE_INLINE_ R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_NEGATIVE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        if constexpr (std::is_same<R, void>::value == true)
        {
            (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
        }
		else if constexpr (std::is_same<R, void>::value == false)
        {
            return (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
		}
    }

    _FE_FORCE_INLINE_ const R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) const noexcept
    {
        FE_NEGATIVE_ASSERT(this->m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_method_pointer));
        if constexpr (std::is_same<R, void>::value == true)
        {
            (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
        }
        else if constexpr (std::is_same<R, void>::value == false)
        {
            return (host_class_instance_p.*this->m_method_pointer)(arguments_p...);
        }
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ method& operator=(method_type method_p) noexcept
    {
        this->m_method_pointer = method_p;
        return *this;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ method& operator=(const method& other_p) noexcept
    {
        this->m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ method& operator=(method&& rvalue_p) noexcept
    {
        this->m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ operator bool() const noexcept
    {
        return this->m_method_pointer != nullptr;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator==(const method& other_p) const noexcept
    {
        return (this->m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ boolean operator!=(const method& other_p) const noexcept
    {
        return (this->m_method_pointer != other_p.m_method_pointer) ? true : false;
    }
};




class task_base
{
public:
    task_base() noexcept = default;
    virtual ~task_base() noexcept = default;

	// For C style tasks
    virtual void operator()(FE::void_ptr out_ret_buffer_p, argument_base* const arguments_p) noexcept = 0;
    
    // For C++ style tasks
    virtual void operator()(FE::void_ptr instance_p, FE::void_ptr out_ret_buffer_p, argument_base* const arguments_p) noexcept = 0;

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
    using return_type = typename FE::method<class_type, task_impl_type>::return_type;
    
private:
    typename task_type::method_type m_method;

public:
    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ cpp_style_task() noexcept : m_method() {}
    virtual ~cpp_style_task() noexcept = default;

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ cpp_style_task(typename task_type::method_type task_p) noexcept : m_method(task_p) {}

	_FE_FORCE_INLINE_ void set_task(typename task_type::method_type task_p) noexcept { this->m_method = task_p; }
	_FE_FORCE_INLINE_ typename task_type::method_type get_task() const noexcept { return this->m_method; }

    virtual void operator()(FE::void_ptr instance_p, _FE_MAYBE_UNUSED_ FE::void_ptr out_ret_buffer_p, _FE_MAYBE_UNUSED_ argument_base* const arguments_p) noexcept override
    {
        FE_ASSERT(instance_p != nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_NULLPTR), TO_STRING(this->m_instance));
        C* l_object = instance_p.get<C*>();
        arguments_buffer_type* const l_arguments = dynamic_cast<arguments_buffer_type* const>(arguments_p);
        if constexpr (arguments_buffer_type::count != ARGUMENTS_COUNT::_0)
        {
            FE_ASSERT(l_arguments != nullptr, "Assertion Failure: failed to down cast an argument instance pointer from argument_base*.");
        }

        if constexpr (std::is_same<return_type, void>::value == true)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {
                (l_object->*m_method)();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                (l_object->*m_method)(l_arguments->_first);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth, l_arguments->_tenth);
            }
        }
        else if constexpr (std::is_same<return_type, void>::value == false)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth, l_arguments->_tenth);
            }
        }
    }

    virtual boolean is_null() const noexcept override
    {
        if (!this->m_method)
        {
            return true;
        }

        return false;
    }

    // for C style tasks
    virtual void operator()(_FE_MAYBE_UNUSED_ FE::void_ptr out_ret_buffer_p, _FE_MAYBE_UNUSED_ argument_base* const arguments_p) noexcept override
    {
        FE_ASSERT(false, "Invalid FE::cpp_style_task invocation");
    }

};


template<typename TaskImpl, class ArgumentsBufferType = typename FE::function<TaskImpl>::arguments_type>
class c_style_task : public task_base
{
public:
    using task_impl_type = TaskImpl;
    using task_type = FE::function<task_impl_type>;
    using arguments_buffer_type = ArgumentsBufferType;
    using return_type = typename FE::function<task_impl_type>::return_type;

private:
    typename task_type::function_type m_function;

public:
    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ c_style_task() noexcept : m_function() {}
    virtual ~c_style_task() noexcept = default;

    _FE_FORCE_INLINE_ _FE_CONSTEXPR20_ c_style_task(typename task_type::function_type task_p) noexcept : m_function(task_p) {}

    _FE_FORCE_INLINE_ void set_task(typename task_type::function_type task_p) noexcept { this->m_function = task_p; }
    _FE_FORCE_INLINE_ typename task_type::function_type get_task() const noexcept { return this->m_function; }

    virtual void operator()(_FE_MAYBE_UNUSED_ FE::void_ptr out_ret_buffer_p, _FE_MAYBE_UNUSED_ argument_base* const arguments_p) noexcept override
    {
        arguments_buffer_type* const l_arguments = dynamic_cast<arguments_buffer_type* const>(arguments_p);
        if constexpr (arguments_buffer_type::count != ARGUMENTS_COUNT::_0)
        {
            FE_NEGATIVE_ASSERT(l_arguments == nullptr, "Assertion Failure: failed to down cast an argument instance pointer from argument_base*.");
        }

        if constexpr (std::is_same<return_type, void>::value == true)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {
                this->m_function();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                this->m_function(l_arguments->_first);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                this->m_function(l_arguments->_first, l_arguments->_second);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth, l_arguments->_tenth);
            }
        }
        else if constexpr (std::is_same<return_type, void>::value == false)
        {
            if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_0)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function();
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_1)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function(l_arguments->_first);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_2)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function(l_arguments->_first, l_arguments->_second);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_3)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_4)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_5)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_6)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_7)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_8)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_9)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth);
            }
            else if constexpr (arguments_buffer_type::count == ARGUMENTS_COUNT::_10)
            {
                *(out_ret_buffer_p.get<return_type*>()) = this->m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth, l_arguments->_tenth);
            }
        }
    }

    virtual boolean is_null() const noexcept override
    {
        if (!this->m_function)
        {
            return true;
        }

        return false;
    }

    // for C++ style tasks
    virtual void operator()(_FE_MAYBE_UNUSED_ FE::void_ptr instance_p, _FE_MAYBE_UNUSED_ FE::void_ptr out_ret_buffer_p, _FE_MAYBE_UNUSED_ argument_base* const arguments_p) noexcept override
    {
        FE_ASSERT(false, "Invalid FE::c_style_task invocation");
    }
};

END_NAMESPACE
#endif