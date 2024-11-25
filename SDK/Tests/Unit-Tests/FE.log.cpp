// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/algorithm/utility.hxx>
#include <gtest/gtest.h>
//std
#include <array>
#include <memory>




TEST(FE_LOG, log)
{
	_FE_MAYBE_UNUSED_ FE::uint32 l_age = 45;
	_FE_MAYBE_UNUSED_ FE::ASCII* l_name = "John";
	
	// From the drama series "SEAL Team"
	FE_LOG("Do you know how many frogmen we need to change a light burb? It is six. One has to change the light burb and the rest have to write a book about it.");

	FE_LOG("His name is ${%s@0} and his age is ${%u@1}.", l_name, &l_age);
}