#ifndef _FE_CORE_FUNCTION_HXX_
#define _FE_CORE_FUNCTION_HXX_
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
#include <FE/type_traits.hxx>

// std
#include <any>




BEGIN_NAMESPACE(FE)


class void_t{};


enum class ArgumentCount : uint8
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


/*
The arguments class template in the _FE namespace is designed to hold up to ten template parameters
providing a way to manage and access a collection of types and their corresponding values
with the last type being customizable through the Tenth template parameter.
*/
template<   typename First = void, 
            typename Second = void, 
            typename Third = void, 
            typename Fourth = void, 
            typename Fifth = void, 
            typename Sixth = void, 
            typename Seventh = void, 
            typename Eighth = void, 
            typename Ninth = void, 
            typename Tenth = void>
class arguments;

template<>
class arguments<void, void, void, void, void, void, void, void, void, void> : public argument_base
{
public:
    static constexpr inline ArgumentCount count = ArgumentCount::_0;
};

template<typename First>
class arguments<First, void, void, void, void, void, void, void, void, void> : public argument_base
{
public:
    using first_type = std::conditional_t< FE::is_const_reference_v<First>, 
        FE::const_ref<FE::remove_const_reference_t<First>>, 

        std::conditional_t< std::is_reference_v<First>, 
        FE::ref<FE::remove_const_reference_t<First>>, 
        First 
        >
    >;

    first_type _first;
    static constexpr inline ArgumentCount count = ArgumentCount::_1;

    arguments() noexcept 
        : _first() {}

    arguments(const first_type& first_p) noexcept
        : _first(first_p) {}
};

template<typename First, typename Second>
class arguments<First, Second, void, void, void, void, void, void, void, void> : public arguments<First, void, void, void, void, void, void, void, void, void>
{
public:
    using base_type = arguments<First, void, void, void, void, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = std::conditional_t < FE::is_const_reference_v<Second>,
        FE::const_ref<FE::remove_const_reference_t<Second>>,

        std::conditional_t< std::is_reference_v<Second>,
        FE::ref<FE::remove_const_reference_t<Second>>,
        Second
        >
    >;

    second_type _second;
    static constexpr inline ArgumentCount count = ArgumentCount::_2;

    arguments() noexcept 
        : base_type(), _second() {}
    
    arguments(const first_type& first_p, const second_type& second_p) noexcept
        : base_type(first_p), _second(second_p) {}
};

template<typename First, typename Second, typename Third>
class arguments<First, Second, Third, void, void, void, void, void, void, void> : public arguments<First, Second, void, void, void, void, void, void, void, void>
{
public:
    using base_type = arguments<First, Second, void, void, void, void, void, void, void, void>;
    using first_type = typename base_type::first_type;
    using second_type = typename base_type::second_type;
    using third_type = std::conditional_t < FE::is_const_reference_v<Third>,
        FE::const_ref<FE::remove_const_reference_t<Third>>,

        std::conditional_t< std::is_reference_v<Third>,
        FE::ref<FE::remove_const_reference_t<Third>>,
        Third
        >
    >;
   
    third_type _third;
    static constexpr inline ArgumentCount count = ArgumentCount::_3;

    arguments() noexcept 
        : base_type(), _third() {}
    
    arguments(const first_type& first_p, const second_type& second_p, const third_type& third_p) noexcept
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
    using fourth_type = std::conditional_t < FE::is_const_reference_v<Fourth>,
        FE::const_ref<FE::remove_const_reference_t<Fourth>>,

        std::conditional_t< std::is_reference_v<Fourth>,
        FE::ref<FE::remove_const_reference_t<Fourth>>,
        Fourth
        >
    >;

    fourth_type _fourth;
    static constexpr inline ArgumentCount count = ArgumentCount::_4;

    arguments() noexcept
        : base_type(), _fourth() {}

    arguments(const first_type& first_p, const second_type& second_p, const third_type& third_p, const fourth_type& fourth_p) noexcept
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
    using fifth_type = std::conditional_t < FE::is_const_reference_v<Fifth>,
        FE::const_ref<FE::remove_const_reference_t<Fifth>>,

        std::conditional_t< std::is_reference_v<Fifth>,
        FE::ref<FE::remove_const_reference_t<Fifth>>,
        Fifth
        >
    >;

    fifth_type _fifth;
    static constexpr inline ArgumentCount count = ArgumentCount::_5;

    arguments() noexcept 
        : base_type(), _fifth() {}

    arguments(const first_type& first_p, const second_type& second_p, const third_type& third_p, const fourth_type& fourth_p,
                             const fifth_type& fifth_p) noexcept
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
    using sixth_type = std::conditional_t < FE::is_const_reference_v<Sixth>,
        FE::const_ref<FE::remove_const_reference_t<Sixth>>,

        std::conditional_t< std::is_reference_v<Sixth>,
        FE::ref<FE::remove_const_reference_t<Sixth>>,
        Sixth
        >
    >;

    sixth_type _sixth;
    static constexpr inline ArgumentCount count = ArgumentCount::_6;

    arguments() noexcept 
        : base_type(), _sixth() {}

    arguments(const first_type& first_p, const second_type& second_p, const third_type& third_p, const fourth_type& fourth_p,
                             const fifth_type& fifth_p, const sixth_type& sixth_p) noexcept
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
    using seventh_type = std::conditional_t < FE::is_const_reference_v<Seventh>,
        FE::const_ref<FE::remove_const_reference_t<Seventh>>,

        std::conditional_t< std::is_reference_v<Seventh>,
        FE::ref<FE::remove_const_reference_t<Seventh>>,
        Seventh
        >
    >;

    seventh_type _seventh;
    static constexpr inline ArgumentCount count = ArgumentCount::_7;


    arguments() noexcept
        : base_type(), _seventh() {}

    arguments(const first_type& first_p, const second_type& second_p, const third_type& third_p, const fourth_type& fourth_p,
                             const fifth_type& fifth_p, const sixth_type& sixth_p, const seventh_type& seventh_p) noexcept
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
    using eighth_type = std::conditional_t < FE::is_const_reference_v<Eighth>,
        FE::const_ref<FE::remove_const_reference_t<Eighth>>,

        std::conditional_t< std::is_reference_v<Eighth>,
        FE::ref<FE::remove_const_reference_t<Eighth>>,
        Eighth
        >
    >;

    eighth_type _eighth;
    static constexpr inline ArgumentCount count = ArgumentCount::_8;

    arguments() noexcept
        : base_type(), _eighth() {}

    arguments(const first_type& first_p, const second_type& second_p, const third_type& third_p, const fourth_type& fourth_p,
                             const fifth_type& fifth_p, const sixth_type& sixth_p, const seventh_type& seventh_p, const eighth_type& eighth_p)
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
    using ninth_type = std::conditional_t < FE::is_const_reference_v<Ninth>,
        FE::const_ref<FE::remove_const_reference_t<Ninth>>,

        std::conditional_t< std::is_reference_v<Ninth>,
        FE::ref<FE::remove_const_reference_t<Ninth>>,
        Ninth
        >
    >;

    ninth_type _ninth;
    static constexpr inline ArgumentCount count = ArgumentCount::_9;

    arguments() noexcept 
        : base_type(), _ninth() {}

    arguments(const first_type& first_p, const second_type& second_p, const third_type& third_p, const fourth_type& fourth_p,
                             const fifth_type& fifth_p, const sixth_type& sixth_p, const seventh_type& seventh_p, const eighth_type& eighth_p,
                             const ninth_type& ninth_p) noexcept
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
    using tenth_type = std::conditional_t < FE::is_const_reference_v<Tenth>,
        FE::const_ref<FE::remove_const_reference_t<Tenth>>,

        std::conditional_t< std::is_reference_v<Tenth>,
        FE::ref<FE::remove_const_reference_t<Tenth>>,
        Tenth
        >
    >;

    tenth_type _tenth;
    static constexpr inline ArgumentCount count = ArgumentCount::_10;


    arguments() noexcept 
        : base_type(), _tenth() {}
        
    arguments(const first_type& first_p, const second_type& second_p, const third_type& third_p, const fourth_type& fourth_p,
                             const fifth_type& fifth_p, const sixth_type& sixth_p, const seventh_type& seventh_p, const eighth_type& eighth_p,
                             const ninth_type& ninth_p, const tenth_type& tenth_p) noexcept
        : base_type(first_p, second_p, third_p, fourth_p, fifth_p, sixth_p, seventh_p, eighth_p, ninth_p), _tenth(tenth_p) {}
};


/*
The cpp_style_task class template in the FE namespace encapsulates a callable task that can invoke a member function of a specified class type with a variable number of arguments
ensuring type safety and supporting both void and non-void return types.
*/
template<class C, typename TaskImpl, class ArgumentsBufferType>
class cpp_style_task;


template<typename TaskImpl, class ArgumentsBufferType>
class c_style_task;


/*
The FE::function<R, Arguments...> class template is a lightweight wrapper for function pointers that allows invoking functions with specified return and argument types
supporting both copy and move semantics.
*/
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
    using arguments_type = FE::arguments<Arguments...>;

    constexpr function() noexcept = default;
    ~function() noexcept = default;

    constexpr function(function_type function_p) noexcept : m_function_pointer(function_p) {}
    constexpr function(const function& other_p) noexcept : m_function_pointer(other_p.m_function_pointer) {}

    _FE_FORCE_INLINE_ R operator()(Arguments&& ...arguments_p) noexcept
    {
        FE_NEGATIVE_ASSERT(m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(m_function_pointer));
		if constexpr (std::is_same<R, void>::value == true)
        {
			m_function_pointer(arguments_p...);
		}
        else if constexpr (std::is_same<R, void>::value == false)
        {
			return m_function_pointer(arguments_p...);
		}
    }

    _FE_FORCE_INLINE_ const R operator()(Arguments&& ...arguments_p) const noexcept
    {
        FE_NEGATIVE_ASSERT(m_function_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(m_function_pointer));
        if constexpr (std::is_same<R, void>::value == true)
        {
            m_function_pointer(arguments_p...);
        }
        else if constexpr (std::is_same<R, void>::value == false)
        {
            return m_function_pointer(arguments_p...);
        }
    }

    _FE_FORCE_INLINE_ constexpr function& operator=(function_type function_p) noexcept
    {
        m_function_pointer = function_p;
        return *this;
    }

    _FE_FORCE_INLINE_ constexpr function& operator=(const function& other_p) noexcept
    {
        m_function_pointer = other_p.m_function_pointer;
        return *this;
    }

    _FE_FORCE_INLINE_ constexpr function& operator=(function&& rvalue_p) noexcept
    {
        m_function_pointer = rvalue_p.m_function_pointer;
        rvalue_p.m_function_pointer = nullptr;
        return *this;
    }

    _FE_FORCE_INLINE_ constexpr operator bool() const noexcept
    {
        return m_function_pointer != nullptr;
    }

    _FE_FORCE_INLINE_ constexpr boolean operator==(const function& other_p) const noexcept
    {
        return (m_function_pointer == other_p.m_function_pointer) ? true : false;
    }

    _FE_FORCE_INLINE_ constexpr boolean operator!=(const function& other_p) const noexcept
    {
        return (m_function_pointer != other_p.m_function_pointer) ? true : false;
    }

    _FE_FORCE_INLINE_ function_type get_raw() const noexcept
    {
        return m_function_pointer;
	}
};


/*
The FE::method class template is a utility that encapsulates a pointer to a const member function of a specified class
allowing for invocation with a given set of arguments while providing type safety and convenience.
*/
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
    using arguments_type = FE::arguments<Arguments...>;
    using class_type = C;

    constexpr method() noexcept = default;
    ~method() noexcept = default;

    constexpr method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    constexpr method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}

    _FE_FORCE_INLINE_ R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_NEGATIVE_ASSERT(m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(m_method_pointer));
        if constexpr (std::is_same<R, void>::value == true)
        {
            (host_class_instance_p.*m_method_pointer)(arguments_p...);
        }
        else if constexpr (std::is_same<R, void>::value == false)
        {
            return (host_class_instance_p.*m_method_pointer)(arguments_p...);
        }
    }

    _FE_FORCE_INLINE_ const R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) const noexcept
    {
        FE_NEGATIVE_ASSERT(m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(m_method_pointer));
        if constexpr (std::is_same<R, void>::value == true)
        {
            (host_class_instance_p.*m_method_pointer)(arguments_p...);
        }
        else if constexpr (std::is_same<R, void>::value == false)
        {
            return (host_class_instance_p.*m_method_pointer)(arguments_p...);
        }
    }

    _FE_FORCE_INLINE_ constexpr method& operator=(method_type method_p) noexcept
    {
        m_method_pointer = method_p;
        return *this;
    }

    _FE_FORCE_INLINE_ constexpr method& operator=(const method& other_p) noexcept
    {
        m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _FE_FORCE_INLINE_ constexpr method& operator=(method&& rvalue_p) noexcept
    {
        m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _FE_FORCE_INLINE_ constexpr operator bool() const noexcept
    {
        return m_method_pointer != nullptr;
    }

    _FE_FORCE_INLINE_ constexpr boolean operator==(const method& other_p) const noexcept
    {
        return (m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _FE_FORCE_INLINE_ constexpr boolean operator!=(const method& other_p) const noexcept
    {
        return (m_method_pointer != other_p.m_method_pointer) ? true : false;
    }

    _FE_FORCE_INLINE_ method_type get_raw() const noexcept
    {
        return m_method_pointer;
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
    using arguments_type = FE::arguments<Arguments...>;
    using class_type = C;

    constexpr method() noexcept = default;
    ~method() noexcept = default;

    constexpr method(method_type method_p) noexcept : m_method_pointer(method_p) {}
    constexpr method(const method& other_p) noexcept : m_method_pointer(other_p.m_method_pointer) {}

    _FE_FORCE_INLINE_ R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) noexcept
    {
        FE_NEGATIVE_ASSERT(m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(m_method_pointer));
        if constexpr (std::is_same<R, void>::value == true)
        {
            (host_class_instance_p.*m_method_pointer)(arguments_p...);
        }
		else if constexpr (std::is_same<R, void>::value == false)
        {
            return (host_class_instance_p.*m_method_pointer)(arguments_p...);
		}
    }

    _FE_FORCE_INLINE_ const R operator()(class_type& host_class_instance_p, Arguments&& ...arguments_p) const noexcept
    {
        FE_NEGATIVE_ASSERT(m_method_pointer == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(m_method_pointer));
        if constexpr (std::is_same<R, void>::value == true)
        {
            (host_class_instance_p.*m_method_pointer)(arguments_p...);
        }
        else if constexpr (std::is_same<R, void>::value == false)
        {
            return (host_class_instance_p.*m_method_pointer)(arguments_p...);
        }
    }

    _FE_FORCE_INLINE_ constexpr method& operator=(method_type method_p) noexcept
    {
        m_method_pointer = method_p;
        return *this;
    }

    _FE_FORCE_INLINE_ constexpr method& operator=(const method& other_p) noexcept
    {
        m_method_pointer = other_p.m_method_pointer;
        return *this;
    }

    _FE_FORCE_INLINE_ constexpr method& operator=(method&& rvalue_p) noexcept
    {
        m_method_pointer = rvalue_p.m_method_pointer;
        rvalue_p.m_method_pointer = nullptr;
        return *this;
    }

    _FE_FORCE_INLINE_ constexpr operator bool() const noexcept
    {
        return m_method_pointer != nullptr;
    }

    _FE_FORCE_INLINE_ constexpr boolean operator==(const method& other_p) const noexcept
    {
        return (m_method_pointer == other_p.m_method_pointer) ? true : false;
    }

    _FE_FORCE_INLINE_ constexpr boolean operator!=(const method& other_p) const noexcept
    {
        return (m_method_pointer != other_p.m_method_pointer) ? true : false;
    }

    _FE_FORCE_INLINE_ method_type get_raw() const noexcept
    {
        return m_method_pointer;
    }
};


class component_base;
/*
The FE::task_base class is an abstract base class designed for defining tasks that can be executed with both C and C++ style function calls
providing a virtual function interface to check for null function pointers.
*/
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

    using system = void(*)(class FE::component_base* const);
	virtual system try_get_as_system() noexcept = 0;
};


template<class C, typename TaskImpl, class ArgumentsBufferType = typename FE::method<C, TaskImpl>::arguments_type>
class cpp_style_task : public task_base
{
    static_assert(std::is_class<C>::value == true, "C must be a class type.");

public:
    using class_type = C;
    using task_impl_type = TaskImpl;
    using return_type = typename FE::method<class_type, task_impl_type>::return_type;

    using task_type = typename FE::method<class_type, task_impl_type>::method_type;
    using arguments_buffer_type = ArgumentsBufferType;
    
private:
    task_type m_method;

public:
    cpp_style_task() noexcept : m_method() {}
    virtual ~cpp_style_task() noexcept = default;

    constexpr cpp_style_task(task_type task_p) noexcept : m_method(task_p) {}

	_FE_FORCE_INLINE_ void set_task(task_type task_p) noexcept { m_method = task_p; }
	_FE_FORCE_INLINE_ task_type get_task() const noexcept { return m_method; }

    virtual void operator()(FE::void_ptr instance_p, _FE_MAYBE_UNUSED_ FE::void_ptr out_ret_buffer_p, _FE_MAYBE_UNUSED_ argument_base* const arguments_p) noexcept override
    {
        FE_ASSERT(instance_p != nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(m_instance));
        C* l_object = instance_p.get<C*>();
        arguments_buffer_type* const l_arguments = FE::polymorphic_cast<arguments_buffer_type* const>(arguments_p);
        if constexpr (arguments_buffer_type::count != ArgumentCount::_0)
        {
            FE_ASSERT(l_arguments != nullptr, "Assertion Failure: failed to down cast an argument instance pointer from argument_base*.");
        }

        if constexpr (std::is_same<return_type, void>::value == true)
        {
            if constexpr (arguments_buffer_type::count == ArgumentCount::_0)
            {
                (l_object->*m_method)();
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_1)
            {
                (l_object->*m_method)(l_arguments->_first);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_2)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_3)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_4)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_5)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_6)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_7)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_8)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_9)
            {
                (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_10)
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
            if constexpr (arguments_buffer_type::count == ArgumentCount::_0)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)();
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_1)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_2)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_3)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_4)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_5)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_6)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_7)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_8)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_9)
            {
                *(out_ret_buffer_p.get<return_type*>()) = (l_object->*m_method)(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_10)
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
        if (!m_method)
        {
            return true;
        }

        return false;
    }

    // for C style tasks
    virtual void operator()(_FE_MAYBE_UNUSED_ FE::void_ptr out_ret_buffer_p, _FE_MAYBE_UNUSED_ argument_base* const arguments_p) noexcept override
    {
        FE_EXIT_IF(true, FE::ErrorCode::_FatalSerializationError_3XX_TypeMismatch, "This operator() overload is not supported for C++ style tasks.");
    }

    virtual system try_get_as_system() noexcept override
    {
        FE_EXIT_IF(true, FE::ErrorCode::_FatalSerializationError_3XX_TypeMismatch, "This method is not supported for C++ style tasks.");
        return nullptr;
	}

};


/*
The FE::c_style_task class template encapsulates a callable task with a specified implementation type and an optional arguments buffer type
allowing for flexible execution of functions with varying numbers of arguments and return types.
*/
template<typename TaskImpl, class ArgumentsBufferType = typename FE::function<TaskImpl>::arguments_type>
class c_style_task : public task_base
{
public:
    using task_impl_type = TaskImpl;
    using return_type = typename FE::function<task_impl_type>::return_type;

    using task_type = typename FE::function<task_impl_type>::function_type;
    using arguments_buffer_type = ArgumentsBufferType;

private:
    task_type m_function;

public:
    c_style_task() noexcept : m_function() {}
    virtual ~c_style_task() noexcept = default;

    constexpr c_style_task(task_type task_p) noexcept : m_function(task_p) {}

    _FE_FORCE_INLINE_ void set_task(task_type task_p) noexcept { m_function = task_p; }
    _FE_FORCE_INLINE_ task_type get_task() const noexcept { return m_function; }

    virtual void operator()(_FE_MAYBE_UNUSED_ FE::void_ptr out_ret_buffer_p, _FE_MAYBE_UNUSED_ argument_base* const arguments_p) noexcept override
    {
        arguments_buffer_type* const l_arguments = FE::polymorphic_cast<arguments_buffer_type* const>(arguments_p);
        if constexpr (arguments_buffer_type::count != ArgumentCount::_0)
        {
            FE_NEGATIVE_ASSERT(l_arguments == nullptr, "Assertion Failure: failed to down cast an argument instance pointer from argument_base*.");
        }

        if constexpr (std::is_same<return_type, void>::value == true)
        {
            if constexpr (arguments_buffer_type::count == ArgumentCount::_0)
            {
                m_function();
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_1)
            {
                m_function(l_arguments->_first);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_2)
            {
                m_function(l_arguments->_first, l_arguments->_second);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_3)
            {
                m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_4)
            {
                m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_5)
            {
                m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_6)
            {
                m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_7)
            {
                m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_8)
            {
                m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_9)
            {
                m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_10)
            {
                m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth, l_arguments->_tenth);
            }
        }
        else if constexpr (std::is_same<return_type, void>::value == false)
        {
            if constexpr (arguments_buffer_type::count == ArgumentCount::_0)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function();
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_1)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function(l_arguments->_first);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_2)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function(l_arguments->_first, l_arguments->_second);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_3)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_4)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_5)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_6)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_7)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_8)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_9)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth);
            }
            else if constexpr (arguments_buffer_type::count == ArgumentCount::_10)
            {
                *(out_ret_buffer_p.get<return_type*>()) = m_function(l_arguments->_first, l_arguments->_second,
                    l_arguments->_third, l_arguments->_fourth,
                    l_arguments->_fifth, l_arguments->_sixth,
                    l_arguments->_seventh, l_arguments->_eighth,
                    l_arguments->_ninth, l_arguments->_tenth);
            }
        }
    }

    virtual boolean is_null() const noexcept override
    {
        if (!m_function)
        {
            return true;
        }

        return false;
    }

    // for C++ style tasks
    virtual void operator()(_FE_MAYBE_UNUSED_ FE::void_ptr instance_p, _FE_MAYBE_UNUSED_ FE::void_ptr out_ret_buffer_p, _FE_MAYBE_UNUSED_ argument_base* const arguments_p) noexcept override
    {
        FE_EXIT_IF(true, FE::ErrorCode::_FatalSerializationError_3XX_TypeMismatch, "This operator() overload is not supported for C style tasks");
    }

    virtual system try_get_as_system() noexcept override
    {
        if constexpr (std::is_same<task_type, typename FE::task_base::system>::value == true)
        {
            return m_function;
        }
        return nullptr;
    }
};


END_NAMESPACE
#endif