#ifndef _FE_HEADER_TOOL_COMMENT_MARKER_IN_BLOCK_COMMENT_TEST_HPP_
#define _FE_HEADER_TOOL_COMMENT_MARKER_IN_BLOCK_COMMENT_TEST_HPP_
// Block comments whose text starts with two slashes.
// Found by fuzzing FHT at commit 0c4394d. Each observation note describes that block alone in a file;
// Release, RelWithDebInfo and Debug agree unless the note says otherwise.
// Every block is valid Frogman C++ and passes g++ -std=c++20 -fsyntax-only with the FE definitions available.
// A block that rejects or hangs the file hides the blocks after it; FHT_repro_0c4394d.zip has smaller files.
// Warning: at 0c4394d the Release and RelWithDebInfo builds grew memory without bound on M1 to M5; run with a memory limit.
// Note: G1 must stay the last block in this file.
//
// Expected generated.cpp once fixed, in any order:
//   construct and destruct: ::marker_ok1 ::marker_ok2 ::marker_ok3 ::marker_ok4 ::marker_ns_ok6::marker_ok6
//                           ::marker_ok7 ::marker_ok8 ::marker_ns_m5::marker_m5 ::marker_ns_m7::marker_m7
//                           ::marker_before_g1 ::marker_lost_g1a ::marker_lost_g1b
//   register_enum_struct: ::marker_kind_ok5 with _A; ::marker_kind_m1 with _A; ::marker_kind_m2 with _A;
//                         ::marker_kind_m3 with _A; ::marker_ns_m4::marker_kind_m4 with _A;
//   associate_system: ::marker_ns_m6::marker_system_m6 for FEWorldTag::Boot at FE::_WorldTick;
#include <FE/prerequisites.hxx>
#include <FE/framework/reflection.hxx>
#include <cstdint>


// ==== C. Controls that already work and must keep working ====
// At 0c4394d, before the fix: exit code 0, every type below is registered.
class marker_ok1
{
	/* // */
};
struct marker_ok2
{
	/* // */
	var::int32 _x;
};
inline void marker_fn_ok3() noexcept
{
	/* // */
}
class marker_ok3 {};
class marker_ok4
{
	enum struct marker_nested_kind_ok4
	{
		_A /* // */
	};
};
enum struct marker_kind_ok5
{
	_A /* x // y */
};
namespace marker_ns_ok6
{
	/* x // */
	class marker_ok6 {};
}
/* a // b */
class marker_ok7 {};
/*
* see http://example.com
*/
class marker_ok8 {};


// ==== M1. Enum struct body ====
// At 0c4394d, before the fix: Release and RelWithDebInfo grow memory without bound; Debug exits at once with code 9.
enum struct marker_kind_m1
{
	_A /* // */
};


// ==== M2. Same with text after the slashes ====
// At 0c4394d, before the fix: same as M1.
enum struct marker_kind_m2
{
	_A /* // x */
};


// ==== M3. Same without a space after the opener ====
// At 0c4394d, before the fix: same as M1.
enum struct marker_kind_m3
{
	_A /*// */
};


// ==== M4. Namespace body before a declaration ====
// At 0c4394d, before the fix: same as M1.
namespace marker_ns_m4
{
	/* // */
	enum struct marker_kind_m4
	{
		_A
	};
}


// ==== M5. Namespace body after the last declaration ====
// At 0c4394d, before the fix: same as M1.
namespace marker_ns_m5
{
	class marker_m5 {};
	/* // */
}


// ==== M6. Right after FE_SYSTEM in a namespace ====
// At 0c4394d, before the fix: Release and RelWithDebInfo do not finish; Debug exits at once with code 9.
namespace marker_ns_m6
{
	FE_SYSTEM(FE::_WorldTick, FEWorldTag::Boot);
	/* // */
	void marker_system_m6(FE::world& world_p);
}


// ==== M7. Inside BEGIN_NAMESPACE ====
// At 0c4394d, before the fix: C1075, the file is skipped.
BEGIN_NAMESPACE(marker_ns_m7)
/* // */
class marker_m7 {};
END_NAMESPACE


// ==== G1. Global scope after another declaration ====
// At 0c4394d, before the fix: exit code 0, but marker_lost_g1a and marker_lost_g1b are silently missing.
class marker_before_g1 {};
/* // */
class marker_lost_g1a {};
class marker_lost_g1b {};


#endif // _FE_HEADER_TOOL_COMMENT_MARKER_IN_BLOCK_COMMENT_TEST_HPP_
