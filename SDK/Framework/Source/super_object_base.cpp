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
#include <FE/framework/super_object_base.hpp>




BEGIN_NAMESPACE(FE::framework)


super_object_base::super_object_base() noexcept : m_is_tick_enabled(false)
{
    this->m_ref_block = FE::framework::managed::allocate_ref_block<super_object_base>();
	this->m_ref_block->_reference.store(this, std::memory_order_relaxed);
}

super_object_base::~super_object_base() noexcept
{
    this->m_ref_block->_reference.store(nullptr, std::memory_order_release);
}

void super_object_base::on_construction()
{
}

void super_object_base::on_destruction()
{
}

void super_object_base::tick(_FE_MAYBE_UNUSED_ FE::float64 delta_second_p)
{
}

super_object_base::operator FE::framework::weak_ptr<super_object_base>() const noexcept
{
	FE::framework::weak_ptr<super_object_base> l_weak_ptr;
	l_weak_ptr.m_ptr = this->m_ref_block;
	return l_weak_ptr;
}

END_NAMESPACE
