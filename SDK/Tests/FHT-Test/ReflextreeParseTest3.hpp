#pragma once
// FHT DOOM 헤더 — 2026-09-15, FHT 커밋 08d33d1 기준. 1~3차 퍼징(유효 입력 약 1,400건 + 수동 probe 약 300건)에서 확인된 악마 전부 (D1~D13, 단 D6 은 삭제 + 스타일 밖 X1~X2 + 정책 14~16). struct 에 OOP 키워드(final/virtual/상속/접근 지정자/생성자·소멸자)를 쓰는 것 자체가 Frogman C++ 에서 UB 라, 이 헤더의 struct 는 전부 멤버 변수만 있는 POD 임
// 유효한 Frogman C++ (g++ -std=c++20 -Wall -Wextra -fsyntax-only 통과). enum struct 만 사용, 다중 상속·union 없음.
// 항목 번호 = 악마 번호(D1~D13) + 변형 문자. SentinelXX 는 "그 뒤 선언이 조용히 사라지는지"를 잡는 감시용.
//
// 패치 후 기대 출력 (generated.cpp, 순서 무관):
//   construct : ::C1b ::C1c ::C1d ::C1f ::S1g ::C1g ::C2a ::C2b ::C2c ::task_queue12b ::use_enum13
//               ::S6b   (X 블록을 남기면) ::SX1a ::CX1b ::SentinelX1
//               ::N7::X7 ::N7::T7 ::N7::Inner7::S7 ::N7::Before7b ::N7::After7b ::N7::Reopened7b
//               ::N8::Q8 ::C10a ::C10b
//               ::Sentinel1a ::Sentinel1e ::Sentinel3a ::Sentinel3b ::Sentinel3c ::Sentinel3d ::Sentinel4a ::Sentinel4b
//               ::Sentinel5a ::Sentinel5b ::Sentinel5c ::Sentinel8 ::Sentinel9a ::Sentinel9b ::Sentinel9c ::Sentinel9d ::Sentinel14
//   register_enum_struct : ::N7::E7 ::kind13
//   associate_system : (::FEWorldTag::Boot, ::FE::_PostPhysics, &::sys11), (FEWorldTag::Boot, FE::_WorldTick, &::N7::sys7),
//                      (X2 를 남기면) (FEWorldTag::Boot, FE::_WorldTick, &::sysX2)   ※ X 블록은 스타일 밖(면책) — 지우면 기대 출력에서도 빼면 됨
//   등록되면 안 되는 것 : ::ic ::ure ::16 ::8 ::E4a(불투명 선언) ::S6d ::S6e(class 대조군) ::C10c(진짜 순수가상) ::task_queue12a(explicit 생성자만 있음)
//                        ::Dead14 ::D15 ::C16, 그리고 "alias_ns4=std;..." 처럼 토큰이 이어 붙은 이름
//   ::Anon2c 는 정책 결정 사항(익명 네임스페이스는 다른 TU에서 접근 불가) — 단 행에 걸리지 않고 Sentinel2c 는 나와야 함

#include <cstdint>
#include <vector>

// ==== D1. 원문 문자 스캔 fallback (tokenizer.cpp 366~446) — 리터럴·주석을 모름 ====
// 1a. 네임스페이스 스코프 리터럴 안의 { }   (현재: C2001 / C1075 로 파일 스킵)
inline const char* g_s1a = "{}";
inline const char* g_s1a2 = "{";
inline constexpr char g_c1a = '{';
inline const char* g_r1a = R"(x{)";
class Sentinel1a {};
// 1b. 클래스 본문 문자열 안의 {   (현재: C1075)
class C1b {
public:
	const char* m_s = "{";
};
// 1c. 클래스 본문의 "a{b}c"   (현재: 재렉싱 단계에서 C2001)
class C1c {
public:
	const char* m_s = "a{b}c";
};
// 1d. 클래스 본문 주석 안의 {   (현재: 줄주석·블록주석 모두 C1075)
class C1d {
public:
	int m_x = 0; // {
	int m_y = 0; /* { */
};
// 1e. 문자 리터럴 ';'   (현재: 이후 전부 누락)
inline constexpr char g_c1e = ';';
class Sentinel1e {};
// 1f. 클래스 본문 문자열 안의 }   (현재: C2001)
class C1f {
public:
	const char* m_s = "}";
};
// 1g. alignas 가 이름 자리에 옴   (현재: 이름이 16/8 이 되고 class 쪽은 비결정적 크래시/행 — D1 이 만든 토큰 경계 오류를 parser.cpp 270~288 이 UB 로 증폭)
struct alignas(16) S1g {};
class alignas(8) C1g {};

// ==== D2. 빈 토큰이면 iterator 를 안 옮기고 continue (tokenizer.cpp 69~71, 491~493) ====
// 2a. 멤버 선언 안의 ?:   (현재: 무한루프)
class C2a {
public:
	int m_m = 1 ? 2 : 3;
	int f(int a) const { return a < 0 ? -a : a; }
};
// 2b. 멤버 선언 안의 줄 연속 \   (현재: 무한루프)
class C2b {
public:
	int m_m = 1 + \
		2;
};
// 2c. 익명 네임스페이스   (현재: 무한루프)
namespace {
	class Anon2c {};
}
class Sentinel2c {};

// ==== D3. 키워드 접두 매칭 (2171 class, 2078 struct, 1791 namespace, 2001 enum) ====
using classic3 = int;
class Sentinel3a {};
using structure3 = int;
class Sentinel3b {};
using namespaced3 = int;
class Sentinel3c {};
using enumerate3 = int;
class Sentinel3d {};

// ==== D4. "다음 { 까지" 스캔이 ; 를 무시 (1857 enum extension, parser 네임스페이스 이름 결합) ====
// 4a. 밑 타입이 있는 enum struct 불투명 선언   (현재: 이후 전부 누락)
enum struct E4a : int;
class Sentinel4a {};
// 4b. 네임스페이스 별칭   (현재: 다음 namespace 의 내용이 "alias_ns4=std;...N4b::" 라는 이름으로 등록)
namespace alias_ns4 = std;
namespace N4b {
	class Sentinel4b {};
}

// ==== D5. 템플릿 렉서의 문맥 스택 미복구 (1104~1107 `template <>`, 본문 없는 템플릿 선언) ====
// 5a. 명시적 특수화   (현재: 다음 선언 하나 누락)
template <typename T> struct T5a { T v; };
template <> struct T5a<int> { int v; int w; };
class Sentinel5a {};
// 5b. 본문 없는 함수 템플릿 선언   (현재: 이후 누락)
template <typename T> T tf5b(T a);
class Sentinel5b {};
// 5c. 별칭 템플릿 · 변수 템플릿   (현재: 이후 누락)
template <typename T> using Alias5c = std::vector<T>;
template <typename T> inline constexpr bool is_thing5c_v = std::is_class_v<T>;
class Sentinel5c {};

// ==== D7. 중첩 네임스페이스가 닫힌 뒤의 선언 (parser.cpp 166~169 `}` 미소비 + tokenizer 1203~1204) ====
namespace N7 {
	namespace Inner7 { struct S7 {}; }
	class X7 {};
	enum struct E7 { V };
	FE_SYSTEM(FE::_WorldTick, FEWorldTag::Boot);
	void sys7(::FE::world&);
	struct T7 {};
}
namespace N7 {
	class Before7b {};
	namespace Inner7b {}
	class After7b {};
}
namespace N7 {
	class Reopened7b {};
}

// ==== D8. (D7 의 감시) 중첩 네임스페이스가 마지막에 오면 지금도 통과해야 하는 대조군 ====
namespace N8 {
	class Q8 {};
	namespace Inner8 {}
}
class Sentinel8 {};

// ==== D9. 파서의 ";" 종료 AnyDecl 을 클래스 본문으로 오인하는 경로의 감시용 대조군 (현재 통과, 회귀 방지) ====
class Sentinel9a; class Sentinel9a {};
struct Sentinel9b;
struct Sentinel9b {};
enum struct E9c;
class Sentinel9c {};
inline const char* g_s9d = "a;b"; // 문자열 안의 ; (현재 우연히 통과)
class Sentinel9d {};

// ==== D10. 가상 함수 본문 안의 "= 0" 을 순수가상으로 오인 (parser.cpp 343~356, 역방향 토큰 패턴) ====
class C10a {
public:
	virtual void f() { for (int i = 0; i < 3; ++i) {} }   // 등록돼야 함
};
class C10b {
public:
	virtual void f() { int i = 0; (void)i; }                // 등록돼야 함
};
class C10c {
public:
	virtual void f() = 0;                                   // 진짜 순수가상 — 등록 금지
};

// ==== D11. FE_SYSTEM 인자의 선행 :: (tokenize_other 317 이 reflection-macro 렉서 323 보다 먼저) ====
FE_SYSTEM(::FE::_PostPhysics, ::FEWorldTag::Boot);
void sys11(::FE::world&);

// ==== D12. explicit + 't'로 시작하는 클래스 이름 → 생성자 변형 미검출 (parser.cpp 410 의 space_insensitive_contains, string.hxx 1054 의 재비교 없는 리셋 — 추정) ====
// (현재: task_queue12a 가 등록됨. s/x 등 다른 글자로 시작하는 이름은 정상. Frogman 코드베이스의 task_/thread_/timer_/tile_ 계열 이름이 전부 해당)
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

// ==== D13. 클래스 본문 안의 C++20 `using enum` (tokenizer.cpp 2001 의 starts_with("enum") 이 plain enum 으로 오인) ====
// (현재: "'enum' or 'enum class' is unsupported" 로 파일 스킵. 함수 본문 안의 using enum 은 통과. 엔진 헤더에 using enum 은 0건 — 표준에서 허용하는지는 결정 사항)
// 같은 스윕에서 통과한 C++20: concept, requires 절/식, consteval, constinit, <=> = default, [[likely]]/[[unlikely]], [[no_unique_address]],
//   char8_t/u8, 지정 초기화 { ._x = 1 }, explicit(bool), auto 매개변수(축약 템플릿), 템플릿 람다, constexpr virtual, 초기화문 있는 range-for, co_await/co_return
enum struct kind13 : FE::uint8 { _A, _B };
class use_enum13
{
public:
	using enum kind13;
	kind13 _k = _A;
};

// ==== X1. 클래스 키워드 앞의 속성 매크로 (스타일 밖 — 엔진 헤더에 0건, 면책)  (현재: 뒤 선언 누락 / FE_CLASS 마커 뒤에 오면 "Reached the end of code before reaching to FE_CLASS() marker" 로 파일 스킵)
_FE_NODISCARD_ struct SX1a {};
FE_CLASS();
_FE_NODISCARD_ class CX1b {};
class SentinelX1 {};

// ==== X2. FE_SYSTEM 뒤 시스템 함수의 속성 매크로 (스타일 밖 — 엔진 헤더에 0건, 면책)  (현재: "cannot be qualified as the Frogman Engine ECS system" 로 파일 스킵; 리터럴 [[maybe_unused]] 는 통과)
FE_SYSTEM(FE::_WorldTick, FEWorldTag::Boot);
_FE_MAYBE_UNUSED_ void sysX2(FE::world& world_p) noexcept;

// ==== (구 D6 삭제) struct 에 OOP 키워드(final/virtual/상속)는 Frogman C++ 에서 UB 라 해당 케이스는 제거. 베이스 절은 class 대조군만 유지 (class 는 정상 처리) ====
class Base6 {};
class S6b : public Base6 { public: S6b() = default; };          // 등록돼야 함
class S6d : public Base6 { public: explicit S6d(int) {} };     // 등록 금지
class S6e : public ::Base6 { private: ~S6e() {} };             // 등록 금지 (소멸자 private)

// ==== 14~16. 등록 정책 (현재 등록되지만 생성 코드가 컴파일되지 않음 — 설계 판단이면 이 블록은 삭제) ====
#if 0
class Dead14 {};
#endif
class Sentinel14 {};
class D15 { public: D15() = delete; };
class C16 { public: virtual void g() = 0; };   // 순수가상 class — 등록 금지 (현재는 정상 동작하는 대조군; polymorphic struct 는 Frogman C++ 미지원이라 제외)