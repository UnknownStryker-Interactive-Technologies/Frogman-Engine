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
#include <FE/prerequisites.h>
#include <FE/framework.h>
#include <FE/renderer.hpp>


#define FROGMAN_ENGINE() CUSTOM_ENGINE(FE::engine)




BEGIN_NAMESPACE(FE)


class engine final : public FE::framework::framework_base
{
    FE::uint32 m_gc_batch_count;
    std::size_t m_fiber_stack_size;

    framework::initializer_list m_entity_list;
	framework::system_table_initializer_list m_system_list;

    std::unique_ptr<window_config> m_window_config;
    std::unique_ptr<FE::renderer> m_renderer;

public:
    class reference : public FE::component_base
    {
    public:
        FE::engine* _engine;

		reference(FE::engine* engine_p) noexcept
            :   _engine(engine_p) {}
    };
private:
    reference m_this_pointer;

public:
    engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
    virtual ~engine() noexcept override;

private:
    virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override;
    virtual FE::int32 run() override;
    virtual FE::int32 shutdown() override;

private:
    _FE_FORCE_INLINE_ static FE::engine& __get_engine() noexcept { return static_cast<FE::engine&>(FE::framework::framework_base::get_framework()); }

private: // Callbacks
	static void __renderer_main(FE::component_base* engine_reference_p) noexcept;
	static void __on_window_close(GLFWwindow* window_p) noexcept;
	static void __key_callback(GLFWwindow* const window_p, FE::int32 key_p, FE::int32 scancode_p, FE::int32 action_p, FE::int32 mods_p) noexcept;
	static void __mouse_button_callback(GLFWwindow* const window_p, FE::int32 button_p, FE::int32 action_p, FE::int32 mods_p) noexcept;
	static void __cursor_position_callback(GLFWwindow* const window_p, double x_p, double y_p) noexcept;
};


END_NAMESPACE
#endif