#ifndef _FROGMAN_ENGINE_HPP_
#define _FROGMAN_ENGINE_HPP_
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
#include <FE/framework.hxx>
#include <FE/framework/smart_ptr.hxx>

#include <FE/reference_table.hxx>

#include <FE/renderer.hxx>
#include <FE/window.hxx>

#include <FE/game.hxx>
#include <FE/world.hxx>

#pragma warning(disable : 4324)
#include <json/json.h> // read game configurations from .froggy file

#include <boost/hash2/fnv1a.hpp>




#define FROGMAN_ENGINE() CUSTOM_ENGINE(::FE::engine, ::FE::engine_program_options)




BEGIN_NAMESPACE(FE)


class async;


struct engine_info // fields are immutable after window creation; modifying these values will not affect any.
{
    std::pmr::string _version;
    FE::directory_string _installation_path;
};

struct path_lut // fields are immutable after window creation; modifying these values will not affect any.
{
    std::pmr::vector<FE::directory_string> _world_paths;
};


struct project_config // fields are immutable after window creation; modifying these values will not affect any.
{
	path_lut _path_lookup_table;

	//FE::system _compression_method = nullptr;
 //   FE::system _decompression_method = nullptr;
	//FE::system _encryption_method = nullptr;
 //   FE::system _decryption_method = nullptr;
	FE::uint32 _fiber_stack_size = 4 * FE::one_MiB;
    FE::uint16 _fibers_per_thread = 4;

	FE::window_config _window_config;
};


class engine_program_options : public FE::framework::program_option
{
	using base = FE::framework::program_option;

	FE::pair<::FE::ASCII*, var::boolean> m_enable_fullscreen;
	FE::pair<::FE::ASCII*, var::boolean> m_enable_vsync;
    FE::pair<::FE::ASCII*, var::boolean> m_recompile_shaders;

public:
    engine_program_options(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
    virtual ~engine_program_options() noexcept override = default;

    FE::boolean is_fullscreen_enabled() const noexcept;
    FE::ASCII* view_enable_fullscreen_title() const noexcept;

	FE::boolean is_vsync_enabled() const noexcept;
    FE::ASCII* view_enable_vsync_title() const noexcept;

	FE::boolean is_recompile_shaders_enabled() const noexcept;
	FE::ASCII* view_recompile_shaders_title() const noexcept;
};


class alignas(FE::CPU_L1_cache_line::size) engine_base : public FE::framework::framework_base
{
    using base = FE::framework::framework_base;
    
public:
	engine_base(std::unique_ptr<engine_program_options> options_p) noexcept 
        : base(std::move(options_p)) 
    {
    };

    virtual ~engine_base() noexcept override = default;
};


#ifdef _FE_SHIPPING_BUILD_
// Shipping build engine class
class alignas(FE::CPU_L1_cache_line::size) engine : public engine_base
{
    using base = FE::framework::framework_base;
}
#else
// Editortime engine class
class alignas(FE::CPU_L1_cache_line::size) engine : public engine_base
{
	using base = engine_base;

public:
    class auth
    {
		friend class FE::async;
        friend class FE::renderer;
        friend class FE::window;
        friend class FE::world;
        friend class FE::internal::renderer::shader;

        constexpr auth() noexcept {};
        constexpr ~auth() noexcept {};

        constexpr auth(auth&&) noexcept {};
        constexpr auth(const auth&) noexcept {};
    };

private:
    FE::directory_string m_runtime_path;
    FE::directory_string m_game_root_directory;
    FE::directory_string m_froggy_path;
    std::pmr::string m_froggy;

    FE::engine_info m_engine_info;
    FE::project_config m_project_config;
    // Game Thread / Render Thread(Culling) / RHI Thread / Async Task Thread
    FE::smart_ptr<FE::processors, FE::RefType::_Owner> m_processors;
    FE::smart_ptr<FE::renderer, FE::RefType::_Owner> m_renderer;
	FE::smart_ptr<FE::window, FE::RefType::_Owner> m_window;
	FE::smart_ptr<FE::game, FE::RefType::_Owner> m_game;
	FE::reference_table m_reference_table;

    using world_list =
        absl::flat_hash_map<world_tag_t, FE::smart_ptr<FE::world, FE::RefType::_Owner>,
        absl::DefaultHashContainerHash<world_tag_t>, absl::DefaultHashContainerEq<world_tag_t>,
        std::pmr::polymorphic_allocator<std::pair<const world_tag_t, FE::smart_ptr<FE::world, FE::RefType::_Owner>>>>;
    world_list m_worlds;
    FE::smart_ptr<FE::world, FE::RefType::_Observer> m_current_world;

    std::atomic_bool m_should_tick;
	FE::clock m_runtime_clock;

    concurrency::concurrent_queue<global_system, std::pmr::polymorphic_allocator<global_system>> m_deferred_engine_initialization_cmd_queue;
    concurrency::concurrent_queue<global_system, std::pmr::polymorphic_allocator<global_system>> m_deferred_game_instance_initialization_cmd_queue;
    concurrency::concurrent_queue<global_system, std::pmr::polymorphic_allocator<global_system>> m_deferred_game_instance_begin_cmd_queue;
    concurrency::concurrent_queue<global_system, std::pmr::polymorphic_allocator<global_system>> m_deferred_pre_game_instance_tick_cmd_queue;
    concurrency::concurrent_queue<global_system, std::pmr::polymorphic_allocator<global_system>> m_deferred_game_instance_tick_cmd_queue;
    concurrency::concurrent_queue<global_system, std::pmr::polymorphic_allocator<global_system>> m_deferred_post_game_instance_tick_cmd_queue;
    concurrency::concurrent_queue<global_system, std::pmr::polymorphic_allocator<global_system>> m_deferred_game_instance_end_cmd_queue;
    concurrency::concurrent_queue<global_system, std::pmr::polymorphic_allocator<global_system>> m_deferred_game_instance_deinitialization_cmd_queue;
    concurrency::concurrent_queue<global_system, std::pmr::polymorphic_allocator<global_system>> m_deferred_engine_deinitialization_cmd_queue;

public:
    engine(std::unique_ptr<engine_program_options> options_p) noexcept;
    virtual ~engine() noexcept override;

public:
    _FE_FORCE_INLINE_ static FE::engine& get_engine() noexcept { return static_cast<FE::engine&>(FE::framework::framework_base::get_framework()); }
    _FE_FORCE_INLINE_ const engine_program_options& get_program_options() const noexcept { return static_cast<const engine_program_options&>(*m_program_options); }
	
    _FE_FORCE_INLINE_ FE::directory_char_t* get_runtime_path() const noexcept { return m_runtime_path.c_str(); }
	_FE_FORCE_INLINE_ FE::directory_char_t* get_game_root_directory() const noexcept { return m_game_root_directory.c_str(); }
	_FE_FORCE_INLINE_ const FE::directory_string& get_froggy_path() const noexcept { return m_froggy_path; }
    
    _FE_FORCE_INLINE_ const FE::engine_info& get_engine_info() const noexcept { return m_engine_info; }
	_FE_FORCE_INLINE_ const FE::project_config& get_project_config() const noexcept { return m_project_config; }
    _FE_FORCE_INLINE_ FE::project_config& get_project_config(_FE_MAYBE_UNUSED_ const auth&) noexcept { return m_project_config; }

    _FE_FORCE_INLINE_ const FE::processors& get_processors() const noexcept { return *m_processors; }
    _FE_FORCE_INLINE_ FE::processors& get_processors(_FE_MAYBE_UNUSED_ const auth&) noexcept { return *m_processors; }
    // excludes the main thread from the count
	_FE_FORCE_INLINE_ FE::int32 count_processors() const noexcept { return base::get_program_options().get_max_concurrency() - 1; } 
    void terminate_all_processors() noexcept;
	FE::boolean should_tick() const noexcept { return m_should_tick.load(std::memory_order_acquire); }

	_FE_FORCE_INLINE_ const FE::renderer& get_renderer() const noexcept { return *m_renderer; }
    _FE_FORCE_INLINE_ FE::renderer& get_renderer(_FE_MAYBE_UNUSED_ const auth&) noexcept { return *m_renderer; }
	_FE_FORCE_INLINE_ const FE::directory_string& get_shader_root_directory() const noexcept { return m_renderer->get_shader_root_directory(); }

	_FE_FORCE_INLINE_ const FE::window& get_window() const noexcept { return *m_window; }
	_FE_FORCE_INLINE_ FE::window& get_window() noexcept { return *m_window; }

	_FE_FORCE_INLINE_ world_list& get_worlds(_FE_MAYBE_UNUSED_ const auth&) noexcept { return m_worlds; }
	_FE_FORCE_INLINE_ void set_current_world(_FE_MAYBE_UNUSED_ const auth&, FE::smart_ptr<FE::world, FE::RefType::_Observer> world_p) noexcept { m_current_world = world_p; }
	_FE_FORCE_INLINE_ const FE::world& get_world() const noexcept { return *m_current_world; }
    _FE_FORCE_INLINE_ FE::world& get_world() noexcept { return *m_current_world; }

	_FE_FORCE_INLINE_ const FE::game& get_game() const noexcept { return *m_game; }
	_FE_FORCE_INLINE_ FE::game& get_game() noexcept { return *m_game; }

	_FE_FORCE_INLINE_ const FE::reference_table& get_reference_table() const noexcept { return m_reference_table; }
	_FE_FORCE_INLINE_ FE::reference_table& get_reference_table() noexcept { return m_reference_table; }

public:
    void enqueue_global_command(global_system system_p, GlobalSystemCallPhase phase_p) noexcept;

private:
    virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override;
    virtual FE::int32 run() override;
    virtual FE::int32 shutdown() override;

	void __game_main() noexcept;

private:
	void __read_froggy() noexcept;
};
#endif


template <typename T, typename... Arguments>
_FE_FORCE_INLINE_ FE::gc_ptr<T> _FE_VECTOR_CALL_ gcnew(std::pmr::memory_resource* const resource_p, Arguments&&... arguments_p) noexcept
{
    static_assert(std::is_reference_v<T> == false, "Static assertion failed: gc_ptr cannot hold a pointer to a reference type variable.");
    static_assert(std::is_const_v<T> == false, "Static assertion failed: gc_ptr cannot hold a pointer to a const type variable.");
    static_assert(std::is_base_of_v<FE::gc_base, std::remove_extent_t<T>> == true, "Static assertion failed: T must be derived from FE::gc_base.");

    FE::gc_ptr<T> l_result(resource_p, std::forward<Arguments>(arguments_p)...);
    FE::engine::get_engine().get_reference_table().__push_back(l_result.m_ptr.load(std::memory_order_acquire));
    return l_result;
}


END_NAMESPACE
#endif