#ifndef _FROGMAN_ENGINE_REFLECTION_HPP_
#define _FROGMAN_ENGINE_REFLECTION_HPP_
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
#include <FE/framework/framework.hxx>
#include <FE/framework/reflection.hxx>
#include <FE/function.hxx>




BEGIN_NAMESPACE(FE::reflection)


using object = framework::reflection::instance_metadata;
using enum_struct = framework::reflection::enum_metadata;


template<typename T>
_FE_FORCE_INLINE_ object* get_object() noexcept // This method may return a nullptr.
{
    return FE::framework::framework_base::get_framework().get_property_reflection().get_instance_metadata<T>();
}

_FE_FORCE_INLINE_ FE::task_base* get_method(FE::ASCII* const signature_p) noexcept // This method may return a nullptr.
{
    return FE::framework::framework_base::get_framework().get_method_reflection().retrieve(signature_p);
}

_FE_FORCE_INLINE_ enum_struct* get_enum_struct(FE::ASCII* const identifier_p) // This method may return a nullptr.
{
    return FE::framework::framework_base::get_framework().get_enum_reflection().retrieve_enum_struct_metadata(identifier_p);
}

template<typename T>
_FE_FORCE_INLINE_ void serialize(std::pmr::string& out_ret_buffer_p, const T& object_p, FE::ASCII* const version_p) noexcept
{
	FE::framework::framework_base::get_framework().get_property_reflection().serialize<T>(out_ret_buffer_p, object_p, version_p);
}

template<typename T>
_FE_FORCE_INLINE_ void deserialize(const std::pmr::string& data_p, T& out_object_p, FE::ASCII* const version_p) noexcept
{
	FE::framework::framework_base::get_framework().get_property_reflection().deserialize<T>(data_p, out_object_p, version_p);
}


END_NAMESPACE
#endif