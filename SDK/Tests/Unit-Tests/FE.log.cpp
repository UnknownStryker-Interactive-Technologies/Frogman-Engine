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

	FE_LOG(FE::log::Severity::_Info, "His name is ${%s@0} and his age is ${%u@1}.", l_name, &l_age);
	FE_LOG(FE::log::Severity::_Warning, "This is a warning!");
}