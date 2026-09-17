#ifndef _FE_HEADER_TOOL_ALIAS_TEMPLATE_PURGE_TEST_HPP_
#define _FE_HEADER_TOOL_ALIAS_TEMPLATE_PURGE_TEST_HPP_
// The declaration right after an alias template is dropped.
// Found by fuzzing FHT at commit 0c4394d. Each Current note describes that block alone in a file;
// Release, RelWithDebInfo and Debug agree unless the note says otherwise.
// Every block is valid Frogman C++ and passes g++ -std=c++20 -fsyntax-only with the FE definitions available.
// A block that rejects or hangs the file hides the blocks after it; FHT_repro_0c4394d.zip has smaller files.
// Note: A7 must stay the last declaration in this file.
//
// Expected generated.cpp once fixed, in any order:
//   construct and destruct: ::alias_after_ok1 ::alias_after_ok2 ::alias_after_ok3 ::alias_after_ok4
//                           ::alias_after_ok5 ::alias_after_a1 ::alias_sentinel_a1 ::alias_after_a2
//                           ::alias_sentinel_a2 ::alias_sentinel_a3 ::alias_ns_a4::alias_inner_a4
//                           ::alias_sentinel_a4 ::alias_ns_a5::alias_after_a5
//                           ::alias_ns_a5::alias_sentinel_a5 ::alias_sentinel_a6
//   register_enum_struct: ::alias_after_kind_a3 with _A;
//   associate_system: ::alias_system_a6 for FEWorldTag::Boot at FE::_WorldTick;
//                     ::alias_system_a7 for FEWorldTag::Boot at FE::_WorldTick;
#include <FE/prerequisites.hxx>
#include <FE/framework/reflection.hxx>
#include <cstdint>
#include <type_traits>
#include <vector>


// ==== C. Controls that already work and must keep working ====
// Current: exit code 0, every type below is registered. The second alias template swallows nothing visible.
template <typename T>
inline constexpr var::boolean alias_value_ok1 = std::is_class_v<T>;
class alias_after_ok1 {};
template <typename T>
T alias_function_ok2(T a_p);
class alias_after_ok2 {};
template <typename T>
struct alias_struct_template_ok3
{
	T _v;
};
class alias_after_ok3 {};
template <typename T>
concept alias_concept_ok4 = requires(T t_p) { t_p.size(); };
class alias_after_ok4 {};
template <typename T>
using alias_type_ok5a = std::vector<T>;
template <typename T>
using alias_type_ok5b = std::vector<T>;
class alias_after_ok5 {};


// ==== A1. Class right after an alias template ====
// Current: exit code 0, but alias_after_a1 is silently missing.
template <typename T>
using alias_type_a1 = std::vector<T>;
class alias_after_a1 {};
class alias_sentinel_a1 {};


// ==== A2. Struct right after an alias template ====
// Current: exit code 0, but alias_after_a2 is silently missing.
template <typename T>
using alias_type_a2 = std::vector<T>;
struct alias_after_a2
{
	var::int32 _x;
};
struct alias_sentinel_a2 {};


// ==== A3. Enum struct right after an alias template ====
// Current: exit code 0, but alias_after_kind_a3 is silently missing.
template <typename T>
using alias_type_a3 = std::vector<T>;
enum struct alias_after_kind_a3
{
	_A
};
class alias_sentinel_a3 {};


// ==== A4. Namespace right after an alias template ====
// Current: exit code 0, but the namespace content is silently missing.
template <typename T>
using alias_type_a4 = std::vector<T>;
namespace alias_ns_a4
{
	class alias_inner_a4 {};
}
class alias_sentinel_a4 {};


// ==== A5. Alias template inside BEGIN_NAMESPACE ====
// Current: exit code 0, but alias_after_a5 is silently missing.
BEGIN_NAMESPACE(alias_ns_a5)
template <typename T>
using alias_type_a5 = std::vector<T>;
class alias_after_a5 {};
class alias_sentinel_a5 {};
END_NAMESPACE


// ==== A6. FE_SYSTEM right after an alias template ====
// Current: rejected, the function is reported as not qualified as an ECS system method.
template <typename T>
using alias_type_a6 = std::vector<T>;
FE_SYSTEM(FE::_WorldTick, FEWorldTag::Boot);
void alias_system_a6(FE::world& world_p);
class alias_sentinel_a6 {};


// ==== A7. Same as A6 when the system function is the last declaration ====
// Current: Release and RelWithDebInfo do not finish; Debug exits at once with code 9.
template <typename T>
using alias_type_a7 = std::vector<T>;
FE_SYSTEM(FE::_WorldTick, FEWorldTag::Boot);
void alias_system_a7(FE::world& world_p);


#endif // _FE_HEADER_TOOL_ALIAS_TEMPLATE_PURGE_TEST_HPP_
