// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/type_traits.hxx>

// std
#include <cassert>
#include <iostream>
#include <string>
#include <string_view>


<<<<<<< HEAD


class base
{

};

class derived : public base
{

};


class object
{
public:
    object() noexcept {}

    ~object() noexcept {}

    object(_MAYBE_UNUSED_ const object& other_p) noexcept
    {
        std::cout << "\nobject copied\n";
    }

    object(_MAYBE_UNUSED_ object&& rvalue_p) noexcept
    {
        std::cout << "\nobject moved\n";
    }

    object& operator=(_MAYBE_UNUSED_ const object& other_p) noexcept
    {
        std::cout << "\nobject copied\n";
        return *this;
    }
    
    object& operator=(_MAYBE_UNUSED_ object&& rvalue_p) noexcept
    {
        std::cout << "\nobject moved\n";
        return *this;
    }
};

=======
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
/*
ls: lists all folders in the current working directory
ls filename: checks the presence of files with the specified name.
*/
int main()
{
<<<<<<< HEAD

=======
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
    _MAYBE_UNUSED_ FE::remove_const_reference<const int&>::type i = 0;

    _MAYBE_UNUSED_ FE::conditional_type<false, int, float>::type false_type = 0.0f;

    assert(FE::is_c_style_constant_string<const char*>::value == true);
    assert(FE::is_char<FE::UTF16>::value == true);
    assert(FE::is_boolean<FE::boolean>::value == true);
    assert(FE::is_primitive<FE::float64>::value == true);
    assert(FE::is_numeric<FE::int64>::value == true);
    assert(FE::is_trivial<std::string>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL);
    assert(FE::is_string_class<std::string_view>::value == true);
<<<<<<< HEAD
    assert(FE::has_base_type<int>::value == false);

    derived* l_derived = new derived;
    _MAYBE_UNUSED_ base* l_base = FE::polymorphic_cast<base*>(l_derived);
    delete l_derived;

    object l_source;
    _MAYBE_UNUSED_ object& l_assign_by_copy = FE::forward_to<object&, decltype(l_source)>(l_source); 
    _MAYBE_UNUSED_ const object& l_assign_by_move = FE::forward_to<const object&, decltype(l_source)>(l_source);
    _MAYBE_UNUSED_ object&& l_assign_by_move2 = FE::forward_to<object&&, decltype(l_source)>(l_source);
    _MAYBE_UNUSED_ object l_assign_by_move3 = FE::forward_to<object, decltype(l_source)>(l_source);
=======

>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
    std::cout << "Compilation test of FE.core.type_traits source code is successful.\n";
    return 0;
}






