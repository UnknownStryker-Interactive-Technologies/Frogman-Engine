#ifndef _FE_HEADER_TOOL_PURE_VIRTUAL_DETECTION_TEST_HPP_
#define _FE_HEADER_TOOL_PURE_VIRTUAL_DETECTION_TEST_HPP_
// Abstract classes whose pure virtual declaration is followed by a member with a body.
// Found by fuzzing FHT at commit 0c4394d. Each Current note describes that block alone in a file;
// Release, RelWithDebInfo and Debug agree unless the note says otherwise.
// Every block is valid Frogman C++ and passes g++ -std=c++20 -fsyntax-only with the FE definitions available.
// A block that rejects or hangs the file hides the blocks after it; FHT_repro_0c4394d.zip has smaller files.
//
// Expected generated.cpp once fixed, in any order:
//   construct and destruct: ::body_virtual_ok4 ::body_virtual_ok5 ::derived_ok6
//   register_enum_struct: none
//   associate_system: none
//   must not be registered: ::abstract_ok1 ::abstract_ok2 ::abstract_ok3 ::abstract_v1 ::abstract_v2
//                           ::abstract_v3
#include <FE/prerequisites.hxx>
#include <FE/framework/reflection.hxx>
#include <cstdint>


// ==== C. Controls that already work and must keep working ====
// Current: exit code 0; only body_virtual_ok4, body_virtual_ok5 and derived_ok6 are registered.
class abstract_ok1
{
public:
	virtual void pure_f(FE::float32 dt_p) noexcept = 0;
};
class abstract_ok2
{
public:
	virtual void pure_f(FE::float32 dt_p) noexcept = 0;
	FE_SYSTEM(FE::_PreEntityTick, ::EditorWorldTag::AssetBrowser);
	static void member_system_ok2(FE::world& world_p) noexcept;
};
class abstract_ok3
{
public:
	void g() {}
	virtual void pure_f() = 0;
};
class body_virtual_ok4
{
public:
	virtual void f() { for (int i = 0; i < 3; ++i) {} }
};
class body_virtual_ok5
{
public:
	virtual void f() { int i = 0; (void)i; }
};
class derived_ok6 : public abstract_ok1
{
public:
	void pure_f(FE::float32 dt_p) noexcept override
	{
		(void)dt_p;
	}
};


// ==== V. Pure virtual declaration followed by a member with a body ====
// Current: exit code 0, all three are registered and generated.cpp fails with an abstract class new-expression.
class abstract_v1
{
public:
	virtual void pure_f(FE::float32 dt_p) noexcept = 0;
	var::int32& operator[](FE::size index_p) noexcept
	{
		(void)index_p;
		return m_a;
	}
private:
	var::int32 m_a = 0;
};
class abstract_v2
{
public:
	virtual void pure_f(FE::float32 dt_p) noexcept = 0;
	var::int32 get() noexcept
	{
		return 0;
	}
};
class abstract_v3
{
public:
	virtual void pure_f() = 0;
	void g() {}
};


#endif // _FE_HEADER_TOOL_PURE_VIRTUAL_DETECTION_TEST_HPP_
