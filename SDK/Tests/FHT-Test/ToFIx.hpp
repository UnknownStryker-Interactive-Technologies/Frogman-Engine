
// ==== D4b. A namespace alias gets glued onto the name of the next namespace (1857 enum extension, parser namespace-name concatenation) ====
// (current: registered under the name "alias_ns4=std;namespaceN4b::Sentinel4b", which cannot compile.
//  The namespace count does not include the alias, so the counting path looks fine and only the
//  name-building path is off.)
namespace alias_ns4 = std;
namespace N4b {
	class Sentinel4b {};
}
class Sentinel4c {};


// ==== D7. Declarations that follow a closed nested namespace (parser.cpp 166~169 unconsumed `}` + tokenizer 1203~1204) ====
// (current: the declarations survive, but the enclosing namespace name is dropped. Inner7::S7 and
//  Before7b, which come before the nested namespace closes, are fine; only X7 / E7 / sys7 / T7 /
//  After7b are wrong. An empty Inner7b reproduces it too, so this is about the closing `}` popping
//  one name too many rather than about the nested namespace's contents.)
namespace N7 {
	namespace Inner7 { struct S7 {}; }
	class X7 {};
	enum struct E7 { V };
	FE_SYSTEM(FE::_WorldTick, FEWorldTag::Boot);
	void sys7(::FE::world&);
	struct T7 {};
}
namespace N7 {
	class Before7b {};      // before the nested namespace — currently correct
	namespace Inner7b {}
	class After7b {};       // after an empty nested namespace — currently ::After7b
}
namespace N7 {
	class Reopened7b {};    // no nested namespace — currently correct
}


// ==== D8. (control for D7) a nested namespace placed last must keep working ====
namespace N8 {
	class Q8 {};
	namespace Inner8 {}
}
class Sentinel8 {};


// ==== D10. `= 0` inside a virtual function body read as pure virtual (parser.cpp 343~356, backward token pattern) ====
// (current: C10a / C10b vanish without a single warning. They are included in the class count, so they
//  are parsed and then dropped during generation.)
class C10a
{
public:
	virtual void f() { for (int i = 0; i < 3; ++i) {} }   // must be registered
};
class C10b
{
public:
	virtual void f() { int i = 0; (void)i; }              // must be registered
};
class C10c
{
public:
	virtual void f() = 0;                                 // genuinely pure virtual — must NOT be registered (control, currently correct)
};


// ==== D12. explicit + a class name starting with 't' defeats constructor detection (parser.cpp 410, string.hxx 1054 — conjecture) ====
// (current: task_queue12a is registered with no warning, giving
//  'no matching function for call to task_queue12a()'. sched_queue12c has the same shape and differs
//  only in the first letter of its name; it is the control that pins the name-dependence conjecture.
//  If it is excluded with a C2512 warning like S6d in ReflextreeParseTest3.hpp, the conjecture holds.)
class task_queue12a
{
public:
	explicit task_queue12a(FE::int32 v_p) noexcept : m_a(v_p)
	{
	}
private:
	var::int32 m_a = 0;
};
class task_queue12b
{
public:
	task_queue12b() noexcept = default;
	explicit task_queue12b(FE::int32 v_p) noexcept : m_a(v_p)
	{
	}
private:
	var::int32 m_a = 0;
};
class sched_queue12c
{
public:
	explicit sched_queue12c(FE::int32 v_p) noexcept : m_a(v_p)
	{
	}
private:
	var::int32 m_a = 0;
};


// ==== 14. A class inside an #if 0 block gets registered (preprocessor directives are not handled) ====
// (current: ::Dead14 is registered, but after preprocessing that name does not exist — a compile error.
//  It is counted in the class total as well.)
#if 0
class Dead14 {};
#endif
class Sentinel14 {};


// ==== 15. A `= delete` default constructor slips past the C2512 check ====
// (current: ::D15 is registered with no warning, giving 'use of deleted function D15::D15()'.
//  The same check does catch D15b, whose default constructor is private — control.)
class D15
{
public:
	D15() = delete;
};
class D15b
{
private:
	D15b() {}
};
class Sentinel15 {};


// ==== 17. An uninitialized const-qualified non-static member implicitly deletes the default constructor ([class.default.ctor]) ====
// (current: S17a / S17b are registered with no warning, giving 'use of deleted function'. Only
//  top-level const counts: S17c holds a pointer to const, is default-constructible, and must be
//  registered. Reference members follow the same rule but fall outside Frogman C++'s POD structs,
//  so they are left out here.)
struct S17a
{
	int* const m_const_ptr;
};
struct S17b
{
	const int m_const_array[4];
};
struct S17c
{
	const int* m_ptr_to_const;
};

