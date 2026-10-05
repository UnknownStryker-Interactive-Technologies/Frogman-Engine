#include "pong.h.generated.h"
#ifndef _PONG_H_
#define _PONG_H_
// Copyright © from 2026 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.
#include <FE/prerequisites.hxx>
#include <FE/reflection.hpp>




FE_WORLD_TAG
{
	_PongWorld
};

FE_SYSTEM(FE::SystemCallPhase::_EngineInitialization, FEWorldTag::_PongWorld);
void hello_world(FE::world& world_p)
{
	FE_LOG(FE::log::Severity::_Info, "Hello, World!");
}




#endif