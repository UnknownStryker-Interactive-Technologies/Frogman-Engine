#ifndef _FROGMAN_ENGINE_HPP_
#define _FROGMAN_ENGINE_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include <FE/prerequisites.hxx>
#include <FE/framework.hxx>
#include <FE/game.hpp>
#include <FE/renderer.hxx>
#pragma warning(disable : 4324)
#include <boost/json.hpp> // read game configurations from .froggy file

#include <boost/hash2/fnv1a.hpp>




#define FROGMAN_ENGINE() CUSTOM_ENGINE(FE::engine)




BEGIN_NAMESPACE(FE)

class ecs;
class mutex;
class shared_mutex;
CLASS_FORWARD_DECLARATION(framework, game_processor);


struct engine_info // fields are immutable after window creation; modifying these values will not affect any.
{
    std::pmr::string _version;
	std::pmr::string _installation_path;
};

struct path_lut // fields are immutable after window creation; modifying these values will not affect any.
{
    std::pmr::string _entry_world_path;
    std::pmr::vector<std::pmr::string> _world_paths;
    std::pmr::vector<std::pmr::string> _module_paths;
};

struct shader_define
{
	std::pmr::string _identifier;
	FE::pair<var::int64, var::int64> _value_range;
};

struct shader
{
	std::pmr::vector<shader_define> _defines;
    std::pmr::vector<std::pmr::string> _permutation_blacklist;
    std::pmr::vector<std::pmr::string> _permutation;
    std::pmr::string _main_function;
    std::pmr::string _source_path;
	std::pmr::string _source_code;
    internal::renderer::ShaderTarget _shader_target;
};

struct project_config // fields are immutable after window creation; modifying these values will not affect any.
{
	path_lut _path_lookup_table;

	FE::system _compression_method = nullptr;
    FE::system _decompression_method = nullptr;
	FE::system _encryption_method = nullptr;
    FE::system _decryption_method = nullptr;
    FE::uint64 _max_engine_component_type_count_hint = 1024;
	FE::uint32 _gc_batch_count = 30;
    FE::uint32 _frames_per_reachability_analysis = 60;
	FE::uint64 _fiber_stack_size = FE::one_MiB;
    FE::uint16 _fibers_per_thread = 3;

	FE::window_config _window_config;
};


class engine_program_options 
{
	FE::pair<::FE::ASCII*, var::boolean> m_enable_fullscreen;
	FE::pair<::FE::ASCII*, var::boolean> m_enable_vsync;
    FE::pair<::FE::ASCII*, var::boolean> m_recompile_shaders;

public:
    engine_program_options(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
    ~engine_program_options() noexcept = default;

    FE::boolean is_fullscreen_enabled() const noexcept;
    FE::ASCII* view_enable_fullscreen_title() const noexcept;

	FE::boolean is_vsync_enabled() const noexcept;
    FE::ASCII* view_enable_vsync_title() const noexcept;
};


class alignas(FE::CPU_L1_cache_line::size) engine final : public FE::framework::framework_base
{
    friend class ecs;
    friend class FE::renderer;
    friend class FE::mutex;
    friend class FE::shared_mutex;

	engine_program_options m_engine_program_options;

    std::pmr::string m_runtime_path;
	std::pmr::string m_game_root_directory;
    std::pmr::string m_froggy_path;
    std::pmr::string m_froggy;

    FE::engine_info m_engine_info;
    FE::project_config m_project_config;
	std::pmr::vector<shader> m_shaders;

    FE::smart_ptr<FE::game, FE::RefType::_Owner> m_game_instance;

    FE::smart_ptr<class FE::framework::game_processor, FE::RefType::_Owner> m_game_processor;
    FE::smart_ptr<FE::renderer, FE::RefType::_Owner> m_renderer;

public:
    engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
    virtual ~engine() noexcept override;

public:
    _FE_FORCE_INLINE_ static FE::engine& get_engine() noexcept { return static_cast<FE::engine&>(FE::framework::framework_base::get_framework()); }
	
    _FE_FORCE_INLINE_ FE::ASCII* get_runtime_path() const noexcept { return m_runtime_path.c_str(); }
	_FE_FORCE_INLINE_ FE::ASCII* get_game_root_directory() const noexcept { return m_game_root_directory.c_str(); }

    _FE_FORCE_INLINE_ const FE::engine_info& get_engine_info() const noexcept { return m_engine_info; }
	_FE_FORCE_INLINE_ const FE::project_config& get_project_config() const noexcept { return m_project_config; }

    _FE_FORCE_INLINE_ static FE::game& get_game_instance() noexcept { return *(get_engine().m_game_instance); }

    _FE_FORCE_INLINE_ FE::int32 count_async_processors() const noexcept { return m_program_options.get_max_concurrency() - 2; /* -(game + renderer) */ }
    
	template <typename T>
    _FE_FORCE_INLINE_ FE::polymorphic_allocator<T> get_thread_local_allocator() noexcept { return FE::polymorphic_allocator<T>( get_memory_resource() ); }

    template <typename T>
	_FE_FORCE_INLINE_ std::pmr::polymorphic_allocator<T> get_std_pmr_thread_local_allocator() noexcept { return std::pmr::polymorphic_allocator<T>( get_memory_resource() ); }

    template <typename T>
    _FE_FORCE_INLINE_ FE::polymorphic_allocator<T> get_large_thread_local_allocator() noexcept { return FE::polymorphic_allocator<T>(get_large_memory_resource()); }

    template <typename T>
    _FE_FORCE_INLINE_ std::pmr::polymorphic_allocator<T> get_std_pmr_large_thread_local_allocator() noexcept { return std::pmr::polymorphic_allocator<T>(get_large_memory_resource()); }

private:
    virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override;
    virtual FE::int32 run() override;
    virtual FE::int32 shutdown() override;

private:
	void __read_froggy() noexcept;

private: // Callbacks
	static void __key_callback(GLFWwindow* const window_p, FE::int32 key_p, FE::int32 scancode_p, FE::int32 action_p, FE::int32 mods_p) noexcept;
	static void __mouse_button_callback(GLFWwindow* const window_p, FE::int32 button_p, FE::int32 action_p, FE::int32 mods_p) noexcept;
	static void __cursor_position_callback(GLFWwindow* const window_p, double x_p, double y_p) noexcept;
	static void __scroll_callback(GLFWwindow* const window_p, double x_offset_p, double y_offset_p) noexcept;
};


END_NAMESPACE
#endif