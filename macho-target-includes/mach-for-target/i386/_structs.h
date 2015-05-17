/*
 * Copyright (c) 2004-2006 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 * 
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * @OSF_COPYRIGHT@
 */

#ifndef _MACH_FOR_TGT_I386__STRUCTS_H_
#define _MACH_FOR_TGT_I386__STRUCTS_H_

typedef unsigned char           uint8_t;

#define	_STRUCT_X86_TGT_THREAD_STATE32	struct __darwin_i386_tgt_thread_state
_STRUCT_X86_TGT_THREAD_STATE32
{
    unsigned int	__eax;
    unsigned int	__ebx;
    unsigned int	__ecx;
    unsigned int	__edx;
    unsigned int	__edi;
    unsigned int	__esi;
    unsigned int	__ebp;
    unsigned int	__esp;
    unsigned int	__ss;
    unsigned int	__eflags;
    unsigned int	__eip;
    unsigned int	__cs;
    unsigned int	__ds;
    unsigned int	__es;
    unsigned int	__fs;
    unsigned int	__gs;
};

/* This structure should be double-word aligned for performance */

#define _STRUCT_TGT_FP_CONTROL	struct __darwin_tgt_fp_control
_STRUCT_TGT_FP_CONTROL
{
    unsigned short		__invalid	:1,
    				__denorm	:1,
				__zdiv		:1,
				__ovrfl		:1,
				__undfl		:1,
				__precis	:1,
						:2,
				__pc		:2,
#define FP_PREC_24B		0
#define	FP_PREC_53B		2
#define FP_PREC_64B		3
				__rc		:2,
#define FP_RND_NEAR		0
#define FP_RND_DOWN		1
#define FP_RND_UP		2
#define FP_CHOP			3
					/*inf*/	:1,
						:3;
};
typedef _STRUCT_TGT_FP_CONTROL	__darwin_tgt_fp_control_t;

/*
 * Status word.
 */

#define _STRUCT_TGT_FP_STATUS	struct __darwin_tgt_fp_status
_STRUCT_TGT_FP_STATUS
{
    unsigned short		__invalid	:1,
    				__denorm	:1,
				__zdiv		:1,
				__ovrfl		:1,
				__undfl		:1,
				__precis	:1,
				__stkflt	:1,
				__errsumm	:1,
				__c0		:1,
				__c1		:1,
				__c2		:1,
				__tos		:3,
				__c3		:1,
				__busy		:1;
};
typedef _STRUCT_TGT_FP_STATUS	__darwin_tgt_fp_status_t;
				
/* defn of 80bit x87 FPU or MMX register  */

#define _STRUCT_TGT_MMST_REG	struct __darwin_tgt_mmst_reg
_STRUCT_TGT_MMST_REG
{
	char	__mmst_reg[10];
	char	__mmst_rsrv[6];
};
typedef _STRUCT_TGT_MMST_REG tgt_mmst_reg_t;

/* defn of 128 bit XMM regs */

#define _STRUCT_TGT_XMM_REG		struct __darwin_tgt_xmm_reg
_STRUCT_TGT_XMM_REG
{
	char		__xmm_reg[16];
};
typedef _STRUCT_TGT_XMM_REG tgt_xmm_reg_t;

/* defn of 256 bit YMM regs */

#define _STRUCT_TGT_YMM_REG		struct __darwin_tgt_ymm_reg
_STRUCT_TGT_YMM_REG
{
	char		__ymm_reg[32];
};
typedef _STRUCT_TGT_YMM_REG tgt_ymm_reg_t;

/* defn of 512 bit ZMM regs */

#define _STRUCT_TGT_ZMM_REG		struct __darwin_tgt_zmm_reg
_STRUCT_TGT_ZMM_REG
{
	char		__zmm_reg[64];
};
typedef _STRUCT_TGT_ZMM_REG tgt_zmm_reg_t;

#define _STRUCT_TGT_OPMASK_REG	struct __darwin_tgt_opmask_reg
_STRUCT_TGT_OPMASK_REG
{
	char		__opmask_reg[8];
};

/* 
 * Floating point state.
 */

#ifndef FP_STATE_BYTES
#define FP_STATE_BYTES		512	/* number of chars worth of data from fpu_fcw */
#endif

#define	_STRUCT_X86_TGT_FLOAT_STATE32	struct __darwin_i386_tgt_float_state
_STRUCT_X86_TGT_FLOAT_STATE32
{
	int 			__fpu_reserved[2];
	_STRUCT_TGT_FP_CONTROL	__fpu_fcw;		/* x87 FPU control word */
	_STRUCT_TGT_FP_STATUS	__fpu_fsw;		/* x87 FPU status word */
	uint8_t		__fpu_ftw;		/* x87 FPU tag word */
	uint8_t		__fpu_rsrv1;		/* reserved */ 
	uint16_t		__fpu_fop;		/* x87 FPU Opcode */
	uint32_t		__fpu_ip;		/* x87 FPU Instruction Pointer offset */
	uint16_t		__fpu_cs;		/* x87 FPU Instruction Pointer Selector */
	uint16_t		__fpu_rsrv2;		/* reserved */
	uint32_t		__fpu_dp;		/* x87 FPU Instruction Operand(Data) Pointer offset */
	uint16_t		__fpu_ds;		/* x87 FPU Instruction Operand(Data) Pointer Selector */
	uint16_t		__fpu_rsrv3;		/* reserved */
	uint32_t		__fpu_mxcsr;		/* MXCSR Register state */
	uint32_t		__fpu_mxcsrmask;	/* MXCSR mask */
	_STRUCT_TGT_MMST_REG	__fpu_stmm0;		/* ST0/MM0   */
	_STRUCT_TGT_MMST_REG	__fpu_stmm1;		/* ST1/MM1  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm2;		/* ST2/MM2  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm3;		/* ST3/MM3  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm4;		/* ST4/MM4  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm5;		/* ST5/MM5  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm6;		/* ST6/MM6  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm7;		/* ST7/MM7  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm0;		/* XMM 0  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm1;		/* XMM 1  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm2;		/* XMM 2  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm3;		/* XMM 3  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm4;		/* XMM 4  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm5;		/* XMM 5  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm6;		/* XMM 6  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm7;		/* XMM 7  */
	char			__fpu_rsrv4[14*16];	/* reserved */
	int 			__fpu_reserved1;
};

#define	_STRUCT_X86_TGT_AVX_STATE32	struct __darwin_i386_tgt_avx_state
_STRUCT_X86_TGT_AVX_STATE32
{
	int 			__fpu_reserved[2];
	_STRUCT_TGT_FP_CONTROL	__fpu_fcw;		/* x87 FPU control word */
	_STRUCT_TGT_FP_STATUS	__fpu_fsw;		/* x87 FPU status word */
	uint8_t		__fpu_ftw;		/* x87 FPU tag word */
	uint8_t		__fpu_rsrv1;		/* reserved */ 
	uint16_t		__fpu_fop;		/* x87 FPU Opcode */
	uint32_t		__fpu_ip;		/* x87 FPU Instruction Pointer offset */
	uint16_t		__fpu_cs;		/* x87 FPU Instruction Pointer Selector */
	uint16_t		__fpu_rsrv2;		/* reserved */
	uint32_t		__fpu_dp;		/* x87 FPU Instruction Operand(Data) Pointer offset */
	uint16_t		__fpu_ds;		/* x87 FPU Instruction Operand(Data) Pointer Selector */
	uint16_t		__fpu_rsrv3;		/* reserved */
	uint32_t		__fpu_mxcsr;		/* MXCSR Register state */
	uint32_t		__fpu_mxcsrmask;	/* MXCSR mask */
	_STRUCT_TGT_MMST_REG	__fpu_stmm0;		/* ST0/MM0   */
	_STRUCT_TGT_MMST_REG	__fpu_stmm1;		/* ST1/MM1  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm2;		/* ST2/MM2  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm3;		/* ST3/MM3  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm4;		/* ST4/MM4  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm5;		/* ST5/MM5  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm6;		/* ST6/MM6  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm7;		/* ST7/MM7  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm0;		/* XMM 0  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm1;		/* XMM 1  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm2;		/* XMM 2  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm3;		/* XMM 3  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm4;		/* XMM 4  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm5;		/* XMM 5  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm6;		/* XMM 6  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm7;		/* XMM 7  */
	char			__fpu_rsrv4[14*16];	/* reserved */
	int 			__fpu_reserved1;
	char			__avx_reserved1[64];
	_STRUCT_TGT_XMM_REG		__fpu_ymmh0;		/* YMMH 0  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh1;		/* YMMH 1  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh2;		/* YMMH 2  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh3;		/* YMMH 3  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh4;		/* YMMH 4  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh5;		/* YMMH 5  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh6;		/* YMMH 6  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh7;		/* YMMH 7  */
};

#define	_STRUCT_X86_TGT_AVX512_STATE32	struct __darwin_i386_tgt_avx512_state
_STRUCT_X86_TGT_AVX512_STATE32
{
	int 			__fpu_reserved[2];
	_STRUCT_TGT_FP_CONTROL	__fpu_fcw;		/* x87 FPU control word */
	_STRUCT_TGT_FP_STATUS	__fpu_fsw;		/* x87 FPU status word */
	uint8_t		__fpu_ftw;		/* x87 FPU tag word */
	uint8_t		__fpu_rsrv1;		/* reserved */ 
	uint16_t		__fpu_fop;		/* x87 FPU Opcode */
	uint32_t		__fpu_ip;		/* x87 FPU Instruction Pointer offset */
	uint16_t		__fpu_cs;		/* x87 FPU Instruction Pointer Selector */
	uint16_t		__fpu_rsrv2;		/* reserved */
	uint32_t		__fpu_dp;		/* x87 FPU Instruction Operand(Data) Pointer offset */
	uint16_t		__fpu_ds;		/* x87 FPU Instruction Operand(Data) Pointer Selector */
	uint16_t		__fpu_rsrv3;		/* reserved */
	uint32_t		__fpu_mxcsr;		/* MXCSR Register state */
	uint32_t		__fpu_mxcsrmask;	/* MXCSR mask */
	_STRUCT_TGT_MMST_REG	__fpu_stmm0;		/* ST0/MM0   */
	_STRUCT_TGT_MMST_REG	__fpu_stmm1;		/* ST1/MM1  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm2;		/* ST2/MM2  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm3;		/* ST3/MM3  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm4;		/* ST4/MM4  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm5;		/* ST5/MM5  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm6;		/* ST6/MM6  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm7;		/* ST7/MM7  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm0;		/* XMM 0  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm1;		/* XMM 1  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm2;		/* XMM 2  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm3;		/* XMM 3  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm4;		/* XMM 4  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm5;		/* XMM 5  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm6;		/* XMM 6  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm7;		/* XMM 7  */
	char			__fpu_rsrv4[14*16];	/* reserved */
	int 			__fpu_reserved1;
	char			__avx_reserved1[64];
	_STRUCT_TGT_XMM_REG		__fpu_ymmh0;		/* YMMH 0  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh1;		/* YMMH 1  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh2;		/* YMMH 2  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh3;		/* YMMH 3  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh4;		/* YMMH 4  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh5;		/* YMMH 5  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh6;		/* YMMH 6  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh7;		/* YMMH 7  */
	_STRUCT_TGT_OPMASK_REG	__fpu_k0;		/* K0 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k1;		/* K1 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k2;		/* K2 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k3;		/* K3 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k4;		/* K4 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k5;		/* K5 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k6;		/* K6 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k7;		/* K7 */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh0;		/* ZMMH 0  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh1;		/* ZMMH 1  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh2;		/* ZMMH 2  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh3;		/* ZMMH 3  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh4;		/* ZMMH 4  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh5;		/* ZMMH 5  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh6;		/* ZMMH 6  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh7;		/* ZMMH 7  */
};

#define _STRUCT_X86_TGT_EXCEPTION_STATE32	struct __darwin_i386_tgt_exception_state
_STRUCT_X86_TGT_EXCEPTION_STATE32
{
	uint16_t	__trapno;
	uint16_t	__cpu;
	uint32_t	__err;
	uint32_t	__faultvaddr;
};

#define _STRUCT_X86_TGT_DEBUG_STATE32	struct __darwin_x86_tgt_debug_state32
_STRUCT_X86_TGT_DEBUG_STATE32
{
	unsigned int	__dr0;
	unsigned int	__dr1;
	unsigned int	__dr2;
	unsigned int	__dr3;
	unsigned int	__dr4;
	unsigned int	__dr5;
	unsigned int	__dr6;
	unsigned int	__dr7;
};

#define _STRUCT_X86_TGT_INSTRUCTION_STATE	struct __x86_tgt_instruction_state
_STRUCT_X86_TGT_INSTRUCTION_STATE
{
        int		__insn_stream_valid_bytes;
        int		__insn_offset;
	int		__out_of_synch;	/*
					 * non-zero when the cacheline that includes the insn_offset
					 * is replaced in the insn_bytes array due to a mismatch
					 * detected when comparing it with the same cacheline in memory
					 */
#define _X86_TGT_INSTRUCTION_STATE_MAX_INSN_BYTES    (2448 - 64 - 4)
        uint8_t	__insn_bytes[_X86_TGT_INSTRUCTION_STATE_MAX_INSN_BYTES];
#define _X86_TGT_INSTRUCTION_STATE_CACHELINE_SIZE	64
	uint8_t	__insn_cacheline[_X86_TGT_INSTRUCTION_STATE_CACHELINE_SIZE];
};

#define _STRUCT_TGT_LAST_BRANCH_RECORD	struct __tgt_last_branch_record
_STRUCT_TGT_LAST_BRANCH_RECORD
{
	uint64_t	__from_ip;
	uint64_t	__to_ip;
	uint32_t	__mispredict : 1,
			__tsx_abort  : 1,
			__in_tsx     : 1,
			__cycle_count: 16,
			__reserved   : 13;
};

#define _STRUCT_TGT_LAST_BRANCH_STATE	struct __tgt_last_branch_state
_STRUCT_TGT_LAST_BRANCH_STATE
{
        int				__lbr_count;
	uint32_t			__lbr_supported_tsx : 1,
					__lbr_supported_cycle_count : 1,
					__reserved : 30;
#define	__TGT_LASTBRANCH_MAX	32
	_STRUCT_TGT_LAST_BRANCH_RECORD	__lbrs[__TGT_LASTBRANCH_MAX];
};

#define	_STRUCT_X86_TGT_PAGEIN_STATE	struct __x86_tgt_pagein_state
_STRUCT_X86_TGT_PAGEIN_STATE
{
	int __pagein_error;
};

/*
 * 64 bit versions of the above
 */

#define	_STRUCT_X86_TGT_THREAD_STATE64	struct __darwin_x86_tgt_thread_state64
_STRUCT_X86_TGT_THREAD_STATE64
{
	uint64_t	__rax;
	uint64_t	__rbx;
	uint64_t	__rcx;
	uint64_t	__rdx;
	uint64_t	__rdi;
	uint64_t	__rsi;
	uint64_t	__rbp;
	uint64_t	__rsp;
	uint64_t	__r8;
	uint64_t	__r9;
	uint64_t	__r10;
	uint64_t	__r11;
	uint64_t	__r12;
	uint64_t	__r13;
	uint64_t	__r14;
	uint64_t	__r15;
	uint64_t	__rip;
	uint64_t	__rflags;
	uint64_t	__cs;
	uint64_t	__fs;
	uint64_t	__gs;
};

/*
 * 64 bit versions of the above (complete)
 */

#define	_STRUCT_X86_TGT_THREAD_FULL_STATE64	struct __darwin_x86_tgt_thread_full_state64
_STRUCT_X86_TGT_THREAD_FULL_STATE64
{
	_STRUCT_X86_TGT_THREAD_STATE64	__ss64;
	uint64_t			__ds;
	uint64_t			__es;
	uint64_t			__ss;
	uint64_t			__gsbase;
};

#define	_STRUCT_X86_TGT_FLOAT_STATE64	struct __darwin_x86_tgt_float_state64
_STRUCT_X86_TGT_FLOAT_STATE64
{
	int 			__fpu_reserved[2];
	_STRUCT_TGT_FP_CONTROL	__fpu_fcw;		/* x87 FPU control word */
	_STRUCT_TGT_FP_STATUS	__fpu_fsw;		/* x87 FPU status word */
	uint8_t		__fpu_ftw;		/* x87 FPU tag word */
	uint8_t		__fpu_rsrv1;		/* reserved */ 
	uint16_t		__fpu_fop;		/* x87 FPU Opcode */

	/* x87 FPU Instruction Pointer */
	uint32_t		__fpu_ip;		/* offset */
	uint16_t		__fpu_cs;		/* Selector */

	uint16_t		__fpu_rsrv2;		/* reserved */

	/* x87 FPU Instruction Operand(Data) Pointer */
	uint32_t		__fpu_dp;		/* offset */
	uint16_t		__fpu_ds;		/* Selector */

	uint16_t		__fpu_rsrv3;		/* reserved */
	uint32_t		__fpu_mxcsr;		/* MXCSR Register state */
	uint32_t		__fpu_mxcsrmask;	/* MXCSR mask */
	_STRUCT_TGT_MMST_REG	__fpu_stmm0;		/* ST0/MM0   */
	_STRUCT_TGT_MMST_REG	__fpu_stmm1;		/* ST1/MM1  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm2;		/* ST2/MM2  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm3;		/* ST3/MM3  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm4;		/* ST4/MM4  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm5;		/* ST5/MM5  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm6;		/* ST6/MM6  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm7;		/* ST7/MM7  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm0;		/* XMM 0  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm1;		/* XMM 1  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm2;		/* XMM 2  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm3;		/* XMM 3  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm4;		/* XMM 4  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm5;		/* XMM 5  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm6;		/* XMM 6  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm7;		/* XMM 7  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm8;		/* XMM 8  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm9;		/* XMM 9  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm10;		/* XMM 10  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm11;		/* XMM 11 */
	_STRUCT_TGT_XMM_REG		__fpu_xmm12;		/* XMM 12  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm13;		/* XMM 13  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm14;		/* XMM 14  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm15;		/* XMM 15  */
	char			__fpu_rsrv4[6*16];	/* reserved */
	int 			__fpu_reserved1;
};

#define	_STRUCT_X86_TGT_AVX_STATE64	struct __darwin_x86_tgt_avx_state64
_STRUCT_X86_TGT_AVX_STATE64
{
	int 			__fpu_reserved[2];
	_STRUCT_TGT_FP_CONTROL	__fpu_fcw;		/* x87 FPU control word */
	_STRUCT_TGT_FP_STATUS	__fpu_fsw;		/* x87 FPU status word */
	uint8_t		__fpu_ftw;		/* x87 FPU tag word */
	uint8_t		__fpu_rsrv1;		/* reserved */ 
	uint16_t		__fpu_fop;		/* x87 FPU Opcode */

	/* x87 FPU Instruction Pointer */
	uint32_t		__fpu_ip;		/* offset */
	uint16_t		__fpu_cs;		/* Selector */

	uint16_t		__fpu_rsrv2;		/* reserved */

	/* x87 FPU Instruction Operand(Data) Pointer */
	uint32_t		__fpu_dp;		/* offset */
	uint16_t		__fpu_ds;		/* Selector */

	uint16_t		__fpu_rsrv3;		/* reserved */
	uint32_t		__fpu_mxcsr;		/* MXCSR Register state */
	uint32_t		__fpu_mxcsrmask;	/* MXCSR mask */
	_STRUCT_TGT_MMST_REG	__fpu_stmm0;		/* ST0/MM0   */
	_STRUCT_TGT_MMST_REG	__fpu_stmm1;		/* ST1/MM1  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm2;		/* ST2/MM2  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm3;		/* ST3/MM3  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm4;		/* ST4/MM4  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm5;		/* ST5/MM5  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm6;		/* ST6/MM6  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm7;		/* ST7/MM7  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm0;		/* XMM 0  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm1;		/* XMM 1  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm2;		/* XMM 2  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm3;		/* XMM 3  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm4;		/* XMM 4  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm5;		/* XMM 5  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm6;		/* XMM 6  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm7;		/* XMM 7  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm8;		/* XMM 8  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm9;		/* XMM 9  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm10;		/* XMM 10  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm11;		/* XMM 11 */
	_STRUCT_TGT_XMM_REG		__fpu_xmm12;		/* XMM 12  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm13;		/* XMM 13  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm14;		/* XMM 14  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm15;		/* XMM 15  */
	char			__fpu_rsrv4[6*16];	/* reserved */
	int 			__fpu_reserved1;
	char			__avx_reserved1[64];
	_STRUCT_TGT_XMM_REG		__fpu_ymmh0;		/* YMMH 0  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh1;		/* YMMH 1  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh2;		/* YMMH 2  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh3;		/* YMMH 3  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh4;		/* YMMH 4  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh5;		/* YMMH 5  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh6;		/* YMMH 6  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh7;		/* YMMH 7  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh8;		/* YMMH 8  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh9;		/* YMMH 9  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh10;		/* YMMH 10  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh11;		/* YMMH 11  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh12;		/* YMMH 12  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh13;		/* YMMH 13  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh14;		/* YMMH 14  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh15;		/* YMMH 15  */
};

#define	_STRUCT_X86_TGT_AVX512_STATE64	struct __darwin_x86_tgt_avx512_state64
_STRUCT_X86_TGT_AVX512_STATE64
{
	int 			__fpu_reserved[2];
	_STRUCT_TGT_FP_CONTROL	__fpu_fcw;		/* x87 FPU control word */
	_STRUCT_TGT_FP_STATUS	__fpu_fsw;		/* x87 FPU status word */
	uint8_t		__fpu_ftw;		/* x87 FPU tag word */
	uint8_t		__fpu_rsrv1;		/* reserved */ 
	uint16_t		__fpu_fop;		/* x87 FPU Opcode */

	/* x87 FPU Instruction Pointer */
	uint32_t		__fpu_ip;		/* offset */
	uint16_t		__fpu_cs;		/* Selector */

	uint16_t		__fpu_rsrv2;		/* reserved */

	/* x87 FPU Instruction Operand(Data) Pointer */
	uint32_t		__fpu_dp;		/* offset */
	uint16_t		__fpu_ds;		/* Selector */

	uint16_t		__fpu_rsrv3;		/* reserved */
	uint32_t		__fpu_mxcsr;		/* MXCSR Register state */
	uint32_t		__fpu_mxcsrmask;	/* MXCSR mask */
	_STRUCT_TGT_MMST_REG	__fpu_stmm0;		/* ST0/MM0   */
	_STRUCT_TGT_MMST_REG	__fpu_stmm1;		/* ST1/MM1  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm2;		/* ST2/MM2  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm3;		/* ST3/MM3  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm4;		/* ST4/MM4  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm5;		/* ST5/MM5  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm6;		/* ST6/MM6  */
	_STRUCT_TGT_MMST_REG	__fpu_stmm7;		/* ST7/MM7  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm0;		/* XMM 0  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm1;		/* XMM 1  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm2;		/* XMM 2  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm3;		/* XMM 3  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm4;		/* XMM 4  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm5;		/* XMM 5  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm6;		/* XMM 6  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm7;		/* XMM 7  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm8;		/* XMM 8  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm9;		/* XMM 9  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm10;		/* XMM 10  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm11;		/* XMM 11 */
	_STRUCT_TGT_XMM_REG		__fpu_xmm12;		/* XMM 12  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm13;		/* XMM 13  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm14;		/* XMM 14  */
	_STRUCT_TGT_XMM_REG		__fpu_xmm15;		/* XMM 15  */
	char			__fpu_rsrv4[6*16];	/* reserved */
	int 			__fpu_reserved1;
	char			__avx_reserved1[64];
	_STRUCT_TGT_XMM_REG		__fpu_ymmh0;		/* YMMH 0  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh1;		/* YMMH 1  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh2;		/* YMMH 2  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh3;		/* YMMH 3  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh4;		/* YMMH 4  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh5;		/* YMMH 5  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh6;		/* YMMH 6  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh7;		/* YMMH 7  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh8;		/* YMMH 8  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh9;		/* YMMH 9  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh10;		/* YMMH 10  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh11;		/* YMMH 11  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh12;		/* YMMH 12  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh13;		/* YMMH 13  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh14;		/* YMMH 14  */
	_STRUCT_TGT_XMM_REG		__fpu_ymmh15;		/* YMMH 15  */
	_STRUCT_TGT_OPMASK_REG	__fpu_k0;		/* K0 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k1;		/* K1 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k2;		/* K2 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k3;		/* K3 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k4;		/* K4 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k5;		/* K5 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k6;		/* K6 */
	_STRUCT_TGT_OPMASK_REG	__fpu_k7;		/* K7 */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh0;		/* ZMMH 0  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh1;		/* ZMMH 1  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh2;		/* ZMMH 2  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh3;		/* ZMMH 3  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh4;		/* ZMMH 4  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh5;		/* ZMMH 5  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh6;		/* ZMMH 6  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh7;		/* ZMMH 7  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh8;		/* ZMMH 8  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh9;		/* ZMMH 9  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh10;		/* ZMMH 10  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh11;		/* ZMMH 11  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh12;		/* ZMMH 12  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh13;		/* ZMMH 13  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh14;		/* ZMMH 14  */
	_STRUCT_TGT_YMM_REG		__fpu_zmmh15;		/* ZMMH 15  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm16;		/* ZMM 16  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm17;		/* ZMM 17  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm18;		/* ZMM 18  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm19;		/* ZMM 19  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm20;		/* ZMM 20  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm21;		/* ZMM 21  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm22;		/* ZMM 22  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm23;		/* ZMM 23  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm24;		/* ZMM 24  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm25;		/* ZMM 25  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm26;		/* ZMM 26  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm27;		/* ZMM 27  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm28;		/* ZMM 28  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm29;		/* ZMM 29  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm30;		/* ZMM 30  */
	_STRUCT_TGT_ZMM_REG		__fpu_zmm31;		/* ZMM 31  */
};

#define _STRUCT_X86_TGT_EXCEPTION_STATE64	struct __darwin_x86_tgt_exception_state64
_STRUCT_X86_TGT_EXCEPTION_STATE64
{
    uint16_t	__trapno;
    uint16_t	__cpu;
    uint32_t	__err;
    uint64_t	__faultvaddr;
};

#define _STRUCT_X86_TGT_DEBUG_STATE64	struct __darwin_x86_tgt_debug_state64
_STRUCT_X86_TGT_DEBUG_STATE64
{
	uint64_t	__dr0;
	uint64_t	__dr1;
	uint64_t	__dr2;
	uint64_t	__dr3;
	uint64_t	__dr4;
	uint64_t	__dr5;
	uint64_t	__dr6;
	uint64_t	__dr7;
};

#define _STRUCT_X86_TGT_CPMU_STATE64	struct __darwin_x86_tgt_cpmu_state64
_STRUCT_X86_TGT_CPMU_STATE64
{
	uint64_t __ctrs[16];
};

#endif /* _MACH_FOR_TGT_I386__STRUCTS_H_ */
