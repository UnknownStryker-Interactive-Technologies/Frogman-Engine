#ifndef _FE_HEADER_TOOL_MULTILINE_COMMENT_IN_BODY_TEST_HPP_
#define _FE_HEADER_TOOL_MULTILINE_COMMENT_IN_BODY_TEST_HPP_
// Block comments that span several lines inside class, struct and function bodies.
// Found by fuzzing FHT at commit 0c4394d. Each Current note describes that block alone in a file;
// Release, RelWithDebInfo and Debug agree unless the note says otherwise.
// Every block is valid Frogman C++ and passes g++ -std=c++20 -fsyntax-only with the FE definitions available.
// A block that rejects or hangs the file hides the blocks after it; FHT_repro_0c4394d.zip has smaller files.
//
// Expected generated.cpp once fixed, in any order:
//   construct and destruct: ::body_comment_ok1 ::body_comment_ok2 ::body_comment_ok3
//                           ::body_comment_ns_ok5::body_comment_ok5 ::body_comment_b1 ::body_comment_b2
//                           ::body_comment_b3 ::body_comment_sentinel_b4 ::body_comment_b5
//                           ::body_comment_sentinel_b5
//   register_enum_struct: ::body_comment_kind_ok4 with _A, _B;
//   associate_system: none
#include <FE/prerequisites.hxx>
#include <cstdint>


// ==== C. Controls that already work and must keep working ====
// Current: exit code 0, every type below is registered.
/*
* several lines at global scope
*/
class body_comment_ok1 {};
struct body_comment_ok2
{
	/***/
};
struct body_comment_ok3
{
	/** doc */
	var::int32 _x;
};
enum struct body_comment_kind_ok4
{
	_A,
	/* first
	second */
	_B
};
namespace body_comment_ns_ok5
{
	/* first
	second */
	class body_comment_ok5 {};
}


// ==== B1. Doxygen comment in a struct body ====
// Current: the run does not finish.
struct body_comment_b1
{
	/**
	* @brief doc
	*/
	var::int32 _x;
};


// ==== B2. Doxygen comment in a class body ====
// Current: the run does not finish.
class body_comment_b2
{
	/**
	*/
};


// ==== B3. Plain block comment in a struct body ====
// Current: the run does not finish.
struct body_comment_b3
{
	/* first
	*/
};


// ==== B4. Free function body ====
// Current: the run does not finish.
inline void body_comment_fn_b4() noexcept
{
	/* first
	second */
}
class body_comment_sentinel_b4 {};


// ==== B5. Member function body ====
// Current: the run does not finish.
class body_comment_b5
{
public:
	void f() noexcept
	{
		/* first
		second */
	}
};
class body_comment_sentinel_b5 {};


#endif // _FE_HEADER_TOOL_MULTILINE_COMMENT_IN_BODY_TEST_HPP_
