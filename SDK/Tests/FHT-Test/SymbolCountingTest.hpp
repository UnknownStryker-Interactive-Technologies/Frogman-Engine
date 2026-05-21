/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#ifndef TEST_REFLECTION_COMPREHENSIVE_HPP
#define TEST_REFLECTION_COMPREHENSIVE_HPP

#include <FE/prerequisites.hxx>
#include <cstdint>
#include <string>


// ============================================================================
// SECTION 1: Global Namespace Declarations
// ============================================================================

// Global enum struct - simple
enum struct GlobalStatus
{
	Inactive,
	Active,
	Pending
};

// Global enum struct - with explicit underlying type
enum struct GlobalErrorCode : uint16_t
{
	Success = 0,
	GeneralError = 1,
	InvalidInput = 100,
	OutOfMemory = 200
};

// Global enum struct - with trailing comma
enum struct GlobalPriority : int32_t
{
	Low,
	Medium,
	High,
};

// Global struct - POD style
struct GlobalPoint
{
	float _x;
	float _y;
	float _z;
};

// Global struct - with constructor and methods
struct GlobalVector
{
	float _x;
	float _y;
	float _z;

	GlobalVector();
	GlobalVector(float x_p, float y_p, float z_p);
	~GlobalVector();

	float magnitude() const;
	void normalize();
	static GlobalVector zero();
};

// Global struct - with inheritance
struct GlobalTransform : public GlobalVector
{
	float _rotation;
	float _scale;

	GlobalTransform();
	virtual ~GlobalTransform();
	virtual void update(float delta_p);
};

// Global class - simple
class GlobalEntity
{
public:
	GlobalEntity();
	~GlobalEntity();

	void set_id(uint32_t id_p);
	uint32_t get_id() const;

private:
	uint32_t _id;
	GlobalStatus _status;
};

// Global class - with multiple access specifiers
class GlobalComponent
{
protected:
	bool _is_enabled;
	uint64_t _component_id;

public:
	GlobalComponent();
	virtual ~GlobalComponent();

	virtual void initialize() = 0;
	virtual void update(float delta_p) = 0;
	virtual void shutdown() = 0;

	bool is_enabled() const noexcept;
	void set_enabled(bool enabled_p) noexcept;

private:
	GlobalStatus _status;
};

// Global class - with virtual inheritance
class GlobalRenderComponent : public GlobalComponent
{
public:
	GlobalRenderComponent();
	virtual ~GlobalRenderComponent() override;

	void initialize() override;
	void update(float delta_p) override;
	void shutdown() override;

protected:
	float _opacity;

private:
	uint32_t _render_id;
};

// Global class - final keyword
class GlobalSingleton final
{
public:
	GlobalSingleton(const GlobalSingleton&) = delete;
	GlobalSingleton& operator=(const GlobalSingleton&) = delete;
	GlobalSingleton(GlobalSingleton&&) = delete;
	GlobalSingleton& operator=(GlobalSingleton&&) = delete;

	static GlobalSingleton& get_instance();

private:
	GlobalSingleton();
	~GlobalSingleton();

	int _state;
};

// Forward declarations - global
struct GlobalForwardStruct;
class GlobalForwardClass;


// ============================================================================
// SECTION 2: Single-Level Namespace
// ============================================================================

BEGIN_NAMESPACE(FE::Test)

// Enum struct - simple
enum struct Color
{
	Red,
	Green,
	Blue,
	Yellow
};

// Enum struct - with explicit type uint8_t
enum struct ByteStatus : uint8_t
{
	Idle = 0,
	Running = 1,
	Paused = 2,
	Stopped = 3,
	Error = 255
};

// Enum struct - with explicit type int64_t
enum struct LongCode : int64_t
{
	MinValue = -9223372036854775807LL,
	Zero = 0,
	MaxValue = 9223372036854775807LL
};

// Enum struct - with only one value
enum struct SingleValue
{
	Only
};

// Enum struct - empty (edge case)
enum struct EmptyEnum
{
};

// Forward declarations
struct ForwardDeclaredStruct;
class ForwardDeclaredClass;

// Struct - simple POD
struct Point2D
{
	float _x;
	float _y;
};

// Struct - multiple fields on same line
struct Rectangle
{
	float _x, _y, _width, _height;
};

// Struct - with constructor
struct Point3D
{
	float _x;
	float _y;
	float _z;

	Point3D();
	Point3D(float x_p, float y_p, float z_p);
};

// Struct - with various method types
struct Vector3D
{
	float _x;
	float _y;
	float _z;

	// Constructors
	Vector3D();
	Vector3D(float x_p, float y_p, float z_p);
	Vector3D(const Vector3D& other_p);
	Vector3D(Vector3D&& other_p) noexcept;

	// Destructor
	~Vector3D();

	// Assignment operators
	Vector3D& operator=(const Vector3D& other_p);
	Vector3D& operator=(Vector3D&& other_p) noexcept;

	// Const methods
	float magnitude() const;
	float dot(const Vector3D& other_p) const;
	float length_squared() const noexcept;

	// Non-const methods
	void normalize();
	void scale(float factor_p);

	// Static methods
	static Vector3D zero();
	static Vector3D one();
	static Vector3D up();

	// Inline methods
	inline float get_x() const { return _x; }
	inline void set_x(float x_p) { _x = x_p; }

	// Constexpr methods
	constexpr float sum() const { return _x + _y + _z; }

	// Operator overloading
	Vector3D operator+(const Vector3D& other_p) const;
	Vector3D operator-(const Vector3D& other_p) const;
	Vector3D operator*(float scalar_p) const;
	bool operator==(const Vector3D& other_p) const;
	bool operator!=(const Vector3D& other_p) const;
};

// Struct - with inheritance (single)
struct Transform : public Vector3D
{
	float _rotation;
	float _scale;

	Transform();
	virtual ~Transform();
	virtual void update(float delta_p);
	virtual void reset();
};

// Struct - with inheritance (multiple base classes)
struct PhysicsTransform : public Transform, public GlobalPoint
{
	float _mass;
	float _velocity;

	PhysicsTransform();
	~PhysicsTransform();

	void apply_force(float force_p);
	void update(float delta_p) override;
};

// Struct - with static members
struct Configuration
{
	static const uint32_t MAX_ENTITIES = 1000;
	static const float DEFAULT_TIMESTEP;
	static constexpr uint64_t BUFFER_SIZE = 4096;

	bool _enable_physics;
	bool _enable_rendering;
	uint32_t _max_fps;

	static void load(const char* path_p);
	static void save(const char* path_p);
	static Configuration& get_default();
};

// Struct - with bitfields
struct Flags
{
	uint32_t _is_visible : 1;
	uint32_t _is_collidable : 1;
	uint32_t _is_static : 1;
	uint32_t _is_dynamic : 1;
	uint32_t _reserved : 28;
};

// Struct - with volatile and const members
struct VolatileData
{
	volatile int _counter;
	const uint32_t _id;
	volatile const bool _flag;

	VolatileData(uint32_t id_p);
};

// Struct - with pointer and reference members
struct PointerData
{
	int* _ptr;
	const int* _const_ptr;
	int** _ptr_ptr;
	int* const _const_ptr_to_int;
	const int* const _const_ptr_to_const_int;

	void process(int& ref_p);
	void process_const(const int& ref_p) const;
	void process_pointer(int* ptr_p);
	void process_const_pointer(const int* ptr_p);
};

// Class - simple with private members only
class SimpleClass
{
private:
	int _value;
	float _data;
};

// Class - with public section
class PublicClass
{
public:
	PublicClass();
	~PublicClass();

	void public_method();
	int get_value() const;

private:
	int _private_value;
};

// Class - with all access specifiers
class FullAccessClass
{
public:
	FullAccessClass();
	~FullAccessClass();

	void public_method();
	int public_field;

protected:
	void protected_method();
	int _protected_field;

private:
	void private_method();
	int _private_field;
};

// Class - with virtual methods
class BaseClass
{
public:
	BaseClass();
	virtual ~BaseClass();

	virtual void virtual_method();
	virtual void pure_virtual_method() = 0;
	virtual int virtual_method_with_return() const;

protected:
	virtual void protected_virtual_method();

private:
	int _base_value;
};

// Class - derived with override
class DerivedClass : public BaseClass
{
public:
	DerivedClass();
	virtual ~DerivedClass() override;

	void virtual_method() override;
	void pure_virtual_method() override;
	int virtual_method_with_return() const override;

protected:
	void protected_virtual_method() override;

private:
	int _derived_value;
};

// Class - final class
class FinalClass final : public BaseClass
{
public:
	FinalClass();
	~FinalClass() override;

	void pure_virtual_method() override final;

private:
	int _final_value;
};

// Class - with deleted methods
class NonCopyableClass
{
public:
	NonCopyableClass();
	~NonCopyableClass();

	NonCopyableClass(const NonCopyableClass&) = delete;
	NonCopyableClass& operator=(const NonCopyableClass&) = delete;
	NonCopyableClass(NonCopyableClass&&) = delete;
	NonCopyableClass& operator=(NonCopyableClass&&) = delete;

private:
	int _value;
};

// Class - with friend declarations
class FriendClass;

class ClassWithFriend
{
public:
	ClassWithFriend();

private:
	int _private_data;

	friend class FriendClass;
	friend void global_friend_function(const ClassWithFriend& obj_p);
};

// Class - with static methods and members
class StaticClass
{
public:
	static void static_method();
	static int static_method_with_return();
	static const StaticClass& get_instance();

	static int s_public_static;
	static constexpr float s_pi = 3.14159f;

private:
	StaticClass();
	~StaticClass();

	static int s_private_static;
	static void private_static_method();
};

// Class - with thread_local members
class ThreadLocalClass
{
public:
	ThreadLocalClass();

	static thread_local int tl_value;
	static thread_local bool tl_flag;

private:
	thread_local int _instance_tl_value;
};

// Class - with constexpr, consteval, constinit
class ConstClass
{
public:
	static constexpr int s_constexpr_value = 100;
	static constinit int s_constinit_value;

	constexpr int constexpr_method() const { return 42; }
	consteval static int consteval_method() { return 84; }

private:
	int _value;
};

// Class - with noexcept methods
class NoexceptClass
{
public:
	NoexceptClass() noexcept;
	~NoexceptClass() noexcept;

	void noexcept_method() noexcept;
	int noexcept_method_with_return() const noexcept;
	void conditional_noexcept() noexcept(true);

private:
	int _value;
};

// Class - with multiple inheritance
class MultipleBaseA
{
public:
	virtual ~MultipleBaseA();
	virtual void method_a() = 0;

protected:
	int _a_value;
};

class MultipleBaseB
{
public:
	virtual ~MultipleBaseB();
	virtual void method_b() = 0;

protected:
	int _b_value;
};

class MultipleInheritance : public MultipleBaseA, public MultipleBaseB
{
public:
	MultipleInheritance();
	~MultipleInheritance() override;

	void method_a() override;
	void method_b() override;

private:
	int _derived_value;
};

// Class - with virtual inheritance
class VirtualBase
{
public:
	virtual ~VirtualBase();

protected:
	int _base_value;
};

class VirtualDerivedA : virtual public VirtualBase
{
public:
	~VirtualDerivedA() override;

protected:
	int _a_value;
};

class VirtualDerivedB : virtual public VirtualBase
{
public:
	~VirtualDerivedB() override;

protected:
	int _b_value;
};

class DiamondInheritance : public VirtualDerivedA, public VirtualDerivedB
{
public:
	DiamondInheritance();
	~DiamondInheritance() override;

private:
	int _diamond_value;
};

// Class - abstract interface
class IInterface
{
public:
	virtual ~IInterface() = default;

	virtual void initialize() = 0;
	virtual void update(float delta_p) = 0;
	virtual void render() = 0;
	virtual void shutdown() = 0;

	virtual const char* get_name() const = 0;
	virtual uint32_t get_id() const = 0;
};

// Class - implementing interface
class ConcreteImplementation : public IInterface
{
public:
	ConcreteImplementation();
	~ConcreteImplementation() override;

	void initialize() override;
	void update(float delta_p) override;
	void render() override;
	void shutdown() override;

	const char* get_name() const override;
	uint32_t get_id() const override;

private:
	uint32_t _id;
	bool _is_initialized;
};

// Class - with operator overloading
class OperatorClass
{
public:
	OperatorClass();
	OperatorClass(int value_p);

	OperatorClass operator+(const OperatorClass& other_p) const;
	OperatorClass operator-(const OperatorClass& other_p) const;
	OperatorClass operator*(const OperatorClass& other_p) const;
	OperatorClass operator/(const OperatorClass& other_p) const;

	OperatorClass& operator+=(const OperatorClass& other_p);
	OperatorClass& operator-=(const OperatorClass& other_p);

	bool operator==(const OperatorClass& other_p) const;
	bool operator!=(const OperatorClass& other_p) const;
	bool operator<(const OperatorClass& other_p) const;
	bool operator>(const OperatorClass& other_p) const;
	bool operator<=(const OperatorClass& other_p) const;
	bool operator>=(const OperatorClass& other_p) const;

	OperatorClass& operator++();
	OperatorClass operator++(int);
	OperatorClass& operator--();
	OperatorClass operator--(int);

	int& operator[](size_t index_p);
	const int& operator[](size_t index_p) const;

	explicit operator bool() const;
	explicit operator int() const;

private:
	int _value;
	int _data[10];
};

// Class - with calling conventions (Windows-specific)
class CallingConventionClass
{
public:
	void __cdecl cdecl_method();
	void __stdcall stdcall_method();
	void __fastcall fastcall_method();
	void __vectorcall vectorcall_method();

private:
	int _value;
};

END_NAMESPACE


// ============================================================================
// SECTION 3: Nested Namespaces
// ============================================================================

BEGIN_NAMESPACE(FE::Test::Nested)

enum
	struct
	NestedColor
{
	Cyan,
	Magenta,
	Yellow,
	Black
};

struct
	NestedPoint
{
	int _x;
	int _y;
};

class
	NestedClass
{
public:
	NestedClass();
	~NestedClass();

	void method();

private:
	int _value;
};

END_NAMESPACE

BEGIN_NAMESPACE(FE::Test::Nested::Deep)

enum struct DeepEnum
{
	Value1,
	Value2
};

struct DeepStruct
{
	float _data;
};

class DeepClass
{
public:
	void deep_method();
};

END_NAMESPACE


// ============================================================================
// SECTION 4: Edge Cases and Complex Scenarios
// ============================================================================

BEGIN_NAMESPACE(FE::Test::EdgeCases)


template <typename T>
class BraceInjectionAttackClass
{
	char _bowoh = '}';
	const char* _waer = "asdadsa}d";
};

template <typename T>
struct BraceInjectionAttackStruct
{
	char _bowoh = '}';
	const char* _waer = "asdadsa}d";
};


// Struct with nested structs
struct OuterStruct
{
	struct InnerStruct
	{
		int _inner_value;
	};

	InnerStruct _nested;
	int _outer_value;
};

// Class with nested classes
class OuterClass
{
public:
	class InnerClass
	{
	public:
		InnerClass();
		void inner_method();

	private:
		int _inner_value;
	};

	OuterClass();
	void outer_method();

private:
	InnerClass _nested;
	int _outer_value;
};

// Enum struct with large values
enum struct LargeEnum : uint64_t
{
	Zero = 0,
	Small = 100,
	Medium = 1000000,
	Large = 1000000000,
	VeryLarge = 18446744073709551615ULL
};

enum struct Fake : char
{
	NerfThis = '}'
};

// Struct with array members
struct ArrayStruct
{
	int _single_array[10];
	int _multi_array[5][5];
	float _three_d_array[3][3][3];
	const int _const_array[20];
};

// Class with complex pointer types
class ComplexPointers
{
public:
	void process(int* ptr_p);
	void process(int** ptr_ptr_p);
	void process(int*** ptr_ptr_ptr_p);
	void process(const int* const* const ptr_p);
	void process(volatile int* volatile ptr_p);

	int* get_pointer();
	const int* get_const_pointer() const;
	int* const get_const_pointer_to_int();

private:
	int* _ptr;
	int** _ptr_ptr;
	const int* _const_ptr;
	int* const _const_ptr_to_int;
	const int* const _const_ptr_to_const_int;
};

// Struct with function pointers
struct FunctionPointers
{
	void (*_func_ptr)();
	int (*_func_ptr_with_args)(int, float);
	void (*_func_ptr_array[10])(int);

	using FuncPtrType = void (*)(int, int);
	FuncPtrType _typed_func_ptr;
};

// Class with mix of everything
class KitchenSink final : public BaseClass, public IInterface
{
public:
	KitchenSink();
	KitchenSink(int value_p);
	KitchenSink(const KitchenSink& other_p) = delete;
	KitchenSink(KitchenSink&& other_p) noexcept;
	virtual ~KitchenSink() override;

	KitchenSink& operator=(const KitchenSink& other_p) = delete;
	KitchenSink& operator=(KitchenSink&& other_p) noexcept;

	// From BaseClass
	void pure_virtual_method() override final;

	// From IInterface
	void initialize() override;
	void update(float delta_p) override;
	void render() override;
	void shutdown() override;
	const char* get_name() const override;
	uint32_t get_id() const override;

	// Own methods
	void normal_method();
	void const_method() const;
	void noexcept_method() noexcept;
	void const_noexcept_method() const noexcept;
	virtual void virtual_method();
	virtual void virtual_const_method() const;

	static void static_method();
	static int static_method_with_return();
	static constexpr int static_constexpr_method() { return 42; }

	inline void inline_method() { ++_value; }
	constexpr int constexpr_method() const { return _value * 2; }

	// Operator overloading
	KitchenSink operator+(const KitchenSink& other_p) const;
	bool operator==(const KitchenSink& other_p) const;

	// Pointer and reference methods
	void process(int& ref_p);
	void process(const int& const_ref_p) const;
	void process(int* ptr_p);
	void process(const int* const_ptr_p);
	int& get_ref();
	const int& get_const_ref() const;

protected:
	virtual void protected_virtual();

	int _protected_value;
	mutable int _mutable_value;

private:
	void private_method();

	int _value;
	float _data;
	const uint32_t _const_id;
	static int s_static_value;
	static thread_local int tl_thread_value;
	volatile bool _volatile_flag;
	mutable int _mutable_cache;

	int* _ptr;
	const int* _const_ptr;
	int& _ref;

	friend class FriendClass;
	friend void kitchen_sink_friend(const KitchenSink& obj_p);

public:
	static constexpr int PUBLIC_CONSTANT = 100;
	static constinit int s_constinit;
};

END_NAMESPACE

#endif // TEST_REFLECTION_COMPREHENSIVE_HPP