#ifndef _FE_HEADER_TOOL_LITERAL_TOKENIZATION_TEST_HPP_
#define _FE_HEADER_TOOL_LITERAL_TOKENIZATION_TEST_HPP_
// Literal tokenization: quotes of the other kind, escaped backslashes, digit separators and raw strings.
// Found by fuzzing FHT at commit 0c4394d. Each Current note describes that block alone in a file;
// Release, RelWithDebInfo and Debug agree unless the note says otherwise.
// Every block is valid Frogman C++ and passes g++ -std=c++20 -fsyntax-only with the FE definitions available.
// A block that rejects or hangs the file hides the blocks after it; FHT_repro_0c4394d.zip has smaller files.
//
// Expected generated.cpp once fixed, in any order:
//   construct and destruct: ::lit_member_c10 ::lit_member_c11 ::lit_sentinel_c ::lit_sentinel_l1a
//                           ::lit_sentinel_l1b ::lit_sentinel_l1c ::lit_sentinel_l1d ::lit_hidden_l2
//                           ::lit_sentinel_l2 ::lit_sentinel_l3a ::lit_sentinel_l3b ::lit_sentinel_l3c
//                           ::lit_sentinel_l4a ::lit_sentinel_l4b ::lit_sentinel_l4c ::lit_sentinel_l4d
//                           ::lit_hidden_l5a ::lit_sentinel_l5a ::lit_hidden_l5b ::lit_sentinel_l5b
//                           ::lit_sentinel_l6a ::lit_sentinel_l6b ::lit_sentinel_l6c ::lit_member_l7a
//                           ::lit_member_l7b ::lit_ns_l7::lit_member_l7c
//   register_enum_struct: ::lit_hidden_kind_l2 with _A;
//   associate_system: none
enum struct kindZ1
{
	_A = 1'000,
	_B
};

inline FE::ASCII* g_holololol = "\\\"";
inline auto g_lol8 = u8'8';
inline auto g_lol16 = u'u';
inline auto g_lol32 = U'U';
inline auto g_lol32 = L'L';
inline constexpr var::int64 g_lollol = 1'000000;


// ==== C. Controls that already work and must keep working ====
// Current: exit code 0, every type below is registered.
inline constexpr var::int64 g_lit_c1 = 1'000'000;
inline FE::ASCII* g_lit_c2 = "\\a";
inline constexpr var::ASCII g_lit_c3 = '\'';
inline FE::ASCII* g_lit_c4 = "\"";
inline FE::ASCII* g_lit_c5 = "\\\"";
inline FE::ASCII* g_lit_c6 = "a'b'c";
inline FE::ASCII* g_lit_c7 = R"(it's)";
inline FE::ASCII* g_lit_c8 = R"d(abc)d";
inline FE::ASCII* g_lit_c9 = "//";
class lit_member_c10
{
public:
	FE::ASCII* m_url = "http://example.com";
};
class lit_member_c11
{
public:
	FE::ASCII* m_path = "a//b";
};
class lit_sentinel_c {};


// ==== L1a. Odd number of digit separators in a file ====
// Current: Release and RelWithDebInfo report C2001 and skip the file; Debug exits with code 0 and the sentinel is silently missing.
inline constexpr var::int64 g_lit_l1a = 1'000;
class lit_sentinel_l1a {};


// ==== L1b. Same with a hexadecimal literal ====
// Current: Release and RelWithDebInfo report C2001 and skip the file; Debug exits with code 0 and the sentinel is silently missing.
inline constexpr var::uint64 g_lit_l1b = 0xFF'FFu;
class lit_sentinel_l1b {};


// ==== L1c. Same with a floating point literal ====
// Current: Release and RelWithDebInfo report C2001 and skip the file; Debug exits with code 0 and the sentinel is silently missing.
inline constexpr var::float64 g_lit_l1c = 1'000.5;
class lit_sentinel_l1c {};


// ==== L1d. Three separators in one literal ====
// Current: Release and RelWithDebInfo report C2001 and skip the file; Debug exits with code 0 and the sentinel is silently missing.
inline constexpr var::int64 g_lit_l1d = 1'000'000'000;
class lit_sentinel_l1d {};


// ==== L2. Two digit separators on different lines ====
// Current: exit code 0, but lit_hidden_l2 and lit_hidden_kind_l2 are silently missing.
inline constexpr var::int64 g_lit_l2a = 1'000;
class lit_hidden_l2 {};
enum struct lit_hidden_kind_l2 { _A };
inline constexpr var::int64 g_lit_l2b = 2'000;
class lit_sentinel_l2 {};


// ==== L3a. Char literal holding an escaped backslash ====
// Current: Release and RelWithDebInfo report C2001 and skip the file; Debug exits with code 0 and the sentinel is silently missing.
inline constexpr var::ASCII g_lit_l3a = '\\';
class lit_sentinel_l3a {};


// ==== L3b. String literal holding only an escaped backslash ====
// Current: C2001, the file is skipped.
inline FE::ASCII* g_lit_l3b = "\\";
class lit_sentinel_l3b {};


// ==== L3c. String literal ending with an escaped backslash ====
// Current: C2001, the file is skipped.
inline FE::ASCII* g_lit_l3c = "a\\";
class lit_sentinel_l3c {};


// ==== L4a. Char literal holding a double quote ====
// Current: C2001, the file is skipped.
inline constexpr var::ASCII g_lit_l4a = '"';
class lit_sentinel_l4a {};


// ==== L4b. Char literal holding an escaped double quote ====
// Current: C2001, the file is skipped.
inline constexpr var::ASCII g_lit_l4b = '\"';
class lit_sentinel_l4b {};


// ==== L4c. String literal holding an escaped apostrophe ====
// Current: C2001, the file is skipped.
inline FE::ASCII* g_lit_l4c = "\'";
class lit_sentinel_l4c {};


// ==== L4d. String literal holding one apostrophe ====
// Current: C2001, the file is skipped.
inline FE::ASCII* g_lit_l4d = "it's";
class lit_sentinel_l4d {};


// ==== L5a. Two such string literals on different lines ====
// Current: exit code 0, but lit_hidden_l5a is silently missing.
inline FE::ASCII* g_lit_l5a1 = "it's";
class lit_hidden_l5a {};
inline FE::ASCII* g_lit_l5a2 = "that's";
class lit_sentinel_l5a {};


// ==== L5b. Two char literals holding a double quote on different lines ====
// Current: exit code 0, but lit_hidden_l5b is silently missing.
inline constexpr var::ASCII g_lit_l5b1 = '"';
class lit_hidden_l5b {};
inline constexpr var::ASCII g_lit_l5b2 = '"';
class lit_sentinel_l5b {};


// ==== L6a. Raw string with a delimiter whose text contains a closing paren and quote ====
// Current: C2001, the file is skipped.
inline FE::ASCII* g_lit_l6a = R"d(a)")d";
class lit_sentinel_l6a {};


// ==== L6b. Same with text after the closing paren and quote ====
// Current: C2001, the file is skipped.
inline FE::ASCII* g_lit_l6b = R"d(x)"y)d";
class lit_sentinel_l6b {};


// ==== L6c. Same with a lone quote first ====
// Current: C2001, the file is skipped.
inline FE::ASCII* g_lit_l6c = R"d(")" })d";
class lit_sentinel_l6c {};


// ==== L7a. Class member string literal whose text starts with two slashes ====
// Current: C2001, the file is skipped. Two slashes later in the text, or at namespace scope, are fine.
class lit_member_l7a
{
public:
	FE::ASCII* m_prefix = "// a";
};


// ==== L7b. Same in a struct ====
// Current: C2001, the file is skipped.
struct lit_member_l7b
{
	FE::ASCII* _prefix = "//";
};


// ==== L7c. Same in a struct inside BEGIN_NAMESPACE ====
// Current: C2001, the file is skipped.
BEGIN_NAMESPACE(lit_ns_l7)
struct lit_member_l7c
{
	FE::ASCII* _prefix = "//";
};
END_NAMESPACE


#endif // _FE_HEADER_TOOL_LITERAL_TOKENIZATION_TEST_HPP_
