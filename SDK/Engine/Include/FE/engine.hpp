#ifndef _FROGMAN_ENGINE_HPP_
#define _FROGMAN_ENGINE_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>
#include <FE/pool/memory_resource.hpp>
#include <FE/framework/ECS.hpp>
#include <FE/framework/framework.hpp>
#include <FE/framework/processors.hpp>
#include <FE/framework/system.hpp>

#define FROGMAN_ENGINE() CUSTOM_ENGINE(FE::engine)




BEGIN_NAMESPACE(FE)


class engine final : public FE::framework::framework_base
{
    FE::uint32 m_gc_batch_count;
    std::size_t m_fiber_stack_size;
	FE::system m_renderer_system;
    FE::system m_physics_system;
    FE::system m_audio_system;
    FE::system m_networking_system;

    framework::initializer_list m_entity_list;
	framework::system_table_initializer_list m_system_list;

public:
    engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
    virtual ~engine() noexcept override;

private:
    virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override;
    virtual FE::int32 run() override;
    virtual FE::int32 shutdown() override;
};


END_NAMESPACE
#endif