// Copyright © 2023~ UNKNOWN STRYKER. All Rights Reserved.
#include <FE/hash.hpp>


FE::random_integer<var::uint64> FE::hash_base::s_random_integer;
FE::uint64 FE::hash_base::s_seed = s_random_integer.ranged_random_integer(0, FE::max_value<var::uint64>);