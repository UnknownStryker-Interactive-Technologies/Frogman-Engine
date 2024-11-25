#ifndef _FE_FRAMEWORK_SUPER_OBJECT_BASE_HPP_
#define _FE_FRAMEWORK_SUPER_OBJECT_BASE_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
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

#include <FE/framework/framework.hpp>




BEGIN_NAMESPACE(FE::framework)

/*
The top class for all game objects in Frogman Engine.
*/
class super_object_base
{
	FE_CLASS(super_object_base);
public:
	super_object_base() noexcept;
	virtual ~super_object_base() noexcept;

protected:
	virtual void on_construction();
	virtual void on_destruction();

	FE_PROPERTY(m_is_tick_enabled);
	var::boolean m_is_tick_enabled;
	virtual void tick(_FE_MAYBE_UNUSED_ FE::float64 delta_second_p);

public:
	template<typename T>
	void serialize(std::filesystem::path path_p, directory_char_t* filename_p, const T& object_p) noexcept
	{
		framework_base::get_engine().access_property_reflection().serialize(path_p, filename_p, object_p);
	}

	template<typename T>
	void deserialize(std::filesystem::path path_p, directory_char_t* filename_p, T& out_object_p) noexcept
	{
		framework_base::get_engine().access_property_reflection().deserialize(path_p, filename_p, out_object_p);
	}

	operator FE::framework::weak_ptr<super_object_base>() const noexcept;

private:
	FE::framework::ref_block<super_object_base>* m_ref_block;
};

END_NAMESPACE
#endif