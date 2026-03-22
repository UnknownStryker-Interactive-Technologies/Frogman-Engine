; Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.
; Licensed under the Frogman Engine Apache License (the "License"); you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
; 	https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md
; Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and limitations under the License.




; ┌─────────────┬────────┬────────┬──────────────────────────────────────────┐
; │  Directive  │  Alias │  Size  │  Description                             │
; ├─────────────┼────────┼────────┼──────────────────────────────────────────┤
; │  BYTE       │  DB    │  1 B   │  Unsigned 8-bit integer                  │
; │  SBYTE      │        │  1 B   │  Signed 8-bit integer                    │
; ├─────────────┼────────┼────────┼──────────────────────────────────────────┤
; │  WORD       │  DW    │  2 B   │  Unsigned 16-bit integer                 │
; │  SWORD      │        │  2 B   │  Signed 16-bit integer                   │
; ├─────────────┼────────┼────────┼──────────────────────────────────────────┤
; │  DWORD      │  DD    │  4 B   │  Unsigned 32-bit integer                 │
; │  SDWORD     │        │  4 B   │  Signed 32-bit integer                   │
; ├─────────────┼────────┼────────┼──────────────────────────────────────────┤
; │  QWORD      │  DQ    │  8 B   │  Unsigned 64-bit integer / pointer       │
; │  SQWORD     │        │  8 B   │  Signed 64-bit integer                   │
; ├─────────────┼────────┼────────┼──────────────────────────────────────────┤
; │  OWORD      │        │  16 B  │  128-bit integer (SSE / XMM operand)     │
; ├─────────────┼────────┼────────┼──────────────────────────────────────────┤
; │  REAL4      │        │  4 B   │  IEEE 754 single-precision float         │
; │  REAL8      │        │  8 B   │  IEEE 754 double-precision float         │
; │  REAL10     │        │  10 B  │  IEEE 754 extended-precision (x87 only)  │
; ├─────────────┼────────┼────────┼──────────────────────────────────────────┤
; │  TBYTE      │  DT    │  10 B  │  10-byte packed BCD / x87 real           │
; ├─────────────┼────────┼────────┼──────────────────────────────────────────┤
; │  XMMWORD    │        │  16 B  │  128-bit SSE type alias (storage hint)   │
; │  YMMWORD    │        │  32 B  │  256-bit AVX type alias (storage hint)   │
; │  ZMMWORD    │        │  64 B  │  512-bit AVX-512 type alias              │
; └─────────────┴────────┴────────┴──────────────────────────────────────────┘
;
; Notes
; ─────
; - DB / DW / DD / DQ are the classic Intel aliases; MASM accepts both forms.
; - SQWORD is rarely seen in practice; QWORD with sign-extending moves is more common.
; - REAL10 / TBYTE are x87-only; the x64 ABI does not pass them in registers.
; - XMMWORD / YMMWORD / ZMMWORD are assembler type hints for PTR casts, e.g.:
;     movdqu  xmm0, XMMWORD PTR [rax]
;   They do not reserve storage on their own — use OWORD or a BYTE array instead.
; - Use QWORD for all 64-bit pointers (RIP-relative addressing, LEA targets, etc.)




; ┌─────────────────────────────────────────────────────────────────────────────────────┐
; │                     x64 Windows ABI — Register Volatility                           │
; ├──────────────┬────────────┬─────────────┬──────────────────────────────────────────┤
; │  Register    │  Width     │  Volatility │  Notes                                   │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │              │            │             │  GENERAL-PURPOSE REGISTERS               │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │  RAX         │  64-bit    │  Volatile   │  Return value                            │
; │  RCX         │  64-bit    │  Volatile   │  1st integer argument                    │
; │  RDX         │  64-bit    │  Volatile   │  2nd integer argument                    │
; │  R8          │  64-bit    │  Volatile   │  3rd integer argument                    │
; │  R9          │  64-bit    │  Volatile   │  4th integer argument                    │
; │  R10         │  64-bit    │  Volatile   │  Scratch                                 │
; │  R11         │  64-bit    │  Volatile   │  Scratch                                 │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │  RBX         │  64-bit    │  Nonvolatile│  Callee must preserve                    │
; │  RBP         │  64-bit    │  Nonvolatile│  Frame pointer (optional use)            │
; │  RDI         │  64-bit    │  Nonvolatile│  Callee must preserve                    │
; │  RSI         │  64-bit    │  Nonvolatile│  Callee must preserve                    │
; │  R12         │  64-bit    │  Nonvolatile│  Callee must preserve                    │
; │  R13         │  64-bit    │  Nonvolatile│  Callee must preserve                    │
; │  R14         │  64-bit    │  Nonvolatile│  Callee must preserve                    │
; │  R15         │  64-bit    │  Nonvolatile│  Callee must preserve                    │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │  RSP         │  64-bit    │  Nonvolatile│  Stack pointer — must be restored        │
; │  RIP         │  64-bit    │  N/A        │  Instruction pointer — not accessible    │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │              │            │             │  XMM / YMM / ZMM REGISTERS              │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │  XMM0        │  128-bit   │  Volatile   │  1st float argument, float return value  │
; │  XMM1        │  128-bit   │  Volatile   │  2nd float argument                      │
; │  XMM2        │  128-bit   │  Volatile   │  3rd float argument                      │
; │  XMM3        │  128-bit   │  Volatile   │  4th float argument                      │
; │  XMM4        │  128-bit   │  Volatile   │  Scratch                                 │
; │  XMM5        │  128-bit   │  Volatile   │  Scratch                                 │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │  XMM6        │  128-bit   │  Nonvolatile│  Low 128 bits must be preserved          │
; │  XMM7        │  128-bit   │  Nonvolatile│  Low 128 bits must be preserved          │
; │  XMM8        │  128-bit   │  Nonvolatile│  Low 128 bits must be preserved          │
; │  XMM9        │  128-bit   │  Nonvolatile│  Low 128 bits must be preserved          │
; │  XMM10       │  128-bit   │  Nonvolatile│  Low 128 bits must be preserved          │
; │  XMM11       │  128-bit   │  Nonvolatile│  Low 128 bits must be preserved          │
; │  XMM12       │  128-bit   │  Nonvolatile│  Low 128 bits must be preserved          │
; │  XMM13       │  128-bit   │  Nonvolatile│  Low 128 bits must be preserved          │
; │  XMM14       │  128-bit   │  Nonvolatile│  Low 128 bits must be preserved          │
; │  XMM15       │  128-bit   │  Nonvolatile│  Low 128 bits must be preserved          │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │  YMM0–YMM5   │  256-bit   │  Volatile   │  Upper 128 bits (bits 255:128) volatile  │
; │  YMM6–YMM15  │  256-bit   │  Volatile*  │  Upper 128 bits volatile even for XMM6+  │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │  ZMM0–ZMM5   │  512-bit   │  Volatile   │  Entirely volatile                       │
; │  ZMM6–ZMM15  │  512-bit   │  Volatile*  │  Bits 511:128 volatile; low 128 saved    │
; │  ZMM16–ZMM31 │  512-bit   │  Volatile   │  Entirely volatile (AVX-512 only)        │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │              │            │             │  SEGMENT / FLAG / OTHER REGISTERS        │
; ├──────────────┼────────────┼─────────────┼──────────────────────────────────────────┤
; │  RFLAGS      │  64-bit    │  Volatile   │  DF must be 0 on call and return         │
; │  CS/SS/DS/ES │  16-bit    │  Volatile   │  Flat model — not used for addressing    │
; │  FS          │  16-bit    │  Nonvolatile│  TEB base (Windows thread-local storage) │
; │  GS          │  16-bit    │  Nonvolatile│  Reserved by OS — do not modify          │
; │  ST0–ST7     │  80-bit    │  Volatile   │  x87 stack — must be empty on return     │
; │  MM0–MM7     │  64-bit    │  Volatile   │  Aliased to x87 ST registers             │
; │  K0–K7       │  64-bit    │  Volatile   │  AVX-512 opmask registers                │
; └──────────────┴────────────┴─────────────┴──────────────────────────────────────────┘
;
; Key rules
; ─────────
; Volatile     — caller must save before CALL if the value is still needed after.
; Nonvolatile  — callee must PUSH/save on entry and POP/restore before RET.
;
; YMM/ZMM upper-bits rule:
;   The ABI only protects the low 128 bits of XMM6–XMM15.
;   Any use of the YMM or ZMM view of those registers makes the
;   upper bits volatile — callee must VZEROUPPER or save/restore
;   the full width explicitly if the caller relies on them.
;
; x87 / MMX:
;   The x87 stack must be empty (all tags = empty) when a function
;   returns. Failure to do so corrupts the caller's FP state.
;   EMMS / FEMMS clear the MMX state when mixing MMX and x87 code.
;
; RFLAGS — direction flag (DF):
;   Must be cleared (CLD) on both call and return. MASM's
;   REP MOVS* / REP STOS* rely on DF=0; violating this is silent
;   data corruption.




; ┌────────┬──────────────────────┬──────────────────────┬────────────────────────────────┐
; │  Slot  │   Integer / pointer  │    Float / double    │   Variadic (both slots used)   │
; ├────────┼──────────────────────┼──────────────────────┼────────────────────────────────┤
; │  1st   │  RCX                 │  XMM0                │  XMM0,    RCX                  │
; │  2nd   │  RDX                 │  XMM1                │  XMM,     RDX                  │
; │  3rd   │  R8                  │  XMM2                │  XMM2,    R8                   │
; │  4th   │  R9                  │  XMM3                │  XMM3,    R9                   │
; ├────────┴──────────────────────┴──────────────────────┴────────────────────────────────┤
; │  5th+  Stack  —  right-to-left, above the 32-byte shadow space                        │
; └───────────────────────────────────────────────────────────────────────────────────────┘
; Shadow space: caller always reserves 32 bytes (4 × 8) above the return address,
;               even if the callee takes zero arguments.




; The Windows AMD64 ABI Shadow Space
; Higher addresses
; ┌──────────────────────────┐
; │  frame of caller         │
; ├──────────────────────────┤
; │  shadow[3]  (r9  home)   │  [rsp+32]
; │  shadow[2]  (r8  home)   │  [rsp+24]
; │  shadow[1]  (rdx home)   │  [rsp+16]
; │  shadow[0]  (rcx home)   │  [rsp+8]   
; ├──────────────────────────┤
; │  return address          │  [rsp]
; └──────────────────────────┘
; Lower addresses




.code
	PUBLIC __switch_to_the_fiber
		__switch_to_the_fiber PROC
		    ; RCX = from_p
			; RDX = to_p
			; R8  = simd_vector_size_p

			mov rax, r8 ; Move simd_vector_size_p to rax
			shr rax, 5 ; Divide simd_vector_size_p by 32 (size of YMM register) to get the case index; result in rax (the case index)

			; Save the SIMD Vector Registers
			lea r10, fiber_context_preservation_switch_case_table ; Load the address of the case table into r10
			mov r11, QWORD PTR [rcx] ; r11 is a pointer to the fiber_context field variable of the fiber* from_p
			jmp QWORD PTR [r10 + rax*8] ; Equivalent as byte array[]; array[rax*8];
			fiber_context_preservation_switch_case_end:

			; Save the non-volatile general-purpose registers
			imul r10, r8, 10 ; r10 = simd_vector_size_p * 10
			mov QWORD PTR [r11 + r10], r12

			add r10, 8
			mov QWORD PTR [r11 + r10], r13

			add r10, 8
			mov QWORD PTR [r11 + r10], r14

			add r10, 8
			mov QWORD PTR [r11 + r10], r15


			add r10, 8
			stmxcsr DWORD PTR [r11 + r10]

			add r10, 4
			fnstcw	WORD PTR [r11 + r10]


			add r10, 4
			mov QWORD PTR [r11 + r10], rbx

			add r10, 8
			mov QWORD PTR [r11 + r10], rbp

			add r10, 8
			mov QWORD PTR [r11 + r10], rdi

			add r10, 8
			mov QWORD PTR [r11 + r10], rsi

			add r10, 8
			mov QWORD PTR [r11 + r10], rsp




			lea r10, fiber_context_restoration_switch_case_table ; Load the address of the case table into r10
			mov r11, QWORD PTR [rdx] ; r11 is a pointer to the fiber_context field variable of the fiber* from_p
			jmp QWORD PTR [r10 + rax*8] ; Equivalent as byte array[]; array[rax*8];
			fiber_context_restoration_switch_case_end:

			; Restore the non-volatile general-purpose registers
			imul r10, r8, 10
			mov r12, QWORD PTR [r11 + r10]

			add r10, 8
			mov r13, QWORD PTR [r11 + r10]

			add r10, 8
			mov r14, QWORD PTR [r11 + r10]

			add r10, 8
			mov r15, QWORD PTR [r11 + r10]


			add r10, 8
			ldmxcsr DWORD PTR [r11 + r10]

			add r10, 4
			fldcw	WORD PTR [r11 + r10]


			add r10, 4
			mov rbx, QWORD PTR [r11 + r10]

			add r10, 8
			mov rbp, QWORD PTR [r11 + r10]

			add r10, 8
			mov rdi, QWORD PTR [r11 + r10]

			add r10, 8
			mov rsi, QWORD PTR [r11 + r10]

			add r10, 8
			mov rsp, QWORD PTR [r11 + r10]
			

			; Restore the GS segment register values (TEB)
			mov r10, [rdx + 8] ; _absolute_begin_of_stack
			mov gs:[1478h], r10

			mov r10, [rdx + 16] ; _stack_base
			mov gs:[08h], r10

			mov r10, [rdx + 24] ; _stack_limit
			mov gs:[10h], r10

			ret




			fiber_context_preservation_switch_case16::
				movapd XMMWORD PTR [r11], xmm6
				movapd XMMWORD PTR [r11 + r8], xmm7

				imul r10, r8, 2
				movapd XMMWORD PTR [r11 + r10], xmm8

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm9

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm10

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm11

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm12

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm13

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm14

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm15
				jmp fiber_context_preservation_switch_case_end

			fiber_context_preservation_switch_case32::
				vmovapd YMMWORD PTR [r11], ymm6
				vmovapd YMMWORD PTR [r11 + r8], ymm7

				imul r10, r8, 2
				vmovapd YMMWORD PTR [r11 + r10], ymm8

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm9

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm10

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm11

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm12

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm13

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm14

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm15
				jmp fiber_context_preservation_switch_case_end

			fiber_context_preservation_switch_case64::
				vmovapd ZMMWORD PTR [r11], zmm6
				vmovapd ZMMWORD PTR [r11 + r8], zmm7

				imul r10, r8, 2
				vmovapd ZMMWORD PTR [r11 + r10], zmm8

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm9

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm10

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm11

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm12

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm13

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm14

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm15
				jmp fiber_context_preservation_switch_case_end




			fiber_context_restoration_switch_case16::
				movapd xmm6, XMMWORD PTR [r11]
				movapd xmm7, XMMWORD PTR [r11 + r8]

				imul r10, r8, 2
				movapd xmm8, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm9, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm10, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm11, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm12, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm13, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm14, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm15, XMMWORD PTR [r11 + r10]
				jmp fiber_context_restoration_switch_case_end

			fiber_context_restoration_switch_case32::
				vmovapd ymm6, YMMWORD PTR [r11]
				vmovapd ymm7, YMMWORD PTR [r11 + r8]

				imul r10, r8, 2
				vmovapd ymm8, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm9, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm10, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm11, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm12, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm13, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm14, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm15, YMMWORD PTR [r11 + r10]
				jmp fiber_context_restoration_switch_case_end

			fiber_context_restoration_switch_case64::
				vmovapd zmm6, ZMMWORD PTR [r11]
				vmovapd zmm7, ZMMWORD PTR [r11 + r8]

				imul r10, r8, 2
				vmovapd zmm8, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm9, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm10, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm11, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm12, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm13, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm14, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm15, ZMMWORD PTR [r11 + r10]
				jmp fiber_context_restoration_switch_case_end
		__switch_to_the_fiber ENDP

	.const
		fiber_context_preservation_switch_case_table QWORD	OFFSET fiber_context_preservation_switch_case16,
															OFFSET fiber_context_preservation_switch_case32,
															OFFSET fiber_context_preservation_switch_case64

		fiber_context_restoration_switch_case_table QWORD	OFFSET fiber_context_restoration_switch_case16,
															OFFSET fiber_context_restoration_switch_case32,
															OFFSET fiber_context_restoration_switch_case64








.code
	PUBLIC __switch_to_new_fiber
		__switch_to_new_fiber PROC
		    ; RCX = from_p
			; RDX = to_p
			; R8  = simd_vector_size_p
			; R9 = void(FE::fiber_scheduler::* to_return_to_p)()

			mov rax, r8 ; Move simd_vector_size_p to rax
			shr rax, 5 ; Divide simd_vector_size_p by 32 (size of YMM register) to get the case index; result in rax (the case index)

			; Save the SIMD Vector Registers
			lea r10, switch_to_new_fiber_context_preservation_switch_case_table ; Load the address of the case table into r10
			mov r11, QWORD PTR [rcx] ; r11 is a pointer to the fiber_context field variable of the fiber* from_p
			jmp QWORD PTR [r10 + rax*8] ; Equivalent as byte array[]; array[rax*8];
			switch_to_new_fiber_context_preservation_switch_case_end:

			; Save the non-volatile general-purpose registers
			imul r10, r8, 10 ; r10 = simd_vector_size_p * 10
			mov QWORD PTR [r11 + r10], r12

			add r10, 8
			mov QWORD PTR [r11 + r10], r13

			add r10, 8
			mov QWORD PTR [r11 + r10], r14

			add r10, 8
			mov QWORD PTR [r11 + r10], r15


			add r10, 8
			stmxcsr DWORD PTR [r11 + r10]

			add r10, 4
			fnstcw	WORD PTR [r11 + r10]


			add r10, 4
			mov QWORD PTR [r11 + r10], rbx

			add r10, 8
			mov QWORD PTR [r11 + r10], rbp

			add r10, 8
			mov QWORD PTR [r11 + r10], rdi

			add r10, 8
			mov QWORD PTR [r11 + r10], rsi

			add r10, 8
			mov QWORD PTR [r11 + r10], rsp




			lea r10, switch_to_new_fiber_context_restoration_switch_case_table ; Load the address of the case table into r10
			mov r11, QWORD PTR [rdx] ; r11 is a pointer to the fiber_context field variable of the fiber* from_p
			jmp QWORD PTR [r10 + rax*8] ; Equivalent as byte array[]; array[rax*8];
			switch_to_new_fiber_context_restoration_switch_case_end:

			; Restore the non-volatile general-purpose registers
			imul r10, r8, 10
			mov r12, QWORD PTR [r11 + r10]

			add r10, 8
			mov r13, QWORD PTR [r11 + r10]

			add r10, 8
			mov r14, QWORD PTR [r11 + r10]

			add r10, 8
			mov r15, QWORD PTR [r11 + r10]


			add r10, 8
			ldmxcsr DWORD PTR [r11 + r10]

			add r10, 4
			fldcw	WORD PTR [r11 + r10]


			add r10, 4
			mov rbx, QWORD PTR [r11 + r10]

			add r10, 8
			mov rbp, QWORD PTR [r11 + r10]

			add r10, 8
			mov rdi, QWORD PTR [r11 + r10]

			add r10, 8
			mov rsi, QWORD PTR [r11 + r10]

			add r10, 8
			mov rsp, QWORD PTR [r11 + r10]
			

			; Restore the GS segment register values (TEB)
			mov r10, [rdx + 8] ; _absolute_begin_of_stack
			mov gs:[1478h], r10

			mov r10, [rdx + 16] ; _stack_base
			mov gs:[08h], r10

			mov r10, [rdx + 24] ; _stack_limit
			mov gs:[10h], r10


			push r9 ; plant the to_return_to_p pointer on the stack as the return address
			
			push rdx ; preserve to_p
			push r14 ; plant the this pointer on the stack as the first argument


			sub rsp, 40 ; Shadow space for the CALL instruction below
			mov rcx, r13 ; l_to_switch.m_impl->_context_ptr->_r13 = (var::uint64)l_to_execute._component;
			call r12
			add rsp, 40 ; Clean up shadow space after the CALL


			pop r14 ; restore the this pointer from the stack after the CALL instruction
			mov rcx, r14 ; mov this to rcx

			pop rdx
			mov DWORD PTR [rdx + 36], 7 ; Mark the fiber task priority as _None (0xFF)

			ret




			switch_to_new_fiber_context_preservation_switch_case16::
				movapd XMMWORD PTR [r11], xmm6
				movapd XMMWORD PTR [r11 + r8], xmm7

				imul r10, r8, 2
				movapd XMMWORD PTR [r11 + r10], xmm8

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm9

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm10

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm11

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm12

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm13

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm14

				add r10, r8
				movapd XMMWORD PTR [r11 + r10], xmm15
				jmp switch_to_new_fiber_context_preservation_switch_case_end

			switch_to_new_fiber_context_preservation_switch_case32::
				vmovapd YMMWORD PTR [r11], ymm6
				vmovapd YMMWORD PTR [r11 + r8], ymm7

				imul r10, r8, 2
				vmovapd YMMWORD PTR [r11 + r10], ymm8

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm9

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm10

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm11

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm12

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm13

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm14

				add r10, r8
				vmovapd YMMWORD PTR [r11 + r10], ymm15
				jmp switch_to_new_fiber_context_preservation_switch_case_end

			switch_to_new_fiber_context_preservation_switch_case64::
				vmovapd ZMMWORD PTR [r11], zmm6
				vmovapd ZMMWORD PTR [r11 + r8], zmm7

				imul r10, r8, 2
				vmovapd ZMMWORD PTR [r11 + r10], zmm8

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm9

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm10

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm11

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm12

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm13

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm14

				add r10, r8
				vmovapd ZMMWORD PTR [r11 + r10], zmm15
				jmp switch_to_new_fiber_context_preservation_switch_case_end




			switch_to_new_fiber_context_restoration_switch_case16::
				movapd xmm6, XMMWORD PTR [r11]
				movapd xmm7, XMMWORD PTR [r11 + r8]

				imul r10, r8, 2
				movapd xmm8, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm9, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm10, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm11, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm12, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm13, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm14, XMMWORD PTR [r11 + r10]

				add r10, r8
				movapd xmm15, XMMWORD PTR [r11 + r10]
				jmp switch_to_new_fiber_context_restoration_switch_case_end

			switch_to_new_fiber_context_restoration_switch_case32::
				vmovapd ymm6, YMMWORD PTR [r11]
				vmovapd ymm7, YMMWORD PTR [r11 + r8]

				imul r10, r8, 2
				vmovapd ymm8, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm9, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm10, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm11, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm12, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm13, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm14, YMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd ymm15, YMMWORD PTR [r11 + r10]
				jmp switch_to_new_fiber_context_restoration_switch_case_end

			switch_to_new_fiber_context_restoration_switch_case64::
				vmovapd zmm6, ZMMWORD PTR [r11]
				vmovapd zmm7, ZMMWORD PTR [r11 + r8]

				imul r10, r8, 2
				vmovapd zmm8, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm9, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm10, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm11, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm12, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm13, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm14, ZMMWORD PTR [r11 + r10]

				add r10, r8
				vmovapd zmm15, ZMMWORD PTR [r11 + r10]
				jmp switch_to_new_fiber_context_restoration_switch_case_end
		__switch_to_new_fiber ENDP

	.const
		switch_to_new_fiber_context_preservation_switch_case_table QWORD	OFFSET switch_to_new_fiber_context_preservation_switch_case16,
																			OFFSET switch_to_new_fiber_context_preservation_switch_case32,
																			OFFSET switch_to_new_fiber_context_preservation_switch_case64

		switch_to_new_fiber_context_restoration_switch_case_table QWORD		OFFSET switch_to_new_fiber_context_restoration_switch_case16,
																			OFFSET switch_to_new_fiber_context_restoration_switch_case32,
																			OFFSET switch_to_new_fiber_context_restoration_switch_case64








.code
	PUBLIC __fork_fiber
		__fork_fiber PROC
		    ; RCX = out_thread_p
			; RDX = fiber_p
			; R8  = simd_vector_size_p
			; R9 = void(FE::fiber_scheduler::* to_return_to_p)()

			mov rax, r8 ; Move simd_vector_size_p to rax
			shr rax, 5 ; Divide simd_vector_size_p by 32 (size of YMM register) to get the case index; result in rax (the case index)

			; Save the SIMD Vector Registers
			lea r10, fork_fiber_context_preservation_switch_case_table ; Load the address of the case table into r10
			mov r11, QWORD PTR [rcx] ; r11 is a pointer to the fiber_context field variable of the fiber* out_thread_p
			jmp QWORD PTR [r10 + rax*8] ; Equivalent as byte array[]; array[rax*8];
			fork_fiber_context_preservation_switch_case_end:

			; Save the non-volatile general-purpose registers
			imul r10, r8, 10
			mov QWORD PTR [r11 + r10], r12

			add r10, 8
			mov QWORD PTR [r11 + r10], r13

			add r10, 8
			mov QWORD PTR [r11 + r10], r14

			add r10, 8
			mov QWORD PTR [r11 + r10], r15


			add r10, 8
			stmxcsr DWORD PTR [r11 + r10]

			add r10, 4
			fnstcw	WORD PTR [r11 + r10]


			add r10, 4
			mov QWORD PTR [r11 + r10], rbx

			add r10, 8
			mov QWORD PTR [r11 + r10], rbp

			add r10, 8
			mov QWORD PTR [r11 + r10], rdi

			add r10, 8
			mov QWORD PTR [r11 + r10], rsi

			add r10, 8
			mov QWORD PTR [r11 + r10], rsp

			; Save the GS segment register value (TEB)
			mov r10, gs:[1478h]
			mov [rcx + 8], r10 ; _absolute_begin_of_stack

			mov r10, gs:[08h] ; _stack_base
			mov [rcx + 16], r10

			mov r10, gs:[10h]
			mov [rcx + 24], r10 ; _stack_limit




			lea r10, fork_fiber_context_restoration_switch_case_table ; Load the address of the case table into r10
			mov r11, QWORD PTR [rdx] ; r11 is a pointer to the fiber_context field variable of the fiber* from_p
			jmp QWORD PTR [r10 + rax*8] ; Equivalent as byte array[]; array[rax*8];
			fork_fiber_context_restoration_switch_case_end:

			; Restore the non-volatile general-purpose registers
			imul r10, r8, 10
			mov r12, QWORD PTR [r11 + r10]

			add r10, 8
			mov r13, QWORD PTR [r11 + r10]

			add r10, 8
			mov r14, QWORD PTR [r11 + r10]

			add r10, 8
			mov r15, QWORD PTR [r11 + r10]


			add r10, 8
			stmxcsr DWORD PTR [r11 + r10]

			add r10, 4
			fnstcw	WORD PTR [r11 + r10]


			add r10, 4
			mov rbx, QWORD PTR [r11 + r10]

			add r10, 8
			mov rbp, QWORD PTR [r11 + r10]

			add r10, 8
			mov rdi, QWORD PTR [r11 + r10]

			add r10, 8
			mov rsi, QWORD PTR [r11 + r10]

			mov rax, [rsp]; Save the return address on the stack before overwriting rsp

			add r10, 8
			mov rsp, QWORD PTR [r11 + r10]

			push rax ; plant the return address back to the stack after restoring rsp


			

			; Restore the GS segment register values (TEB)
			mov r10, [rdx + 8] ; _absolute_begin_of_stack
			mov gs:[1478h], r10

			mov r10, [rdx + 16] ; _stack_base
			mov gs:[08h], r10

			mov r10, [rdx + 24] ; _stack_limit
			mov gs:[10h], r10


			push r9 ; plant the to_return_to_p pointer on the stack
			push rdx ; preserve fiber_p
			push r14 ; plant the this pointer on the stack


			sub rsp, 40 ; make room for return address and shadow space
			mov rcx, r13 ; smuggle its argument pointer too... HAHA!
			call r12 ; r12 has the function pointer to jump to, which is the start of the fiber execution.
			add rsp, 40 ; Clean up shadow space after the CALL


			pop r14 ; restore the this pointer from the stack after the CALL instruction
			mov rcx, r14 ; mov this to rcx

			pop rdx
			mov DWORD PTR [rdx + 36], 7 ; Mark the fiber task priority as _None (0xFF)

			pop r9 ; 
			sub rsp, 40 ; make room for return address and shadow space
			call r9 ; call FE::fiber_scheduler::switch_fiber_context();
			add rsp, 40 ; Clean up shadow space after the CALL

			ret




		fork_fiber_context_preservation_switch_case16::
			movapd XMMWORD PTR [r11], xmm6
			movapd XMMWORD PTR [r11 + r8], xmm7

			imul r10, r8, 2
			movapd XMMWORD PTR [r11 + r10], xmm8

			add r10, r8
			movapd XMMWORD PTR [r11 + r10], xmm9

			add r10, r8
			movapd XMMWORD PTR [r11 + r10], xmm10

			add r10, r8
			movapd XMMWORD PTR [r11 + r10], xmm11

			add r10, r8
			movapd XMMWORD PTR [r11 + r10], xmm12

			add r10, r8
			movapd XMMWORD PTR [r11 + r10], xmm13

			add r10, r8
			movapd XMMWORD PTR [r11 + r10], xmm14

			add r10, r8
			movapd XMMWORD PTR [r11 + r10], xmm15
			jmp fork_fiber_context_preservation_switch_case_end

		fork_fiber_context_preservation_switch_case32::
			vmovapd YMMWORD PTR [r11], ymm6
			vmovapd YMMWORD PTR [r11 + r8], ymm7

			imul r10, r8, 2
			vmovapd YMMWORD PTR [r11 + r10], ymm8

			add r10, r8
			vmovapd YMMWORD PTR [r11 + r10], ymm9

			add r10, r8
			vmovapd YMMWORD PTR [r11 + r10], ymm10

			add r10, r8
			vmovapd YMMWORD PTR [r11 + r10], ymm11

			add r10, r8
			vmovapd YMMWORD PTR [r11 + r10], ymm12

			add r10, r8
			vmovapd YMMWORD PTR [r11 + r10], ymm13

			add r10, r8
			vmovapd YMMWORD PTR [r11 + r10], ymm14

			add r10, r8
			vmovapd YMMWORD PTR [r11 + r10], ymm15
			jmp fork_fiber_context_preservation_switch_case_end

		fork_fiber_context_preservation_switch_case64::
			vmovapd ZMMWORD PTR [r11], zmm6
			vmovapd ZMMWORD PTR [r11 + r8], zmm7

			imul r10, r8, 2
			vmovapd ZMMWORD PTR [r11 + r10], zmm8

			add r10, r8
			vmovapd ZMMWORD PTR [r11 + r10], zmm9

			add r10, r8
			vmovapd ZMMWORD PTR [r11 + r10], zmm10

			add r10, r8
			vmovapd ZMMWORD PTR [r11 + r10], zmm11

			add r10, r8
			vmovapd ZMMWORD PTR [r11 + r10], zmm12

			add r10, r8
			vmovapd ZMMWORD PTR [r11 + r10], zmm13

			add r10, r8
			vmovapd ZMMWORD PTR [r11 + r10], zmm14

			add r10, r8
			vmovapd ZMMWORD PTR [r11 + r10], zmm15
			jmp fork_fiber_context_preservation_switch_case_end




		fork_fiber_context_restoration_switch_case16::
			movapd xmm6, XMMWORD PTR [r11]
			movapd xmm7, XMMWORD PTR [r11 + r8]

			imul r10, r8, 2
			movapd xmm8, XMMWORD PTR [r11 + r10]

			add r10, r8
			movapd xmm9, XMMWORD PTR [r11 + r10]

			add r10, r8
			movapd xmm10, XMMWORD PTR [r11 + r10]

			add r10, r8
			movapd xmm11, XMMWORD PTR [r11 + r10]

			add r10, r8
			movapd xmm12, XMMWORD PTR [r11 + r10]

			add r10, r8
			movapd xmm13, XMMWORD PTR [r11 + r10]

			add r10, r8
			movapd xmm14, XMMWORD PTR [r11 + r10]

			add r10, r8
			movapd xmm15, XMMWORD PTR [r11 + r10]
			jmp fork_fiber_context_restoration_switch_case_end

		fork_fiber_context_restoration_switch_case32::
			vmovapd ymm6, YMMWORD PTR [r11]
			vmovapd ymm7, YMMWORD PTR [r11 + r8]

			imul r10, r8, 2
			vmovapd ymm8, YMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd ymm9, YMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd ymm10, YMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd ymm11, YMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd ymm12, YMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd ymm13, YMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd ymm14, YMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd ymm15, YMMWORD PTR [r11 + r10]
			jmp fork_fiber_context_restoration_switch_case_end

		fork_fiber_context_restoration_switch_case64::
			vmovapd zmm6, ZMMWORD PTR [r11]
			vmovapd zmm7, ZMMWORD PTR [r11 + r8]

			imul r10, r8, 2
			vmovapd zmm8, ZMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd zmm9, ZMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd zmm10, ZMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd zmm11, ZMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd zmm12, ZMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd zmm13, ZMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd zmm14, ZMMWORD PTR [r11 + r10]

			add r10, r8
			vmovapd zmm15, ZMMWORD PTR [r11 + r10]
			jmp fork_fiber_context_restoration_switch_case_end
		__fork_fiber ENDP

	.const
		fork_fiber_context_preservation_switch_case_table QWORD	OFFSET fork_fiber_context_preservation_switch_case16,
																OFFSET fork_fiber_context_preservation_switch_case32,
																OFFSET fork_fiber_context_preservation_switch_case64

		fork_fiber_context_restoration_switch_case_table QWORD	OFFSET fork_fiber_context_restoration_switch_case16,
																OFFSET fork_fiber_context_restoration_switch_case32,
																OFFSET fork_fiber_context_restoration_switch_case64








.code
	PUBLIC __join_fiber
		__join_fiber PROC
			; RCX = thread_p
			; RDX = simd_vector_size_p
			mov r8, rdx ; for the imul below.
			mov rax, r8 ; Move simd_vector_size_p to rax
			shr rax, 5 ; Divide simd_vector_size_p by 32 (size of YMM register) to get the case index; result in rax (the case index)
			
			lea r10, join_fiber_context_restoration_switch_case_table ; Load the address of the case table into r10
			mov r11, QWORD PTR [rcx] ; r11 is a pointer to the fiber_context field variable of the fiber* from_p
			jmp QWORD PTR [r10 + rax*8] ; Equivalent as byte array[]; array[rax*8];
			join_fiber_context_restoration_switch_case_end:

			; Restore the non-volatile general-purpose registers
			imul r9, r8, 10
			mov r12, QWORD PTR [r11 + r9]

			add r9, 8
			mov r13, QWORD PTR [r11 + r9]

			add r9, 8
			mov r14, QWORD PTR [r11 + r9]

			add r9, 8
			mov r15, QWORD PTR [r11 + r9]


			add r9, 8
			ldmxcsr DWORD PTR [r11 + r9]

			add r9, 4
			fldcw	WORD PTR [r11 + r9]


			add r9, 4
			mov rbx, QWORD PTR [r11 + r9]

			add r9, 8
			mov rbp, QWORD PTR [r11 + r9]

			add r9, 8
			mov rdi, QWORD PTR [r11 + r9]

			add r9, 8
			mov rsi, QWORD PTR [r11 + r9]

			add r9, 8
			mov rsp, QWORD PTR [r11 + r9]
			

			; Restore the GS segment register values (TEB)
			mov r9, [rcx + 8] ; _absolute_begin_of_stack
			mov gs:[1478h], r9

			mov r9, [rcx + 16] ; _stack_base
			mov gs:[08h], r9

			mov r9, [rcx + 24] ; _stack_limit
			mov gs:[10h], r9

			ret




		join_fiber_context_restoration_switch_case16::
			movapd xmm6, XMMWORD PTR [r11]
			movapd xmm7, XMMWORD PTR [r11 + r8]

			imul r9, r8, 2
			movapd xmm8, XMMWORD PTR [r11 + r9]

			add r9, r8
			movapd xmm9, XMMWORD PTR [r11 + r9]

			add r9, r8
			movapd xmm10, XMMWORD PTR [r11 + r9]

			add r9, r8
			movapd xmm11, XMMWORD PTR [r11 + r9]

			add r9, r8
			movapd xmm12, XMMWORD PTR [r11 + r9]

			add r9, r8
			movapd xmm13, XMMWORD PTR [r11 + r9]

			add r9, r8
			movapd xmm14, XMMWORD PTR [r11 + r9]

			add r9, r8
			movapd xmm15, XMMWORD PTR [r11 + r9]
			jmp join_fiber_context_restoration_switch_case_end

		join_fiber_context_restoration_switch_case32::
			vmovapd ymm6, YMMWORD PTR [r11]
			vmovapd ymm7, YMMWORD PTR [r11 + r8]

			imul r9, r8, 2
			vmovapd ymm8, YMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd ymm9, YMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd ymm10, YMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd ymm11, YMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd ymm12, YMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd ymm13, YMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd ymm14, YMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd ymm15, YMMWORD PTR [r11 + r9]
			jmp join_fiber_context_restoration_switch_case_end

		join_fiber_context_restoration_switch_case64::
			vmovapd zmm6, ZMMWORD PTR [r11]
			vmovapd zmm7, ZMMWORD PTR [r11 + r8]

			imul r9, r8, 2
			vmovapd zmm8, ZMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd zmm9, ZMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd zmm10, ZMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd zmm11, ZMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd zmm12, ZMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd zmm13, ZMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd zmm14, ZMMWORD PTR [r11 + r9]

			add r9, r8
			vmovapd zmm15, ZMMWORD PTR [r11 + r9]
			jmp join_fiber_context_restoration_switch_case_end
		__join_fiber ENDP

	.const
		join_fiber_context_restoration_switch_case_table QWORD	OFFSET join_fiber_context_restoration_switch_case16,
																OFFSET join_fiber_context_restoration_switch_case32,
																OFFSET join_fiber_context_restoration_switch_case64
END