/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include <FE/framework/ECS.hpp>
#include <FE/framework/reflection.hpp>


class terrorist : public FE::archetype_base
{
	FE_CLASS(terrorist);
	FE_CLASS_HAS_A_BASE(FE::archetype_base);
public:
	terrorist() = default;
	~terrorist() = default;
};

class AK47 : public FE::archetype_base
{
	FE_CLASS(AK47);
	FE_CLASS_HAS_A_BASE(FE::archetype_base);
public:
	AK47() = default;
	~AK47() = default;
};

class ak_ammo : public FE::archetype_base
{
	FE_CLASS(ak_ammo);
	FE_CLASS_HAS_A_BASE(FE::archetype_base);
public:
	ak_ammo() = default;
	~ak_ammo() = default;
};

class ak_magazine : public FE::component_base
{
	FE_CLASS(ak_magazine);
	FE_CLASS_HAS_A_BASE(FE::archetype_base);
	std::vector<ak_ammo> _rounds;
public:
	ak_magazine() = default;
	~ak_magazine() = default;
};
