#ifndef _FE_HEADER_TOOL_ATTRIBUTE_MACRO_PLACEMENT_TEST_HPP_
#define _FE_HEADER_TOOL_ATTRIBUTE_MACRO_PLACEMENT_TEST_HPP_
// Attribute macros between the class key and the type name.
// Found by fuzzing FHT at commit 0c4394d. Each Current note describes that block alone in a file;
// Release, RelWithDebInfo and Debug agree unless the note says otherwise.
// Every block is valid Frogman C++ and passes g++ -std=c++20 -fsyntax-only with the FE definitions available.
// A block that rejects or hangs the file hides the blocks after it; FHT_repro_0c4394d.zip has smaller files.
// Note: P4 must stay the last declaration in this file because it rejects the file.
//
// Expected generated.cpp once fixed, in any order:
//   construct and destruct: ::attr_ok1 ::attr_ok3 ::attr_ok4 ::attr_p1 ::attr_p2 ::attr_p4
//   register_enum_struct: ::attr_kind_ok2 with _A; ::attr_kind_p3 with _A;
//   associate_system: none
#include <FE/prerequisites.hxx>
#include <cstdint>


// ==== C. Controls that already work and must keep working ====
// Current: exit code 0, every type below is registered under its own name.
class [[maybe_unused]] attr_ok1 {};
enum struct [[nodiscard]] attr_kind_ok2
{
	_A
};
struct [[nodiscard]] attr_ok3
{
	var::int32 _x;
};
struct alignas(16) attr_ok4
{
	var::int32 _x;
};


// ==== P1. Class ====
// Current: registered as the macro name glued to the class name.
class _FE_MAYBE_UNUSED_ attr_p1 {};


// ==== P2. Final class ====
// Current: registered as the macro name glued to the class name and final.
class _FE_MAYBE_UNUSED_ attr_p2 final {};


// ==== P3. Enum struct ====
// Current: registered under the macro name, with attr_kind_p3 and _A as its fields.
enum struct _FE_NODISCARD_ attr_kind_p3
{
	_A
};


// ==== P4. Struct ====
// Current: rejected, the struct is reported as polymorphic.
struct _FE_NODISCARD_ attr_p4
{
	var::int32 _x;
};


#endif // _FE_HEADER_TOOL_ATTRIBUTE_MACRO_PLACEMENT_TEST_HPP_
