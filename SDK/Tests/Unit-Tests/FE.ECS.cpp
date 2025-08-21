#include "FE.ECS.hpp"




TEST(ECS, instantiate_entity)
{
	FE::ECS ecs;
	FE::entity<object> e = ecs.instanciate_entity<object>("TestEntity", 100);
}