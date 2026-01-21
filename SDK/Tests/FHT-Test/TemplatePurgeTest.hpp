// custom_header_test.hpp
// Comprehensive C++ Template Syntax Test Header

#ifndef CUSTOM_HEADER_TEST_HPP
#define CUSTOM_HEADER_TEST_HPP

#include <type_traits>
#include <iostream>
#include <vector>
#include <memory>

// ============================================================================
// 1. Basic Template Declarations - typename vs class
// ============================================================================

template <typename T>
class BasicTemplateTypename {
public:
    T value;
};

template <class T>
class BasicTemplateClass {
public:
    T value;
};

// ============================================================================
// 2. Multiple Template Parameters
// ============================================================================

template <typename T, typename U>
struct Pair {
    T first;
    U second;
};

// ============================================================================
// 3. Default Template Arguments
// ============================================================================

template <typename T = int, typename U = double>
class DefaultArgs {
public:
    T t_value;
    U u_value;
};

// ============================================================================
// 4. Non-Type Template Parameters
// ============================================================================

template <int N>
struct IntArray {
    int data[N];
};

template <size_t Size>
class FixedBuffer {
    char buffer[Size];
};

template <typename T, T value>
struct ValueWrapper {
    static constexpr T val = value;
};

// ============================================================================
// 5. Auto Non-Type Template Parameters (C++17)
// ============================================================================

template <auto N>
struct AutoValue {
    static constexpr auto value = N;
};

// ============================================================================
// 6. Variadic Templates
// ============================================================================

template <typename... Args>
class VariadicClass {
public:
    static constexpr size_t count = sizeof...(Args);
};

template <typename... Args>
void variadicFunction(Args... args) {
    ((std::cout << args << " "), ...); // fold expression C++17
}

// ============================================================================
// 7. Template Template Parameters
// ============================================================================

template <template <typename> class Container>
class TemplateTemplateClass {
public:
    Container<int> int_container;
    Container<double> double_container;
};

template <template <typename> typename Container>  // C++17 style
class TemplateTemplateTypename {
public:
    Container<int> container;
};

// ============================================================================
// 8. Variable Templates (C++14)
// ============================================================================

template <typename T>
constexpr T pi = T(3.1415926535897932385);

template <typename T>
constexpr bool is_pointer_v = std::is_pointer<T>::value;

// ============================================================================
// 9. Template Alias (C++11)
// ============================================================================

template <typename T>
using Vec = std::vector<T>;

template <typename T>
using Ptr = std::unique_ptr<T>;

// ============================================================================
// 10. Full Template Specialization
// ============================================================================

template <typename T>
class Specialized {
public:
    void print() { std::cout << "Generic version\n"; }
};

template <>
class Specialized<int> {
public:
    void print() { std::cout << "Int specialization\n"; }
};

template <>
class Specialized<double> {
public:
    void print() { std::cout << "Double specialization\n"; }
};

// ============================================================================
// 11. Partial Template Specialization
// ============================================================================

template <typename T, typename U>
class PartialSpec {
public:
    void print() { std::cout << "Primary template\n"; }
};

template <typename T>
class PartialSpec<T, int> {
public:
    void print() { std::cout << "Partial specialization: second is int\n"; }
};

template <typename T>
class PartialSpec<T*, T*> {
public:
    void print() { std::cout << "Partial specialization: both pointers\n"; }
};

// ============================================================================
// 12. Dependent Type Names - typename keyword
// ============================================================================

template <typename T>
class DependentTypes {
public:
    // Use typename to indicate that T::value_type is a type
    using ValueType = typename T::value_type;

    typename T::iterator begin();
    typename T::const_iterator cbegin() const;

    // Nested dependent type
    template <typename U>
    typename U::nested_type getNestedType() {
        return typename U::nested_type{};
    }
};

// ============================================================================
// 13. Dependent Template Names - template keyword
// ============================================================================

template <typename T>
class DependentTemplate {
public:
    // Use template keyword for dependent template member
    template <typename U>
    void callDependentTemplate(T& obj) {
        obj.template member<U>();
    }

    template <typename U>
    auto accessNestedTemplate(T& obj) {
        return obj.template get<U>();
    }
};

template <typename T>
class HasTemplateMethod {
public:
    template <typename U>
    void member() {
        std::cout << "Template member called\n";
    }

    template <typename U>
    U get() {
        return U{};
    }
};

// ============================================================================
// 14. Template with Static Assert
// ============================================================================

template <typename T>
class OnlyIntegral {
    static_assert(std::is_integral<T>::value, "T must be integral type");
public:
    T value;
};

// ============================================================================
// 15. Friend Templates
// ============================================================================

template <typename T>
class FriendTest;

template <typename T>
void friendFunction(const FriendTest<T>&);

template <typename T>
class FriendTest {
private:
    T data;

    // Friend function template
    template <typename U>
    friend void friendFunction(const FriendTest<U>&);

    // Friend class template
    template <typename U>
    friend class FriendTest;
};

// ============================================================================
// 16. Member Function Templates
// ============================================================================

class MemberFunctionTemplates {
public:
    template <typename T>
    void memberTemplate(T value) {
        std::cout << "Member template: " << value << "\n";
    }

    template <typename T, typename U>
    auto convert(T value) -> U {
        return static_cast<U>(value);
    }
};

// ============================================================================
// 17. Explicit Instantiation Declaration
// ============================================================================

extern template class BasicTemplateTypename<int>;
extern template class BasicTemplateTypename<double>;

// ============================================================================
// 18. SFINAE with std::enable_if
// ============================================================================

template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
class OnlyForIntegral {
public:
    T value;
};

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, void>::type
processFloat(T value) {
    std::cout << "Float: " << value << "\n";
}

// ============================================================================
// 19. Concepts (C++20) - if supported
// ============================================================================

#if __cplusplus >= 202002L

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric T>
class ConceptClass {
public:
    T value;
};

template <typename T>
    requires Numeric<T>
T add(T a, T b) {
    return a + b;
}

template <typename T>
concept HasSize = requires(T t) {
    { t.size() } -> std::convertible_to<size_t>;
};

template <HasSize T>
size_t getSize(const T& container) {
    return container.size();
}

#endif

// ============================================================================
// 20. Variadic Template with Parameter Pack Expansion
// ============================================================================

template <typename... Types>
struct TypeList {
    static constexpr size_t size = sizeof...(Types);
};

template <typename T, typename... Args>
std::unique_ptr<T> makeUnique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// ============================================================================
// 21. Complex Nested Template Example
// ============================================================================

template <typename T>
class Outer {
public:
    template <typename U>
    class Inner {
    public:
        T outer_value;
        U inner_value;

        template <typename V>
        void tripleNested(V value) {
            std::cout << "Triple nested template\n";
        }
    };

    // Dependent name resolution
    template <typename U>
    typename Inner<U>::template_type getInnerType();
};

// ============================================================================
// 22. Template with decltype and trailing return type
// ============================================================================

template <typename T, typename U>
auto multiply(T t, U u) -> decltype(t* u) {
    return t * u;
}

// ============================================================================
// 23. Template with constexpr
// ============================================================================

template <typename T>
constexpr T square(T value) {
    return value * value;
}

template <int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

// ============================================================================
// 24. Perfect Forwarding
// ============================================================================

template <typename T>
class Wrapper {
    T value;
public:
    template <typename U>
    Wrapper(U&& val) : value(std::forward<U>(val)) {}
};

// ============================================================================
// 25. Explicit Instantiation Definition (in .cpp file, shown here for demo)
// ============================================================================

// These would typically go in a .cpp file:
// template class BasicTemplateTypename<float>;
// template void variadicFunction<int, double>(int, double);

#endif // CUSTOM_HEADER_TEST_HPP