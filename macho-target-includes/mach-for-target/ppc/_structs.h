/*
 * Copyright (c) 2004 Apple Computer, Inc. All rights reserved.
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

#ifndef	_MACH_TGT_PPC__STRUCTS_H_
#define	_MACH_TGT_PPC__STRUCTS_H_

/*
 * ppc_thread_state is the structure that is exported to user threads for 
 * use in status/mutate calls.  This structure should never change.
 *
 */

#define	_STRUCT_PPC_TGT_THREAD_STATE	struct __darwin_ppc_tgt_thread_state
_STRUCT_PPC_TGT_THREAD_STATE
{
  uint32_t __srr0;	/* Instruction address register (PC) */
  uint32_t __srr1;	/* Machine state register (supervisor) */
  uint32_t __r0;
  uint32_t __r1;
  uint32_t __r2;
  uint32_t __r3;
  uint32_t __r4;
  uint32_t __r5;
  uint32_t __r6;
  uint32_t __r7;
  uint32_t __r8;
  uint32_t __r9;
  uint32_t __r10;
  uint32_t __r11;
  uint32_t __r12;
  uint32_t __r13;
  uint32_t __r14;
  uint32_t __r15;
  uint32_t __r16;
  uint32_t __r17;
  uint32_t __r18;
  uint32_t __r19;
  uint32_t __r20;
  uint32_t __r21;
  uint32_t __r22;
  uint32_t __r23;
  uint32_t __r24;
  uint32_t __r25;
  uint32_t __r26;
  uint32_t __r27;
  uint32_t __r28;
  uint32_t __r29;
  uint32_t __r30;
  uint32_t __r31;

  uint32_t __cr;	/* Condition register */
  uint32_t __xer;	/* User's integer exception register */
  uint32_t __lr;	/* Link register */
  uint32_t __ctr;	/* Count register */
  uint32_t __mq;	/* MQ register (601 only) */

  uint32_t __vrsave;	/* Vector Save Register */
};

#pragma pack(4)			/* Make sure the structure stays as we defined it */

#define _STRUCT_PPC_TGT_THREAD_STATE64	struct __darwin_ppc_tgt_thread_state64
_STRUCT_PPC_TGT_THREAD_STATE64
{
  uint64_t __srr0;	/* Instruction address register (PC) */
  uint64_t __srr1;	/* Machine state register (supervisor) */
  uint64_t __r0;
  uint64_t __r1;
  uint64_t __r2;
  uint64_t __r3;
  uint64_t __r4;
  uint64_t __r5;
  uint64_t __r6;
  uint64_t __r7;
  uint64_t __r8;
  uint64_t __r9;
  uint64_t __r10;
  uint64_t __r11;
  uint64_t __r12;
  uint64_t __r13;
  uint64_t __r14;
  uint64_t __r15;
  uint64_t __r16;
  uint64_t __r17;
  uint64_t __r18;
  uint64_t __r19;
  uint64_t __r20;
  uint64_t __r21;
  uint64_t __r22;
  uint64_t __r23;
  uint64_t __r24;
  uint64_t __r25;
  uint64_t __r26;
  uint64_t __r27;
  uint64_t __r28;
  uint64_t __r29;
  uint64_t __r30;
  uint64_t __r31;

  uint32_t __cr;		/* Condition register */
  uint64_t __xer;	/* User's integer exception register */
  uint64_t __lr;	/* Link register */
  uint64_t __ctr;	/* Count register */

  uint32_t __vrsave;		/* Vector Save Register */
};

#pragma pack()

/* This structure should be double-word aligned for performance */

#define	_STRUCT_PPC_TGT_FLOAT_STATE	struct __darwin_ppc_tgt_float_state
_STRUCT_PPC_TGT_FLOAT_STATE
{
  double  __fpregs[32];

  uint32_t __fpscr_pad; /* fpscr is 64 bits, 32 bits of rubbish */
  uint32_t __fpscr;	/* floating point status register */
};

#pragma pack(4)		/* Make sure the structure stays as we defined it */

#define _STRUCT_PPC_TGT_VECTOR_STATE	struct __darwin_ppc_tgt_vector_state
_STRUCT_PPC_TGT_VECTOR_STATE
{
  uint32_t	__save_vr[32][4];
  uint32_t	__save_vscr[4];
  uint32_t	__save_pad5[4];
  uint32_t	__save_vrvalid;			/* VRs that have been saved */
  uint32_t	__save_pad6[7];
};

#pragma pack()

/*
 * ppc_exception_state
 *
 * This structure corresponds to some additional state of the user
 * registers as saved in the PCB upon kernel entry. They are only
 * available if an exception is passed out of the kernel, and even
 * then not all are guaranteed to be updated.
 *
 * Some padding is included in this structure which allows space for
 * servers to store temporary values if need be, to maintain binary
 * compatiblity.
 */

/* Exception state for 32-bit thread (on 32-bit processor) */
/* Still available on 64-bit processors, but may fall short */
/* of covering the full potential state (hi half available). */

#pragma pack(4)	/* Make sure the structure stays as we defined it */

#define _STRUCT_PPC_TGT_EXCEPTION_STATE	struct __darwin_ppc_tgt_exception_state
_STRUCT_PPC_TGT_EXCEPTION_STATE
{
  uint32_t __dar;		/* Fault registers for coredump */
  uint32_t __dsisr;
  uint32_t __exception;	/* number of powerpc exception taken */
  uint32_t __pad0;		/* align to 16 bytes */
  uint32_t __pad1[4];		/* space in PCB "just in case" */
};

#define _STRUCT_PPC_TGT_EXCEPTION_STATE64	struct __darwin_ppc_tgt_exception_state64
_STRUCT_PPC_TGT_EXCEPTION_STATE64
{
  uint64_t __dar;	/* Fault registers for coredump */
  uint32_t  __dsisr;
  uint32_t  __exception;	/* number of powerpc exception taken */
  uint32_t  __pad1[4];	/* space in PCB "just in case" */
};

#pragma pack()

#endif /* _MACH_PPC__STRUCTS_H_ */
