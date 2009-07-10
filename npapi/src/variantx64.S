/*
 * Copyright 2008, 2009 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// cf. http://www.codesourcery.com/cxx-abi/abi.html#mangling

#ifdef __x86_64__

#define VSIZE   $16     // sizeof(Any)
#define VTYPE   8       // offsetof(Any, type)

#define VBOOL   $1
#define VU8     $2
#define VS16    $3
#define VU16    $4
#define VS32    $5
#define VU32    $6
#define VS64    $7
#define VU64    $8
#define VFLT    $9
#define VDBL    $10
#define VSTRING $11
#define VOBJECT $12

#define VVARIANT    $0x80000000

        .text

// Any apply(argc: rdi, argv: rsi, function: rdx);
// INTEGER: %edi, %rsi, %rdx, %rcx, %r8, %r9
// SSE: %xmm0 to %xmm7
// Return value (Any): %rax, %rdx
// temporary registers: %rax, %r10, %r11, %xmm8-%xmm15
// callee-saved registers: %rbx, %rbp, %r12-%r15
// %r10: gpr count
// %r11: fpr count
// %r12: argc
// %r13: type

// Any apply(int argc, Any* argv, Any (*function)());
        .globl  _Z5applyiP3AnyPFS_vE
_Z5applyiP3AnyPFS_vE:

apply:
        pushq   %r12
        pushq   %r13
        pushq   %r14
        pushq   %rbp
        movq    %rsp, %rbp

        pushq   %rdx            // Save function (%rsp must be aligned on a 16 byte boundary)
        testl   %edi, %edi
        jle     invoke

        xorl    %r10d, %r10d
        xorl    %r11d, %r11d
        movl    %edi, %r12d
        movq    %rsi, %rax

while:
        // Check parameter class
        movl    VTYPE(%rax), %r13d
        testl   %r13d, %r13d
        js      variant         // Test VVARIANT flag
        cmpl    VFLT, %r13d
        jz      fpr32
        cmpl    VDBL, %r13d
        jz      fpr64
gpr:
        incl    %r10d
        cmpl    $6, %r10d
        jg      next            // Push the argument on the stack later
        cmpl    VBOOL, %r13d
        jz      bv
        movq    (%rax), %r13
        jmp     *0f(,%r10,8)
bv:     movzbq  (%rax), %r13
        jmp     *0f(,%r10,8)
0:
        .quad   0
        .quad   1f
        .quad   2f
        .quad   3f
        .quad   4f
        .quad   5f
        .quad   6f
1:      movq    %r13, %rdi
        jmp     next
2:      movq    %r13, %rsi
        jmp     next
3:      movq    %r13, %rdx
        jmp     next
4:      movq    %r13, %rcx
        jmp     next
5:      movq    %r13, %r8
        jmp	next
6:      movq    %r13, %r9
        jmp     next

variant:
        addl    $2, %r10d
        cmpl    $6, %r10d
        jg      next            // Push the argument on the stack later
        andl    $0x7fffffff, %r13d
        jmp     *0f(,%r10,8)
0:
        .quad   0
        .quad   0
        .quad   1f
        .quad   2f
        .quad   3f
        .quad   4f
        .quad   5f
1:      movq    (%rax), %rdi
        movl    %r13d, %esi
        jmp     next
2:      movq    (%rax), %rsi
        movl    %r13d, %edx
        jmp     next
3:      movq    (%rax), %rdx
        movl    %r13d, %ecx
        jmp     next
4:      movq    (%rax), %rcx
        movl    %r13d, %r8d
        jmp     next
5:      movq    (%rax), %r8
        movl    %r13d, %r9d
        jmp     next

fpr32:
        incl    %r11d
        cmpl    $8, %r11d
        jg      next            // Push the argument on the stack later
        jmp     *0f(,%r11,8)
0:
        .quad   0
        .quad   1f
        .quad   2f
        .quad   3f
        .quad   4f
        .quad   5f
        .quad   6f
        .quad   7f
        .quad   8f
1:      movss   (%rax), %xmm0
        jmp     next
2:      movss   (%rax), %xmm1
        jmp     next
3:      movss   (%rax), %xmm2
        jmp     next
4:      movss   (%rax), %xmm3
        jmp     next
5:      movss   (%rax), %xmm4
        jmp     next
6:      movss   (%rax), %xmm5
        jmp     next
7:      movss   (%rax), %xmm6
        jmp     next
8:      movss   (%rax), %xmm7
        jmp     next

fpr64:
        incl    %r11d
        cmpl    $8, %r11d
        jg      next            // Push the argument on the stack later
        jmp     *0f(,%r11,8)
0:
        .quad   0
        .quad   1f
        .quad   2f
        .quad   3f
        .quad   4f
        .quad   5f
        .quad   6f
        .quad   7f
        .quad   8f
1:      movsd   (%rax), %xmm0
        jmp     next
2:      movsd   (%rax), %xmm1
        jmp     next
3:      movsd   (%rax), %xmm2
        jmp     next
4:      movsd   (%rax), %xmm3
        jmp     next
5:      movsd   (%rax), %xmm4
        jmp     next
6:      movsd   (%rax), %xmm5
        jmp     next
7:      movsd   (%rax), %xmm6
        jmp     next
8:      movsd   (%rax), %xmm7
        jmp     next

next:
        decl    %r12d
        jz      break
        addq    VSIZE, %rax
        jmp     while
break:

        // Push the argument on the stack
        // Adjust rsp alignment
        xorl    %r12d, %r12d
        subl    $6, %r10d
        jle     0f
        movl    %r10d, %r12d
0:      subl    $8, %r11d
        jle     1f
        addl    %r11d, %r12d
1:      andl    $1, %r12d
        jz      2f
        leaq    -8(%rsp), %rsp
2:

push_argments:
        testl   %r10d, %r10d
        jg      0f
        testl   %r11d, %r11d
        jg      0f
        jmp     invoke
0:
        // Check parameter class
        movl    VTYPE(%rax), %r13d
        testl   %r13d, %r13d
        js      2f                      // Test VVARIANT flag
        cmpl    VFLT, %r13d
        jz      1f
        cmpl    VDBL, %r13d
        jz      1f
        // GPR
        testl   %r10d, %r10d
        jle     skip
        pushq   (%rax)
        decl    %r10d
        jmp     skip
1:      // FPR
        testl   %r11d, %r11d
        jle     skip
        pushq   (%rax)
        decl    %r11d
        jmp     skip
2:      // variant
        testl   %r10d, %r10d
        jle     skip
        andl    $0x7fffffff, %r13d
        // At this point, %r10d must be greater than one
        pushq   %r13
        pushq   (%rax)
        subl    $2, %r10d

skip:
        subq    VSIZE, %rax
        jmp     push_argments

invoke:
        movq    -8(%rbp), %rax
        call    *%rax
        leave
        popq    %r14
        popq    %r13
        popq    %r12
        ret
        // End of apply

// Any apply(int argc, Any* argv, bool (*function)());
        .globl _Z5applyiP3AnyPFbvE
_Z5applyiP3AnyPFbvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movl    VBOOL, %edx
        leave
        ret

// Any apply(int argc, Any* argv, uint8_t (*function)());
        .globl _Z5applyiP3AnyPFhvE
_Z5applyiP3AnyPFhvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movl    VU8, %edx
        leave
        ret

// Any apply(int argc, Any* argv, int16_t (*function)());
        .globl _Z5applyiP3AnyPFsvE
_Z5applyiP3AnyPFsvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movl    VS16, %edx
        leave
        ret

// Any apply(int argc, Any* argv, uint16_t (*function)());
        .globl _Z5applyiP3AnyPFtvE
_Z5applyiP3AnyPFtvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movl    VU16, %edx
        leave
        ret

// Any apply(int argc, Any* argv, int32_t (*function)());
        .globl _Z5applyiP3AnyPFivE
_Z5applyiP3AnyPFivE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movl    VS32, %edx
        leave
        ret

// Any apply(int argc, Any* argv, uint32_t (*function)());
        .globl _Z5applyiP3AnyPFjvE
_Z5applyiP3AnyPFjvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movl    VU32, %edx
        leave
        ret

// Any apply(int argc, Any* argv, int64_t (*function)());
        .globl _Z5applyiP3AnyPFxvE
_Z5applyiP3AnyPFxvE:
        .globl _Z5applyiP3AnyPFlvE
_Z5applyiP3AnyPFlvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movl    VS64, %edx
        leave
        ret

// Any apply(int argc, Any* argv, uint64_t (*function)());
        .globl _Z5applyiP3AnyPFyvE
_Z5applyiP3AnyPFyvE:
        .globl _Z5applyiP3AnyPFmvE
_Z5applyiP3AnyPFmvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movl    VU64, %edx
        leave
        ret

// Any apply(int argc, Any* argv, float (*function)());
        .globl _Z5applyiP3AnyPFfvE
_Z5applyiP3AnyPFfvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movss   %xmm0, -4(%rsp)
        movl    -4(%rsp), %eax
        movl    VFLT, %edx
        leave
        ret

// Any apply(int argc, Any* argv, double (*function)());
        .globl _Z5applyiP3AnyPFdvE
_Z5applyiP3AnyPFdvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movsd   %xmm0, -8(%rsp)
        movq    -8(%rsp), %rax
        movl    VDBL, %edx
        leave
        ret

// Any apply(int argc, Any* argv, const char* (*function)());
        .globl _Z5applyiP3AnyPFPKcvE
_Z5applyiP3AnyPFPKcvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movl    VSTRING, %edx
        leave
        ret

// Any apply(int argc, Any* argv, Object* (*function)());
        .globl _Z5applyiP3AnyPFP6ObjectvE
_Z5applyiP3AnyPFP6ObjectvE:
        pushq   %rbp
        movq    %rsp, %rbp
        call    apply
        movl    VOBJECT, %edx
        leave
        ret

// long long evaluate(const Any& variant);
_Z8evaluateRK3Any:
        .globl _Z8evaluateRK3Any
        pushq   %rbp
        movq    %rsp, %rbp
        movl    VTYPE(%rdi), %eax
        cmpl    VOBJECT, %eax
        jge     invalid_type
        jmp     *0f(,%eax,8)
0:
        .quad   0
        .quad   1f      // bool
        .quad   1f      // u8
        .quad   2f      // s16
        .quad   2f      // u16
        .quad   3f      // s32
        .quad   3f      // u32
        .quad   4f      // s64
        .quad   4f      // u64
        .quad   5f      // flt
        .quad   6f      // dbl
        .quad   4f      // string
        .quad   4f      // object
1:      // return bool, uint8_t
        movzbq  (%rdi), %rax
        leave
        ret
2:      // return int16_t, uint16_t
        movzwq  (%rdi), %rax
        leave
        ret
3:      // return int32_t, uint32_t, *
        movl    (%rdi), %eax
        leave
        ret
4:      // return int64_t, uint64_t
        movq    (%rdi), %rax
        leave
        ret
5:      // return float
        movss   (%rdi), %xmm0
        leave
        ret
6:      // return double
        movsd   (%rdi), %xmm0
        leave
        ret
invalid_type:
        xorl    %eax, %eax
        leave
        ret

#endif  // __x86_64__
