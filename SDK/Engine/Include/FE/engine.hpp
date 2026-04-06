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

#include <boost/json.hpp> // read game configurations from .froggy file


#define FROGMAN_ENGINE() CUSTOM_ENGINE(FE::engine)




BEGIN_NAMESPACE(FE)

class ecs;
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


class engine final : public FE::framework::framework_base
{
    friend class ecs;
    friend class FE::renderer;

    std::pmr::string m_runtime_path;
    std::pmr::string m_froggy_path;
    std::pmr::string m_froggy;

    FE::smart_ptr<FE::engine_info, FE::RefType::_Owner> m_engine_info;
	FE::smart_ptr<FE::project_config, FE::RefType::_Owner> m_project_config;

    FE::smart_ptr<FE::game, FE::RefType::_Owner> m_game_instance;

    FE::smart_ptr<class FE::framework::game_processor, FE::RefType::_Owner> m_game_processor;
    FE::smart_ptr<FE::renderer, FE::RefType::_Owner> m_renderer;

public:
    engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
    virtual ~engine() noexcept override;

public:
    _FE_FORCE_INLINE_ static FE::engine& get_engine() noexcept { return static_cast<FE::engine&>(FE::framework::framework_base::get_framework()); }
	_FE_FORCE_INLINE_ const FE::engine_info& get_engine_info() const noexcept { return *m_engine_info; }
	_FE_FORCE_INLINE_ const FE::project_config& get_project_config() const noexcept { return *m_project_config; }
    _FE_FORCE_INLINE_ static FE::game& get_game_instance() noexcept { return *(get_engine().m_game_instance); }
    _FE_FORCE_INLINE_ FE::int32 get_async_processor_count() const noexcept { return m_max_concurrency.get_max_concurrency() - 2; /* -(game + renderer) */ }
    
	template <typename T>
    _FE_FORCE_INLINE_ FE::polymorphic_allocator<T> get_thread_local_allocator() noexcept { return FE::polymorphic_allocator<T>( get_memory_resource() ); }

    template <typename T>
	_FE_FORCE_INLINE_ std::pmr::polymorphic_allocator<T> get_std_pmr_thread_local_allocator() noexcept { return std::pmr::polymorphic_allocator<T>( get_memory_resource() ); }

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