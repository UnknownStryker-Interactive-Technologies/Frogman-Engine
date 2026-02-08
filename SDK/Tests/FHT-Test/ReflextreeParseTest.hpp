/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#ifndef TEST_REFLEXTREE_GENERATION_HPP
#define TEST_REFLEXTREE_GENERATION_HPP

#include <FE/prerequisites.hxx>
#include <cstdint>

#define _NO_UNIQUE_ADDRESS_
#define _MAYBE_UNUSED_ [[maybe_unused]]
#define _NODISCARD_ [[nodiscard]]

// ============================================================================
// SECTION 0: FE_SYSTEM
// ============================================================================

FE_SYSTEM(_EngineInitialization, TargetComponentType, WorldTagEnumType)
void system(FE::component_base* const comptr_p) noexcept
{
	// System implementation
}

FE_SYSTEM(_EngineInitialization, TargetComponentType, WorldTagEnumType)
void           system2(FE::component_base* const comptr_p) noexcept;

FE_SYSTEM(_EngineInitialization, TargetComponentType, WorldTagEnumType)
void system3(FE::component_base* const comptr_p);

FE_SYSTEM(_EngineInitialization, TargetComponentType, WorldTagEnumType)
void system4(FE::component_base* const comptr_p)
{

}




// ============================================================================
// SECTION 1: Global Namespace with FE Macros
// ============================================================================

// Global enum struct with FE macro
FE_ENUM_STRUCT()
enum struct GlobalGameState
{
	MainMenu,
	InGame,
	Paused,
	GameOver
};

FE_ENUM_STRUCT()
enum struct [[nodiscard]] GlobalGameState2
{
	MainMenu,
	InGame,
	Paused,
	GameOver
};

FE_ENUM_STRUCT()
enum struct _NODISCARD_ GlobalGameState3
{
	MainMenu,
	InGame,
	Paused,
	GameOver
};

FE_ENUM_STRUCT()
enum struct _NODISCARD_ _MAYBE_UNUSED_ GlobalGameState4
{
	MainMenu,
	InGame,
	Paused,
	GameOver
};

FE_ENUM_STRUCT()
enum struct [[nodiscard]] _MAYBE_UNUSED_ GlobalGameState5
{
	MainMenu,
	InGame,
	Paused,
	GameOver
};

FE_ENUM_STRUCT()
enum struct _NODISCARD_ [[maybe_unused]] GlobalGameState6
{
	MainMenu,
	InGame,
	Paused,
	GameOver
};




// Global enum struct with explicit type
FE_ENUM_STRUCT()
enum struct GlobalEntityType : uint16_t
{
	Player = 0,
	Enemy = 1,
	Projectile = 100,
	Pickup = 200
};

FE_ENUM_STRUCT()
enum struct [[nodiscard]] GlobalEntityType2 : uint16_t
{
	Player = 0,
	Enemy = 1,
	Projectile = 100,
	Pickup = 200
};

FE_ENUM_STRUCT()
enum struct _NODISCARD_ GlobalEntityType3 : uint16_t
{
	Player = 0,
	Enemy = 1,
	Projectile = 100,
	Pickup = 200
};

FE_ENUM_STRUCT()
enum struct _NODISCARD_ _MAYBE_UNUSED_ GlobalEntityType4 : uint16_t
{
	Player = 0,
	Enemy = 1,
	Projectile = 100,
	Pickup = 200
};




// Global struct with FE macro - POD style
FE_STRUCT()
struct GlobalTransform2D
{
	float _x;
	float _y;
	float _rotation;
};




// Global struct with FE macro - with methods
FE_STRUCT()
struct GlobalVelocity
{
	float _vx;
	float _vy;

	GlobalVelocity();
	GlobalVelocity(float vx_p, float vy_p);
	~GlobalVelocity();

	float magnitude() const;
	void normalize();
	static GlobalVelocity zero();
};

// Global struct with FE macro - with inheritance
FE_STRUCT()
struct GlobalRigidBody : public GlobalVelocity
{
	float _mass;
	float _friction;

	GlobalRigidBody();
	virtual ~GlobalRigidBody();
	virtual void apply_force(float fx_p, float fy_p);
};




// Global class with FE macro - simple
FE_CLASS()
class GlobalGameObject
{
public:
	GlobalGameObject();
	~GlobalGameObject();

	void set_active(bool active_p);
	bool is_active() const;

private:
	bool _is_active;
	uint32_t _object_id;
	GlobalGameState _state;
};

// Global class with FE macro - attr
FE_CLASS()
class GlobalGameObject2
{
public:
	GlobalGameObject2();
	~GlobalGameObject2();

	void set_active(bool active_p);
	bool is_active() const;

private:
	bool _is_active;
	uint32_t _object_id;
	GlobalGameState _state;
};

FE_CLASS()
class [[maybe_unused]] GlobalGameObject3
{
public:
	GlobalGameObject3();
	~GlobalGameObject3();

	void set_active(bool active_p);
	bool is_active() const;

private:
	bool _is_active;
	uint32_t _object_id;
	GlobalGameState _state;
};

FE_CLASS()
class GlobalGameObject4
{
public:
	GlobalGameObject4();
	~GlobalGameObject4();

	void set_active(bool active_p);
	bool is_active() const;

private:
	bool _is_active;
	uint32_t _object_id;
	GlobalGameState _state;
};

FE_CLASS()
class _MAYBE_UNUSED_ GlobalGameObject5
{
public:
	GlobalGameObject5();
	~GlobalGameObject5();

	void set_active(bool active_p);
	bool is_active() const;

private:
	bool _is_active;
	uint32_t _object_id;
	GlobalGameState _state;
};

FE_CLASS()
class _MAYBE_UNUSED_ GlobalGameObject6
{
public:
	GlobalGameObject6();
	~GlobalGameObject6();

	void set_active(bool active_p);
	bool is_active() const;

private:
	bool _is_active;
	uint32_t _object_id;
	GlobalGameState _state;
};

FE_CLASS()
class [[maybe_unused]] GlobalGameObject7
{
public:
	GlobalGameObject7();
	~GlobalGameObject7();

	void set_active(bool active_p);
	bool is_active() const;

private:
	bool _is_active;
	uint32_t _object_id;
	GlobalGameState _state;
};




// Global class with FE macro - with multiple access specifiers
FE_CLASS()
class GlobalComponentBase
{
protected:
	bool _enabled;
	uint64_t _component_id;

public:
	GlobalComponentBase() {}
	virtual ~GlobalComponentBase() {}

	virtual void on_create() = 0;
	virtual void on_update(float delta_p) = 0;
	virtual void on_destroy() = 0;

	bool is_enabled() const noexcept;
	void set_enabled(bool enabled_p) noexcept;

private:
	GlobalGameState _state;
};

// Global class with FE macro - virtual inheritance
FE_CLASS()
class GlobalRenderableComponent : public GlobalComponentBase
{
public:
	GlobalRenderableComponent();
	virtual ~GlobalRenderableComponent() override;

	void on_create() override;
	void on_update(float delta_p) override;
	void on_destroy() override;

	void set_sprite_id(uint32_t sprite_id_p);

protected:
	uint32_t _sprite_id;
	float _alpha;

private:
	bool _visible;
};

// Global class with FE macro - final keyword
FE_CLASS()
class GlobalSingletonManager final
{
public:
	GlobalSingletonManager(const GlobalSingletonManager&) = delete;
	GlobalSingletonManager& operator=(const GlobalSingletonManager&) = delete;
	GlobalSingletonManager(GlobalSingletonManager&&) = delete;
	GlobalSingletonManager& operator=(GlobalSingletonManager&&) = delete;

	static GlobalSingletonManager& get_instance();

	void initialize();
	void shutdown();

private:
	GlobalSingletonManager();
	~GlobalSingletonManager();

	bool _initialized;
};




// ============================================================================
// SECTION 2: Namespace with FE Macros
// ============================================================================

BEGIN_NAMESPACE(FE::Test)

void escape()
{

}

// Enum struct with FE macro - simple
FE_ENUM_STRUCT()
enum struct InputState
{
	Released,
	Pressed,
	Held,
	JustReleased
};

// Enum struct with FE macro - explicit type uint8_t
FE_ENUM_STRUCT()
enum struct RenderLayer : uint8_t
{
	Background = 0,
	Gameplay = 1,
	UI = 2,
	Debug = 3,
	PostProcess = 255
};

// Enum struct with FE macro - explicit type int64_t
FE_ENUM_STRUCT()
enum struct EntityID : int64_t
{
	Invalid = -1,
	First = 0,
	MaxEntities = 9223372036854775807LL
};

// Enum struct with FE macro - single value
FE_ENUM_STRUCT()
enum struct DefaultWorldTag
{
	MainWorld
};

// Enum struct with FE macro - empty (edge case)
FE_ENUM_STRUCT()
enum struct ReservedEnum
{
};

// Struct with FE macro - simple POD
FE_STRUCT()
struct Position
{
	float _x;
	float _y;
};

// Struct with FE macro - multiple fields on same line
FE_STRUCT()
struct Bounds
{
	float _left, _top, _right, _bottom;
};

// Struct with FE macro - with constructor
FE_STRUCT()
struct Size
{
	float _width;
	float _height;

	Size();
	Size(float width_p, float height_p);
};

// Struct with FE macro - with various method types
FE_STRUCT()
struct Transform
{
	float _x;
	float _y;
	float _rotation;

	// Constructors
	Transform();
	Transform(float x_p, float y_p, float rotation_p);
	Transform(const Transform& other_p);
	Transform(Transform&& other_p) noexcept;

	// Destructor
	~Transform();

	// Assignment operators
	Transform& operator=(const Transform& other_p);
	Transform& operator=(Transform&& other_p) noexcept;

	// Const methods
	float get_angle_radians() const;
	Position get_position() const;

	// Non-const methods
	void translate(float dx_p, float dy_p);
	void rotate(float angle_p);

	// Static methods
	static Transform identity();
	static Transform from_position(const Position& pos_p);

	// Inline methods
	inline float get_x() const { return _x; }
	inline void set_x(float x_p) { _x = x_p; }

	// Constexpr methods
	constexpr float sum_components() const { return _x + _y + _rotation; }
};

// Struct with FE macro - with inheritance (single)
FE_STRUCT()
struct AnimatedTransform : public Transform
{
	float _animation_speed;
	float _current_frame;

	AnimatedTransform();
	virtual ~AnimatedTransform();
	virtual void update(float delta_p);
	virtual void reset_animation();
};

// Struct with FE macro - with static members
FE_STRUCT()
struct GameConfig
{
	static const uint32_t MAX_PLAYERS = 4;
	static const float DEFAULT_TIMESTEP;
	static constexpr uint64_t FRAME_BUFFER_SIZE = 8192;

	bool _fullscreen;
	bool _vsync;
	uint32_t _target_fps;

	static void load_from_file(const char* path_p);
	static void save_to_file(const char* path_p);
	static GameConfig& get_default();
};

// Struct with FE macro - with bitfields
FE_STRUCT()
struct EntityFlags
{
	uint32_t _is_visible : 1;
	uint32_t _is_collidable : 1;
	uint32_t _is_kinematic : 1;
	uint32_t _is_trigger : 1;
	uint32_t _reserved : 28;
};

// Class with FE macro - simple with private members only
FE_CLASS()
class Component
{
private:
	uint32_t _id;
	float _priority;
};

// Class with FE macro - with public section
FE_CLASS()
class SpriteComponent
{
public:
	SpriteComponent();
	~SpriteComponent();

	void set_sprite(uint32_t sprite_id_p);
	uint32_t get_sprite() const;

private:
	uint32_t _sprite_id;
};

// Class with FE macro - with all access specifiers
FE_CLASS()
class PhysicsComponent
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	void apply_impulse(float x_p, float y_p);
	float mass;

protected:
	void update_internal(float delta_p);
	float _velocity_x;

private:
	void calculate_forces();
	float _mass;
};

// Class with FE macro - with virtual methods
FE_CLASS()
class EntityBase
{
public:
	EntityBase() {}
	virtual ~EntityBase() {}

	virtual void on_spawn();
	virtual void on_tick(float delta_p) = 0;
	virtual int get_type_id() const;

protected:
	virtual void cleanup();

private:
	uint32_t _entity_id;
};

// Class with FE macro - derived with override
FE_CLASS()
class EnemyEntity : public EntityBase
{
public:
	EnemyEntity();
	virtual ~EnemyEntity() override;

	void on_spawn() override;
	void on_tick(float delta_p) override;
	int get_type_id() const override;

protected:
	void cleanup() override;

private:
	float _health;
};

// Class with FE macro - final class
FE_CLASS()
class PlayerEntity final : public EntityBase
{
public:
	PlayerEntity();
	~PlayerEntity() override;

	void on_tick(float delta_p) override final;

private:
	float _score;
};

// Class with FE macro - with deleted methods
FE_CLASS()
class NonCopyableEntity
{
public:
	NonCopyableEntity();
	~NonCopyableEntity();

	NonCopyableEntity(const NonCopyableEntity&) = delete;
	NonCopyableEntity& operator=(const NonCopyableEntity&) = delete;
	NonCopyableEntity(NonCopyableEntity&&) = delete;
	NonCopyableEntity& operator=(NonCopyableEntity&&) = delete;

private:
	uint64_t _unique_id;
};

// Class with FE macro - with static methods and members
FE_CLASS()
class EntityFactory
{
public:
	static void initialize();
	static EntityBase* create_entity(int type_id_p);
	static const EntityFactory& get_instance();

	static int s_entity_count;
	static constexpr float s_spawn_radius = 100.0f;

private:
	EntityFactory() {}
	~EntityFactory() {}

	static int s_next_id;
	static void register_type(int type_id_p);
};

// Class with FE macro - with multiple inheritance
FE_CLASS()
class IUpdatable
{
public:
	virtual ~IUpdatable();
	virtual void update(float delta_p) = 0;

protected:
	float _last_update_time;
};

FE_CLASS()
class IRenderable
{
public:
	virtual ~IRenderable();
	virtual void render() = 0;

protected:
	uint32_t _render_layer;
};

FE_CLASS()
class UpdatableRenderable : public IUpdatable, public IRenderable
{
public:
	UpdatableRenderable();
	~UpdatableRenderable() override;

	void update(float delta_p) override;
	void render() override;

private:
	bool _dirty;
};

// Class with FE macro - abstract interface
FE_CLASS()
class IGameSystem
{
public:
	virtual ~IGameSystem() = default;

	virtual void initialize() = 0;
	virtual void tick(float delta_p) = 0;
	virtual void render() = 0;
	virtual void shutdown() = 0;

	virtual const char* get_system_name() const = 0;
	virtual uint32_t get_priority() const = 0;
};

union att
{

};

// Class with FE macro - implementing interface
FE_CLASS()
class RenderSystem : public IGameSystem
{
public:
	RenderSystem();
	~RenderSystem() override;

	void initialize() override;
	void tick(float delta_p) override;
	void render() override;
	void shutdown() override;

	const char* get_system_name() const override;
	uint32_t get_priority() const override;

private:
	uint32_t _render_target;
	bool _is_initialized;
};


END_NAMESPACE




// ============================================================================
// SECTION 3: Nested Namespaces with FE Macros
// ============================================================================

BEGIN_NAMESPACE(FE::Test::Nested)

FE_ENUM_STRUCT()
enum struct NestedInputType
{
	Keyboard,
	Mouse,
	Gamepad,
	Touch
};

FE_STRUCT()
struct NestedInput
{
	int _device_id;
	float _value;
};

FE_CLASS()
class NestedInputManager
{
public:
	NestedInputManager();
	~NestedInputManager();

	void process_input();

private:
	bool _enabled;
};

END_NAMESPACE

BEGIN_NAMESPACE(FE::Test::Nested::Deep)

FE_ENUM_STRUCT()
enum struct DeepActionType
{
	Move,
	Attack
};

FE_STRUCT()
struct DeepAction
{
	DeepActionType _type;
	float _intensity;
};

FE_CLASS()
class DeepActionHandler
{
public:
	void handle_action(const DeepAction& action_p);
};

END_NAMESPACE




// ============================================================================
// SECTION 4: Edge Cases and Complex Scenarios with FE Macros
// ============================================================================

BEGIN_NAMESPACE(FE::Test::EdgeCases)

// Struct with FE macro - nested structs
FE_STRUCT()
struct OuterData
{
	FE_STRUCT()
		struct InnerData
	{
		int _value;
	};

	InnerData _data;
	float _scale;
};

// Class with FE macro - nested classes
FE_CLASS()
class OuterSystem
{
public:
	FE_CLASS()
		class InnerSystem
	{
	public:
		InnerSystem();
		void process();

	private:
		int _state;
	};

	OuterSystem();
	void run();

private:
	InnerSystem _subsystem;
	bool _active;
};

// Enum struct with FE macro - large values
FE_ENUM_STRUCT()
enum struct LargeIDType : uint64_t
{
	None = 0,
	Small = 100,
	Medium = 1000000,
	Large = 1000000000,
	Maximum = 18446744073709551615ULL
};


// Enum struct with FE macro - character type
FE_ENUM_STRUCT()
enum struct CharacterEnum : char
{
	TypeA = 'A',
	TypeB = 'B',
	TypeC = 'C'
};

// Struct with FE macro - array members
FE_STRUCT()
struct ArrayContainer
{
	int _simple_array[10];
	float _matrix[4][4];
	uint32_t _constant_buffer[64];
};

// Class with FE macro - complex pointer types
FE_CLASS()
class PointerManager
{
public:
	void process_data(int* ptr_p);
	void process_multi(int** ptr_ptr_p);
	void process_const(const int* const* const ptr_p);

	int* get_data();
	const int* get_const_data() const;

private:
	int* _data_ptr;
	int** _indirect_ptr;
	const int* _const_ptr;
};

// Struct with FE macro - function pointers
FE_STRUCT()
struct CallbackContainer
{
	void (*_on_update)();
	int (*_on_event)(int, float);
	void (*_callback_array[8])(int);

	using EventCallback = void (*)(int, int);
	EventCallback _typed_callback;
};

END_NAMESPACE

#endif // TEST_REFLEXTREE_GENERATION_HPP