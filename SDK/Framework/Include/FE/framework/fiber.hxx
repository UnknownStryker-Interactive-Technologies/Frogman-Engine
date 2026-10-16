#ifndef _FE_FRAMEWORK_FIBER_HXX_
#define _FE_FRAMEWORK_FIBER_HXX_
#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to alignment specifier
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/memory.hxx>
#include <FE/fqueue.hxx>
#include <FE/framework/smart_ptr.hxx>

#include <atomic>
#include <memory_resource>

#include <absl/container/flat_hash_map.h>

#include <concurrent_priority_queue.h>




extern "C"
{
	struct alignas(FE::SIMD_auto_alignment::size) SIMD
	{
		var::uint64 _low[FE::SIMD_auto_alignment::size / 16];
		var::uint64 _high[FE::SIMD_auto_alignment::size / 16];
	};


	struct alignas(4) mxcsr
	{
		bool _invalid_op_flag : 1;		// bit 0
		bool _denormal_flag : 1;		// bit 1
		bool _divide_by_zero_flag : 1;	// bit 2
		bool _overflow_flag : 1;		// bit 3
		bool _underflow_flag : 1;		// bit 4
		bool _precision_flag : 1;		// bit 5

		bool _denormals_are_zero : 1;	// bit 6

		bool _invalid_op_mask : 1;		// bit 7
		bool _denormal_operand_mask : 1;// bit 8
		bool _divide_by_zero_mask : 1;	// bit 9
		bool _overflow_mask : 1;		// bit 10
		bool _underflow_mask : 1;		// bit 11
		bool _precision_mask : 1;		// bit 12

		unsigned char _rounding_control : 2;	// bits 13-14

		bool _flush_to_zero : 1;		// bit 15

		// The remaining bits (16-31) are reserved and should be set to 0.
	};


	struct alignas(4) fpcw
	{
		bool _invalid_op_mask : 1;		// bit 0
		bool _denormal_operand_mask : 1;// bit 1
		bool _divide_by_zero_mask : 1;	// bit 2
		bool _overflow_mask : 1;		// bit 3
		bool _underflow_mask : 1;		// bit 4
		bool _precision_mask : 1;		// bit 5

		unsigned char _reserved : 2;	// bits 6-7

		unsigned char _precision_control : 2;	// bits 8-9

		unsigned char _rounding_control : 2;	// bits 10-11

		bool _infinity_control : 1;	// bit 12
	};


	/*
		Vector registers with the same index number share their storage. e.g. zmm6, ymm6, and xmm6
		GPRs with the same index number share their storage. e.g. rcx and ecx

		For example, writing data to zmm6 overwrites the data stored in xmm6.
		When context-switching a fiber, all non-volatile registers have to be preserved throughout the context-switches.

	[FROGMAN ENGINE AMD64 ABI SPECIFICATION]
		Unlike the Windows x64 ABI which treats the upper bits of vector registers as volatile
		the Frogman Engine AMD64 ABI explicitly preserves the full widths of SIMD registers 6 through 15.
		Depending on FE::SIMD_auto_alignment::size, this guarantees the preservation of full
		YMM (256-bit) or ZMM (512-bit) state across fiber context switches, allowing safe yields
		during heavy vectorized workloads.
	*/
	struct alignas(sizeof(SIMD)) fiber_context
	{
		SIMD _simd6, _simd7, _simd8, _simd9, _simd10, _simd11, _simd12, _simd13, _simd14, _simd15;

		var::uint64 _r12, _r13, _r14, _r15;

		mxcsr _mxcsr;
		fpcw _fpcw;

		var::uint64 _rbx;
		var::uint64 _rbp;
		var::uint64 _rdi;
		var::uint64 _rsi;
		var::byte* _rsp; // goes downwards
	};


	enum TaskPriority : var::uint32
	{
		_Critical = 0,
		_High = 1,
		_AboveNormal = 2,
		_Normal = 3,
		_BelowNormal = 4,
		_Low = 5,
		_Background = 6,
		_None = 7
	};


	struct fiber_impl
	{
		fiber_context* _context_ptr;
		void* _absolute_begin_of_stack;
		void* _stack_base;
		void* _stack_limit; // the stack top; below this is a guarded page.

		// engine specific properties
		var::int32 _host_thread_id;
		TaskPriority _task_type;
	};
}




namespace FE
{
	// temp decl
	class world;

	using impl = void(*)(void*);
	namespace internal
	{
		struct vertex
		{
			void* _argument;
			var::int64 _reference_count;
			std::pmr::vector<impl> _runs_before;
		};
	}

	struct task
	{
		impl _system;
		void* _argument;
		TaskPriority _task_type;
	};

	using task_map = absl::flat_hash_map<	impl, internal::vertex,
		absl::DefaultHashContainerHash<impl>,
		absl::DefaultHashContainerEq<impl>,
		std::pmr::polymorphic_allocator< std::pair<const impl, internal::vertex> >
	>;

	class processors;

	class fiber_flow
	{
	public:
		class auth
		{
			friend class processors;
		};

		class task
		{
			FE::task m_task;
			FE::smart_ptr< task_map, RefType::_Observer> m_tasks;

		public:
			task(const FE::task& task_p, FE::smart_ptr< task_map, RefType::_Observer> tasks_p) noexcept;
			~task() noexcept = default;

			task(const task& other_p) noexcept;
			task& operator=(const task& other_p) noexcept;

			task(task&& other_p) noexcept;
			task& operator=(task&& other_p) noexcept;

		public:
			void runs_before(const FE::fiber_flow::task& other_p) noexcept;
			void runs_after(const FE::fiber_flow::task& other_p) noexcept;
		};

	private:
		mutable std::pmr::vector<FE::task> m_topologically_ordered_tasks;
		mutable FE::smart_ptr< task_map, RefType::_Owner> m_tasks;
		TaskPriority m_task_type;

	public:
		fiber_flow(TaskPriority task_type_p = TaskPriority::_None) noexcept;
		~fiber_flow() noexcept = default;

		fiber_flow(fiber_flow&& other_p) noexcept;
		fiber_flow& operator=(fiber_flow&& other_p) noexcept;

		fiber_flow(const fiber_flow& other_p) noexcept;
		fiber_flow& operator=(const fiber_flow& other_p) noexcept;

	public:
		_FE_FORCE_INLINE_ void set_task_type(TaskPriority task_type_p) noexcept { m_task_type = task_type_p; }
		_FE_FORCE_INLINE_ TaskPriority get_task_type() const noexcept { return m_task_type; }
		typename FE::fiber_flow::task emplace(const FE::task& task_p) noexcept;

		void sort_tasks_topologically(auth) const noexcept;
		std::pmr::vector<FE::task>& get_topologically_ordered_tasks(auth) const noexcept;
	};

	class priority_comparator
	{
	public:
		inline bool operator()(const task& lhs_p, const task& rhs_p) noexcept
		{
			return static_cast<var::int32>(lhs_p._task_type) > static_cast<var::int32>(rhs_p._task_type);
		}

		inline bool operator()(const std::pmr::vector<FE::task>& lhs_p, const std::pmr::vector<FE::task>& rhs_p) noexcept
		{
			return static_cast<var::int32>(lhs_p.front()._task_type) > static_cast<var::int32>(rhs_p.front()._task_type);
		}
	};

	class thread_context
	{
	public:
		fiber_impl _thread_context;

	private:
		alignas(sizeof(SIMD)) var::byte m_page[FE::one_KiB * 8];

	public:
		thread_context() noexcept;
		~thread_context() noexcept = default;

		thread_context(thread_context&&) noexcept = delete;
		thread_context& operator=(thread_context&&) noexcept = delete;

		thread_context(const thread_context&) = delete;
		thread_context& operator=(const thread_context&) = delete;
	};


	// fiber cannot be thread_local static!
	class fiber final
	{
		friend class fiber_scheduler;

		fiber_impl* m_impl;

	public:
		_FE_CDECL_ fiber() noexcept = default;
		_FE_CDECL_ fiber(size_t stack_size_p) noexcept;
		_FE_CDECL_ ~fiber() noexcept;

		_FE_CDECL_ fiber(fiber&& other_p) noexcept;
		fiber& _FE_CDECL_ operator=(fiber&& other_p) noexcept;

		inline var::int32 _FE_CDECL_ get_host_thread_id() const noexcept { return m_impl->_host_thread_id; }

		fiber(const fiber&) noexcept = delete;
		fiber& operator=(const fiber&) noexcept = delete;

	private:
		thread_local static std::pmr::monotonic_buffer_resource tl_s_fiber_pool;
	};


	class fiber_scheduler final
	{
		FE::fqueue<FE::fiber, 8> m_fiber_pool;
		std::array<FE::fqueue<FE::fiber, 8>, 8> m_active_fibers; // The size of TaskPriority slots == 7
		var::size m_fibers;
		std::atomic_bool m_is_locked;
		concurrency::concurrent_priority_queue<task, priority_comparator, std::pmr::polymorphic_allocator<task>> m_task_queue;
		concurrency::concurrent_priority_queue<std::pmr::vector<FE::task>, priority_comparator, std::pmr::polymorphic_allocator<std::pmr::vector<FE::task>>> m_fiber_flow_queue;
		std::pmr::vector<FE::task> m_topologically_ordered_tasks;
		std::pmr::vector<FE::task>::iterator m_ordered_tasks_iterator;

		std::unique_ptr<thread_context> m_thread_context;

	public:
		fiber_scheduler() noexcept;
		~fiber_scheduler() noexcept = default;

		// not thread-safe.
		void _FE_CDECL_ create_fiber(FE::size stack_size_p = FE::system_large_page_size) noexcept;

		void _FE_CDECL_ schedule_task(const task& task_p) noexcept;
		void _FE_CDECL_ schedule_tasks(std::pmr::vector<FE::task>&& tasks_p) noexcept;

		// not thread-safe.
		int _FE_CDECL_ execute() noexcept;

		// calling it from other thread is an undefined behavior. The fiber context switch must be called from the same thread that is executing the fiber scheduler.
		void _FE_CDECL_ switch_fiber_context() noexcept;

		void attach_to_current_thread() noexcept;
		void detach_from_current_thread() noexcept;

		void sigunlock() noexcept;
		void siglock() noexcept;

		static void _FE_CDECL_ yield_back_if_not_locked() noexcept;
		static void _FE_CDECL_ yield() noexcept;


		static bool is_fiber() noexcept;
		static FE::fiber_scheduler* get_current_fiber_scheduler() noexcept;

	private:
		thread_local static fiber_impl* tl_s_current_fiber;
		thread_local static FE::fiber_scheduler* tl_s_this_thread_fiber_scheduler;

	public:
		fiber_scheduler(const fiber_scheduler&) = delete;
		fiber_scheduler& operator=(const fiber_scheduler&) = delete;
	};
}
#pragma warning(pop)
#endif