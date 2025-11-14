#ifndef _FROGMAN_ENGINE_HXX_
#define _FROGMAN_ENGINE_HXX_
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

#include <boost/json.hpp> // read game configurations from .froggy file


#define FROGMAN_ENGINE() CUSTOM_ENGINE(FE::engine)




BEGIN_NAMESPACE(FE)


class async;
class ecs;
class memory;
class window_control;


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

struct project_config // fields are immutable after window creation; modifying these values will not affect any.
{
	path_lut _path_lookup_table;

	FE::system _compression_method = nullptr;
    FE::system _decompression_method = nullptr;
	FE::system _encryption_method = nullptr;
    FE::system _decryption_method = nullptr;
    FE::uint64 _max_engine_entities = 10240;
    FE::uint64 _max_engine_component_type_count_hint = 1024;
	FE::uint64 _max_engine_system_count_hint = 1024;
	FE::uint32 _gc_iterations_per_frame = 30;
	FE::uint64 _fiber_stack_size = FE::one_MiB;
    FE::uint16 _fibers_per_thread = 3;

	FE::window_config _window_config;
};


class engine final : public FE::framework::framework_base
{
    friend class async;
    friend class ecs;
    friend class memory;
    friend class FE::window_control;

    std::pmr::string m_runtime_path;
    std::pmr::string m_froggy_path;
    std::pmr::string m_froggy;

    FE::smart_ptr<FE::engine_info, FE::RefType::_Owner> m_engine_info;
	FE::smart_ptr<FE::project_config, FE::RefType::_Owner> m_project_config;

	std::atomic_bool m_should_exit;
    FE::smart_ptr<FE::internal::game_processor, FE::RefType::_Owner> m_game_processor;
	FE::framework::thread m_renderer_thread;
    FE::smart_ptr<FE::renderer, FE::RefType::_Owner> m_renderer;
    FE::smart_ptr<FE::game, FE::RefType::_Owner> m_game_instance;

public:
    engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
    virtual ~engine() noexcept override;

public:
    _FE_FORCE_INLINE_ static FE::game& get_game_instance() noexcept { return *(__get_engine().m_game_instance); }

private:
    virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override;
    virtual FE::int32 run() override;
    virtual FE::int32 shutdown() override;

    _FE_FORCE_INLINE_ static FE::engine& __get_engine() noexcept { return static_cast<FE::engine&>(FE::framework::framework_base::get_framework()); }
	void __read_froggy() noexcept;
	void __initialize_window_and_renderer() noexcept;
    _FE_FORCE_INLINE_ FE::int32 __calculate_async_processor_count() const noexcept { return m_program_options.get_max_concurrency() - 2; /* -(game + renderer) */ }

private: // Callbacks
	static void __renderer_main(FE::component_base* const) noexcept;
	static void __on_window_close(GLFWwindow* window_p) noexcept;
	static void __key_callback(GLFWwindow* const window_p, FE::int32 key_p, FE::int32 scancode_p, FE::int32 action_p, FE::int32 mods_p) noexcept;
	static void __mouse_button_callback(GLFWwindow* const window_p, FE::int32 button_p, FE::int32 action_p, FE::int32 mods_p) noexcept;
	static void __cursor_position_callback(GLFWwindow* const window_p, double x_p, double y_p) noexcept;
	static void __scroll_callback(GLFWwindow* const window_p, double x_offset_p, double y_offset_p) noexcept;
};


END_NAMESPACE
#endif